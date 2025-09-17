#ifndef GEOMETRIA_H
#define GEOMETRIA_H

#include "pontos.h"
#include "menu.h"  


void calcular_centro_medio(ponto *centro,Pontos* pontos, Poligono poligono);

void translacao_com_mouse(Pontos* pontos, ponto centro, int x_mouse, int y_mouse);
void transladar_para_origem(Pontos *pontos, ponto centro);
void trasladar_de_volta(Pontos *pontos, ponto centro);

void rotacionar(Pontos *pontos, ponto centro, float angulo_graus);


#endif
