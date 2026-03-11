#include <stdio.h>
#include <stdlib.h>

typedef struct block {
    int id;
    int start;
    int size;
    struct block *next;
} Block;

Block *head = NULL;

/* create node */
Block* createNode(int id, int start, int size) {
    Block *n = (Block*)malloc(sizeof(Block));
    n->id = id;
    n->start = start;
    n->size = size;
    n->next = NULL;
    return n;
}

/* insert block in sorted order of start address */
void insertSorted(int id, int start, int size) {
    Block *n = createNode(id, start, size);

    if (head == NULL || start < head->start) {
        n->next = head;
        head = n;
        return;
    }

    Block *t = head;
    while (t->next != NULL && t->next->start < start)
        t = t->next;

    n->next = t->next;
    t->next = n;
}

/* add free memory block */
void addBlock() {
    int id, start, size;
    printf("Enter BlockID StartAddress Size:\n");
    scanf("%d %d %d", &id, &start, &size);
    insertSorted(id, start, size);
}

/* allocate memory */
void allocateMemory() {
    int req;
    printf("Enter requested size:\n");
    scanf("%d", &req);

    Block *t = head, *prev = NULL;

    while (t != NULL) {
        if (t->size >= req) {
            t->start += req;
            t->size -= req;

            if (t->size == 0) {
                if (prev == NULL)
                    head = t->next;
                else
                    prev->next = t->next;
                free(t);
            }

            printf("Memory Allocated\n");
            return;
        }
        prev = t;
        t = t->next;
    }

    printf("Out of Memory\n");
}

/* merge adjacent free blocks */
void mergeBlocks() {
    Block *t = head;
    while (t != NULL && t->next != NULL) {
        if (t->start + t->size == t->next->start) {
            Block *d = t->next;
            t->size += d->size;
            t->next = d->next;
            free(d);
        } else {
            t = t->next;
        }
    }
}

/* free memory */
void freeMemory() {
    int id, start, size;
    printf("Enter BlockID StartAddress Size:\n");
    scanf("%d %d %d", &id, &start, &size);

    insertSorted(id, start, size);
    mergeBlocks();
}

/* display free list */
void display() {
    Block *t = head;
    if (t == NULL) {
        printf("Free list empty\n");
        return;
    }

    printf("Free List:\n");
    while (t != NULL) {
        printf("ID:%d Start:%d Size:%d\n", t->id, t->start, t->size);
        t = t->next;
    }
}

/* main */
int main() {
    int ch;
    while (1) {
        printf("\n1.Add Free Block\n2.Allocate Memory\n3.Free Memory\n4.Display Free List\n5.Exit\n");
        scanf("%d", &ch);

        switch (ch) {
            case 1: addBlock(); break;
            case 2: allocateMemory(); break;
            case 3: freeMemory(); break;
            case 4: display(); break;
            case 5: exit(0);
            default: printf("Invalid choice\n");
        }
    }
}

