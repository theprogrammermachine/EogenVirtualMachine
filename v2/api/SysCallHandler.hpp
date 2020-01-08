
#include <string>
#include "../v2/Models.hpp"


using namespace std;

class SysCallHandler {

public:
    Codes::Code* handleSystemCall(string callRefStr, unordered_map<string, Codes::Code*> entries);
};
