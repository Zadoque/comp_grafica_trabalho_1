#include "../includes/transformacoes.h"
#include <math.h>

/* --- função auxiliar para liberar matriz 3x3 --- */
static void liberar_matriz(float **m) {
  if (!m) return;
  for (int i = 0; i < 3; i++) free(m[i]);
  free(m);
}

void calcular_centro_medio(ponto *centro, Pontos *pontos) {
  centro->point[0] = 0;
  centro->point[1] = 0;
  for (int i = 0; i < (int)(pontos->quantidade_atual); i++) {
    centro->point[0] += pontos->data[i].point[0];
    centro->point[1] += pontos->data[i].point[1];
  }
  centro->point[0] /= (float)pontos->quantidade_atual;
  centro->point[1] /= (float)pontos->quantidade_atual;
}

void translacao_com_mouse(Pontos *pontos, ponto centro, int x_mouse,
                          int y_mouse) {
  for (int i = 0; i < pontos->quantidade_atual; i++) {
    pontos->data[i].point[0] =
        pontos->data[i].point[0] - (float)centro.point[0] + (float)x_mouse;
    pontos->data[i].point[1] =
        pontos->data[i].point[1] - (float)centro.point[1] + (float)y_mouse;
  }
}

void rotacionar_com_mouse(Pontos *pontos, ponto *centro, ponto mouse,
                          int indice) {
  ponto vetor1, vetor2;
  vetor1.point[2] = 1;
  vetor2.point[2] = 1;

  vetor1.point[0] = pontos->data[indice].point[0] - centro->point[0];
  vetor1.point[1] = pontos->data[indice].point[1] - centro->point[1];
  vetor2.point[0] = mouse.point[0] - centro->point[0];
  vetor2.point[1] = mouse.point[1] - centro->point[1];

  float angulo1 = atan2f(vetor1.point[1], vetor1.point[0]);
  float angulo2 = atan2f(vetor2.point[1], vetor2.point[0]);
  float angulo_rad = angulo2 - angulo1;

  float **rotacao = CriarMatrizRotacao(angulo_rad);
  float **translacao1 = CriarMatrizTranslacao(-centro->point[0], -centro->point[1]);
  float **translacao2 = CriarMatrizTranslacao(centro->point[0], centro->point[1]);

  /* M = T1 * R * T2   (vetor-linha: ponto * M) */
  float **M = MultiplicaMatriz(translacao1, rotacao);
  float **temp = MultiplicaMatriz(M, translacao2);
  liberar_matriz(M);
  M = temp;

  for (int i = 0; i < pontos->quantidade_atual; i++) {
    float *novo = MultiplicaPonto(pontos->data[i].point, M);
    pontos->data[i].point[0] = novo[0];
    pontos->data[i].point[1] = novo[1];
    free(novo);
  }

  liberar_matriz(M);
  liberar_matriz(rotacao);
  liberar_matriz(translacao1);
  liberar_matriz(translacao2);
}

/* ================================================================
   ESCALA corrigida — versão limpa com proteção contra div/0
   ================================================================ */
void escala_com_mouse(Pontos *pontos, ponto *centro, ponto mouse, int indice) {
  ponto p;
  p.point[0] = pontos->data[indice].point[0];
  p.point[1] = pontos->data[indice].point[1];

  float d_original = calcula_distancia(p, *centro);
  if (d_original < 1e-6f) return;          /* evita divisão por zero */

  float d_mouse = calcula_distancia(mouse, *centro);
  float escala = d_mouse / d_original;

  float **T1 = CriarMatrizTranslacao(-centro->point[0], -centro->point[1]);
  float **S  = CriarMatrizEscala(escala, escala);
  float **T2 = CriarMatrizTranslacao(centro->point[0], centro->point[1]);

  /* M = T1 * S * T2 */
  float **M = MultiplicaMatriz(T1, S);
  float **temp = MultiplicaMatriz(M, T2);
  liberar_matriz(M);
  M = temp;

  for (int i = 0; i < pontos->quantidade_atual; i++) {
    float *novo = MultiplicaPonto(pontos->data[i].point, M);
    pontos->data[i].point[0] = novo[0];
    pontos->data[i].point[1] = novo[1];
    free(novo);
  }

  liberar_matriz(M);
  liberar_matriz(T1);
  liberar_matriz(S);
  liberar_matriz(T2);
}

/* ================================================================
   SHEAR (cisalhamento) em X e Y com mouse
   ================================================================ */
