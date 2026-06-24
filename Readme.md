# 📐 Gerador de Curvas Paramétricas 2D

> **Trabalho de Computação Gráfica — UENF 2025/2026**
> Aplicação interativa em C com OpenGL para geração, visualização e manipulação de curvas paramétricas fechadas, com suporte a picking eficiente por estrutura de árvore AABB.

> 🇺🇸 An English version of this README is available in [README_EN.md](./README_EN.md).

---

## 🎯 Objetivo

Desenvolver uma aplicação gráfica interativa que permita criar e manipular curvas paramétricas 2D fechadas a partir de pontos definidos pelo usuário, com transformações geométricas em tempo real e picking preciso por mouse.

---

## ✅ Funcionalidades Implementadas

### 📊 Curvas Suportadas
- [x] **Hermite**
- [x] **Bézier**
- [x] **B-Spline** (com subdivisão adaptativa)
- [x] **Catmull-Rom**

### 🖱️ Interação
- [x] Criação de pontos de controle por clique do mouse
- [x] Polígono de controle fechado visualizável
- [x] Seleção e movimentação de vértices (picking por mouse)
- [x] Alternância entre tipos de curva
- [x] Menu interativo lateral

### 🔄 Transformações Geométricas
- [x] Translação
- [x] Rotação em torno de um ponto
- [x] Escala
- [ ] Espelhamento
- [ ] Shear (cisalhamento)

### ⚡ Picking Eficiente (B-Spline)
- [x] Construção de **AABB Tree** hierárquica por segmento
- [x] Poda por distância mouse–caixa (`melhor_dist` propagado)
- [x] Exploração pelo filho mais próximo primeiro
- [x] Refinamento adaptativo do parâmetro `t` por subdivisão binária na folha
- [x] Visualização de debug das AABBs (por índice e por hierarquia)

---

## 🏗️ Arquitetura

```
comp_grafica_trabalho_1/
├── src/
│   ├── curvas/
│   │   └── bspline.c         # Avaliação, subdivisão adaptativa e picking
│   ├── opengl.c              # Loop principal, eventos, desenho
│   └── ...
├── includes/
│   ├── curvas/
│   │   └── bspline.h
│   ├── tipos.h               # ponto, Pontos, AABB, AABBTREE, ResultadoPicking
│   └── ...
├── MakeFile.sh               # Script de compilação e execução
└── shell.nix                 # Ambiente Nix (opcional)
```

---

## 🧠 Destaques Técnicos

### Subdivisão Adaptativa
A curva é gerada recursivamente: dados dois pontos extremos `A` e `B` no intervalo `[t0, t1]`, o ponto médio `M` é calculado. Se `M` estiver suficientemente próximo do segmento `AB` (dentro da tolerância), aceita-se o ponto. Caso contrário, subdivide-se em `[t0, tmid]` e `[tmid, t1]`. Isso garante densidade de pontos proporcional à curvatura.

### AABB Tree para Picking
Cada segmento da B-Spline possui sua própria AABB calculada a partir dos **pontos reais da curva** (não dos pontos de controle), garantindo caixas tight. Uma árvore binária hierárquica agrupa os segmentos, permitindo poda eficiente: o picking desce pela árvore priorizando o filho com caixa mais próxima do mouse e usa `melhor_dist` para descartar ramos que não podem melhorar o resultado atual.

### Refinamento de `t` na Folha
Ao chegar em uma folha candidata, `subdivide_picking_bspline` realiza subdivisão binária no intervalo `[0, 1]` do segmento, calculando a AABB de cada metade e descendo pelo lado mais próximo do mouse até `t1 - t0 < 0.001`, quando retorna o `t` e a distância real ao ponto da curva.

---

## 💻 Tecnologias

| Item | Detalhe |
|------|---------|
| Linguagem | C (C99) |
| Gráficos | OpenGL + GLUT |
| Compilador | GCC |
| Plataforma | Linux (também compilável no Windows com MinGW) |
| Ambiente opcional | Nix (`shell.nix`) |

---

## ⚡ Como Executar

```bash
# Clonar o repositório
git clone https://github.com/Zadoque/comp_grafica_trabalho_1
cd comp_grafica_trabalho_1

# Compilar e executar
chmod +x MakeFile.sh
./MakeFile.sh
```

> **Dependências:** `gcc`, `libGL`, `libGLU`, `freeglut` (ou `libglut`).
> No Nix: `nix-shell` já provê o ambiente completo.

---

## 🎮 Controles

| Ação | Comando |
|------|---------|
| Adicionar ponto de controle | Clique esquerdo |
| Selecionar / mover vértice | Clique esquerdo sobre o ponto |
| Alternar tipo de curva | Menu lateral |
| Aplicar transformação | Menu lateral |
| Sair | `ESC` |

---

## 🔧 Melhorias Planejadas

- [ ] **Arena de memória para AABBTREE** — eliminar cache miss alocando todos os nós em vetor contíguo (`2n - 1` nós para `n` segmentos), substituindo `malloc` por nó por um único `malloc` da arena
- [ ] **Pré-calcular `box_left` / `box_right`** — guardar as AABBs das duas metades de cada segmento dentro da própria struct `AABB`, eliminando o retrabalho de `criar_box_picking` a cada chamada de picking
- [ ] Espelhamento e shear
- [ ] Suporte a múltiplas curvas simultâneas

---

## 👨‍💻 Autor

**Zadoque Carneiro** — Estudante de Ciência da Computação, UENF  
[github.com/Zadoque](https://github.com/Zadoque)
