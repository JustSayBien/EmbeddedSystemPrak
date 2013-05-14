/* Use internal memory*/

/*
  Memory settings for ALTERA DB_4CE10 FPGA

  ram:	 	  SDRAM (32 MB)
  flash:	  FLASH-ROM (0 MB)
  sram:	 	  SRAM (0 MB)
  vectors:	  on-chip RAM (4 KB), split for Booloader and exceptions

// TODO!
*/

MEMORY    
{
	/*ram	: ORIGIN = 0x00000000, LENGTH = 64M*/
	lram	: ORIGIN = 0x00000000, LENGTH = 8M
	vram	: ORIGIN = 0x00800000, LENGTH = 8M
	vectors	: ORIGIN = 0xff000000, LENGTH = 4K

/*
    ld_info     : ORIGIN = 0x00000000, LENGTH = 0x000000F0 
    vectors     : ORIGIN = 0x00000100, LENGTH = 0x00000D00 - 0x100
    flash       : ORIGIN = 0x00000D00, LENGTH = 0x00002000 - 0x0A00
    ram         : ORIGIN = 0x00003000, LENGTH = 0x00001000
*/
}


__USTACK_SIZE = DEFINED (__USTACK_SIZE) ? __USTACK_SIZE : 4K;


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
        __USTACK_TOP = . ;
        . = __USTACK_TOP + __USTACK_SIZE ;
        __USTACK_BOTTOM = . ;

    } > vram

    __RAM_END = . ;


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
