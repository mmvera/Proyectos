#ifndef _GRAFOST21_H
#define _GRAFOST21_H 

typedef unsigned int  u32;

typedef struct GrafoSt* Grafo;

struct GrafoSt{

   u32 nro_vertices;
   u32 nro_lados;
   u32 delta;
   struct vertice* a_vertices;
   u32* orden_greedy;
};

struct vertice{
    
    u32 nombre;
    u32 color;
    u32 grado;
    u32 posc;
    u32 posc_actual;
    struct vecino* lista_vecinos;
};

struct vecino{
    u32 pos;
    u32 peso;
};

struct arista{
    u32 v1;
    u32 v2;
};

#endif