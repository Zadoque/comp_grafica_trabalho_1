#include "./../includes/menu.h"
#include <GL/glut.h>
//#include <stdio.h>
#include <string.h>

// Implementar todas as funções do menu que te mostrei antes
EstadoAplicacao estado_atual = {MODO_CRIAR_PONTO, MODO_POLIGONO_ABERTO,
                                MODO_CURVA_HERMITE, NENHUMA};

int menu_largura = 200;
Botoes botoes;
BotoesOperacoes botoes_operacoes[4];

void inicializar_menu() {
  // Definir os botões (posição relativa ao menu)
  strcpy(botoes.botoes1[0].texto, "Criar Ponto");
  botoes.botoes1[0].x = 10;
  botoes.botoes1[0].y = 50;
  botoes.botoes1[0].largura = 180;
  botoes.botoes1[0].altura = 40;
  botoes.botoes1[0].acao = MODO_CRIAR_PONTO;

  strcpy(botoes.botoes1[1].texto, "Selecionar vertice");
  botoes.botoes1[1].x = 10;
  botoes.botoes1[1].y = 100;
  botoes.botoes1[1].largura = 180;
  botoes.botoes1[1].altura = 40;
  botoes.botoes1[1].acao = MODO_SELECIONAR_PONTO;

  strcpy(botoes.botoes1[2].texto, "Selecionar Poligono");
  botoes.botoes1[2].x = 10;
  botoes.botoes1[2].y = 150;
  botoes.botoes1[2].largura = 180;
  botoes.botoes1[2].altura = 40;
  botoes.botoes1[2].acao = MODO_SELECIONAR_POLIGONO;

  strcpy(botoes.botoes2[0].texto, "Polígono Aberto");
  botoes.botoes2[0].x = 10;
  botoes.botoes2[0].y = 250;
  botoes.botoes2[0].largura = 180;
  botoes.botoes2[0].altura = 40;
  botoes.botoes2[0].acao = MODO_POLIGONO_ABERTO;

  strcpy(botoes.botoes2[1].texto, "Polígono Fechado");
  botoes.botoes2[1].x = 10;
  botoes.botoes2[1].y = 300;
  botoes.botoes2[1].largura = 180;
  botoes.botoes2[1].altura = 40;
  botoes.botoes2[1].acao = MODO_POLIGONO_FECHADO;

  strcpy(botoes.botoes3[0].texto, "Hermite");
  botoes.botoes3[0].x = 10;
  botoes.botoes3[0].y = 400;
  botoes.botoes3[0].largura = 180;
  botoes.botoes3[0].altura = 40;
  botoes.botoes3[0].acao = MODO_CURVA_HERMITE;

  strcpy(botoes.botoes3[1].texto, "Catmull-Rom");
  botoes.botoes3[1].x = 10;
  botoes.botoes3[1].y = 450;
  botoes.botoes3[1].largura = 180;
  botoes.botoes3[1].altura = 40;
  botoes.botoes3[1].acao = MODO_CURVA_CATMULLROM;

  strcpy(botoes.botoes3[2].texto, "B-Spline");
  botoes.botoes3[2].x = 10;
  botoes.botoes3[2].y = 500;
  botoes.botoes3[2].largura = 180;
  botoes.botoes3[2].altura = 40;
  botoes.botoes3[2].acao = MODO_CURVA_BSPLINE;

  strcpy(botoes.botoes3[3].texto, "Bezier");
  botoes.botoes3[3].x = 10;
  botoes.botoes3[3].y = 550;
  botoes.botoes3[3].largura = 180;
  botoes.botoes3[3].altura = 40;
  botoes.botoes3[3].acao = MODO_CURVA_BEZIER;
  
  strcpy(botoes_operacoes[0].texto, "Translacao");
  botoes_operacoes[0].x = 10;
  botoes_operacoes[0].y = 650;
  botoes_operacoes[0].largura = 180;
  botoes_operacoes[0].altura = 40;
  botoes_operacoes[0].acao = TRANSLACAO;

   strcpy(botoes_operacoes[1].texto, "Rotacao");
  botoes_operacoes[1].x = 10;
  botoes_operacoes[1].y = 700;
  botoes_operacoes[1].largura = 180;
  botoes_operacoes[1].altura = 40;
  botoes_operacoes[1].acao = ROTACAO;

   strcpy(botoes_operacoes[2].texto, "Escala");
  botoes_operacoes[2].x = 10;
  botoes_operacoes[2].y = 750;
  botoes_operacoes[2].largura = 180;
  botoes_operacoes[2].altura = 40;
  botoes_operacoes[2].acao = ESCALA;

   strcpy(botoes_operacoes[3].texto, "Shear");
  botoes_operacoes[3].x = 10;
  botoes_operacoes[3].y = 800;
  botoes_operacoes[3].largura = 180;
  botoes_operacoes[3].altura = 40;
  botoes_operacoes[3].acao = SHEAR;
}

