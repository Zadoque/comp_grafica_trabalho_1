#include "./../includes/opengl.h"
#include <GL/glut.h>
#include <stdio.h>
#include "../includes/performance.h"
#include "../includes/menu.h"

int main(int argc, char **argv) {
  printf("=== Trabalho de Computação Gráfica ===\n");
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(800, 800);
  glutCreateWindow("Trabalho 1 de computação Gráfica");

  glutReshapeFunc(AlteraTamanhoJanela);
  glutDisplayFunc(display);
  glutMouseFunc(menu_mouse_dispatch);
  glutPassiveMotionFunc(menu_passive_motion_dispatch);
  glutMotionFunc(menu_motion_dispatch);

  performance_init();
  inicializar_menu();
  initGL();

  printf("Clique na janela para adicionar pontos!\n");
  glutMainLoop();

  pontos_free(&g_clicks);
  pontos_free(&g_curva_atual);
  pontos_free(&g_nuvem_pontos);
  menu_shutdown();
  performance_shutdown();
  return 0;
}
