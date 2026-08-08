#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include <stddef.h>
#include "menu.h"

#define PERFORMANCE_HISTORY_MAX 64

typedef struct {
    char timestamp[32];
    char machine_id[80];
    char processor[160];
    char memory[64];
    char gpu[160];
    char architecture[32];
    char os[80];
    char curve[32];
    double generation_ms;
    double drawing_ms;
    size_t points_generated;
    size_t buffer_used;
    size_t buffer_capacity;
} PerformanceMetric;

void performance_init(void);
void performance_shutdown(void);
void performance_begin_generation(void);
void performance_end_generation(Curva curva, size_t points_generated,
                                size_t buffer_used, size_t buffer_capacity);
void performance_begin_drawing(void);
void performance_end_drawing(void);
const PerformanceMetric *performance_current(void);
int performance_history(PerformanceMetric *out, int max_items);
const char *performance_processor(void);
const char *performance_memory(void);
const char *performance_gpu(void);
const char *performance_machine_id(void);

#endif
