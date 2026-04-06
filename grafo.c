/*
 * REPRESENTACAO DE GRAFOS - Versao 2026/1
 */


//Gabriel Erick Mendes 10420391
//Camila Nunes Carniel 10338558
//BRUNO GERMANETTI RAMALHO 10426491
// MIGUEL PINEIRO CORATOLO SIMOES 10427085


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <memory.h>
#include <string.h>

/*
 * Estrutura de dados para representar grafos
 */
typedef struct a
{ /* Celula de uma lista de arestas */
    int vizinho;
    struct a *prox;
} Aresta;

typedef struct v
{ /* Cada vertice tem um ponteiro para uma lista de arestas incidentes nele */
    int nome;
    Aresta *prim;
} Vertice;

/*
 * Declaracoes das funcoes para manipulacao de grafos
 */
void criaGrafo(Vertice **G, int ordem);
void destroiGrafo(Vertice **G, int ordem);
int acrescentaAresta(Vertice G[], int ordem, int v1, int v2);
void imprimeGrafo(Vertice G[], int ordem);
void retiraVertice(Vertice vertice, Vertice G[], int ordem);
void colocaVertice(Vertice G[], int ordem, int arestas[], int qntArestas, Vertice vertice);

/*
 * Criacao de um grafo com ordem predefinida (passada como argumento),
 *   e, inicilamente, sem nenhuma aresta
 */
void criaGrafo(Vertice **G, int ordem)
{
    int i;
    *G = (Vertice *)malloc(sizeof(Vertice) * ordem); /* Alcacao dinamica de um vetor de vertices */

    for (i = 0; i < ordem; i++)
    {
        (*G)[i].nome = i;
        (*G)[i].prim = NULL; /* Cada vertice sem nenhuma aresta incidente */
    }
}

/*
 * Deasaloca a memoria dinamica usada para armazenar um grafo.
 */
void destroiGrafo(Vertice **G, int ordem)
{
    int i;
    Aresta *a, *n;

    for (i = 0; i < ordem; i++)
    { /* Remove lista de adjacencia de cada vertice */
        a = (*G)[i].prim;
        while (a != NULL)
        {
            n = a->prox;
            free(a);
            a = n;
        }
    }
    free(*G); /* Remove o vetor de vertices */
}

/*
 * Acrescenta uma nova aresta em um grafo previamente criado.
 *   Devem ser passados os extremos v1 e v2 da aresta a ser acrescentada
 * Como o grafo nao e orientado, para uma aresta com extremos i e j, quando
 *   i != j, serao criadas, na estrutura de dados, arestas (i,j) e (j,i) .
 */
int acrescentaAresta(Vertice G[], int ordem, int v1, int v2)
{
    Aresta *A1, *A2;

    if (v1 < 0 || v1 >= ordem) /* Testo se vertices sao validos */
        return 0;
    if (v2 < 0 || v2 >= ordem)
        return 0;

    /* Acrescento aresta na lista do vertice v1 */
    A1 = (Aresta *)malloc(sizeof(Aresta));
    A1->vizinho = v2;
    A1->prox = G[v1].prim;
    G[v1].prim = A1;

    if (v1 == v2)
        return 1; /* Aresta e um laco */

    /* Acrescento aresta na lista do vertice v2 se v2 != v1 */
    A2 = (Aresta *)malloc(sizeof(Aresta));
    A2->vizinho = v1;
    A2->prox = G[v2].prim;
    G[v2].prim = A2;

    return 1;
}

/* Recebe um grafo G não vazio como argumento e retorna:
 * 1, se o grafo for conexo; ou
 * 0, se o grafo não for conexo.
 */
int eConexo(Vertice G[], int ordemG)
{
    if (ordemG <= 0)
        return 0; // Proteção para grafos vazios ou inválidos

    // aloca vetor para armazenar as marcações (0 ou 1)
    int *marcados = (int *)calloc(sizeof(int), ordemG);
    int i;

    // Passos 1 e 2: Marca v0 com 1 e todos os demais com 0
    marcados[0] = 1;

    // Passo 3: Repita até que nenhum novo vértice seja marcado
    int novo_marcado;
    do
    {
        novo_marcado = 0; // Flag para rastrear se houve mudança na iteração atual

        for (i = 0; i < ordemG; i++)
        {
            // a) Se existir uma aresta que incide em um vértice vi marcado com 1
            if (marcados[i] == 1)
            {
                Aresta *aux = G[i].prim;
                while (aux != NULL)
                {
                    int vj = aux->vizinho;
                    if (marcados[vj] == 0)
                    {
                        marcados[vj] = 1;
                        novo_marcado = 1; // registra que uma nova marcação foi feita
                    }
                    aux = aux->prox;
                }
            }
        }
    } while (novo_marcado); // Repete enquanto a flag indicar mudanças

    // Passo 4: Se todos os vértices estiverem marcados com 1, retorne 1; senão 0
    int conexo = 1;
    for (i = 0; i < ordemG; i++)
    {
        if (marcados[i] == 0)
        {
            conexo = 0; // Encontrou um vértice não alcançado
            break;
        }
    }

    // Libera a memória alocada dinamicamente para o vetor auxiliar
    free(marcados);

    return conexo;
}

