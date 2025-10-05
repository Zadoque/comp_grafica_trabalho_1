#include "../includes/transformacoes.h"

//#include <stdio.h>

void calcular_centro_medio(ponto *centro, Pontos *pontos) {
  centro->point[0] = 0;
  centro->point[1] = 0;
  for (int i = 0; i < (int)(pontos->quantidade_atual); i++) {
    centro->point[0] += pontos->data[i].point[0];
    centro->point[1] += pontos->data[i].point[1];
  }
  centro->point[0] /= (float)pontos->quantidade_atual;
  centro->point[1] /= (float)pontos->quantidade_atual;
}

void translacao_com_mouse(Pontos *pontos, ponto centro, int x_mouse,
                          int y_mouse) {
  for (int i = 0; i < pontos->quantidade_atual; i++) {
    pontos->data[i].point[0] = pontos->data[i].point[0] - (float)centro.point[0] + (float)x_mouse;
    pontos->data[i].point[1] = pontos->data[i].point[1] - (float)centro.point[1] + (float)y_mouse;
  }
}


void rotacionar(Pontos *pontos, ponto centro, float angulo_graus) {
  float ** rotacao = CriarMatrizRotacao(angulo_graus);
  float ** translacao1 = CriarMatrizTranslacao(-centro.point[0], -centro.point[1]);
  float ** translacao2 = CriarMatrizTranslacao(centro.point[0], centro.point[1]);
  float ** result = MultiplicaMatriz(translacao1,rotacao);
  result = MultiplicaMatriz(result,translacao2);
  for (int i = 0; i < pontos->quantidade_atual; i++) {
    float* new = MultiplicaPonto(pontos->data[i].point, result);
    pontos->data[i].point[0] = new[0];
    pontos->data[i].point[1] = new[1];
  }
}
void aumentar_escala(Pontos *pontos, ponto centro) {
  for (int i = 0; i < pontos->quantidade_atual; i++) {
    pontos->data[i].point[0] += (pontos->data[i].point[0] - centro.point[0]) * 0.05;
    pontos->data[i].point[1] += (pontos->data[i].point[1] - centro.point[1]) * 0.05;
  }
}

void diminuir_escala(Pontos *pontos, ponto centro) {
  for (int i = 0; i < pontos->quantidade_atual; i++) {
    pontos->data[i].point[0] -= (pontos->data[i].point[0] - centro.point[0]) * 0.05;
    pontos->data[i].point[1] -= (pontos->data[i].point[1] - centro.point[1]) * 0.05;
  }
}
/*


void transladar_para_origem(Pontos *pontos, ponto centro) {
  for (int i = 0; i < pontos->quantidade_atual; i++) {
    pontos->data[i].point[0] -= centro.point[0];
    pontos->data[i].point[1] -= centro.point[1];
  }
}

void trasladar_de_volta(Pontos *pontos, ponto centro) {
  for (int i = 0; i < pontos->quantidade_atual; i++) {
    pontos->data[i].point[0] += centro.point[0];
    pontos->data[i].point[1] += centro.point[1];
  }
}

*/

