#include "./../includes/menu.h"
#include <GL/glut.h>
#include <string.h>
#include <stdio.h>
EstadoAplicacao estado_atual = {MODO_CRIAR_PONTO, MODO_POLIGONO_ABERTO,
                                MODO_CURVA_HERMITE, NENHUMA, "0", "0"};

int menu_largura = 200;
float menu_escala_fonte = 0.1f;
float menu_altura_fonte = 12.0f;
float info_escala_fonte = 5.0f;
void *menu_fonte_stroke = GLUT_STROKE_ROMAN;
Botoes botoes;
BotoesOperacoes botoes_operacoes[4];
//description descricao[2]; adicionar depois quando for fazer o hover para dizer o que são os números
information info;
// Função para calcular dimensões dinâmicas
void atualizar_dimensoes_menu() {
    int altura_janela = glutGet(GLUT_WINDOW_HEIGHT);
    int largura_janela = glutGet(GLUT_WINDOW_WIDTH);
    printf("\n\tA largura da janela é %d e a altura %d", altura_janela, largura_janela); 
    // Menu ocupa 15% da largura ou mínimo 150px
    menu_largura = (largura_janela * 0.15 < 150) ? 150 : largura_janela * 0.15;
    
    // Margens e espaçamentos proporcionais
    int margem = menu_largura * 0.05;
    int largura_botao = menu_largura - (2 * margem);
    int altura_botao = altura_janela * 0.05; // 5% da altura
    int espacamento = altura_janela * 0.01; // 1% da altura
    
     // === recalcular fonte responsiva ===
    // Usa ~45% da altura do botão como altura visual do texto
    menu_altura_fonte = altura_botao * 0.45f;
    menu_escala_fonte = (largura_botao * 0.09f) / 121.05f;
    info_escala_fonte = (largura_botao * 0.05f ) /121.0f; 
    // === INFORMAÇÃO NO CANTO INFERIOR ESQUERDO PARA SABER QUANTOS PONTOS ESTÃO SENDO USADOS ===
    int y_inicio_info = altura_janela * 0.01;
    // Aqui depois tem que fazer uma função que transforma 1000 em 1k, 1542 em 1.5k, 1 456 987 em 1.46M para garantir que vai caber na tela
      info.x = margem;
    info.y = y_inicio_info;
   

    /*
     Ainda falta implementar descrição *****************
    strcpy(descricao[0].texto, "Numero de pontos de controle que existem na aplicacao");
    descricao[0].x = margem;
    descricao[0].y = y_inicio_info + info.altura + 1;
    descricao[0].altura
    */
    // === BOTÕES DE CRIAÇÃO/SELEÇÃO (Grupo 1) ===
    int y_inicio_grupo1 = altura_janela * 0.05;
    strcpy(botoes.botoes1[0].texto, "Criar Ponto");
    botoes.botoes1[0].x = margem;
    botoes.botoes1[0].y = y_inicio_grupo1;
    botoes.botoes1[0].largura = largura_botao;
    botoes.botoes1[0].altura = altura_botao;
    botoes.botoes1[0].acao = MODO_CRIAR_PONTO;

    strcpy(botoes.botoes1[1].texto, "Selecionar vertice");
    botoes.botoes1[1].x = margem;
    botoes.botoes1[1].y = y_inicio_grupo1 + (altura_botao + espacamento);
    botoes.botoes1[1].largura = largura_botao;
    botoes.botoes1[1].altura = altura_botao;
    botoes.botoes1[1].acao = MODO_SELECIONAR_PONTO;

    strcpy(botoes.botoes1[2].texto, "Operar Poligono");
    botoes.botoes1[2].x = margem;
    botoes.botoes1[2].y = y_inicio_grupo1 + 2 * (altura_botao + espacamento);
    botoes.botoes1[2].largura = largura_botao;
    botoes.botoes1[2].altura = altura_botao;
    botoes.botoes1[2].acao = MODO_SELECIONAR_POLIGONO;

    // === BOTÕES DE POLÍGONO (Grupo 2) ===
    int y_inicio_grupo2 = y_inicio_grupo1 + 3 * (altura_botao + espacamento) + altura_janela * 0.03;
    
    strcpy(botoes.botoes2[0].texto, "Poligono Aberto");
    botoes.botoes2[0].x = margem;
    botoes.botoes2[0].y = y_inicio_grupo2;
    botoes.botoes2[0].largura = largura_botao;
    botoes.botoes2[0].altura = altura_botao;
    botoes.botoes2[0].acao = MODO_POLIGONO_ABERTO;

    strcpy(botoes.botoes2[1].texto, "Poligono Fechado");
    botoes.botoes2[1].x = margem;
    botoes.botoes2[1].y = y_inicio_grupo2 + (altura_botao + espacamento);
    botoes.botoes2[1].largura = largura_botao;
    botoes.botoes2[1].altura = altura_botao;
    botoes.botoes2[1].acao = MODO_POLIGONO_FECHADO;

    // === BOTÕES DE CURVA (Grupo 3) ===
    int y_inicio_grupo3 = y_inicio_grupo2 + 2 * (altura_botao + espacamento) + altura_janela * 0.03;
    
    strcpy(botoes.botoes3[0].texto, "Hermite");
    botoes.botoes3[0].x = margem;
    botoes.botoes3[0].y = y_inicio_grupo3;
    botoes.botoes3[0].largura = largura_botao;
    botoes.botoes3[0].altura = altura_botao;
    botoes.botoes3[0].acao = MODO_CURVA_HERMITE;

    strcpy(botoes.botoes3[1].texto, "Catmull-Rom");
    botoes.botoes3[1].x = margem;
    botoes.botoes3[1].y = y_inicio_grupo3 + (altura_botao + espacamento);
    botoes.botoes3[1].largura = largura_botao;
    botoes.botoes3[1].altura = altura_botao;
    botoes.botoes3[1].acao = MODO_CURVA_CATMULLROM;

    strcpy(botoes.botoes3[2].texto, "B-Spline");
    botoes.botoes3[2].x = margem;
    botoes.botoes3[2].y = y_inicio_grupo3 + 2 * (altura_botao + espacamento);
    botoes.botoes3[2].largura = largura_botao;
    botoes.botoes3[2].altura = altura_botao;
    botoes.botoes3[2].acao = MODO_CURVA_BSPLINE;

    strcpy(botoes.botoes3[3].texto, "Bezier");
    botoes.botoes3[3].x = margem;
    botoes.botoes3[3].y = y_inicio_grupo3 + 3 * (altura_botao + espacamento);
    botoes.botoes3[3].largura = largura_botao;
    botoes.botoes3[3].altura = altura_botao;
    botoes.botoes3[3].acao = MODO_CURVA_BEZIER;

    // === BOTÕES DE OPERAÇÃO (Grupo 4) ===
    int y_inicio_grupo4 = y_inicio_grupo3 + 4 * (altura_botao + espacamento) + altura_janela * 0.03;
    
    strcpy(botoes_operacoes[0].texto, "Translacao");
    botoes_operacoes[0].x = margem;
    botoes_operacoes[0].y = y_inicio_grupo4;
    botoes_operacoes[0].largura = largura_botao;
    botoes_operacoes[0].altura = altura_botao;
    botoes_operacoes[0].acao = TRANSLACAO;

    strcpy(botoes_operacoes[1].texto, "Rotacao");
    botoes_operacoes[1].x = margem;
    botoes_operacoes[1].y = y_inicio_grupo4 + (altura_botao + espacamento);
    botoes_operacoes[1].largura = largura_botao;
    botoes_operacoes[1].altura = altura_botao;
    botoes_operacoes[1].acao = ROTACAO;

    strcpy(botoes_operacoes[2].texto, "Escala");
    botoes_operacoes[2].x = margem;
    botoes_operacoes[2].y = y_inicio_grupo4 + 2 * (altura_botao + espacamento);
    botoes_operacoes[2].largura = largura_botao;
    botoes_operacoes[2].altura = altura_botao;
    botoes_operacoes[2].acao = ESCALA;

    strcpy(botoes_operacoes[3].texto, "Shear");
    botoes_operacoes[3].x = margem;
    botoes_operacoes[3].y = y_inicio_grupo4 + 3 * (altura_botao + espacamento);
    botoes_operacoes[3].largura = largura_botao;
    botoes_operacoes[3].altura = altura_botao;
    botoes_operacoes[3].acao = SHEAR;


}

