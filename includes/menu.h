#ifndef MENU_H
#define MENU_H

#include "pontos.h"

typedef enum {
    MODO_CRIAR_PONTO,
    MODO_APAGAR_PONTO,
    MODO_SELECIONAR_PONTO,
    MODO_SELECIONAR_POLIGONO,
    MODO_SELEICIONAR_CURVA
} Criacao_ou_selecao;

typedef enum { TRANSLACAO, ROTACAO, ESCALA, SHEAR, NENHUMA } Operacoes;
typedef enum { MODO_POLIGONO_ABERTO, MODO_POLIGONO_FECHADO } Poligono;
typedef enum {
    MODO_CURVA_HERMITE,
    MODO_CURVA_CATMULLROM,
    MODO_CURVA_BSPLINE,
    MODO_CURVA_BEZIER
} Curva;

typedef struct {
    Criacao_ou_selecao criacao_ou_selecao;
    Poligono poligono;
    Curva curva;
    Operacoes operacao;
    char qtd_pontos_controle[50];
    char qtd_nuvem_pontos[50];
    int qtd_nuvem_pontos_number;
} EstadoAplicacao;

extern EstadoAplicacao estado_atual;
extern int menu_largura;

void inicializar_menu(void);
void atualizar_dimensoes_menu(void);
void menu_mouse_move(int x, int y);
void menu_mouse_button(int button, int state, int x, int y);
void menu_mouse_scroll(int direction, int x, int y);
void menu_render(void);
void menu_render_metricas_fullscreen(void);
void menu_shutdown(void);

#endif
