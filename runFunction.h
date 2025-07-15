/** Is used by Swift in order to run C++ functions from within Swift (used for cbs) 
 * 
 * @warning I don't know why, but this **must be compiled to run_function.dylib and runFunction.dylib**. I don't know why, but Swift's linker complains otherwise. 
 * 
*/

#include <iostream>
#include <functional>

#pragma once

extern "C" {
    void runFunction(std::function<void(const char* str)>* func, const char* str);
}