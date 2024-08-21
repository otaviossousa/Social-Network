#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

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
    int id;                // ID do nó adjacente
    struct AdjacencyNode *next;  // Ponteiro para o próximo nó na lista
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
// id: ID do nó adjacente
struct AdjacencyNode *createNode(int id) {
    struct AdjacencyNode *newNode = (struct AdjacencyNode *) malloc(sizeof(struct AdjacencyNode));
    if (!newNode) exit(1); // Verificação de alocação de memória
    newNode->id = id;
    newNode->next = NULL;
    return newNode;
}

// Função para criar um grafo com um número fixo de usuários
// numUsers: Número total de usuários (nós)
// names: Array de nomes dos usuários
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
// graph: Ponteiro para o grafo
// src: ID do usuário de origem
// dest: ID do usuário de destino
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

/* 2-etapa: Gerar Conexões Aleatórias
 * Objetivo: Adicionar conexões aleatórias entre os usuários para simular uma rede social.
 * Gerar conexões aleatórias:
 * Para cada par de usuários, decida aleatoriamente se eles estão conectados.
 */

// Função para verificar se uma conexão entre dois usuários já existe
// graph: Ponteiro para o grafo
// src: ID do usuário de origem
// dest: ID do usuário de destino
// Retorna true se a conexão já existir, caso contrário, retorna false
bool connectionExists(struct Graph* graph, int src, int dest) {
    struct AdjacencyNode* temp = graph->adjList[src];
    while (temp) {
        if (temp->id == dest) {
            return true;  // A conexão já existe
        }
        temp = temp->next;
    }
    return false;  // A conexão não existe
}

// Função para gerar conexões aleatórias entre os usuários
// graph: Ponteiro para o grafo
// numConnections: Número desejado de conexões a serem adicionadas
void generateRandomConnections(struct Graph* graph, int numConnections) {
    // Verifica se o número solicitado de conexões excede o número máximo possível
    if (numConnections > graph->numUsers * (graph->numUsers - 1) / 2) {
        printf("Número de conexões solicitado é maior do que o máximo possível.\n");
        return;  // Se o número solicitado for maior, exibe uma mensagem de erro e retorna
    }

    srand(time(0));  // Inicializa o gerador de números aleatórios com a semente baseada no tempo atual

    int connectionsAdded = 0;  // Contador para acompanhar o número de conexões adicionadas

    // Continua tentando adicionar conexões até que o número desejado de conexões seja alcançado
    while (connectionsAdded < numConnections) {
        int src = rand() % graph->numUsers;  // Seleciona aleatoriamente um usuário de origem
        int dest = rand() % graph->numUsers;  // Seleciona aleatoriamente um usuário de destino

        // Verifica se a conexão não é uma auto-conexão e se a conexão ainda não existe
        if (src != dest && !connectionExists(graph, src, dest)) {
            addConnection(graph, src, dest);  // Adiciona a conexão ao grafo
            connectionsAdded++;  // Incrementa o contador de conexões adicionadas
        }
    }
}

// Função para contar o número total de conexões (arestas) no grafo
int countConnections(struct Graph* graph) {
    int count = 0;  // Inicializa o contador de conexões

    // Itera sobre todos os usuários no grafo
    for (int i = 0; i < graph->numUsers; i++) {
        struct AdjacencyNode* temp = graph->adjList[i];  // Pega a lista de adjacências para o usuário i

        // Itera sobre todos os nós adjacentes ao usuário i
        while (temp) {
            count++;  // Incrementa o contador de conexões para cada nó adjacente
            temp = temp->next;  // Move para o próximo nó na lista de adjacências
        }
    }

    // Como cada conexão é contada duas vezes (uma para cada direção em um grafo não direcionado),
    // dividimos o total de contagens por 2 para obter o número real de conexões
    return count / 2;
}

/* --------------------------------------------------------------------------------------------------------*/

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

    // Gerar conexões aleatórias
    generateRandomConnections(graph, 20);

    // Imprime o grafo
    printGraph(graph);

    // Contar e verificar o número de conexões
    int connectionCount = countConnections(graph);
    printf("\nNumero total de conexoes no grafo: %d\n", connectionCount);

    // Libera a memória alocada para o grafo
    freeGraph(graph);
    return 0;
}


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
