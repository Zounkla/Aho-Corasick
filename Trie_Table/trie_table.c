#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "trie_table.h"

#define DEF_VALUE -1

Trie createTrie(int maxNode){
    Trie t = malloc(sizeof(struct _trie));
    if(t == NULL){
        fprintf(stderr, "Erreur allocation\n");
        return NULL;
    }
    t->insertedNode = 0;
    t->maxNode = maxNode;
    t->nextNode = 1;
    t->transition = malloc((size_t)(maxNode) * sizeof(int*));
    if(t->transition == NULL){
        fprintf(stderr, "Erreur allocation tableau\n");
        return NULL;
    }
    t->finite = malloc((size_t)maxNode * sizeof(int));
    if(t->finite == NULL){
        free(t->transition);
        fprintf(stderr, "Erreur allocation finite\n");
    }
    for (int i = 0; i < maxNode; i += 1) {
        t->finite[i] = 0;
    }
    for(int i = 0; i < maxNode; i += 1) {
        t->transition[i] = malloc(UCHAR_MAX * sizeof(int));
        if(t->transition[i] == NULL){
            fprintf(stderr, "Erreur allocation cellule tableau\n");
            for(int j = 0; j < i; j += 1){
                free(t->transition[j]);
            }
            return NULL;
        }
        for(int j = 0; j < UCHAR_MAX; j += 1){
            t->transition[i][j] = DEF_VALUE;
        }
    }
    return t;
}

int isInTrie(Trie trie, unsigned char *w){
    if(w == NULL){
        fprintf(stderr, "Erreur w == NULL\n");
        return -1;
    }
    int currentNode = 0;
    for(int i = 0; w[i] != '\0'; i += 1){
        if(trie->transition[currentNode][w[i]] == -1){
            return 0;
        }
        currentNode = trie->transition[currentNode][w[i]];
    }
    if(trie->finite[currentNode]){
        return 1;
    }
    return 0;
}

int is_transition(Trie trie, int start_node, unsigned char letter){
    if(trie->transition[start_node][letter] != DEF_VALUE){
        return 1;
    }
    return 0;
}

int get_target(Trie trie, int start_node, unsigned char letter){
    return trie->transition[start_node][letter];
}

void print_trie(Trie t){
    for(int i = 0; i < t->maxNode; i += 1){
        printf("|");
        for(int j = 0; j < UCHAR_MAX; j += 1){
            printf("%d|", t->transition[i][j]);
        }
        printf("\n");
    }
}

void insertInTrie(Trie trie, unsigned char *w){
    if(w == NULL){
        fprintf(stderr, "Erreur w == NULL\n");
        return;
    }
    int currentNode = 0;
    for(int i = 0; w[i] != '\0'; i += 1){
        if(trie->transition[currentNode][w[i]] == -1){
            trie->transition[currentNode][w[i]] = trie->nextNode;
            trie->nextNode += 1;
        }
        currentNode = trie->transition[currentNode][w[i]];
    }
    trie->finite[currentNode] = 1;
    trie->insertedNode = currentNode;
}

void create_transition(Trie trie, int start_node, char letter, int target_node){
    trie->transition[start_node][(unsigned char)letter] = target_node;
}

void dispose_trie(Trie t){
    for(int i = 0; i < t->maxNode; i += 1){
        if(t->transition[i] != NULL){
            free(t->transition[i]);
        }
    }
    free(t->transition);
    free(t->finite);
    free(t);
}

