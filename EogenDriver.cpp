#include <iostream>
#include "EogenRuntime.hpp"

using namespace std;

Codes::Code* createTimeBlock(string label) {
    auto* div = new Codes::MathExpDivide();
    auto* funcId = new Codes::Identifier();
    funcId->name = "currentMillis";
    auto* call = new Codes::Call();
    call->funcReference = funcId;
    unordered_map<string, Codes::Code*> entries;
    call->entries = entries;
    auto* classId = new Codes::Identifier();
    classId->name = "time";
    auto* onObj = new Codes::On();
    onObj->code1 = call;
    onObj->code2 = classId;
    div->value1 = onObj;
    auto* vn1 = new Codes::ValueNumber();
    vn1->setValue(1000);
    div->value2 = vn1;

    auto* sum = new Codes::MathExpSum();
    sum->value1 = div;
    auto* str = new Codes::ValueString();
    str->value = " : " + label + "\n";
    sum->value2 = str;

    auto* funcId2 = new Codes::Identifier();
    funcId2->name = "print";
    auto* call2 = new Codes::Call();
    call2->funcReference = funcId2;
    unordered_map<string, Codes::Code*> entries2;
    entries2["text"] = sum;
    call2->entries = entries2;
    auto* classId2 = new Codes::Identifier();
    classId2->name = "io";
    auto* onObj2 = new Codes::On();
    onObj2->code1 = call2;
    onObj2->code2 = classId2;

    return onObj2;
}

int main(int argc, char const *argv[]) {

    list<Codes::Code*> codes;

    codes.push_back(createTimeBlock("start"));

    auto* id = new Codes::Identifier();
    id->name = "num1";
    auto* assign = new Codes::Assignment();
    assign->var = id;
    auto* sum = new Codes::MathExpPower();
    auto* v1 = new Codes::ValueNumber();
    auto* v2 = new Codes::ValueNumber();
    v1->setValue(5555555);
    v2->setValue(333333);
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
    strVal->value = "\n";
    auto* strSum = new Codes::MathExpSum();
    strSum->value1 = id;
    strSum->value2 = strVal;
    entries["text"] = strSum;
    call->entries = entries;
    auto* classId = new Codes::Identifier();
    classId->name = "io";
    auto* onObj = new Codes::On();
    onObj->code1 = call;
    onObj->code2 = classId;
    codes.push_back(onObj);

    codes.push_back(createTimeBlock("end"));

    EogenRuntime er {};
    er.run_code(codes);
    int a;
    cin >> a;
    return 0;
}
