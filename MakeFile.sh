#!/bin/bash

# Nome do projeto
PROJECT_NAME="computacao_grafica"

# Cores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${YELLOW}=== Compilando $PROJECT_NAME ===${NC}"

# Criar diretórios necessários
echo "Criando diretórios..."
mkdir -p obj bin

# Verificar se existem arquivos .c em src/
if [ ! -f src/*.c ]; then
    echo -e "${RED}Erro: Nenhum arquivo .c encontrado em src/${NC}"
    exit 1
fi

# Compilar arquivos .c individualmente
echo "Compilando arquivos fonte..."
COMPILE_ERROR=0

for srcfile in src/*.c; do
    if [ -f "$srcfile" ]; then
        objfile="obj/$(basename ${srcfile%.c}.o)"
        echo "  $(basename $srcfile) -> $(basename $objfile)"
        
        if ! gcc -c -Wall -Wextra -std=c99 -Iinclude "$srcfile" -o "$objfile"; then
            echo -e "${RED}Erro ao compilar $srcfile${NC}"
            COMPILE_ERROR=1
        fi
    fi
done

# Verificar se houve erros na compilação
if [ $COMPILE_ERROR -eq 1 ]; then
    echo -e "${RED}Falha na compilação!${NC}"
    exit 1
fi

# Linkar e gerar executável
echo "Linkando executável..."
if ! gcc obj/*.o -lGL -lGLU -lglut -o bin/$PROJECT_NAME; then
    echo -e "${RED}Erro ao linkar o executável!${NC}"
    exit 1
fi

echo -e "${GREEN}Compilação concluída com sucesso!${NC}"
echo -e "${GREEN}Executável gerado: bin/$PROJECT_NAME${NC}"

# Executar o programa
echo -e "${YELLOW}Executando programa...${NC}"
./bin/$PROJECT_NAME
