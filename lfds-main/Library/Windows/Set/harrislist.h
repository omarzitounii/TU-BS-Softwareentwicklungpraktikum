//
// Created by keanukreed on 01.05.23.
//
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef HARRISLIST_HARRISLIST_H
#define HARRISLIST_HARRISLIST_H

typedef struct node{
    _Atomic int key;
    struct node* _Atomic next;
    _Atomic int mark;
    _Atomic int count; //reference counter
}node_t;

typedef struct list{
    node_t* head;
    node_t* tail;
}list_t;

//implementation of reference counter
static inline void
ref_inc(const struct node *ref)
{
    atomic_fetch_add((_Atomic int *)&ref->count, 1); //increases reference counter
}

static inline void
ref_dec(const struct node *ref)
{
    if (atomic_fetch_sub((_Atomic int *)&ref->count, 1) == 1){ //decreases ref counter and frees if 1
        free((void*)ref);
    }

}

//useable List functions
int insert(int key);
int delete(int key);
int find(int key);
void print_list();

//non useable list function
node_t* search(node_t **left, int key);

//Helper functions
int is_marked_node(node_t* node);
void set_unmarked_node(node_t* node);
void set_marked_node(node_t* expected);
int initialise_list(node_t* head, node_t* tail);
node_t* initialise_node(int key);
int mark_neighbour_node(node_t* given, node_t* expected);



#endif //HARRISLIST_HARRISLIST_H
