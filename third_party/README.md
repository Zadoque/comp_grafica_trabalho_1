# MicroUI

O menu da aplicacao usa **MicroUI 2.02**, uma biblioteca de interface immediate-mode escrita em C.

- Repositorio oficial: https://github.com/rxi/microui
- Documentacao: https://github.com/rxi/microui/tree/master/doc
- Licenca: MIT

A biblioteca e independente do sistema de renderizacao. O projeto implementa o renderer sobre o OpenGL/GLUT ja utilizado pela aplicacao.

O `MakeFile.sh` baixa `microui.h` e `microui.c` do repositorio oficial somente quando esses arquivos ainda nao existem em `third_party/`.
