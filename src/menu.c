#include "../includes/menu.h"
#include "../includes/performance.h"

#include <GL/gl.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

extern Pontos g_clicks;
extern int precisa_refazer_curva;

#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_IMPLEMENTATION
#include "../third_party/nuklear.h"

EstadoAplicacao estado_atual = {
    MODO_CRIAR_PONTO,
    MODO_POLIGONO_ABERTO,
    MODO_CURVA_HERMITE,
    NENHUMA,
    "0",
    "0",
    0
};

int menu_largura = 200;

static struct nk_context nk_ctx;
static struct nk_user_font nk_font;
static int nk_initialized = 0;
static int mouse_x = 0;
static int mouse_y = 0;
static int mouse_left_down = 0;
static int mouse_right_down = 0;
static int scroll_direction = 0;
static int mostrar_metricas = 0;
static int menu_altura = 800;

static float font_width(nk_handle userdata, float height, const char *text, int len) {
    (void)userdata;
    float scale = height / 119.05f;
    float width = 0.0f;
    for (int i = 0; i < len; ++i) {
        unsigned char c = (unsigned char)text[i];
        if (c < 128) width += glutStrokeWidth(GLUT_STROKE_ROMAN, c) * scale;
        else width += 60.0f * scale;
    }
    return width;
}

static void color4(struct nk_color c) {
    glColor4ub(c.r, c.g, c.b, c.a);
}

static void draw_rect(float x, float y, float w, float h, struct nk_color color) {
    color4(color);
    glBegin(GL_QUADS);
    glVertex2f(x, menu_altura - y);
    glVertex2f(x + w, menu_altura - y);
    glVertex2f(x + w, menu_altura - y - h);
    glVertex2f(x, menu_altura - y - h);
    glEnd();
}

static void draw_line(float x0, float y0, float x1, float y1, float thickness, struct nk_color color) {
    color4(color);
    glLineWidth(thickness > 0 ? thickness : 1.0f);
    glBegin(GL_LINES);
    glVertex2f(x0, menu_altura - y0);
    glVertex2f(x1, menu_altura - y1);
    glEnd();
}

static void draw_circle(float x, float y, float w, float h, struct nk_color color, int filled) {
    color4(color);
    float cx = x + w * 0.5f;
    float cy = menu_altura - (y + h * 0.5f);
    float rx = w * 0.5f;
    float ry = h * 0.5f;
    glBegin(filled ? GL_TRIANGLE_FAN : GL_LINE_LOOP);
    for (int i = 0; i < 32; ++i) {
        float a = 6.28318530718f * (float)i / 32.0f;
        glVertex2f(cx + rx * cosf(a), cy + ry * sinf(a));
    }
    glEnd();
}

static void draw_triangle(float x0, float y0, float x1, float y1, float x2, float y2,
                          struct nk_color color, int filled) {
    color4(color);
    glBegin(filled ? GL_TRIANGLES : GL_LINE_LOOP);
    glVertex2f(x0, menu_altura - y0);
    glVertex2f(x1, menu_altura - y1);
    glVertex2f(x2, menu_altura - y2);
    glEnd();
}

static void draw_polygon(const struct nk_vec2i *points, int count, struct nk_color color, int filled) {
    color4(color);
    glBegin(filled ? GL_POLYGON : GL_LINE_LOOP);
    for (int i = 0; i < count; ++i)
        glVertex2f(points[i].x, menu_altura - points[i].y);
    glEnd();
}

static void draw_text(const struct nk_command_text *cmd) {
    color4(cmd->foreground);
    float scale = cmd->height / 119.05f;
    glPushMatrix();
    glTranslatef((float)cmd->x, menu_altura - (float)cmd->y - cmd->height, 0.0f);
    glScalef(scale, scale, 1.0f);
    for (int i = 0; i < cmd->length; ++i) {
        unsigned char c = (unsigned char)cmd->string[i];
        if (c < 128) glutStrokeCharacter(GLUT_STROKE_ROMAN, c);
    }
    glPopMatrix();
}

