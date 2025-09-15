#ifndef OPENGL_H
#define OPENGL_H

#include "pontos.h"  // Para acessar estruturas de pontos
#include "menu.h"    // Para acessar funcionalidades do menu

// Variável global de pontos
extern Pontos g_clicks;

// Funções OpenGL existentes
void initGL();
void AlteraTamanhoJanela(int w, int h);
void display();
void onMouse(int button, int state, int x, int y);
void onMouseMove(int x, int y);

// Funções auxiliares
void desenhar_conteudo_principal();
void processar_clique_desenho(int x, int y);
void processar_clique_menu(int x, int y) ;
int traduzCoordenadaX(int x);
int traduzCoordenadaY(int y);

#endif

