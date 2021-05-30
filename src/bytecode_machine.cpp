#include "bytecode_machine.hpp"

#include <functional>
#include "game_state.hpp"
#include "graphic.hpp"
#include "input.hpp"
#include "render.hpp"
#include <stack>
#include <stdexcept>

namespace BytecodeMachine
{
    static constexpr int COPY_SIZE = std::min( sizeof( float ), sizeof( VMInstruction ) );

    static VMValue stack_pop( std::stack<VMValue> & stack );
    static void floatMath( std::stack<VMValue> & stack, const VMInstruction * ip, const Bytecode & bytecode, std::function<float(float, float)> action );

    void run( const Bytecode & bytecode, LevelState & level_state )
    {
        std::stack<VMValue> stack;
        const VMInstruction * ip = &bytecode.instructions.list[ 0 ];
        printf( "%ld\n", bytecode.instructions.size );
        while ( ip != &vector_last( bytecode.instructions ) )
        {
            switch ( *ip++ )
            {
                case ( VM_LITERAL ):
                {
                    stack.push( vm_create_instruction( *ip++ ) );
                }
                break;
                case ( VM_CONST ):
                {
                    stack.push( bytecode.constants.list[ *ip++ ] );
                }
                break;
                case ( VM_BRK ):
                {
                    return;
                }
                break;
                case ( VM_JMP ):
                {
                    VMInstruction jump = stack_pop( stack ).value.instruction_;
                    ip = &bytecode.instructions.list[ jump - 1 ];
                }
                break;
                case ( VM_CJMP ):
                {
                    bool condition = stack_pop( stack ).value.bool_;
                    VMInstruction jump = stack_pop( stack ).value.instruction_;
                    if ( condition )
                    {
                        ip = &bytecode.instructions.list[ jump - 1 ];
                        printf( "JUMP TO #%ld\n", bytecode.instructions.list[ jump - 1 ] );
                    }
                }
                break;
                case ( VM_CNJMP ):
                {
                    bool condition = stack_pop( stack ).value.bool_;
                    VMInstruction jump = stack_pop( stack ).value.instruction_;
                    if ( !condition )
                    {
                        ip = &bytecode.instructions.list[ jump - 1 ];
                        printf( "JUMP TO #%ld\n", bytecode.instructions.list[ jump - 1 ] );
                    }
                }
                break;
                case ( VM_EQUAL ):
                {
                    floatMath( stack, ip, bytecode, []( float x, float y ) { return x == y; } );
                }
                break;
                case ( VM_NOTEQUAL ):
                {
                    floatMath( stack, ip, bytecode, []( float x, float y ) { return x != y; } );
                }
                break;
                case ( VM_LESS_THAN ):
                {
                    floatMath( stack, ip, bytecode, []( float x, float y ) { return x < y; } );
                }
                break;
                case ( VM_GREATER_THAN ):
                {
                    floatMath( stack, ip, bytecode, []( float x, float y ) { return x > y; } );
                }
                break;
                case ( VM_LESS_THAN_EQUAL ):
                {
                    floatMath( stack, ip, bytecode, []( float x, float y ) { return x <= y; } );
                }
                break;
                case ( VM_GREATER_THAN_EQUAL ):
                {
                    floatMath( stack, ip, bytecode, []( float x, float y ) { return x >= y; } );
                }
                break;
                case ( VM_ADD ):
                {
                    floatMath( stack, ip, bytecode, []( float x, float y ) { return x + y; } );
                }
                break;
                case ( VM_SUB ):
                {
                    floatMath( stack, ip, bytecode, []( float x, float y ) { return x - y; } );
                }
                break;
                case ( VM_MUL ):
                {
                    floatMath( stack, ip, bytecode, []( float x, float y ) { return x * y; } );
                }
                break;
                case ( VM_DIV ):
                {
                    floatMath( stack, ip, bytecode, []( float x, float y ) { return x / y; } );
                }
                break;
                case ( VM_MAX ):
                {
                    floatMath( stack, ip, bytecode, []( float x, float y ) { return std::max( x, y ); } );
                }
                break;
                case ( VM_MIN ):
                {
                    floatMath( stack, ip, bytecode, []( float x, float y ) { return std::min( x, y ); } );
                }
                break;
                case ( VM_NEG ):
                {
                    const VMValue v = stack_pop( stack );
                    switch ( v.type )
                    {
                        case ( VT_INT ):
                        {
                            stack.push( vm_create_int( -v.value.int_ ) );
                        }
                        break;
                        case ( VT_FLOAT ):
                        {
                            stack.push( vm_create_float( -v.value.float_ ) );
                        }
                        break;
                        default:
                        {
                            throw std::runtime_error( "Invalid value for NEG" );
                        }
                    }
                }
                break;
                case ( VM_GET_RIGHT_HELD ):
                {
                    stack.push( vm_create_bool( Input::heldRight() ) );
                }
                break;
                case ( VM_GET_LEFT_HELD ):
                {
                    stack.push( vm_create_bool( Input::heldLeft() ) );
                }
                break;
                case ( VM_GET_PLAYER_X ):
                {
                    stack.push( vm_create_float( level_state.hero.position.x ) );
                }
                break;
                case ( VM_SET_PLAYER_X ):
                {
                    level_state.hero.position.x = stack_pop( stack ).value.float_;
                }
                break;
                case ( VM_GET_PLAYER_ACCX ):
                {
                    stack.push( vm_create_float( level_state.hero.accx ) );
                }
                break;
                case ( VM_SET_PLAYER_ACCX ):
                {
                    level_state.hero.accx = stack_pop( stack ).value.float_;
                }
                break;
                case ( VM_GET_PLAYER_VX ):
                {
                    stack.push( vm_create_float( level_state.hero.vx ) );

                }
                break;
                case ( VM_SET_PLAYER_VX ):
                {
                    level_state.hero.vx = stack_pop( stack ).value.float_;
                }
                break;
                case ( VM_GET_PLAYER_PROP ):
                {
                    stack.push( level_state.hero.props[ *ip++ ] );
                }
                break;
                case ( VM_SET_PLAYER_PROP ):
                {
                    level_state.hero.props[ *ip++ ] = stack_pop( stack );
                }
                break;
                case ( VM_GET_PLAYER_TOP_SPEED ):
                {
                    stack.push( vm_create_float( level_state.hero.top_speed ) );
                }
                break;
                case ( VM_SET_PLAYER_TOP_SPEED ):
                {
                    level_state.hero.top_speed = stack_pop( stack ).value.float_;
                }
                break;
                case ( VM_UPDATE_PLAYER_GRAPHICS_POSITION ):
                {
                    Graphic & g = Render::getGraphic( level_state.hero.gfx );
                    g.data.sprite.dest = level_state.hero.position;
                }
                break;
                case ( VM_GET_PLAYER_GRAPHICS_ROTATION_Y ):
                {
                    Graphic & g = Render::getGraphic( level_state.hero.gfx );
                    stack.push( vm_create_float( g.data.sprite.rotation_y ) );
                }
                break;
                case ( VM_SET_PLAYER_GRAPHICS_ROTATION_Y ):
                {
                    Graphic & g = Render::getGraphic( level_state.hero.gfx );
                    g.data.sprite.rotation_y = stack_pop( stack ).value.float_;
                }
                break;
                default:
                {
                    printf( "INVALID INSTRUCTION #%ld @ %ld\n", *( ip - 1 ), ( int )( ip - bytecode.instructions.list ) );
                }
            }
        }
    };

