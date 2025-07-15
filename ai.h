#include <iostream>
#include <cstdint>
#include <functional>

#pragma once

#ifndef __APPLE__ 
static_assert("apple-intelligence/ai.h is exclusively compatible with MacOS Systems. Please either remove the header \"#include <apple-intelligence/ai.h>\" (or something similar), or run this on an Apple Intelligence supported Mac.");
#endif

namespace apple_intelligence {
    /** Used for possible error codes while **STARTING** the model
     * 
     * @attention After the model has started, these will no longer be used. Instead, an error is denoted with "Sorry, we are unable to process that request right now."
     */
    enum error_codes {
        /** The model is up and running. Feel free to send() requests */
        available,
        /** The device this is running on does not support Apple Intelligence. */
        notEligible,
        /** The user doesn't have Apple Intelligence enabled */
        notEnabled,
        /** The system is currently installing Apple Intelligence
         * @warning There has been several documented cases of the download of Apple Intelligence getting stuck. 
         */
        notReady,
        /** This is a catch all error. If seen, **panic** (most likely just try again) */
        unavailable
    };

    /** Is the options for the AI. Leaving everyone undefined will have the Foundations Model API itself chose it for you. So, there's nothing wrong with leaving it untouched.
     * @see https://developer.apple.com/documentation/foundationmodels/generationoptions
     */
    struct generation_options {
        /** Maximum number of tokens the model is allowed to create in its response */
        int32_t maximumResponseTokens;
        /** Always selects the most likely token */
        bool greedy = false;
        /** The temperature of the AI (see https://www.ibm.com/think/topics/llm-temperature). Must be between 0 and 2 (exclusive) */
        double temperature;
    };

    /** Represents the callback for the system callback
     * @warning Use ai_cb_t
     */
    typedef std::function<void(const char* str)> ai_cb_cstring_t;
    /** Represents the callback for your apple-intelligence send event */
    typedef std::function<void(std::string &str)> ai_cb_t;

    extern "C" {
        /** @warning Don't run directly to avoid manual memory management. Instead, use start_model() */
        error_codes start_model_to_swift(generation_options &x);
        /** @warning Don't run directly to avoid manual memory management. Instead, use send() */
        void send_to_swift(const char* str, ai_cb_cstring_t* cb);
        /** @warning Don't run directly to avoid manual memory management. Instead, use set_instructions() */
        void set_instructions_to_swift(const char* str);
        /** Delete a Swift-allocated c-string
         * 
         * @warning This is all managed internally. Therefore, you should never run this function.
         */
        void free_swift_string(const char* str);
        /**
         * Returns a bool indicating whether it's currently processing a message.
         * @return Whether it is processing. Trying to send() while is_processing() returns true will fail both responses
         *
         * @example test.cpp
         */
        bool is_processing();
    }

    /** Starts the model (thus, starts a chat log as well). If you'd like to erase the history, simply run this again. You **must** run this after setting instructions. */
    error_codes start_model() {
        generation_options x;
        return start_model_to_swift(x);
    }
    /** Starts the model (thus, starts a chat log as well). If you'd like to erase the history, simply run this again. You **must** run this after setting instructions. The difference between this and start_model() is that this one expects generation_options, while start_model() *auto-generates it* */
    error_codes start_model(generation_options &x) {
        return start_model_to_swift(x);
    }

    /** Will ask the AI a question. It will then run a callback that automatically handles C strings, covnerts it to std::string, then runs your callback
     * This will not throw an error, instead the Swift code will change the response to "Sorry, we are unable to process that request right now."
     * 
     * @attention Run this in a while(true) loop, with is_processing() checking if it's running
     * @returns Nothing, but it will pass a std::string to your callback **(not a c-style string!)**
     */
    void send(std::string str, ai_cb_t &callback) {
		char* cStr = new char[str.size()];
        for(int i = 0; i < str.size(); i++) {
            cStr[i] = str[i];
        }
        
        // Declare the system call back (takes in a cStr), converts it to a std::string, frees any used memory, then runs your callback
		ai_cb_cstring_t *SYSTEM_CALL_BACK = new ai_cb_cstring_t([&](const char* str) {
            std::string newStr = str; // Make sure to make a copy, not by reference
            free_swift_string(str); 
            callback(newStr);
            delete[] cStr; // delete the prompt str
        });  
    

		// runs the to_swift send function with the system callback
        send_to_swift(cStr, SYSTEM_CALL_BACK);
    }

    /** Will set the instructions. These instructions will be refreshed only when creating a new model. Therefore, run this before running start_model() 
     * @attention There are no "default instructions.
    */
    void set_instructions(std::string str) {
        const char* cStr = str.c_str();
		set_instructions_to_swift(cStr);
    }
}
