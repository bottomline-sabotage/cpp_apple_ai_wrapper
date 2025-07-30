# C++ Apple Intelligence Wrapper
Is a very simple and thrown together (aka, this *might* have some weird bugs) C++ wrapper for the macOS [Foundation Models API](https://developer.apple.com/documentation/foundationmodels) (available on macOS 26) which allows Swift code to use Apple Intelligence. 

See a proper example in test.cpp

> [!CAUTION]
> This is ONLY supported on macOS Tahoe (which is currently in beta).

## Compiling
Compiling is pretty simple. For reference, I used Apple clang++ (with C++20) and swiftc.
Just ensure you follow this order:
  1. Compile runFunction.cpp
  2. Compile src.swift (link to the compled runFunction) using swiftc
  3. Compile code that uses the wrapper (e.g., test.cpp), linking to the compiled src.swift
