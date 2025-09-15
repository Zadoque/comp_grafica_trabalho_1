#include "./../includes/opengl.h"
#include <GL/glut.h>
#include <stdio.h>

// Variável global para armazenar cliques
Pontos g_clicks;

// Configurações visuais
static float cor_fundo[3] = {0.2f, 0.3f, 0.4f};

void initGL() {
  glClearColor(cor_fundo[0], cor_fundo[1], cor_fundo[2], 1.0f);
  glPointSize(7.0f);
  glLineWidth(2.0f);
  printf("OpenGL inicializado com sucesso!\n");
}

void AlteraTamanhoJanela(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  int meio_x = (w - 200) / 2;
  int meio_y = h / 2;
  gluOrtho2D(-meio_x, meio_x, -meio_y, meio_y);

  glMatrixMode(GL_MODELVIEW);
}
void desenhar_conteudo_principal() {
  // Desenhar pontos sempre
  glColor3f(1.0f, 0.0f, 0.0f);
  glPointSize(2.0f);
  glBegin(GL_POINTS);
  for (int i = 0; i < g_clicks.quantidade_atual; i++) {
    glVertex2f(g_clicks.data[i].x, g_clicks.data[i].y);
  }
  glEnd();

  switch (estado_atual.poligono) {
  case MODO_POLIGONO_ABERTO:
    if (g_clicks.quantidade_atual >= 2) {
      glColor3f(0.0f, 1.0f, 0.0f);
      glLineWidth(2.0f);
      glBegin(GL_LINE_STRIP);
      for (int i = 0; i < g_clicks.quantidade_atual; i++) {
        glVertex2f(g_clicks.data[i].x, g_clicks.data[i].y);
      }
      glEnd();
    }
    break;
  case MODO_POLIGONO_FECHADO:
    if (g_clicks.quantidade_atual >= 3) {
      glColor3f(0.0f, 1.0f, 0.0f);
      glLineWidth(3.0f);
      glBegin(GL_LINE_LOOP);
      for (int i = 0; i < g_clicks.quantidade_atual; i++) {
        glVertex2f(g_clicks.data[i].x, g_clicks.data[i].y);
      }
      glEnd();
    }
    break;
  }
  // printf("\n O número de pontos a serem desenhados são %zu \n",
  // g_clicks.quantidade_atual);
  /*
    // Desenhar conforme o estado atual
    switch (estado_atual) {
    case MODO_CRIAR_PONTO:
      printf("Modo criar ponto agora\n");
      break;
    case MODO_SELECIONAR_PONTO:
      printf("Modo Selecionar ponto agora\n");
      break;

    case MODO_POLIGONO_ABERTO:

      break;

    case MODO_POLIGONO_FECHADO:


    case MODO_CURVA_HERMITE:
      // Aqui você implementará as curvas depois
      printf("Modo Hermite ativo - implementar curva\n");
      break;

    case MODO_CURVA_BEZIER:
      printf("Modo Bezier ativo - implementar curva\n");
      break;
    case MODO_CURVA_BSPLINE:
      printf("Modo BSpline ativo - implementar curva\n");
      break;
    case MODO_CURVA_CATMULLROM:
      printf("Modo Hermite ativo - implementar curva\n");
      break;
    default:
      printf("Modo incorreto selceionado");
      break;
    }
      */
}

void display() {
  int largura_janela = glutGet(GLUT_WINDOW_WIDTH);
  int altura_janela = glutGet(GLUT_WINDOW_HEIGHT);
  int largura_desenho = largura_janela - menu_largura;

  glClear(GL_COLOR_BUFFER_BIT);

  // === PARTE 1: DESENHAR ÁREA PRINCIPAL ===
  glViewport(0, 0, largura_desenho, altura_janela);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D((int)(largura_desenho / 2 * (-1)), (int)(largura_desenho / 2),
             (int)(altura_janela / 2 * (-1)), (int)(altura_janela / 2));
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Desenhar eixos
  glColor3f(0.5f, 0.5f, 0.5f);
  glBegin(GL_LINES);
  glVertex2f((float)(largura_desenho) / 2 * (-1), 0);
  glVertex2f((float)(largura_desenho) / 2, 0);
  glVertex2f(0, (float)(altura_janela) / 2 * (-1));
  glVertex2f(0, (float)(altura_janela) / 2);
  glEnd();

  // Desenhar pontos e formas conforme o estado atual
  desenhar_conteudo_principal();

  // === PARTE 2: DESENHAR MENU LATERAL ===
  glViewport(largura_desenho, 0, menu_largura, altura_janela);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, menu_largura, 0, altura_janela);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Fundo do menu
  glColor3f(0.8f, 0.8f, 0.8f);
  glBegin(GL_QUADS);
  glVertex2f(0, 0);
  glVertex2f(menu_largura, 0);
  glVertex2f(menu_largura, altura_janela);
  glVertex2f(0, altura_janela);
  glEnd();

  // Desenhar botões
  desenhar_botoes_menu();

  glFlush();
}

int traduzCoordenadaX(int x) {
  int largura = (glutGet(GLUT_WINDOW_WIDTH) - menu_largura) / 2;
  return (x < largura) ? (largura - x) * (-1) : x - largura;
}

