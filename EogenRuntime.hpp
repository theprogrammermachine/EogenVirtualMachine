#include "SysCallHandler.h"

using namespace std;

class EogenRuntime {

    public:
        EogenRuntime();
        void run_code(const list<Codes::Code*>& codes);
        bool handle_if_section(Codes::Code* condition, list<Codes::Code*> codes);
        bool handle_switch_section(Codes::Code* c1, Codes::Code* c2);
        void reduce_code(Codes::Code* code, Codes::Code* result);
        template <typename T>
        static bool convertBoostAnyToType(boost::any var, T* address);
};