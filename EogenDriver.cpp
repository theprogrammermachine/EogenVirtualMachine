#include <iostream>
#include "EogenRuntime.hpp"

using namespace std;

int main(int argc, char const *argv[])
{
    list<Codes::Code*> codes;

    auto* id = new Codes::Identifier();
    id->name = "num1";
    auto* assign = new Codes::Assignment();
    assign->var = id;
    auto* sum = new Codes::MathExpSum();
    auto* v1 = new Codes::ValueNumber();
    auto* v2 = new Codes::ValueNumber();
    v1->value = 10;
    v2->value = 15;
    sum->value1 = v1;
    sum->value2 = v2;
    assign->value = sum;
    codes.push_back(assign);

    auto* funcId = new Codes::Identifier();
    funcId->name = "print";
    auto* call = new Codes::Call();
    call->funcReference = funcId;
    unordered_map<string, Codes::Code*> entries;
    auto* strVal = new Codes::ValueString();
    strVal->value = "Result is : ";
    auto* strSum = new Codes::MathExpSum();
    strSum->value1 = strVal;
    strSum->value2 = id;
    entries["text"] = strSum;
    call->entries = entries;
    auto* classId = new Codes::Identifier();
    classId->name = "io";
    auto* onObj = new Codes::On();
    onObj->code1 = call;
    onObj->code2 = classId;
    codes.push_back(onObj);

    EogenRuntime er {};
    er.run_code(codes);
    int a;
    cin >> a;
    return 0;
}
