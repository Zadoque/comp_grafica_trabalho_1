#ifndef BSPLINE_H
#define BSPLINE_H
#include "../pontos.h"
#include "../aabb.h"

// ─── Avaliação da curva ───────────────────────────────────────────────────────

ponto calcular_ponto_bspline(ponto P0, ponto P1, ponto P2, ponto P3, float t);

// ─── Geração da curva (subdivisão adaptativa) ────────────────────────────────

void subdivide(ponto P0, ponto P1, ponto P2, ponto P3,
               Pontos *curva_resultado,
               ponto A, ponto B,
               float t0, float t1,
               float tolerance, AABB *box);

void gerar_curva_bspline(ponto P0, ponto P1, ponto P2, ponto P3,
                          Pontos *curva_resultado, AABB *box);

// ─── Picking ─────────────────────────────────────────────────────────────────

ResultadoPicking picking_bspline(AABBTREE *arvore, Pontos *clicks,
                                  ponto mouse, float tolerancia, float melhor_dist);

#endif // BSPLINE_H
