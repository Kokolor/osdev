ENTRY(entry)

PHDRS {
    limine_requests PT_LOAD;
    text PT_LOAD;
    rodata PT_LOAD;
    data PT_LOAD;
}

SECTIONS {
    . = 0xffffffff80000000;

    .limine_requests : {
        KEEP(*(.limine_requests_start))
        KEEP(*(.limine_requests))
        KEEP(*(.limine_requests_end))
    } :limine_requests

    . = ALIGN(CONSTANT(MAXPAGESIZE));

    text_start = .;

    .text : {
        *(.text .text.*)
    } :text

    text_end = .;

    . = ALIGN(CONSTANT(MAXPAGESIZE));

    rodata_start = .;

    .rodata : {
        *(.rodata .rodata.*)
    } :rodata

    rodata_end = .;

    . = ALIGN(CONSTANT(MAXPAGESIZE));

    data_start = .;

    .data : {
        *(.data .data.*)
    } :data

    .bss : {
        *(.bss .bss.*)
        *(COMMON)
    } :data

    data_end = .;

    /DISCARD/ : {
        *(.eh_frame*)
        *(.note .note.*)
    }
}