static void render_nuklear(void) {
    const struct nk_command *cmd;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, menu_largura, 0, menu_altura, -1, 1);

    nk_foreach(cmd, &nk_ctx) {
        switch (cmd->type) {
            case NK_COMMAND_SCISSOR: {
                const struct nk_command_scissor *c = (const struct nk_command_scissor *)cmd;
                glEnable(GL_SCISSOR_TEST);
                glScissor(c->x, menu_altura - c->y - c->h, c->w, c->h);
                break;
            }
            case NK_COMMAND_LINE: {
                const struct nk_command_line *c = (const struct nk_command_line *)cmd;
                draw_line(c->begin.x, c->begin.y, c->end.x, c->end.y,
                          c->line_thickness, c->color);
                break;
            }
            case NK_COMMAND_RECT: {
                const struct nk_command_rect *c = (const struct nk_command_rect *)cmd;
                color4(c->color);
                glLineWidth(c->line_thickness > 0 ? c->line_thickness : 1.0f);
                glBegin(GL_LINE_LOOP);
                glVertex2f(c->x, menu_altura - c->y);
                glVertex2f(c->x + c->w, menu_altura - c->y);
                glVertex2f(c->x + c->w, menu_altura - c->y - c->h);
                glVertex2f(c->x, menu_altura - c->y - c->h);
                glEnd();
                break;
            }
            case NK_COMMAND_RECT_FILLED: {
                const struct nk_command_rect_filled *c = (const struct nk_command_rect_filled *)cmd;
                draw_rect(c->x, c->y, c->w, c->h, c->color);
                break;
            }
            case NK_COMMAND_RECT_MULTI_COLOR: {
                const struct nk_command_rect_multi_color *c = (const struct nk_command_rect_multi_color *)cmd;
                glBegin(GL_QUADS);
                color4(c->left); glVertex2f(c->x, menu_altura - c->y);
                color4(c->top); glVertex2f(c->x + c->w, menu_altura - c->y);
                color4(c->right); glVertex2f(c->x + c->w, menu_altura - c->y - c->h);
                color4(c->bottom); glVertex2f(c->x, menu_altura - c->y - c->h);
                glEnd();
                break;
            }
            case NK_COMMAND_CIRCLE: {
                const struct nk_command_circle *c = (const struct nk_command_circle *)cmd;
                draw_circle(c->x, c->y, c->w, c->h, c->color, 0);
                break;
            }
            case NK_COMMAND_CIRCLE_FILLED: {
                const struct nk_command_circle_filled *c = (const struct nk_command_circle_filled *)cmd;
                draw_circle(c->x, c->y, c->w, c->h, c->color, 1);
                break;
            }
            case NK_COMMAND_TRIANGLE: {
                const struct nk_command_triangle *c = (const struct nk_command_triangle *)cmd;
                draw_triangle(c->a.x, c->a.y, c->b.x, c->b.y, c->c.x, c->c.y, c->color, 0);
                break;
            }
            case NK_COMMAND_TRIANGLE_FILLED: {
                const struct nk_command_triangle_filled *c = (const struct nk_command_triangle_filled *)cmd;
                draw_triangle(c->a.x, c->a.y, c->b.x, c->b.y, c->c.x, c->c.y, c->color, 1);
                break;
            }
            case NK_COMMAND_POLYGON: {
                const struct nk_command_polygon *c = (const struct nk_command_polygon *)cmd;
                draw_polygon(c->points, c->point_count, c->color, 0);
                break;
            }
            case NK_COMMAND_POLYGON_FILLED: {
                const struct nk_command_polygon_filled *c = (const struct nk_command_polygon_filled *)cmd;
                draw_polygon(c->points, c->point_count, c->color, 1);
                break;
            }
            case NK_COMMAND_POLYLINE: {
                const struct nk_command_polyline *c = (const struct nk_command_polyline *)cmd;
                draw_polygon(c->points, c->point_count, c->color, 0);
                break;
            }
            case NK_COMMAND_TEXT:
                draw_text((const struct nk_command_text *)cmd);
                break;
            default:
                break;
        }
    }
    glDisable(GL_SCISSOR_TEST);
    glEnable(GL_DEPTH_TEST);
    nk_clear(&nk_ctx);
}

