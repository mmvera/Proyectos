#ifndef __uh_H
#define __uh_H

#include <stdbool.h>
#include "GrafoSt21.h"

/* Estructura auxilar para implementar OrdenPorBloqueDeColores 
una terna para la posc del vertice, el color, y el orden que tendra cuando se ordene por color */

struct Info_vertice_actual{
    u32 posc;
    u32 color;
    u32 nombre;
};
struct Bloques{
    u32 pos_inicio;
    u32 length;
};

/* Colorea el grafo G con algun coloreo propio */

u32 Greedy(Grafo G);

/* Funcion que comprueba si el grafo tiene un coloreo propio; si tiene un coloreo propio
   devuelve true, false en caso contrario */

bool coloreoPropio(Grafo G);

/* Establece un orden aleatorio de los vetices en el orden interno del grafo G*/

char AleatorizarVertices(Grafo G,u32 R);

/* Devuelve 1 si G es bipartito y lo colorea con un coloreo propio,caso contrario 
   colorea el grafo con alun coloreo propio  devuelve 0 */

char Bipartito(Grafo G);

/* Ordena los vertices de G de acuerdo a los colores que tiene el grafo, 
   con el orden de colores dado por perm[] */
char OrdenPorBloqueDeColores(Grafo G,u32* perm);

#endif