void shear_com_mouse(Pontos *pontos, ponto *centro, ponto mouse, int indice) {
  /* vetores do centro até o ponto de controle e até o mouse */
  float vx_ponto = pontos->data[indice].point[0] - centro->point[0];
  float vy_ponto = pontos->data[indice].point[1] - centro->point[1];
  float vx_mouse = mouse.point[0] - centro->point[0];
  float vy_mouse = mouse.point[1] - centro->point[1];

  /* fatores de shear: deslocamento relativo / coordenada perpendicular */
  float shx = 0.0f, shy = 0.0f;
  if (fabsf(vy_ponto) > 1e-6f)
    shx = (vx_mouse - vx_ponto) / vy_ponto;   /* shear horizontal */
  if (fabsf(vx_ponto) > 1e-6f)
    shy = (vy_mouse - vy_ponto) / vx_ponto;   /* shear vertical   */

  float **T1 = CriarMatrizTranslacao(-centro->point[0], -centro->point[1]);
  float **ShX = CriarMatrizCisalhamentoX(shx);
  float **ShY = CriarMatrizCisalhamentoY(shy);
  float **T2 = CriarMatrizTranslacao(centro->point[0], centro->point[1]);

  /* M = T1 * ShX * ShY * T2  (ordem: shearX depois shearY) */
  float **M = MultiplicaMatriz(T1, ShX);
  float **temp = MultiplicaMatriz(M, ShY);
  liberar_matriz(M);  M = temp;
  temp = MultiplicaMatriz(M, T2);
  liberar_matriz(M);  M = temp;

  for (int i = 0; i < pontos->quantidade_atual; i++) {
    float *novo = MultiplicaPonto(pontos->data[i].point, M);
    pontos->data[i].point[0] = novo[0];
    pontos->data[i].point[1] = novo[1];
    free(novo);
  }

  liberar_matriz(M);
  liberar_matriz(T1);
  liberar_matriz(ShX);
  liberar_matriz(ShY);
  liberar_matriz(T2);
}

/* --- versões separadas caso queira shear em um eixo só --- */
void shearX_com_mouse(Pontos *pontos, ponto *centro, ponto mouse, int indice) {
  float vx_ponto = pontos->data[indice].point[0] - centro->point[0];
  float vy_ponto = pontos->data[indice].point[1] - centro->point[1];
  float vx_mouse = mouse.point[0] - centro->point[0];

  float shx = 0.0f;
  if (fabsf(vy_ponto) > 1e-6f)
    shx = (vx_mouse - vx_ponto) / vy_ponto;

  float **T1 = CriarMatrizTranslacao(-centro->point[0], -centro->point[1]);
  float **ShX = CriarMatrizCisalhamentoX(shx);
  float **T2 = CriarMatrizTranslacao(centro->point[0], centro->point[1]);

  float **M = MultiplicaMatriz(T1, ShX);
  float **temp = MultiplicaMatriz(M, T2);
  liberar_matriz(M);  M = temp;

  for (int i = 0; i < pontos->quantidade_atual; i++) {
    float *novo = MultiplicaPonto(pontos->data[i].point, M);
    pontos->data[i].point[0] = novo[0];
    pontos->data[i].point[1] = novo[1];
    free(novo);
  }

  liberar_matriz(M);
  liberar_matriz(T1);
  liberar_matriz(ShX);
  liberar_matriz(T2);
}

void shearY_com_mouse(Pontos *pontos, ponto *centro, ponto mouse, int indice) {
  float vx_ponto = pontos->data[indice].point[0] - centro->point[0];
  float vy_ponto = pontos->data[indice].point[1] - centro->point[1];
  float vy_mouse = mouse.point[1] - centro->point[1];

  float shy = 0.0f;
  if (fabsf(vx_ponto) > 1e-6f)
    shy = (vy_mouse - vy_ponto) / vx_ponto;

  float **T1 = CriarMatrizTranslacao(-centro->point[0], -centro->point[1]);
  float **ShY = CriarMatrizCisalhamentoY(shy);
  float **T2 = CriarMatrizTranslacao(centro->point[0], centro->point[1]);

  float **M = MultiplicaMatriz(T1, ShY);
  float **temp = MultiplicaMatriz(M, T2);
  liberar_matriz(M);  M = temp;

  for (int i = 0; i < pontos->quantidade_atual; i++) {
    float *novo = MultiplicaPonto(pontos->data[i].point, M);
    pontos->data[i].point[0] = novo[0];
    pontos->data[i].point[1] = novo[1];
    free(novo);
  }

  liberar_matriz(M);
  liberar_matriz(T1);
  liberar_matriz(ShY);
  liberar_matriz(T2);
}

void aumentar_escala(Pontos *pontos, ponto centro) {
  for (int i = 0; i < pontos->quantidade_atual; i++) {
    pontos->data[i].point[0] +=
        (pontos->data[i].point[0] - centro.point[0]) * 0.05f;
    pontos->data[i].point[1] +=
        (pontos->data[i].point[1] - centro.point[1]) * 0.05f;
  }
}

void diminuir_escala(Pontos *pontos, ponto centro) {
  for (int i = 0; i < pontos->quantidade_atual; i++) {
    pontos->data[i].point[0] -=
        (pontos->data[i].point[0] - centro.point[0]) * 0.05f;
    pontos->data[i].point[1] -=
        (pontos->data[i].point[1] - centro.point[1]) * 0.05f;
  }
}
