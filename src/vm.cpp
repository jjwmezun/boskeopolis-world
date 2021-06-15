#include "config.hpp"
#include <cstdio>
#include "vm.hpp"

#define VM_CONSTANTS_INIT_CAPACITY 64
#define VM_INSTRUCTIONS_INIT_CAPACITY 64
#define VM_STACK_INIT_CAPACITY 32

#define BINARY_OP( op ) \
    do\
    {\
        VMValue b = vm_stack_pop( &vm->stack );\
        VMValue a = vm_stack_pop( &vm->stack );\
        if ( a.type != b.type )\
        {\
        }\
        switch ( a.type )\
        {\
            case ( VMT_FLOAT ):\
            {\
                a.value.float_ op b.value.float_;\
            }\
            break;\
            case ( VMT_INT ):\
            {\
                a.value.int_ op b.value.int_;\
            }\
            break;\
            case ( VMT_BOOL ):\
            {\
            }\
            break;\
            case ( VMT_INSTRUCTION ):\
            {\
            }\
            break;\
        }\
        vm_stack_push( &vm->stack, a );\
    } while ( false )

static int vm_code_disassemble_instruction( VMCode * vm_code, VMValueList * stack, int offset );
static int vm_code_disassemble_instruction_simple( const char* name, int offset );
static int vm_code_disassemble_instruction_const( const char * name, VMCode * vm_code, int offset );
static void vm_print_value( VMValue value );

static void vm_stack_push( VMValueList * stack, VMValue value );
static VMValue vm_stack_pop( VMValueList * stack );

void vm_init( VM * vm )
{
    vm->code.constants.capacity = VM_CONSTANTS_INIT_CAPACITY;
    vm->code.instructions.capacity = VM_INSTRUCTIONS_INIT_CAPACITY;
    vm->stack.capacity = VM_STACK_INIT_CAPACITY;
    vm->stack.size = vm->code.constants.size = vm->code.instructions.size = 0;
    vm->code.constants.list = ( VMValue * )( calloc( VM_CONSTANTS_INIT_CAPACITY, sizeof( VMValue ) ) );
    vm->code.instructions.list = ( VMInstruction * )( calloc( VM_INSTRUCTIONS_INIT_CAPACITY, sizeof( VMInstruction ) ) );
    vm->stack.list = ( VMValue * )( calloc( VM_STACK_INIT_CAPACITY, sizeof( VMValue ) ) );
    vm->code.line_nums = ( int * )( calloc( VM_INSTRUCTIONS_INIT_CAPACITY, sizeof( int ) ) );
};

void vm_close( VM * vm )
{
    free( vm->code.constants.list );
    free( vm->code.instructions.list );
    free( vm->stack.list );
    free( vm->code.line_nums );
};

InterpretResult vm_code_interpret( VM * vm )
{
    vm->ip = vm->code.instructions.list;
    while ( true )
    {
        #ifdef DEBUG
            printf( "          " );
            for ( int i = 0; i < vm->stack.size; ++i )
            {
                printf( "[ " );
                vm_print_value( vm->stack.list[ i ] );
                printf( " ]" );
            }
            printf( "\n" );
            vm_code_disassemble_instruction( &vm->code, &vm->stack, ( int )( vm->ip - vm->code.instructions.list ) );
        #endif
        VMInstruction instruction;
        switch ( instruction = *vm->ip++ )
        {
            case OP_CONST:
            {
                vm_stack_push( &vm->stack, vm->code.constants.list[ *vm->ip++ ] );
            }
            break;
            case OP_NEG:
            {
                switch ( vm->stack.list[ vm->stack.size - 1 ].type )
                {
                    case ( VMT_FLOAT ):
                    {
                        vm->stack.list[ vm->stack.size - 1 ].value.float_ *= -1.0f;
                    }
                    break;
                    case ( VMT_INT ):
                    {
                        vm->stack.list[ vm->stack.size - 1 ].value.int_ *= -1;
                    }
                    break;
                    case ( VMT_BOOL ):
                    {
                        vm->stack.list[ vm->stack.size - 1 ].value.bool_ = !vm->stack.list[ vm->stack.size - 1 ].value.bool_;
                    }
                    break;
                    case ( VMT_INSTRUCTION ):
                    {
                        // ERROR
                    }
                    break;
                }
            }
            break;
            case OP_ADD:
            {
                BINARY_OP( += );
            }
            break;
            case OP_SUB:
            {
                BINARY_OP( -= );
            }
            break;
            case OP_MUL:
            {
                BINARY_OP( *= );
            }
            break;
            case OP_DIV:
            {
                BINARY_OP( /= );
            }
            break;
            case OP_RETURN:
            {
                vm_print_value( vm_stack_pop( &vm->stack ) );
                return INTERPRET_OK;
            }
        }
    }
};

