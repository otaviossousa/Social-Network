#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_QUEUE_SIZE 100
#define MAX_NAME_LENGTH 10

/* 1-etapa: Estrutura de Dados para o Grafo
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
    int numUsers;               // Número de usuários (nós)
    char** names;               // Array de nomes dos usuários
    struct AdjacencyNode** adjList;  // Array de listas de adjacências
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
struct Graph* createGraph(int numUsers, char* names[]) {
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    if (!graph) exit(1); // Verificação de alocação de memória
    graph->numUsers = numUsers;

    // Alocar e copiar os nomes dos usuários
    graph->names = (char**) malloc(numUsers * sizeof(char*));
    for (int i = 0; i < numUsers; i++) {
        graph->names[i] = (char*) malloc(MAX_NAME_LENGTH * sizeof(char));
        strncpy(graph->names[i], names[i], MAX_NAME_LENGTH);
    }

    // Alocar a lista de adjacências
    graph->adjList = (struct AdjacencyNode**) malloc(numUsers * sizeof(struct AdjacencyNode*));

    // Inicializa a lista de adjacências para cada usuário como NULL
    for (int i = 0; i < numUsers; i++) {
        graph->adjList[i] = NULL;
    }
    return graph;
}

// Função para adicionar uma conexão (aresta) entre dois usuários
void addConnection(struct Graph* graph, int src, int dest) {
    // Adiciona uma conexão de src para dest
    struct AdjacencyNode* newNode = createNode(dest);
    newNode->next = graph->adjList[src];
    graph->adjList[src] = newNode;

    // Como o grafo é não direcionado, adiciona também a conexão de dest para src
    newNode = createNode(src);
    newNode->next = graph->adjList[dest];
    graph->adjList[dest] = newNode;
}

// Função para imprimir o grafo
void printGraph(struct Graph* graph) {
    for (int i = 0; i < graph->numUsers; i++) {
        struct AdjacencyNode* temp = graph->adjList[i];
        printf("%s (%d): ", graph->names[i], i);
        while (temp) {
            printf("%s ", graph->names[temp->id]);
            temp = temp->next;
        }
        printf("\n");
    }
}

// Função para liberar a memória alocada para o grafo
void freeGraph(struct Graph* graph) {
    for (int i = 0; i < graph->numUsers; i++) {
        struct AdjacencyNode *temp = graph->adjList[i];
        while (temp) {
            struct AdjacencyNode *toDelete = temp;
            temp = temp->next;
            free(toDelete);
        }
    }
    free(graph->adjList);
    for (int i = 0; i < graph->numUsers; i++) {
        free(graph->names[i]);
    }
    free(graph->names);
    free(graph);
}

int main() {
    int numUsers = 20;
    char* names[] = {
        "Andrew", "Carlos", "Damaira", "David", "Evaldo",
        "Helena", "Hyan", "Jefte", "Jonatan", "Jose",
        "Luana", "Oresto", "Otavio", "Patrine", "Paulo",
        "Sabrina", "Samuel", "Terto", "Thalyson", "Thiago"
    };

    // Cria o grafo
    struct Graph* graph = createGraph(numUsers, names);

    // Adiciona conexões
    addConnection(graph, 0, 1);  // Andrew <-> Carlos
    addConnection(graph, 0, 2);  // Andrew <-> Damaira
    addConnection(graph, 1, 3);  // Carlos <-> David
    addConnection(graph, 2, 4);  // Damaira <-> Evaldo
    addConnection(graph, 3, 5);  // David <-> Helena
    addConnection(graph, 4, 6);  // Evaldo <-> Hyan
    addConnection(graph, 5, 7);  // Helena <-> Jefte
    addConnection(graph, 6, 8);  // Hyan <-> Jonatan
    addConnection(graph, 7, 9);  // Jefte <-> Jose
    addConnection(graph, 8, 10); // Jonatan <-> Luana
    addConnection(graph, 9, 11); // Jose <-> Oresto
    addConnection(graph, 10, 12);// Luana <-> Otavio
    addConnection(graph, 11, 13);// Oresto <-> Patrine
    addConnection(graph, 12, 14);// Otavio <-> Paulo
    addConnection(graph, 13, 15);// Patrine <-> Sabrina
    addConnection(graph, 14, 16);// Paulo <-> Samuel
    addConnection(graph, 15, 17);// Sabrina <-> Terto
    addConnection(graph, 16, 18);// Samuel <-> Thalyson
    addConnection(graph, 17, 19);// Terto <-> Thiago
    addConnection(graph, 18, 19);// Thalyson <-> Thiago

    // Imprime o grafo para verificar se as conexões estão corretas
    printf("Grafo apos adicao de conexoes:\n");
    printGraph(graph);

    // Libera a memória alocada para o grafo
    freeGraph(graph);
    return 0;
}

/* 2-etapa: Gerar Conexões Aleatórias
 * Objetivo: Adicionar conexões aleatórias entre os usuários para simular uma rede social.
 * Gerar conexões aleatórias:
 * Para cada par de usuários, decida aleatoriamente se eles estão conectados.
 */


/* 3-etapa: Implementação do BFS para Encontrar o Menor Caminho
 * Objetivo: Implementar o algoritmo de Busca em Largura (BFS) para encontrar o menor caminho entre dois usuários.
 * Implementação do BFS:
 * Usaremos uma fila para explorar os nós (usuários) camada por camada.

 * Crie um array para rastrear se um usuário foi visitado e um array para armazenar as distâncias a partir do nó de origem.
 * Funcionalidade:
 * A função deve receber dois IDs de usuários e imprimir o caminho mais curto entre eles.
 * Caso não haja conexão, informar ao usuário.
 */


/* 4-etapa: Imprimir Conexões Mais Próximas e Mais Distantes
 * Objetivo: Imprimir a conexão mais próxima e a mais distante entre dois usuários.
 * Implementação:
 * Use o resultado da busca BFS para identificar a conexão mais próxima e mais distante.
 * Adicione funcionalidades para encontrar essas conexões e imprimi-las.
 */
