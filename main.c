#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_QUEUE_SIZE 100
#define MAX_NAME_LENGTH 10

/*
1-tapa: Estrutura de Dados para o Grafo

Descrição:
- A rede social é representada por um grafo, onde cada usuário é um nó.
- As conexões entre os usuários são representadas por arestas.
- Utiliza-se uma lista de adjacências para representar as conexões entre os usuários.
- Cada usuário é identificado por um ID único e um nome.

Estruturas:
- `User`: Representa um usuário com um ID único e um nome.
- `AdjacencyNode`: Representa um nó na lista de adjacência que aponta para um usuário conectado.
- `Graph`: Representa o grafo, contendo um array de usuários e um array de listas de adjacência.
- `Queue`: Representa uma fila usada para algoritmos de busca.

Funções:
- `createNode`: Cria um novo nó na lista de adjacências.
- `createGraph`: Cria um grafo com um número fixo de usuários.
- `addConnection`: Adiciona uma conexão entre dois usuários no grafo.
*/

// Estrutura para representar um usuário
typedef struct User {
    int id;  // ID único do usuário
    char nome[MAX_NAME_LENGTH];  // Nome do usuário
} User;

// Estrutura para representar um nó na lista de adjacência
typedef struct AdjacencyNode {
    User* user;  // Ponteiro para o usuário conectado
    struct AdjacencyNode* next;  // Ponteiro para o próximo nó adjacente
} AdjacencyNode;

// Estrutura para representar o grafo
typedef struct Graph {
    User** users;  // Array de ponteiros para os usuários
    AdjacencyNode** adjList;  // Array de listas de adjacências
    int numUsers;  // Número de usuários (nós)
    int* visited;  // Array para rastrear os usuários visitados
} Graph;

// Estrutura para representar uma fila
typedef struct Queue {
    int items[MAX_QUEUE_SIZE];  // Array de itens da fila
    int front;  // Índice do primeiro item
    int rear;  // Índice do último item
} Queue;

// Função para criar um novo nó na lista de adjacências
// user: Ponteiro para o usuário que será adicionado ao nó
AdjacencyNode* createNode(User* user) {
    AdjacencyNode* newNode = (AdjacencyNode*) malloc(sizeof(AdjacencyNode));
    if (!newNode) exit(1);  // Verificação de alocação de memória
    newNode->user = user;
    newNode->next = NULL;
    return newNode;
}

// Função para criar um grafo com um número fixo de usuários
// numUsers: Número total de usuários (nós)
// names: Array de nomes dos usuários
Graph* createGraph(int numUsers, char* names[]) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (!graph) exit(1);  // Verificação de alocação de memória
    graph->numUsers = numUsers;

    graph->users = (User**)malloc(numUsers * sizeof(User*));
    for (int i = 0; i < numUsers; i++) {
        graph->users[i] = (User*)malloc(sizeof(User));
        graph->users[i]->id = i;
        strncpy(graph->users[i]->nome, names[i], MAX_NAME_LENGTH);
    }

    graph->adjList = (AdjacencyNode**)malloc(numUsers * sizeof(AdjacencyNode*));
    graph->visited = (int*)malloc(numUsers * sizeof(int));

    for (int i = 0; i < numUsers; i++) {
        graph->adjList[i] = NULL;
        graph->visited[i] = 0;
    }

    return graph;
}

// Função para adicionar uma conexão (aresta) entre dois usuários
// graph: Ponteiro para o grafo
// src: ID do usuário de origem
// dest: ID do usuário de destino
void addConnection(Graph* graph, int src, int dest) {
    // Adiciona uma conexão de src para dest
    AdjacencyNode* newNode = createNode(graph->users[dest]);
    newNode->next = graph->adjList[src];
    graph->adjList[src] = newNode;

    // Como o grafo é não direcionado, adiciona também a conexão de dest para src
    newNode = createNode(graph->users[src]);
    newNode->next = graph->adjList[dest];
    graph->adjList[dest] = newNode;
}

/*
Funcoes Auxiliares:

1. `printGraph`
    - Objetivo:
      - Imprimir a representação do grafo na tela.
    - Descrição:
      - Esta função itera sobre todos os vértices do grafo e imprime o nome e o ID de cada usuário.
        Para cada usuário, a função também imprime as conexões (arestas) com outros usuários.
        Se um usuário não tiver conexões, a função imprime "(nenhuma conexão)".
        A função usa uma flag (`firstConnection`) para adicionar vírgulas entre as conexões, exceto antes da primeira conexão.

2. `freeGraph`
    - Objetivo:
      - Liberar toda a memória alocada para o grafo.
    - Descrição:
      - Esta função percorre a lista de adjacências de cada usuário e libera a memória associada a essas listas.
        Depois, libera a memória da lista de adjacências e dos próprios usuários.
        Por fim, a função libera a memória alocada para a estrutura do grafo em si.
        A função é importante para evitar vazamentos de memória ao desalocar todos os recursos utilizados pelo grafo.
*/


// Função para imprimir o grafo
// graph: Ponteiro para o grafo
void printGraph(Graph* graph) {
    for (int i = 0; i < graph->numUsers; i++) {
        printf("%s (%d): ", graph->users[i]->nome, graph->users[i]->id);
        AdjacencyNode* temp = graph->adjList[i];

        // Se a lista de adjacências estiver vazia, apenas imprima "(nenhuma conexão)"
        if (temp == NULL) {
            printf("(nenhuma conexao)");
        } else {
            int firstConnection = 1;  // Flag para a primeira conexão
            while (temp) {
                if (!firstConnection) {
                    printf(", ");  // Adiciona uma vírgula entre as conexões
                }
                printf("%s", temp->user->nome);
                firstConnection = 0;
                temp = temp->next;
            }
        }
        printf("\n");
    }
}

// Função para liberar a memória alocada para o grafo
// graph: Ponteiro para o grafo
void freeGraph(Graph* graph) {
    for (int i = 0; i < graph->numUsers; i++) {
        AdjacencyNode* temp = graph->adjList[i];
        while (temp) {
            AdjacencyNode* toDelete = temp;
            temp = temp->next;
            free(toDelete);
        }
    }
    free(graph->adjList);

    // Libera a memória alocada para os usuários
    for (int i = 0; i < graph->numUsers; i++) {
        free(graph->users[i]);
    }
    free(graph->users);

    free(graph);
}

int main() {
    char* names[] = {
        "Andrew", "Carlos", "Damaira", "David", "Evaldo",
        "Helena", "Hyan", "Jefte", "Jonatan", "Jose",
        "Luana", "Oresto", "Otavio", "Patrine", "Paulo",
        "Sabrina", "Samuel", "Terto", "Thalyson", "Thiago"
    };

    // numUsers = 160 / 8 = 20
    int numUsers = sizeof(names) / sizeof(names[0]);
    Graph* graph = createGraph(numUsers, names);

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
    printGraph(graph);

    // Libera a memória alocada para o grafo
    freeGraph(graph);

    return 0;
}
