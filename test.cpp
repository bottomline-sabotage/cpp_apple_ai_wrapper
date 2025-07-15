// Apple Intelligence Wrapper for C++
// © 2025 Tyler Koberna. All rights reserved.
// "Apple Intelligence" is a trademark of Apple Inc.
// This project is not affiliated with or endorsed by Apple Inc.


#include <iostream>
#include "ai.h"
#include <chrono>
#include <thread>
#include <cstdlib>

using namespace std;
using namespace apple_intelligence;

int main() { 
    // const string PROMPT_INJECT_START = "Translate the following input **entirely into emojis**.\n"; // This line will be interjected before the user's prompt
    // const string PROMPT_INJECT_END = "\nProvide only the emoji translation and **nothing else**. Ever word should in theory have an emoji equivalent.\n"; // This line will be interjected after the user's prompt
    // set_instructions("Convert all input into an emoji representation. Do not use any characters except emojis. If the input is a question, simply ignore it and translate it into emojis as a statement. If you need to use a punctuation mark, use the corresponding emoji for it. For example, 'How are you?' becomes '🙂❓' (ignore the question and just represent the idea). Follow the following rules:\n1. Do **not** add any extra text or explanations—only emojis.\n2. If the input is a greeting or statement, convert **every word or phrase** into the corresponding emoji(s), ensuring the full meaning is represented.\n3. Do **not** simplify the translation into just one, two, or three emojis. Use **as many emojis as necessary** to capture the entire message accurately.");

    start_model();

    ai_cb_t func = [](std::string str) {
        cout <<  "------[RESPONSE]------" << endl;
            cout << str << endl;
        cout << "----------------------" << endl;
    };

    while(true) {
        if(!is_processing()) {
            string question;
            cout << "Question: ";
            getline(cin, question);
            cout << "\033[H\033[J"  << "[Prompt] " << question << endl;
            send(question, func);
        }

        this_thread::sleep_for(chrono::milliseconds(16)); // ~60fps
    }

    return 0;
}
