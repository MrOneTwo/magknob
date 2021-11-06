/* Define memory regions. */
MEMORY
{
  /* FLASH memory 0x0800 0000 : 0801 FFFF (128K) */
  rom (rx)   : ORIGIN = 0x08000000, LENGTH = 128K
  /* SRAM memory (20K) */
  ram (rwx) : ORIGIN = 0x20000000, LENGTH = 20K
}

INCLUDE cortex-m-generic.ld