void desenhar_texto(float x, float y, void *font, char *texto) {
  glRasterPos2f(x, y);
  for (char *c = texto; *c != '\0'; c++) {
    glutBitmapCharacter(font, *c);
  }
}

void desenhar_botoes_menu() {
  for (int i = 0; i < 3; i++) {
    Botoes1 *botao = &botoes.botoes1[i];
    if (estado_atual.criacao_ou_selecao == botao->acao) {
      glColor3f(0.2f, 0.7f, 0.2f); // Verde se ativo
    } else if (botao->destacado) {
      glColor3f(0.9f, 0.9f, 0.7f); // Amarelo se mouse em cima
    } else {
      glColor3f(0.95f, 0.95f, 0.95f); // Cinza claro normal
    }
    // Desenhar retângulo do botão
    glBegin(GL_QUADS);
    glVertex2f(botao->x, botao->y);
    glVertex2f(botao->x + botao->largura, botao->y);
    glVertex2f(botao->x + botao->largura, botao->y + botao->altura);
    glVertex2f(botao->x, botao->y + botao->altura);
    glEnd();

    // Contorno do botão
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(botao->x, botao->y);
    glVertex2f(botao->x + botao->largura, botao->y);
    glVertex2f(botao->x + botao->largura, botao->y + botao->altura);
    glVertex2f(botao->x, botao->y + botao->altura);
    glEnd();

    // === DESENHAR TEXTO DO BOTÃO ===
    glColor3f(0.0f, 0.0f, 0.0f); // Texto preto

    // Calcular posição central do texto
    void *font = GLUT_BITMAP_HELVETICA_12;
    int texto_largura = 0;

    // Calcular largura do texto
    for (char *c = botao->texto; *c != '\0'; c++) {
      texto_largura += glutBitmapWidth(font, *c);
    }

    // Posição centralizada
    float texto_x = botao->x + (botao->largura - texto_largura) / 2.0f;
    float texto_y = botao->y + (botao->altura / 2.0f) -
                    6; // -6 para centralizar verticalmente

    // Desenhar o texto
    desenhar_texto(texto_x, texto_y, font, botao->texto);
  }
  for (int i = 0; i < 4; i++) {
    Botoes2 *botao = &botoes.botoes2[i];
    if (estado_atual.poligono == botao->acao) {
      glColor3f(0.2f, 0.7f, 0.2f); // Verde se ativo
    } else if (botao->destacado) {
      glColor3f(0.9f, 0.9f, 0.7f); // Amarelo se mouse em cima
    } else {
      glColor3f(0.95f, 0.95f, 0.95f); // Cinza claro normal
    }
    // Desenhar retângulo do botão
    glBegin(GL_QUADS);
    glVertex2f(botao->x, botao->y);
    glVertex2f(botao->x + botao->largura, botao->y);
    glVertex2f(botao->x + botao->largura, botao->y + botao->altura);
    glVertex2f(botao->x, botao->y + botao->altura);
    glEnd();

    // Contorno do botão
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(botao->x, botao->y);
    glVertex2f(botao->x + botao->largura, botao->y);
    glVertex2f(botao->x + botao->largura, botao->y + botao->altura);
    glVertex2f(botao->x, botao->y + botao->altura);
    glEnd();

    // === DESENHAR TEXTO DO BOTÃO ===
    glColor3f(0.0f, 0.0f, 0.0f); // Texto preto

    // Calcular posição central do texto
    void *font = GLUT_BITMAP_HELVETICA_12;
    int texto_largura = 0;

    // Calcular largura do texto
    for (char *c = botao->texto; *c != '\0'; c++) {
      texto_largura += glutBitmapWidth(font, *c);
    }

    // Posição centralizada
    float texto_x = botao->x + (botao->largura - texto_largura) / 2.0f;
    float texto_y = botao->y + (botao->altura / 2.0f) -
                    6; // -6 para centralizar verticalmente

    // Desenhar o texto
    desenhar_texto(texto_x, texto_y, font, botao->texto);
  }
  for (int i = 0; i < 4; i++) {
    Botoes3 *botao = &botoes.botoes3[i];
    if (estado_atual.curva == botao->acao) {
      glColor3f(0.2f, 0.7f, 0.2f); // Verde se ativo
    } else if (botao->destacado) {
      glColor3f(0.9f, 0.9f, 0.7f); // Amarelo se mouse em cima
    } else {
      glColor3f(0.95f, 0.95f, 0.95f); // Cinza claro normal
    }
    // Desenhar retângulo do botão
    glBegin(GL_QUADS);
    glVertex2f(botao->x, botao->y);
    glVertex2f(botao->x + botao->largura, botao->y);
    glVertex2f(botao->x + botao->largura, botao->y + botao->altura);
    glVertex2f(botao->x, botao->y + botao->altura);
    glEnd();

    // Contorno do botão
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(botao->x, botao->y);
    glVertex2f(botao->x + botao->largura, botao->y);
    glVertex2f(botao->x + botao->largura, botao->y + botao->altura);
    glVertex2f(botao->x, botao->y + botao->altura);
    glEnd();

    // === DESENHAR TEXTO DO BOTÃO ===
    glColor3f(0.0f, 0.0f, 0.0f); // Texto preto

    // Calcular posição central do texto
    void *font = GLUT_BITMAP_HELVETICA_12;
    int texto_largura = 0;

    // Calcular largura do texto
    for (char *c = botao->texto; *c != '\0'; c++) {
      texto_largura += glutBitmapWidth(font, *c);
    }

    // Posição centralizada
    float texto_x = botao->x + (botao->largura - texto_largura) / 2.0f;
    float texto_y = botao->y + (botao->altura / 2.0f) -
                    6; // -6 para centralizar verticalmente

    // Desenhar o texto
    desenhar_texto(texto_x, texto_y, font, botao->texto);
  }
  if (estado_atual.criacao_ou_selecao == MODO_SELECIONAR_POLIGONO |
      estado_atual.criacao_ou_selecao == MODO_SELECIONAR_PONTO) {
    for (int i = 0; i < 4; i++) {
      BotoesOperacoes *botao = &botoes_operacoes[i];
      if (estado_atual.operacao == botao->acao) {
        glColor3f(0.2f, 0.7f, 0.2f); // Verde se ativo
      } else if (botao->destacado) {
        glColor3f(0.9f, 0.9f, 0.7f); // Amarelo se mouse em cima
      } else {
        glColor3f(0.95f, 0.95f, 0.95f); // Cinza claro normal
      }
      // Desenhar retângulo do botão
      glBegin(GL_QUADS);
      glVertex2f(botao->x, botao->y);
      glVertex2f(botao->x + botao->largura, botao->y);
      glVertex2f(botao->x + botao->largura, botao->y + botao->altura);
      glVertex2f(botao->x, botao->y + botao->altura);
      glEnd();

      // Contorno do botão
      glColor3f(0.0f, 0.0f, 0.0f);
      glLineWidth(1.0f);
      glBegin(GL_LINE_LOOP);
      glVertex2f(botao->x, botao->y);
      glVertex2f(botao->x + botao->largura, botao->y);
      glVertex2f(botao->x + botao->largura, botao->y + botao->altura);
      glVertex2f(botao->x, botao->y + botao->altura);
      glEnd();

      // === DESENHAR TEXTO DO BOTÃO ===
      glColor3f(0.0f, 0.0f, 0.0f); // Texto preto

      // Calcular posição central do texto
      void *font = GLUT_BITMAP_HELVETICA_12;
      int texto_largura = 0;

      // Calcular largura do texto
      for (char *c = botao->texto; *c != '\0'; c++) {
        texto_largura += glutBitmapWidth(font, *c);
      }

      // Posição centralizada
      float texto_x = botao->x + (botao->largura - texto_largura) / 2.0f;
      float texto_y = botao->y + (botao->altura / 2.0f) -
                      6; // -6 para centralizar verticalmente

      // Desenhar o texto
      desenhar_texto(texto_x, texto_y, font, botao->texto);
    }
  }
}
