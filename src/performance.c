#define _POSIX_C_SOURCE 200809L
#include "../includes/performance.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/utsname.h>
#include <unistd.h>

#define PERFORMANCE_FILE "data/performance.csv"

static PerformanceMetric current_metric;
static PerformanceMetric history_cache[PERFORMANCE_HISTORY_MAX];
static int history_count = 0;
static struct timespec generation_start;
static struct timespec drawing_start;
static int generation_running = 0;
static int drawing_running = 0;
static int pending_record = 0;

static double elapsed_ms(struct timespec a, struct timespec b) {
    return (double)(b.tv_sec - a.tv_sec) * 1000.0 +
           (double)(b.tv_nsec - a.tv_nsec) / 1000000.0;
}

static void copy_text(char *dst, size_t size, const char *src) {
    if (!dst || size == 0) return;
    snprintf(dst, size, "%s", src ? src : "Nao identificado");
}

static void read_cpu(void) {
    FILE *f = fopen("/proc/cpuinfo", "r");
    char line[256];
    if (!f) {
        copy_text(current_metric.processor, sizeof(current_metric.processor), "Nao identificado");
        return;
    }
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "model name", 10) == 0) {
            char *colon = strchr(line, ':');
            if (colon) {
                colon++;
                while (*colon == ' ' || *colon == '\t') colon++;
                colon[strcspn(colon, "\r\n")] = '\0';
                copy_text(current_metric.processor, sizeof(current_metric.processor), colon);
                fclose(f);
                return;
            }
        }
    }
    fclose(f);
    copy_text(current_metric.processor, sizeof(current_metric.processor), "Nao identificado");
}

static void read_memory(void) {
    FILE *f = fopen("/proc/meminfo", "r");
    char line[256];
    unsigned long long kb = 0;
    if (f) {
        while (fgets(line, sizeof(line), f)) {
            if (sscanf(line, "MemTotal: %llu kB", &kb) == 1) break;
        }
        fclose(f);
    }
    if (kb > 0) {
        snprintf(current_metric.memory, sizeof(current_metric.memory), "%llu MB", (kb + 1023) / 1024);
    } else {
        copy_text(current_metric.memory, sizeof(current_metric.memory), "Nao identificado");
    }
}

static void read_gpu(void) {
    FILE *pipe = popen("lspci 2>/dev/null | grep -Ei 'VGA compatible controller|3D controller|Display controller' | head -n 1", "r");
    char line[256];
    if (pipe && fgets(line, sizeof(line), pipe)) {
        line[strcspn(line, "\r\n")] = '\0';
        copy_text(current_metric.gpu, sizeof(current_metric.gpu), line);
    } else {
        copy_text(current_metric.gpu, sizeof(current_metric.gpu), "Nao identificado");
    }
    if (pipe) pclose(pipe);
}

static void read_platform(void) {
    struct utsname info;
    if (uname(&info) == 0) {
        copy_text(current_metric.architecture, sizeof(current_metric.architecture), info.machine);
        copy_text(current_metric.os, sizeof(current_metric.os), info.sysname);
        snprintf(current_metric.machine_id, sizeof(current_metric.machine_id),
                 "%s-%s", info.nodename, info.machine);
    } else {
        copy_text(current_metric.architecture, sizeof(current_metric.architecture), "Nao identificado");
        copy_text(current_metric.os, sizeof(current_metric.os), "Nao identificado");
        copy_text(current_metric.machine_id, sizeof(current_metric.machine_id), "maquina-desconhecida");
    }
}

static const char *curve_name(Curva curva) {
    switch (curva) {
        case MODO_CURVA_HERMITE: return "Hermite";
        case MODO_CURVA_CATMULLROM: return "Catmull-Rom";
        case MODO_CURVA_BSPLINE: return "B-Spline";
        case MODO_CURVA_BEZIER: return "Bezier";
        default: return "Desconhecida";
    }
}

static void sanitize_csv_field(char *text) {
    if (!text) return;
    for (char *p = text; *p; ++p) {
        if (*p == ',') *p = ';';
        if (*p == '\n' || *p == '\r') *p = ' ';
    }
}

static void ensure_metrics_file(void) {
    FILE *f = fopen(PERFORMANCE_FILE, "r");
    if (f) {
        fclose(f);
        return;
    }
    f = fopen(PERFORMANCE_FILE, "w");
    if (!f) return;
    fprintf(f, "timestamp,machine_id,processor,memory,gpu,architecture,os,curve,generation_ms,drawing_ms,points_generated,buffer_used,buffer_capacity\n");
    fclose(f);
}

