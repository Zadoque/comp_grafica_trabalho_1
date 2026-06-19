#include "../../includes/curvas/catmullrom.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#ifndef FLT_MAX
#define FLT_MAX 99999999.0f
#endif

static const float MATRIZ_CATMULLROM[4][4] = {
    {-1.0f/2.0f,  3.0f/2.0f, -3.0f/2.0f,  1.0f/2.0f},
    { 2.0f/2.0f, -5.0f/2.0f,  4.0f/2.0f, -1.0f/2.0f},
    {-1.0f/2.0f,  0.0f/2.0f,  1.0f/2.0f,  0.0f/2.0f},
    { 0.0f/2.0f,  2.0f/2.0f,  0.0f/2.0f,  0.0f/2.0f}};

ponto calcular_ponto_catmullrom(ponto P0, ponto P1, ponto P2, ponto P3, float t) {
    float t2 = t*t, t3 = t2*t;
    float vetor_t[4] = {t3, t2, t, 1.0f};
    float c[4] = {0};
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            c[i] += vetor_t[j] * MATRIZ_CATMULLROM[j][i];
    ponto r;
    r.point[0] = c[0]*P0.point[0] + c[1]*P1.point[0] + c[2]*P2.point[0] + c[3]*P3.point[0];
    r.point[1] = c[0]*P0.point[1] + c[1]*P1.point[1] + c[2]*P2.point[1] + c[3]*P3.point[1];
    return r;
}

static void subdivide_catmullrom(ponto P0, ponto P1, ponto P2, ponto P3,
                                  Pontos *curva, ponto A, ponto B,
                                  float t0, float t1, float tol, AABB *box) {
    float tmid = (t0+t1)/2.0f;
    ponto M = calcular_ponto_catmullrom(P0,P1,P2,P3, tmid);
    if (calcula_dist_ponto_segmento(M, A, B) < tol) {
        pontos_push(curva, M);
        box->x_min = fminf(box->x_min, M.point[0]); box->y_min = fminf(box->y_min, M.point[1]);
        box->x_max = fmaxf(box->x_max, M.point[0]); box->y_max = fmaxf(box->y_max, M.point[1]);
    } else {
        subdivide_catmullrom(P0,P1,P2,P3, curva, A, M, t0, tmid, tol, box);
        subdivide_catmullrom(P0,P1,P2,P3, curva, M, B, tmid, t1, tol, box);
    }
}

void gerar_curva_catmullrom(ponto P0, ponto P1, ponto P2, ponto P3,
                              Pontos *curva, AABB *box) {
    curva->quantidade_atual = 0;
    ponto A = calcular_ponto_catmullrom(P0,P1,P2,P3, 0.0f);
    ponto B = calcular_ponto_catmullrom(P0,P1,P2,P3, 1.0f);
    box->x_min = fminf(A.point[0], B.point[0]); box->y_min = fminf(A.point[1], B.point[1]);
    box->x_max = fmaxf(A.point[0], B.point[0]); box->y_max = fmaxf(A.point[1], B.point[1]);
    pontos_push(curva, A);
    subdivide_catmullrom(P0,P1,P2,P3, curva, A, B, 0.0f, 1.0f, 0.1f, box);
    pontos_push(curva, B);
}

static float dist_mouse_aabb_c(ponto mouse, AABB box) {
    float dx = fmaxf(0.0f, fmaxf(box.x_min - mouse.point[0], mouse.point[0] - box.x_max));
    float dy = fmaxf(0.0f, fmaxf(box.y_min - mouse.point[1], mouse.point[1] - box.y_max));
    return sqrtf(dx*dx + dy*dy);
}

static void criar_box_picking_c(ponto P0, ponto P1, ponto P2, ponto P3,
                                  ponto A, ponto B, float t0, float t1,
                                  float tol, AABB *box, int lado) {
    float tmid = (t0+t1)/2.0f;
    ponto M = calcular_ponto_catmullrom(P0,P1,P2,P3, tmid);
    if (calcula_dist_ponto_segmento(M, A, B) < tol) {
        box->x_min = fminf(box->x_min, M.point[0]); box->y_min = fminf(box->y_min, M.point[1]);
        box->x_max = fmaxf(box->x_max, M.point[0]); box->y_max = fmaxf(box->y_max, M.point[1]);
    } else {
        if (lado == 0) criar_box_picking_c(P0,P1,P2,P3, A, M, t0, tmid, tol, box, 0);
        else           criar_box_picking_c(P0,P1,P2,P3, M, B, tmid, t1, tol, box, 1);
    }
}

