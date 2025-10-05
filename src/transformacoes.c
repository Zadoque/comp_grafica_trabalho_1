#include "../includes/transformacoes.h"
#include <math.h>
//#include <stdio.h>

void calcular_centro_medio(ponto *centro, Pontos *pontos) {
  centro->point[0] = 0;
  centro->point[1] = 0;
  for (int i = 0; i < (int)(pontos->quantidade_atual); i++) {
    centro->point[0] += pontos->data[i].point[0];
    centro->point[1] += pontos->data[i].point[1];
  }
  centro->point[0] /= (float)pontos->quantidade_atual;
  centro->point[0] /= (float)pontos->quantidade_atual;
}

void translacao_com_mouse(Pontos *pontos, ponto centro, int x_mouse,
                          int y_mouse) {
  for (int i = 0; i < pontos->quantidade_atual; i++) {
    pontos->data[i].point[0] = pontos->data[i].point[0] - (float)centro.point[0] + (float)x_mouse;
    pontos->data[i].point[1] = pontos->data[i].point[1] - (float)centro.point[1] + (float)y_mouse;
  }
}
void rotacionar(Pontos *pontos, ponto centro, float angulo_graus) {
  float angulo_rad = angulo_graus * 3.14159 / 180;
  
  for (int i = 0; i < pontos->quantidade_atual; i++) {
    float x_original = pontos->data[i].point[0];
    float y_original = pontos->data[i].point[1];
    pontos->data[i].point[0] =
        x_original * cosf(angulo_rad) - y_original * sinf(angulo_rad);
    pontos->data[i].point[1] =
        x_original * sinf(angulo_rad) + y_original * cosf(angulo_rad);
  }
  
}
/*


void transladar_para_origem(Pontos *pontos, ponto centro) {
  for (int i = 0; i < pontos->quantidade_atual; i++) {
    pontos->data[i].point[0] -= centro.x;
    pontos->data[i].point[1] -= centro.y;
  }
}

void trasladar_de_volta(Pontos *pontos, ponto centro) {
  for (int i = 0; i < pontos->quantidade_atual; i++) {
    pontos->data[i].x += centro.x;
    pontos->data[i].y += centro.y;
  }
}



void aumentar_escala(Pontos *pontos, ponto centro) {
  for (int i = 0; i < pontos->quantidade_atual; i++) {
    pontos->data[i].x += (pontos->data[i].x - centro.x) * 0.05;
    pontos->data[i].y += (pontos->data[i].y - centro.y) * 0.05;
  }
}

void diminuir_escala(Pontos *pontos, ponto centro) {
  for (int i = 0; i < pontos->quantidade_atual; i++) {
    pontos->data[i].x -= (pontos->data[i].x - centro.x) * 0.05;
    pontos->data[i].y -= (pontos->data[i].y - centro.y) * 0.05;
  }
}*/