
#include "../includes/matriz.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


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

float** CriarMatrizTranslacao(float Tx, float Ty){
   float **MatrizT = (float **)malloc(3 * sizeof(float *));
  if (!MatrizT) {
    printf("erro ao alocar memória");
    exit(-1);
  }
  for (int i = 0; i < 3; i++) {
    MatrizT[i] = (float *)calloc(3, sizeof(float));
    if (!MatrizT[i]) {
      printf("erro ao alocar memória");
      exit(-1);
    }
    MatrizT[i][i] = 1;
  }
  MatrizT[2][0] = Tx;
  MatrizT[2][1] = Ty;
  return MatrizT;
}

float** CriarMatrizEscala(float Sx, float Sy) {
    float **MatrizS = (float **)malloc(3 * sizeof(float *));
    if (!MatrizS) {
        printf("erro ao alocar memória");
        exit(-1);
    }
    for (int i = 0; i < 3; i++) {
        MatrizS[i] = (float *)calloc(3, sizeof(float));
        if (!MatrizS[i]) {
            printf("erro ao alocar memória");
            exit(-1);
        }
    }
    MatrizS[0][0] = Sx;
    MatrizS[1][1] = Sy;
    MatrizS[2][2] = 1;
    return MatrizS;
}


float** CriarMatrizRotacao(float angulo) {
    float rad = angulo * M_PI / 180.0;  // Converte para radianos
    float **MatrizR = (float **)malloc(3 * sizeof(float *));
    if (!MatrizR) {
        printf("erro ao alocar memória");
        exit(-1);
    }
    for (int i = 0; i < 3; i++) {
        MatrizR[i] = (float *)calloc(3, sizeof(float));
        if (!MatrizR[i]) {
            printf("erro ao alocar memória");
            exit(-1);
        }
    }
    MatrizR[0][0] = cos(rad);
    MatrizR[0][1] = sin(rad);
    MatrizR[1][0] = -sin(rad);
    MatrizR[1][1] = cos(rad);
    MatrizR[2][2] = 1;
    return MatrizR;
}

// Cisalhamento em X (horizontal)
float** CriarMatrizCisalhamentoX(float shx) {
    float **MatrizCx = (float **)malloc(3 * sizeof(float *));
    if (!MatrizCx) {
        printf("erro ao alocar memória");
        exit(-1);
    }
    for (int i = 0; i < 3; i++) {
        MatrizCx[i] = (float *)calloc(3, sizeof(float));
        if (!MatrizCx[i]) {
            printf("erro ao alocar memória");
            exit(-1);
        }
        MatrizCx[i][i] = 1;
    }
    MatrizCx[0][1] = shx;
    return MatrizCx;
}

// Cisalhamento em Y (vertical)
float** CriarMatrizCisalhamentoY(float shy) {
    float **MatrizCy = (float **)malloc(3 * sizeof(float *));
    if (!MatrizCy) {
        printf("erro ao alocar memória");
        exit(-1);
    }
    for (int i = 0; i < 3; i++) {
        MatrizCy[i] = (float *)calloc(3, sizeof(float));
        if (!MatrizCy[i]) {
            printf("erro ao alocar memória");
            exit(-1);
        }
        MatrizCy[i][i] = 1;
    }
    MatrizCy[1][0] = shy;
    return MatrizCy;
}

// Reflexão em relação ao eixo X
float** CriarMatrizEspelhoX() {
    float **MatrizEx = (float **)malloc(3 * sizeof(float *));
    if (!MatrizEx) {
        printf("erro ao alocar memória");
        exit(-1);
    }
    for (int i = 0; i < 3; i++) {
        MatrizEx[i] = (float *)calloc(3, sizeof(float));
        if (!MatrizEx[i]) {
            printf("erro ao alocar memória");
            exit(-1);
        }
    }
    MatrizEx[0][0] = 1;
    MatrizEx[1][1] = -1;
    MatrizEx[2][2] = 1;
    return MatrizEx;
}

// Reflexão em relação ao eixo Y
float** CriarMatrizEspelhoY() {
    float **MatrizEy = (float **)malloc(3 * sizeof(float *));
    if (!MatrizEy) {
        printf("erro ao alocar memória");
        exit(-1);
    }
    for (int i = 0; i < 3; i++) {
        MatrizEy[i] = (float *)calloc(3, sizeof(float));
        if (!MatrizEy[i]) {
            printf("erro ao alocar memória");
            exit(-1);
        }
    }
    MatrizEy[0][0] = -1;
    MatrizEy[1][1] = 1;
    MatrizEy[2][2] = 1;
    return MatrizEy;
}

// Reflexão em relação à origem
float** CriarMatrizEspelhoOrigem() {
    float **MatrizEo = (float **)malloc(3 * sizeof(float *));
    if (!MatrizEo) {
        printf("erro ao alocar memória");
        exit(-1);
    }
    for (int i = 0; i < 3; i++) {
        MatrizEo[i] = (float *)calloc(3, sizeof(float));
        if (!MatrizEo[i]) {
            printf("erro ao alocar memória");
            exit(-1);
        }
    }
    MatrizEo[0][0] = -1;
    MatrizEo[1][1] = -1;
    MatrizEo[2][2] = 1;
    return MatrizEo;
}
