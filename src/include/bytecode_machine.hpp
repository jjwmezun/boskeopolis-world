#ifndef BYTECODE_MACHINE_H
#define BYTECODE_MACHINE_H

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

class LevelState;

namespace BytecodeMachine
{
    struct Bytecode
    {
        std::vector<std::string> constants;
        std::vector<int_fast16_t> instructions;
    };
    void run( const Bytecode & bytecode, LevelState & level_state );
}

#endif // BYTECODE_MACHINE_H