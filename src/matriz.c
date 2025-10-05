
#include "../includes/matriz.h"
#include <stdio.h>
#include <stdlib.h>

float MatrizT[3][3] = {{1.0, 0, 0}, {0, 1.0, 0}, {0, 0, 0}};

float *MultiplicaPonto(float *vetor, float **matriz) {
  float *result = (float *)malloc(3 * sizeof(float));
  if (!result) {
    printf("erro ao alocar memória");
    exit(-1);
  }
  result[0] = vetor[0] * matriz[0][0] + vetor[1] * matriz[1][0] +
              vetor[2] * matriz[2][0];
  result[1] = vetor[0] * matriz[0][1] + vetor[1] * matriz[1][1] +
              vetor[2] * matriz[2][1];
  result[2] = vetor[0] * matriz[0][2] + vetor[1] * matriz[1][2] +
              vetor[2] * matriz[2][2];
  return result;
}

float **MultiplicaMatriz(float **matriz1, float **matriz2) {
  float **result = (float **)malloc(3 * sizeof(float *));
  if (!result) {
    printf("erro ao alocar memória");
    exit(-1);
  }
  for (int i = 0; i < 3; i++) {
    result[i] = (float *)malloc(3 * sizeof(float));
    if (!result[i]) {
      printf("erro ao alocar memória");
      exit(-1);
    }
  }
  // Multiplicação das matrizes
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      result[i][j] = 0;
      for (int k = 0; k < 3; k++) {
        result[i][j] += matriz1[i][k] * matriz2[k][j];
      }
    }
  }
  return result;
}