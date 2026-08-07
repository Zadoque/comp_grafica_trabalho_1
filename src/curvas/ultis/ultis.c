#include "../../../includes/curvas/ultis/ultis.h"
#include <math.h>


 float dist_mouse_aabb(ponto mouse, AABB box) {
    float dx = fmaxf(0.0f, fmaxf(box.x_min - mouse.point[0], mouse.point[0] - box.x_max));
    float dy = fmaxf(0.0f, fmaxf(box.y_min - mouse.point[1], mouse.point[1] - box.y_max));
    return sqrtf(dx*dx + dy*dy);
}


 float dist_mouse_aabb_left(ponto mouse, AABB box){
    float dx = fmaxf(0.0f, fmaxf(box.esq_x_min - mouse.point[0], mouse.point[0] - box.esq_x_max));
    float dy = fmaxf(0.0f, fmaxf(box.esq_y_min - mouse.point[1], mouse.point[1] - box.esq_y_max));
    return sqrtf(dx*dx + dy*dy);
}

 float dist_mouse_aabb_right(ponto mouse, AABB box){
    float dx = fmaxf(0.0f, fmaxf(box.dir_x_min - mouse.point[0], mouse.point[0] - box.dir_x_max));
    float dy = fmaxf(0.0f, fmaxf(box.dir_y_min - mouse.point[1], mouse.point[1] - box.dir_y_max));
    return sqrtf(dx*dx + dy*dy);
}
