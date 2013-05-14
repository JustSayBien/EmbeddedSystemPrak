/* Use internal memory*/

/*
  Memory settings for ALTERA DE2-70 FPGA

  ram:	 	  SDRAM (64 MB)
  flash:	  FLASH-ROM (8 MB)
  sram:	 	  SRAM (2 MB)
  vectors:	  on-chip RAM (4 KB), split for Booloader and exceptions

*/

MEMORY    
{
	/*ram	: ORIGIN = 0x00000000, LENGTH = 64M*/
	lram	: ORIGIN = 0x00000000, LENGTH = 32M
	vram	: ORIGIN = 0x02000000, LENGTH = 32M
	flash	: ORIGIN = 0x04000000, LENGTH = 8M
	sram	: ORIGIN = 0x05000000, LENGTH = 2M
	vectors	: ORIGIN = 0xff000000, LENGTH = 4K

/*
    ld_info     : ORIGIN = 0x00000000, LENGTH = 0x000000F0 
    vectors     : ORIGIN = 0x00000100, LENGTH = 0x00000D00 - 0x100
    flash       : ORIGIN = 0x00000D00, LENGTH = 0x00002000 - 0x0A00
    ram         : ORIGIN = 0x00003000, LENGTH = 0x00001000
*/
}


__USTACK_SIZE = DEFINED (__USTACK_SIZE) ? __USTACK_SIZE : 4K;
__TSTACK_SIZE = DEFINED (__TSTACK_SIZE) ? __TSTACK_SIZE : 4K;
__TSTACK_N = DEFINED (__TSTACK_N) ? __TSTACK_N : 16;
__VGA_FRAME_SIZE = DEFINED (__VGA_FRAME_SIZE) ? __VGA_FRAME_SIZE : 960000;


/*
  The following section defines where to put the different input sections.
  .text contains the code.
  .data contains the initialized data.
  .bss contains uninitialized data.
  .sdata contains small constant data.
*/

SECTIONS
{
/*
      .ld_info :
      {
        revision.o(.data)        
      } > ld_info
    .vectors :   { *(.vectors) }    > vectors
*/


    .text ALIGN(4) :
    {
	*(.text)
    } > lram =0

    .rodata ALIGN(4) :
    {
	*(.rodata) *(.rodata.*)

	/* Create clear and copy tables */
    	. = ALIGN(4) ;
        PROVIDE(__clear_table = .) ;
	LONG(0 + ADDR(.bss));     LONG(SIZEOF(.bss));
    	LONG(-1);                 LONG(-1);
    	PROVIDE(__copy_table = .) ;
    	LONG(LOADADDR(.data));    LONG(ABSOLUTE(DATA_BASE)); LONG(SIZEOF(.data));
    	/*LONG(LOADADDR(.sdata));   LONG(ABSOLUTE(SDATA_BASE));LONG(SIZEOF(.sdata));*/
    	LONG(-1);                 LONG(-1);                  LONG(-1);

    } > lram

    _etext = ALIGN(4);
    PROVIDE(etext = _etext);


    .data : AT(_etext)
    {
	. = ALIGN(4) ;
    	DATA_BASE = ABSOLUTE(.) ;
	*(.data)
    } > vram

    .bss ALIGN(4) (NOLOAD) : AT(LOADADDR(.data) + SIZEOF(.data))
    {
	*(.bss)

	. = ALIGN(4) ;
        __KSTACK_TOP = .;
        . = __KSTACK_TOP + __USTACK_SIZE;
        __KSTACK_BOTTOM = .;
        __ISTACK_TOP = .;
        . = __ISTACK_TOP + __USTACK_SIZE;
        __ISTACK_BOTTOM = .;
        __USTACK_TOP = . ;
        . = __USTACK_TOP + __USTACK_SIZE ;
        __USTACK_BOTTOM = . ;

	__TSTACK_TOP = . ;
	. = __TSTACK_TOP + (__TSTACK_N * __TSTACK_SIZE);
	__TSTACK_BOTTOM = . ;
/*
        __USTACK00_TOP = .;
        . = __USTACK00_TOP + __USTACK_SIZE;
        __USTACK00_BOTTOM = .;
        __USTACK01_TOP = .;
        . = __USTACK01_TOP + __USTACK_SIZE;
        __USTACK01_BOTTOM = .;
        __USTACK02_TOP = .;
        . = __USTACK02_TOP + __USTACK_SIZE;
        __USTACK02_BOTTOM = .;
        __USTACK03_TOP = .;
        . = __USTACK03_TOP + __USTACK_SIZE;
        __USTACK03_BOTTOM = .;
        __USTACK04_TOP = .;
        . = __USTACK04_TOP + __USTACK_SIZE;
        __USTACK04_BOTTOM = .;
        __USTACK05_TOP = .;
        . = __USTACK05_TOP + __USTACK_SIZE;
        __USTACK05_BOTTOM = .;
        __USTACK06_TOP = .;
        . = __USTACK06_TOP + __USTACK_SIZE;
        __USTACK06_BOTTOM = .;
        __USTACK07_TOP = .;
        . = __USTACK07_TOP + __USTACK_SIZE;
        __USTACK07_BOTTOM = .;
        __USTACK08_TOP = .;
        . = __USTACK08_TOP + __USTACK_SIZE;
        __USTACK08_BOTTOM = .;
        __USTACK09_TOP = .;
        . = __USTACK09_TOP + __USTACK_SIZE;
        __USTACK09_BOTTOM = .;
        __USTACK10_TOP = .;
        . = __USTACK10_TOP + __USTACK_SIZE;
        __USTACK10_BOTTOM = .;
        __USTACK11_TOP = .;
        . = __USTACK11_TOP + __USTACK_SIZE;
        __USTACK11_BOTTOM = .;
        __USTACK12_TOP = .;
        . = __USTACK12_TOP + __USTACK_SIZE;
        __USTACK12_BOTTOM = .;
        __USTACK13_TOP = .;
        . = __USTACK13_TOP + __USTACK_SIZE;
        __USTACK13_BOTTOM = .;
        __USTACK14_TOP = .;
        . = __USTACK14_TOP + __USTACK_SIZE;
        __USTACK14_BOTTOM = .;
        __USTACK15_TOP = .;
        . = __USTACK15_TOP + __USTACK_SIZE;
        __USTACK15_BOTTOM = .;
*/
    } > vram

/*     
    .stack : ALIGN(4)
    {
        __USTACK_TOP = .;
        . = __USTACK_TOP + __USTACK_SIZE;
        __USTACK_BOTTOM = .;
    }                               > lram
*/    
    __RAM_END = . ;

    .vga_mem (NOLOAD) : 
    {
        __VGA_MEM = . ;
	. = . + __VGA_FRAME_SIZE;
	__VGA_END = . ;
    } > sram


}

/*
  Definitions of identifiers that control initialization and memory allocation:
  These two symbols must be present.
   __BSS_START  : Start of uninitialized data
   __BSS_END    : End of data to be cleared
*/

__CODE_START  = ADDR( .text );
__CODE_END    = ADDR( .text ) + SIZEOF( .text );

__DATA_START  = ADDR( .data );
__DATA_END    = ADDR( .data ) + SIZEOF( .data );

__BSS_START   = ADDR( .bss );
__BSS_END     = ADDR( .bss ) + SIZEOF( .bss );
