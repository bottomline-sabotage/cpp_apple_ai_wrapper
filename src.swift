/* 

swiftc -emit-library \
-Xlinker /Users/tyler/C++/includes/apple-intelligence/runFunction.dylib \
-parse-as-library \
-o src.dylib \
src.swift

       */

import FoundationModels
import Dispatch

@_silgen_name("runFunction")
func runFunction(_ function : UnsafeRawPointer?, _ text : UnsafeMutablePointer<CChar>?);

public struct generation_options {
    public var maximumResponseTokens: Int32;
    public var greedy : CBool;
    public var temperature : CDouble?;
};

var model = SystemLanguageModel.default;
var initedOnce : Bool = false;
var session : LanguageModelSession? = nil;
var sessionOptions : GenerationOptions?;
var instructions : String = "";
var callback : UnsafeRawPointer?;
var isResponding : CBool?; // session?.isResponding exists, but it didn't work for me.

@_cdecl("start_model_to_swift")
public func start_model_to_swift(_ ptr: UnsafeRawPointer) -> Int32 {
    let options = ptr.assumingMemoryBound(to: generation_options.self).pointee;

    switch model.availability {
        case .unavailable(.deviceNotEligible):
            return 1;
        case .unavailable(.appleIntelligenceNotEnabled):
            return 2;
        case .unavailable(.modelNotReady):
            return 3;
        case .unavailable(let other):
            print("Failed to init Apple Intelligence: ", other);
            return 4;

        // Hey, it works!
        default:
            initedOnce = true;
			let maxTokens : Int = Int(options.maximumResponseTokens);
            sessionOptions = GenerationOptions(sampling: (options.greedy ? .greedy : nil), temperature: options.temperature, maximumResponseTokens: maxTokens);
            
            session = nil; session = LanguageModelSession(instructions: instructions);

            return 0; 
    }
}

@_cdecl("set_instructions_to_swift")
public func set_instructions_to_swift(_ ptr: UnsafePointer<CChar>) {
    instructions = String(cString: ptr);
}

func handle_request(_ request : String) async {

    let errorMessage = "Sorry, we are unable to process that request right now.";
    
    do {
        let response = try await session?.respond(to: request);
        let str = response?.content;
        let cString = strdup(str);
        if cString != nil {
            runFunction(callback, cString);
        } else {
            free(cString);
        }
    } catch let error {
        print(error);
        let cString = strdup(errorMessage);
        runFunction(callback, cString);
    }

    isResponding = false;
}

@_cdecl("send_to_swift")
public func send_to_swift(_ request: UnsafePointer<CChar>, _ cb : UnsafeRawPointer) {
    if isResponding ?? false {return;}
    isResponding = true;

    callback = cb;

    Task {
        await handle_request(String(cString: request));
    }

    // runFunction(callback, strdup("Test"));
}

@_cdecl("is_processing")
public func is_processing() -> CBool {
    return isResponding ?? false;
}

@_cdecl("free_swift_string")
public func free_swift_string(_ ptr : UnsafeMutablePointer<CChar>?) {
    if ptr != nil {
        free(ptr);
    }
}



// let instructions = """
//     Respond in JSON:

//     {
//         "content": ...
//     }
// """;
// session = LanguageModelSession(instructions: instructions);

// let samplePrompt = "What is the Swift API to create a window called?";
// let response = try await session?.respond(to: samplePrompt)

// print(response);