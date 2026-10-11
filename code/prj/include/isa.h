//Opcode
enum Opcode {
    OPC_MOV,
    OPC_ADD,
    OPC_SUB,
    OPC_LOAD,
    OPC_STORE,
    OPC_JMP,
    OPC_JZ, 
    OPC_HLT
};
//OperandType
enum OperandType {
    OPERAND_NONE,
    OPERAND_REG,
    OPERAND_IMM,
    OPERAND_MEM
};
//Operand
struct Operand {
    enum OperandType type;
    uint32_t value;
};
//Instruction
struct Instruction {
    enum Opcode opcode;
    struct Operand dst;
    struct Operand src;
};