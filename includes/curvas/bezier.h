#ifndef BEZIER_H
#define BEZIER_H

#include "../pontos.h"

/**
 * Curva de Bézier
 * - Usa pontos de controle que "atraem" a curva
 * - Sempre passa pelos pontos inicial e final
 * - Pontos intermediários controlam a forma
 * - Ideal para design gráfico
 */

void gerar_curva_bezier(ponto P0, ponto P1, ponto P2, ponto P3, Pontos* curva_resultado);
ponto calcular_ponto_bezier(ponto P0, ponto P1, ponto P2, ponto P3, float t);
void subdivide_Bezier(ponto P0, ponto P1, ponto P2, ponto P3, Pontos *curva_resultado, float t0, float t1, float tolerance);
#endif
