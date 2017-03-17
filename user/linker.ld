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
    __text_end = .;
 
    __rodata_start = .;
    .rodata :
    {
        *(.rodata)
    }
    __rodata_end = .;
 
    __data_start = .;
    .data :
    {
        *(.data)
    }
    __data_end = .;
 
    __bss_start = .;
    .bss :
    {
        bss = .;
        *(.bss)
    }
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
