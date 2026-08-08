#include "./../includes/opengl.h"
#include <stdio.h>
#include <time.h>

MetricasPerformance metricas_performance = {0};

static double __attribute__((no_instrument_function)) agora_ms(void) {
  return (double)clock() * 1000.0 / (double)CLOCKS_PER_SEC;
}

static __thread double inicio_geracao_hermite;
static __thread double inicio_geracao_bezier;
static __thread double inicio_geracao_bspline;
static __thread double inicio_geracao_catmullrom;
static __thread double inicio_desenho;

void __attribute__((no_instrument_function))
__cyg_profile_func_enter(void *func, void *caller) {
  (void)caller;
  if (func == (void *)&gerar_curva_hermite) inicio_geracao_hermite = agora_ms();
  else if (func == (void *)&gerar_curva_bezier) inicio_geracao_bezier = agora_ms();
  else if (func == (void *)&gerar_curva_bspline) inicio_geracao_bspline = agora_ms();
  else if (func == (void *)&gerar_curva_catmullrom) inicio_geracao_catmullrom = agora_ms();
  else if (func == (void *)&desenhar_curva_atual) inicio_desenho = agora_ms();
}

void __attribute__((no_instrument_function))
__cyg_profile_func_exit(void *func, void *caller) {
  (void)caller;

  if (func == (void *)&gerar_curva_hermite ||
      func == (void *)&gerar_curva_bezier ||
      func == (void *)&gerar_curva_bspline ||
      func == (void *)&gerar_curva_catmullrom) {
    double inicio = inicio_geracao_hermite;
    if (func == (void *)&gerar_curva_bezier) inicio = inicio_geracao_bezier;
    else if (func == (void *)&gerar_curva_bspline) inicio = inicio_geracao_bspline;
    else if (func == (void *)&gerar_curva_catmullrom) inicio = inicio_geracao_catmullrom;

    metricas_performance.tempo_geracao_ms += agora_ms() - inicio;
    metricas_performance.pontos_gerados += g_curva_atual.quantidade_atual;
  } else if (func == (void *)&desenhar_curva_atual) {
    metricas_performance.tempo_desenho_ms += agora_ms() - inicio_desenho;
  }

  metricas_performance.buffer_curva_uso = g_curva_atual.quantidade_atual;
  metricas_performance.buffer_curva_capacidade = g_curva_atual.quantidade_max;
  metricas_performance.buffer_controle_uso = g_clicks.quantidade_atual;
  metricas_performance.buffer_controle_capacidade = g_clicks.quantidade_max;

  if (func == (void *)&desenhar_curva_atual) {
    printf("[PERF] geracao=%.3f ms | desenho=%.3f ms | pontos=%zu | "
           "buffer curva=%zu/%zu | buffer controle=%zu/%zu\n",
           metricas_performance.tempo_geracao_ms,
           metricas_performance.tempo_desenho_ms,
           metricas_performance.pontos_gerados,
           metricas_performance.buffer_curva_uso,
           metricas_performance.buffer_curva_capacidade,
           metricas_performance.buffer_controle_uso,
           metricas_performance.buffer_controle_capacidade);
  }
}
