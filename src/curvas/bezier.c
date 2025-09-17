#include "../../includes/curvas/bezier.h"
// #include <math.h>
//#include <stdio.h>

// Matriz de Bézier (corrigida)
static const float MATRIZ_BEZIER[4][4] = {
    {-1.0f, 3.0f, -3.0f, 1.0f}, // -t³ + 3t² - 3t + 1
    {3.0f, -6.0f, 3.0f, 0.0f},  // 3t³ - 6t² + 3t
    {-3.0f, 3.0f, 0.0f, 0.0f},  // -3t³ + 3t²
    {1.0f, 0.0f, 0.0f, 0.0f}    // t³
};

ponto calcular_ponto_bezier(ponto P0, ponto P1, ponto P2, ponto P3, float t) {
  // Vetor de parâmetros [t³, t², t, 1]
  float t2 = t * t;
  float t3 = t2 * t;
  float vetor_t[4] = {t3, t2, t, 1.0f};

  // Multiplicar: vetor_t × MATRIZ_BEZIER
  float coeficientes[4] = {0.0f, 0.0f, 0.0f, 0.0f};
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      coeficientes[i] += vetor_t[j] * MATRIZ_BEZIER[j][i];
    }
  }

  // Aplicar aos pontos de controle
  ponto resultado;
  resultado.x = coeficientes[0] * P0.x + coeficientes[1] * P1.x +
                coeficientes[2] * P2.x + coeficientes[3] * P3.x;
  resultado.y = coeficientes[0] * P0.y + coeficientes[1] * P1.y +
                coeficientes[2] * P2.y + coeficientes[3] * P3.y;

  return resultado;
}

void gerar_curva_bezier(Pontos *pontos_controle, Pontos *curva_resultado
                        ) {
  if (pontos_controle->quantidade_atual < 4)
    return;

  curva_resultado->quantidade_atual = 0;

  // Processar grupos de 4 pontos
  for (int i = 0; i <= pontos_controle->quantidade_atual - 4; i += 3) {
    ponto P0 = pontos_controle->data[i];
    ponto P1 = pontos_controle->data[i + 1];
    ponto P2 = pontos_controle->data[i + 2];
    ponto P3 = pontos_controle->data[i + 3];
    int resolucao = (int)(calcula_distancia(P0, P1)) * 5;
    // Gerar pontos do segmento
    for (int j = 0; j <= resolucao; j++) {
      float t = (float)j / (float)resolucao;
      ponto p = calcular_ponto_bezier(P0, P1, P2, P3, t);

      // Evitar duplicar pontos entre segmentos
      if (i > 0 && j == 0)
        continue;

      pontos_push(curva_resultado, p.x, p.y);
    }
  }

//  printf("Curva Bézier gerada com %zu pontos\n",
         //curva_resultado->quantidade_atual);
}
