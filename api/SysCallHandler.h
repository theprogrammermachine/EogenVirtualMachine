
#include <c++/string>
#include "../Models.hpp"


using namespace std;

class SysCallHandler {

public:
    void handleSystemCall(string callRefStr, unordered_map<string, Codes::Code*> entries);
};
