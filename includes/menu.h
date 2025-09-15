#ifndef MENU_H
#define MENU_H

#include "pontos.h"  // Menu precisa conhecer estruturas de pontos

// Estados da aplicação
typedef enum {
    MODO_CRIAR_PONTO,
    MODO_SELECIONAR_PONTO,
    MODO_SELECIONAR_POLIGONO,
    MODO_POLIGONO_ABERTO,
    MODO_POLIGONO_FECHADO,
    MODO_CURVA_HERMITE,
    MODO_CURVA_CATMULLROM,
    MODO_CURVA_BSPLINE,
    MODO_CURVA_BEZIER
} EstadoAplicacao;

// Estrutura dos botões
typedef struct {
    int x, y, largura, altura;
    char texto[20];
    EstadoAplicacao acao;
    int destacado;
} BotaoMenu;

// Variáveis globais do menu
extern EstadoAplicacao estado_atual;
extern BotaoMenu botoes_menu[9];
extern int menu_largura;

// Funções do menu
void inicializar_menu();
void desenhar_texto(float x, float y, void* font, char* texto);
void desenhar_botoes_menu();
void processar_clique_menu(int x, int y);
void processar_mouse_move_menu(int x, int y);

#endif
