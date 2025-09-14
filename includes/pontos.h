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
  float x;
  float y;
} ponto;

typedef struct {
  size_t quantidade_atual;
  size_t quantidade_max;
  ponto *data;
} Pontos;

// Funções relacionadas a Pontos
void pontos_init(Pontos *v, size_t cap0);
void pontos_push(Pontos *v, float x, float y);
void pontos_free(Pontos *v);

#endif
