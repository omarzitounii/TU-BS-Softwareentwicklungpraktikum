//
// Created by keanukreed on 01.05.23.
//

#include "../header/harrislist.h"

//testing with only one static list
//list_t list;

//fills head and tail in list
int initialise_list(list_t* list, harris_node_t* head, harris_node_t* tail){
    head->next = tail;
    list->head = head;
    list->tail = tail;
    return 1;
}

//creates node and initialises it
void initialise_node(harris_node_t* tmp, int key){
    atomic_store(&tmp->key, key);
    tmp->next = NULL;
    tmp->count = 1;
}

int is_marked_ref(long i)
{
    return (int) (i & 0x1L);
}

long unset_mark(long i)
{
    i &= ~0x1L;
    return i;
}

long set_mark(long i)
{
    i |= 0x1L;
    return i;
}

long get_unmarked_ref(long w)
{
    return w & ~0x1L;
}

long get_marked_ref(long w)
{
    return w | 0x1L;
}

//inserts node based on key and location
int insert(list_t* list, int key, struct harris_FreeStack* freeStack){
    //create helper and new node
    harris_node_t *new_node, *right_node, *left_node;
    new_node = harris_reuse(freeStack);
    initialise_node(new_node,key);
    do{
        //pinpoint location for new node
        right_node = search(list,&left_node, key, freeStack);
        //check if node with similar key is in list
        if((right_node != list->tail) && (right_node->key == key)){
            harris_retire(freeStack,new_node);
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

harris_node_t* search(list_t* list, harris_node_t **left, int key, harrisFreeStack* freeStack){
    //create helper nodes
    harris_node_t *left_next, *right_node;

    search_again:
    do{
        //set temp nodes to iterate through list
        harris_node_t* temp = list->head;
        harris_node_t* temp_next = list->head->next;
        *left = temp;

        //find left and right node
        do{
            /*temp->count++;
            temp_next->count++;*/
            //if node not marked set left bounds
            if(!is_marked_ref((long)temp_next)){
                (*left) = temp;
                left_next = temp_next;
            }
            //iterate and unmark iterated node
            //temp->count--;
            temp = (harris_node_t*) get_unmarked_ref((long)temp_next);
            //if iterated through complete list end loop
            if(temp == list->tail){
                /*temp->count--;
                temp_next->count--;*/
                break;
            }
            //set temp_next
            //temp->count--;
            //temp_next->count--;
            temp_next = temp->next;
        }while(is_marked_ref((long)temp_next) || atomic_load(&temp->key) < key);
        //do this until node is either marked or key does not fit anymore, temp is our right bound
        right_node = temp;

        //check if nodes are next to each other
        if(left_next == right_node){
            //check if new node has been logically deleted
            if((right_node != list->tail) && is_marked_ref((long)right_node->next)){
                //list has been modified there is a logically deleted node now
                goto search_again;
            } else {
                //list is in consistent state nodes have been set correctly
                return right_node;
            }
        }

        //error while setting left bound, repeat first step
        /*if((*left) == list->tail){
            continue;
        }*/

        //physically delete logically deleted nodes
        //save current node
        //check if marked node is still there
        if(atomic_compare_exchange_strong(&(*left)->next, &left_next, right_node)){
            //delete marked node
            harris_retire(freeStack, left_next);
            //check if there is another logically deleted node or if list has been modified
            if((right_node != list->tail) && is_marked_ref((long)right_node->next)){
                goto search_again;
            } else {
                return right_node;
            }
        }
    }while(1);
}

int delete(list_t* list, int key, struct harris_FreeStack* freeStack){
    //create helper nodes
    harris_node_t *left_node, * right_node_next, *right_node;
    //harris_node_t* right_node = harris_reuse(freeStack);
    //tmp needs to be allocated because if used out of scope, undefined things happen
    //harris_node_t* right_node_next= harris_reuse(freeStack);

    do{
        //pinpoint location of to be deleted
        right_node = search(list,&left_node, key, freeStack);
        //key is not in list
        if((right_node == list->tail) || (right_node->key != key)){
            return 0;
        }
        //check if wanted already logically deleted
        right_node_next = right_node->next;
        if(!is_marked_ref((long)right_node_next)){
            //create marked version of right next, tmp node
            //if list was not modified swap node to its marked version
            if(atomic_compare_exchange_strong(&(right_node->next),&right_node_next, (harris_node_t*)get_marked_ref((long)right_node_next))){
                break;
            }
        }
    } while(1);
    //if node isnt already phisically deleted delete it through search function
    if(!atomic_compare_exchange_strong(&(left_node->next), &right_node, right_node_next)){
        right_node = search(list, &left_node, right_node->key, freeStack);
    }
    //node can be decreased and freed if not used
    harris_retire(freeStack, right_node);
    return 1;
}

int find(list_t* list, int key, harrisFreeStack* freeStack){
    //create helper nodes
    harris_node_t *right_node, *left_node;
    //pinpoint wanted node location
    right_node = search(list, &left_node, key,freeStack);
    //check if wanted node was found
    if((right_node == list->tail) || (right_node->key != key)) {
        return 0;
    } else {
        return 1;
    }
}


void print_list(list_t* list){
    harris_node_t* tmp = list->head;
    while(tmp != NULL){
        printf("%d\n",tmp->key);
        tmp = tmp->next;
    }
}


//--------------------------------------3 Versionen von diesen FREE STRUCT UND FUNKTIONEN-----------------------


void harris_free_push(harrisFreeStack *freeStack, harris_node_t* data) {//push pointer  to the free list
    harrisFreeNode* node = malloc(sizeof(harrisFreeNode));
    node->data = data;
    node->next = freeStack->top;
    freeStack->top = node;
}

harris_node_t *harris_free_pop(harrisFreeStack *freeStack) { //take  adress from the free list
    if(freeStack->top == NULL) { //check if list empty
        return malloc(sizeof(harris_node_t));
    }
    harrisFreeNode *result = freeStack->top;
    freeStack->top = freeStack->top->next;
    harris_node_t* ret = result->data;
    free(result);
    return ret;


}

void harris_freeAll(harrisFreeStack *freeStack) { //delete all nodes at stack and free stack
    while(freeStack->top != NULL) {
        harrisFreeNode * temp = freeStack->top;
        freeStack->top = freeStack->top->next;
        free(temp->data);
        free(temp);
    }

}

void harris_retire(harrisFreeStack *freeStack, harris_node_t *pointer) { //add Node to the list to be later used
    harris_free_push(freeStack,pointer);
}

harris_node_t *harris_reuse(harrisFreeStack *freeStack) { //free node
    harris_node_t *result = harris_free_pop(freeStack);
    return result;
}

void freeSet(list_t *list) {
    while (list->head != NULL) {
        harris_node_t *temp = list->head;
        list->head = list->head->next;
        free(temp);
    }
}




