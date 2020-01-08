
#include "List.h"
#include "Dictionary.h"

typedef struct {
    char* type;
} Code;

typedef struct
{
    Code base;
    char* type;
} Exp;

typedef struct
{
    Exp exp;
    Exp value1;
    char* value1Type;
    Exp value2;
    char* value2Type;
} Sum;

typedef struct
{
    Exp exp;
    Exp value1;
    char* value1Type;
    Exp value2;
    char* value2Type;
} Subtract;

typedef struct
{
    Exp exp;
    Exp value1;
    char* value1Type;
    Exp value2;
    char* value2Type;
} Multiply;

typedef struct
{
    Exp exp;
    Exp value1;
    char* value1Type;
    Exp value2;
    char* value2Type;
} Division;

typedef struct
{
    Exp exp;
    Exp value1;
    char* value1Type;
    Exp value2;
    char* value2Type;
} Mod;

typedef struct
{
    Exp exp;
    Exp value1;
    char* value1Type;
    Exp value2;
    char* value2Type;
} Pow;

typedef struct
{
    Exp exp;
    Exp value1;
    char* value1Type;
    Exp value2;
    char* value2Type;
} Equal;

typedef struct
{
    Exp exp;
    Exp value1;
    char* value1Type;
    Exp value2;
    char* value2Type;
} NE;

typedef struct
{
    Exp exp;
    Exp value1;
    char* value1Type;
    Exp value2;
    char* value2Type;
} LT;

typedef struct
{
    Exp exp;
    Exp value1;
    char* value1Type;
    Exp value2;
    char* value2Type;
} LE;

typedef struct
{
    Exp exp;
    Exp value1;
    char* value1Type;
    Exp value2;
    char* value2Type;
} GE;

typedef struct
{
    Exp exp;
    Exp value1;
    char* value1Type;
    Exp value2;
    char* value2Type;
} GT;

typedef struct
{
    Exp exp;
    Exp value1;
    char* value1Type;
    Exp value2;
    char* value2Type;
} And;

typedef struct
{
    Exp exp;
    Exp value1;
    char* value1Type;
    Exp value2;
    char* value2Type;
} Or;

typedef struct {
    Exp exp;
    char* valueType;
} Value;

typedef struct {
    Value base;
    char* value;
} StringValue;

typedef struct {
    Value base;
    double value;
} DoubleValue;

typedef struct {
    Value base;
    float value;
} FloatValue;

typedef struct {
    Value base;
    long value;
} LongValue;

typedef struct {
    Value base;
    int value;
} IntValue;

typedef struct {
    Value base;
    short value;
} ShortValue;

typedef struct {
    Value base;
    bool value;
} BoolValue;

typedef struct {
    Code base;
    Dictionary* value;
    Dictionary* funcs;
} Object;

typedef struct {
    Exp exp;
    char* id;
} Identifier;

struct Reference {
    Code base;
    Identifier* currentChain;
    struct Reference* restOfTheChain;
};

struct If {
    Code base;
    Exp* condition;
    struct List* codes;
    struct List* elseParts;
};

typedef struct {
    Code base;
    Exp* condition;
    struct List* codes;
} ElseIf;

typedef struct {
    Code base;
    struct List* codes;
} Else;

typedef struct {
    Code base;
    Exp condition;
    struct List* codes;
} ConditionalLoop;

typedef struct {
    Code base;
    Identifier* counter;
    Exp* limit;
    Exp* step;
    struct List* codes;
} CounterLoop;

typedef struct {
    Code base;
    Code* funcRef;
    Dictionary* entries;
} Call;

typedef struct {
    Code base;
    Identifier* id;
    unsigned long loc;
    char* value;
} Prop;

typedef struct {
    Code base;
    char* funcName;
    struct List* params;
    unsigned long loc;
    char* codes;
} Function;

typedef struct {
    Code base;
    char* className;
    struct List* inheritance;
    struct List* behavior;
    struct List* properties;
    Dictionary* functions;
} Class;

typedef struct {
    char* code;
    unsigned long loc;
    unsigned long pointer;
} CodePack;
