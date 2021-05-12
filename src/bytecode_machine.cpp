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
    static constexpr int COPY_SIZE = std::min( sizeof( float ), sizeof( int_fast16_t ) );

    enum class Instruction
    {
        LITERAL,
        BRK,
        JMP,
        CJMP,
        CNJMP,
        EQUAL,
        NOTEQUAL,
        LESS_THAN,
        GREATER_THAN,
        LESS_THAN_EQUAL,
        GREATER_THAN_EQUAL,
        ADD,
        SUB,
        MUL,
        DIV,
        MAX,
        MIN,
        NEG,
        PRINT_STACK,
        PRINT_STATIC,
        GET_RIGHT_HELD,
        GET_LEFT_HELD,
        GET_PLAYER_X,
        SET_PLAYER_X,
        GET_PLAYER_ACCX,
        SET_PLAYER_ACCX,
        GET_PLAYER_VX,
        SET_PLAYER_VX,
        GET_PLAYER_PROP,
        SET_PLAYER_PROP,
        GET_PLAYER_TOP_SPEED,
        SET_PLAYER_TOP_SPEED,
        UPDATE_PLAYER_GRAPHICS_POSITION,
        GET_PLAYER_GRAPHICS_ROTATION_Y,
        SET_PLAYER_GRAPHICS_ROTATION_Y
    };

    static void copyData( void * to, void * from );
    static void floatMath( std::stack<int_fast16_t> & stack, std::function<float(float, float)> action );
    static void floatToStack( std::stack<int_fast16_t> & stack, float value );
    static float floatFromStack( std::stack<int_fast16_t> & stack );

    void run( const std::vector<int_fast16_t> & bytecode, LevelState & level_state )
    {
        std::stack<int_fast16_t> stack;
        for ( int i = ( int )( bytecode[ 0 ] + 1 ); i < bytecode.size(); ++i )
        {
            switch ( ( Instruction )( bytecode[ i ] ) )
            {
                case ( Instruction::LITERAL ):
                {
                    stack.push( bytecode[ ++i ] );
                }
                break;
                case ( Instruction::BRK ):
                {
                    return;
                }
                break;
                case ( Instruction::JMP ):
                {
                    int_fast16_t jump = stack.top();
                    stack.pop();
                    i = ( int )( jump ) - 1;
                }
                break;
                case ( Instruction::CJMP ):
                {
                    int_fast16_t condition = stack.top();
                    stack.pop();
                    int_fast16_t jump = stack.top();
                    stack.pop();
                    if ( condition )
                    {
                        i = ( int )( jump ) - 1;
                    }
                }
                break;
                case ( Instruction::CNJMP ):
                {
                    int_fast16_t condition = stack.top();
                    stack.pop();
                    int_fast16_t jump = stack.top();
                    stack.pop();
                    if ( !condition )
                    {
                        i = ( int )( jump ) - 1;
                    }
                }
                break;
                case ( Instruction::EQUAL ):
                {
                    floatMath( stack, []( float x, float y ) { return x == y; } );
                }
                break;
                case ( Instruction::NOTEQUAL ):
                {
                    floatMath( stack, []( float x, float y ) { return x != y; } );
                }
                break;
                case ( Instruction::LESS_THAN ):
                {
                    floatMath( stack, []( float x, float y ) { return x < y; } );
                }
                break;
                case ( Instruction::GREATER_THAN ):
                {
                    floatMath( stack, []( float x, float y ) { return x > y; } );
                }
                break;
                case ( Instruction::LESS_THAN_EQUAL ):
                {
                    floatMath( stack, []( float x, float y ) { return x <= y; } );
                }
                break;
                case ( Instruction::GREATER_THAN_EQUAL ):
                {
                    floatMath( stack, []( float x, float y ) { return x >= y; } );
                }
                break;
                case ( Instruction::ADD ):
                {
                    floatMath( stack, []( float x, float y ) { return x + y; } );
                }
                break;
                case ( Instruction::SUB ):
                {
                    floatMath( stack, []( float x, float y ) { return x - y; } );
                }
                break;
                case ( Instruction::MUL ):
                {
                    floatMath( stack, []( float x, float y ) { return x * y; } );
                }
                break;
                case ( Instruction::DIV ):
                {
                    floatMath( stack, []( float x, float y ) { return x / y; } );
                }
                break;
                case ( Instruction::MAX ):
                {
                    floatMath( stack, []( float x, float y ) { return std::max( x, y ); } );
                }
                break;
                case ( Instruction::MIN ):
                {
                    floatMath( stack, []( float x, float y ) { return std::min( x, y ); } );
                }
                break;
                case ( Instruction::NEG ):
                {
                    float x;
                    copyData( &x, &stack.top() );
                    stack.pop();
                    float z = -x;
                    int_fast16_t zint;
                    copyData( &zint, &z );
                    stack.push( zint );
                }
                break;
                case ( Instruction::PRINT_STACK ):
                {
                    std::string text;
                    while ( stack.top() != '\0' )
                    {
                        text += ( char )( stack.top() );
                        stack.pop();
                    }
                    printf( "%s\n", text.c_str() );
                }
                break;
                case ( Instruction::PRINT_STATIC ):
                {
                    int index = ( int )( stack.top() );
                    stack.pop();
                    std::string text;
                    while ( bytecode[ index ] != '\0' )
                    {
                        text += bytecode[ index++ ];
                    }
                    printf( "%s\n", text.c_str() );
                }
                break;
                case ( Instruction::GET_RIGHT_HELD ):
                {
                    stack.push( ( int_fast16_t )( Input::heldRight() ) );
                }
                break;
                case ( Instruction::GET_LEFT_HELD ):
                {
                    stack.push( ( int_fast16_t )( Input::heldLeft() ) );
                }
                break;
                case ( Instruction::GET_PLAYER_X ):
                {
                    floatToStack( stack, level_state.hero.position.x );
                }
                break;
                case ( Instruction::SET_PLAYER_X ):
                {
                    level_state.hero.position.x = floatFromStack( stack );
                }
                break;
                case ( Instruction::GET_PLAYER_ACCX ):
                {
                    floatToStack( stack, level_state.hero.accx );
                }
                break;
                case ( Instruction::SET_PLAYER_ACCX ):
                {
                    level_state.hero.accx = floatFromStack( stack );
                }
                break;
                case ( Instruction::GET_PLAYER_VX ):
                {
                    floatToStack( stack, level_state.hero.vx );

                }
                break;
                case ( Instruction::SET_PLAYER_VX ):
                {
                    level_state.hero.vx = floatFromStack( stack );
                }
                break;
                case ( Instruction::GET_PLAYER_PROP ):
                {
                    int key_index = ( int )( stack.top() );
                    stack.pop();
                    std::string key;
                    while ( bytecode[ key_index ] != '\0' )
                    {
                        key += ( char )( bytecode[ key_index++ ] );
                    }
                    auto seek = level_state.hero.props->find( key );
                    if ( seek == level_state.hero.props->end() )
                    {
                        throw std::runtime_error( "Could not find property “" + key + "” in player’s properties." );
                    }
                    stack.push( seek->second );
                }
                break;
                case ( Instruction::SET_PLAYER_PROP ):
                {
                    int key_index = ( int )( stack.top() );
                    stack.pop();
                    std::string key;
                    while ( bytecode[ key_index ] != '\0' )
                    {
                        key += ( char )( bytecode[ key_index++ ] );
                    }
                    auto seek = level_state.hero.props->find( key );
                    if ( seek == level_state.hero.props->end() )
                    {
                        throw std::runtime_error( "Could not find property “" + key + "” in player’s properties." );
                    }
                    seek->second = stack.top();
                    stack.pop();
                }
                break;
                case ( Instruction::GET_PLAYER_TOP_SPEED ):
                {
                    floatToStack( stack, level_state.hero.top_speed );

                }
                break;
                case ( Instruction::SET_PLAYER_TOP_SPEED ):
                {
                    level_state.hero.top_speed = floatFromStack( stack );
                }
                break;
                case ( Instruction::UPDATE_PLAYER_GRAPHICS_POSITION ):
                {
                    Graphic & g = Render::getGraphic( level_state.hero.gfx );
                    g.data.sprite.dest = level_state.hero.position;
                }
                break;
                case ( Instruction::GET_PLAYER_GRAPHICS_ROTATION_Y ):
                {
                    Graphic & g = Render::getGraphic( level_state.hero.gfx );
                    floatToStack( stack, g.data.sprite.rotation_y );
                }
                break;
                case ( Instruction::SET_PLAYER_GRAPHICS_ROTATION_Y ):
                {
                    Graphic & g = Render::getGraphic( level_state.hero.gfx );
                    g.data.sprite.rotation_y = floatFromStack( stack );
                }
                break;
                default:
                {
                    throw std::runtime_error( "INVALID INSTRUCTION!" );
                }
            }
        }
    };

    static void copyData( void * to, void * from )
    {
        memcpy( to, from, COPY_SIZE );
    }

    static void floatMath( std::stack<int_fast16_t> & stack, std::function<float(float, float)> action )
    {
        float x;
        float y;
        copyData( &x, &stack.top() );
        stack.pop();
        copyData( &y, &stack.top() );
        stack.pop();
        float z = action( x, y );
        int_fast16_t zint;
        copyData( &zint, &z );
        stack.push( zint );
    };

    static void floatToStack( std::stack<int_fast16_t> & stack, float value )
    {
        int_fast16_t n;
        copyData( &n, &value );
        stack.push( n );
    };

    static float floatFromStack( std::stack<int_fast16_t> & stack )
    {
        float n;
        copyData( &n, &stack.top() );
        stack.pop();
        return n;
    };
}