int traduzCoordenadaY(int y) {
  int altura = glutGet(GLUT_WINDOW_HEIGHT) / 2;
  return (y < altura) ? (altura - y) : (y - altura) * (-1);
}

void processar_clique_desenho(int x, int y) {
  // Converter coordenadas para o sistema cartesiano
  x = traduzCoordenadaX(x);
  y = traduzCoordenadaY(y);
  if (estado_atual.criacao_ou_selecao == MODO_CRIAR_PONTO) {
    pontos_push(&g_clicks, (float)x, (float)y);
    // printf("Ponto adicionado: (%.1f, %.1f)\n", (float)x, (float)y);
    glutPostRedisplay();
  } else if (estado_atual.criacao_ou_selecao == MODO_SELECIONAR_PONTO) {
  }
  // Aqui você adicionará lógica para outros modos depois
}

void processar_clique_menu(int x, int y) {
  int altura_janela = glutGet(GLUT_WINDOW_HEIGHT);
  y = altura_janela - y; // Inverter Y (GLUT usa origem superior esquerda)
  if (y <= 190) {
    for (int i = 0; i < 3; i++) {
      Botoes1 *botao = &botoes.botoes1[i];
      // Verificar se clique foi dentro do botão
      if (x >= botao->x && x <= botao->x + botao->largura && y >= botao->y &&
          y <= botao->y + botao->altura) {

        estado_atual.criacao_ou_selecao = botao->acao;
        if(estado_atual.criacao_ou_selecao == MODO_CRIAR_PONTO){
          estado_atual.operacao = NENHUMA;
        }
        printf("Estado alterado para: %d\n", estado_atual.criacao_ou_selecao);
        glutPostRedisplay();
        break;
      }
    }
  } else if (y <= 340) {
    for (int i = 0; i < 2; i++) {
      Botoes2 *botao = &botoes.botoes2[i];
      // Verificar se clique foi dentro do botão
      if (x >= botao->x && x <= botao->x + botao->largura && y >= botao->y &&
          y <= botao->y + botao->altura) {

        estado_atual.poligono = botao->acao;
        printf("Estado alterado para: %d\n", estado_atual.poligono);
        glutPostRedisplay();
        break;
      }
    }
  } else if( y <= 590){
    for (int i = 0; i < 4; i++) {
      Botoes3 *botao = &botoes.botoes3[i];
      // Verificar se clique foi dentro do botão
      if (x >= botao->x && x <= botao->x + botao->largura && y >= botao->y &&
          y <= botao->y + botao->altura) {

        estado_atual.curva = botao->acao;
        printf("Estado alterado para: %d\n", estado_atual.curva);
        glutPostRedisplay();
        break;
      }
    }
  } else if(estado_atual.criacao_ou_selecao != MODO_CRIAR_PONTO){
    for (int i = 0; i < 4; i++) {
      BotoesOperacoes *botao = &botoes_operacoes[i];
      // Verificar se clique foi dentro do botão
      if (x >= botao->x && x <= botao->x + botao->largura && y >= botao->y &&
          y <= botao->y + botao->altura) {

        estado_atual.operacao= botao->acao;
        printf("Estado alterado para: %d\n", estado_atual.curva);
        glutPostRedisplay();
        break;
      }
    }
  }
}

void onMouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    int largura_janela = glutGet(GLUT_WINDOW_WIDTH);
    int largura_desenho = largura_janela - menu_largura;

    if (x < largura_desenho) {
      // Clique na área de desenho
      processar_clique_desenho(x, y);
    } else {
      // Clique no menu
      processar_clique_menu(x - largura_desenho, y);
    }
  }
}

void onMouseMove(int x, int y) {
  int largura_janela = glutGet(GLUT_WINDOW_WIDTH);
  int largura_desenho = largura_janela - menu_largura;

  if (x >= largura_desenho) {
    // Mouse no menu
    int menu_x = x - largura_desenho;
    int menu_y = glutGet(GLUT_WINDOW_HEIGHT) - y;

    for (int i = 0; i < 3; i++) {
      Botoes1 *botao = &botoes.botoes1[i];
      botao->destacado =
          (menu_x >= botao->x && menu_x <= botao->x + botao->largura &&
           menu_y >= botao->y && menu_y <= botao->y + botao->altura);
    }
    for (int i = 0; i < 2; i++) {
      Botoes2 *botao = &botoes.botoes2[i];
      botao->destacado =
          (menu_x >= botao->x && menu_x <= botao->x + botao->largura &&
           menu_y >= botao->y && menu_y <= botao->y + botao->altura);
    }
    for (int i = 0; i < 4; i++) {
      Botoes3 *botao = &botoes.botoes3[i];
      botao->destacado =
          (menu_x >= botao->x && menu_x <= botao->x + botao->largura &&
           menu_y >= botao->y && menu_y <= botao->y + botao->altura);
    }
    if(estado_atual.criacao_ou_selecao != MODO_CRIAR_PONTO){
      for (int i = 0; i < 4; i++) {
      BotoesOperacoes *botao = &botoes_operacoes[i];
      botao->destacado =
          (menu_x >= botao->x && menu_x <= botao->x + botao->largura &&
           menu_y >= botao->y && menu_y <= botao->y + botao->altura);
    }
    }
    glutPostRedisplay();
  }
}
