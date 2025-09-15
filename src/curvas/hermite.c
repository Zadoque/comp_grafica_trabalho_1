#include "../../includes/curvas/hermite.h"
// #include <math.h>
#include <stdio.h>

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
void calcular_tangentes_hermite(Pontos* pontos_controle, ponto* T1, ponto* T2, int indice) {
    if (pontos_controle->quantidade_atual < 2) return;
    
    // Tangente T1 (no primeiro ponto)
    if (indice > 0) {
        // Usar diferença com ponto anterior
        T1->x = pontos_controle->data[indice].x - pontos_controle->data[indice-1].x;
        T1->y = pontos_controle->data[indice].y - pontos_controle->data[indice-1].y;
    } else {
        // Primeiro ponto: usar diferença com próximo
        T1->x = pontos_controle->data[indice+1].x - pontos_controle->data[indice].x;
        T1->y = pontos_controle->data[indice+1].y - pontos_controle->data[indice].y;
    }
    
    // Tangente T2 (no segundo ponto)  
    if (indice < pontos_controle->quantidade_atual - 2) {
        // Usar diferença com próximo ponto
        T2->x = pontos_controle->data[indice+2].x - pontos_controle->data[indice+1].x;
        T2->y = pontos_controle->data[indice+2].y - pontos_controle->data[indice+1].y;
    } else {
        // Último segmento: usar diferença anterior
        T2->x = pontos_controle->data[indice+1].x - pontos_controle->data[indice].x;
        T2->y = pontos_controle->data[indice+1].y - pontos_controle->data[indice].y;
    }
}

void gerar_curva_hermite(Pontos *pontos_controle, Pontos *curva_resultado,
                         int resolucao) {
  if (pontos_controle->quantidade_atual < 2)
    return;

  // Limpar curva anterior
  curva_resultado->quantidade_atual = 0;
  
  // Gerar segmentos entre pares de pontos consecutivos
  for (int i = 0; i < pontos_controle->quantidade_atual - 1; i++) {
    ponto P1 = pontos_controle->data[i];
    ponto P2 = pontos_controle->data[i + 1];

    // Calcular tangentes para este segmento
    ponto T1, T2;
    calcular_tangentes_hermite(pontos_controle, &T1, &T2, i);

    // Gerar pontos do segmento
    for (int j = 0; j <= resolucao; j++) {
      float t = (float)j / (float)resolucao;
      ponto p = calcular_ponto_hermite(P1, P2, T1, T2, t);

      // Evitar duplicar pontos entre segmentos
      if (i > 0 && j == 0)
        continue;

      pontos_push(curva_resultado, p.x, p.y);
    }
  }
  
  printf("Curva Hermite Fechada gerada com %zu pontos\n",
         curva_resultado->quantidade_atual);
}
