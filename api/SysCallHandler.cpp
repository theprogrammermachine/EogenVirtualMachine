
#include <c++/iostream>
#include "SysCallHandler.h"
#include "IO.hpp"

using namespace std;

void SysCallHandler::handleSystemCall(string callRef, unordered_map<string, Codes::Code*> entries) {
    if (callRef == "io.print") {
        IO* ioTools = new IO();
        unordered_map<string, Codes::Code*>::const_iterator i = entries.find("text");
        if (i == entries.end()) {
            cout << "\nfuck you !\n";
        }
        Codes::ValueString* textVal = dynamic_cast<Codes::ValueString*>(entries["text"]);
        ioTools->print(textVal->value);
    }
}