int componetesGrafo(Vertice G[], int ordem)
{
    if (ordem <= 0)
        return 0; // Proteção para grafos vazios ou inválidos

    // aloca vetor para armazenar as marcações (0 ou 1)
    int *marcados = (int *)calloc(sizeof(int), ordem);
    int i;

    // Passos 1 e 2: Marca v0 com 1 e todos os demais com 0
    marcados[0] = 1;

    // Passo 3: Repita até que nenhum novo vértice seja marcado
    int novo_marcado;
    int acabou_componentes;
    int qnt_Componentes = 0;
    do
    {
        acabou_componentes = 1;
        do
        {
            novo_marcado = 0; // Flag para rastrear se houve mudança na iteração atual

            for (i = 0; i < ordem; i++)
            {
                // a) Se existir uma aresta que incide em um vértice vi marcado com 1
                if (marcados[i] == 1)
                {
                    Aresta *aux = G[i].prim;
                    while (aux != NULL)
                    {
                        int vj = aux->vizinho;
                        if (marcados[vj] == 0)
                        {
                            marcados[vj] = 1;
                            novo_marcado = 1; // registra que uma nova marcação foi feita
                        }
                        aux = aux->prox;
                    }
                }
            }
        } while (novo_marcado); // Repete enquanto a flag indicar mudanças

        // Incrimenta componentes
        qnt_Componentes++;

        // Passo 4: Se todos os vértices estiverem marcados com 1, retorne 1; senão 0
        for (i = 0; i < ordem; i++)
        {
            if (marcados[i] == 0)
            {
                acabou_componentes = 0; // Encontrou um vértice não alcançado
                marcados[i] = 1;        // Marca novo componente com 1 e reiniciar a contagem de componentes
                break;
            }
        }
    } while (acabou_componentes == 0);

    // Libera a memória alocada dinamicamente para o vetor auxiliar
    free(marcados);

    return qnt_Componentes;
}

/* Remove uma aresta especifica do grafo (de ambos os lados, pois nao e orientado) */
void retiraAresta(Vertice G[], int v1, int v2)
{
    Aresta *atual, *ant;

    // Remove v2 da lista de vizinhos de v1
    atual = G[v1].prim;
    ant = NULL;
    while (atual != NULL && atual->vizinho != v2)
    {
        ant = atual;
        atual = atual->prox;
    }
    if (atual != NULL)
    {
        if (ant == NULL)
            G[v1].prim = atual->prox;
        else
            ant->prox = atual->prox;
        free(atual);
    }

    if (v1 == v2)
        return; // Se for um laco, finaliza aqui

    // Remove v1 da lista de vizinhos de v2
    atual = G[v2].prim;
    ant = NULL;
    while (atual != NULL && atual->vizinho != v1)
    {
        ant = atual;
        atual = atual->prox;
    }
    if (atual != NULL)
    {
        if (ant == NULL)
            G[v2].prim = atual->prox;
        else
            ant->prox = atual->prox;
        free(atual);
    }
}

/*
 * Imprime um grafo com uma notacao similar a uma lista de adjacencia.
 */
void imprimeGrafo(Vertice G[], int ordem)
{
    int i;
    Aresta *aux;

    printf("\nOrdem:   %d", ordem);
    printf("\nLista de Adjacencia:\n");

    for (i = 0; i < ordem; i++)
    {
        printf("\n    v%d: ", i);
        aux = G[i].prim;
        for (; aux != NULL; aux = aux->prox)
            printf("  v%d", aux->vizinho);
    }
    printf("\n\n");
}

/* * Preenche o vetor 'vetorCorte' com os indices dos vertices de corte.
 * Retorna a quantidade de vertices de corte encontrados.
 */