void inicializar_menu() {
    atualizar_dimensoes_menu();
}

void desenhar_texto(float x, float y, void *font, const char *texto, float escala) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(escala, escala, 1.0f);

    for (const char *c = texto; *c != '\0'; c++) {
        glutStrokeCharacter(font, *c);
    }

    glPopMatrix();
}

void desenhar_botao_generico(void *botao, TipoBotao tipo, int indice) {
    int x, y, largura, altura;
    char *texto;
    int ativo = 0;
    int destacado = 0;

    switch (tipo) {
    case TIPO_BOTAO1: {
        Botoes1 *b = (Botoes1 *)botao;
        x = b->x;
        y = b->y;
        largura = b->largura;
        altura = b->altura;
        texto = b->texto;
        destacado = b->destacado;
        ativo = (estado_atual.criacao_ou_selecao == b->acao);
        break;
    }
    case TIPO_BOTAO2: {
        Botoes2 *b = (Botoes2 *)botao;
        x = b->x;
        y = b->y;
        largura = b->largura;
        altura = b->altura;
        texto = b->texto;
        destacado = b->destacado;
        ativo = (estado_atual.poligono == b->acao);
        break;
    }
    case TIPO_BOTAO3: {
        Botoes3 *b = (Botoes3 *)botao;
        x = b->x;
        y = b->y;
        largura = b->largura;
        altura = b->altura;
        texto = b->texto;
        destacado = b->destacado;
        ativo = (estado_atual.curva == b->acao);
        break;
    }
    case TIPO_OPERACAO: {
        BotoesOperacoes *b = (BotoesOperacoes *)botao;
        x = b->x;
        y = b->y;
        largura = b->largura;
        altura = b->altura;
        texto = b->texto;
        destacado = b->destacado;
        ativo = (estado_atual.operacao == b->acao);
        break;
    }
    }

    if (ativo) {
        glColor3f(0.2f, 0.7f, 0.2f);
    } else if (destacado) {
        glColor3f(0.9f, 0.9f, 0.7f);
    } else {
        glColor3f(0.95f, 0.95f, 0.95f);
    }

    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + largura, y);
    glVertex2f(x + largura, y + altura);
    glVertex2f(x, y + altura);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + largura, y);
    glVertex2f(x + largura, y + altura);
    glVertex2f(x, y + altura);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);

    float texto_largura = calcular_largura_texto_stroke(menu_fonte_stroke, texto, menu_escala_fonte);
    float texto_altura = calcular_altura_fonte_stroke(menu_escala_fonte);

    float texto_x = x + (largura - texto_largura) / 2.0f;
    float texto_y = y + (altura - texto_altura) / 2.0f;

    desenhar_texto(texto_x, texto_y, menu_fonte_stroke, texto, menu_escala_fonte);
}

