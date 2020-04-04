#ifndef ALU_CLASS
#define ALU_CLASS
#include <vector>
#include <cstdint>
#include <iostream>
using namespace std;
class ALU {
    private:
        int ALU_control_inputs;
    public:
        // TODO:
        // Generate the control inputs for the ALU
        void generate_control_inputs(int ALU_op, int funct, int opcode) {        
            if (opcode == 0){ // if rtype
            //std::cout<<"funct: "<<std::bitset<32>funct<<std::endl;
                if (funct ==  0b100000 || funct == 0b100001){ //rtype Add
                    ALU_control_inputs = 2;//0010;
                    //cout<< "rtype add" << endl;
                }
                else if (funct ==  0b100010 || funct == 0b100011){ //rtype sub
                    ALU_control_inputs = 6;//0110;
                    //cout<< "rtype sub" << endl;
                }
                else if (funct ==  0b100100)
                { //rtype and
                    ALU_control_inputs = 0000;
                   // cout<< "rtype add" << endl;
                }else if (funct ==  0b100101){
                     //rtype or
                    ALU_control_inputs = 0001;
                  //  cout<< "rtype add" << endl;
                }
                else if (funct ==  0b101010)
                { //rtype set on less than
                    ALU_control_inputs = 7;//0111;
                    //cout<< "rtype add" << endl;
                }
                else if(funct == 0b010) //srl
                {
                    ALU_control_inputs = 12;
                }
                else if (funct == 0x27) // nor
                {
                    ALU_control_inputs = 3;
                }
                else if (funct == 0b00) // sll
                {
                    ALU_control_inputs = 4;
                }
                else if (funct ==  39){ //rtype nor
                    ALU_control_inputs = 3;
                }
            }
            else{ //I type and j type
            //std::cout<<"opcode: "<<opcode<<std::endl;
                if (ALU_op == 00){ //for loads and stores
                    ALU_control_inputs = 2;//0010;
                    //cout<< "load/store" << endl;
                }
                else if(ALU_op == 01){//beq and bne
                    ALU_control_inputs = 6;//0110;
                    //cout<< "rtype add" << endl;
                }
                /*Below are all misc I-types*/
                else if(opcode == 12){//itype andi
                    ALU_control_inputs = 0000;
                    //cout<< "itype and" << endl;
                }
                else if(opcode == 8 || opcode == 9){//itype addi and addiu
                    ALU_control_inputs = 2;
                    //std::cout<<"itype add"<<std::endl;
                }
                else if(opcode == 13){//itype ori
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
            uint32_t output = 0;
            if(ALU_control_inputs == 2){ //add op
            //std::cout<<"o: "<<(int32_t) operand_1<<std::endl;
            //std::cout<<"o: "<<(int32_t) operand_2<<std::endl;
               return output =  operand_1 + operand_2;
            }
            else if(ALU_control_inputs == 6){ //sub op
                output =  operand_1 - operand_2;
                if(output == 0)
                {
                    ALU_zero = 1;
                }
                else
                {
                    ALU_zero = 0;
                }
                return output;
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
            else if(ALU_control_inputs == 12){ // srl
                return operand_2 >> (int) operand_1;
            }
            else if(ALU_control_inputs == 4){ // sll
                return operand_1 << (int) operand_2;
            }
            else if(ALU_control_inputs == 3){ // nor
                uint32_t r = (operand_1) | (operand_2);
                uint32_t n = ~r;
                return output = (n);
            }
        return 0;
        }
        
        /*void print(){ //print method to make sure the control singals are correct
            std::cout<<"alu control: " << ALU_control_inputs << std::endl;
        }*/
            
};
#endif
