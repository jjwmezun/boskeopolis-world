#ifndef VM_H
#define VM_H

#include <cinttypes>
#include <cstdlib>
#include <cstring>

typedef int_fast16_t VMInstruction;

typedef enum
{
    OP_CONST,
    OP_RETURN,
    OP_NEG,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV
} VMOpCode;

typedef enum
{
    VMT_FLOAT,
    VMT_INT,
    VMT_BOOL,
    VMT_INSTRUCTION
} VMValueType;

typedef struct
{
    VMValueType type;
    union
    {
        float float_;
        int int_;
        bool bool_;
        VMInstruction instruction_;
    } value;
} VMValue;

typedef struct { int capacity; int size; VMInstruction * list; } VMInstructionList;
typedef struct { int capacity; int size; VMValue * list; } VMValueList;
typedef struct { VMValueList constants; VMInstructionList instructions; int * line_nums; } VMCode;

typedef struct
{
    VMInstruction * ip;
    VMCode code;
    VMValueList stack;
} VM;

typedef enum
{
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

void vm_init( VM * vm );
void vm_close( VM * vm );
void vm_scan( VM * vm, const char * source );
InterpretResult vm_code_interpret( VM * vm );

#endif // VM_H