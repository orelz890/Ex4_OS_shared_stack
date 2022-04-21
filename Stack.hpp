#include <string>
#include <string.h>

using namespace std;

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
    int alloc_more_space(){
        string* temp = new string[stack_size*2];
        for (int i = 0; i < stack_size; i++){
            temp[i] = stack[i];
        }
        delete[] stack;
        stack_size *= 2;
        stack = temp;
        return 0;
    }

    string* get_stack(){
        return stack;
    }
    bool set_stack_at(int i, string s){
        if (i >=  stack_size){
            alloc_more_space();
        }
        stack[i] = s;
        return 0;
    }
    string get_data_at(int i){
        if (i < stack_size){
            return stack[i];
        }
        return NULL;
    }
    int push(std::string data){
        set_stack_at(pos++, data);
        return 0;
    }

    string pop(){
        if (pos <= 0){
            return "Empty";
        }
        string ans = get_data_at(--pos);
        stack[pos].clear();
        return ans;
    }

    string top(){
        if (pos <= 0){
            return "Empty";
        }
        return get_data_at(pos -1);
    }
};