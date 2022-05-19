// We were aided by this site in this part:
// https://stackoverflow.com/questions/5422061/malloc-implementation

#include "Stack.hpp"
#include <iostream>

static free_block free_block_list_head = { 0, 0 };
static const size_t overhead = sizeof(size_t);
static const size_t align_to = 16;


void* my_malloc(size_t size) {
    
    size = (size + sizeof(size_t) + (align_to - 1)) & ~ (align_to - 1);
    free_block* block = free_block_list_head.next;
    free_block** head = &(free_block_list_head.next);
    while (block != 0) {
        if (block->size >= size) {
            *head = block->next;
            return ((char*)block) + sizeof(size_t);
        }
        head = &(block->next);
        block = block->next;
    }

    block = (free_block*)sbrk(size);
    block->size = size;

    return ((char*)block) + sizeof(size_t);
}

void my_free(void* ptr) {
    free_block* block = (free_block*)(((char*)ptr) - sizeof(size_t));
    block->next = free_block_list_head.next;
    free_block_list_head.next = block;
}


bool Stack::push(const char t[1024]){

    pthread_mutex_lock(&safe_lock);
    pnode new_space = (pnode)this->malloc(sizeof(node));
    memset(new_space,0,sizeof(new_space));

    new_space->next = NULL;
    new_space->prev = NULL;
    this->stack_size++;
    strcpy(new_space->txt, t);
    new_space->next = this->stack;

    if (this->stack != NULL || this->stack_size <= 0)
    {
        this->stack->prev = new_space;
    }
    this->stack = new_space;

    pthread_mutex_unlock(&safe_lock);
    return 0;
}

string Stack::pop(){
    pthread_mutex_lock(&safe_lock);
    if (stack_size <= 0 || this->stack == NULL){
        pthread_mutex_unlock(&safe_lock);
        return "Non stack is empty";
    }
    
    string ans = this->stack->txt;
    pnode temp = this->stack;
    this->stack = this->stack->next;
    if (this->stack != NULL)
    {
        this->stack->prev = NULL;
    }
    this->free(temp);
    this->stack_size--;
    pthread_mutex_unlock(&safe_lock);
    return ans;
}

string Stack::top(){
    
    pthread_mutex_lock(&safe_lock);
    if (stack_size <= 0 || this->stack == NULL){
        pthread_mutex_unlock(&safe_lock);
        return "Non stack is empty";
    }
    pthread_mutex_unlock(&safe_lock);
    return this->stack->txt;
}
