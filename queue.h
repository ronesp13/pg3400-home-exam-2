typedef struct Node {
    char *key;
    struct Node *next;
} Node;

typedef struct Queue {
    Node *first;
    Node *last;
} Queue;

void enqueue(Queue *queue, char *key);
char *dequeue(Queue *queue);