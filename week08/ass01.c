#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_SIZE 10

typedef struct {
    int buffer[MAX_SIZE];
    int front, rear;
    pthread_mutex_t mutex;
    pthread_cond_t not_full, not_empty;
} CircularQueue;

void initQueue(CircularQueue *queue) {
    queue->front = queue->rear = 0;
    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->not_full, NULL);
    pthread_cond_init(&queue->not_empty, NULL);
}

void enQueue(CircularQueue *queue, int item) {
    pthread_mutex_lock(&queue->mutex);
    while ((queue->rear + 1) % MAX_SIZE == queue->front) {
        pthread_cond_wait(&queue->not_full, &queue->mutex);
    }
    queue->buffer[queue->rear] = item;
    queue->rear = (queue->rear + 1) % MAX_SIZE;
    pthread_cond_signal(&queue->not_empty);
    pthread_mutex_unlock(&queue->mutex);
}

int deQueue(CircularQueue *queue) {
    pthread_mutex_lock(&queue->mutex);
    while (queue->front == queue->rear) {
        pthread_cond_wait(&queue->not_empty, &queue->mutex);
    }
    int item = queue->buffer[queue->front];
    queue->front = (queue->front + 1) % MAX_SIZE;
    pthread_cond_signal(&queue->not_full);
    pthread_mutex_unlock(&queue->mutex);
    return item;
}

int isEmptyQ(CircularQueue *queue) {
    return queue->front == queue->rear;
}

int isFullQ(CircularQueue *queue) {
    return (queue->rear + 1) % MAX_SIZE == queue->front;
}

void *producer(void *data) {
    CircularQueue *queue = (CircularQueue *)data;
    while (1) {
        int item;
        printf("Enter an integer to produce: ");
        scanf("%d", &item);
        enQueue(queue, item);
        printf("Produced: %d\n", item);
    }
    return NULL;
}

void *consumer(void *data) {
    CircularQueue *queue = (CircularQueue *)data;
    while (1) {
        int item = deQueue(queue);
        printf("Consumed: %d\n", item);
    }
    return NULL;
}

int main() {
    int m, n;
    printf("Enter the number of producer threads: ");
    scanf("%d", &m);
    printf("Enter the number of consumer threads: ");
    scanf("%d", &n);

    CircularQueue queue;
    initQueue(&queue);

    pthread_t producer_threads[m], consumer_threads[n];

    for (int i = 0; i < m; i++) {
        pthread_create(&producer_threads[i], NULL, producer, (void *)&queue);
    }

    for (int i = 0; i < n; i++) {
        pthread_create(&consumer_threads[i], NULL, consumer, (void *)&queue);
    }

    for (int i = 0; i < m; i++) {
        pthread_join(producer_threads[i], NULL);
    }

    for (int i = 0; i < n; i++) {
        pthread_join(consumer_threads[i], NULL);
    }

    return 0;
}

