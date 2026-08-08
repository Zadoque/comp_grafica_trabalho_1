#include "../includes/menu.h"
#include "../includes/performance.h"
#include "../third_party/microui.h"

#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

extern Pontos g_clicks;
extern int precisa_refazer_curva;

EstadoAplicacao estado_atual = { MODO_CRIAR_PONTO, MODO_POLIGONO_ABERTO, MODO_CURVA_HERMITE, NENHUMA, "0", "0", 0 };
int menu_largura = 200;
Botoes botoes;
BotoesOperacoes botoes_operacoes[4];

static mu_Context ui;
static int ui_initialized = 0;
static int janela_largura = 800;
static int janela_altura = 800;
static int mouse_x = 0;
static int mouse_y = 0;
static int mostrar_metricas = 0;

static void mu_labelf(mu_Context *ctx, const char *fmt, ...) {
    char buffer[512]; va_list args; va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args); va_end(args); mu_label(ctx, buffer);
}
static int text_width(mu_Font font, const char *text, int len) {
    (void)font; int width = 0; for (int i = 0; i < len; ++i) width += ((unsigned char)text[i] < 128) ? glutStrokeWidth(GLUT_STROKE_ROMAN, text[i]) / 8 : 8; return width;
}
static int text_height(mu_Font font) { (void)font; return 16; }
static void draw_rect(mu_Rect r, mu_Color c, int filled) {
    glColor4ub(c.r,c.g,c.b,c.a); glBegin(filled ? GL_QUADS : GL_LINE_LOOP);
    glVertex2i(r.x,r.y); glVertex2i(r.x+r.w,r.y); glVertex2i(r.x+r.w,r.y+r.h); glVertex2i(r.x,r.y+r.h); glEnd();
}
static void draw_text(const mu_TextCommand *cmd) {
    glColor4ub(cmd->color.r,cmd->color.g,cmd->color.b,cmd->color.a); glPushMatrix();
    glTranslatef((float)cmd->pos.x,(float)cmd->pos.y+12.0f,0); glScalef(.125f,.125f,1);
    int len = (int)strlen(cmd->str); for (int i=0;i<len;++i) if ((unsigned char)cmd->str[i] < 128) glutStrokeCharacter(GLUT_STROKE_ROMAN,cmd->str[i]); glPopMatrix();
}
static void render_commands(void) {
    mu_Command *cmd=NULL; while(mu_next_command(&ui,&cmd)) switch(cmd->type) {
        case MU_COMMAND_CLIP: glEnable(GL_SCISSOR_TEST); glScissor(cmd->clip.rect.x,janela_altura-cmd->clip.rect.y-cmd->clip.rect.h,cmd->clip.rect.w,cmd->clip.rect.h); break;
        case MU_COMMAND_RECT: draw_rect(cmd->rect.rect,cmd->rect.color,1); break;
        case MU_COMMAND_TEXT: draw_text(&cmd->text); break;
        case MU_COMMAND_ICON: { mu_Rect r=cmd->icon.rect; glColor4ub(cmd->icon.color.r,cmd->icon.color.g,cmd->icon.color.b,cmd->icon.color.a); glLineWidth(2); glBegin(GL_LINE_LOOP); glVertex2i(r.x,r.y); glVertex2i(r.x+r.w,r.y); glVertex2i(r.x+r.w,r.y+r.h); glVertex2i(r.x,r.y+r.h); glEnd(); break; }
        default: break;
    }
    glDisable(GL_SCISSOR_TEST);
}
static int local_x(void) { return mouse_x - (janela_largura-menu_largura); }
static void begin_ui_frame(void) { mu_begin(&ui); mu_input_mousemove(&ui,local_x(),mouse_y); }

