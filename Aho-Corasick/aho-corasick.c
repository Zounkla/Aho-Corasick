#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "aho-corasick.h"
#include "../Queue/queue.h"
#include "../List/list.h"

#define MAX_NODE 100

typedef struct transition
{
    int start_node;
    unsigned char letter;
    int target_node;
} Transition;

int sup[MAX_NODE];

Trie pre_ac(char **word_list, size_t word_count);
void complete(Trie *t);

size_t aho_corasick(char **word_list, size_t word_count, unsigned char *text, size_t text_length) {
    Trie e = pre_ac(word_list, word_count);
    size_t count = 0;
    int current_node = 0;
    for(size_t j = 0; j < text_length; j += 1){
        while (!is_transition(e, current_node, text[j])){
            current_node = sup[current_node];
        }
        current_node = get_target(e, current_node, text[j]);
        if(is_finite_node(e, current_node)){
            printf("Un mot de l'entrée à été trouvé\n");
            count += 1;
        }
    }
    return count;
}

Trie pre_ac(char **word_list, size_t word_count){
    Trie trie = createTrie(MAX_NODE);
    for(size_t i = 0; i < word_count; i += 1){
        insertInTrie(trie, (unsigned char*)word_list[i]);
    }
    for(unsigned char a = 0; a < UCHAR_MAX; a += 1){
        if(!is_transition(trie, 0, a)){
            create_transition(trie, 0, (char)a, 0);
        }
    }
    // complete(0);
    return trie;
}

void complete(Trie *t){
    Queue *q = create_queue();
    List *l = create_list();
    for(unsigned char c = 0; c < UCHAR_MAX; c += 1){
        Transition *transition = malloc(sizeof transition);
        if(transition == NULL){
            dispose_queue(&q);
            dispose_list(&l);
            exit(EXIT_FAILURE);
        }
        transition->start_node = 0;
        transition->letter = c;
        transition->target_node = get_target(*t, 0, c);
        append(l, transition);
    }

    while (!is_empty(l)){
        Transition *transition = (Transition *)retrieve(l);
        enque(q, &transition->target_node);
        sup[transition->target_node] = 0;
    }

    while (!queue_is_empty(q)){
        int start_node = *(int *)dequeue(q);
        for(unsigned char c = 0; c < UCHAR_MAX; c += 1){
            Transition *transition = malloc(sizeof transition);
            if(transition == NULL){
                dispose_queue(&q);
                dispose_list(&l);
                exit(EXIT_FAILURE);
            }
            transition->start_node = start_node;
            transition->letter = c;
            transition->target_node = get_target(*t, start_node, c);
            append(l, transition);
        }
        while (!is_empty(l)){
            Transition *transition = (Transition *)retrieve(l);
            enque(q, &transition->target_node);
            int s = sup[transition->start_node];
            while (!is_transition(*t, s, transition->letter)){
                s = sup[s];
            }
            sup[transition->target_node] = get_target(*t, s, transition->letter);
            if(is_finite_node(*t, sup[transition->target_node])){
                declare_finite_state(*t, transition->target_node);
            }
        }
    }
    dispose_list(&l);
    dispose_queue(&q);
}