#!/bin/bash

# Nome do projeto
PROJECT_NAME="computacao_grafica"

# Cores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${YELLOW}=== Compilando $PROJECT_NAME ===${NC}"

# Criar diretórios necessários
echo "Criando diretórios..."
mkdir -p obj bin

# Verificar se existem arquivos .c em src/
if ! ls src/*.c 1> /dev/null 2>&1; then
    echo -e "${RED}Erro: Nenhum arquivo .c encontrado em src/${NC}"
    exit 1
fi

# Listar arquivos que serão compilados
echo -e "${BLUE}Arquivos encontrados:${NC}"
for srcfile in src/*.c; do
    if [ -f "$srcfile" ]; then
        echo "  - $(basename $srcfile)"
    fi
done

# Compilar arquivos .c individualmente
echo -e "${BLUE}Compilando arquivos fonte...${NC}"
COMPILE_ERROR=0

for srcfile in src/*.c; do
    if [ -f "$srcfile" ]; then
        objfile="obj/$(basename ${srcfile%.c}.o)"
        echo -n "  $(basename $srcfile) -> $(basename $objfile)... "
        
        if gcc -c -Wall -Wextra -std=c99 -Iinclude "$srcfile" -o "$objfile" 2>/dev/null; then
            echo -e "${GREEN}OK${NC}"
        else
            echo -e "${RED}ERRO${NC}"
            echo -e "${RED}Detalhes do erro:${NC}"
            gcc -c -Wall -Wextra -std=c99 -Iinclude "$srcfile" -o "$objfile"
            COMPILE_ERROR=1
        fi
    fi
done

# Verificar se houve erros na compilação
if [ $COMPILE_ERROR -eq 1 ]; then
    echo -e "${RED}Falha na compilação!${NC}"
    exit 1
fi

# Verificar se todos os arquivos objeto foram criados
echo -e "${BLUE}Verificando arquivos objeto...${NC}"
EXPECTED_FILES=(main pontos opengl menu)
for file in "${EXPECTED_FILES[@]}"; do
    if [ -f "obj/${file}.o" ]; then
        echo -e "  ${file}.o: ${GREEN}✓${NC}"
    else
        echo -e "  ${file}.o: ${YELLOW}⚠ (não encontrado)${NC}"
    fi
done

# Linkar e gerar executável
echo -e "${BLUE}Linkando executável...${NC}"
if gcc obj/*.o -lGL -lGLU -lglut -o bin/$PROJECT_NAME 2>/dev/null; then
    echo -e "${GREEN}Linkagem concluída com sucesso!${NC}"
else
    echo -e "${RED}Erro ao linkar o executável!${NC}"
    echo -e "${RED}Detalhes do erro:${NC}"
    gcc obj/*.o -lGL -lGLU -lglut -o bin/$PROJECT_NAME
    exit 1
fi

echo -e "${GREEN}╔════════════════════════════════════╗${NC}"
echo -e "${GREEN}║     Compilação Bem Sucedida!       ║${NC}"
echo -e "${GREEN}╚════════════════════════════════════╝${NC}"
echo -e "${GREEN}Executável: bin/$PROJECT_NAME${NC}"

# Executar o programa
echo -e "${YELLOW}Executando programa...${NC}"
echo -e "${BLUE}(Pressione Ctrl+C para sair)${NC}"
./bin/$PROJECT_NAME
