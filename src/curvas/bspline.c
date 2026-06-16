#include "../../includes/curvas/bspline.h"
#include <math.h>
#include <stdlib.h>
#ifndef FLT_MAX
#define FLT_MAX  99999999.0f
#endif
//#include <stdio.h>
float *t;
int qtd;
// Matriz B-Spline (corrigida e dividida por 6)
static const float MATRIZ_BSPLINE[4][4] = {
    {-1.0f / 6.0f, 3.0f / 6.0f, -3.0f / 6.0f, 1.0f / 6.0f},
    {3.0f / 6.0f, -6.0f / 6.0f, 3.0f / 6.0f, 0.0f / 6.0f},
    {-3.0f / 6.0f, 0.0f / 6.0f, 3.0f / 6.0f, 0.0f / 6.0f},
    {1.0f / 6.0f, 4.0f / 6.0f, 1.0f / 6.0f, 0.0f / 6.0f}};

ponto calcular_ponto_bspline(ponto P0, ponto P1, ponto P2, ponto P3, float t) {
  // Vetor de parâmetros [t³, t², t, 1]
  float t2 = t * t;
  float t3 = t2 * t;
  float vetor_t[4] = {t3, t2, t, 1.0f};

  // Multiplicar: vetor_t × MATRIZ_BSPLINE
  float coeficientes[4] = {0.0f, 0.0f, 0.0f, 0.0f};
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      coeficientes[i] += vetor_t[j] * MATRIZ_BSPLINE[j][i];
    }
  }

  // Aplicar aos pontos de controle
  ponto resultado;
  resultado.point[0] = coeficientes[0] * P0.point[0] + coeficientes[1] * P1.point[0] +
                coeficientes[2] * P2.point[0] + coeficientes[3] * P3.point[0];
  resultado.point[1] = coeficientes[0] * P0.point[1] + coeficientes[1] * P1.point[1] +
                coeficientes[2] * P2.point[1] + coeficientes[3] * P3.point[1];

  return resultado;
}
void subdivide(ponto P0, ponto P1, ponto P2, ponto P3, Pontos *curva_resultado, ponto A, ponto B, float t0, float t1, float tolerance, AABB *box) {
  float tmid = (t0 + t1) / 2.0f;
  ponto M = calcular_ponto_bspline(P0,P1,P2,P3, tmid);
  float dist = calcula_dist_ponto_segmento(M, A, B);
  if (dist < tolerance) {
    pontos_push(curva_resultado, M);
    box->x_min = fminf(box->x_min, M.point[0]);
    box->y_min = fminf(box->y_min, M.point[1]);
    box->x_max = fmaxf(box->x_max, M.point[0]);
    box->y_max = fmaxf(box->y_max, M.point[1]);
  } else {
    // Esquerda: A permanece, B vira M
    subdivide(P0,P1,P2,P3, curva_resultado, A, M, t0, tmid, tolerance, box);
    // Direita: A vira M, B permanece
    subdivide(P0,P1,P2,P3, curva_resultado, M, B, tmid, t1, tolerance, box);
  }
}

void gerar_curva_bspline(ponto P0, ponto P1, ponto P2, ponto P3, Pontos *curva_resultado, AABB *box) {
  curva_resultado->quantidade_atual = 0;
  ponto A = calcular_ponto_bspline(P0,P1,P2,P3, 0.0f);
  ponto B = calcular_ponto_bspline(P0,P1,P2,P3, 1.0f);

  box->x_min = fminf(A.point[0], B.point[0]);
  box->y_min = fminf(A.point[1], B.point[1]);
  box->x_max = fmaxf(A.point[0], B.point[0]);
  box->y_max = fmaxf(A.point[1], B.point[1]);

  pontos_push(curva_resultado, A);
  subdivide(P0,P1,P2,P3, curva_resultado, A, B, 0.0f, 1.0f, 0.1f, box);
  pontos_push(curva_resultado, B);
}



static ResultadoPicking subdivide_picking(
        ponto P0, ponto P1, ponto P2, ponto P3,
        ponto mouse,
        ponto A, ponto B,
        float t0, float t1,
        float raio, float tolerancia_t) {

    ResultadoPicking sem_resultado = { -1, 0.0f, FLT_MAX };

    // Poda: mouse longe do segmento AB
    float dist = calcula_dist_ponto_segmento(mouse, A, B);
    if (dist > raio) return sem_resultado;

    // Convergiu: intervalo pequeno o suficiente
    if ((t1 - t0) < tolerancia_t) {
      float tmid = (t0 + t1) / 2.0f;
      ponto C = calcular_ponto_bspline(P0,P1,P2,P3, tmid);
    
      // Distância real ao ponto da curva, não ao segmento AB
      float dx = mouse.point[0] - C.point[0];
      float dy = mouse.point[1] - C.point[1];
      float dist_real = sqrtf(dx*dx + dy*dy);
    
      ResultadoPicking r;
      r.segmento_indice = 0;
      r.t = tmid;
      r.distancia = dist_real;  // ← distância real, não aproximada
      return r;
    
    }

    float tmid = (t0 + t1) / 2.0f;
    ponto M = calcular_ponto_bspline(P0,P1,P2,P3, tmid);

    ResultadoPicking esq = subdivide_picking(P0,P1,P2,P3, mouse, A, M,
                                              t0, tmid, raio, tolerancia_t);
    ResultadoPicking dir = subdivide_picking(P0,P1,P2,P3, mouse, M, B,
                                              tmid, t1, raio, tolerancia_t);

    // Retorna o mais próximo
    if (esq.segmento_indice == -1) return dir;
    if (dir.segmento_indice == -1) return esq;
    return esq.distancia < dir.distancia ? esq : dir;
}

ResultadoPicking picking_bspline(
        AABBTREE* arvore,
        Pontos* clicks,
        ponto mouse,
        float raio) {

    ResultadoPicking sem_resultado = { -1, 0.0f, FLT_MAX };
    if (arvore == NULL) return sem_resultado;

    // Poda pela árvore
    if (!ponto_dentro_aabb(arvore->box, mouse.point[0], mouse.point[1]))
        return sem_resultado;

    // Folha: testa o segmento real
    if (arvore->esquerda == NULL && arvore->direita == NULL) {
        int i = arvore->box.segmento_indice;
        ponto P0 = clicks->data[i];
        ponto P1 = clicks->data[i+1];
        ponto P2 = clicks->data[i+2];
        ponto P3 = clicks->data[i+3];
        ponto A = calcular_ponto_bspline(P0,P1,P2,P3, 0.0f);
        ponto B = calcular_ponto_bspline(P0,P1,P2,P3, 1.0f);

        ResultadoPicking r = subdivide_picking(P0,P1,P2,P3, mouse,
                                               A, B, 0.0f, 1.0f,
                                               raio, 0.001f);
        r.segmento_indice = (r.distancia < FLT_MAX) ? i : -1;
        return r;
    }

    // Nó interno: desce nos dois filhos
    ResultadoPicking esq = picking_bspline(arvore->esquerda, clicks, mouse, raio);
    ResultadoPicking dir = picking_bspline(arvore->direita,  clicks, mouse, raio);

    if (esq.segmento_indice == -1) return dir;
    if (dir.segmento_indice == -1) return esq;
    return esq.distancia < dir.distancia ? esq : dir;
}