void desenhar_botoes_menu() {
    // Botões de criação/seleção
    for (int i = 0; i < 3; i++) {
        desenhar_botao_generico(&botoes.botoes1[i], TIPO_BOTAO1, i);
    }
    
    // Botões de polígono
    for (int i = 0; i < 2; i++) {
        desenhar_botao_generico(&botoes.botoes2[i], TIPO_BOTAO2, i);
    }
    
    // Botões de curva
    for (int i = 0; i < 4; i++) {
        desenhar_botao_generico(&botoes.botoes3[i], TIPO_BOTAO3, i);
    }
    
    // Botões de operação (condicionais)
    if (estado_atual.criacao_ou_selecao == MODO_SELECIONAR_POLIGONO) {
        for (int i = 0; i < 4; i++) {
            desenhar_botao_generico(&botoes_operacoes[i], TIPO_OPERACAO, i);
        }
    } else if(estado_atual.criacao_ou_selecao == MODO_SELECIONAR_PONTO){
      desenhar_botao_generico(&botoes_operacoes[0], TIPO_OPERACAO,0);
      estado_atual.operacao = TRANSLACAO;
    }
}

float calcular_largura_texto_stroke(void *font, const char *texto, float escala) {
    float largura = 0.0f;
    for (const char *c = texto; *c != '\0'; c++) {
        largura += glutStrokeWidth(font, *c);
    }
    return largura * escala;
}

float calcular_altura_fonte_stroke(float escala) {
    // Altura aproximada da GLUT_STROKE_ROMAN
    return 119.05f * escala;
}
void recalcula_dimensoes_info(){
  info.largura = calcular_largura_texto_stroke(menu_fonte_stroke, info.texto, info_escala_fonte);
  info.altura = calcular_altura_fonte_stroke(info_escala_fonte);
}
void atualiza_info(){
    char texto[50];
    strcpy(texto, estado_atual.qtd_pontos_controle);
    strcat(texto, " / ");
    strcat(texto, estado_atual.qtd_nuvem_pontos);
    strcpy(info.texto, texto);
}
void desenhar_info(){
  atualiza_info();
  recalcula_dimensoes_info();
  desenhar_texto((float)(info.x) , (float)(info.y), menu_fonte_stroke, info.texto, info_escala_fonte);
}