int verticesDeCorte(Vertice G[], int ordem, int vetorCorte[])
{
    int qntComponentesIniciais = componetesGrafo(G, ordem);
    int qntVerticesCorte = 0;
    int i;

    // Aloca vetor temporario para guardar as arestas que será desconectada
    int *vizinhosSalvos = (int *)malloc(ordem * sizeof(int));

    for (i = 0; i < ordem; i++)
    {
        int qntArestas = 0;
        Aresta *aux = G[i].prim;

        // 1. Salva os vizinhos do vertice 'i'
        while (aux != NULL)
        {
            vizinhosSalvos[qntArestas++] = aux->vizinho;
            aux = aux->prox;
        }

        // 2. Remove todas as arestas incidentes em i
        int j;
        for (j = 0; j < qntArestas; j++)
        {
            retiraAresta(G, i, vizinhosSalvos[j]);
        }

        // 3. Verifica se o grafo se partiu
        int novosComponentes = componetesGrafo(G, ordem);

        /* Se o vertice se isolou E separou outros componentes,
           a diferenca de componentes eh >= 2 */
        if (novosComponentes >= qntComponentesIniciais + 2)
        {
            vetorCorte[qntVerticesCorte++] = i;
        }

        // 4. Recoloca as arestas originais para testar o proximo vertice
        for (j = 0; j < qntArestas; j++)
        {
            acrescentaAresta(G, ordem, i, vizinhosSalvos[j]);
        }
    }

    free(vizinhosSalvos);
    return qntVerticesCorte;
}

/*
 * Preenche os vetores arestaV1 e arestaV2 com os pares de arestas de corte.
 * Retorna a quantidade de arestas de corte encontradas.
 */
int arestasDeCorte(Vertice G[], int ordem, int arestaV1[], int arestaV2[])
{
    int qntComponentesIniciais = componetesGrafo(G, ordem);
    int qntArestasCorte = 0;
    int i;

    for (i = 0; i < ordem; i++)
    {
        Aresta *aux = G[i].prim;
        while (aux != NULL)
        {
            int vizinho = aux->vizinho;

            // Verifica apenas se i < vizinho para nao testar a mesma aresta duas vezes
            if (i < vizinho)
            {
                // Remove a aresta
                retiraAresta(G, i, vizinho);

                // Verifica componentes
                if (componetesGrafo(G, ordem) > qntComponentesIniciais)
                {
                    arestaV1[qntArestasCorte] = i;
                    arestaV2[qntArestasCorte] = vizinho;
                    qntArestasCorte++;
                }

                // Recoloca a aresta
                acrescentaAresta(G, ordem, i, vizinho);
            }
            aux = aux->prox;
        }
    }
    return qntArestasCorte;
}

/*
 * Programa simples para testar a representacao de grafo
 */
int main(int argc, char *argv[])
{
    int i;
    Vertice *G;
    int ordemG = 4; /* Vertices identificado de 0 ate 9 */

    criaGrafo(&G, ordemG);
    acrescentaAresta(G, ordemG, 0, 2);
    acrescentaAresta(G, ordemG, 0, 3);

    imprimeGrafo(G, ordemG);

    //=========================Componentes==============================
    int componentes = componetesGrafo(G, ordemG);
    printf("Quantidade de componentes conexas: %d\n\n", componentes);
    //============================================================

    //=========================Vertices de Corte=========================
    int *vCorte = (int *)malloc(ordemG * sizeof(int));
    int qntVCorte = verticesDeCorte(G, ordemG, vCorte);

    printf("Vertices de corte encontrados: %d\n", qntVCorte);
    for (int i = 0; i < qntVCorte; i++)
    {
        printf("> Vertice v%d\n", vCorte[i]);
    }
    printf("\n");
    free(vCorte);
    //============================================================

    //=========================Arestas de Corte=========================
    int maxArestas = (ordemG * (ordemG - 1)) / 2; 
    int *aCorteV1 = (int*) malloc(maxArestas * sizeof(int));
    int *aCorteV2 = (int *)malloc(maxArestas * sizeof(int));
    
    int qntACorte = arestasDeCorte(G, ordemG, aCorteV1, aCorteV2);
    
    printf("Arestas de corte encontradas: %d\n", qntACorte);
    for (int i = 0; i < qntACorte; i++) {
        printf("> Aresta (v%d, v%d)\n", aCorteV1[i], aCorteV2[i]);
    }
    printf("\n");
    
    free(aCorteV1);
    free(aCorteV2);
    //============================================================

    
    destroiGrafo(&G, ordemG);

    printf("Pressione uma tecla para terminar\n");
    getchar();
    return (0);
}
