#include <stdlib.h>

#ifndef QUEUE
#define QUEUE "queue.h"
#include QUEUE
#endif

void enqueue(Queue *queue, char *key) {
    Node *temp = (Node*) malloc(sizeof(Node));
    temp->key = key;
    temp->next = NULL;
    if (queue->first == NULL && queue->last == NULL) {
        queue->first = temp;
        queue->last = temp;
        return;
    }
    queue->last->next = temp;
    queue->last = temp;
}

char *dequeue(Queue *queue) {
    Node *temp = queue->first;
    if (queue->first == NULL) {
        return NULL;
    }
    if (queue->first == queue->last) {
        queue->first = NULL;
        queue->last = NULL;
    } else {
        queue->first = queue->first->next;
    }
    char *key = temp->key;
    free(temp);
    return key;
}