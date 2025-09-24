#ifndef HERMITE_H
#define HERMITE_H

#include "../pontos.h"

/**
 * Curva de Hermite
 * - Conecta 2 pontos usando vetores tangentes
 * - Controla a "velocidade" de entrada e saída
 * - Ideal para animações suaves
 */

// Gerar curva Hermite completa
void gerar_curva_hermite(Pontos* pontos_controle, Pontos* curva_resultado, int poligono);

// Calcular um ponto específico na curva (t entre 0 e 1)
ponto calcular_ponto_hermite(ponto P1, ponto P2, ponto T1, ponto T2, float t);

// Calcular tangentes automáticas baseadas nos pontos
void calcular_tangentes_hermite( ponto P1, ponto P2, ponto P3, ponto P4, ponto* T1, ponto* T2);

#endif
