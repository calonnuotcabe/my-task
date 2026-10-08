# Baby allocator

### Checksec
```
[*] '/pwn/babyallocator_patched'
    Arch:       amd64-64-little
    RELRO:      Full RELRO
    Stack:      No canary found
    NX:         NX enabled
    PIE:        No PIE (0x3fe000)
    RUNPATH:    b'.'
```

### Rev

#### Main:
```c
void run()
{
  signed __int64 requested_size; // rax
  _BYTE *v1; // rax
  void *v2; // rsp
  _BYTE name_[16]; // [rsp+0h] [rbp-30h] BYREF
  unsigned __int64 active_buffer; // [rsp+10h] [rbp-20h]
  void *ptr; // [rsp+18h] [rbp-18h]
  size_t size; // [rsp+20h] [rbp-10h]
  unsigned __int64 v7; // [rsp+28h] [rbp-8h]

  v7 = 0;
  size = 0;
  ptr = nullptr;
  active_buffer = 0;
  memset(name_, 0, sizeof(name_));
  while ( 1 )
  {
    noti();
    switch ( read_() )
    {
      case 1LL:
        if ( ptr || v7 )
          goto LABEL_12;
        read__("Size :");
        size = (int)read_();
        if ( size <= 0x7F || size > 0xFFF )
        {
          puts("too small or too big !");
          exit(0);
        }
        requested_size = 16 * ((size + 30) / 0x10);
        if ( (unsigned __int64)&name_[-requested_size] >= __readfsqword(0x70u) )   // (if not enough stack) { }
        {
          v2 = alloca(requested_size);
          v1 = name_;
        }
        else
        {
          v1 = (_BYTE *)allocate(requested_size + 15);
        }
        v7 = 16 * ((unsigned __int64)(v1 + 15) >> 4);
        active_buffer = v7;
        read__("What's name of the allocator ? :");
        _isoc99_scanf("%15s", name_);
        break;
      case 2LL:
        if ( v7 || ptr )
        {
LABEL_12:
          puts("You can't alloca a memory space again!");
        }
        else
        {
          read__("Size :");
          size = (int)read_();
          if ( size <= 0xFF )
          {
            puts("too small !");
            exit(0);
          }
          ptr = malloc(size);
          active_buffer = (unsigned __int64)ptr;
          read__("What's name of the allocator ? :");
          _isoc99_scanf("%15s", name_);
        }
        break;
      case 3LL:
        if ( active_buffer )
        {
          read__("Content :");
          sub_400FC6(active_buffer, (unsigned int)size);
        }
        else
        {
          puts("You need to alloca a memory first !");
        }
        break;
      case 4LL:
        run();
        break;
      case 5LL:
        read__("Release !!");
        puts(&nullbyte);
        free(ptr);
        return;
      case 6LL:
        puts("Bye !");
        exit(0);
      default:
        puts("Invalid choice");
        break;
    }
  }
}
```

### rev the path that allocate more stack:

```asm
loc_401325:                             ; CODE XREF: run+C3↑j
.text:0000000000401325                 mov     rax, [rbp+size]   // rax = size
.text:0000000000401329                 lea     rdx, [rax+0Fh]    // rdx = size + 15
.text:000000000040132D                 mov     eax, 10h          // rax = 16
.text:0000000000401332                 sub     rax, 1            // rax = 15
.text:0000000000401336                 add     rax, rdx          // rax = size + 30
.text:0000000000401339                 mov     ecx, 10h          // ecx = 0x10
.text:000000000040133E                 mov     edx, 0            // edx = 0
.text:0000000000401343                 div     rcx               // rax:rdx / rcx => (size + 30)/0x10
.text:0000000000401346                 imul    rax, 10h          // 0x10((size + 30) / 0x10)
.text:000000000040134A                 mov     rdx, rsp           
.text:000000000040134D                 sub     rdx, rax          //if (rsp - 0x10*((size + 30) / 0x10)) >= fs:0x70
.text:0000000000401350                 cmp     rdx, fs:70h
.text:0000000000401359                 jnb     short loc_401369  // "<" path
.text:000000000040135B                 add     rax, 0Fh
.text:000000000040135F                 mov     rdi, rax        ; size
.text:0000000000401362                 call    allocate          //allocate more
.text:0000000000401367                 jmp     short loc_40136F
.text:0000000000401369 ; ---------------------------------------------------------------------------
.text:0000000000401369
.text:0000000000401369 loc_401369:                             ; CODE XREF: run+10D↑j
.text:0000000000401369                 sub     rsp, rax         rsp - (requested_size after)
.text:000000000040136C                 mov     rax, rsp         rax = ptr 
.text:000000000040136F
.text:000000000040136F loc_40136F:                             ; CODE XREF: run+11B↑j
.text:000000000040136F                 add     rax, 0Fh
.text:0000000000401373                 shr     rax, 4
.text:0000000000401377                 shl     rax, 4
.text:000000000040137B                 mov     [rbp+stack_buffer], rax
.text:000000000040137F                 mov     rax, [rbp+stack_buffer]
.text:0000000000401383                 mov     [rbp+active_buffer], rax
.text:0000000000401387                 mov     edi, offset aWhatSNameOfThe ; "What's name of the allocator ? :"
.text:000000000040138C                 call    read__
.text:0000000000401391                 lea     rax, [rbp+name_]
.text:0000000000401395                 mov     rsi, rax
.text:0000000000401398                 mov     edi, offset a15s ; "%15s"
.text:000000000040139D                 mov     eax, 0
.text:00000000004013A2                 call    __isoc99_scanf
.text:00000000004013A7                 jmp     loc_4014C9

```

#### allocate(size_t size):
#### Info for rev:

Struct time(for ez read):

```c
/* The first stack segment allocated for this thread.  */
/*a k a the ptr to the first seg */
__thread struct stack_segment *__morestack_segments
  __attribute__ ((visibility ("default")));

/* The stack segment that we think we are currently using.  This will
   be correct in normal usage, but will be incorrect if an exception
   unwinds into a different stack segment or if longjmp jumps to a
   different stack segment.  */
/*ptr to the current seg*/
__thread struct stack_segment *__morestack_current_segment
  __attribute__ ((visibility ("default")));

/* The initial stack pointer and size for this thread.  */

__thread struct initial_sp __morestack_initial_sp
  __attribute__ ((visibility ("default")));

```

```c
    struct stack_segment *seg, *current;
    struct dynamic_allocation_blocks *p;  
```

```c
struct stack_segment
{
  /* The previous stack segment--when a function running on this stack
     segment returns, it will run on the previous one.  */
  struct stack_segment *prev;
  /* The next stack segment, if it has been allocated--when a function
     is running on this stack segment, the next one is not being
     used.  */
  struct stack_segment *next;
  /* The total size of this stack segment.  */
  size_t size;
  /* The stack address when this stack was created.  This is used when
     popping the stack.  */
  void *old_stack;
  /* A list of memory blocks allocated by dynamic stack
     allocation.  */
  struct dynamic_allocation_blocks *dynamic_allocation;
  /* A list of dynamic memory blocks no longer needed.  */
  struct dynamic_allocation_blocks *free_dynamic_allocation;
  /* An extra pointer in case we need some more information some
     day.  */
  void *extra;
};
```

```c
struct dynamic_allocation_blocks
{
  /* The next block in the list.  */
  struct dynamic_allocation_blocks *next;
  /* The size of the allocated memory.  */
  size_t size;
  /* The allocated memory.  */
  void *block;
};
```

