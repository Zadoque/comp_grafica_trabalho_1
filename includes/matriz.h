#ifndef MATRIZ_H
#define MATRIZ_H
#include <math.h>

float *MultiplicaPonto(float *vetor, float **matriz);

float **MultiplicaMatriz(float **matriz1, float **matriz2);

float** CriarMatrizTranslacao(float Tx, float Ty);

float** CriarMatrizEscala(float Sx, float Sy);

float** CriarMatrizRotacao(float angulo);
// Cisalhamento em X (horizontal)
float** CriarMatrizCisalhamentoX(float shx);

// Cisalhamento em Y (vertical)
float** CriarMatrizCisalhamentoY(float shy);
// Reflexão em relação ao eixo X
float** CriarMatrizEspelhoX();

// Reflexão em relação ao eixo Y
float** CriarMatrizEspelhoY();
// Reflexão em relação à origem
float** CriarMatrizEspelhoOrigem();


#endif