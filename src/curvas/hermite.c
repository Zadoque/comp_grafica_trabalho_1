#include "../../includes/curvas/hermite.h"

// #include <math.h>
//#include <stdio.h>

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
  resultado.x = coeficientes[0] * P1.x + coeficientes[1] * P2.x +
                coeficientes[2] * T1.x + coeficientes[3] * T2.x;
  resultado.y = coeficientes[0] * P1.y + coeficientes[1] * P2.y +
                coeficientes[2] * T1.y + coeficientes[3] * T2.y;

  return resultado;
}
void calcular_tangentes_hermite(ponto P1, ponto P2, ponto P3, ponto P4,
                                ponto *T1, ponto *T2) {
  T1->x = P2.x - P1.x;
  T1->y = P2.y - P1.y;
  T2->x = P4.x - P3.x;
  T2->y = P4.y - P3.y;
}

void gerar_curva_hermite(Pontos *pontos_controle, Pontos *curva_resultado) {
  if (pontos_controle->quantidade_atual <= 2)
    return;

  // Limpar curva anterior

  curva_resultado->quantidade_atual = 0;
  int i = 0;
  ponto P0;
  ponto P1;
  ponto P2;
  ponto T1;
  ponto T2;
  int resolucao;
  while (pontos_controle->quantidade_atual - i >= 4) {
    ponto P3 = pontos_controle->data[i + 3];
    P0 = pontos_controle->data[i];
    P1 = pontos_controle->data[i + 1];
    P2 = pontos_controle->data[i + 2];
    resolucao = (int)(calcula_distancia(P0, P1)) * 5;
    calcular_tangentes_hermite(P0, P1, P2, P3, &T1, &T2);
    for (int j = 0; j <= resolucao; j++) {
      float t = ((float)(j) + 0.0001) / (float)(resolucao);
      ponto p = calcular_ponto_hermite(P0, P1, T1, T2, t);
      pontos_push(curva_resultado, p.x, p.y);
    }
    i++;
  }
  P0 = pontos_controle->data[i];
  P1 = pontos_controle->data[i + 1];
  P2 = pontos_controle->data[i + 2];
  calcular_tangentes_hermite(P0, P1, P1, P2, &T1, &T2);
  resolucao = (int)(calcula_distancia(P0, P1)) * 5;
  for (int j = 0; j <= resolucao; j++) {
    float t = ((float)(j) + 0.0001) / (float)(resolucao);
    ponto p = calcular_ponto_hermite(P0, P1, T1, T2, t);
    pontos_push(curva_resultado, p.x, p.y);
  }
}
