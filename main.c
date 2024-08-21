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

// Estrutura para representar um usuário
typedef struct User {
    int id;
    char nome[MAX_NAME_LENGTH];
} User;

// Estrutura para representar um nó na lista de adjacência
typedef struct AdjacencyNode {
    struct User* user;  // Ponteiro para o usuário conectado
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
    int items[MAX_QUEUE_SIZE];
    int front;
    int rear;
} Queue;

// Função para criar um novo nó na lista de adjacências
// id: ID do nó adjacente
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
    if (!graph) {
        perror("Erro na alocação de memória");
        exit(EXIT_FAILURE);
    }
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
bool connectionExists(Graph* graph, int src, int dest) {
    AdjacencyNode* temp = graph->adjList[src];
    while (temp) {
        if (temp->user->id == dest) {
            return true;  // A conexão já existe
        }
        temp = temp->next;
    }
    return false;  // A conexão não existe
}

// Função para gerar conexões aleatórias entre os usuários
// graph: Ponteiro para o grafo
// numConnections: Número desejado de conexões a serem adicionadas
void generateRandomConnections(Graph* graph, int numConnections) {
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
// graph: Ponteiro para o grafo
int countConnections(Graph* graph) {
    int count = 0;  // Inicializa o contador de conexões

    // Itera sobre todos os usuários no grafo
    for (int i = 0; i < graph->numUsers; i++) {
        AdjacencyNode* temp = graph->adjList[i];  // Pega a lista de adjacências para o usuário i

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

/* 3-etapa: Implementação do BFS para Encontrar o Menor Caminho
 * Objetivo: Implementar o algoritmo de Busca em Largura (BFS) para encontrar o menor caminho entre dois usuários.
 * Implementação do BFS:
 * Usaremos uma fila para explorar os nós (usuários) camada por camada.

 * Crie um array para rastrear se um usuário foi visitado e um array para armazenar as distâncias a partir do nó de origem.
 * Funcionalidade:
 * A função deve receber dois IDs de usuários e imprimir o caminho mais curto entre eles.
 * Caso não haja conexão, informar ao usuário.
 */

// Função para inicializar a fila
Queue* createQueue() {
    Queue* queue = (Queue*) malloc(sizeof(Queue));
    queue->front = -1;
    queue->rear = -1;
    return queue;
}

// Verifica se a fila está vazia
int isEmpty(Queue* queue) {
    if (queue->rear == -1)
        return 1;
    return 0;
}

// Função para adicionar um elemento na fila
int enqueue(Queue* queue, int value) {
    if (queue->rear == MAX_QUEUE_SIZE - 1) {
        // Fila cheia
        return 0;  // Retorna 0 para indicar falha
    } else {
        if (queue->front == -1) {
            queue->front = 0;
        }
        queue->rear++;
        queue->items[queue->rear] = value;
        return 1;  // Retorna 1 para indicar sucesso
    }
}

// Função para remover um elemento da fila
int dequeue(Queue* queue) {
    if (isEmpty(queue)) {
        // Fila vazia
        return -1;  // Retorna um valor especial para indicar que a fila está vazia
    } else {
        int item = queue->items[queue->front];
        queue->front++;
        if (queue->front > queue->rear) {
            // Reinicializa a fila quando todos os elementos são removidos
            queue->front = -1;
            queue->rear = -1;
        }
        return item;
    }
}

// Função para liberar a memória alocada para a fila
void freeQueue(Queue* queue) {
    if (queue != NULL) {
        free(queue);  // Libera a memória alocada para a estrutura Queue
        queue = NULL; // Define o ponteiro como NULL para evitar referências pendentes
    }
}

void bfsMenorCaminho(struct Graph* graph, int startVertex, int finalVertex) {
    Queue* q = createQueue();
    int* predecessor = malloc(graph->numUsers * sizeof(int));  // Array para armazenar predecessores
    int* distance = malloc(graph->numUsers * sizeof(int));    // Array para armazenar as distâncias

    // Inicializa todos os vértices como não visitados, sem predecessores e com distância infinita
    for (int i = 0; i < graph->numUsers; i++) {
        graph->visited[i] = 0;
        predecessor[i] = -1;
        distance[i] = -1;
    }

    // Configura o vértice inicial
    graph->visited[startVertex] = 1;
    distance[startVertex] = 0;
    enqueue(q, startVertex);

    while (!isEmpty(q)) {
        int currentVertex = dequeue(q);
        struct AdjacencyNode* temp = graph->adjList[currentVertex];

        while (temp != NULL) {
            int adjVertex = temp->user->id;

            if (graph->visited[adjVertex] == 0) {
                graph->visited[adjVertex] = 1;
                distance[adjVertex] = distance[currentVertex] + 1;
                predecessor[adjVertex] = currentVertex;
                enqueue(q, adjVertex);

                if (adjVertex == finalVertex) {
                    // Reconstruir o caminho usando uma pilha
                    int path[graph->numUsers];
                    int top = -1;
                    int crawl = finalVertex;

                    while (crawl != -1) {
                        path[++top] = crawl;
                        crawl = predecessor[crawl];
                    }

                    printf("\nCaminho mais curto: ");
                    while (top >= 0) {
                        printf("%s", graph->users[path[top--]]->nome);
                        if (top >= 0) {
                            printf(" -> ");
                        }
                    }
                    printf("\nDistancia: %d\n", distance[finalVertex]);

                    free(predecessor);
                    free(distance);
                    freeQueue(q);
                    return;
                }
            }
            temp = temp->next;
        }
    }

    // Se a BFS terminar e o finalVertex não foi alcançado
    printf("\nCaminho nao encontrado entre %s e %s.\n", graph->users[startVertex]->nome, graph->users[finalVertex]->nome);

    free(predecessor);
    free(distance);
    freeQueue(q);
}

void sorteiaUsuariosECalculaMenorCaminho(struct Graph* graph) {
    srand(time(NULL));

    int startVertex = rand() % graph->numUsers;
    int finalVertex;
    do {
        finalVertex = rand() % graph->numUsers;
    } while (finalVertex == startVertex);

    printf("\nUsuario: %s\n", graph->users[startVertex]->nome);
    printf("Usuario: %s\n", graph->users[finalVertex]->nome);

    bfsMenorCaminho(graph, startVertex, finalVertex);
}

/* --------------------------------------------------------------------------------------------------------*/

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

    // Gerar conexões aleatórias
    generateRandomConnections(graph, 20);

    // Imprime o grafo
    printGraph(graph);

    // Contar e verificar o número de conexões
    int connectionCount = countConnections(graph);
    printf("\nNumero total de conexoes no grafo: %d\n", connectionCount);

    // Chama a busca
    sorteiaUsuariosECalculaMenorCaminho(graph);

    // Libera a memória alocada para o grafo
    freeGraph(graph);
    return 0;
}



/* 4-etapa: Imprimir Conexões Mais Próximas e Mais Distantes
 * Objetivo: Imprimir a conexão mais próxima e a mais distante entre dois usuários.
 * Implementação:
 * Use o resultado da busca BFS para identificar a conexão mais próxima e mais distante.
 * Adicione funcionalidades para encontrar essas conexões e imprimi-las.
 */