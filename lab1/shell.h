/***************************************************************/
/*                                                             */
/*   MIPS-32 Instruction Level Simulator                       */
/*                                                             */
/*   ECE 447                                                   */
/*   Carnegie Mellon University                                */
/*                                                             */
/***************************************************************/

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/*          DO NOT MODIFY THIS FILE!                            */
/*          You should only change sim.c!                       */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

#ifndef _SIM_SHELL_H_
#define _SIM_SHELL_H_

#include <stdint.h>

#define FALSE 0
#define TRUE  1

#define MIPS_REGS 32

//Enum table for different opcode as per R4000 MIPS manual
enum{
 OP_J = 2,
 OP_JAL = 3,
 OP_BEQ = 4,
 OP_BNE = 5,
 OP_BLEZ = 6,
 OP_BGTZ = 7,
 OP_ADDI = 8,
 OP_ADDIU = 9,
 OP_SLTI = 10,
 OP_SLTIU = 11,
 OP_ANDI = 12,
 OP_ORI = 13,
 OP_XORI = 14,
 OP_LUI = 15,
 OP_LB = 32,
 OP_LH = 33,
 OP_LW = 35,
 OP_LBU = 36,
 OP_LHU = 37,
 OP_SB = 40,
 OP_SH = 41,
 OP_SW = 43,
 OP_REGIMM = 1,
 OP_BLTZ = 0,
 OP_BGEZ = 1,
 OP_BLTZAL = 16,
 OP_BGEZAL = 17,
 OP_SPECIAL = 0,
 OP_SLL = 0,
 OP_SRL = 2,
 OP_SRA = 3,
 OP_SLLV = 4,
 OP_SRLV = 6,
 OP_SRAV = 7,
 OP_JR = 8,
 OP_JALR = 9,
 OP_ADD = 32,
 OP_ADDU = 33,
 OP_SUB = 34,
 OP_SUBU = 35,
 OP_AND = 36,
 OP_OR = 37,
 OP_XOR = 38,
 OP_NOR = 39,
 OP_SLT = 42,
 OP_SLTU = 43,
 OP_MULT = 24,
 OP_MFHI = 16,
 OP_MFLO = 18,
 OP_MTHI = 17,
 OP_MTLO = 19,
 OP_MULTU = 25,
 OP_DIV = 26,
 OP_DIVU = 27,
 OP_SYSCALL = 12,
	
}opcode;

typedef struct CPU_State_Struct {

  uint32_t PC;		/* program counter */
  uint32_t REGS[MIPS_REGS]; /* register file. */
  uint32_t HI, LO;          /* special regs for mult/div. */
} CPU_State;

/* Data Structure for Latch */

extern CPU_State CURRENT_STATE, NEXT_STATE;

extern int RUN_BIT;	/* run bit */

uint32_t mem_read_32(uint32_t address);
void     mem_write_32(uint32_t address, uint32_t value);
void     mem_write_16(uint32_t address, uint16_t value);
void     mem_write_8(uint32_t address, uint8_t value);
/* YOU IMPLEMENT THIS FUNCTION */
void process_instruction();

#endif
