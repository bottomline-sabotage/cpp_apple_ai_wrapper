/** Is used by Swift in order to run C++ functions from within Swift (used for cbs) 
 * 
*/

#include <iostream>
#include <functional>

#pragma once

extern "C" {
    void runFunction(std::function<void(const char* str)>* func, const char* str);
}
