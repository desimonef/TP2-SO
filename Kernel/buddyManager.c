// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//#ifdef BUDDY

#include "include/memManager.h"
#include <stdint.h>
#include <stdio.h>
#include "include/naiveConsole.h"
#include "include/lib.h"

#define IS_POWER_OF_2(x) (!((x)&((x)-1)))

static unsigned align(unsigned size);

typedef struct node {
    struct node *left;
    struct node *right;
    unsigned int index;
    void *address;
    unsigned int size;
    int status;
} node;

typedef struct node *node_t;
node_t root;
unsigned int memAllocated = 0;

void initMM(void) {
    root = (node *) BEGIN_MEM;
    root->address = (void *) MEM_START;
    root->index = 0;
    root->size = TOTALMEM;
    root->status = EMPTY;
    root->left = NULL;
}

void setDescendants(node_t node) {
    unsigned int parentIdx = node->index;
    unsigned int leftIdx = parentIdx * 2 + 1;
    uint64_t descendantSize = ((uint64_t)(node->size) / 2);
    node->left = node + leftIdx;

    if ((uint64_t) node->left >= LIMIT) {
        ncPrint("Not enough space to allocate descendants (testing print)\n");
        return;
    }
    node->left->index = leftIdx;
    node->left->size = descendantSize;
    node->left->address = node->address;
    node->left->status = EMPTY;

    unsigned int rightIdx = leftIdx + 1;
    node->right = node + rightIdx;
    if ((uint64_t) node->right >= LIMIT) {
        ncPrint("Not enough space to allocate descendants (testing print)\n");
        return;
    }
    node->right->index = rightIdx;
    node->right->size = descendantSize;
    node->right->address = (void *) ((uint64_t)(node->address) + descendantSize);
    node->right->status = EMPTY;
}

int hasDescendants(node_t node) {
    return node->left == NULL || node->right == NULL;
}

void finalizeRecursiveCall(node_t node) {
    if (hasDescendants(node)) {
        node->status = EMPTY;
        return;
    }
    if (node->left->status == FULL && node->right->status == FULL) {
        node->status = FULL;
    } else if (node->left->status == FULL || node->right->status == FULL
               || node->left->status == PARTIAL || node->right->status == PARTIAL) {
        node->status = PARTIAL;
    } else {
        node->status = EMPTY;
    }

}

void *mallocRec(node_t node, unsigned int size) {
    if (node->status == FULL) {
        return NULL;
    }

    if (node->left != NULL || node->right != NULL) {
        void *auxNode = mallocRec(node->left, size);
        if (auxNode == NULL) {
            auxNode = mallocRec(node->right, size);
        }
        finalizeRecursiveCall(node);
        return auxNode;
    } else {
        if (size > node->size) {
            return NULL;
        }
        if ((node->size / 2) >= size) {
            setDescendants(node);
            void *auxNode = mallocRec(node->left, size);
            finalizeRecursiveCall(node);
            return auxNode;
        }
        node->status = FULL;
        memAllocated += size;
        return node->address;
    }
}

void *malloc(int size) {
    if (size < MIN_SIZE) {
        size = MIN_SIZE;
    } else if (size > root->size) {
        ncPrint("No hay memoria existente para alocar tanto.\n");
        return NULL;
    }
    if (!IS_POWER_OF_2(size)) {
        size = align(size);
    }
    void *allocAttempt = mallocRec(root, size);
    return allocAttempt;
}

void freeRec(node_t node, void *addr) {
    if (node->left != NULL || node->right != NULL) {
        if (node->right != 0 && (uint64_t) node->right->address > (uint64_t) addr) {
            freeRec(node->left, addr);

        } else {
            freeRec(node->right, addr);
        }

        finalizeRecursiveCall(node);

        if (node->status == EMPTY) {
            node->right = NULL;
            node->left = NULL;

        }
        return;


    } else if (node->status == FULL) {
        if (node->address == addr) {
            node->status = EMPTY;
            memAllocated -= node->size;
            return;
        }

        return;

    }
}

void free(void *addr) {
    freeRec(root, addr);
}

void memDump(void) {
    ncNewline();
    ncNewline();
    ncPrint("-----Estado de memoria-----");
    ncNewline();
    ncPrint("Memoria total: ");
    ncPrintDec((uint64_t) TOTALMEM);
    ncNewline();
    ncPrint("---------------------------");
    ncNewline();
    ncPrint("Memoria ocupada: ");
    ncPrintDec((uint64_t) memAllocated);
    ncNewline();
    ncPrint("---------------------------");
    ncNewline();
    ncPrint("Memoria libre: ");
    ncPrintDec((uint64_t) TOTALMEM - (uint64_t) memAllocated);
    ncNewline();
    ncPrint("---------------------------");
}


static unsigned align(unsigned size) {
    size |= size >> 1;
    size |= size >> 2;
    size |= size >> 4;
    size |= size >> 8;
    size |= size >> 16;
    return size + 1;
}

//#endif