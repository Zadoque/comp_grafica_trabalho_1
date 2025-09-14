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

  int meio_x = w / 2;
  int meio_y = h / 2;
  gluOrtho2D(-meio_x, meio_x, -meio_y, meio_y);

  glMatrixMode(GL_MODELVIEW);
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);

  // Desenhar eixos cartesianos
  glColor3f(1.0f, 0.0f, 0.0f);
  glBegin(GL_LINES);
  // Eixo X
  glVertex2f((float)(glutGet(GLUT_WINDOW_WIDTH)) / 2 * (-1), 0.0f);
  glVertex2f((float)(glutGet(GLUT_WINDOW_WIDTH)) / 2, 0.0f);
  // Eixo Y
  glVertex2f(0.0f, (float)(glutGet(GLUT_WINDOW_HEIGHT)) / 2);
  glVertex2f(0.0f, (float)(glutGet(GLUT_WINDOW_HEIGHT)) / 2 * (-1));
  glEnd();

  // Desenhar pontos clicados
  glColor3f(0.0f, 0.0f, 1.0f);
  glBegin(GL_POINTS);
  for (int i = 0; i < g_clicks.quantidade_atual; i++) {
    glVertex2f(g_clicks.data[i].x, g_clicks.data[i].y);
  }
  glEnd();
  if (g_clicks.quantidade_atual >= 3) {
    glColor3f(0.0f, 1.0f, 0.0f); // Verde
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP); // Conecta todos os pontos e fecha automaticamente
    for (int i = 0; i < g_clicks.quantidade_atual; i++) {
      glVertex2f(g_clicks.data[i].x, g_clicks.data[i].y);
    }
    glEnd();
  }
  glFlush();
}

int traduzCoordenadaX(int x) {
  int largura = glutGet(GLUT_WINDOW_WIDTH) / 2;
  return (x < largura) ? (largura - x) * (-1) : x - largura;
}

int traduzCoordenadaY(int y) {
  int altura = glutGet(GLUT_WINDOW_HEIGHT) / 2;
  return (y < altura) ? (altura - y) : (y - altura) * (-1);
}

void onMouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    x = traduzCoordenadaX(x);
    y = traduzCoordenadaY(y);
    pontos_push(&g_clicks, (float)x, (float)y);

    // Debug dos pontos
    printf("Pontos atuais (%zu):\n", g_clicks.quantidade_atual);
    for (int i = 0; i < g_clicks.quantidade_atual; i++) {
      printf("  [%d] (%.1f, %.1f)\n", i, g_clicks.data[i].x,
             g_clicks.data[i].y);
    }

    glutPostRedisplay();
  }
}
