// Daniel Lee z5218637
// This file is for the implementation of the Counter ADT
// The program can create counter, free and make array

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Counter.h"

struct counter {
    struct node *head;
    int numItems;
};

struct node {
    struct item item;
    struct node *left;
    struct node *right;
};

// Helper Func
static struct node *createToken(Counter c, struct node *newNode, char *token);
static void nodeFree(struct node *head);
static int nodeFreq(struct node *Node, char *token);
static void makeArray(struct node *head, struct item *itemArray, int *counter);

Counter CounterNew(void) {
    struct counter *newCounter = malloc(sizeof(struct counter));
    newCounter->head = NULL;
    newCounter->numItems = 0;
    return newCounter;
}

void CounterFree(Counter c) {
    if (c->head == NULL) {
        free(c);
        return;
    }
    nodeFree(c->head);
    free(c);
}

static void nodeFree(struct node *head) {
    if (head == NULL) {
        return;
    }
    nodeFree(head->left);
    nodeFree(head->right);
    free(head->item.token);
    free(head);
}

void CounterAdd(Counter c, char *token) {
    // If there is no item in beginning
    if (c->head == NULL) {
        c->head = createToken(c, c->head, token);
        return;
    }
    createToken(c, c->head, token);
}
// CounterAdd:HelperFunc - traversing bst to increase freq or make new node
static struct node *createToken(Counter c, struct node *newNode, char *token) {
    if (newNode == NULL) {
        struct node *newNode = malloc(sizeof(struct node));
        newNode->item.token = strdup(token);
        newNode->item.freq = 1;
        newNode->left = NULL;
        newNode->right = NULL;
        c->numItems++;
        return newNode;
    }
    // Increasing frequency if same token
    int stringCompare = strcmp(newNode->item.token, token);
    if (stringCompare == 0) {
        newNode->item.freq++;
    } else if (stringCompare < 0) {
        newNode->left = createToken(c, newNode->left, token);
    } else if (stringCompare > 0) {
        newNode->right = createToken(c, newNode->right, token);
    }
    return newNode;
}
// Returns number of nodes
int CounterNumItems(Counter c) {
    return c->numItems;
}

int CounterGet(Counter c, char *token) {
    return nodeFreq(c->head, token);
}
// CounterGet:HelperFunc - traversing bst to find the freq or 0 if new node
static int nodeFreq(struct node *Node, char *token) {
    if (Node == NULL) {
        return 0;
    }
    int stringCompare = strcmp(Node->item.token, token);
    if (stringCompare < 0) {
        return nodeFreq(Node->left, token);
    } else if (stringCompare > 0) {
        return nodeFreq(Node->right, token);
    }
    return Node->item.freq;
}

struct item *CounterItems(Counter c, int *numItems) {
    // Creating enough memory for item
    struct item *itemArray = malloc(sizeof(struct item) * CounterNumItems(c));
    int counter = 0;
    makeArray(c->head, itemArray, &counter);
    //outparameter
    *numItems = CounterNumItems(c);
    return itemArray;
}
// CounterItems:HelperFunc - traversing to implement token and freq into array
static void makeArray(struct node *head, struct item *itemArray, int *counter) {
    if (head == NULL) {
        return;
    }
    itemArray[*counter].token = strdup(head->item.token);
    // If token is not a string
    if (itemArray[*counter].token == NULL) {
        return;
    }
    itemArray[*counter].freq = head->item.freq;
    (*counter)++;
    makeArray(head->left, itemArray, counter);
    makeArray(head->right, itemArray, counter);
}
