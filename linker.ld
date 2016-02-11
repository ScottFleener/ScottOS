/* Bootloader looks here for symbol to start execution at */
ENTRY(_start)

/* Tell where various sections of the objects files will be in the final kernel
   image */
SECTIONS
{
  /* Start putting sections at 1MiB, a conventional place for kernels to be
     loaded by the bootloader */
  . = 1M;

  /* First is multiboot header, required to be early in the image or the
     bootloader doesn't recognize the file format */
  .text BLOCK(4K) : ALIGN(4K)
  {
    *(.multiboot)
    *(.text)
  }

  /* Read-only data */
  .rodata BLOCK(4K) : ALIGN(4K)
  {
    *(.rodata)
  }

  /* Read-write data (initialized) */
  .data BLOCK(4K) : ALIGN(4K)
  {
    *(.data)
  }

  /* Read-write data (uninitialized) and stack */
  .bss BLOCK(4K) : ALIGN(4K)
  {
    *(COMMON)
    *(.bss)
    *(.bootstrap_stack)
  }

  /* The compiler may produce other sections, by default puts them in a segment
     with the same name. Add more here as needed. */
}
