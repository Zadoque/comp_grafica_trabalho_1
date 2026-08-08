#!/bin/sh

PROJECT_NAME="computacao_grafica"
MICROUI_URL="https://raw.githubusercontent.com/rxi/microui/master/src"

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

echo -e "${YELLOW}=== Compilando $PROJECT_NAME ===${NC}"
mkdir -p obj bin third_party data

# MicroUI e uma dependencia C pequena e renderer-agnostic.
# Baixar somente se ainda nao estiver no projeto.
for file in microui.h microui.c; do
    if [ ! -f "third_party/$file" ]; then
        echo -e "${BLUE}Baixando MicroUI ($file)...${NC}"
        if command -v curl >/dev/null 2>&1; then
            curl -L --fail --silent --show-error "$MICROUI_URL/$file" -o "third_party/$file" || exit 1
        elif command -v wget >/dev/null 2>&1; then
            wget -q "$MICROUI_URL/$file" -O "third_party/$file" || exit 1
        else
            echo -e "${RED}Erro: instale curl ou wget para baixar MicroUI.${NC}"
            exit 1
        fi
    fi
done

SRC_FILES="
src/main.c
src/pontos.c
src/opengl.c
src/performance.c
src/menu.c
src/matriz.c
src/vetor.c
src/aabb.c
src/transformacoes.c
src/curvas/hermite.c
src/curvas/bezier.c
src/curvas/bspline.c
src/curvas/catmullrom.c
src/curvas/ultis/ultis.c
third_party/microui.c
"

COMPILE_ERROR=0
COMPILED_COUNT=0

for srcfile in $SRC_FILES; do
    if [ -f "$srcfile" ]; then
        objfile="obj/$(basename ${srcfile%.c}.o)"
        EXTRA_CFLAGS=""
        case "$srcfile" in
            src/curvas/*.c) EXTRA_CFLAGS="-finstrument-functions" ;;
        esac

        echo -n "  $(basename "$srcfile") -> $(basename "$objfile")... "
        if gcc -c -Wall -Wextra -std=c99 $EXTRA_CFLAGS -Iincludes -Ithird_party "$srcfile" -o "$objfile" 2>/dev/null; then
            echo -e "${GREEN}OK${NC}"
            COMPILED_COUNT=$((COMPILED_COUNT + 1))
        else
            echo -e "${RED}ERRO${NC}"
            gcc -c -Wall -Wextra -std=c99 $EXTRA_CFLAGS -Iincludes -Ithird_party "$srcfile" -o "$objfile"
            COMPILE_ERROR=1
        fi
    fi
done

if [ $COMPILE_ERROR -eq 1 ]; then
    echo -e "${RED}Falha na compilacao!${NC}"
    exit 1
fi

if gcc obj/*.o -lGL -lGLU -lglut -lm -o "bin/$PROJECT_NAME"; then
    echo -e "${GREEN}Linkagem concluida com sucesso!${NC}"
else
    echo -e "${RED}Erro ao linkar o executavel!${NC}"
    exit 1
fi

echo -e "${GREEN}Compilacao concluida: $COMPILED_COUNT arquivos.${NC}"
"./bin/$PROJECT_NAME"
