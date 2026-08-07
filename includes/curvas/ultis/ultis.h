#ifndef ULTIS_H
#define ULTIS_H

#include "../../pontos.h"
#include "../../aabb.h"

/**
 * Funções usadas por todas as curvas

 */

float dist_mouse_aabb(ponto mouse, AABB box);

float dist_mouse_aabb_left(ponto mouse, AABB box);

float dist_mouse_aabb_right(ponto mouse, AABB box);
#endif