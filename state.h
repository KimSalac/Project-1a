#ifndef STATE
#define STATE
#include <vector>
#include <cstdint>
#include <iostream>
#include "control.h"
//
// Pipeline registers implementation
//
// TODO:
// IFID Pipeline register, only contains instruction and pc + 1
struct IFID {
};

// TODO:
// IDEX Pipeline register
struct IDEX {
    control_t control;
};

// TODO:
// EXMEM Pipeline register
struct EXMEM {
    control_t control;
};

// TODO:
// MEMWB Pipwline register
struct MEMWB {
    control_t control;
};

struct state_t {
    uint32_t pc;
    IFID ifid;
    IDEX idex;
    EXMEM exmem;
    MEMWB memwb;
};


#endif
