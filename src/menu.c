#include "./../includes/menu.h"
#include <string.h>
//#include <stdio.h>
#include <GL/glut.h>

// Implementar todas as funções do menu que te mostrei antes
EstadoAplicacao estado_atual = MODO_CRIAR_PONTO;
int menu_largura = 200;
BotaoMenu botoes_menu[9];

void inicializar_menu() {
    // Definir os botões (posição relativa ao menu)
    strcpy(botoes_menu[0].texto, "Criar Ponto");
    botoes_menu[0].x = 10; botoes_menu[0].y = 50;
    botoes_menu[0].largura = 180; botoes_menu[0].altura = 40;
    botoes_menu[0].acao = MODO_CRIAR_PONTO;
    
    strcpy(botoes_menu[1].texto, "Selecionar vertice");
    botoes_menu[1].x = 10; botoes_menu[1].y = 100;
    botoes_menu[1].largura = 180; botoes_menu[1].altura = 40;
    botoes_menu[1].acao = MODO_SELECIONAR_PONTO;

    strcpy(botoes_menu[2].texto, "Selecionar Poligono");
    botoes_menu[2].x = 10; botoes_menu[2].y = 150;
    botoes_menu[2].largura = 180; botoes_menu[2].altura = 40;
    botoes_menu[2].acao = MODO_SELECIONAR_POLIGONO;
    
    strcpy(botoes_menu[3].texto, "Polígono Aberto");
    botoes_menu[3].x = 10; botoes_menu[3].y = 250;
    botoes_menu[3].largura = 180; botoes_menu[3].altura = 40;
    botoes_menu[3].acao = MODO_POLIGONO_ABERTO;
    
    strcpy(botoes_menu[4].texto, "Polígono Fechado");
    botoes_menu[4].x = 10; botoes_menu[4].y = 300;
    botoes_menu[4].largura = 180; botoes_menu[4].altura = 40;
    botoes_menu[4].acao = MODO_POLIGONO_FECHADO;
    
    strcpy(botoes_menu[5].texto, "Hermite");
    botoes_menu[5].x = 10; botoes_menu[5].y = 400;
    botoes_menu[5].largura = 180; botoes_menu[5].altura = 40;
    botoes_menu[5].acao = MODO_CURVA_HERMITE;
    
    strcpy(botoes_menu[6].texto, "Catmull-Rom");
    botoes_menu[6].x = 10; botoes_menu[6].y = 450;
    botoes_menu[6].largura = 180; botoes_menu[6].altura = 40;
    botoes_menu[6].acao = MODO_CURVA_CATMULLROM;
    
    strcpy(botoes_menu[7].texto, "B-Spline");
    botoes_menu[7].x = 10; botoes_menu[7].y = 500;
    botoes_menu[7].largura = 180; botoes_menu[7].altura = 40;
    botoes_menu[7].acao = MODO_CURVA_BSPLINE;
    
    strcpy(botoes_menu[8].texto, "Bezier");
    botoes_menu[8].x = 10; botoes_menu[8].y = 550;
    botoes_menu[8].largura = 180; botoes_menu[8].altura = 40;
    botoes_menu[8].acao = MODO_CURVA_BEZIER;
}

void desenhar_texto(float x, float y, void* font, char* texto) {
    glRasterPos2f(x, y);
    for (char* c = texto; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void desenhar_botoes_menu() {
    for (int i = 0; i < 9; i++) {
        BotaoMenu *botao = &botoes_menu[i];
        
        // Cor do botão (ativo, destacado ou normal)
        if (estado_atual == botao->acao) {
            glColor3f(0.2f, 0.7f, 0.2f); // Verde se ativo
        } else if (botao->destacado) {
            glColor3f(0.9f, 0.9f, 0.7f); // Amarelo se mouse em cima
        } else {
            glColor3f(0.95f, 0.95f, 0.95f); // Cinza claro normal
        }
        
        // Desenhar retângulo do botão
        glBegin(GL_QUADS);
            glVertex2f(botao->x, botao->y);
            glVertex2f(botao->x + botao->largura, botao->y);
            glVertex2f(botao->x + botao->largura, botao->y + botao->altura);
            glVertex2f(botao->x, botao->y + botao->altura);
        glEnd();
        
        // Contorno do botão
        glColor3f(0.0f, 0.0f, 0.0f);
        glLineWidth(1.0f);
        glBegin(GL_LINE_LOOP);
            glVertex2f(botao->x, botao->y);
            glVertex2f(botao->x + botao->largura, botao->y);
            glVertex2f(botao->x + botao->largura, botao->y + botao->altura);
            glVertex2f(botao->x, botao->y + botao->altura);
        glEnd();
        
        // === DESENHAR TEXTO DO BOTÃO ===
        glColor3f(0.0f, 0.0f, 0.0f); // Texto preto
        
        // Calcular posição central do texto
        void* font = GLUT_BITMAP_HELVETICA_12;
        int texto_largura = 0;
        
        // Calcular largura do texto
        for (char* c = botao->texto; *c != '\0'; c++) {
            texto_largura += glutBitmapWidth(font, *c);
        }
        
        // Posição centralizada
        float texto_x = botao->x + (botao->largura - texto_largura) / 2.0f;
        float texto_y = botao->y + (botao->altura / 2.0f) - 6; // -6 para centralizar verticalmente
        
        // Desenhar o texto
        desenhar_texto(texto_x, texto_y, font, botao->texto);
    }
}

