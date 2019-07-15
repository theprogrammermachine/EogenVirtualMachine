
#include "SysCallHandler.h"
#include "IO.hpp"

using namespace std;

void SysCallHandler::handleSystemCall(string callRef, unordered_map<string, Codes::Code*> entries) {
    if (callRef == "io.print") {
        IO ioTools {};
        Codes::ValueString* textVal = dynamic_cast<Codes::ValueString*>(entries["text"]);
        ioTools.print(textVal->value);
    }
}
