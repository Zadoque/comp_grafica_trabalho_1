#include "./../includes/opengl.h"
#include <GL/glut.h>
#include <stdio.h>

int main(int argc, char **argv) {
  printf("=== Trabalho de Computação Gráfica ===\n");
  
  // Inicializar GLUT
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(800, 800);
  glutCreateWindow("Trabalho 1 de computação Gráfica");

  // Registrando callbacks
  glutReshapeFunc(AlteraTamanhoJanela);
  glutDisplayFunc(display);
  glutMouseFunc(onMouse);

  // Inicializar estruturas de dados para os pontos
  pontos_init(&g_clicks, 20);
  
  // Inicializar OpenGL
  initGL();
  
  printf("Clique na janela para adicionar pontos!\n");
  
  // Loop principal
  glutMainLoop();
  
  // Limpeza (nunca será executada devido ao glutMainLoop)
  pontos_free(&g_clicks);
  return 0;
}
