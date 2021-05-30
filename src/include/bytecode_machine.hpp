#ifndef BYTECODE_MACHINE_H
#define BYTECODE_MACHINE_H

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

class LevelState;

enum VMInstructionType
{
    VM_LITERAL,
    VM_CONST,
    VM_BRK,
    VM_JMP,
    VM_CJMP,
    VM_CNJMP,
    VM_EQUAL,
    VM_NOTEQUAL,
    VM_LESS_THAN,
    VM_GREATER_THAN,
    VM_LESS_THAN_EQUAL,
    VM_GREATER_THAN_EQUAL,
    VM_ADD,
    VM_SUB,
    VM_MUL,
    VM_DIV,
    VM_MAX,
    VM_MIN,
    VM_NEG,
    VM_PRINT_STACK,
    VM_PRINT_STATIC,
    VM_GET_RIGHT_HELD,
    VM_GET_LEFT_HELD,
    VM_GET_PLAYER_X,
    VM_SET_PLAYER_X,
    VM_GET_PLAYER_ACCX,
    VM_SET_PLAYER_ACCX,
    VM_GET_PLAYER_VX,
    VM_SET_PLAYER_VX,
    VM_GET_PLAYER_PROP,
    VM_SET_PLAYER_PROP,
    VM_GET_PLAYER_TOP_SPEED,
    VM_SET_PLAYER_TOP_SPEED,
    VM_UPDATE_PLAYER_GRAPHICS_POSITION,
    VM_GET_PLAYER_GRAPHICS_ROTATION_Y,
    VM_SET_PLAYER_GRAPHICS_ROTATION_Y
};

typedef int_fast16_t VMInstruction;

enum VMValueType
{
    VT_FLOAT,
    VT_INT,
    VT_STRING,
    VT_INSTRUCTION,
    VT_BOOL
};

struct VMValue
{
    VMValueType type;
    union
    {
        float float_;
        int int_;
        const char * string_;
        VMInstruction instruction_;
        bool bool_;
    } value;
};

VMValue vm_create_float( float value );
VMValue vm_create_int( int value );
VMValue vm_create_instruction( VMInstruction value );
VMValue vm_create_bool( bool value );
float vm_convert_to_float( VMValue value );

#define vector_type( type ) struct { int capacity; int size; type * list; }
#define vector_last( vector ) vector.list[ vector.size ]

#define vector_init( vector, type, capacity_ ) do { \
    vector.capacity = capacity_;\
    vector.size = 0;\
    vector.list = ( type * )( calloc( capacity_, sizeof( type ) ) );\
} while( 0 )\

struct Bytecode
{
    vector_type( VMValue ) constants;
    vector_type( VMInstruction ) instructions;
};

Bytecode bytecode_create( std::vector<VMValue> constants, std::vector<VMInstruction> instructions );

namespace BytecodeMachine
{
    void run( const Bytecode & bytecode, LevelState & level_state );
}

#endif // BYTECODE_MACHINE_H