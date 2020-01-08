#include "api/SysCallHandler.hpp"

using namespace std;

class EogenRuntime {

public:
    EogenRuntime();
    Codes::Code* run_code(const list<Codes::Code*>& codes);
    bool handle_if_section(Codes::Code* condition, const list<Codes::Code*>& codes);
    bool handle_switch_section(Codes::Code* c1, Codes::Code* c2);
    Codes::Code* reduce_code(Codes::Code* code);
};