void vm_code_push_instruction( VMCode * vm_code, VMInstruction value, int line_num )
{
    if ( vm_code->instructions.capacity < vm_code->instructions.size + 1 )
    {
        int old_capacity = vm_code->instructions.capacity;
        vm_code->instructions.capacity *= 2;
        VMInstruction * new_list = ( VMInstruction * )( calloc( vm_code->instructions.capacity, sizeof( VMInstruction ) ) );
        memcpy( new_list, vm_code->instructions.list, sizeof( VMInstruction ) * old_capacity );
        free( vm_code->instructions.list );
        vm_code->instructions.list = new_list;

        int * new_line_nums = ( int * )( calloc( vm_code->instructions.capacity, sizeof( int ) ) );
        memcpy( new_line_nums, vm_code->line_nums, sizeof( int ) * old_capacity );
        free( vm_code->line_nums );
        vm_code->line_nums = new_line_nums;
    }
    vm_code->instructions.list[ vm_code->instructions.size ] = value;
    vm_code->line_nums[ vm_code->instructions.size ] = line_num;
    ++vm_code->instructions.size;
};

VMInstruction vm_code_push_constant( VMCode * vm_code, VMValue value )
{
    if ( vm_code->constants.capacity < vm_code->constants.size + 1 )
    {
        int old_capacity = vm_code->constants.capacity;
        vm_code->constants.capacity *= 2;
        VMValue * new_list = ( VMValue * )( calloc( vm_code->constants.capacity, sizeof( VMValue ) ) );
        memcpy( new_list, vm_code->constants.list, sizeof( VMValue ) * old_capacity );
        free( vm_code->constants.list );
        vm_code->constants.list = new_list;
    }
    vm_code->constants.list[ vm_code->constants.size ] = value;
    return ( VMInstruction )( vm_code->constants.size++ );
};

VMValue vm_create_float( float value )
{
    VMValue v;
    v.type = VMT_FLOAT;
    v.value.float_ = value;
    return v;
};

static int vm_code_disassemble_instruction( VMCode * vm_code, VMValueList * stack, int offset )
{
    printf( "%04d ", offset );
    if ( offset > 0 && vm_code->line_nums[ offset ] == vm_code->line_nums[ offset - 1 ] )
    {
        printf( "   | " );
    }
    else
    {
        printf( "%4d ", vm_code->line_nums[ offset ] );
    }
    VMInstruction instruction = vm_code->instructions.list[ offset ];
    switch ( instruction )
    {
        case OP_RETURN:
            return vm_code_disassemble_instruction_simple( "OP_RETURN", offset );
        case OP_CONST:
            return vm_code_disassemble_instruction_const( "OP_CONST", vm_code, offset );
        case OP_NEG:
            return vm_code_disassemble_instruction_simple( "OP_NEG", offset );
        case OP_ADD:
            return vm_code_disassemble_instruction_simple( "OP_ADD", offset );
        case OP_SUB:
            return vm_code_disassemble_instruction_simple( "OP_SUB", offset );
        case OP_MUL:
            return vm_code_disassemble_instruction_simple( "OP_MUL", offset );
        case OP_DIV:
            return vm_code_disassemble_instruction_simple( "OP_DIV", offset );
        default:
            printf( "Unknown opcode %ld\n", instruction );
            return offset + 1;
    }
};

static int vm_code_disassemble_instruction_simple( const char* name, int offset )
{
    printf( "%s\n", name );
    return offset + 1;
};

static int vm_code_disassemble_instruction_const( const char * name, VMCode * vm_code, int offset )
{
    int constant = ( int )( vm_code->instructions.list[ offset + 1 ] );
    printf( "%-16s %4d “", name, constant );
    vm_print_value( vm_code->constants.list[ constant ] );
    printf( "”\n" );
    return offset + 2;
};

static void vm_print_value( VMValue value )
{
    switch ( value.type )
    {
        case ( VMT_FLOAT ):
        {
            printf( "%f", value.value.float_ );
        }
        break;
    }
};

static void vm_stack_push( VMValueList * stack, VMValue value )
{
    if ( stack->capacity < stack->size + 1 )
    {
        int old_capacity = stack->capacity;
        stack->capacity *= 2;
        VMValue * new_list = ( VMValue * )( calloc( stack->capacity, sizeof( VMValue ) ) );
        memcpy( new_list, stack->list, sizeof( VMValue ) * old_capacity );
        free( stack->list );
        stack->list = new_list;
    }
    stack->list[ stack->size++ ] = value;
};

static VMValue vm_stack_pop( VMValueList * stack )
{
    return stack->list[ --stack->size ];
};