#ifndef CURVA_H
#define CURVA_H

#include <stdlib.h>

// Matrizes constantes para curvas
extern const float HERMITE[4][4];
extern const float BEZIER[4][4];
extern const float BSPLINE[4][4];
extern const float CATMULLROM[4][4];

// Tipos para curvas e pontos
typedef enum {
  CURVE_HERMITE,
  CURVE_BEZIER,
  CURVE_BSPLINE,
  CURVE_CATMULLROM
} CurveType;

typedef struct {
  float point[3];
} ponto;

typedef struct {
  int quantidade_atual;
  int quantidade_max;
  ponto *data;
} Pontos;

// Funções relacionadas a Pontos
void pontos_init(Pontos *v, size_t cap0);
void pontos_push(Pontos *v, float x, float y);
void pontos_free(Pontos *v);
void esvazia_points(Pontos *v);
double raiz_quadrada_newton(double n);
float calcula_distancia(ponto P1, ponto P2);

#endif
