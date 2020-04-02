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
            if (opcode = 000000){ // if rtype
                if (funct == 100000){ //rtype Add
                    ALU_control_inputs = 0010;
                }
                else if (funct == 100010){ //rtype sub
                    ALU_control_inputs = 0110;
                }
                else if (funct == 100100){ //rtype and
                    ALU_control_inputs = 0000;
                }else if (funct == 100101){ //rtype or
                    ALU_control_inputs = 0001;
                }
                else if (funct == 101010){ //rtype set on less than
                    ALU_control_inputs = 0111;
                }
            }
            else{ //I type and j type
                if (ALU_op == 00){ //for lw and sw
                    ALU_control_inputs = 0010;
                }
                else if (ALU_op == 01){//beq
                    ALU_control_inputs = 0110;
                }
                else { //itype add, sub, and, or, set less than
                     ALU_control_inputs = ALU_op;
                }

            }
        }
        
        // TODO:
        // execute ALU operations, generate result, and set the zero control signal if necessary
        uint32_t execute(uint32_t operand_1, uint32_t operand_2, uint32_t &ALU_zero) {
        return 0;
        }
            
};
#endif
