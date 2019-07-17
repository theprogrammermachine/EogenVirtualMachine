#include"EogenRuntime.hpp"
#include <vector>
#include <cmath>
#include <c++/iostream>
#include "api/IO.hpp"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace std;

vector< unordered_map<string, Codes::Code*> > ids;
vector< unordered_map<string, Codes::Function*> > funcs;
unordered_map<string, Codes::Class*> classes;

EogenRuntime::EogenRuntime() {

    unordered_map<string, Codes::Code*> levelIds;
    ids.push_back(levelIds);
    unordered_map<string, Codes::Function*> levelFuncs;
    funcs.push_back(levelFuncs);

    auto* ioClass = new Codes::Class();
    auto* io_print_syscall = new Codes::SysCall();
    io_print_syscall->callRefStr = "io.print";
    auto* io_print_func = new Codes::Function();
    io_print_func->codes.push_back(io_print_syscall);
    ioClass->funcsData["print"] = io_print_func;
    classes["io"] = ioClass;
}

template <typename K, typename V>
list<pair<K, V>> readWholeMap(unordered_map<K, V> map) {
    list<pair<K, V>> result;
    for (pair<K, V> item : map)
        result.push_back(item);
    return result;
}

void EogenRuntime::run_code(const list<Codes::Code*>& codes) {

    for (Codes::Code* code : codes) {

        if (auto* syscall = dynamic_cast<Codes::SysCall*>(code)) {
            auto* sysCallHandler = new SysCallHandler();
            sysCallHandler->handleSystemCall(syscall->callRefStr, ids.back());
        }
        else if (auto *asg = dynamic_cast<Codes::Assignment*>(code)) {
            if (auto* id = dynamic_cast<Codes::Identifier*>(asg->var)) {
                bool found = false;
                for (int counter = ids.size() - 1; counter >= 0 ; counter--) {
                    if (ids[counter][id->name] != nullptr) {
                        Codes::Code* redValue = reduce_code(asg->value);
                        ids[counter][id->name] = redValue;
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    ids[ids.size() - 1][id->name] = reduce_code(asg->value);
                }
            }
        }
        else if (auto *call = dynamic_cast<Codes::Call*>(code)) {
            Codes::Code* redFunc = reduce_code(call->funcReference);
            if (auto* func = dynamic_cast<Codes::Function*>(redFunc)) {
                unordered_map<string, Codes::Code*> levelIds;
                for (const pair<string, Codes::Code*>& kvp : readWholeMap<string, Codes::Code*>(call->entries)) {
                    Codes::Code* redEntry = reduce_code(kvp.second);
                    levelIds[kvp.first] = redEntry;
                }
                ids.push_back(levelIds);
                unordered_map<string, Codes::Function*> levelFuncs;
                funcs.push_back(levelFuncs);
                run_code(func->codes);
                funcs.pop_back();
                ids.pop_back();
            }
        }
        else if (auto *cl = dynamic_cast<Codes::Class*>(code)) {
            classes[cl->name] = cl;
        }
        else if (auto *func = dynamic_cast<Codes::Function*>(code)) {
            funcs.back()[func->name] = func;
        }
        else if (auto *counterLoop = dynamic_cast<Codes::CounterFor*>(code)) {
            Codes::Identifier id;
            id.name = "counter";
            Codes::Code* redCounter = reduce_code(counterLoop->limit);
            Codes::Code* redStep = reduce_code(counterLoop->step);
            if (auto *limitVal = dynamic_cast<Codes::ValueNumber*>(redCounter)) {
                if (auto *stepVal = dynamic_cast<Codes::ValueNumber*>(redStep)) {
                    for (int counter = 0; counter < limitVal->value; counter += (int)(stepVal->value)) {
                        if ((limitVal = dynamic_cast<Codes::ValueNumber*>(redCounter))) {
                            if ((stepVal = dynamic_cast<Codes::ValueNumber*>(redStep))) {
                                unordered_map<string, Codes::Code*> levelIds;
                                levelIds[id.name] = limitVal;
                                ids.push_back(levelIds);
                                unordered_map<string, Codes::Function*> levelFuncs;
                                funcs.push_back(levelFuncs);
                                run_code(counterLoop->codes);
                                funcs.pop_back();
                                ids.pop_back();
                                redCounter = reduce_code(counterLoop->limit);
                                redStep = reduce_code(counterLoop->step);
                            }
                        }
                    }
                }
            }
        }
        else if (auto *foreachLoop = dynamic_cast<Codes::Foreach*>(code)) {
            Codes::Code* arr = reduce_code(foreachLoop->collection);
            if (auto *coll = dynamic_cast<Codes::Array*>(arr)) {
                if (auto *id = dynamic_cast<Codes::Identifier*>(&foreachLoop->temp)) {
                    for (Codes::Code* item : coll->items) {
                        Codes::Code* redItem = reduce_code(item);
                        unordered_map<string, Codes::Code*> levelIds;
                        levelIds[id->name] = redItem;
                        ids.push_back(levelIds);
                        unordered_map<string, Codes::Function*> levelFuncs;
                        funcs.push_back(levelFuncs);
                        run_code(foreachLoop->codes);
                        funcs.pop_back();
                        ids.pop_back();
                    }
                }
            }
        }
        else if (auto *whileLoop = dynamic_cast<Codes::While*>(code)) {
            while (true) {
                Codes::Code* redCond = reduce_code(whileLoop->condition);
                if (auto *cond = dynamic_cast<Codes::ValueBool*>(redCond)) {
                    if (cond->value) {
                        unordered_map<string, Codes::Code*> levelIds;
                        ids.push_back(levelIds);
                        unordered_map<string, Codes::Function*> levelFuncs;
                        funcs.push_back(levelFuncs);
                        run_code(whileLoop->codes);
                        funcs.pop_back();
                        ids.pop_back();
                    }
                    else {
                        break;
                    }
                }
            }
        }
        else if (auto *switchCond = dynamic_cast<Codes::Switch*>(code)) {
            for (Codes::Case* caseExp : switchCond->cases) {
                if (handle_switch_section(switchCond->value, caseExp->value)) {
                    unordered_map<string, Codes::Code*> levelIds;
                    ids.push_back(levelIds);
                    unordered_map<string, Codes::Function*> levelFuncs;
                    funcs.push_back(levelFuncs);
                    run_code(caseExp->codes);
                    funcs.pop_back();
                    ids.pop_back();
                    break;
                }
            }
        }
        else if (auto *ifCond = dynamic_cast<Codes::If*>(code)) {
            if (!handle_if_section(ifCond->condition, ifCond->codes)) {
                if (!ifCond->extras.empty()) {
                    for (Codes::Code* extra : ifCond->extras) {
                        if (auto *elseifCond = dynamic_cast<Codes::ElseIf*>(extra)) {
                            if (handle_if_section(elseifCond->condition, elseifCond->codes))
                                break;
                        }
                        else if (auto *elseCond = dynamic_cast<Codes::Else*>(extra)) {
                            unordered_map<string, Codes::Code*> levelIds;
                            ids.push_back(levelIds);
                            unordered_map<string, Codes::Function*> levelFuncs;
                            funcs.push_back(levelFuncs);
                            run_code(elseCond->codes);
                            funcs.pop_back();
                            ids.pop_back();
                        }
                    }
                }
            }
        }
        else if (auto *tryCatch = dynamic_cast<Codes::Try*>(code)) {
            try {
                unordered_map<string, Codes::Code*> levelIds;
                ids.push_back(levelIds);
                unordered_map<string, Codes::Function*> levelFuncs;
                funcs.push_back(levelFuncs);
                run_code(tryCatch->tryCode);
                funcs.pop_back();
                ids.pop_back();
            }
            catch(exception e) {
                unordered_map<string, Codes::Code*> levelIds;
                Codes::ValueObject val;
                val.value = e;
                levelIds["e"] = &val;
                ids.push_back(levelIds);
                unordered_map<string, Codes::Function*> levelFuncs;
                funcs.push_back(levelFuncs);
                run_code(tryCatch->catchCode);
                funcs.pop_back();
                ids.pop_back();
            }
        }
        else if (auto *onChain = dynamic_cast<Codes::On*>(code)) {
            Codes::Code* redCode2 = reduce_code(onChain->code2);
            if (auto* callO = dynamic_cast<Codes::Call*>(onChain->code1)) {
                Codes::Code* redCallFunc = reduce_code(callO->funcReference);
                if (auto* funcO = dynamic_cast<Codes::Identifier*>(redCallFunc)) {
                    if (auto *instance = dynamic_cast<Codes::Instance*>(redCode2)) {
                        Codes::Code* rawClassRef = reduce_code(instance->classReference);
                        if (auto *classRef = dynamic_cast<Codes::Class*>(rawClassRef)) {
                            Codes::Function* f = classRef->funcsData[funcO->name];
                            unordered_map<string, Codes::Code*> levelIds;
                            for (pair<string, Codes::Code*>& kvp : readWholeMap<string, Codes::Code*>(callO->entries)) {
                                Codes::Code* redEntry = reduce_code(kvp.second);
                                levelIds[kvp.first] = redEntry;
                            }
                            ids.push_back(levelIds);
                            unordered_map<string, Codes::Function*> levelFuncs;
                            funcs.push_back(levelFuncs);
                            run_code(f->codes);
                            funcs.pop_back();
                            ids.pop_back();
                        }
                    }
                    else if (auto *classObj = dynamic_cast<Codes::Class*>(redCode2)) {
                        Codes::Function* f = classObj->funcsData[funcO->name];
                        unordered_map<string, Codes::Code*> levelIds;
                        for (pair<string, Codes::Code*> kvp : callO->entries) {
                            Codes::Code* redEntry = reduce_code(kvp.second);
                            levelIds[kvp.first] = redEntry;
                        }
                        ids.push_back(levelIds);
                        unordered_map<string, Codes::Function*> levelFuncs;
                        funcs.push_back(levelFuncs);
                        run_code(f->codes);
                        funcs.pop_back();
                        ids.pop_back();
                    }
                }
            }
        }
    }
}

Codes::Code* EogenRuntime::reduce_code(Codes::Code* code) {
    if (auto* opSum = dynamic_cast<Codes::MathExpSum*>(code)) {
        Codes::Code* redOperand1 = reduce_code(opSum->value1);
        Codes::Code* redOperand2 = reduce_code(opSum->value2);
        if (auto* operand1 = dynamic_cast<Codes::ValueNumber*>(redOperand1)) {
            if (auto* operand2 = dynamic_cast<Codes::ValueNumber*>(redOperand2)) {
                auto* c = new Codes::ValueNumber();
                c->value = operand1->value + operand2->value;
                return c;
            }
            else if (auto* operand2_1 = dynamic_cast<Codes::ValueString*>(redOperand2)) {
                auto* c = new Codes::ValueString();
                c->value = to_string(operand1->value) + operand2_1->value;
                return c;
            }
            else if (auto* operand2_2 = dynamic_cast<Codes::ValueBool*>(redOperand2)) {
                auto* c = new Codes::ValueNumber();
                c->value = operand1->value + (operand2_2->value ? 1 : 0);
                return c;
            }
        }
        else if (auto* operand1_1 = dynamic_cast<Codes::ValueString*>(redOperand1)) {
            if (auto* operand2 = dynamic_cast<Codes::ValueNumber*>(redOperand2)) {
                auto* c = new Codes::ValueString();
                c->value = operand1_1->value + to_string(operand2->value);
                return c;
            }
            else if (auto* operand2_1 = dynamic_cast<Codes::ValueString*>(redOperand2)) {
                auto* c = new Codes::ValueString();
                c->value = operand1_1->value + operand2_1->value;
                return c;
            }
            else if (auto* operand2_2 = dynamic_cast<Codes::ValueBool*>(redOperand2)) {
                auto* c = new Codes::ValueString();
                c->value = operand1_1->value + to_string(operand2_2->value);
                return c;
            }
        }
        else if (auto* operand1_2 = dynamic_cast<Codes::ValueBool*>(redOperand1)) {
            if (auto* operand2 = dynamic_cast<Codes::ValueNumber*>(redOperand2)) {
                auto* c = new Codes::ValueNumber();
                c->value = (operand1_2->value ? 1 : 0) + operand2->value;
                return c;
            }
            else if (auto* operand2_1 = dynamic_cast<Codes::ValueString*>(redOperand2)) {
                auto* c = new Codes::ValueString();
                c->value = to_string(operand1_2->value) + operand2_1->value;
                return c;
            }
            else if (auto* operand2_2 = dynamic_cast<Codes::ValueBool*>(redOperand2)) {
                auto* c = new Codes::ValueBool();
                c->value = operand1_2->value || operand2_2->value;
                return c;
            }
        }
    }
    else if (auto* opMinus = dynamic_cast<Codes::MathExpSum*>(code)) {
        Codes::Code* redOperand1 = reduce_code(opMinus->value1);
        Codes::Code* redOperand2 = reduce_code(opMinus->value2);
        if (auto* operand1 = dynamic_cast<Codes::ValueNumber*>(redOperand1)) {
            if (auto* operand2 = dynamic_cast<Codes::ValueNumber*>(redOperand2)) {
                auto* c = new Codes::ValueNumber();
                c->value = operand1->value - operand2->value;
                return c;
            }
        }
    }
    else if (auto* opMultiply = dynamic_cast<Codes::MathExpSum*>(code)) {
        Codes::Code* redOperand1 = reduce_code(opMultiply->value1);
        Codes::Code* redOperand2 = reduce_code(opMultiply->value2);
        if (auto* operand1 = dynamic_cast<Codes::ValueNumber*>(redOperand1)) {
            if (auto* operand2 = dynamic_cast<Codes::ValueNumber*>(redOperand2)) {
                auto* c = new Codes::ValueNumber();
                c->value = operand1->value * operand2->value;
                return c;
            }
        }
    }
    else if (auto* opDivide = dynamic_cast<Codes::MathExpSum*>(code)) {
        Codes::Code* redOperand1 = reduce_code(opDivide->value1);
        Codes::Code* redOperand2 = reduce_code(opDivide->value2);
        if (auto* operand1 = dynamic_cast<Codes::ValueNumber*>(redOperand1)) {
            if (auto* operand2 = dynamic_cast<Codes::ValueNumber*>(redOperand2)) {
                auto* c = new Codes::ValueNumber();
                c->value = operand1->value / operand2->value;
                return c;
            }
        }
    }
    else if (auto* opPower = dynamic_cast<Codes::MathExpPower*>(code)) {
        Codes::Code* redOperand1 = reduce_code(opPower->value1);
        Codes::Code* redOperand2 = reduce_code(opPower->value2);
        if (auto* operand1 = dynamic_cast<Codes::ValueNumber*>(redOperand1)) {
            if (auto* operand2 = dynamic_cast<Codes::ValueNumber*>(redOperand2)) {
                auto* c = new Codes::ValueNumber();
                c->value = pow(operand1->value, operand2->value);
                return c;
            }
        }
    }
    else if (auto* opAnd = dynamic_cast<Codes::MathExpAnd*>(code)) {
        Codes::Code* redOperand1 = reduce_code(opAnd->value1);
        Codes::Code* redOperand2 = reduce_code(opAnd->value2);
        if (auto* operand1 = dynamic_cast<Codes::ValueBool*>(redOperand1)) {
            if (auto* operand2 = dynamic_cast<Codes::ValueBool*>(redOperand2)) {
                auto* c = new Codes::ValueBool();
                c->value = operand1->value && operand2->value;
                return c;
            }
        }
    }
    else if (auto* opOr = dynamic_cast<Codes::MathExpOr*>(code)) {
        Codes::Code* redOperand1 = reduce_code(opOr->value1);
        Codes::Code* redOperand2 = reduce_code(opOr->value2);
        if (auto* operand1 = dynamic_cast<Codes::ValueBool*>(redOperand1)) {
            if (auto* operand2 = dynamic_cast<Codes::ValueBool*>(redOperand2)) {
                auto* c = new Codes::ValueBool();
                c->value = operand1->value || operand2->value;
                return c;
            }
        }
    }
    else if (auto* ofChain = dynamic_cast<Codes::Of*>(code)) {
        Codes::Code* redCode1 = reduce_code(ofChain->code1);
        Codes::Code* redCode2 = reduce_code(ofChain->code2);
        if (auto* prop = dynamic_cast<Codes::Identifier*>(redCode1)) {
            if (auto* refInst = dynamic_cast<Codes::Instance*>(redCode2)) {
                auto* field = refInst->data[prop->name];
                if (field != nullptr) {
                    return field;
                }
            }
            else if (auto* refClass = dynamic_cast<Codes::Class*>(redCode2)) {
                auto* field = refClass->fieldsData[prop->name];
                if (field != nullptr) {
                    return field;
                }
            }
        }
    }
    else if (auto* onChain = dynamic_cast<Codes::On*>(code)) {
        Codes::Code* redCode2 = reduce_code(onChain->code2);
        if (auto* call = dynamic_cast<Codes::Call*>(onChain->code1)) {
            Codes::Code* redCallFunc = reduce_code(call->funcReference);
            if (auto* func = dynamic_cast<Codes::Function*>(redCallFunc)) {
                if (auto *instance = dynamic_cast<Codes::Instance *>(redCode2)) {
                    Codes::Code* rawClassRef = reduce_code(instance->classReference);
                    if (auto *classRef = dynamic_cast<Codes::Class*>(rawClassRef)) {
                        Codes::Function* f = classRef->funcsData[func->name];
                        unordered_map<string, Codes::Code*> levelIds;
                        ids.push_back(levelIds);
                        unordered_map<string, Codes::Function*> levelFuncs;
                        funcs.push_back(levelFuncs);
                        run_code(f->codes);
                        funcs.pop_back();
                        ids.pop_back();
                        return new Codes::Code();
                    }
                }
                else if (auto *classObj = dynamic_cast<Codes::Class*>(redCode2)) {
                    Codes::Function* f = classObj->funcsData[func->name];
                    unordered_map<string, Codes::Code*> levelIds;
                    ids.push_back(levelIds);
                    unordered_map<string, Codes::Function*> levelFuncs;
                    funcs.push_back(levelFuncs);
                    run_code(f->codes);
                    funcs.pop_back();
                    ids.pop_back();
                    return new Codes::Code();
                }
            }
        }
    }
    else if (auto* instantiate = dynamic_cast<Codes::Instantiate*>(code)) {
        unordered_map<string, Codes::Code*> entries;
        for (const pair<string, Codes::Code*>& kvp : readWholeMap(instantiate->entries)) {
            Codes::Code* redEntry = reduce_code(kvp.second);
            entries[kvp.first] = redEntry;
        }
        auto* instance = new Codes::Instance();
        instance->classReference = instantiate->classReference;
        instance->data = entries;
        return instance;
    }
    else if (auto* identifier = dynamic_cast<Codes::Identifier*>(code)) {
        Codes::Code* varVal;
        for (int counter = ids.size() - 1; counter >= 0; counter--) {
            unordered_map<string, Codes::Code*>::const_iterator i = ids[counter].find(identifier->name);
            if (i != ids[counter].end()) {
                varVal = ids[counter][identifier->name];
                Codes::Code* c = reduce_code(varVal);
                return c;
            }
        }
        Codes::Function* funcVal;
        for (int counter = funcs.size() - 1; counter >= 0; counter--) {
            unordered_map<string, Codes::Function*>::const_iterator i = funcs[counter].find(identifier->name);
            if (i != funcs[counter].end()) {
                funcVal = funcs[counter][identifier->name];
                return reduce_code(funcVal);
            }
        }
        Codes::Class* classVal = classes[identifier->name];
        if (classVal != nullptr) {
            return classVal;
        }
        auto* id = new Codes::Identifier();
        id->name = identifier->name;
        return id;
    }
    else if (auto *call = dynamic_cast<Codes::Call*>(code)) {
        Codes::Code* redFunc = reduce_code(call->funcReference);
        if (auto* func = dynamic_cast<Codes::Function*>(redFunc)) {
            unordered_map<string, Codes::Code*> levelIds;
            for (const pair<string, Codes::Code*>& kvp : readWholeMap<string, Codes::Code*>(call->entries)) {
                Codes::Code* redEntry = reduce_code(kvp.second);
                levelIds[kvp.first] = redEntry;
            }
            ids.push_back(levelIds);
            unordered_map<string, Codes::Function*> levelFuncs;
            funcs.push_back(levelFuncs);
            run_code(func->codes);
            funcs.pop_back();
            ids.pop_back();
        }
    }

    return code;
}

bool EogenRuntime::handle_if_section(Codes::Code* condition, const list<Codes::Code*>& codes) {
    bool matched = false;
    Codes::Code* redCond = reduce_code(condition);
    if (auto *value = dynamic_cast<Codes::Value*>(redCond)) {
        if (auto *valueNum = dynamic_cast<Codes::ValueNumber*>(value)) {
            if (valueNum->value == 1) {
                unordered_map<string, Codes::Code*> levelIds;
                ids.push_back(levelIds);
                unordered_map<string, Codes::Function*> levelFuncs;
                funcs.push_back(levelFuncs);
                run_code(codes);
                matched = true;
                funcs.pop_back();
                ids.pop_back();
            }
        }
        else if (auto *valueBool = dynamic_cast<Codes::ValueBool*>(value)) {
            if (valueBool->value != 0) {
                unordered_map<string, Codes::Code*> levelIds;
                ids.push_back(levelIds);
                unordered_map<string, Codes::Function*> levelFuncs;
                funcs.push_back(levelFuncs);
                run_code(codes);
                matched = true;
                funcs.pop_back();
                ids.pop_back();
            }
        }
    }
    return matched;
}

bool EogenRuntime::handle_switch_section(Codes::Code* c1, Codes::Code* c2) {
    Codes::Value *v1, *v2;
    Codes::Code* redCode1 = reduce_code(c1);
    if (auto *value1 = dynamic_cast<Codes::Value*>(redCode1)) {
        v1 = value1;
    }
    else {
        return false;
    }
    Codes::Code* redCode2 = reduce_code(c2);
    if (auto *value2 = dynamic_cast<Codes::Value*>(redCode2)) {
        v2 = value2;
    }
    else {
        return false;
    }

    if (auto *v1Num = dynamic_cast<Codes::ValueNumber*>(v1)) {
        if (auto *v2Num = dynamic_cast<Codes::ValueNumber*>(v2)) {
            if (v1Num->value == v2Num->value) {
                return true;
            }
        }
    }

    if (auto *v1Num = dynamic_cast<Codes::ValueBool*>(v1)) {
        if (auto *v2Num = dynamic_cast<Codes::ValueBool*>(v2)) {
            if (v1Num->value == v2Num->value) {
                return true;
            }
        }
    }

    if (auto *v1Num = dynamic_cast<Codes::ValueString*>(v1)) {
        if (auto *v2Num = dynamic_cast<Codes::ValueString*>(v2)) {
            if (v1Num->value == v2Num->value) {
                return true;
            }
        }
    }

    return false;
}