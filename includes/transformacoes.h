#ifndef TRANSFORMACOES_H
#define TRANSFORMACOES_H

#include "pontos.h"
#include "menu.h" 
#include "matriz.h" 


void calcular_centro_medio(ponto *centro,Pontos* pontos);

void translacao_com_mouse(Pontos* pontos, ponto centro, int x_mouse, int y_mouse);
void transladar_para_origem(Pontos *pontos, ponto centro);
void trasladar_de_volta(Pontos *pontos, ponto centro);

void rotacionar(Pontos *pontos, ponto centro, float angulo_graus);

void aumentar_escala(Pontos* pontos, ponto centro);

void diminuir_escala(Pontos* pontos, ponto centro);


#endif
