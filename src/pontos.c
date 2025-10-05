#include "./../includes/pontos.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


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

void pontos_push(Pontos *v, ponto p) {
  pontos_ensure(v, v->quantidade_atual + 1);
  v->data[v->quantidade_atual++] = p;
}

void pontos_free(Pontos *v) {
  free(v->data);
  v->data = NULL;
  v->quantidade_atual = v->quantidade_max = 0;
}

void esvazia_points(Pontos *v){
  for(int i = 0; i < v->quantidade_atual; i++){
    v->data[i].point[0] = 0;
    v->data[i].point[1] = 0;
  }
  v ->quantidade_atual = 0;
}

float calcula_distancia(ponto P1, ponto P2) {
  double dist_x = (double)(P2.point[0] - P1.point[0]);
  double dist_y = (double)(P2.point[1] - P1.point[1]);
  return sqrt(dist_x * dist_x + dist_y * dist_y);
}