
#include <string>
#include <iostream>
#include "Stack.hpp"

using namespace std;
int main(int argc, char const *argv[])
{
    Stack a;
    cout << "Size is: " << a.get_size() << "\ntop = " << a.top();
    a.push("aaa");
    cout << "Size is: " << a.get_size() << "\ntop = " << a.top();
    a.push("bbb");
    cout << "Size is: " << a.get_size() << "\ntop = " << a.top();
    a.pop();
    cout << "Size is: " << a.get_size() << "\ntop = " << a.top();

    // string b = "aaaa";
    // cout << b.substr(0,3);
    // cout << b.compare(b.substr(0,3));

    return 0;
}