static void apply_style(void) {
    nk_style_default(&nk_ctx);
    nk_ctx.style.window.background = nk_rgba(36, 42, 50, 245);
    nk_ctx.style.window.border_color = nk_rgba(80, 90, 105, 255);
    nk_ctx.style.window.border = 1.0f;
    nk_ctx.style.window.rounding = 4.0f;
    nk_ctx.style.window.padding = nk_vec2(8, 8);
    nk_ctx.style.button.normal = nk_style_item_color(nk_rgba(55, 64, 76, 255));
    nk_ctx.style.button.hover = nk_style_item_color(nk_rgba(75, 88, 104, 255));
    nk_ctx.style.button.active = nk_style_item_color(nk_rgba(40, 130, 80, 255));
    nk_ctx.style.button.border_color = nk_rgba(105, 115, 130, 255);
    nk_ctx.style.button.text_normal = nk_rgba(235, 240, 245, 255);
    nk_ctx.style.button.text_hover = nk_rgba(255, 255, 255, 255);
    nk_ctx.style.button.text_active = nk_rgba(255, 255, 255, 255);
}

static void draw_status_window(void) {
    char controle[64], nuvem[64];
    snprintf(controle, sizeof(controle), "Pontos de controle: %d", g_clicks.quantidade_atual);
    snprintf(nuvem, sizeof(nuvem), "Nuvem de pontos: %d", estado_atual.qtd_nuvem_pontos_number);

    if (nk_begin(&nk_ctx, "status",
                 nk_rect(5, menu_altura - 94, menu_largura - 10, 88),
                 NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_dynamic(&nk_ctx, 28, 1);
        if (nk_button_label(&nk_ctx, controle)) { }
        if (nk_widget_is_hovered(&nk_ctx))
            nk_tooltip(&nk_ctx, "Quantidade de pontos usados para definir a forma da curva.");
        if (nk_button_label(&nk_ctx, nuvem)) { }
        if (nk_widget_is_hovered(&nk_ctx))
            nk_tooltip(&nk_ctx, "Quantidade de pontos amostrados como referencia geometrica.");
    }
    nk_end(&nk_ctx);
}

static void draw_metrics_window(void) {
    if (!mostrar_metricas) return;

    struct nk_rect bounds = nk_rect(80, 60, 720, 620);
    if (nk_begin(&nk_ctx, "metricas", bounds,
                 NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_MOVABLE |
                 NK_WINDOW_SCALABLE | NK_WINDOW_CLOSABLE)) {
        nk_layout_row_dynamic(&nk_ctx, 24, 1);
        const PerformanceMetric *current = performance_current();
        nk_labelf(&nk_ctx, NK_TEXT_LEFT, "Maquina: %s", current->machine_id);
        nk_labelf(&nk_ctx, NK_TEXT_LEFT, "CPU: %s", current->processor);
        nk_labelf(&nk_ctx, NK_TEXT_LEFT, "Memoria: %s", current->memory);
        nk_labelf(&nk_ctx, NK_TEXT_LEFT, "GPU: %s", current->gpu);

        nk_layout_row_dynamic(&nk_ctx, 26, 1);
        nk_label(&nk_ctx, "Ultima medicao", NK_TEXT_LEFT);
        nk_labelf(&nk_ctx, NK_TEXT_LEFT, "Curva: %s", current->curve[0] ? current->curve : "-");
        nk_labelf(&nk_ctx, NK_TEXT_LEFT, "Geracao: %.4f ms", current->generation_ms);
        nk_labelf(&nk_ctx, NK_TEXT_LEFT, "Desenho: %.4f ms", current->drawing_ms);
        nk_labelf(&nk_ctx, NK_TEXT_LEFT, "Pontos gerados: %zu", current->points_generated);
        nk_labelf(&nk_ctx, NK_TEXT_LEFT, "Buffer: %zu / %zu", current->buffer_used, current->buffer_capacity);

        PerformanceMetric items[64];
        int count = performance_history(items, 64);

        nk_layout_row_dynamic(&nk_ctx, 28, 1);
        nk_label(&nk_ctx, "Resumo por tipo de curva", NK_TEXT_LEFT);
        const char *nomes[] = {"Hermite", "Catmull-Rom", "B-Spline", "Bezier"};
        for (int c = 0; c < 4; ++c) {
            int n = 0;
            double gen = 0.0, draw = 0.0;
            size_t max_points = 0;
            for (int i = 0; i < count; ++i) {
                if (strcmp(items[i].curve, nomes[c]) == 0) {
                    ++n;
                    gen += items[i].generation_ms;
                    draw += items[i].drawing_ms;
                    if (items[i].points_generated > max_points) max_points = items[i].points_generated;
                }
            }
            if (n > 0) {
                nk_layout_row_dynamic(&nk_ctx, 22, 1);
                nk_labelf(&nk_ctx, NK_TEXT_LEFT,
                          "%s | amostras %d | geracao media %.4f ms | desenho medio %.4f ms | max pts %zu",
                          nomes[c], n, gen / n, draw / n, max_points);
            }
        }

        nk_layout_row_dynamic(&nk_ctx, 28, 1);
        nk_label(&nk_ctx, "Historico salvo em data/performance.csv", NK_TEXT_LEFT);

        if (nk_group_begin(&nk_ctx, "historico", NK_WINDOW_BORDER | NK_WINDOW_SCROLL_AUTO_HIDE)) {
            nk_layout_row_dynamic(&nk_ctx, 22, 1);
            for (int i = 0; i < count; ++i) {
                nk_labelf(&nk_ctx, NK_TEXT_LEFT,
                          "%s | %s | gen %.3f ms | draw %.3f ms | pts %zu",
                          items[i].timestamp, items[i].curve,
                          items[i].generation_ms, items[i].drawing_ms,
                          items[i].points_generated);
            }
            nk_group_end(&nk_ctx);
        }
    }
    nk_end(&nk_ctx);
    if (nk_window_is_closed(&nk_ctx, "metricas")) mostrar_metricas = 0;
}

static void apply_action(Criacao_ou_selecao action) {
    precisa_refazer_curva = (estado_atual.criacao_ou_selecao != action);
    estado_atual.criacao_ou_selecao = action;
    if (action == MODO_CRIAR_PONTO) estado_atual.operacao = NENHUMA;
}

static void draw_sidebar(void) {
    if (nk_begin(&nk_ctx, "menu", nk_rect(0, 0, menu_largura, menu_altura),
                 NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BACKGROUND)) {
        nk_layout_row_dynamic(&nk_ctx, 26, 1);
        nk_label(&nk_ctx, "Curvas Parametricas", NK_TEXT_CENTERED);
        nk_spacing(&nk_ctx, 1);

        nk_layout_row_dynamic(&nk_ctx, 30, 1);
        if (nk_button_label(&nk_ctx, "Criar Ponto")) apply_action(MODO_CRIAR_PONTO);
        if (nk_button_label(&nk_ctx, "Apagar Ponto")) apply_action(MODO_APAGAR_PONTO);
        if (nk_button_label(&nk_ctx, "Selecionar Ponto")) apply_action(MODO_SELECIONAR_PONTO);
        if (nk_button_label(&nk_ctx, "Operar Poligono")) apply_action(MODO_SELECIONAR_POLIGONO);
        if (nk_button_label(&nk_ctx, "Selecionar Curva")) apply_action(MODO_SELEICIONAR_CURVA);

        nk_layout_row_dynamic(&nk_ctx, 24, 1);
        nk_label(&nk_ctx, "Poligono", NK_TEXT_LEFT);
        nk_layout_row_dynamic(&nk_ctx, 30, 2);
        if (nk_option_label(&nk_ctx, "Aberto", estado_atual.poligono == MODO_POLIGONO_ABERTO)) {
            estado_atual.poligono = MODO_POLIGONO_ABERTO;
            precisa_refazer_curva = 1;
        }
        if (nk_option_label(&nk_ctx, "Fechado", estado_atual.poligono == MODO_POLIGONO_FECHADO)) {
            estado_atual.poligono = MODO_POLIGONO_FECHADO;
            precisa_refazer_curva = 1;
        }

        nk_layout_row_dynamic(&nk_ctx, 24, 1);
        nk_label(&nk_ctx, "Tipo de curva", NK_TEXT_LEFT);
        nk_layout_row_dynamic(&nk_ctx, 30, 2);
        if (nk_option_label(&nk_ctx, "Hermite", estado_atual.curva == MODO_CURVA_HERMITE)) {
            estado_atual.curva = MODO_CURVA_HERMITE; precisa_refazer_curva = 1;
        }
        if (nk_option_label(&nk_ctx, "Bezier", estado_atual.curva == MODO_CURVA_BEZIER)) {
            estado_atual.curva = MODO_CURVA_BEZIER; precisa_refazer_curva = 1;
        }
        if (nk_option_label(&nk_ctx, "B-Spline", estado_atual.curva == MODO_CURVA_BSPLINE)) {
            estado_atual.curva = MODO_CURVA_BSPLINE; precisa_refazer_curva = 1;
        }
        if (nk_option_label(&nk_ctx, "Catmull-Rom", estado_atual.curva == MODO_CURVA_CATMULLROM)) {
            estado_atual.curva = MODO_CURVA_CATMULLROM; precisa_refazer_curva = 1;
        }

        if (estado_atual.criacao_ou_selecao == MODO_SELECIONAR_POLIGONO) {
            nk_layout_row_dynamic(&nk_ctx, 24, 1);
            nk_label(&nk_ctx, "Operacoes", NK_TEXT_LEFT);
            nk_layout_row_dynamic(&nk_ctx, 30, 2);
            if (nk_button_label(&nk_ctx, "Translacao")) { estado_atual.operacao = TRANSLACAO; precisa_refazer_curva = 1; }
            if (nk_button_label(&nk_ctx, "Rotacao")) { estado_atual.operacao = ROTACAO; precisa_refazer_curva = 1; }
            if (nk_button_label(&nk_ctx, "Escala")) { estado_atual.operacao = ESCALA; precisa_refazer_curva = 1; }
            if (nk_button_label(&nk_ctx, "Shear")) { estado_atual.operacao = SHEAR; precisa_refazer_curva = 1; }
        }

        nk_layout_row_dynamic(&nk_ctx, 34, 1);
        if (nk_button_label(&nk_ctx, "Metricas de desempenho")) mostrar_metricas = 1;
    }
    nk_end(&nk_ctx);
}

void atualizar_dimensoes_menu(void) {
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);
    menu_largura = (int)(width * 0.15f);
    if (menu_largura < 150) menu_largura = 150;
    if (menu_largura > width - 100) menu_largura = width - 100;
    menu_altura = height;
}

