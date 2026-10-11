//VMStatus
enum VMStatus {
    VM_READY,
    VM_RUNNING,
    VM_HALTED,
    VM_FAULT
};
//VMError
enum VMError {
    FILE_ERR,
    SYNTAX_ERR,
    OPCODE_ERR,
    REG_ERR,
    IMM_ERR,
    BOUND_ERR,
    JUMP_ERR,
    PC_ERR,
    LIMIT_ERR
};
//VM
struct vm {
    uint32_t regs[4];
    uint8_t memory[256];
    size_t pc;
    bool zf;
    enum VMStatus status;
    enum VMError error;
};

