#ifndef CONTROL_CLASS
#define CONTROL_CLASS
#include <vector>
#include <cstdint>
#include <iostream>
using namespace std;

// Control signals for the processor
struct control_t {
    bool reg_dest;           // 0 if rt, 1 if rd
    bool jump;               // 1 if jummp Register
    bool branch;             // 1 if branch
    bool mem_read;           // 1 if memory needs to be read
    bool mem_to_reg; // 1 if memory needs to written to reg
    unsigned ALU_op : 2;     // 10 for R-type, 00 for LW/SW, 01 for BEQ/BNE, 11 for others
    bool mem_write;          // 1 if needs to be written to memory
    bool ALU_src;            // 0 if second operand is from reg_file, 1 if imm
    bool reg_write;          // 1 if need to write back to reg file
	unsigned store_reg : 2;				// dealing specifically with sh and asb
	unsigned load_reg: 2; // deals with lbu and lhu
	bool sign_zero;			// sign-extended or zero-extended
	bool beq;
    
    void print() {      // Prints the generated contol signals
        cout << "REG_DEST: " << reg_dest << "\n";
        cout << "JUMP: " << jump << "\n";
        cout << "BRANCH: " << branch << "\n";
        cout << "MEM_READ: " << mem_read << "\n";
        cout << "MEM_TO_REG: " << mem_to_reg << "\n";
        cout << "ALU_OP: " << ALU_op << "\n";
        cout << "MEM_WRITE: " << mem_write << "\n";
        cout << "ALU_SRC: " << ALU_src << "\n";
        cout << "REG_WRITE: " << reg_write << "\n";
		//cout << "STORE_REG: " << store_reg << "\n";
		//cout << "SIGN_ZERO: " << sign_zero << "\n";
		//cout << "BEQ: " << beq << "\n";
    }
    // TODO:
    // Decode instructions into control signals
    void decode(uint32_t instruction) {

      //reset all signals

    reg_dest = 0;
    jump = 0;
    branch = 0;
    mem_read = 0;
    mem_to_reg = 0;
    ALU_op = 0b00;
    mem_write = 0;
    ALU_src = 0;
    reg_write = 0;
	store_reg = 0b00;
	load_reg = 0b00;
	sign_zero = 0;
	beq = 0; //1 if beq, 0 if bne

      bool rType = !(instruction >> 26);
      bool load = ((instruction >> 26) == 0b100100) || ((instruction >> 26) == 0b100101) || ((instruction >> 26) == 0b001111) || ((instruction >> 26) == 0b100011) || ((instruction >> 26) == 0b110000);
      bool store = ((instruction >> 26) == 0b101000) || ((instruction >> 26) == 0b101001) || ((instruction >> 26) == 0b101011);
      bool jumps = ((instruction >> 26) == 0x02) || ((instruction >> 26) == 0x03);
      bool beqne = ((instruction >> 26) == 0b100) || ((instruction >> 26) == 0b101);
      bool iType = !(rType || load || store || jumps || beqne);

    if(rType) // good: sets signals for all r-type instructions
	{
	  if((instruction & 0x1f) == 0x08) // detects exception of jumpregister
	    {
	      reg_dest = 0;
	      jump = 1;
		  reg_write = 0;
		  ALU_op = 0b00;
	    }
	  else
	    {
	      reg_dest = 1;
		  reg_write = 1;
		  ALU_op = 0b10;
	    }
	}

    /*if(jumps) //sets signals for all jump  instructions
	{
	  jump = 1;
	  if((instruction >> 26) == 0x02)
	    {
	      reg_write = 0;
	    }
	  else
	    {
	      reg_write = 1;
	    }
	  ALU_op = 0b11;
	}*/

    if(beqne) //good: sets signals for all branch  instructions
	{
		if((instruction >> 26) == 0b000100) // beq
		{
			beq = 1;
		}
		else // bne
		{
			beq = 0;
		}

	  branch = 1;
	  ALU_op = 0b01;
	}

    if(load) //good: sets signals for all loads  instructions
	{
		mem_to_reg = 1;
		if((instruction >> 26) == 0b100100) // lbu
		{
			load_reg = 0b10;
		}
		if((instruction >> 26) == 0b100101) // lhu
		{
			load_reg = 0b11;
		}
		if((instruction >> 26) == 0b001111) // lui
		{
			mem_to_reg = 0;
		}
		if((instruction >> 26) == 0b100011) // lw
		{
			load_reg = 0b00;
		}

	  mem_read = 1;
	  ALU_src = 1;
	  reg_write = 1;
	  ALU_op = 0b00;
	}

    if(store) //sets signals for all store  instructions
	{
	  mem_write = 1;
	  ALU_src = 1;
	  ALU_op = 0b00;
	  if((instruction >> 26) == 0b101011) //sw
	    {
	      store_reg = 0b10;
	    }
	  	else if((instruction >> 26) == 0b101000) // sb
	    {
	      store_reg = 0b01;
		}
		else if((instruction >> 26) == 0b101001) // sh
		{
		  store_reg = 0b00;
		}
	}

      if(iType)
	{
	  ALU_op = 0b11;
	  ALU_src = 1;
	  reg_write = 1;
	  if(((instruction >> 26) == 0b001100) || ((instruction >> 26) == 0b001101)) // andi or ori
	  {
		  sign_zero = 1;
	  }
	}
    }	
};




#endif