    static VMValue stack_pop( std::stack<VMValue> & stack )
    {
        VMValue value = stack.top();
        stack.pop();
        return value;
    };

    static void floatMath( std::stack<VMValue> & stack, const VMInstruction * ip, const Bytecode & bytecode, std::function<float(float, float)> action )
    {
        VMValue x = stack_pop( stack );
        VMValue y = stack_pop( stack );
        float vx = vm_convert_to_float( x );
        float vy = vm_convert_to_float( y );
        stack.push( vm_create_float( action( vx, vy ) ) );
    };
}

VMValue vm_create_float( float value )
{
    VMValue v;
    v.type = VT_FLOAT;
    v.value.float_ = value;
    return v;
};

VMValue vm_create_int( int value )
{
    VMValue v;
    v.type = VT_INT;
    v.value.int_ = value;
    return v;
};

VMValue vm_create_instruction( VMInstruction value )
{
    VMValue v;
    v.type = VT_INSTRUCTION;
    v.value.instruction_ = value;
    return v;
};

VMValue vm_create_bool( bool value )
{
    VMValue v;
    v.type = VT_BOOL;
    v.value.bool_ = value;
    return v;
};

float vm_convert_to_float( VMValue value )
{
    switch ( value.type )
    {
        case ( VT_INSTRUCTION ):
        {
            return ( float )( value.value.instruction_ );
        }
        break;
        case ( VT_INT ):
        {
            return ( float )( value.value.int_ );
        }
        break;
        case ( VT_FLOAT ):
        {
            return value.value.float_;
        }
        break;
        case ( VT_BOOL ):
        {
            return ( float )( value.value.bool_ );
        }
        break;
    }
};

Bytecode bytecode_create( std::vector<VMValue> constants, std::vector<VMInstruction> instructions )
{
    Bytecode bytecode;
    vector_init( bytecode.constants, VMValue, constants.size() );
    vector_init( bytecode.instructions, VMInstruction, instructions.size() );
    bytecode.constants.size = bytecode.constants.capacity;
    bytecode.instructions.size = bytecode.instructions.capacity;
    for ( int i = 0; i < constants.size(); ++i )
    {
        bytecode.constants.list[ i ] = constants[ i ];
    }
    for ( int i = 0; i < instructions.size(); ++i )
    {
        bytecode.instructions.list[ i ] = instructions[ i ];
    }
    return bytecode;
};