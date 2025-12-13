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




void rotacionar_com_mouse(Pontos *pontos, ponto *centro, ponto mouse, int indice){
  ponto vetor1;
  ponto vetor2;
  vetor1.point[2] = 1;
  vetor2.point[2] = 1;

  vetor1.point[0] = pontos->data[indice].point[0] - centro->point[0];
  vetor1.point[1] =  pontos->data[indice].point[1] - centro->point[1];
  vetor2.point[0] =  mouse.point[0] - centro->point[0];
  vetor2.point[1] = mouse.point[1] - centro->point[1];
  //printf("O Vetor1 é:\n\tx: %2.f\n\ty: %2.f\nO vetor2 é:\n\tx: %2.f\n\ty: %2.f\n", vetor1.point[0], vetor1.point[1], vetor2.point[0], vetor2.point[1]);
  float angulo1 = atan2f(vetor1.point[1], vetor1.point[0]);
  float angulo2 = atan2f(vetor2.point[1], vetor2.point[0]);
  float angulo_rad = angulo2 - angulo1;
  //printf("\n\t O angulo entre eles é %f\n\t", angulo_rad);
  float ** rotacao = CriarMatrizRotacao(angulo_rad);
  float ** translacao1 = CriarMatrizTranslacao(-centro->point[0], -centro->point[1]);
  float ** translacao2 = CriarMatrizTranslacao(centro->point[0], centro->point[1]);
  float ** result = MultiplicaMatriz(translacao1,rotacao);
  result = MultiplicaMatriz(result,translacao2);
  for (int i = 0; i < pontos->quantidade_atual; i++) {
    float* new = MultiplicaPonto(pontos->data[i].point, result);
    pontos->data[i].point[0] = new[0];
    pontos->data[i].point[1] = new[1];
  } 
}
void escala_com_mouse(Pontos* pontos, ponto* centro, ponto mouse, int indice){
 
  float ** translacao1 = CriarMatrizTranslacao(-centro->point[0], -centro->point[1]);
  float ** translacao2 = CriarMatrizTranslacao(centro->point[0], centro->point[1]);
  ponto p;
  p.point[0] = pontos->data[indice].point[0];
  p.point[1] = pontos->data[indice].point[1];
  p.point[2] = 1;
  float escala = calcula_distancia(mouse, *centro) / calcula_distancia(p, *centro);

  float ** Matriz_escala = CriarMatrizEscala(escala, escala);
  Matriz_escala = MultiplicaMatriz(translacao1, Matriz_escala);
  Matriz_escala = MultiplicaMatriz(translacao2, Matriz_escala);
  for (int i = 0; i < pontos->quantidade_atual; i++){
    float* new = MultiplicaPonto(pontos->data[i].point, Matriz_escala);
    pontos->data[i].point[0] = new[0];
    pontos->data[i].point[1] = new[1];
  }void escala_com_mouse(Pontos* pontos, ponto* centro, ponto mouse, int indice){
 ponto vetor1;
  ponto vetor2;
  vetor1.point[2] = 1;
  vetor2.point[2] = 1;

  vetor1.point[0] = pontos->data[indice].point[0] - centro->point[0];
  vetor1.point[1] =  pontos->data[indice].point[1] - centro->point[1];
  vetor2.point[0] =  mouse.point[0] - centro->point[0];
  vetor2.point[1] = mouse.point[1] - centro->point[1];
  float escala_x = vetor2.point[0] / vetor1.point[0];
  float escala_y = vetor2.point[1] / vetor1.point[1];
  float ** Matriz_escala = CriarMatrizEscala(escala_x, escala_y);
  for (int i = 0; i < pontos->quantidade_atual; i++){
    float* new = MultiplicaPonto(pontos->data[i].point, Matriz_escala);
    pontos->data[i].point[0] = new[0];
    pontos->data[i].point[1] = new[1];
  }
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

