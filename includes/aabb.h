#ifndef AABB_H
#define AABB_H

typedef struct {
    float x_min, y_min;
    float x_max, y_max;
    int segmento_indice;
} AABB;

typedef struct aabbtree {
    int altura;
    struct aabbtree* esquerda;
    struct aabbtree* direita;
    AABB box; // inline, não ponteiro — menos indireção, mais cache-friendly
} AABBTREE;

typedef struct {
    AABB* dados;
    int quantidade;
    int capacidade;
} AABB_vec;

// Inicializa o vetor com capacidade inicial
void aabb_vec_init(AABB_vec* v, int capacidade_inicial);

// Adiciona uma AABB, realoca 1.5x se necessário
void aabb_vec_push(AABB_vec* v, AABB box);

// Testa se ponto está dentro da caixa — sem branch
int ponto_dentro_aabb(AABB box, float px, float py);
AABBTREE* aabb_vec_para_arvore(AABB_vec* v);
void      aabb_tree_free(AABBTREE* no);

#endif
