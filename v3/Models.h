#include "Dictionary.h"
#include "List.h"

typedef struct
{
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
    void* value;
    char* valueType;
} Value;

typedef struct {
    Exp exp;
    char* id;
    Value value;
} Identifier;

typedef struct {
    char* type;
} Code;

typedef struct {
    Code base;
    Exp condition;
    List codes;
} If;

typedef struct {
    Code base;
    Exp condition;
    List codes;
} ConditionalLoop;

typedef struct {
    Code base;
    Identifier counter;
    Exp limit;
    Exp step;
    List codes;
} CounterLoop;

typedef struct {
    Exp exp;
    void* funcRef;
    Dictionary entries;
} Call;

typedef struct {
    char* funcName;
    List params;
    List codes;
} Function;