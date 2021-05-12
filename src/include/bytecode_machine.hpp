#ifndef BYTECODE_MACHINE_H
#define BYTECODE_MACHINE_H

#include <cstdint>
#include <cstring>
#include <vector>

class LevelState;

namespace BytecodeMachine
{
    void run( const std::vector<int_fast16_t> & bytecode, LevelState & level_state );
}

#endif // BYTECODE_MACHINE_H