static void load_history(void) {
    FILE *f = fopen(PERFORMANCE_FILE, "r");
    char line[1024];
    if (!f) return;
    history_count = 0;
    fgets(line, sizeof(line), f);
    while (fgets(line, sizeof(line), f)) {
        PerformanceMetric m;
        memset(&m, 0, sizeof(m));
        int parsed = sscanf(line,
            "%31[^,],%79[^,],%159[^,],%63[^,],%159[^,],%31[^,],%79[^,],%31[^,],%lf,%lf,%zu,%zu,%zu",
            m.timestamp, m.machine_id, m.processor, m.memory, m.gpu,
            m.architecture, m.os, m.curve, &m.generation_ms, &m.drawing_ms,
            &m.points_generated, &m.buffer_used, &m.buffer_capacity);
        if (parsed == 13) {
            if (history_count < PERFORMANCE_HISTORY_MAX) {
                history_cache[history_count++] = m;
            } else {
                memmove(&history_cache[0], &history_cache[1],
                        sizeof(history_cache[0]) * (PERFORMANCE_HISTORY_MAX - 1));
                history_cache[PERFORMANCE_HISTORY_MAX - 1] = m;
            }
        }
    }
    fclose(f);
}

void performance_init(void) {
    memset(&current_metric, 0, sizeof(current_metric));
    read_cpu();
    read_memory();
    read_gpu();
    read_platform();
    ensure_metrics_file();
    load_history();
}

void performance_shutdown(void) {
}

void performance_begin_generation(void) {
    clock_gettime(CLOCK_MONOTONIC, &generation_start);
    generation_running = 1;
}

void performance_end_generation(Curva curva, size_t points_generated,
                                size_t buffer_used, size_t buffer_capacity) {
    struct timespec end;
    if (!generation_running) return;
    clock_gettime(CLOCK_MONOTONIC, &end);
    current_metric.generation_ms = elapsed_ms(generation_start, end);
    current_metric.points_generated = points_generated;
    current_metric.buffer_used = buffer_used;
    current_metric.buffer_capacity = buffer_capacity;
    copy_text(current_metric.curve, sizeof(current_metric.curve), curve_name(curva));
    generation_running = 0;
    pending_record = 1;
}

void performance_begin_drawing(void) {
    clock_gettime(CLOCK_MONOTONIC, &drawing_start);
    drawing_running = 1;
}

void performance_end_drawing(void) {
    struct timespec end;
    if (!drawing_running) return;
    clock_gettime(CLOCK_MONOTONIC, &end);
    current_metric.drawing_ms = elapsed_ms(drawing_start, end);
    drawing_running = 0;

    if (!pending_record) return;
    pending_record = 0;
    if (current_metric.points_generated == 0 || current_metric.curve[0] == '\0') return;

    time_t now = time(NULL);
    struct tm tm_now;
    localtime_r(&now, &tm_now);
    strftime(current_metric.timestamp, sizeof(current_metric.timestamp),
             "%Y-%m-%d %H:%M:%S", &tm_now);

    ensure_metrics_file();
    FILE *f = fopen(PERFORMANCE_FILE, "a");
    if (!f) return;
    sanitize_csv_field(current_metric.machine_id);
    sanitize_csv_field(current_metric.processor);
    sanitize_csv_field(current_metric.memory);
    sanitize_csv_field(current_metric.gpu);
    sanitize_csv_field(current_metric.architecture);
    sanitize_csv_field(current_metric.os);
    sanitize_csv_field(current_metric.curve);
    fprintf(f, "%s,%s,%s,%s,%s,%s,%s,%s,%.6f,%.6f,%zu,%zu,%zu\n",
            current_metric.timestamp, current_metric.machine_id,
            current_metric.processor, current_metric.memory, current_metric.gpu,
            current_metric.architecture, current_metric.os, current_metric.curve,
            current_metric.generation_ms, current_metric.drawing_ms,
            current_metric.points_generated, current_metric.buffer_used,
            current_metric.buffer_capacity);
    fclose(f);
    load_history();
}

const PerformanceMetric *performance_current(void) {
    return &current_metric;
}

int performance_history(PerformanceMetric *out, int max_items) {
    if (!out || max_items <= 0) return 0;
    load_history();
    int n = history_count < max_items ? history_count : max_items;
    for (int i = 0; i < n; ++i) {
        out[i] = history_cache[history_count - 1 - i];
    }
    return n;
}

const char *performance_processor(void) { return current_metric.processor; }
const char *performance_memory(void) { return current_metric.memory; }
const char *performance_gpu(void) { return current_metric.gpu; }
const char *performance_machine_id(void) { return current_metric.machine_id; }
