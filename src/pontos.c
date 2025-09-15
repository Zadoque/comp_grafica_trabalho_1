#include "./../includes/pontos.h"
#include <stdio.h>
#include <stdlib.h>

// Definição das matrizes (movidas de global para aqui)
const float HERMITE[4][4] = {{2.0f, -2.0f, 1.0f, 1.0f},
                            {-3.0f, 3.0f, -2.0f, -1.0f},
                            {0.0f, 0.0f, 1.0f, 0.0f},
                            {1.0f, 0.0f, 0.0f, 0.0f}};

const float BEZIER[4][4] = {{-1.0f, 3.0f, -3.0f, 1.0f},
                           {3.0f, -6.0f, 3.0f, 0.0f},
                           {-3.0f, 3.0f, 0.0f, 0.0f},
                           {1.0f, 0.0f, 0.0f, 0.0f}};

const float BSPLINE[4][4] = {
    {-1.0f / 6.0f, 3.0f / 6.0f, -3.0f / 6.0f, 1.0f / 6.0f},
    {3.0f / 6.0f, -6.0f / 6.0f, 3.0f / 6.0f, 0.0f / 6.0f},
    {-3.0f / 6.0f, 0.0f / 6.0f, 3.0f / 6.0f, 0.0f / 6.0f},
    {1.0f / 6.0f, 4.0f / 6.0f, 1.0f / 6.0f, 0.0f / 6.0f}};

const float CATMULLROM[4][4] = {{-0.5f, 1.5f, -1.5f, 0.5f},
                               {1.0f, -2.5f, 2.0f, -0.5f},
                               {-0.5f, 0.0f, 0.5f, 0.0f},
                               {0.0f, 1.0f, 0.0f, 0.0f}};

void pontos_init(Pontos *v, size_t cap0) {
  v->quantidade_atual = 0;
  v->quantidade_max = cap0 ? cap0 : 20;
  v->data = (ponto *)malloc(v->quantidade_max * sizeof(ponto));
  if (!v->data) {
    printf("Erro: falha ao alocar memória para pontos\n");
    exit(1);
  }
}

static void pontos_ensure(Pontos *v, size_t needed) {
  if (needed <= v->quantidade_max) return;
  
  size_t newcap = v->quantidade_max + v->quantidade_max / 2;
  if (newcap < needed) newcap = needed;
  
  ponto *tmp = (ponto *)realloc(v->data, newcap * sizeof(ponto));
  if (!tmp) {
    printf("Erro: falha ao realocar memória\n");
    return;
  }
  v->data = tmp;
  v->quantidade_max = newcap;
}

void pontos_push(Pontos *v, float x, float y) {
  pontos_ensure(v, v->quantidade_atual + 1);
  v->data[v->quantidade_atual++] = (ponto){x, y};
}

void pontos_free(Pontos *v) {
  free(v->data);
  v->data = NULL;
  v->quantidade_atual = v->quantidade_max = 0;
}

void esvazia_points(Pontos *v){
  for(int i = 0; i < v->quantidade_atual; i++){
    v->data[i].x = 0;
    v->data[i].y = 0;
  }
  v ->quantidade_atual = 0;
}