static void action_changed(void) { precisa_refazer_curva=1; glutPostRedisplay(); }
static void draw_sidebar(void) {
    if(!mu_begin_window_ex(&ui,"menu",mu_rect(0,0,menu_largura,janela_altura),MU_OPT_NOCLOSE|MU_OPT_NORESIZE|MU_OPT_NOTITLE)) return;
    mu_layout_row(&ui,1,(int[]){-1},26); mu_label(&ui,"Curvas Parametricas");
    mu_layout_row(&ui,1,(int[]){-1},30);
    if(mu_button(&ui,"Criar Ponto")){estado_atual.criacao_ou_selecao=MODO_CRIAR_PONTO;estado_atual.operacao=NENHUMA;action_changed();}
    if(mu_button(&ui,"Apagar Ponto")){estado_atual.criacao_ou_selecao=MODO_APAGAR_PONTO;action_changed();}
    if(mu_button(&ui,"Selecionar Ponto")){estado_atual.criacao_ou_selecao=MODO_SELECIONAR_PONTO;action_changed();}
    if(mu_button(&ui,"Operar Poligono")){estado_atual.criacao_ou_selecao=MODO_SELECIONAR_POLIGONO;action_changed();}
    if(mu_button(&ui,"Selecionar Curva")){estado_atual.criacao_ou_selecao=MODO_SELEICIONAR_CURVA;action_changed();}
    mu_layout_row(&ui,1,(int[]){-1},22); mu_label(&ui,"Poligono");
    mu_layout_row(&ui,2,(int[]){-1,-1},28);
    if(mu_button(&ui,estado_atual.poligono==MODO_POLIGONO_ABERTO?"[x] Aberto":"Aberto")){estado_atual.poligono=MODO_POLIGONO_ABERTO;action_changed();}
    if(mu_button(&ui,estado_atual.poligono==MODO_POLIGONO_FECHADO?"[x] Fechado":"Fechado")){estado_atual.poligono=MODO_POLIGONO_FECHADO;action_changed();}
    mu_layout_row(&ui,1,(int[]){-1},22); mu_label(&ui,"Tipo de curva");
    mu_layout_row(&ui,2,(int[]){-1,-1},28);
    if(mu_button(&ui,estado_atual.curva==MODO_CURVA_HERMITE?"[x] Hermite":"Hermite")){estado_atual.curva=MODO_CURVA_HERMITE;action_changed();}
    if(mu_button(&ui,estado_atual.curva==MODO_CURVA_BEZIER?"[x] Bezier":"Bezier")){estado_atual.curva=MODO_CURVA_BEZIER;action_changed();}
    if(mu_button(&ui,estado_atual.curva==MODO_CURVA_BSPLINE?"[x] B-Spline":"B-Spline")){estado_atual.curva=MODO_CURVA_BSPLINE;action_changed();}
    if(mu_button(&ui,estado_atual.curva==MODO_CURVA_CATMULLROM?"[x] Catmull-Rom":"Catmull-Rom")){estado_atual.curva=MODO_CURVA_CATMULLROM;action_changed();}
    if(estado_atual.criacao_ou_selecao==MODO_SELECIONAR_POLIGONO){
        mu_layout_row(&ui,1,(int[]){-1},22); mu_label(&ui,"Operacoes"); mu_layout_row(&ui,2,(int[]){-1,-1},28);
        if(mu_button(&ui,"Translacao")){estado_atual.operacao=TRANSLACAO;action_changed();} if(mu_button(&ui,"Rotacao")){estado_atual.operacao=ROTACAO;action_changed();}
        if(mu_button(&ui,"Escala")){estado_atual.operacao=ESCALA;action_changed();} if(mu_button(&ui,"Shear")){estado_atual.operacao=SHEAR;action_changed();}
    }
    mu_layout_row(&ui,1,(int[]){-1},34); if(mu_button(&ui,"Metricas de desempenho")) mostrar_metricas=1; mu_end_window(&ui);
}
static void draw_status(void) {
    char controle[64],nuvem[64]; snprintf(controle,sizeof(controle),"Pontos de controle: %d",g_clicks.quantidade_atual); snprintf(nuvem,sizeof(nuvem),"Nuvem de pontos: %d",estado_atual.qtd_nuvem_pontos_number);
    if(!mu_begin_window_ex(&ui,"status",mu_rect(5,janela_altura-86,menu_largura-10,80),MU_OPT_NOCLOSE|MU_OPT_NORESIZE|MU_OPT_NOTITLE)) return;
    mu_layout_row(&ui,1,(int[]){-1},22); mu_Rect a=mu_layout_next(&ui); mu_draw_control_text(&ui,controle,a,MU_COLOR_TEXT,0);
    mu_layout_row(&ui,1,(int[]){-1},22); mu_Rect b=mu_layout_next(&ui); mu_draw_control_text(&ui,nuvem,b,MU_COLOR_TEXT,0);
    if(mu_mouse_over(&ui,a)){mu_Rect t=mu_rect(a.x,a.y-30,a.w,26);mu_draw_rect(&ui,t,mu_color(30,35,42,245));mu_draw_text(&ui,ui.style->font,"Quantidade de pontos usados para definir a forma da curva.",55,mu_vec2(t.x+4,t.y+5),mu_color(240,240,240,255));}
    else if(mu_mouse_over(&ui,b)){mu_Rect t=mu_rect(b.x,b.y-30,b.w,26);mu_draw_rect(&ui,t,mu_color(30,35,42,245));mu_draw_text(&ui,ui.style->font,"Quantidade de pontos amostrados como referencia geometrica.",60,mu_vec2(t.x+4,t.y+5),mu_color(240,240,240,255));}
    mu_end_window(&ui);
}
static void draw_metrics(void) {
    if(!mostrar_metricas)return;
    if(!mu_begin_window_ex(&ui,"metricas",mu_rect(5,5,menu_largura-10,janela_altura-10),MU_OPT_HOLDFOCUS|MU_OPT_NOCLOSE))return;
    const PerformanceMetric *m=performance_current(); mu_layout_row(&ui,1,(int[]){-1},22);mu_label(&ui,"METRICAS DE DESEMPENHO");
    mu_layout_row(&ui,1,(int[]){-1},30);if(mu_button(&ui,"Fechar metricas")){mostrar_metricas=0;mu_end_window(&ui);return;}
    mu_labelf(&ui,"Maquina: %s",m->machine_id);mu_labelf(&ui,"CPU: %s",m->processor);mu_labelf(&ui,"RAM: %s",m->memory);mu_labelf(&ui,"GPU: %s",m->gpu);mu_labelf(&ui,"Arq: %s",m->architecture);mu_labelf(&ui,"OS: %s",m->os);
    mu_label(&ui,"Ultima medicao");mu_labelf(&ui,"Curva: %s",m->curve[0]?m->curve:"-");mu_labelf(&ui,"Tempo de geracao: %.6f ms",m->generation_ms);mu_labelf(&ui,"Tempo de desenho: %.6f ms",m->drawing_ms);mu_labelf(&ui,"Pontos gerados: %zu",m->points_generated);mu_labelf(&ui,"Buffer da curva: %zu / %zu",m->buffer_used,m->buffer_capacity);mu_labelf(&ui,"Buffer de controle: %zu / %zu",m->control_buffer_used,m->control_buffer_capacity);
    mu_label(&ui,"Historico por tipo de curva"); PerformanceMetric items[64];int count=performance_history(items,64);const char* nomes[]={"Hermite","Catmull-Rom","B-Spline","Bezier"};
    for(int c=0;c<4;c++){int n=0;double gen=0,draw=0;size_t pts=0;for(int i=0;i<count;i++)if(strcmp(items[i].curve,nomes[c])==0){n++;gen+=items[i].generation_ms;draw+=items[i].drawing_ms;pts+=items[i].points_generated;}if(n)mu_labelf(&ui,"%s: %d exec | geracao %.4f ms | desenho %.4f ms | pontos %zu",nomes[c],n,gen/n,draw/n,pts);}
    mu_label(&ui,"Historico salvo em data/performance.csv"); mu_layout_row(&ui,1,(int[]){-1},-180);mu_begin_panel_ex(&ui,"historico",0);for(int i=0;i<count;i++)mu_labelf(&ui,"%s | %s | %s | gen %.4f | draw %.4f | pts %zu",items[i].timestamp,items[i].machine_id,items[i].curve,items[i].generation_ms,items[i].drawing_ms,items[i].points_generated);mu_end_panel(&ui);mu_end_window(&ui);
}
void atualizar_dimensoes_menu(void){janela_largura=glutGet(GLUT_WINDOW_WIDTH);janela_altura=glutGet(GLUT_WINDOW_HEIGHT);menu_largura=(int)(janela_largura*.15f);if(menu_largura<150)menu_largura=150;if(menu_largura>janela_largura-100)menu_largura=janela_largura-100;}
void inicializar_menu(void){atualizar_dimensoes_menu();mu_init(&ui);ui.style->font=(mu_Font)GLUT_STROKE_ROMAN;ui.text_width=text_width;ui.text_height=text_height;ui.style->size=mu_vec2(80,16);ui.style->padding=5;ui.style->spacing=4;ui.style->title_height=24;ui.style->scrollbar_size=12;ui_initialized=1;}
void menu_mouse_move(int x,int y){mouse_x=x;mouse_y=y;if(ui_initialized)mu_input_mousemove(&ui,x-(janela_largura-menu_largura),y);}
void menu_mouse_button(int button,int state,int x,int y){mouse_x=x;mouse_y=y;if(!ui_initialized)return;int b=button==GLUT_LEFT_BUTTON?MU_MOUSE_LEFT:button==GLUT_RIGHT_BUTTON?MU_MOUSE_RIGHT:0;if(!b)return;int lx=x-(janela_largura-menu_largura);if(state==GLUT_DOWN)mu_input_mousedown(&ui,lx,y,b);else mu_input_mouseup(&ui,lx,y,b);}
void menu_mouse_scroll(int direction,int x,int y){mouse_x=x;mouse_y=y;if(ui_initialized)mu_input_scroll(&ui,0,direction);}
void menu_render(void){if(!ui_initialized)return;atualizar_dimensoes_menu();glMatrixMode(GL_PROJECTION);glLoadIdentity();glOrtho(0,menu_largura,janela_altura,0,-1,1);glMatrixMode(GL_MODELVIEW);glLoadIdentity();begin_ui_frame();draw_sidebar();draw_status();draw_metrics();mu_end(&ui);render_commands();}
int menu_bloqueia_desenho(void){return mostrar_metricas;}
void menu_shutdown(void){ui_initialized=0;}
void menu_mouse_dispatch(int button,int state,int x,int y){int lw=glutGet(GLUT_WINDOW_WIDTH);if(x>=lw-menu_largura)menu_mouse_button(button,state,x,y);}
void menu_passive_motion_dispatch(int x,int y){menu_mouse_move(x,y);}
void menu_motion_dispatch(int x,int y){menu_mouse_move(x,y);}
void desenhar_botoes_menu(void){}
void desenhar_info(void){}
