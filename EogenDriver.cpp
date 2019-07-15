#include <iostream>
#include "EogenRuntime.hpp"

using namespace std;

int main(int argc, char const *argv[])
{
    list<Codes::Code*> codes;

    Codes::Identifier id;
    id.name = "num1";
    Codes::Assignment assign;
    assign.var = &id;
    Codes::MathExpSum sum;
    Codes::ValueNumber v1, v2;
    v1.value = 10;
    v2.value = 15;
    sum.value1 = &v1;
    sum.value2 = &v2;
    assign.value = &sum;
    codes.push_back(&assign);

    Codes::Identifier funcId;
    funcId.name = "print";
    Codes::Call call;
    call.funcReference = &funcId;
    unordered_map<string, Codes::Code*> entries;
    Codes::ValueString strVal;
    strVal.value = "Hello world !";
    entries["text"] = &strVal;
    call.entries = entries;
    Codes::Identifier classId;
    classId.name = "io";
    Codes::On onObj;
    onObj.code1 = &call;
    onObj.code2 = &classId;
    codes.push_back(&onObj);

    EogenRuntime er {};
    er.run_code(codes);
    cout << "finished :)";
    int a;
    cin >> a;
    return 0;
}
