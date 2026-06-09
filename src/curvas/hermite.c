#include "../../includes/curvas/hermite.h"

// #include <math.h>
// #include <stdio.h>

// Matriz de Hermite (corrigida)
static const float MATRIZ_HERMITE[4][4] = {
    {2.0f, -2.0f, 1.0f, 1.0f},   // 2t³ - 2t² + t + 1
    {-3.0f, 3.0f, -2.0f, -1.0f}, // -3t³ + 3t² - 2t - 1
    {0.0f, 0.0f, 1.0f, 0.0f},    // t
    {1.0f, 0.0f, 0.0f, 0.0f}     // 1
};

ponto calcular_ponto_hermite(ponto P1, ponto P2, ponto T1, ponto T2, float t) {
  // Vetor de parâmetros [t³, t², t, 1]
  float t2 = t * t;
  float t3 = t2 * t;
  float vetor_t[4] = {t3, t2, t, 1.0f};

  // Multiplicar: vetor_t × MATRIZ_HERMITE
  float coeficientes[4] = {0.0f, 0.0f, 0.0f, 0.0f};
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      coeficientes[i] += vetor_t[j] * MATRIZ_HERMITE[j][i];
    }
  }

  // Aplicar aos pontos de controle: P(t) = c₀P₁ + c₁P₂ + c₂T₁ + c₃T₂
  ponto resultado;
  resultado.point[0] = coeficientes[0] * P1.point[0] + coeficientes[1] * P2.point[0] +
                coeficientes[2] * T1.point[0] + coeficientes[3] * T2.point[0];
  resultado.point[1] = coeficientes[0] * P1.point[1] + coeficientes[1] * P2.point[1] +
                coeficientes[2] * T1.point[1] + coeficientes[3] * T2.point[1];

  return resultado;
}
void calcular_tangentes_hermite(ponto P1, ponto P2, ponto P3, ponto P4,
                                ponto *T1, ponto *T2) {
  T1->point[0] = P2.point[0] - P1.point[1];
  T1->point[1] = P2.point[1] - P1.point[1];
  T2->point[0] = P4.point[0] - P3.point[0];
  T2->point[1] = P4.point[1] - P3.point[1];
}

void subdivide_Hermite(ponto P0, ponto P1, ponto P2, ponto P3, Pontos *curva_resultado, float t0, float t1, float tolerance) {
    ponto A = calcular_ponto_hermite(P0,P1,P2,P3, t0);
    ponto B = calcular_ponto_hermite(P0,P1,P2,P3, t1);
    ponto M = calcular_ponto_hermite(P0,P1,P2,P3, (t0+t1)/2);
    
    float dist = calcula_dist_ponto_segmento(M, A, B);
    
    if (dist < tolerance) {
       pontos_push(curva_resultado, M);
    } else {
        subdivide_Hermite(P0, P1, P2, P3, curva_resultado, t0, (t0+t1)/2, tolerance);
        subdivide_Hermite(P0, P1, P2, P3, curva_resultado, (t0+t1)/2, t1, tolerance);
    }
}
void gerar_curva_hermite(ponto P0, ponto P1, ponto P2, ponto P3, Pontos *curva_resultado) {
  ponto T1, T2;
  calcular_tangentes_hermite(P0, P1, P2, P3, &T1, &T2);
  curva_resultado->quantidade_atual = 0;
  ponto A = calcular_ponto_hermite(P0, P1, T1, T2,0.0f);
  pontos_push(curva_resultado, A);
  subdivide_Hermite(P0, P1, P2, P3, curva_resultado, 0.0f, 1.0f, 0.1f);
  ponto B = calcular_ponto_hermite(P0,P1,P2,P3, 1.0f);
  pontos_push(curva_resultado, B);
 }
