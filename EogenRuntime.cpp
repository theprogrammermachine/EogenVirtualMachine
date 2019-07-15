#include "EogenRuntime.hpp"
#include <vector>
#include <cmath>
#include <c++/iostream>
#include "api/IO.hpp"

using namespace std;

vector< unordered_map<string, Codes::Code*> > ids;
vector< unordered_map<string, Codes::Function*> > funcs;
unordered_map<string, Codes::Class*> classes;

EogenRuntime::EogenRuntime() {

    unordered_map<string, Codes::Code*> levelIds;
    ids.push_back(levelIds);
    unordered_map<string, Codes::Function*> levelFuncs;
    funcs.push_back(levelFuncs);

    Codes::Class ioClass {};
    Codes::SysCall io_print_syscall {};
    io_print_syscall.callRefStr = "io.print";
    Codes::Function io_print_func {};
    io_print_func.codes.push_back(&io_print_syscall);
    ioClass.funcsData["print"] = &io_print_func;
    classes.insert({"io", &ioClass});
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
            SysCallHandler sysCall {};
            sysCall.handleSystemCall(syscall->callRefStr, syscall->entries);
        }
        else if (auto *asg = dynamic_cast<Codes::Assignment*>(code)) {
            Codes::Code redId;
            reduce_code(asg->var, &redId);
            if (auto* id = dynamic_cast<Codes::Identifier*>(&redId)) {
                bool found = false;
                for (int counter = ids.size() - 1; counter >= 0 ; counter--) {
                    if (ids[counter][id->name] != nullptr) {
                        Codes::Code redValue;
                        reduce_code(asg->value, &redValue);
                        ids[counter][id->name] = &redValue;
                        found = true;
                        cout << "hi";
                        break;
                    }
                }
                if (!found) {
                    Codes::Code redValue;
                    reduce_code(asg->value, &redValue);
                    ids[ids.size() - 1].insert({id->name, &redValue});
                }
            }
        }
        else if (auto *call = dynamic_cast<Codes::Call*>(code)) {
            Codes::Code redFunc;
            reduce_code(call->funcReference, &redFunc);
            if (auto* func = dynamic_cast<Codes::Function*>(&redFunc)) {
                unordered_map<string, Codes::Code*> levelIds;
                for (const pair<string, Codes::Code*>& kvp : readWholeMap<string, Codes::Code*>(call->entries)) {
                    Codes::Code redEntry;
                    reduce_code(kvp.second, &redEntry);
                    levelIds[kvp.first] = &redEntry;
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
            Codes::Code redCounter;
            reduce_code(counterLoop->limit, &redCounter);
            Codes::Code redStep;
            reduce_code(counterLoop->step, &redStep);
            if (auto *limitVal = dynamic_cast<Codes::ValueNumber*>(&redCounter)) {
                if (auto *stepVal = dynamic_cast<Codes::ValueNumber*>(&redStep)) {
                    for (int counter = 0; counter < limitVal->value; counter += (int)(stepVal->value)) {
                        if ((limitVal = dynamic_cast<Codes::ValueNumber*>(&redCounter))) {
                            if ((stepVal = dynamic_cast<Codes::ValueNumber*>(&redStep))) {
                                unordered_map<string, Codes::Code*> levelIds;
                                levelIds[id.name] = limitVal;
                                ids.push_back(levelIds);
                                unordered_map<string, Codes::Function*> levelFuncs;
                                funcs.push_back(levelFuncs);
                                run_code(counterLoop->codes);
                                funcs.pop_back();
                                ids.pop_back();
                                reduce_code(counterLoop->limit, &redCounter);
                                reduce_code(counterLoop->step, &redStep);
                            }
                        }
                    }
                }
            }
        }
        else if (auto *foreachLoop = dynamic_cast<Codes::Foreach*>(code)) {
            Codes::Code arr;
            reduce_code(foreachLoop->collection, &arr);
            if (auto *coll = dynamic_cast<Codes::Array*>(&arr)) {
                if (auto *id = dynamic_cast<Codes::Identifier*>(&foreachLoop->temp)) {
                    for (Codes::Code* item : coll->items) {
                        Codes::Code redItem;
                        reduce_code(item, &redItem);
                        unordered_map<string, Codes::Code*> levelIds;
                        levelIds[id->name] = &redItem;
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
                Codes::Code redCond;
                reduce_code(whileLoop->condition, &redCond);
                if (auto *cond = dynamic_cast<Codes::ValueBool*>(&redCond)) {
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
            Codes::Code redCode2;
            reduce_code(onChain->code2, &redCode2);
            if (auto* callO = dynamic_cast<Codes::Call*>(onChain->code1)) {
                Codes::Code redCallFunc;
                reduce_code(callO->funcReference, &redCallFunc);
                cout << typeid(redCallFunc).name();
                if (auto* funcO = dynamic_cast<Codes::Identifier*>(&redCallFunc)) {
                    cout << "hmm";
                    if (auto *instance = dynamic_cast<Codes::Instance *>(&redCode2)) {
                        Codes::Code rawClassRef;
                        reduce_code(instance->classReference, &rawClassRef);
                        if (auto *classRef = dynamic_cast<Codes::Class *>(&rawClassRef)) {
                            Codes::Function* f = classRef->funcsData[funcO->name];
                            unordered_map<string, Codes::Code*> levelIds;
                            ids.push_back(levelIds);
                            unordered_map<string, Codes::Function*> levelFuncs;
                            funcs.push_back(levelFuncs);
                            run_code(f->codes);
                            funcs.pop_back();
                            ids.pop_back();
                        }
                    }
                    else if (auto *classObj = dynamic_cast<Codes::Class*>(&redCode2)) {
                        Codes::Function* f = classObj->funcsData[funcO->name];
                        unordered_map<string, Codes::Code*> levelIds;
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

void EogenRuntime::reduce_code(Codes::Code* code, Codes::Code* result) {
    if (auto* opSum = dynamic_cast<Codes::MathExpSum*>(code)) {
        Codes::Code redOperand1;
        reduce_code(opSum->value1, &redOperand1);
        Codes::Code redOperand2;
        reduce_code(opSum->value2, &redOperand2);
        if (auto* operand1 = dynamic_cast<Codes::ValueNumber*>(code)) {
            if (auto* operand2 = dynamic_cast<Codes::ValueNumber*>(code)) {
                Codes::ValueNumber c;
                c.value = operand1->value + operand2->value;
                *result = c;
            }
            else if (auto* operand2_1 = dynamic_cast<Codes::ValueString*>(code)) {
                Codes::ValueString c;
                c.value = to_string(operand1->value) + operand2_1->value;
                *result = c;
            }
            else if (auto* operand2_2 = dynamic_cast<Codes::ValueBool*>(code)) {
                Codes::ValueNumber c;
                c.value = operand1->value + (operand2_2->value ? 1 : 0);
                *result = c;
            }
        }
        else if (auto* operand1_1 = dynamic_cast<Codes::ValueString*>(code)) {
            if (auto* operand2 = dynamic_cast<Codes::ValueNumber*>(code)) {
                Codes::ValueString c;
                c.value = operand1_1->value + to_string(operand2->value);
                *result = c;
            }
            else if (auto* operand2_1 = dynamic_cast<Codes::ValueString*>(code)) {
                Codes::ValueString c;
                c.value = operand1_1->value + operand2_1->value;
                *result = c;
            }
            else if (auto* operand2_2 = dynamic_cast<Codes::ValueBool*>(code)) {
                Codes::ValueString c;
                c.value = operand1_1->value + to_string(operand2_2->value);
                *result = c;
            }
        }
        else if (auto* operand1_2 = dynamic_cast<Codes::ValueBool*>(code)) {
            if (auto* operand2 = dynamic_cast<Codes::ValueNumber*>(code)) {
                Codes::ValueNumber c;
                c.value = (operand1_2->value ? 1 : 0) + operand2->value;
                *result = c;
            }
            else if (auto* operand2_1 = dynamic_cast<Codes::ValueString*>(code)) {
                Codes::ValueString c;
                c.value = to_string(operand1_2->value) + operand2_1->value;
                *result = c;
            }
            else if (auto* operand2_2 = dynamic_cast<Codes::ValueBool*>(code)) {
                Codes::ValueBool c;
                c.value = operand1_2->value || operand2_2->value;
                *result = c;
            }
        }
    }
    else if (auto* opMinus = dynamic_cast<Codes::MathExpSum*>(code)) {
        Codes::Code redOperand1;
        reduce_code(opMinus->value1, &redOperand1);
        Codes::Code redOperand2;
        reduce_code(opMinus->value2, &redOperand2);
        if (auto* operand1 = dynamic_cast<Codes::ValueNumber*>(code)) {
            if (auto* operand2 = dynamic_cast<Codes::ValueNumber*>(code)) {
                Codes::ValueNumber c;
                c.value = operand1->value - operand2->value;
                *result = c;
            }
        }
    }
    else if (auto* opMultiply = dynamic_cast<Codes::MathExpSum*>(code)) {
        Codes::Code redOperand1;
        reduce_code(opMultiply->value1, &redOperand1);
        Codes::Code redOperand2;
        reduce_code(opMultiply->value2, &redOperand2);
        if (auto* operand1 = dynamic_cast<Codes::ValueNumber*>(code)) {
            if (auto* operand2 = dynamic_cast<Codes::ValueNumber*>(code)) {
                Codes::ValueNumber c;
                c.value = operand1->value * operand2->value;
                *result = c;
            }
        }
    }
    else if (auto* opDivide = dynamic_cast<Codes::MathExpSum*>(code)) {
        Codes::Code redOperand1;
        reduce_code(opDivide->value1, &redOperand1);
        Codes::Code redOperand2;
        reduce_code(opDivide->value2, &redOperand2);
        if (auto* operand1 = dynamic_cast<Codes::ValueNumber*>(code)) {
            if (auto* operand2 = dynamic_cast<Codes::ValueNumber*>(code)) {
                Codes::ValueNumber c;
                c.value = operand1->value / operand2->value;
                *result = c;
            }
        }
    }
    else if (auto* opPower = dynamic_cast<Codes::MathExpPower*>(code)) {
        Codes::Code redOperand1;
        reduce_code(opPower->value1, &redOperand1);
        Codes::Code redOperand2;
        reduce_code(opPower->value2, &redOperand2);
        if (auto* operand1 = dynamic_cast<Codes::ValueNumber*>(code)) {
            if (auto* operand2 = dynamic_cast<Codes::ValueNumber*>(code)) {
                Codes::ValueNumber c;
                c.value = pow(operand1->value, operand2->value);
                *result = c;
            }
        }
    }
    else if (auto* opAnd = dynamic_cast<Codes::MathExpAnd*>(code)) {
        Codes::Code redOperand1;
        reduce_code(opAnd->value1, &redOperand1);
        Codes::Code redOperand2;
        reduce_code(opAnd->value2, &redOperand2);
        if (auto* operand1 = dynamic_cast<Codes::ValueBool*>(code)) {
            if (auto* operand2 = dynamic_cast<Codes::ValueBool*>(code)) {
                Codes::ValueBool c;
                c.value = operand1->value && operand2->value;
                *result = c;
            }
        }
    }
    else if (auto* opOr = dynamic_cast<Codes::MathExpOr*>(code)) {
        Codes::Code redOperand1;
        reduce_code(opOr->value1, &redOperand1);
        Codes::Code redOperand2;
        reduce_code(opOr->value2, &redOperand2);
        if (auto* operand1 = dynamic_cast<Codes::ValueBool*>(code)) {
            if (auto* operand2 = dynamic_cast<Codes::ValueBool*>(code)) {
                Codes::ValueBool c;
                c.value = operand1->value || operand2->value;
                *result = c;
            }
        }
    }
    else if (auto* ofChain = dynamic_cast<Codes::Of*>(code)) {
        Codes::Code redCode1;
        reduce_code(ofChain->code1, &redCode1);
        Codes::Code redCode2;
        reduce_code(ofChain->code2, &redCode2);
        if (auto* prop = dynamic_cast<Codes::Identifier*>(&redCode1)) {
            if (auto* refInst = dynamic_cast<Codes::Instance*>(&redCode2)) {
                auto* field = refInst->data[prop->name];
                if (field != nullptr) {
                    *result = *field;
                }
            }
            else if (auto* refClass = dynamic_cast<Codes::Class*>(&redCode2)) {
                auto* field = refClass->fieldsData[prop->name];
                if (field != nullptr) {
                    *result = *field;
                }
            }
        }
    }
    else if (auto* onChain = dynamic_cast<Codes::On*>(code)) {
        Codes::Code redCode2;
        reduce_code(onChain->code2, &redCode2);
        if (auto* call = dynamic_cast<Codes::Call*>(onChain->code1)) {
            Codes::Code redCallFunc;
            reduce_code(call->funcReference, &redCallFunc);
            if (auto* func = dynamic_cast<Codes::Function*>(&redCallFunc)) {
                if (auto *instance = dynamic_cast<Codes::Instance *>(&redCode2)) {
                    Codes::Code rawClassRef;
                    reduce_code(instance->classReference, &rawClassRef);
                    if (auto *classRef = dynamic_cast<Codes::Class *>(&rawClassRef)) {
                        Codes::Function* f = classRef->funcsData[func->name];
                        unordered_map<string, Codes::Code*> levelIds;
                        ids.push_back(levelIds);
                        unordered_map<string, Codes::Function*> levelFuncs;
                        funcs.push_back(levelFuncs);
                        run_code(f->codes);
                        funcs.pop_back();
                        ids.pop_back();
                    }
                }
                else if (auto *classObj = dynamic_cast<Codes::Class*>(&redCode2)) {
                    Codes::Function* f = classObj->funcsData[func->name];
                    unordered_map<string, Codes::Code*> levelIds;
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
    else if (auto* instantiate = dynamic_cast<Codes::Instantiate*>(code)) {
        unordered_map<string, Codes::Code*> entries;
        for (const pair<string, Codes::Code*>& kvp : readWholeMap(instantiate->entries)) {
            Codes::Code redEntry;
            reduce_code(kvp.second, &redEntry);
            entries[kvp.first] = &redEntry;
        }
        Codes::Instance instance;
        instance.classReference = instantiate->classReference;
        instance.data = entries;
        *result = instance;
    }
    else if (auto* identifier = dynamic_cast<Codes::Identifier*>(code)) {
        Codes::Code* varVal;
        for (int counter = ids.size() - 1; counter >= 0; counter--) {
            varVal = ids[counter][identifier->name];
            if (varVal != nullptr) {
                reduce_code(varVal, result);
                return;
            }
        }
        Codes::Function* funcVal;
        for (int counter = funcs.size() - 1; counter >= 0; counter--) {
            funcVal = funcs[counter][identifier->name];
            if (funcVal != nullptr) {
                reduce_code(funcVal, result);
                return;
            }
        }
        Codes::Class* classVal = classes[identifier->name];
        if (classVal != nullptr) {
            *result = *classVal;
            return;
        }
        cout << "is call " << identifier->name;
        *result = *identifier;
    }
    else if (auto *call = dynamic_cast<Codes::Call*>(code)) {
        Codes::Code redFunc;
        reduce_code(call->funcReference, &redFunc);
        if (auto* func = dynamic_cast<Codes::Function*>(&redFunc)) {
            unordered_map<string, Codes::Code*> levelIds;
            for (const pair<string, Codes::Code*>& kvp : readWholeMap<string, Codes::Code*>(call->entries)) {
                Codes::Code redEntry;
                reduce_code(kvp.second, &redEntry);
                levelIds[kvp.first] = &redEntry;
            }
            ids.push_back(levelIds);
            unordered_map<string, Codes::Function*> levelFuncs;
            funcs.push_back(levelFuncs);
            run_code(func->codes);
            funcs.pop_back();
            ids.pop_back();
        }
    }
    *result = Codes::Code();
}

template <typename T>
bool EogenRuntime::convertBoostAnyToType(boost::any arg, T* address) {
    try {
        *address = boost::any_cast<T &>(arg);
        return true;
    }
    catch (exception) {
        return false;
    }
}

bool EogenRuntime::handle_if_section(Codes::Code* condition, list<Codes::Code*> codes) {
    bool matched = false;
    Codes::Code redCond;
    reduce_code(condition, &redCond);
    if (auto *value = dynamic_cast<Codes::Value*>(&redCond)) {
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
    Codes::Code redCode1;
    reduce_code(c1, &redCode1);
    if (auto *value1 = dynamic_cast<Codes::Value*>(&redCode1)) {
        v1 = value1;
    }
    else {
        return false;
    }
    Codes::Code redCode2;
    reduce_code(c2, &redCode2);
    if (auto *value2 = dynamic_cast<Codes::Value*>(&redCode2)) {
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