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
2-etapa: Gerar Conexões Aleatórias

Descrição:
- Conectar aleatoriamente os usuários para simular as interações em uma rede social.
- Cada conexão entre dois usuários é representada por uma aresta no grafo.
- A geração das conexões será feita de forma aleatória, respeitando as restrições de não haver auto-conexões e não duplicar conexões.

Funções:
- `connectionExists`: Verifica se uma conexão entre dois usuários já existe.
- `generateRandomConnections`: Gera e adiciona conexões aleatórias entre os usuários no grafo.
- `countConnections`: Conta o número total de conexões (arestas) presentes no grafo.
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
        printf("Numero de conexoes solicitado e maior do que o maximo possivel.\n");
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

/*
3-tapa: Implementar a Busca em Largura (BFS) para encontrar o menor caminho

Descrição:
- A função `bfsFindShortestPath` utiliza a BFS para explorar o grafo, partindo de um usuário inicial (startVertex) e procurando o caminho mais curto até o usuário final (finalVertex).
- Durante o processo, a BFS mantém rastros de predecessores e distâncias para reconstruir o caminho ao encontrar o destino.
- O uso de uma fila (queue) é essencial para gerenciar a exploração em camadas de proximidade.

Funções de Suporte:
- `createQueue`: Inicializa e retorna uma nova fila.
- `isEmpty`: Verifica se a fila está vazia.
- `enqueue`: Adiciona um elemento na fila.
- `dequeue`: Remove e retorna um elemento da fila.
- `freeQueue`: Libera a memória alocada para a fila.

Função Principal:
- `bfsFindShortestPath`: Executa a BFS para encontrar e exibir o menor caminho entre dois usuários.
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

// Função para encontrar o menor caminho entre dois usuários usando BFS
// graph: Ponteiro para o grafo
// startVertex: ID do usuário de origem
// finalVertex: ID do usuário de destino
void bfsFindShortestPath(Graph* graph, int startVertex, int finalVertex) {
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
        AdjacencyNode* temp = graph->adjList[currentVertex];

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

/*
4ª Etapa: Encontrar o Menor e o Maior Caminho entre Usuários

Descrição:
- Implementação de funções que utilizam DFS (Busca em Profundidade) para encontrar o maior caminho possível  entre dois usuários sorteados aleatoriamente.
- As funções fornecem a capacidade de explorar o grafo para encontrar caminhos e avaliar a conectividade entre usuários.

Funções:

1. `dfsFindLongestPath`
   - Objetivo: Encontrar o caminho mais longo possível no grafo a partir de um vértice inicial.
   - Parâmetros:
     - `graph`: Ponteiro para o grafo.
     - `currentVertex`: Vértice atual sendo explorado.
     - `visited`: Array para marcar os vértices já visitados.
     - `currentPath`: Array que armazena o caminho atual.
     - `pathIndex`: Índice atual do caminho.
     - `maxPathLength`: Comprimento máximo do caminho encontrado até agora.
     - `endVertex`: Vértice final do caminho mais longo encontrado.
     - `bestPath`: Array que armazena o melhor caminho (o mais longo) encontrado.
   - Descrição: Explora todos os vértices adjacentes, atualiza o caminho mais longo encontrado e permite a reconstrução do caminho mais longo possível.

2. `findLongestPath`
   - Objetivo: Encontrar e imprimir o caminho mais longo
   - Descrição: Itera sobre todos os vértices do grafo, utilizando a função `dfsFindLongestPath` para encontrar o caminho mais longo a partir de cada vértice. Imprime o caminho mais longo encontrado, se existir.

3. `findPathsBetweenUsers`
   - Objetivo: Sorteia dois usuários e calcula o menor e o maior caminho entre eles.
   - Descrição: Sorteia dois usuários diferentes, calcula o menor caminho usando BFS e o maior caminho possível no grafo usando DFS. Imprime os resultados.

*/

