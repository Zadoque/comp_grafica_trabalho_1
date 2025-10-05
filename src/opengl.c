#include "./../includes/opengl.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdio.h>

// Variáveis globais
Pontos g_clicks;
Pontos g_curva_atual; // Nova: armazenar pontos da curva gerada
Selecao selecao_ponto;
int selecao_poligono;
ponto centro;

// Configurações visuais
static float cor_fundo[3] = {0.2f, 0.3f, 0.4f};

void initGL() {
  glClearColor(cor_fundo[0], cor_fundo[1], cor_fundo[2], 1.0f);
  glPointSize(7.0f);
  glLineWidth(2.0f);

  // Inicializar estrutura para curva
  pontos_init(&g_curva_atual, 100);
  selecao_ponto.selecionado = 0;
  selecao_ponto.indice = 0;
  selecao_poligono = 0;
  centro.point[0] = 0;
  centro.point[1] = 0;
  centro.point[2] = 1;
  printf("OpenGL inicializado com sucesso!\n");
}

void gerar_curva_selecionada() {
  if (g_clicks.quantidade_atual < 3)
    return;
  int poligono = (estado_atual.poligono == MODO_POLIGONO_FECHADO) ? 1 : 0;
  switch (estado_atual.curva) {
  case MODO_CURVA_HERMITE:
    gerar_curva_hermite(&g_clicks, &g_curva_atual, poligono);
    break;

  case MODO_CURVA_BEZIER:
    if (g_clicks.quantidade_atual >= 4) {
      gerar_curva_bezier(&g_clicks, &g_curva_atual, poligono);
    }
    break;

  case MODO_CURVA_BSPLINE:
    if (g_clicks.quantidade_atual >= 4) {
      gerar_curva_bspline(&g_clicks, &g_curva_atual);
    }
    break;

  case MODO_CURVA_CATMULLROM:
    if (g_clicks.quantidade_atual >= 4) {
      gerar_curva_catmullrom(&g_clicks, &g_curva_atual);
    }
    break;
  }
}

void desenhar_curva_atual() {
  if ((g_clicks.quantidade_atual < 4 ))
    return;

  // Definir cor da curva baseada no tipo
  switch (estado_atual.curva) {
  case MODO_CURVA_HERMITE:
    glColor3f(1.0f, 0.0f, 1.0f); // Magenta
    break;
  case MODO_CURVA_BEZIER:
    glColor3f(0.0f, 0.0f, 1.0f); // Azul
    break;
  case MODO_CURVA_BSPLINE:
    glColor3f(1.0f, 0.5f, 0.0f); // Laranja
    break;
  case MODO_CURVA_CATMULLROM:
    glColor3f(0.5f, 1.0f, 0.5f); // Verde claro
    break;
  }
  gerar_curva_selecionada();
  glLineWidth(3.0f);
  glBegin(GL_LINE_STRIP);
  for (int i = 0; i < g_curva_atual.quantidade_atual; i++) {
    glVertex2f(g_curva_atual.data[i].point[0], g_curva_atual.data[i].point[1]);
  }
  glEnd();
}

// Suas funções existentes...
void AlteraTamanhoJanela(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  int meio_x = (w - 200) / 2;
  int meio_y = h / 2;
  gluOrtho2D(-meio_x, meio_x, -meio_y, meio_y);

  glMatrixMode(GL_MODELVIEW);
}
void desenhar_centro_poligono() {
  if (g_clicks.quantidade_atual < 2)
    return;

  // Usar função robusta da geometria.c

  // Desenhar cruz no centro
  glColor3f(1.0f, 1.0f, 0.0f); // Magenta
  glLineWidth(3.0f);

  glBegin(GL_LINES);
  glVertex2f(centro.point[0] - 10, centro.point[1]);
  glVertex2f(centro.point[0] + 10, centro.point[1]);
  glVertex2f(centro.point[0], centro.point[1] - 10);
  glVertex2f(centro.point[0], centro.point[1] + 10);
  glEnd();

  glPointSize(8.0f);
  glBegin(GL_POINTS);
  glVertex2f(centro.point[0], centro.point[1]);
  glEnd();
}

