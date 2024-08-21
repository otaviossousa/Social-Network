#include <stdio.h>
#include <stdlib.h>
#define SIZE 20

struct vertice {
    int vertice;
    struct vertice* next;
};

struct grafo {
    int qtdVertices;
    struct vertice** adjList;
    int* visita;
};

// ----- coisas da fila de busca fonte:programiz.com ---------- //

struct queue {
  int items[SIZE];
  int front;
  int rear;
};

struct queue* createQueue() {
  struct queue* q = malloc(sizeof(struct queue));
  q->front = -1;
  q->rear = -1;
  return q;
}

// Check if the queue is empty
int isEmpty(struct queue* q) {
  if (q->rear == -1)
    return 1;
  else
    return 0;
}

// Adding elements into queue
void enqueue(struct queue* q, int value) {
  if (q->rear == SIZE - 1)
    printf("\nQueue is Full!!");
  else {
    if (q->front == -1)
      q->front = 0;
    q->rear++;
    q->items[q->rear] = value;
  }
}

// Removing elements from queue
int dequeue(struct queue* q) {
  int item;
  if (isEmpty(q)) {
    printf("Queue is empty");
    item = -1;
  } else {
    item = q->items[q->front];
    q->front++;
    if (q->front > q->rear) {
      printf("Resetting queue ");
      q->front = q->rear = -1;
    }
  }
  return item;
}

// Print the queue
void printQueue(struct queue* q) {
  int i = q->front;

  if (isEmpty(q)) {
    printf("Queue is empty");
  } else {
    printf("\nQueue contains ");
    for (i = q->front; i < q->rear + 1; i++) {
      printf("%d, ", q->items[i]);
    }
  }
  return;
}
// ---------------------------------------- //

// Cria um novo vertice
struct vertice* createVertice(int vert){
  struct vertice* novo = malloc(sizeof(struct vertice));
  novo->vertice = vert;
  novo->next = NULL;
  return novo;
}

// Inicializa o grafo com 20 espaços
struct grafo* createGraph(){
  struct grafo* grafo = malloc(sizeof(struct grafo));
  grafo->qtdVertices = SIZE;
  grafo->adjList = malloc(SIZE * sizeof(struct vertice*));
  grafo->visita = malloc(SIZE * sizeof(int));
  for(int i = 0; i < SIZE; i++){
    grafo->adjList[i] = NULL;
    grafo->visita[i] = 0;
  }
  return grafo;
}
// função que cria ligações entre os vertices
void ligar(struct grafo* grafo, int inicial, int final){ 
  // indo
  struct vertice* novoVertice = createVertice(final);
  novoVertice->next = grafo->adjList[inicial];
  grafo->adjList[inicial] = novoVertice;

  // voltando
  novoVertice = createVertice(inicial);
  novoVertice->next = grafo->adjList[final];
  grafo->adjList[final] = novoVertice;
}

//algoritmo de busca em largura 
void bfs(struct grafo* graph, int startVertex) {
  struct queue* q = createQueue();

  graph->visita[startVertex] = 1;
  enqueue(q, startVertex);

  while (!isEmpty(q)) {
    printQueue(q);
    int currentVertex = dequeue(q);
    printf("Visited %d\n", currentVertex);

    struct vertice* temp = graph->adjList[currentVertex];

    while (temp) {
      int adjVertex = temp->vertice;

      if (graph->visita[adjVertex] == 0) {
        graph->visita[adjVertex] = 1;
        enqueue(q, adjVertex);
      }
      temp = temp->next;
    }
  }
}

int main() {
    struct grafo* g = createGraph();
    ligar(g, 0, 1);
    ligar(g, 0, 2);
    ligar(g, 1, 2);
    ligar(g, 2, 3);
    ligar(g, 0, 1);
    ligar(g, 0, 3);
    ligar(g, 1, 4);
    ligar(g, 2, 5);
    ligar(g, 2, 6);
    ligar(g, 3, 7);
    ligar(g, 4, 8);
    ligar(g, 5, 9);
    ligar(g, 6, 10);
    ligar(g, 7, 11);
    ligar(g, 8, 12);
    ligar(g, 9, 13);
    ligar(g, 10, 14);
    ligar(g, 11, 15);
    ligar(g, 12, 16);
    ligar(g, 13, 17);
    ligar(g, 14, 18);
    ligar(g, 15, 19);

    bfs(g, 0);

    return 0;
}