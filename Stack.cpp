
#include "Stack.hpp"


int Stack::alloc_more_space(){
    string* temp = new string[stack_size*2];
    for (int i = 0; i < stack_size; i++){
        temp[i] = stack[i];
    }
    delete[] stack;
    stack_size *= 2;
    stack = temp;
    return 0;
}

bool Stack::set_stack_at(int i, string s){
    if (i >=  stack_size){
        alloc_more_space();
    }
    stack[i] = s;
    return 0;
}

string Stack::push(std::string data){
    pthread_mutex_lock(&safe_lock);
    set_stack_at(pos++, data);
    pthread_mutex_unlock(&safe_lock);
    return data;
}

string Stack::pop(){
    pthread_mutex_lock(&safe_lock);
    if (pos <= 0){
        return "Non stack is empty";
    }
    string ans = get_data_at(--pos);
    stack[pos].clear();
    pthread_mutex_unlock(&safe_lock);
    return ans;
}

string Stack::top(){
    pthread_mutex_lock(&safe_lock);
    if (pos <= 0){
        return "Non stack is empty";
    }
    pthread_mutex_unlock(&safe_lock);
    return get_data_at(pos -1);
}