void desenhar_conteudo_principal() {
  // Desenhar pontos de controle
  glColor3f(1.0f, 0.0f, 0.0f); // Vermelho
  glPointSize(8.0f);
  glBegin(GL_POINTS);
  for (int i = 0; i < g_clicks.quantidade_atual; i++) {
    glVertex2f(g_clicks.data[i].point[0], g_clicks.data[i].point[1]);
  }
  glEnd();
  glColor3f(0.0f, 1.0f, 0.0f); // Verde
  glLineWidth(2.0f);

  switch (estado_atual.poligono) {
  case MODO_POLIGONO_FECHADO:
    if (g_clicks.quantidade_atual <= 2) {
      glBegin(GL_LINE_STRIP);
      for (int i = 0; i < g_clicks.quantidade_atual; i++) {
        glVertex2f(g_clicks.data[i].point[0], g_clicks.data[i].point[1]);
      }
      glEnd();
    } else {
      glBegin(GL_LINE_LOOP);
      for (int i = 0; i < g_clicks.quantidade_atual; i++) {
        glVertex2f(g_clicks.data[i].point[0], g_clicks.data[i].point[1]);
      }
      glEnd();
    }
    break;
  case MODO_POLIGONO_ABERTO:
    if (g_clicks.quantidade_atual >= 2) {
      glBegin(GL_LINE_STRIP);
      for (int i = 0; i < g_clicks.quantidade_atual; i++) {
        glVertex2f(g_clicks.data[i].point[0], g_clicks.data[i].point[1]);
      }
      glEnd();
    }
    break;
  }

  desenhar_centro_poligono();
  desenhar_curva_atual();
}

void display() {
  int largura_janela = glutGet(GLUT_WINDOW_WIDTH);
  int altura_janela = glutGet(GLUT_WINDOW_HEIGHT);
  int largura_desenho = largura_janela - menu_largura;

  glClear(GL_COLOR_BUFFER_BIT);

  //  PARTE 1: DESENHAR ÁREA PRINCIPAL
  glViewport(0, 0, largura_desenho, altura_janela);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D((float)(largura_desenho) / 2 * (-1), (float)(largura_desenho) / 2,
             (float)(altura_janela) / 2 * (-1), (float)(altura_janela) / 2);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Desenhar eixos
  glColor3f(0.5f, 0.5f, 0.5f);
  glBegin(GL_LINES);
  glVertex2f(-(int)(largura_desenho / 2), 0);
  glVertex2f((int)(largura_desenho / 2), 0);
  glVertex2f(0, -(int)(altura_janela / 2));
  glVertex2f(0, (int)(altura_janela / 2));
  glEnd();

  // Desenhar conteúdo
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

  desenhar_botoes_menu();
  glFlush();
}

void processar_clique_desenho(int x, int y) {
  x = traduzCoordenadaX(x);
  y = traduzCoordenadaY(y);

  switch (estado_atual.criacao_ou_selecao) {
  case MODO_CRIAR_PONTO:
    pontos_push(&g_clicks, (float)x, (float)y);
    calcular_centro_medio(&centro, &g_clicks);
    glutPostRedisplay();
    break;
  case MODO_SELECIONAR_PONTO:
    if (g_clicks.quantidade_atual >= 1) {
      for (int i = 0; i < g_clicks.quantidade_atual; i++) {
        ponto mouse;
        mouse.point[0] = x;
        mouse.point[1] = y;
        if (calcula_distancia(mouse, g_clicks.data[i]) < 3) {
          selecao_ponto.selecionado = 1;
          selecao_ponto.indice = i;
        }
      }
    }
    break;
  case MODO_SELECIONAR_POLIGONO:
    if (g_clicks.quantidade_atual >= 2) {
      ponto mouse;
      mouse.point[0] = x;
      mouse.point[1] = y;
      if ((calcula_distancia(centro, mouse)) < 3) {
        selecao_poligono = 1;
      }
      break;
    }
  }
}
int traduzCoordenadaX(int x) {
  int largura = (glutGet(GLUT_WINDOW_WIDTH) - menu_largura) / 2;
  return (x < largura) ? (largura - x) * (-1) : x - largura;
}

int traduzCoordenadaY(int y) {
  int altura = glutGet(GLUT_WINDOW_HEIGHT) / 2;
  return (y < altura) ? (altura - y) : (y - altura) * (-1);
}

