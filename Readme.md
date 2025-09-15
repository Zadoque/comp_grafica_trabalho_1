# 📐 Gerador de Curvas Paramétricas 2D

> **Trabalho de Computação Gráfica - UENF 2025**  
> Aplicativo interativo para geração e manipulação de curvas paramétricas fechadas com transformações geométricas.

## 🎯 Objetivo do Projeto

Desenvolvimento de uma aplicação gráfica interativa que permite:
- ✨ **Criação de curvas paramétricas** a partir de pontos definidos por clique do mouse
- 🔄 **Visualização de 4 tipos de curvas**: Hermite, Bézier, B-Spline e Catmull-Rom  
- 🎛️ **Manipulação interativa** de vértices e polígonos de controle
- 🔧 **Aplicação de transformações** geométricas em tempo real

## 🚀 Funcionalidades

### 📊 **Visualização**
- [x] Polígono de controle fechado
- [x] Pontos de controle interativos
- [x] Curva Hermite
- [x] Curva Bézier  
- [x] Curva B-Spline
- [x] Curva Catmull-Rom

### 🎮 **Interação**
- [x] Criação de pontos por clique do mouse
- [x] Sistema de coordenadas cartesianas
- [x] Menu interativo
- [ ] Seleção e movimentação de vértices
- [x] Alternância entre tipos de curva

### 🔄 **Transformações Geométricas**
- [ ] **Translação** - Mover objetos no plano
- [ ] **Rotação** - Rotacionar em torno de um ponto
- [ ] **Escala** - Redimensionar objetos
- [ ] **Espelhamento** - Reflexão em eixos
- [ ] **Shear** - Cisalhamento/inclinação

## 🏗️ Arquitetura do Projeto

```
projeto/
├── 📁 src/           # Código fonte (.c)
├── 📁 include/       # Headers (.h)
├── 📁 bin/           # Executável
├── 📁 obj/           # Arquivos objeto
├── 📁 docs/          # Documentação
└── 🚀 MakFile.sh     # Script de compilação e execução
```

## 💻 Tecnologias Utilizadas

- **Linguagem**: C (C99)
- **Gráficos**: OpenGL + GLUT
- **Compilador**: GCC
- **Plataforma**: Linux/Windows (MinGW)

## ⚡ Como Executar

```
# Clonar o repositório
git clone <seu-repositorio>
cd projeto

# Compilar e executar
chmod +x MakeFile.sh
./MakeFile
```

## 🎮 Controles

| Ação | Comando |
|------|---------|
| Adicionar ponto | Clique esquerdo |
| Alternar modo |Pelo menu Interativo|
| Limpar pontos | Ainda não é possível|
| Menu | a direita|
| Sair | ESC |


