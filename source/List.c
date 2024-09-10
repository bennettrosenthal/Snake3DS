#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "List.h"


ListPtr createList() {
    ListPtr l = (ListPtr)malloc(sizeof(ListObj));
    l->length = 0;
    l->head = NULL;
    return l;
}

void destroyList(ListPtr *pL) {
    NodePtr next;
    if (*pL != NULL) {
        NodePtr n = (*pL)->head;
        while (n != NULL) {
            next = n->next;
//printf("destroy: about to free node containing: %d\n", *(int *) n->data );
            free(n);
            n = next;
        }
        free(*pL);
    *pL = NULL;
    }
}

int lengthList(ListPtr L) {
    if (L != NULL) {
        return L->length;
    }
    return -1;
}

Coordinate getList(ListPtr L, int i) {
    Coordinate zero_coord = {-1, 0};
    if (L == NULL) return zero_coord;
    NodePtr n = L->head;
    if (i < 0 || n == NULL || i >= L->length) {
        return zero_coord;
    }
    for (;i > 0;i--)
        n = n->next;
    Coordinate d = n->data;
    return d;
}

Coordinate deleteList(ListPtr L, int i) {
    Coordinate zero_coord = {-1, 0};
    if (L == NULL) return zero_coord;
    NodePtr n = L->head;
    NodePtr prev = NULL;
    if (i < 0 || n == NULL || i >= L->length) {
        return zero_coord;
    }
    for (;i > 0;i--) {
    prev = n;
        n = n->next;
    }
    if (prev == NULL) {
        L->head =n->next;
    } else {
        prev->next = n->next;
    }
    Coordinate d = n->data;
    L->length--;
    free(n);
    return d;
}

bool list_insert(ListPtr L, int pos, Coordinate data) {
    if ( L == NULL || L->length < pos || pos < 0) {
        return false;
    }
    NodePtr n = L->head;
    NodePtr newNode = (NodePtr)malloc(sizeof(NodeObj));
    newNode->next = NULL;
    newNode->data = data;
    if (pos == 0) {
        newNode->next = L->head;
        L->head = newNode;
    } else {
        for (;pos > 1; pos--) {
            n = n->next;
        }
            newNode->next = n->next;
        n->next = newNode;
    }
    L->length++;
    return true;
}

bool appendList(ListPtr L, Coordinate data) {
    if (L == NULL) return false;
    return list_insert(L, L->length, data);
}

bool insertAtFrontOfList(ListPtr L, Coordinate data) {
    if (L == NULL) return false;
    return list_insert(L, 0, data);
}