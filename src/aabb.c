#include <stdlib.h>
#include "./../includes/aabb.h"
#include <math.h>
int ponto_dentro_aabb(AABB box, float px, float py) {
    // Cada comparação retorna 0 ou 1 como int
    return (px >= box.x_min) & (px <= box.x_max) &
           (py >= box.y_min) & (py <= box.y_max);
}


void aabb_vec_push(AABB_vec* v, AABB box) {
    if (v->quantidade >= v->capacidade) {
        v->capacidade = (int)(v->capacidade * 1.5f) + 1;
        v->dados = realloc(v->dados, v->capacidade * sizeof(AABB));
    }
    v->dados[v->quantidade] = box;
    v->dados[v->quantidade].segmento_indice = v->quantidade;
    v->quantidade++;
}

void aabb_vec_init(AABB_vec* v, int capacidade_inicial) {
    v->dados      = (AABB*)malloc(capacidade_inicial * sizeof(AABB));
    v->quantidade = 0;
    v->capacidade = capacidade_inicial;

    if (v->dados == NULL) {
        v->capacidade = 0;
        exit(-1);
    }
}


static AABBTREE* construir_recursivo(AABB* dados, int inicio, int fim) {
    AABBTREE* no = (AABBTREE*)malloc(sizeof(AABBTREE));
    if (no == NULL) return NULL;

    if (inicio == fim) {
        // Nó folha
        no->box      = dados[inicio];
        no->esquerda = NULL;
        no->direita  = NULL;
        no->altura   = 0;
        return no;
    }

    int meio = (inicio + fim) / 2;
    no->esquerda = construir_recursivo(dados, inicio, meio);
    no->direita  = construir_recursivo(dados, meio + 1, fim);

    // União das caixas dos filhos
    AABB* e = &no->esquerda->box;
    AABB* d = &no->direita->box;

    no->box.x_min = fminf(e->x_min, d->x_min);
    no->box.y_min = fminf(e->y_min, d->y_min);
    no->box.x_max = fmaxf(e->x_max, d->x_max);
    no->box.y_max = fmaxf(e->y_max, d->y_max);
    no->box.segmento_indice = -1;

    // Altura = 1 + maior filho
    int alt_e = no->esquerda  ? no->esquerda->altura  : -1;
    int alt_d = no->direita   ? no->direita->altura   : -1;
    no->altura = 1 + (alt_e > alt_d ? alt_e : alt_d);

    return no;
}

AABBTREE* aabb_vec_para_arvore(AABB_vec* v) {
    if (v == NULL || v->quantidade == 0) return NULL;
    return construir_recursivo(v->dados, 0, v->quantidade - 1);
}

void aabb_tree_free(AABBTREE* no) {
    if (no == NULL) return;
    aabb_tree_free(no->esquerda);
    aabb_tree_free(no->direita);
    free(no);
}
