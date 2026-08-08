#ifndef OPENGL_H
#define OPENGL_H

#include "pontos.h"
#include "vetor.h"
#include "menu.h"
#include "aabb.h"

#include "curvas/hermite.h"
#include "curvas/bezier.h"
#include "curvas/bspline.h"
#include "curvas/catmullrom.h"
#include "transformacoes.h"

extern Pontos g_clicks;
extern Pontos g_curva_atual;
extern Pontos g_nuvem_pontos;

typedef struct {
    int indice;
    int selecionado;
} Selecao;

typedef struct {
    int seg_curva;
    float t;
    Curva curva_selecionada;
} Selecao_curva;

void initGL(void);
void AlteraTamanhoJanela(int w, int h);
void display(void);
void desenhar_conteudo_principal(void);
void desenhar_aabbs(AABB_vec *v);
void gerar_curva_selecionada(void);
void desenhar_curva_atual(void);
void onMouse(int button, int state, int x, int y);
void onMouseMove(int x, int y);
void onMotion(int x, int y);
int traduzCoordenadaX(int x);
int traduzCoordenadaY(int y);

#endif
