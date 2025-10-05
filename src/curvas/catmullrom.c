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

void gerar_curva_catmullrom(Pontos *pontos_controle, Pontos *curva_resultado) {
  if (pontos_controle->quantidade_atual < 4)
    return;

  curva_resultado->quantidade_atual = 0;

  // Catmull-Rom: interpola entre pontos P1 e P2, usando P0 e P3 para tangentes
  for (int i = 0; i <= pontos_controle->quantidade_atual - 4; i++) {
    ponto P0 = pontos_controle->data[i];
    ponto P1 = pontos_controle->data[i + 1];
    ponto P2 = pontos_controle->data[i + 2];
    ponto P3 = pontos_controle->data[i + 3];
    int resolucao = (int)(calcula_distancia(P1, P2)) * 5;

    // Gerar pontos do segmento
    for (int j = 0; j <= resolucao; j++) {
      float t = (float)(j + 0.000001) / (float)resolucao;
      ponto p = calcular_ponto_catmullrom(P0, P1, P2, P3, t);
      // Evitar duplicar pontos entre segmentos
      pontos_push(curva_resultado, p);
    }
  }
  
  
    
    //printf("Curva Catmull-Rom gerada com %zu pontos\n",
          // curva_resultado->quantidade_atual);
  
           
}