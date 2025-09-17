#ifndef BSPLINE_H
#define BSPLINE_H

#include "../pontos.h"

/**
 * Curva B-Spline
 * - Curva suave que NÃO passa pelos pontos de controle
 * - Cada ponto influencia apenas uma região local
 * - Muito estável para edição
 * - Ideal para CAD e modelagem
 */

void gerar_curva_bspline(Pontos* pontos_controle, Pontos* curva_resultado);
ponto calcular_ponto_bspline(ponto P0, ponto P1, ponto P2, ponto P3, float t);

#endif
