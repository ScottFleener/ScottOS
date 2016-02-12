# Declare constants for multiboot header
.set ALIGN,   1<<0              # align loaded modules on page boundaries
.set MEMINFO, 1<<1              # provide memory map
.set FLAGS,   ALIGN | MEMINFO   # multiboot flag field
.set MAGIC,   0x1BADB002        # 'magic number', lets bootloader find header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of MAGIC to prove multiboot

# Declare header as in multiboot standard. Comes first to force the header to
# be at start of final program.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Provide small temporary stack
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

# Specify start as entry point to kernel
.section .text
.global _start
.type _start, @function
_start:
  # Set stack pointer to temporary stack
  movl $stack_top, %esp

  # Initialize core kernel before running global constructors
  call kernel_early

  # Call global constructors, not needed for now
  # call init

  # Pass control to kernel
  call kernel_main

  # Hang if kernel_main returns unexpectedly
  cli
.Lhang:
  hlt
  jmp .Lhang

# Sets size of _start symbol to the current location (.) minus its start
.size _start, . - _start
