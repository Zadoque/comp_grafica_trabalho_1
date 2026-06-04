#include "../../includes/curvas/bspline.h"
// #include <math.h>
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
void subdivide(ponto P0, ponto P1, ponto P2, ponto P3, Pontos *curva_resultado, float t0, float t1, float tolerance) {
    ponto A = calcular_ponto_bspline(P0,P1,P2,P3, t0);
    ponto B = calcular_ponto_bspline(P0,P1,P2,P3, t1);
    ponto M = calcular_ponto_bspline(P0,P1,P2,P3, (t0+t1)/2);
    
    float dist = calcula_dist_ponto_segmento(M, A, B);
    
    if (dist < tolerance) {
       pontos_push(curva_resultado, M);
    } else {
        subdivide(P0, P1, P2, P3, curva_resultado, t0, (t0+t1)/2, tolerance);
        subdivide(P0, P1, P2, P3, curva_resultado, (t0+t1)/2, t1, tolerance);
    }
}

void gerar_curva_bspline(ponto P0, ponto P1, ponto P2, ponto P3, Pontos *curva_resultado, vec *vetor_parametro) {
  curva_resultado->quantidade_atual = 0;
  // B-Spline: cada grupo de 4 pontos consecutivos gera um segmento
  // Gerar pontos do segmento
  ponto A = calcular_ponto_bspline(P0, P1, P2, P3,0.0f);
  pontos_push(curva_resultado, A);
  subdivide(P0, P1, P2, P3, curva_resultado, 0.0f, 1.0f, 0.1f);
  ponto B = calcular_ponto_bspline(P0,P1,P2,P3, 1.0f);
  pontos_push(curva_resultado, B);
  //for (int i = 0; i <= vetor_parametro->quantidade_atual; i++) {
 //   ponto p = calcular_ponto_bspline(P0, P1, P2, P3, vetor_parametro->vector[i]);
   // pontos_push(curva_resultado, p);
  // }
  //printf("Curva B-Spline gerada com %zu pontos\n",
         //curva_resultado->quantidade_atual);
}
