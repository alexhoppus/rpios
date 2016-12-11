ENTRY(_start)
 
SECTIONS
{
    . = 0x800000;
    _start = .;
    __text_start = .;
    .text :
    {
        *(.text, .text.boot)
    }
    . = ALIGN(4096); /* align to page size */
    __text_end = .;
 
    __rodata_start = .;
    .rodata :
    {
        *(.rodata)
    }
    . = ALIGN(4096); /* align to page size */
    __rodata_end = .;
 
    __data_start = .;
    .data :
    {
        *(.data)
    }
    . = ALIGN(4096); /* align to page size */
    __data_end = .;
 
    __bss_start = .;
    .bss :
    {
        bss = .;
        *(.bss)
    }
    . = ALIGN(4096); /* align to page size */
    __bss_end = .;

    __init_array_start = .;
    .init_array :
    {
	    KEEP (*(.init_array))
	    *(.init_array)
    }
    . = ALIGN(4);
    __init_array_end = .;

   __end = .;
}
