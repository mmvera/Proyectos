#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "uh.h"
#include "GrafoSt21.h"
#include "RomaVictor.h"
#include "queue.h"

#define U32_MAX  4294967295

/* Funcion auxiliar para qsort que compara dos elementos de la estructura Info_vertice_actual */
int cmpColor(const void *p, const void *q){
    const u32 a = ((struct Info_vertice_actual *)p)->color;
    const u32 b = ((struct Info_vertice_actual *)q)->color;

    if (a < b){
        return -1;
    }
    else if (a > b){
        return 1;
    }
    else {
        return 0;
    }
} 

/* Para ordenar la copia de per en la funcion OrdenPorBloqueDeColores() */
int cmpPerm (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

/* Funcion auxiliar para qsort que compara dos elementos de la estructura Info_vertice_actual */
int cmpNombre(const void *p, const void *q){
    const u32 a = ((struct Info_vertice_actual *)p)->nombre;
    const u32 b = ((struct Info_vertice_actual *)q)->nombre;

    if (a < b){
        return -1;
    }
    else if (a > b){
        return 1;
    }
    else {
        return 0;
    }
}

/* Funcion auxilar que retorna el minimo color disponible con el que se puede colorear un vertice en particular */

static u32 min_color(u32 cant_vecinos, u32 vert, u32 cant_colores,Grafo G){

    u32 color = 0u;
    u32 min = 0u;
    bool *aux = NULL;
/* Arrelgo auxiliar para saber que colores estan usados en cada vecino del vertice que quiero colorear */    
    aux = malloc((cant_colores + 1) * sizeof(bool));

    if(aux == NULL){
        return U32_MAX;
    }
    for (u32 i = 0; i <= cant_colores; i++){
        aux[i] = false;
    }
/* Reviso el color cada vecino del vertice que quiero colorear */    
    for(u32 j = 0; j < cant_vecinos; j++){
        color = ColorVecino(j, vert, G);
        if(color != U32_MAX){ 
            aux[color] = true;
        }
    }
/* Reviso cual es el minimo color disponible */    
    while(aux[min]){
        min ++;
    }
    free(aux);
    return min;
}

u32 Greedy(Grafo G){

    u32 nro_vertices = NumeroDeVertices(G);
    u32 color;
    u32 cant_colores = 1u;
    u32 cant_vecinos;
/* Seteamos los colores de G con un color que no se usara */    
    for (u32 i = 0; i < nro_vertices; i++){  
        FijarColor(U32_MAX, i, G);    
    }
/* Recorremos todos los vecionos de cada vertice */
    for(u32 i = 0; i < nro_vertices; i++){
        
        cant_vecinos = Grado(i,G);
        color = min_color(cant_vecinos, i, cant_colores,G);
/* Si no se pudo asignar memoria en la funcion auxiliar min_color() */        
        if(color == U32_MAX){
            return U32_MAX;
        }
        FijarColor(color, i, G);
/* Verifico si hay un color mayor al acutual e incremento la cantidad de colores*/
        if(color >= cant_colores){
            cant_colores++;
        }
    }
    return cant_colores;
}

char AleatorizarVertices(Grafo G,u32 R){

    int random;
    u32 temp = 0u;
    u32 cant_vertices = NumeroDeVertices(G);
    u32 * aux = NULL; 
/*Arreglo auxiliar para hacer una aleatorizacion del orden de los vertices*/    
    aux = malloc(cant_vertices * sizeof(u32));
    
    if(aux == NULL){
        return 1;
    }
    srand(R);

    for(u32 i = 0; i < cant_vertices; i++){
        aux[i] = i;
    }
    for(u32 i = 0; i < cant_vertices; i++){
        random = rand() % cant_vertices;
        temp = aux[i];
        aux[i] = aux[random];
        aux[random] = temp;
    }
    for(u32 i = 0; i < cant_vertices; i++){
        FijarOrden(i, G, aux[i]);
    }
    free(aux);
    return 0;
}

char Bipartito(Grafo G){

    u32 cant_vert = NumeroDeVertices(G);
    u32 cant_vecinos; u32 posc_vecino; u32 sig_vertice;
    bool * aux = NULL;
/* Arreglo auxiliar para saber que vertice ya puse en la cola */    
    aux = malloc(cant_vert * sizeof(bool));
    for(u32 i = 0; i < cant_vert; i++){
        aux[i] = false;
    }
    queue cola = queue_empty();
/* Seteamos los colores de G con un color que no se usara */
    for(u32 i = 0; i < cant_vert; i++){
        FijarColor(U32_MAX,i,G);
    }

    for(u32 i=0; i < cant_vert; i++){
        if(!aux[i]){
            FijarColor(1,i,G);
            cola=queue_enqueue(cola, i);

            while(!queue_is_empty(cola)){

                sig_vertice = queue_first(cola);
                cola = queue_dequeue(cola);
                //printf("first %u -> %u\n",sig_vertice, Nombre(sig_vertice,G));
                u32 color_actual = Color(sig_vertice,G);
                cant_vecinos = Grado(sig_vertice,G);

                for (u32 i = 0u; i < cant_vecinos; i++){
                    posc_vecino = OrdenVecino(i, sig_vertice, G);

                    if(!aux[posc_vecino]){
                        //printf("enque %u -> nombre %u\n", posc_vecino,NombreVecino(i,sig_vertice,G));
                        cola = queue_enqueue(cola,posc_vecino);
                        FijarColor(1-color_actual,posc_vecino,G);
                        aux[posc_vecino] = true;
                    }
                    else if(Color(posc_vecino,G) == color_actual){
                        free(aux);
                        cola = queue_free(cola);
                        Greedy(G);
                        return 0;
                    }
                }

            }
        }
    }
    free(aux);
    cola = queue_free(cola);

    return 1;
}

/* Funcion auxiliar para verificar que efectivamente reedy haya coloreado un grafo con un coloreo propio*/
bool coloreoPropio(Grafo G){

    u32 nro_vertices = NumeroDeVertices(G);
    u32 cant_vecinos;
    bool b = true;

    for(u32 i = 0; i < nro_vertices; i++){
        cant_vecinos = Grado(i, G);
        for (u32 j = 0; j < cant_vecinos; j++){
            if(ColorVecino(j,i,G) == Color(i,G)){
                b = false;
            }
        }
    }
    return b;
}

char OrdenPorBloqueDeColores(Grafo G,u32* perm){

    u32 nro_vertices = NumeroDeVertices(G);
    struct Info_vertice_actual *copia_orden_actual = NULL;
    u32 *orden_final = NULL;
    copia_orden_actual = malloc(nro_vertices * sizeof(struct Info_vertice_actual));
    orden_final = malloc(nro_vertices * sizeof(u32));
    u32 max_color = 0;
/* Seteo de los colores y los nombres con sus posiciones en el orden interno original */
/* Aprovecho la recorrida para saber la cantidad de colores */
    for(u32 i = 0; i < nro_vertices; i++){
        copia_orden_actual[i].color = Color(i, G);
        copia_orden_actual[i].nombre = Nombre(i,G);
        if(max_color < Color(i, G)){
            max_color = Color(i, G);
        }
    }
    u32 nro_colores = max_color + 1;
/* Hago una copia de perm para verificar que sea una permutacion de la cantidad de colores de G*/    
    u32* copia_perm = NULL;
    copia_perm = malloc(nro_colores * sizeof(u32));
    for (u32 i = 0; i < nro_colores; i++){
        copia_perm[i] = perm[i];
    }
    qsort(copia_perm, nro_colores, sizeof(u32), &cmpPerm);
    for (u32 i = 0; i < nro_colores; i++){
        printf("%u, ",copia_perm[i]);
    }
    
    for (u32 i = 0; i < nro_colores; i++){
        if(copia_perm[i] != i){
            free(copia_perm);
            free(orden_final);
            free(copia_orden_actual);
            return 0;
        }
    }
    free(copia_perm);
    //Ordeno la copia segun el nombre, de esa forma el orden interno = orden natural en el que estan cargados los vert del grafo
    qsort(copia_orden_actual, nro_vertices, sizeof(struct Info_vertice_actual), cmpNombre);

    //guardo la posc actual ya que luego sera reordenado según el color
    for(u32 i = 0; i < nro_vertices; i++){
        copia_orden_actual[i].posc = i;
    }
    //ahora que se la cantidad de colores creo los bloques necesarios
    struct Bloques *array_bloques = NULL;
    array_bloques = malloc(nro_colores * sizeof(struct Bloques));
    //inicializo los campos de los bloques del array
    for(u32 i = 0; i < nro_colores; i++){
        array_bloques[i].length = 0;
    }
    //armo los bloques ordenados segun su color de menor a mayor
    qsort(copia_orden_actual, nro_vertices, sizeof(struct Info_vertice_actual), cmpColor);
    u32 color_actual = U32_MAX;

    //genero los datos para los bloques de colores
    for(u32 i = 0; i < nro_vertices;i++){
        if(copia_orden_actual[i].color != color_actual){
            color_actual = copia_orden_actual[i].color;
            array_bloques[color_actual].pos_inicio = i;
        }
        else {
            array_bloques[color_actual].length++;
        }
    }
    
    //nos va a marcar donde debemos continuar agregando información asi en total recorremos 1 sola vez
    u32 indice = 0;
    //cargo el orden por bloques en el array orden_final[], complejidad N ya que solo recorremos copia_orden_actual 1 sola vez en total
    for(u32 i = 0; i < nro_colores; i++){
        u32 pos_inicio = array_bloques[perm[i]].pos_inicio;
        orden_final[indice] = copia_orden_actual[pos_inicio].posc; //posc original de lso vertices
        indice++;
        for(u32 j = 1; j <= array_bloques[perm[i]].length;j++){
            orden_final[indice] = copia_orden_actual[pos_inicio+j].posc;
            indice++;
        }
    }
    //ahora orden_final[] tiene las posiciones
    for(u32 i = 0; i < nro_vertices; i++){
        FijarOrden(i,G,orden_final[i]);
    }
    free(orden_final);
    free(copia_orden_actual);
    free(array_bloques);
    return 1;
}

