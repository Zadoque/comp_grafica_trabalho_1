#include "../includes/operacoes.h"
#include <math.h>
#include <stdio.h>

void calcular_centro_medio(ponto *centro, Pontos *pontos, Poligono poligono){
int j;
if(poligono == MODO_POLIGONO_FECHADO && pontos->quantidade_atual > 2){
  j = 1;
}else{ j = 0;}
  for (int i = 0; i < (int)(pontos->quantidade_atual) - j; i++)
  {
    centro->x += pontos->data[i].x;
    centro->y += pontos->data[i].y;
  }

  centro->x /= (float)pontos->quantidade_atual;
  centro->y /= (float)pontos->quantidade_atual;
}

void translacao_com_mouse(Pontos *pontos, ponto centro, int x_mouse,
                          int y_mouse)
{
  for (int i = 0; i < pontos->quantidade_atual; i++)
  {
    pontos->data[i].x = pontos->data[i].x - (float)centro.x + (float)x_mouse;
    pontos->data[i].y = pontos->data[i].y - (float)centro.y + (float)y_mouse;
  }
}

void transladar_para_origem(Pontos *pontos, ponto centro)
{
  for (int i = 0; i < pontos->quantidade_atual; i++)
  {
    pontos->data[i].x -= centro.x;
    pontos->data[i].y -= centro.y;
  }
}

void trasladar_de_volta(Pontos *pontos, ponto centro)
{
  for (int i = 0; i < pontos->quantidade_atual; i++)
  {
    pontos->data[i].x += centro.x;
    pontos->data[i].y += centro.y;
  }
}

void rotacionar(Pontos *pontos, ponto centro, float angulo_graus)
{
  float angulo_rad = angulo_graus * 3.14159 / 180;
  transladar_para_origem(pontos, centro);
  for (int i = 0; i < pontos->quantidade_atual; i++)
  {
    float x_original = pontos->data[i].x;
    float y_original = pontos->data[i].y;
    pontos->data[i].x =
        x_original * cosf(angulo_rad) - y_original * sinf(angulo_rad);
    pontos->data[i].y =
        x_original * sinf(angulo_rad) + y_original * cosf(angulo_rad);
  }
  trasladar_de_volta(pontos, centro);
}