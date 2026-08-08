#define _POSIX_C_SOURCE 200809L
#include "../includes/performance.h"
#include "../includes/opengl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/utsname.h>
#include <unistd.h>

#define PERFORMANCE_FILE "data/performance.csv"
static PerformanceMetric current_metric;
static PerformanceMetric history_cache[PERFORMANCE_HISTORY_MAX];
static int history_count=0;
static struct timespec generation_start;
static struct timespec drawing_start;
static double drawing_total_ms=0.0;
static int generation_running=0;
static int drawing_depth=0;
static size_t generation_points=0;
Pontos g_nuvem_pontos;

static double elapsed_ms(struct timespec a,struct timespec b){return (double)(b.tv_sec-a.tv_sec)*1000.0+(double)(b.tv_nsec-a.tv_nsec)/1000000.0;}
static void copy_text(char*d,size_t n,const char*s){if(d&&n)snprintf(d,n,"%s",s?s:"Nao identificado");}
static void read_cpu(void){FILE*f=fopen("/proc/cpuinfo","r");char l[256];if(!f){copy_text(current_metric.processor,sizeof(current_metric.processor),"Nao identificado");return;}while(fgets(l,sizeof(l),f)){if(strncmp(l,"model name",10)==0){char*c=strchr(l,':');if(c){c++;while(*c==' '||*c=='\t')c++;c[strcspn(c,"\r\n")]=0;copy_text(current_metric.processor,sizeof(current_metric.processor),c);fclose(f);return;}}}fclose(f);copy_text(current_metric.processor,sizeof(current_metric.processor),"Nao identificado");}
static void read_memory(void){FILE*f=fopen("/proc/meminfo","r");char l[256];unsigned long long kb=0;if(f){while(fgets(l,sizeof(l),f))if(sscanf(l,"MemTotal: %llu kB",&kb)==1)break;fclose(f);}if(kb)snprintf(current_metric.memory,sizeof(current_metric.memory),"%llu MB",(kb+1023)/1024);else copy_text(current_metric.memory,sizeof(current_metric.memory),"Nao identificado");}
static void read_gpu(void){FILE*p=popen("lspci 2>/dev/null | grep -Ei 'VGA compatible controller|3D controller|Display controller' | head -n 1","r");char l[256];if(p&&fgets(l,sizeof(l),p)){l[strcspn(l,"\r\n")]=0;copy_text(current_metric.gpu,sizeof(current_metric.gpu),l);}else copy_text(current_metric.gpu,sizeof(current_metric.gpu),"Nao identificado");if(p)pclose(p);}
static void read_platform(void){struct utsname i;if(uname(&i)==0){copy_text(current_metric.architecture,sizeof(current_metric.architecture),i.machine);copy_text(current_metric.os,sizeof(current_metric.os),i.sysname);snprintf(current_metric.machine_id,sizeof(current_metric.machine_id),"%s-%s",i.nodename,i.machine);}else{copy_text(current_metric.architecture,sizeof(current_metric.architecture),"Nao identificado");copy_text(current_metric.os,sizeof(current_metric.os),"Nao identificado");copy_text(current_metric.machine_id,sizeof(current_metric.machine_id),"maquina-desconhecida");}}
static const char*curve_name(Curva c){switch(c){case MODO_CURVA_HERMITE:return "Hermite";case MODO_CURVA_CATMULLROM:return "Catmull-Rom";case MODO_CURVA_BSPLINE:return "B-Spline";case MODO_CURVA_BEZIER:return "Bezier";default:return "Desconhecida";}}
static void sanitize(char*t){if(!t)return;for(char*p=t;*p;p++){if(*p==',')*p=';';if(*p=='\n'||*p=='\r')*p=' ';}}
static void ensure_file(void){FILE*f=fopen(PERFORMANCE_FILE,"r");if(f){fclose(f);return;}f=fopen(PERFORMANCE_FILE,"w");if(!f)return;fprintf(f,"timestamp,machine_id,processor,memory,gpu,architecture,os,curve,generation_ms,drawing_ms,points_generated,buffer_used,buffer_capacity,control_buffer_used,control_buffer_capacity\n");fclose(f);}
static void load_history(void){FILE*f=fopen(PERFORMANCE_FILE,"r");char l[1024];if(!f)return;history_count=0;fgets(l,sizeof(l),f);while(fgets(l,sizeof(l),f)){PerformanceMetric m;memset(&m,0,sizeof(m));int p=sscanf(l,"%31[^,],%79[^,],%159[^,],%63[^,],%159[^,],%31[^,],%79[^,],%31[^,],%lf,%lf,%zu,%zu,%zu,%zu,%zu",m.timestamp,m.machine_id,m.processor,m.memory,m.gpu,m.architecture,m.os,m.curve,&m.generation_ms,&m.drawing_ms,&m.points_generated,&m.buffer_used,&m.buffer_capacity,&m.control_buffer_used,&m.control_buffer_capacity);if(p==15){if(history_count<PERFORMANCE_HISTORY_MAX)history_cache[history_count++]=m;else{memmove(&history_cache[0],&history_cache[1],sizeof(history_cache[0])*(PERFORMANCE_HISTORY_MAX-1));history_cache[PERFORMANCE_HISTORY_MAX-1]=m;}}}fclose(f);}
void performance_init(void){memset(&current_metric,0,sizeof(current_metric));pontos_init(&g_nuvem_pontos,500);read_cpu();read_memory();read_gpu();read_platform();ensure_file();load_history();}
void performance_shutdown(void){}
void performance_begin_generation(void){generation_points=0;clock_gettime(CLOCK_MONOTONIC,&generation_start);generation_running=1;drawing_total_ms=0;}
void performance_end_generation(Curva c,size_t points,size_t used,size_t cap){(void)used;(void)cap;if(!generation_running)return;struct timespec e;clock_gettime(CLOCK_MONOTONIC,&e);current_metric.generation_ms=elapsed_ms(generation_start,e);current_metric.points_generated=generation_points?generation_points:points;current_metric.buffer_used=g_curva_atual.quantidade_atual;current_metric.buffer_capacity=g_curva_atual.quantidade_max;current_metric.control_buffer_used=g_clicks.quantidade_atual;current_metric.control_buffer_capacity=g_clicks.quantidade_max;copy_text(current_metric.curve,sizeof(current_metric.curve),curve_name(c));generation_running=0;}
void performance_begin_drawing(void){clock_gettime(CLOCK_MONOTONIC,&drawing_start);}
void performance_end_drawing(void){struct timespec e;clock_gettime(CLOCK_MONOTONIC,&e);drawing_total_ms+=elapsed_ms(drawing_start,e);}
static void save_current(void){time_t now=time(NULL);struct tm tm_now;localtime_r(&now,&tm_now);strftime(current_metric.timestamp,sizeof(current_metric.timestamp),"%Y-%m-%d %H:%M:%S",&tm_now);current_metric.drawing_ms=drawing_total_ms;ensure_file();FILE*f=fopen(PERFORMANCE_FILE,"a");if(!f)return;sanitize(current_metric.machine_id);sanitize(current_metric.processor);sanitize(current_metric.memory);sanitize(current_metric.gpu);sanitize(current_metric.architecture);sanitize(current_metric.os);sanitize(current_metric.curve);fprintf(f,"%s,%s,%s,%s,%s,%s,%s,%s,%.6f,%.6f,%zu,%zu,%zu,%zu,%zu\n",current_metric.timestamp,current_metric.machine_id,current_metric.processor,current_metric.memory,current_metric.gpu,current_metric.architecture,current_metric.os,current_metric.curve,current_metric.generation_ms,current_metric.drawing_ms,current_metric.points_generated,current_metric.buffer_used,current_metric.buffer_capacity,current_metric.control_buffer_used,current_metric.control_buffer_capacity);fclose(f);load_history();}
void __attribute__((no_instrument_function)) __cyg_profile_func_enter(void*func,void*caller){(void)caller;if(func==(void*)&gerar_curva_selecionada){performance_begin_generation();}else if(func==(void*)&desenhar_curva_atual){if(drawing_depth++==0)clock_gettime(CLOCK_MONOTONIC,&drawing_start);}}
void __attribute__((no_instrument_function)) __cyg_profile_func_exit(void*func,void*caller){(void)caller;if(func==(void*)&gerar_curva_hermite||func==(void*)&gerar_curva_bezier||func==(void*)&gerar_curva_bspline||func==(void*)&gerar_curva_catmullrom){generation_points+=g_curva_atual.quantidade_atual;}else if(func==(void*)&desenhar_curva_atual){if(--drawing_depth==0){struct timespec e;clock_gettime(CLOCK_MONOTONIC,&e);drawing_total_ms+=elapsed_ms(drawing_start,e);}}else if(func==(void*)&gerar_curva_selecionada){performance_end_generation(estado_atual.curva,generation_points,g_curva_atual.quantidade_atual,g_curva_atual.quantidade_max);if(generation_points>0)save_current();}}
const PerformanceMetric*performance_current(void){return &current_metric;}
int performance_history(PerformanceMetric*out,int max){if(!out||max<=0)return 0;load_history();int n=history_count<max?history_count:max;for(int i=0;i<n;i++)out[i]=history_cache[history_count-1-i];return n;}
const char*performance_processor(void){return current_metric.processor;} const char*performance_memory(void){return current_metric.memory;} const char*performance_gpu(void){return current_metric.gpu;} const char*performance_machine_id(void){return current_metric.machine_id;}
