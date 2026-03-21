/* 
 * REPRESENTACAO DE GRAFOS - Versao 2026/1
 */

#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<memory.h>


/* 
 * Estrutura de dados para representar grafos
 */
typedef struct a{ /* Celula de uma lista de arestas */
	int    vizinho;
	struct a *prox;
}Aresta;

typedef struct v{  /* Cada vertice tem um ponteiro para uma lista de arestas incidentes nele */
	int nome;
	Aresta *prim;
}Vertice;

/*
 * Declaracoes das funcoes para manipulacao de grafos 
 */
void criaGrafo(Vertice **G, int ordem);
void destroiGrafo(Vertice **G, int ordem);
int  acrescentaAresta(Vertice G[], int ordem, int v1, int v2);
void imprimeGrafo(Vertice G[], int ordem);
 

/*
 * Criacao de um grafo com ordem predefinida (passada como argumento),
 *   e, inicilamente, sem nenhuma aresta 
 */
void criaGrafo(Vertice **G, int ordem){
	int i;
	*G= (Vertice*) malloc(sizeof(Vertice)*ordem); /* Alcacao dinamica de um vetor de vertices */
	
	for(i=0; i<ordem; i++){
		(*G)[i].nome= i;
		(*G)[i].prim= NULL;    /* Cada vertice sem nenhuma aresta incidente */
	}
}

/*
 * Deasaloca a memoria dinamica usada para armazenar um grafo.
 */
void destroiGrafo(Vertice **G, int ordem){
	int i;
    Aresta *a, *n;
    
	for(i=0; i<ordem; i++){ /* Remove lista de adjacencia de cada vertice */
	    a= (*G)[i].prim;
        while (a!= NULL){
              n= a->prox;
              free(a);
              a= n;
        }
	}
    free(*G);  /* Remove o vetor de vertices */
}

/*  
 * Acrescenta uma nova aresta em um grafo previamente criado. 
 *   Devem ser passados os extremos v1 e v2 da aresta a ser acrescentada 
 * Como o grafo nao e orientado, para uma aresta com extremos i e j, quando
 *   i != j, serao criadas, na estrutura de dados, arestas (i,j) e (j,i) .
 */
int acrescentaAresta(Vertice G[], int ordem, int v1, int v2){
    Aresta *A1, *A2;
    
	if (v1<0 || v1 >= ordem) /* Testo se vertices sao validos */
	   return 0;
	if (v2<0 || v2 >= ordem)
	   return 0;
	
	/* Acrescento aresta na lista do vertice v1 */
	A1= (Aresta *) malloc(sizeof(Aresta));
	A1->vizinho= v2;
	A1->prox= G[v1].prim;
	G[v1].prim= A1;

	if (v1 == v2) return 1; /* Aresta e um laco */

	/* Acrescento aresta na lista do vertice v2 se v2 != v1 */	
	A2= (Aresta *) malloc(sizeof(Aresta));
	A2->vizinho= v1;
	A2->prox= G[v2].prim;
	G[v2].prim= A2;
	
	return 1;
}

/* Recebe um grafo G não vazio como argumento e retorna: 
 * 1, se o grafo for conexo; ou 
 * 0, se o grafo não for conexo. 
 */ 
int eConexo(Vertice G[], int ordemG) {
    if (ordemG <= 0) return 0; // Proteção para grafos vazios ou inválidos

    // aloca vetor para armazenar as marcações (0 ou 1)
    int *marcados = (int *)calloc(sizeof(int), ordemG);
    int i;

    // Passos 1 e 2: Marca v0 com 1 e todos os demais com 0
    marcados[0] = 1; 

    // Passo 3: Repita até que nenhum novo vértice seja marcado
    int novo_marcado;
    do {
        novo_marcado = 0; // Flag para rastrear se houve mudança na iteração atual
        
        for (i = 0; i < ordemG; i++) {
            // a) Se existir uma aresta que incide em um vértice vi marcado com 1
            if (marcados[i] == 1) { 
                Aresta *aux = G[i].prim;
                while (aux != NULL) {
                    int vj = aux->vizinho;
                    if (marcados[vj] == 0) { 
                        marcados[vj] = 1;
                        novo_marcado = 1; //registra que uma nova marcação foi feita
                    }
                    aux = aux->prox;
                }
            }
        }
    } while (novo_marcado); // Repete enquanto a flag indicar mudanças

    // Passo 4: Se todos os vértices estiverem marcados com 1, retorne 1; senão 0
    int conexo = 1;
    for (i = 0; i < ordemG; i++) {
        if (marcados[i] == 0) {
            conexo = 0; // Encontrou um vértice não alcançado
            break;
        }
    }

    // Libera a memória alocada dinamicamente para o vetor auxiliar
    free(marcados);
    
    return conexo;
}



int componetesGrafo(Vertice G[], int ordem){


}

/*  
 * Imprime um grafo com uma notacao similar a uma lista de adjacencia.
 */
void imprimeGrafo(Vertice G[], int ordem){
	int i;
	Aresta *aux;

	printf("\nOrdem:   %d",ordem);
	printf("\nLista de Adjacencia:\n");

	for (i=0; i<ordem; i++){
		printf("\n    v%d: ", i);
		aux= G[i].prim;
		for( ; aux != NULL; aux= aux->prox)
			printf("  v%d", aux->vizinho);
	}
	printf("\n\n");

}

/*
 * Programa simples para testar a representacao de grafo
 */
int main(int argc, char *argv[]) {
    int i;
	Vertice *G;
	int ordemG= 10; /* Vertices identificado de 0 ate 9 */
		
	criaGrafo(&G, ordemG);
	acrescentaAresta(G,ordemG,0,1);
	acrescentaAresta(G,ordemG,0,2);
	acrescentaAresta(G,ordemG,0,7);
	acrescentaAresta(G,ordemG,2,4);
	acrescentaAresta(G,ordemG,2,2);
	acrescentaAresta(G,ordemG,2,5);
	acrescentaAresta(G,ordemG,3,5);
	acrescentaAresta(G,ordemG,4,6);
	acrescentaAresta(G,ordemG,3,6);
	acrescentaAresta(G,ordemG,7,7);
	acrescentaAresta(G,ordemG,8,9);

	imprimeGrafo(G, ordemG);
	printf("%d", eConexo(G, ordemG));
	destroiGrafo(&G, ordemG);

	printf("Pressione uma tecla para terminar\n");
    getchar();
	return(0);
}