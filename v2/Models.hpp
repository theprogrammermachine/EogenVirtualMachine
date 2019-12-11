#include <list>
#include <string>
#include </home/keyhan/globe_libs/boost_1_70_0/boost/any.hpp>
#include <unordered_map>
#include "mpir/mpirxx.h"
#include <stdio.h>

using namespace std;

class Codes {

    public:

    enum DataType {
        Number,
        String,
        Bool
    };

    enum DataLevel {
        ClassLevel,
        InstanceLevel
    };


    class Code {

        public:
        virtual ~Code() = default;

    };

    class SysCall : public Code {
    public:
        string callRefStr;
        unordered_map<string, Codes::Code*> entries;
    };

    class Identifier : public Code {

        public:
        string name;
    };

    class NotSatisfied : public Code {

    };

    class Prop : public Code {

        public:
        Identifier id;
        Code* value;
        DataLevel level;
    };

    class CounterFor : public Code {

        public:
        Code* limit;
        Code* step;
        list<Code*> codes;
    };

    class As : public Code {

        public:
        Code* code1;
        Identifier id;
    };

    class On : public Code {

        public:
        Code* code1;
        Code* code2;
    };

    class Of : public Code {

        public:
        Code* code1;
        Code* code2;
    };

    class Try : public Code {

        public:
        list<Code*> tryCode;
        Identifier exVar;
        list<Code*> catchCode;
    };

    class If : public Code {

        public:
        Code* condition;
        list<Code*> codes;
        list<Code*> extras;
    };

    class ElseIf : public Code {

        public:
        Code* condition;
        list<Code*> codes;
    };

    class Else : public Code {

        public:
        list<Code*> codes;
    };

    class Case : public Code {

        public:
        Code* value;
        list<Code*> codes;
    };

    class Switch : public Code {

        public:
        Code* value;
        list<Case*> cases;
    };

    class While : public Code {

        public:
        Code* condition;
        list<Code*> codes;
    };

    class For : public Code {

        public:
        Code* counter;
        Code* condition;
        Code* action;
        list<Code*> codes;
    };

    class Foreach : public Code {

        public:
        Identifier temp;
        Code* collection;
        list<Code*> codes;
    };

    class Function : public Code {

        public:
        string name;
        DataLevel level;
        list<Identifier*> params;
        list<Code*> codes;
    };

    class Value : public Code {

    };

    class ValueNumber : public Code {

        public:
        mpz_t value {};
        void setValue(long double val) {
            mpz_set_d(value, val);
        }
    };

    class ValueBool : public Code {

        public:
        bool value;
    };

    class ValueString : public Code {

        public:
        string value;
    };

    class ValueObject : public Code {
    public:
        boost::any value;
    };

    class Call : public Code {

        public:
        Code* funcReference;
        unordered_map<string, Code*> entries;
    };

    class Assignment : public Code {
        
        public:
        Code* var;
        Code* value;
    };

    class Variable : public Code {

        public:
        string name;
    };

    class Definition : public Code {

        public:
        string varName;
        string dataType;
    };

    class MathExpSum : public Code {

        public:
        Code* value1;
        Code* value2;
    };

    class MathExpSubstract : public Code {

        public:
        Code* value1;
        Code* value2;
    };

    class MathExpMultiply : public Code {

        public:
        Code* value1;
        Code* value2;
    };

    class MathExpDivide : public Code {

        public:
        Code* value1;
        Code* value2;
    };

    class MathExpMod : public Code {

        public:
        Code* value1;
        Code* value2;
    };

    class MathExpPower : public Code {

        public:
        Code* value1;
        Code* value2;
    };

    class MathExpUminus : public Code {

        public:
        Code* value;
    };

    class MathExpEqual : public Code {

        public:
        Code* value1;
        Code* value2;
    };

    class MathExpNE : public Code {

        public:
        Code* value1;
        Code* value2;
    };

    class MathExpGT : public Code {

        public:
        Code* value1;
        Code* value2;
    };

    class MathExpGE : public Code {

        public:
        Code* value1;
        Code* value2;
    };

    class MathExpLT : public Code {

        public:
        Code* value1;
        Code* value2;
    };

    class MathExpLE : public Code {

        public:
        Code* value1;
        Code* value2;
    };

    class MathExpAnd : public Code {

        public:
        Code* value1;
        Code* value2;
    };

    class MathExpOr : public Code {

        public:
        Code* value1;
        Code* value2;
    };

    class Increment : public Code {

        public:
        Variable var;
    };

    class Decrement : public Code {

        public:
        Variable var;
    };

    class Return : public Code {

        public:
        Code* value;
    };

    class Field : public Code {

        public:
        string name;
        Code* value;
    };

    class Instantiate : public Code {

        public:
        Code* classReference;
        unordered_map<string, Code*> entries;
    };

    class Instance : public Code {

        public:
        Code* classReference;
        unordered_map<string, Code*> data;
    };

    class Is : public Code {

        public:
        Code* code1;
        Code* code2;
    };

    class Behaviour : public Code {

        public:
        string name;
        list<Code*> codes;
    };

    class BasedOnExtension : public Code {

        public:
        string name;
    };

    class BehavesLikeExtension : public Code {

        public:
        string name;
    };

    class Class : public Code {

        public:
        string name;
        list<Code*> extensions;
        list<Code*> codes;
        unordered_map<string, Codes::Code*> fieldsData;
        unordered_map<string, Codes::Function*> funcsData;
    };

    class Array : public Code {

        public:
        list<Code*> items;
    };

    class ClassReference : public Code {

        public:
        string className;
    };

    class ArrayPushItem : public Code {

        public:
        string arrName;
        Code* value;
    };

    class ArrayItem : public Code {

        public:
        Code* array;
        Code* index;
    };

    class Chains : public Code {

        public:
        Code* code1;
        Code* code2;
    };

    class Reference : public Code {

        public:
        string name;
    };

    class Parenthesis : public Code {

        public:
        Code* code;
    };
};