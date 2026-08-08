#ifndef MENU_H
#define MENU_H

#include "pontos.h"

typedef enum { MODO_CRIAR_PONTO, MODO_APAGAR_PONTO, MODO_SELECIONAR_PONTO, MODO_SELECIONAR_POLIGONO, MODO_SELEICIONAR_CURVA } Criacao_ou_selecao;
typedef enum { TRANSLACAO, ROTACAO, ESCALA, SHEAR, NENHUMA } Operacoes;
typedef enum { MODO_POLIGONO_ABERTO, MODO_POLIGONO_FECHADO } Poligono;
typedef enum { MODO_CURVA_HERMITE, MODO_CURVA_CATMULLROM, MODO_CURVA_BSPLINE, MODO_CURVA_BEZIER } Curva;

typedef struct { Criacao_ou_selecao criacao_ou_selecao; Poligono poligono; Curva curva; Operacoes operacao; char qtd_pontos_controle[50]; char qtd_nuvem_pontos[50]; int qtd_nuvem_pontos_number; } EstadoAplicacao;
extern EstadoAplicacao estado_atual;
extern int menu_largura;

/* Compatibilidade de tipos antigos; a renderizacao efetiva usa MicroUI. */
typedef enum { TIPO_BOTAO1, TIPO_BOTAO2, TIPO_BOTAO3, TIPO_OPERACAO } TipoBotao;
typedef struct { int x,y,largura,altura; char texto[20]; Criacao_ou_selecao acao; int destacado; } Botoes1;
typedef struct { int x,y,largura,altura; char texto[20]; Poligono acao; int destacado; } Botoes2;
typedef struct { int x,y,largura,altura; char texto[20]; Curva acao; int destacado; } Botoes3;
typedef struct { int x,y,largura,altura; char texto[20]; Operacoes acao; int destacado; } BotoesOperacoes;
typedef struct { Botoes1 botoes1[5]; Botoes2 botoes2[2]; Botoes3 botoes3[4]; } Botoes;
extern Botoes botoes;
extern BotoesOperacoes botoes_operacoes[4];

void inicializar_menu(void);
void atualizar_dimensoes_menu(void);
void menu_mouse_move(int x, int y);
void menu_mouse_button(int button, int state, int x, int y);
void menu_mouse_scroll(int direction, int x, int y);
void menu_render(void);
int menu_bloqueia_desenho(void);
void menu_shutdown(void);
void menu_mouse_dispatch(int button, int state, int x, int y);
void menu_passive_motion_dispatch(int x, int y);
void menu_motion_dispatch(int x, int y);
void desenhar_botoes_menu(void);
void desenhar_info(void);

#endif
