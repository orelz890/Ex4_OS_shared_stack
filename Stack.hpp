#include <string>
#include <string.h>
#include <iostream>
#include <mutex>
#include <pthread.h>

using namespace std;

#define PUSH 1
#define POP 2
#define TOP 3

pthread_mutex_t safe;

class Stack{

    private:
    string* stack;
    int pos = 0;
    int stack_size;

    

    public:
    Stack(){
        stack_size = 1;
        stack = new string[stack_size];
    }

    ~Stack(){
        delete[] stack;
    }
    int get_size(){
        return stack_size;
    }
    
    string* get_stack(){
        return stack;
    }
    
    string get_data_at(int i){
        if (i < stack_size){
            return stack[i];
        }
        return NULL;
    }

    bool set_stack_at(int i, string s);

    int alloc_more_space();

    string push(std::string data);

    string pop();

    string top();

};