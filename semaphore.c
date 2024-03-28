#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

typedef struct Node {
    int data;
    struct Node* prev;
    struct Node* next;
} Node;

int insertCnt = 0;
int deleteCnt = 0;
int nodeCnt = 0;
int seed = 1;
Node* head = NULL;
Node* tail = NULL;
sem_t SEM;

void* insert(void* arg) {
    int limit = *((int*) arg);

    for (int i = limit - 10; i < limit; i++) {
        sem_wait(&SEM);
        Node* node = (Node*) malloc(sizeof(Node));

        node->data = i;
        node->prev = NULL;
        node->next = NULL;

        if (head == NULL && tail == NULL) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            node->prev = tail;
            tail = node;
        }

        nodeCnt++;
        insertCnt++;
        sem_post(&SEM);
    }

    return NULL;
}

void* delete(void* arg) {
    srand(seed);

    while (!nodeCnt) {
        usleep(100000);
    }

    while (nodeCnt > 0 || insertCnt < 30) {
        int data = rand() % 30;
        Node* del = head;
        int findSignal = 0;

        sem_wait(&SEM);

        while (del != NULL) {
            if (del->data == data) {
                findSignal = 1;

                if (del->prev != NULL && del->next != NULL) {
                    del->prev->next = del->next;
                    del->next->prev = del->prev;
                    deleteCnt++;
                    free(del);
                } else {
                    if (del->prev == NULL && del->next == NULL) {
                        deleteCnt++;
                        free(del);
                        head = NULL;
                        tail = NULL;
                    } else if (del->prev == NULL) {
                        del->next->prev = del->prev;
                        deleteCnt++;
                        free(del);
                        head = tail;
                    } else {
                        del->prev->next = del->next;
                        deleteCnt++;
                        free(del);
                        tail = head;
                    }
                }

                nodeCnt--;
                break;
            }

            del = del->next;
        }

        sem_post(&SEM);

        if (findSignal == 0) {
            usleep(100000);
        }
    }

    return NULL;
}

void* search(void* arg) {
    srand(seed);

    while (!nodeCnt) {
        usleep(100000);
    }

    while (nodeCnt > 0 || insertCnt < 30) {
        int data = rand() % 30;
        Node* cur = head;
        int findSignal = 0;

        while (cur != NULL) {
            if (cur->data == data) {
                findSignal = 1;
                break;
            }

            cur = cur->next;
        }

        if (findSignal == 0) {
            usleep(100000);
        }
    }

    return NULL;
}

int main(void) {
    pthread_t threadForInsert[3];
    pthread_t threadForDelete;
    pthread_t threadForRead[5];
    clock_t start;
    clock_t end;
    
    start = clock();
    sem_init(&SEM, 0, 1);

    for (int i = 0; i < 3; i++) {
        int limit = 10 * (i + 1);
        pthread_create(&threadForInsert[i], NULL, insert, &limit);
    }

    pthread_create(&threadForDelete, NULL, delete, NULL);

    for (int i = 0; i < 5; i++) {
        pthread_create(&threadForRead[i], NULL, search, NULL);
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(threadForInsert[i], NULL);
    }

    pthread_join(threadForDelete, NULL);

    for (int i = 0; i < 5; i++) {
        pthread_join(threadForRead[i], NULL);
    }

    sem_destroy(&SEM);
    end = clock();
    double time = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("------------------------------------------------\n\n");
    printf("[Semaphore]\n\n");
    printf("[Number of Insertions]: %d\n\n", insertCnt);
    printf("[Number of Deletions]: %d\n\n", deleteCnt);

    if (head == NULL && tail == NULL && insertCnt == 30 && deleteCnt == 30) {
        printf("All work has been completed.\n\n");
    } else {
        printf("Not all tasks were completed.\n\n");
    }

    printf("[Seed]: %d\n\n", seed);
    printf("[Performance Measurement]: %f second\n\n", time);
    printf("------------------------------------------------\n\n");
    return 0;
}