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
            if (opcode == 0){ // if rtype
            //std::cout<<"funct: "<<std::bitset<32>funct<<std::endl;
                if (funct ==  0b100000){ //rtype Add
                    ALU_control_inputs = 2;//0010;
                }
                else if (funct ==  0b100010){ //rtype sub
                    ALU_control_inputs = 6;//0110;
                }
                else if (funct ==  0b100100){ //rtype and
                    ALU_control_inputs = 0000;
                }else if (funct ==  0b100101){ //rtype or
                    ALU_control_inputs = 0001;
                }
                else if (funct ==  0b101010){ //rtype set on less than
                    ALU_control_inputs = 7;//0111;
                }
            }
            else{ //I type and j type
            std::cout<<"opcode: "<<opcode<<std::endl;
                if (ALU_op == 00){ //for lw and sw
                    ALU_control_inputs = 2;//0010;
                }
                else if (ALU_op == 01){//beq
                    ALU_control_inputs = 6;//0110;
                }
                //else { //itype add, sub, and, or, set less than
                     //ALU_control_inputs = ALU_op;
                //}
                else if(opcode == 12){//itype and
                    ALU_control_inputs = 0000;
                }
                else if(opcode == 8){//itype add
                    ALU_control_inputs = 2;
                    std::cout<<"itype add"<<std::endl;
                }
                else if(opcode == 13){//itype or
                    ALU_control_inputs = 1;
                }
                else{ //itype set less than
                    ALU_control_inputs = 7;
                }
            }
        }
        
        // TODO:
        // execute ALU operations, generate result, and set the zero control signal if necessary
        uint32_t execute(uint32_t operand_1, uint32_t operand_2, uint32_t &ALU_zero) { //havent set up alu_zero
            uint32_t output;
            if(ALU_control_inputs == 2){ //add op
            //std::cout<<"o: "<<(int32_t) operand_1<<std::endl;
            //std::cout<<"o: "<<(int32_t) operand_2<<std::endl;
               return output =  operand_1 + operand_2;
            }
            else if(ALU_control_inputs == 6){ //sub op
                return output =  operand_1 - operand_2;
            }
            else if(ALU_control_inputs == 0){ //and op
                return output =  operand_1 & operand_2;
            }
            else if(ALU_control_inputs == 1){ //or op
                return output =  operand_1 | operand_2;
            }
            else if(ALU_control_inputs == 7){ //set less than op
                //if a is less than b, output 1 else output 0
                int32_t n =  (int32_t) operand_1 - (int32_t) operand_2; 
                if (n < 0){
                    return 1;
                }
                else{
                    return 0;
                }
            }
        return 0;
        }
        
        void print(){ //print method to make sure the control singals are correct
            std::cout<<"alu control: "<<ALU_control_inputs<<std::endl;
        }
            
};
#endif
