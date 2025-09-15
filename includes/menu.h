#ifndef MENU_H
#define MENU_H

#include "pontos.h" // Menu precisa conhecer estruturas de pontos

// Estados da aplicação
typedef enum {
  MODO_CRIAR_PONTO,
  MODO_SELECIONAR_PONTO,
  MODO_SELECIONAR_POLIGONO,
} Criacao_ou_selecao;

typedef enum { TRANSLACAO, ROTACAO, ESCALA, ESPALHADO, SHEAR, NENHUMA } Operacoes;

typedef enum { MODO_POLIGONO_ABERTO, MODO_POLIGONO_FECHADO } Poligono;

typedef enum {
  MODO_CURVA_HERMITE,
  MODO_CURVA_CATMULLROM,
  MODO_CURVA_BSPLINE,
  MODO_CURVA_BEZIER
} Curva;
typedef enum { TIPO_BOTAO1, TIPO_BOTAO2, TIPO_BOTAO3, TIPO_OPERACAO } TipoBotao;

typedef struct {
  Criacao_ou_selecao criacao_ou_selecao;
  Poligono poligono;
  Curva curva;
  Operacoes operacao;
} EstadoAplicacao;

// Estrutura dos botões
typedef struct {
  int x, y, largura, altura;
  char texto[20];
  Criacao_ou_selecao acao;
  int destacado;
} Botoes1;

typedef struct {
  int x, y, largura, altura;
  char texto[20];
  Poligono acao;
  int destacado;
} Botoes2;

typedef struct {
  int x, y, largura, altura;
  char texto[20];
  Curva acao;
  int destacado;
} Botoes3;

typedef struct {
  int x, y, largura, altura;
  char texto[20];
  Operacoes acao;
  int destacado;
} BotoesOperacoes;

typedef struct {
  Botoes1 botoes1[3];
  Botoes2 botoes2[2];
  Botoes3 botoes3[4];
} Botoes;

// Variáveis globais do menu
extern EstadoAplicacao estado_atual;
extern Botoes botoes;
extern BotoesOperacoes botoes_operacoes[4];
extern int menu_largura;

// Funções do menu
void inicializar_menu();
void desenhar_texto(float x, float y, void *font, char *texto);
void desenhar_botao_generico(void* botao, TipoBotao tipo, int indice);
void desenhar_botoes_menu();
void processar_clique_menu(int x, int y);
void processar_mouse_move_menu(int x, int y);

#endif
