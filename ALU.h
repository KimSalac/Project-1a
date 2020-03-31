#ifndef ALU_CLASS
#define ALU_CLASS
#include <vector>
#include <cstdint>
#include <iostream>
class ALU {
    private:
        int ALU_control_inputs;
    public:
        // TODO:
        // Generate the control inputs for the ALU
        void generate_control_inputs(int ALU_op, int funct, int opcode) {
        
        }
        
        // TODO:
        // execute ALU operations, generate result, and set the zero control signal if necessary
        uint32_t execute(uint32_t operand_1, uint32_t operand_2, uint32_t &ALU_zero) {
        return 0;
        }
            
};
#endif
