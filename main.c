#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 100

/* 1: Estrutura de Dados para o Grafo
 * Objetivo: Criar a estrutura de dados que representará a rede social (grafo).
 * Lista de adjacências para representar as conexões entre os usuários.
 * Cada usuário será representado por um ID único (número).
 * A lista de adjacências é um array de listas, onde cada índice do array representa um usuário e a lista em cada
 * índice contém os IDs dos usuários conectados.
*/

// Estrutura para representar um nó na lista de adjacência
struct AdjacencyNode {
    int id;
    struct AdjacencyNode *next;
};

// Estrutura para representar o grafo
struct Graph {
    int numUsers; // Número de usuários (nós)
    struct AdjacencyNode **adjList; // Array de listas de adjacências
};

struct Queue {
    int items[MAX_QUEUE_SIZE];
    int front;
    int rear;
};

// Função para criar um novo nó na lista de adjacências
struct AdjacencyNode *createNode(int id) {
    struct AdjacencyNode *newNode = (struct AdjacencyNode *) malloc(sizeof(struct AdjacencyNode));
    if (!newNode) exit(1); // Verificação de alocação de memória
    newNode->id = id;
    newNode->next = NULL;
    return newNode;
}

// Função para criar um grafo com um número fixo de usuários
struct Graph *createGraph(int numUsers) {
    struct Graph *graph = (struct Graph *) malloc(sizeof(struct Graph));
    if (!graph) exit(1); // Verificação de alocação de memória
    graph->numUsers = numUsers;
    graph->adjList = (struct AdjacencyNode **) malloc(numUsers * sizeof(struct AdjacencyNode *));

    // Inicializa a lista de adjacências para cada usuário como NULL
    for (int i = 0; i < numUsers; i++) {
        graph->adjList[i] = NULL;
    }

    return graph;
}

// Função para adicionar uma conexão (aresta) entre dois usuários
void addConnection(struct Graph *graph, int src, int dest) {
    // Adiciona uma conexão de src para dest
    struct AdjacencyNode *newNode = createNode(dest);
    newNode->next = graph->adjList[src];
    graph->adjList[src] = newNode;

    // Como o grafo é não direcionado, adiciona também a conexão de dest para src
    newNode = createNode(src);
    newNode->next = graph->adjList[dest];
    graph->adjList[dest] = newNode;
}

// Função para imprimir o grafo
void printGraph(struct Graph *graph) {
    for (int i = 0; i < graph->numUsers; i++) {
        struct AdjacencyNode *temp = graph->adjList[i];
        printf("User %d: ", i);
        while (temp) {
            printf("%d -> ", temp->id);
            temp = temp->next;
        }
        printf("NULL\n");
    }
}

int main() {
    int numUsers = 5; // Número de usuários para teste
    struct Graph *graph = createGraph(numUsers);

    // Adicionando algumas conexões para testar
    addConnection(graph, 0, 1);
    addConnection(graph, 0, 2);
    addConnection(graph, 1, 2);
    addConnection(graph, 1, 3);
    addConnection(graph, 2, 4);
    addConnection(graph, 3, 4);

    // Imprimindo o grafo para verificar as conexões
    printGraph(graph);

    // Libere a memória alocada para o grafo
    for (int i = 0; i < numUsers; i++) {
        struct AdjacencyNode *temp = graph->adjList[i];
        while (temp) {
            struct AdjacencyNode *toDelete = temp;
            temp = temp->next;
            free(toDelete);
        }
    }
    free(graph->adjList);
    free(graph);

    return 0;
}

/* 2: Gerar Conexões Aleatórias
 * Objetivo: Adicionar conexões aleatórias entre os usuários para simular uma rede social.
 * Gerar conexões aleatórias:
 * Para cada par de usuários, decida aleatoriamente se eles estão conectados.
 */


/* 3: Implementação do BFS para Encontrar o Menor Caminho
 * Objetivo: Implementar o algoritmo de Busca em Largura (BFS) para encontrar o menor caminho entre dois usuários.
 * Implementação do BFS:
 * Usaremos uma fila para explorar os nós (usuários) camada por camada.

 * Crie um array para rastrear se um usuário foi visitado e um array para armazenar as distâncias a partir do nó de origem.
 * Funcionalidade:
 * A função deve receber dois IDs de usuários e imprimir o caminho mais curto entre eles.
 * Caso não haja conexão, informar ao usuário.
 */


/* 4: Imprimir Conexões Mais Próximas e Mais Distantes
 * Objetivo: Imprimir a conexão mais próxima e a mais distante entre dois usuários.
 * Implementação:
 * Use o resultado da busca BFS para identificar a conexão mais próxima e mais distante.
 * Adicione funcionalidades para encontrar essas conexões e imprimi-las.
 */
