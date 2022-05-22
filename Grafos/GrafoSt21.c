#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "GrafoSt21.h"
#include "RomaVictor.h"
#include <string.h>

static void proxima_linea(FILE* file){

    char c = 'a';
    while (c != '\n'){
        c = fgetc(file);
    }
}

int cmp(const void *p, const void *q){
    const u32 a = ((struct arista *)p)->v1;
    const u32 b = ((struct arista *)q)->v1;

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

int cmp2(const void *p, const void *q){
    const u32 a = *(u32*)p;
    const u32 b = ((struct vertice *)q)->nombre;
  
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

int buscar_p(FILE* file){

    char c = fgetc(file);
    while (c != 'p' && !feof(file)){

        proxima_linea(file);
        c = fgetc(file);
    }
    c=fgetc(file);
    if(c == ' '){
        return 0;
    }
    else{
        return 1;
    }

}

Grafo ConstruccionDelGrafo(){

    FILE * file;
    file = stdin;
    struct GrafoSt* g = NULL;
    u32 n,m,lado1,lado2;
    char edge[10];
     //busco la posicion de la 'p' dentro del file
    if(buscar_p(file) == 1){
        printf("no encontro p\n");
        return NULL;
    }
    if(fscanf(file,"%s %u %u ",edge, &n,&m) == EOF){
        printf("fscanf fallo\n");
        return NULL;
    }
    if(strcmp(edge, "edge") != 0){
        printf("fallo edge\n");
        return NULL;
    }

    g = malloc(sizeof(struct GrafoSt)); //memoria para la estructura
    assert(g!=NULL);
    g->nro_vertices = n;
    g->nro_lados = m;
    g->a_vertices =  malloc(n*sizeof(struct vertice)); //memoria para el array de vertices
    assert(g->a_vertices!=NULL);
    g->orden_greedy = malloc(n*sizeof(u32)); // memoria para el orden de greedy
    assert(g->orden_greedy!=NULL);
    for (unsigned int i = 0; i < n; i++){ //inicializo lista de vecinos y grado
        g->a_vertices[i].grado = 0;
        g->orden_greedy[i] = i;
    }
    struct arista* aux = NULL; // arreglo auxiliar para cargar todas las aristas de a forma (v1,v2) (v2,v1) 
    aux = malloc((2*m )* sizeof (struct arista));
    u32 cont_lineas = 0;
    assert(aux!=NULL);
    int j = 0;
    char e[10];
    for (unsigned int i = 0; i < m && !feof(file) ; i++){
        if(fscanf(file," %s %u %u",e, &lado1,&lado2) != EOF){
            if(strcmp(e, "e") != 0){
                printf("formato de lado malo\n");
                return NULL;
            }
            aux[j].v1 = lado1;
            aux[j].v2 = lado2;
            aux[j+1].v1 = lado2;
            aux[j+1].v2 = lado1;
            j = j + 2;
            cont_lineas++;
        }  
    }
    if(cont_lineas != m){
        DestruccionDelGrafo(g);
        g = NULL;
        return g;
    }

    qsort(aux, 2*m, sizeof(struct arista), cmp);
    int grado_vertice = 0;
    u32 sig_pos = 0u;
    for (unsigned int i = 0; i < 2*m; i++){ 
       g->a_vertices[sig_pos].nombre = aux[i].v1;
       g->a_vertices[sig_pos].posc = sig_pos;
       g->a_vertices[sig_pos].posc_actual = sig_pos;
       grado_vertice++;       
       if(i != (2*m - 1)){
           if(aux[i].v1 != aux[i+1].v1){
               g->a_vertices[sig_pos].lista_vecinos = malloc(grado_vertice*sizeof(struct vecino));
               sig_pos++;
               grado_vertice = 0;
            }
        }
        else {
            g->a_vertices[sig_pos].lista_vecinos = malloc(grado_vertice*sizeof(struct vecino));
        }
        if(sig_pos >= n){
            i = 2*m;
        }
    }

    struct vertice *posc_vecino = NULL;
    long unsigned int vecino_valor;
    sig_pos = 0u;
    u32 Delta = 0u;
    
    for (unsigned int i = 0; i < (2*m); i++){
        u32 key = aux[i].v2;
        posc_vecino = bsearch(&key, g->a_vertices, n, sizeof(struct vertice),cmp2);
        vecino_valor = (((unsigned long int)posc_vecino - (unsigned long int)(g->a_vertices)) / (sizeof(struct vertice)));
       
        g->a_vertices[sig_pos].lista_vecinos[g->a_vertices[sig_pos].grado].pos = (u32)vecino_valor;
        g->a_vertices[sig_pos].lista_vecinos[g->a_vertices[sig_pos].grado].peso = 0; 
        g->a_vertices[sig_pos].grado++;
        if(i != (2*m - 1)){
            if(aux[i].v1 != aux[i+1].v1){
                sig_pos++;
                
            }
        }
        if(g->a_vertices[sig_pos].grado > Delta){
            Delta = g->a_vertices[sig_pos].grado;
            g->delta = Delta;
        }
    }
    free(aux);
    return g;
}

u32 NumeroDeVertices(Grafo G){
    assert(G!=NULL);
    return (G->nro_vertices);
}

u32 NumeroDeLados(Grafo G){
    assert(G!=NULL);
    return (G->nro_lados);
}

void DestruccionDelGrafo(Grafo G){
    assert(G!=NULL);
    for(u32 i = 0; i < NumeroDeVertices(G);i++){
        
        free(G->a_vertices[i].lista_vecinos);    
    }
    free(G->a_vertices);
    free(G->orden_greedy);
    free(G);
    G = NULL;

}

u32 Delta(Grafo G){
    assert(G!=NULL);
    return G->delta;
}

u32 Nombre(u32 i,Grafo G){
    assert(G!=NULL);
    u32 temp;
    temp = G->orden_greedy[i];
    return G->a_vertices[temp].nombre;
    
  
}

u32 Color(u32 i,Grafo G){
    assert(G!=NULL);   
    u32 temp;
    if(i <= NumeroDeVertices(G)){
        temp = G->orden_greedy[i];
        return G->a_vertices[temp].color;
    }
    else {
        return (2^32)-1;
    }
    
}

u32 Grado(u32 i,Grafo G){
    assert(G!=NULL);
    u32 temp;
    if(i <= NumeroDeVertices(G)){
        temp = G->orden_greedy[i];
        return G->a_vertices[temp].grado;
    }
    else {
        return (2^32)-1;
    }
}

u32 ColorVecino(u32 j,u32 i,Grafo G){
    assert(G!=NULL);
    u32 vert_temp,vecino_temp;
    if(i <= NumeroDeVertices(G)){
        vert_temp = G->orden_greedy[i];
    }
    else {
        return (2^32)-1;
    }
    if(j <= Grado(i, G)){
        vecino_temp = G->a_vertices[vert_temp].lista_vecinos[j].pos;
        return G->a_vertices[vecino_temp].color;
    }
    else {
        return (2^32)-1;
    }
    
}

u32 NombreVecino(u32 j,u32 i,Grafo G){
    assert(G!=NULL);
    u32 vert_temp,vecino_temp;
    vert_temp = G->orden_greedy[i];
    vecino_temp = G->a_vertices[vert_temp].lista_vecinos[j].pos;
    return G->a_vertices[vecino_temp].nombre;
}

u32 OrdenVecino(u32 j,u32 i,Grafo G){
    assert(G!=NULL);
    u32 vert_temp,vecino_temp;
    vert_temp = G->orden_greedy[i];
    vecino_temp = G->a_vertices[vert_temp].lista_vecinos[j].pos;
    return G->a_vertices[vecino_temp].posc_actual;
}

u32 PesoLadoConVecino(u32 j,u32 i,Grafo G){
    assert(G!=NULL);
    u32 vert_temp,peso_vecino_temp;
    vert_temp = G->orden_greedy[i];
    peso_vecino_temp = G->a_vertices[vert_temp].lista_vecinos[j].peso;
    return peso_vecino_temp;
}

char FijarColor(u32 x,u32 i,Grafo G){
    assert(G!=NULL);
    u32 temp;
    if(i <= NumeroDeVertices(G)){
        temp = G->orden_greedy[i];
        G->a_vertices[temp].color = x;
        return 0;
    }
    else {
        return 1;
    }
}

char FijarOrden(u32 i,Grafo G,u32 N){
    assert(G!=NULL);    
    if(i < NumeroDeVertices(G) && N < NumeroDeVertices(G)){
        G->a_vertices[N].posc_actual = i;
        G->orden_greedy[i]= N;
        return 0;
    }
    else{
        return 1;
    }
}

u32 FijarPesoLadoConVecino(u32 j,u32 i,u32 p,Grafo G){
    assert(G!=NULL);    
    if (i < NumeroDeVertices(G) && j < Grado(i,G)){
        G->a_vertices[i].lista_vecinos[j].peso = p;
        return 0;
        
    }
    else {
        return (2^32)-1;
    }
}

Grafo CopiarGrafo(Grafo G){
    assert(G!=NULL);
    struct GrafoSt *G_nuevo = NULL;
    G_nuevo = malloc(sizeof(struct GrafoSt));

    G_nuevo->delta = Delta(G);
    G_nuevo->nro_lados = NumeroDeLados(G);
    G_nuevo->nro_vertices = NumeroDeVertices(G);
    G_nuevo->a_vertices = malloc(NumeroDeVertices(G_nuevo)*sizeof(struct vertice));
    G_nuevo->orden_greedy = malloc(NumeroDeVertices(G_nuevo)*sizeof(u32));
    for(unsigned int i = 0; i < NumeroDeVertices(G); i++){
        G_nuevo->orden_greedy[i] = G->orden_greedy[i];
        G_nuevo->a_vertices[i].nombre = G->a_vertices[i].nombre;
        G_nuevo->a_vertices[i].grado = G->a_vertices[i].grado;
        G_nuevo->a_vertices[i].lista_vecinos = malloc((G_nuevo->a_vertices[i].grado)*sizeof(struct vecino));
        G_nuevo->a_vertices[i].color = G->a_vertices[i].color;
        G_nuevo->a_vertices[i].posc_actual = G->a_vertices[i].posc_actual;

        for(u32 j = 0; j < G_nuevo->a_vertices[i].grado; j++){
            G_nuevo->a_vertices[i].lista_vecinos[j].peso = G->a_vertices[i].lista_vecinos[j].peso;
            G_nuevo->a_vertices[i].lista_vecinos[j].pos = G->a_vertices[i].lista_vecinos[j].pos;
            
        }
        
    }

    return G_nuevo;
}


