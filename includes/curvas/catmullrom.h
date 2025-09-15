#ifndef CATMULLROM_H
#define CATMULLROM_H

#include "../pontos.h"

/**
 * Curva Catmull-Rom
 * - SEMPRE passa pelos pontos de controle
 * - Tangentes calculadas automaticamente
 * - Interpolação suave entre pontos
 * - Ideal para trajetórias e animações
 */

void gerar_curva_catmullrom(Pontos* pontos_controle, Pontos* curva_resultado, int resolucao);
ponto calcular_ponto_catmullrom(ponto P0, ponto P1, ponto P2, ponto P3, float t);

#endif