void inicializar_menu(void) {
    atualizar_dimensoes_menu();
    memset(&nk_font, 0, sizeof(nk_font));
    nk_font.height = 14.0f;
    nk_font.width = font_width;
    if (!nk_init_default(&nk_ctx, &nk_font)) {
        fprintf(stderr, "Falha ao inicializar Nuklear.\n");
        return;
    }
    nk_initialized = 1;
    apply_style();
}

void menu_mouse_move(int x, int y) {
    mouse_x = x;
    mouse_y = y;
}

void menu_mouse_button(int button, int state, int x, int y) {
    mouse_x = x;
    mouse_y = y;
    if (button == GLUT_LEFT_BUTTON) mouse_left_down = (state == GLUT_DOWN);
    if (button == GLUT_RIGHT_BUTTON) mouse_right_down = (state == GLUT_DOWN);
}

void menu_mouse_scroll(int direction, int x, int y) {
    mouse_x = x;
    mouse_y = y;
    scroll_direction = direction;
}

void menu_render(void) {
    if (!nk_initialized) return;

    int sidebar_mouse_x = mouse_x - (glutGet(GLUT_WINDOW_WIDTH) - menu_largura);
    nk_input_begin(&nk_ctx);
    nk_input_motion(&nk_ctx, sidebar_mouse_x, mouse_y);
    nk_input_button(&nk_ctx, NK_BUTTON_LEFT, sidebar_mouse_x, mouse_y, mouse_left_down);
    nk_input_button(&nk_ctx, NK_BUTTON_RIGHT, sidebar_mouse_x, mouse_y, mouse_right_down);
    if (scroll_direction != 0) {
        nk_input_scroll(&nk_ctx, nk_vec2(0.0f, (float)scroll_direction));
        if (!mostrar_metricas) scroll_direction = 0;
    }
    nk_input_end(&nk_ctx);

    draw_sidebar();
    draw_status_window();
    render_nuklear();
}

void menu_render_metricas_fullscreen(void) {
    if (!nk_initialized || !mostrar_metricas) return;

    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);
    menu_altura = height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    nk_input_begin(&nk_ctx);
    nk_input_motion(&nk_ctx, mouse_x, mouse_y);
    nk_input_button(&nk_ctx, NK_BUTTON_LEFT, mouse_x, mouse_y, mouse_left_down);
    nk_input_button(&nk_ctx, NK_BUTTON_RIGHT, mouse_x, mouse_y, mouse_right_down);
    if (scroll_direction != 0) {
        nk_input_scroll(&nk_ctx, nk_vec2(0.0f, (float)scroll_direction));
        scroll_direction = 0;
    }
    nk_input_end(&nk_ctx);

    draw_metrics_window();
    render_nuklear();
}

void menu_shutdown(void) {
    if (nk_initialized) {
        nk_free(&nk_ctx);
        nk_initialized = 0;
    }
}
