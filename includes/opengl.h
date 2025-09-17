#ifndef OPENGL_H
#define OPENGL_H

#include "pontos.h"
#include "menu.h"

// Incluir todas as curvas
#include "curvas/hermite.h"
#include "curvas/bezier.h"
#include "curvas/bspline.h"
#include "curvas/catmullrom.h"
#include "operacoes.h"  




// Variáveis globais
extern Pontos g_clicks;
extern Pontos g_curva_atual;  // Nova: para armazenar pontos da curva

typedef struct {
      int indice;
      int selecionado;
} Selecao;
// Funções existentes...
void initGL();
void AlteraTamanhoJanela(int w, int h);
void display();
void desenhar_conteudo_principal();

// Novas funções para curvas
void gerar_curva_selecionada();
void desenhar_curva_atual();

// Resto das suas funções...
void onMouse(int button, int state, int x, int y);
void onMouseMove(int x, int y);
void onMotion(int x, int y);
int traduzCoordenadaX(int x);
int traduzCoordenadaY(int y);

#endif
