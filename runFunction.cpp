#include <iostream>
#include <functional>
#include <any>

using namespace std;

extern "C" {
    void runFunction(std::function<void(const char* str)>* func, const char* str) {

        (*func)(str); 

        delete func;
    }
}