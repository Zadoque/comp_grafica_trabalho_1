#include "../../includes/curvas/bspline.h"
// #include <math.h>
//#include <stdio.h>

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

void gerar_curva_bspline(Pontos *pontos_controle, Pontos *curva_resultado, int poligono) {
  if (pontos_controle->quantidade_atual < 4)
    return;
  curva_resultado->quantidade_atual = 0;

  // B-Spline: cada grupo de 4 pontos consecutivos gera um segmento
  for (int i = 0; i <= pontos_controle->quantidade_atual - 4; i++) {
    ponto P0 = pontos_controle->data[i];
    ponto P1 = pontos_controle->data[i + 1];
    ponto P2 = pontos_controle->data[i + 2];
    ponto P3 = pontos_controle->data[i + 3];
    int resolucao = (int)(calcula_distancia(P1, P2)) * 5;
    // Gerar pontos do segmento
    for (int j = 0; j <= resolucao; j++) {
      float t = (float)(j + 0.00001) / (float)resolucao;
      ponto p = calcular_ponto_bspline(P0, P1, P2, P3, t);
      pontos_push(curva_resultado, p);
    }
  }
  if (poligono) {
    for (int i = 0; i < 3; i++) {
      ponto P0 =
          pontos_controle->data[(pontos_controle->quantidade_atual - 3 + i) %
                                pontos_controle->quantidade_atual];
      ponto P1 =
          pontos_controle->data[(pontos_controle->quantidade_atual - 2 + i) %
                                pontos_controle->quantidade_atual];
      ponto P2 =
          pontos_controle->data[(pontos_controle->quantidade_atual - 1 + i) %
                                pontos_controle->quantidade_atual];
      ponto P3 = pontos_controle->data[(pontos_controle->quantidade_atual + i) %
                                       pontos_controle->quantidade_atual];
      int resolucao = (int)(calcula_distancia(P1, P2)) * 5;
      // Gerar pontos do segmento
      for (int j = 0; j <= resolucao; j++) {
        float t = (float)(j + 0.00001) / (float)resolucao;
        ponto p = calcular_ponto_bspline(P0, P1, P2, P3, t);
        pontos_push(curva_resultado, p);
      }
    }
  }

  //printf("Curva B-Spline gerada com %zu pontos\n",
         //curva_resultado->quantidade_atual);
}
