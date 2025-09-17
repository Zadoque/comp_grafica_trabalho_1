#include "./../includes/menu.h"
#include <GL/glut.h>
// #include <stdio.h>
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

  strcpy(botoes.botoes1[2].texto, "Operar Poligono");
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
void desenhar_botao_generico(void *botao, TipoBotao tipo, int indice) {
  // Casting baseado no tipo
  int x, y, largura, altura;
  char *texto;
  int ativo = 0;
  int destacado = 0;

  switch (tipo) {
  case TIPO_BOTAO1: {
    Botoes1 *b = (Botoes1 *)botao;
    x = b->x;
    y = b->y;
    largura = b->largura;
    altura = b->altura;
    texto = b->texto;
    destacado = b->destacado;
    ativo = (estado_atual.criacao_ou_selecao == b->acao);
    break;
  }
  case TIPO_BOTAO2: {
    Botoes2 *b = (Botoes2 *)botao;
    x = b->x;
    y = b->y;
    largura = b->largura;
    altura = b->altura;
    texto = b->texto;
    destacado = b->destacado;
    ativo = (estado_atual.poligono == b->acao);
    break;
  }
  case TIPO_BOTAO3: {
    Botoes3 *b = (Botoes3 *)botao;
    x = b->x;
    y = b->y;
    largura = b->largura;
    altura = b->altura;
    texto = b->texto;
    destacado = b->destacado;
    ativo = (estado_atual.curva == b->acao);
    break;
  }
  case TIPO_OPERACAO: {
    BotoesOperacoes *b = (BotoesOperacoes *)botao;
    x = b->x;
    y = b->y;
    largura = b->largura;
    altura = b->altura;
    texto = b->texto;
    destacado = b->destacado;
    ativo = (estado_atual.operacao == b->acao);
    break;
  }
  }

  // Código comum de desenho
  if (ativo) {
    glColor3f(0.2f, 0.7f, 0.2f);
  } else if (destacado) {
    glColor3f(0.9f, 0.9f, 0.7f);
  } else {
    glColor3f(0.95f, 0.95f, 0.95f);
  }

   // Desenhar retângulo do botão
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + largura, y);
    glVertex2f(x + largura, y + altura);
    glVertex2f(x, y + altura);
    glEnd();

    // Contorno do botão
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + largura, y);
    glVertex2f(x + largura, y + altura);
    glVertex2f(x, y + altura);
    glEnd();

    // === DESENHAR TEXTO DO BOTÃO ===
    glColor3f(0.0f, 0.0f, 0.0f); // Texto preto

    // Calcular posição central do texto
    void *font = GLUT_BITMAP_HELVETICA_12;
    int texto_largura = 0;

    // Calcular largura do texto
    for (char *c = texto; *c != '\0'; c++) {
      texto_largura += glutBitmapWidth(font, *c);
    }

    // Posição centralizada
    float texto_x = x + (largura - texto_largura) / 2.0f;
    float texto_y = y + (altura / 2.0f) -
                    6; // -6 para centralizar verticalmente

    // Desenhar o texto
    desenhar_texto(texto_x, texto_y, font, texto);
}
void desenhar_botoes_menu() {
    // Botões de criação/seleção
    for (int i = 0; i < 3; i++) {
        desenhar_botao_generico(&botoes.botoes1[i], TIPO_BOTAO1, i);
    }
    
    // Botões de polígono
    for (int i = 0; i < 2; i++) {
        desenhar_botao_generico(&botoes.botoes2[i], TIPO_BOTAO2, i);
    }
    
    // Botões de curva
    for (int i = 0; i < 4; i++) {
        desenhar_botao_generico(&botoes.botoes3[i], TIPO_BOTAO3, i);
    }
    
    // Botões de operação (condicionais)
    if (estado_atual.criacao_ou_selecao == MODO_SELECIONAR_POLIGONO) {
        for (int i = 0; i < 4; i++) {
            desenhar_botao_generico(&botoes_operacoes[i], TIPO_OPERACAO, i);
        }
    } else if(estado_atual.criacao_ou_selecao == MODO_SELECIONAR_PONTO){
      desenhar_botao_generico(&botoes_operacoes[0], TIPO_OPERACAO,0);
      estado_atual.operacao = TRANSLACAO;
    }
}