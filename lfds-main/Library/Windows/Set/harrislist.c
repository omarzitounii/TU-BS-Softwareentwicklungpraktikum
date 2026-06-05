//
// Created by keanukreed on 01.05.23.
//

#include "harrislist.h"

//testing with only one static list
list_t list;

//fills head and tail in list
int initialise_list(node_t* head, node_t* tail){
    head->next = tail;
    list.head = head;
    list.tail = tail;
    return 1;
}

//creates node and initialises it
node_t* initialise_node(int key){
    node_t* tmp = malloc(sizeof(node_t));
    tmp->key = key;
    tmp->mark = 0;
    tmp->next = NULL;
    tmp->count = 1;
    return tmp;
}

//inserts node based on key and location
int insert(int key){
    //create helper and new node
    node_t *new_node, *right_node, *left_node;
    new_node = initialise_node(key);
    //increase new node ref
    ref_inc(new_node);
    do{
        //pinpoint location for new node
        right_node = search(&left_node, key);
        //check if node with similar key is in list
        if((right_node != list.tail) && (right_node->key == key)){
            return 0;
        }
        //attach right part to new node
        new_node->next = right_node;
        //attach right part of left node to new node
        if(atomic_compare_exchange_strong(&(left_node->next), &right_node, new_node)){
            //list was not modified new node can be inserted
            return 1;
        }
        //list was modified while inserting need to relocate node position
    }while(1);
}

node_t* search(node_t **left, int key){
    //create helper nodes
    node_t *left_next, *right_node;

    search_again:
    do{
        //set temp nodes to iterate through list
        node_t* temp = list.head;
        node_t* temp_next = list.head->next;

        //find left and right node
        do{
            //if node not marked set left bounds
            if(!atomic_load(&(temp_next->mark))){
                (*left) = temp;
                left_next = temp_next;
            }
            //iterate and unmark iterated node
            temp = temp_next;
            atomic_store(&(temp->mark), 0);
            //if iterated through complete list end loop
            if(temp == list.tail){
                break;
            }
            //set temp_next
            temp_next = temp->next;
        }while(atomic_load(&temp_next->mark) || atomic_load(&temp->key) < key);
        //do this until node is either marked or key does not fit anymore, temp is our right bound
        right_node = temp;

        //check if nodes are next to each other
        if(left_next == right_node){
            //check if new node has been logically deleted
            if((right_node != list.tail) && is_marked_node(right_node->next)){
                //list has been modified there is a logically deleted node now
                goto search_again;
            } else {
                //list is in consistent state nodes have been set correctly
                return right_node;
            }
        }

        //error while setting left bound, repeat first step
        if(*left == NULL){
            continue;
        }

        //physically delete logically deleted nodes
        //save current node
        node_t* to_free = left_next;
        //check if marked node is still there
        if(atomic_compare_exchange_strong(&((*left)->next), &left_next, right_node)){
            //delete marked node
            ref_dec(to_free);
            //check if there is another logically deleted node or if list has been modified
            if((right_node != list.tail) && is_marked_node(right_node->next)){
                goto search_again;
            } else {
                return right_node;
            }
        }
    }while(1);
}

int delete(int key){
    //create helper nodes
    node_t *right_node, *right_node_next, *left_node, tmp;

    do{
        //pinpoint location of to be deleted
        right_node = search(&left_node, key);
        //key is not in list
        if((right_node == list.tail) || (right_node->key != key)){
            return 0;
        }
        //check if wanted already logically deleted
        right_node_next = right_node->next;
        if(!is_marked_node(right_node_next)){
            //create marked version of right next, tmp node
            tmp = *right_node_next;
            set_marked_node(&tmp);
            //if list wasn`t modified swap node to its marked version
            if(atomic_compare_exchange_strong(&(right_node->next),&right_node_next,&tmp)){
                break;
            }
        }
    } while(1);
    //if node isnt already phisically deleted delete it through search function
    if(!atomic_compare_exchange_strong(&(left_node->next), &right_node, right_node_next)){
        right_node = search(&left_node, right_node->key);
    }
    //node can be decreased and freed if not used
    ref_dec(right_node);
    return 1;
}

int find(int key){
    //create helper nodes
    node_t *right_node, *left_node;
    //pinpoint wanted node location
    right_node = search(&left_node, key);
    //check if wanted node was found
    if((right_node == list.tail) || (right_node->key != key)) {
        return 0;
    } else {
        return 1;
    }
}


//helper functions (some currently modiefied or unused)
int is_marked_node(node_t* node){
    return atomic_load(&node->mark);
}

void set_unmarked_node(node_t* node){
    atomic_store(&(node->mark), 0);
}

void set_marked_node(node_t* node){
    atomic_store(&(node->mark), 1);
}

int mark_neighbour_node(node_t* given, node_t* expected){
    return NULL;
}

void print_list(){
    node_t* tmp = list.head;
    while(tmp != NULL){
        printf("%d\n",tmp->key);
        tmp = tmp->next;
    }
}