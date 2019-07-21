
#include <iostream>
#include "SysCallHandler.hpp"
#include "IO.hpp"
#include "Time.h"
#include "/usr/local/include/mpir.h"
#include "/usr/local/include/gmp.h"

using namespace std;

Codes::Code* SysCallHandler::handleSystemCall(string callRef, unordered_map<string, Codes::Code*> entries) {
    if (callRef == "io.print") {
        IO* ioTools = new IO();
        auto i = entries.find("text");
        Codes::ValueString* textVal = dynamic_cast<Codes::ValueString*>(entries["text"]);
        ioTools->print(textVal->value);
        return new Codes::Code();
    }
    if (callRef == "time.currentMillis") {
        Time* timeTools = new Time();
        auto result = timeTools->getCurrentTimeMillis();
        auto* valNum = new Codes::ValueNumber();
        valNum->setValue(result);
        return valNum;
    }
}
