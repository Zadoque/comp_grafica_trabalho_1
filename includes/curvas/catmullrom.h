#ifndef CATMULLROM_H
#define CATMULLROM_H

#include "../pontos.h"
#include "../aabb.h"

/**
 * Curva Catmull-Rom
 * - SEMPRE passa pelos pontos de controle
 * - Tangentes calculadas automaticamente
 * - Interpolação suave entre pontos
 * - Ideal para trajetórias e animações
 */
void arrastar_ponto_catmullrom(Pontos *clicks, int seg, float ti, ponto mouse);
ResultadoPicking picking_catmullrom(AABBTREE *arvore, Pontos *clicks, ponto mouse, float tolerancia, float melhor_dist);
ponto calcular_ponto_catmullrom(ponto P0, ponto P1, ponto P2, ponto P3, float t);
void gerar_curva_catmullrom(ponto P0, ponto P1, ponto P2, ponto P3, Pontos *curva, AABB *box);

#endif
