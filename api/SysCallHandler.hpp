
#include <string>
#include "../Models.hpp"


using namespace std;

class SysCallHandler {

public:
    Codes::Code* handleSystemCall(string callRefStr, unordered_map<string, Codes::Code*> entries);
};