The source:
```c
void *
__morestack_allocate_stack_space (size_t size)
{
  struct stack_segment *seg, *current;
  struct dynamic_allocation_blocks *p;

  /* We have to block signals to avoid getting confused if we get
     interrupted by a signal whose handler itself uses alloca or a
     variably sized array.  */
  __morestack_block_signals ();

  /* Since we don't want to call free while we are low on stack space,
     we may have a list of already allocated blocks waiting to be
     freed.  Release them all, unless we find one that is large
     enough.  We don't look at every block to see if one is large
     enough, just the first one, because we aren't trying to build a
     memory allocator here, we're just trying to speed up common
     cases.  */

  current = __morestack_current_segment;
  p = NULL;
  for (seg = __morestack_segments; seg != NULL; seg = seg->next)
    {
      p = seg->free_dynamic_allocation;
      if (p != NULL)
	{
	  if (p->size >= size)
	    {
	      seg->free_dynamic_allocation = p->next;
	      break;
	    }

	  free_dynamic_blocks (p);
	  seg->free_dynamic_allocation = NULL;
	  p = NULL;
	}
    }

  if (p == NULL)
    {
      /* We need to allocate additional memory.  */
      p = malloc (sizeof (*p));
      if (p == NULL)
	abort ();
      p->size = size;
      p->block = malloc (size);
      if (p->block == NULL)
	abort ();
    }

  /* If we are still on the initial stack, then we have a space leak.
     FIXME.  */
  if (current != NULL)
    {
      p->next = current->dynamic_allocation;
      current->dynamic_allocation = p;
    }

  __morestack_unblock_signals ();

  return p->block;
}

```
the free func:
```
free_dynamic_blocks (struct dynamic_allocation_blocks *p)
{
  while (p != NULL)
    {
      struct dynamic_allocation_blocks *next;

      next = p->next;
      free (p->block);
      free (p);
      p = next;
    }
}
```

i will rev this func based on the source code, however, i will not dig into the gcc source code for -fsplit-stack too much.
#### The func that i rev

```c
__int64 __fastcall allocate(size_t size)
{
  __int64 current; // r13
  __int64 seg; // rbx
  _QWORD *p; // rbp
  _QWORD *ptr; // rax
  void *block; // rax

  ::block();                                    //  __morestack_block_signals ();
  current = *(_QWORD *)(__readfsqword(0) - 16); // take the current
  for ( seg = *(_QWORD *)(__readfsqword(0) - 8); seg; seg = *(_QWORD *)(seg + 8) )// scan from the first seg
  {
    p = *(_QWORD **)(seg + 0x28);               // seg->free_dynamic
    if ( p )
    {
      if ( p[1] >= size )                       // check the size to see if we can use this block
      {
        *(_QWORD *)(seg + 0x28) = *p;           // if the ptr to this block is valid and the size is okay, then take it
        goto LABEL_8;
      }
      free_(*(void **)(seg + 0x28));            // free the whole list
      *(_QWORD *)(seg + 0x28) = 0;              // fix the dangling ptr
    }                                           // check if any seg can be reuse
                                                // 
  }
  ptr = malloc(0x18u);
  p = ptr;                                      // p = malloc(size(*p));
  if ( !ptr || (ptr[1] = size, block = malloc(size), (p[2] = block) == 0) )// 
                                                // if (p == NULL) {abort();}
                                                // p->size = size; fill the info for the struct
                                                // p->block;
                                                // if (p->block == NULL) {abort();}
    abort();
LABEL_8:
  if ( current )
  {
    *p = *(_QWORD *)(current + 32);             // use p as a new stack; p->next = current->dynamic_allocation; 
                                                // current->dynamic_allocation is the head of the linked list
    *(_QWORD *)(current + 32) = p;              // current->dynamic_allocation is now p, so p become the head of the linked list
  }
  unblock();
  return p[2];                                  // p->block
}
```

### Reference

The source code for allocate is [here](https://github.com/gcc-mirror/gcc/blob/master/libgcc/generic-morestack.c#L451)
The writeup i read is [here]([https://github.com/jwang-a/CTF/blob/master/Practice/Pwnable.tw/babyallocator/exp.py](<https://github.com/jwang-a/CTF/blob/master/Practice/Pwnable.tw/babyallocator/exp.py>))
