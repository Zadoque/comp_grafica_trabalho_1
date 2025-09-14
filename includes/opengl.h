#ifndef OPENGL_H
#define OPENGL_H

#include "pontos.h"

// Variável global para pontos clicados
extern Pontos g_clicks;

// Funções OpenGL/GLUT
void initGL();
void AlteraTamanhoJanela(int w, int h);
void display();
void onMouse(int button, int state, int x, int y);
int traduzCoordenadaX(int x);
int traduzCoordenadaY(int y);

#endif