void verificar_clique_botao_generico(void *botao, TipoBotao tipo, int x,
                                     int y) {
  int x_botao, y_botao, altura_botao, largura_botao, mudanca;
  Criacao_ou_selecao criacao_ou_selecao_ponto;
  Poligono poligono;
  Curva curva;
  Operacoes operacao;
  // printf("função começa\n");
  switch (tipo) {
  case TIPO_BOTAO1: {
    Botoes1 *b = (Botoes1 *)botao;
    x_botao = b->x;
    y_botao = b->y;
    altura_botao = b->altura;
    largura_botao = b->largura;
    criacao_ou_selecao_ponto = b->acao;
    mudanca = 0;
    break;
  }
  case TIPO_BOTAO2: {
    Botoes2 *b = (Botoes2 *)botao;
    x_botao = b->x;
    y_botao = b->y;
    altura_botao = b->altura;
    largura_botao = b->largura;
    poligono = b->acao;
    mudanca = 1;
    break;
  }
  case TIPO_BOTAO3: {
    Botoes3 *b = (Botoes3 *)botao;
    x_botao = b->x;
    y_botao = b->y;
    altura_botao = b->altura;
    largura_botao = b->largura;
    curva = b->acao;
    mudanca = 2;
    break;
  }
  case TIPO_OPERACAO: {
    BotoesOperacoes *b = (BotoesOperacoes *)botao;
    x_botao = b->x;
    y_botao = b->y;
    altura_botao = b->altura;
    largura_botao = b->largura;
    operacao = b->acao;
    mudanca = 3;
    break;
  }
  }
  //  Verificar se clique foi dentro do botão
  if (x >= x_botao && x <= x_botao + largura_botao && y >= y_botao &&
      y <= y_botao + altura_botao) {
    switch (mudanca) {
    case 0:
      estado_atual.criacao_ou_selecao = criacao_ou_selecao_ponto;
      if (estado_atual.criacao_ou_selecao == MODO_CRIAR_PONTO) {
        estado_atual.operacao = NENHUMA;
      }
      break;
    case 1:
      estado_atual.poligono = poligono;
      break;
    case 2:
      estado_atual.curva = curva;
      printf("Agora é %u\n", estado_atual.curva);
      break;
    case 3:
      estado_atual.operacao = operacao;
      if (operacao == ROTACAO) {
        rotacionar(&g_clicks, centro, 30.0);
      }
      break;
    }
    glutPostRedisplay();
  }
}
void processar_clique_menu(int x, int y) {
  int altura_janela = glutGet(GLUT_WINDOW_HEIGHT);
  y = altura_janela - y; // Inverter Y (GLUT usa origem superior esquerda)
  if (y <= 190) {
    for (int i = 0; i < 3; i++) {
      verificar_clique_botao_generico(&botoes.botoes1[i], TIPO_BOTAO1, x, y);
    }
  } else if (y <= 340) {
    for (int i = 0; i < 2; i++) {
      verificar_clique_botao_generico(&botoes.botoes2[i], TIPO_BOTAO2, x, y);
    }
  } else if (y <= 590) {
    for (int i = 0; i < 4; i++) {
      verificar_clique_botao_generico(&botoes.botoes3[i], TIPO_BOTAO3, x, y);
    }
  } else if (estado_atual.criacao_ou_selecao != MODO_CRIAR_PONTO) {
    for (int i = 0; i < 4; i++) {
      verificar_clique_botao_generico(&botoes_operacoes[i], TIPO_OPERACAO, x,
                                      y);
    }
  }
}

void onMouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON) {
    if (state == GLUT_DOWN) {
      int largura_janela = glutGet(GLUT_WINDOW_WIDTH);
      int largura_desenho = largura_janela - menu_largura;
      if (x < largura_desenho) {
        // Clique na área de desenho
        processar_clique_desenho(x, y);
      } else {
        // Clique no menu
        processar_clique_menu(x - largura_desenho, y);
      }
    } else {
      if (selecao_ponto.selecionado) {
        selecao_ponto.selecionado = 0;
        selecao_ponto.indice = 0;
      }
      if (selecao_poligono) {
        selecao_poligono = 0;
      }
    }
  }
  if ((button == 3 || button == 4) && estado_atual.operacao == ESCALA) {
    ponto mouse;
    mouse.point[0] = traduzCoordenadaX(x);
    mouse.point[1] = traduzCoordenadaY(y);
    if (calcula_distancia(mouse, centro) < 3) {
      if (button == 3) {
        aumentar_escala(&g_clicks, centro);
      } else {
        diminuir_escala(&g_clicks, centro);
      }
      calcular_centro_medio(&centro, &g_clicks);
      glutPostRedisplay();
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
    if (estado_atual.criacao_ou_selecao != MODO_CRIAR_PONTO) {
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

void onMotion(int x, int y) {
  x = traduzCoordenadaX(x);
  y = traduzCoordenadaY(y);
  if (selecao_ponto.selecionado) {
    g_clicks.data[selecao_ponto.indice].point[0] = x;
    g_clicks.data[selecao_ponto.indice].point[1] = y;
    calcular_centro_medio(&centro, &g_clicks);
    glutPostRedisplay(); // Redesenhar se necessário
  } else if (selecao_poligono == 1 &&
             estado_atual.criacao_ou_selecao == MODO_SELECIONAR_POLIGONO) {
    translacao_com_mouse(&g_clicks, centro, x, y);
    calcular_centro_medio(&centro, &g_clicks);
    glutPostRedisplay();
  }
}