// Função auxiliar para DFS que encontra o caminho mais longo
void dfsFindLongestPath(struct Graph* graph, int currentVertex, int* visited, int* currentPath, int pathIndex, int* maxPathLength, int* endVertex, int* bestPath) {
    visited[currentVertex] = 1;  // Marca o vértice atual como visitado
    currentPath[pathIndex] = currentVertex;  // Adiciona o vértice ao caminho atual
    pathIndex++;

    AdjacencyNode* temp = graph->adjList[currentVertex];
    int hasNeighbor = 0;

    // Explora todos os vértices adjacentes
    while (temp != NULL) {
        int adjVertex = temp->user->id;
        if (!visited[adjVertex]) {
            hasNeighbor = 1;
            dfsFindLongestPath(graph, adjVertex, visited, currentPath, pathIndex, maxPathLength, endVertex, bestPath);
        }
        temp = temp->next;
    }

    // Se não houver mais vizinhos para explorar
    if (!hasNeighbor) {
        // Se o caminho atual for o mais longo encontrado, atualiza o melhor caminho
        if (pathIndex > *maxPathLength) {
            *maxPathLength = pathIndex;
            *endVertex = currentVertex;
            for (int i = 0; i < pathIndex; i++) {
                bestPath[i] = currentPath[i];
            }
        }
    }

    pathIndex--;
    visited[currentVertex] = 0;  // Desmarca o vértice atual para futuras explorações
}

// Função para encontrar e imprimir o caminho mais longo usando DFS
void findLongestPath(struct Graph* graph) {
    int* visited = malloc(graph->numUsers * sizeof(int));      // Array de visitados
    int* currentPath = malloc(graph->numUsers * sizeof(int));  // Array para o caminho atual
    int* bestPath = malloc(graph->numUsers * sizeof(int));     // Array para armazenar o melhor caminho
    int maxPathLength = 0;                                     // Comprimento máximo do caminho encontrado
    int endVertex = -1;                                        // Vértice final do caminho mais longo

    // Inicializa o array de visitados
    for (int i = 0; i < graph->numUsers; i++) {
        visited[i] = 0;
    }

    // Chama DFS para encontrar o caminho mais longo a partir de cada vértice
    for (int i = 0; i < graph->numUsers; i++) {
        if (!visited[i]) {
            dfsFindLongestPath(graph, i, visited, currentPath, 0, &maxPathLength, &endVertex, bestPath);
        }
    }

    // Imprime o caminho mais longo encontrado, se existir
    if (endVertex != -1) {
        printf("\nCaminho mais longo :\n");
        for (int i = 0; i < maxPathLength; i++) {
            printf("%s", graph->users[bestPath[i]]->nome);
            if (i < maxPathLength - 1) {
                printf(" -> ");
            }
        }
        printf("\nDistancia: %d\n", maxPathLength - 1);
    } else {
        printf("\nNao ha caminhos no grafo.\n");
    }

    // Libera a memória alocada
    free(visited);
    free(currentPath);
    free(bestPath);
}

// Função que sorteia dois usuários e calcula o menor e o maior caminho entre eles
void findPathsBetweenUsers(Graph* graph) {
    srand(time(NULL));  // Inicializa a semente para números aleatórios

    int startVertex = rand() % graph->numUsers;  // Sorteia o vértice inicial
    int finalVertex;
    do {
        finalVertex = rand() % graph->numUsers;  // Sorteia o vértice final diferente do inicial
    } while (finalVertex == startVertex);

    printf("\nUsuario inicial: %s\n", graph->users[startVertex]->nome);
    printf("Usuario final: %s\n", graph->users[finalVertex]->nome);

    // Calcula e imprime o menor caminho entre os usuários sorteados
    bfsFindShortestPath(graph, startVertex, finalVertex);

    // Calcula e imprime o maior caminho encontrado no grafo
    findLongestPath(graph);
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

    // Gerar conexões aleatórias
    generateRandomConnections(graph, 20);

    // Imprime o grafo
    printGraph(graph);

    // Contar e verificar o número de conexões
    int connectionCount = countConnections(graph);
    printf("\nNumero total de conexoes no grafo: %d\n", connectionCount);

    // Chama a busca
    findPathsBetweenUsers(graph);

    // Libera a memória alocada para o grafo
    freeGraph(graph);
    return 0;
}