#include "../../includes/curvas/catmullrom.h"
// #include <math.h>
//#include <stdio.h>

// Matriz Catmull-Rom (corrigida e dividida por 2)
static const float MATRIZ_CATMULLROM[4][4] = {
    {-1.0f / 2.0f, 3.0f / 2.0f, -3.0f / 2.0f, 1.0f / 2.0f},
    {2.0f / 2.0f, -5.0f / 2.0f, 4.0f / 2.0f, -1.0f / 2.0f},
    {-1.0f / 2.0f, 0.0f / 2.0f, 1.0f / 2.0f, 0.0f / 2.0f},
    {0.0f / 2.0f, 2.0f / 2.0f, 0.0f / 2.0f, 0.0f / 2.0f}};

ponto calcular_ponto_catmullrom(ponto P0, ponto P1, ponto P2, ponto P3,
                                float t) {
  // Vetor de parâmetros [t³, t², t, 1]
  float t2 = t * t;
  float t3 = t2 * t;
  float vetor_t[4] = {t3, t2, t, 1.0f};

  // Multiplicar: vetor_t × MATRIZ_CATMULLROM
  float coeficientes[4] = {0.0f, 0.0f, 0.0f, 0.0f};
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      coeficientes[i] += vetor_t[j] * MATRIZ_CATMULLROM[j][i];
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
void subdivide_catmullrom(ponto P0, ponto P1, ponto P2, ponto P3, Pontos *curva_resultado, float t0, float t1, float tolerance) {
    ponto A = calcular_ponto_catmullrom(P0,P1,P2,P3, t0);
    ponto B = calcular_ponto_catmullrom(P0,P1,P2,P3, t1);
    ponto M = calcular_ponto_catmullrom(P0,P1,P2,P3, (t0+t1)/2);
    
    float dist = calcula_dist_ponto_segmento(M, A, B);
    
    if (dist < tolerance) {
       pontos_push(curva_resultado, M);
    } else {
        subdivide_catmullrom(P0, P1, P2, P3, curva_resultado, t0, (t0+t1)/2, tolerance);
        subdivide_catmullrom(P0, P1, P2, P3, curva_resultado, (t0+t1)/2, t1, tolerance);
    }
}


void gerar_curva_catmullrom(ponto P0, ponto P1, ponto P2, ponto P3, Pontos *curva_resultado) {

    curva_resultado->quantidade_atual = 0;
  ponto A = calcular_ponto_catmullrom(P0, P1, P2, P3,0.0f);
  pontos_push(curva_resultado, A);
  subdivide_catmullrom(P0, P1, P2, P3, curva_resultado, 0.0f, 1.0f, 0.1f);
  ponto B = calcular_ponto_catmullrom(P0,P1,P2,P3, 1.0f);
  pontos_push(curva_resultado, B);

           
}