static ResultadoPicking subdivide_picking_catmullrom(
        ponto P0, ponto P1, ponto P2, ponto P3, ponto mouse,
        float t0, float t1, float melhor_dist, float tolerancia_t) {
    ResultadoPicking sem = { -1, 0.0f, FLT_MAX };
    if ((t1-t0) < tolerancia_t) {
        float tmid = (t0+t1)/2.0f;
        ponto p = calcular_ponto_catmullrom(P0,P1,P2,P3, tmid);
        float dx = p.point[0]-mouse.point[0], dy = p.point[1]-mouse.point[1];
        ResultadoPicking r = { 0, tmid, sqrtf(dx*dx+dy*dy) }; return r;
    }
    float tmid = (t0+t1)/2.0f;
    ponto A = calcular_ponto_catmullrom(P0,P1,P2,P3, t0);
    ponto M = calcular_ponto_catmullrom(P0,P1,P2,P3, tmid);
    ponto B = calcular_ponto_catmullrom(P0,P1,P2,P3, t1);
    AABB be; reset_box(&be); criar_box_picking_c(P0,P1,P2,P3, A, M, t0, tmid, 0.1f, &be, 0);
    AABB bd; reset_box(&bd); criar_box_picking_c(P0,P1,P2,P3, M, B, tmid, t1, 0.1f, &bd, 1);
    float de = dist_mouse_aabb_c(mouse, be), dd = dist_mouse_aabb_c(mouse, bd);
    ResultadoPicking melhor = sem;
    if (de <= dd) {
        if (de < melhor_dist) melhor = subdivide_picking_catmullrom(P0,P1,P2,P3, mouse, t0, tmid, melhor_dist, tolerancia_t);
        float lim = (melhor.segmento_indice != -1) ? melhor.distancia : melhor_dist;
        if (dd < lim) { ResultadoPicking r = subdivide_picking_catmullrom(P0,P1,P2,P3, mouse, tmid, t1, lim, tolerancia_t); if (r.distancia < melhor.distancia) melhor = r; }
    } else {
        if (dd < melhor_dist) melhor = subdivide_picking_catmullrom(P0,P1,P2,P3, mouse, tmid, t1, melhor_dist, tolerancia_t);
        float lim = (melhor.segmento_indice != -1) ? melhor.distancia : melhor_dist;
        if (de < lim) { ResultadoPicking r = subdivide_picking_catmullrom(P0,P1,P2,P3, mouse, t0, tmid, lim, tolerancia_t); if (r.distancia < melhor.distancia) melhor = r; }
    }
    return melhor;
}

static void pesos_catmullrom(float t, float B[4]) {
    float t2 = t*t, t3 = t2*t;
    B[0] = (-t3 + 2.0f*t2 - t) / 2.0f;
    B[1] = ( 3.0f*t3 - 5.0f*t2 + 2.0f) / 2.0f;
    B[2] = (-3.0f*t3 + 4.0f*t2 + t) / 2.0f;
    B[3] = (t3 - t2) / 2.0f;
}

void arrastar_ponto_catmullrom(Pontos *clicks, int seg, float ti, ponto mouse) {
    if (!clicks || clicks->quantidade_atual < 4) return;
    int n = clicks->quantidade_atual;
    int i0 = (seg+0)%n, i1 = (seg+1)%n, i2 = (seg+2)%n, i3 = (seg+3)%n;
    ponto P0 = clicks->data[i0], P1 = clicks->data[i1];
    ponto P2 = clicks->data[i2], P3 = clicks->data[i3];
    ponto C = calcular_ponto_catmullrom(P0,P1,P2,P3, ti);
    float ex = mouse.point[0]-C.point[0], ey = mouse.point[1]-C.point[1];
    float B[4]; pesos_catmullrom(ti, B);
    float soma = B[0]*B[0]+B[1]*B[1]+B[2]*B[2]+B[3]*B[3];
    if (soma < 1e-8f) return;
    float fx = ex/soma, fy = ey/soma;
    clicks->data[i0].point[0] += fx*B[0]; clicks->data[i0].point[1] += fy*B[0];
    clicks->data[i1].point[0] += fx*B[1]; clicks->data[i1].point[1] += fy*B[1];
    clicks->data[i2].point[0] += fx*B[2]; clicks->data[i2].point[1] += fy*B[2];
    clicks->data[i3].point[0] += fx*B[3]; clicks->data[i3].point[1] += fy*B[3];
}

ResultadoPicking picking_catmullrom(AABBTREE *arvore, Pontos *clicks,
                                     ponto mouse, float tolerancia, float melhor_dist) {
    ResultadoPicking sem = { -1, 0.0f, FLT_MAX };
    if (!arvore) return sem;
    float dc = dist_mouse_aabb_c(mouse, arvore->box);
    if (dc > tolerancia && dc > melhor_dist) return sem;
    if (arvore->esquerda == NULL && arvore->direita == NULL) {
        int i = arvore->box.segmento_indice, n = clicks->quantidade_atual;
        ponto P0 = clicks->data[i%n], P1 = clicks->data[(i+1)%n];
        ponto P2 = clicks->data[(i+2)%n], P3 = clicks->data[(i+3)%n];
        ResultadoPicking r = subdivide_picking_catmullrom(P0,P1,P2,P3, mouse, 0.0f, 1.0f, FLT_MAX, 0.001f);
        r.segmento_indice = (r.distancia <= tolerancia) ? i : -1;
        return r;
    }
    float de = arvore->esquerda ? dist_mouse_aabb_c(mouse, arvore->esquerda->box) : FLT_MAX;
    float dd = arvore->direita  ? dist_mouse_aabb_c(mouse, arvore->direita->box)  : FLT_MAX;
    ResultadoPicking melhor = sem;
    if (de <= dd) {
        melhor = picking_catmullrom(arvore->esquerda, clicks, mouse, tolerancia, melhor_dist);
        float lim = (melhor.segmento_indice != -1) ? melhor.distancia : melhor_dist;
        ResultadoPicking r = picking_catmullrom(arvore->direita, clicks, mouse, tolerancia, lim);
        if (r.distancia < melhor.distancia) melhor = r;
    } else {
        melhor = picking_catmullrom(arvore->direita, clicks, mouse, tolerancia, melhor_dist);
        float lim = (melhor.segmento_indice != -1) ? melhor.distancia : melhor_dist;
        ResultadoPicking r = picking_catmullrom(arvore->esquerda, clicks, mouse, tolerancia, lim);
        if (r.distancia < melhor.distancia) melhor = r;
    }
    return melhor;
}
