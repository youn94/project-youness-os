
ENTRY(_start)

SECTIONS
{
    . = 0x9000; /* <--- CHANGÉ ICI : Évite 64 Ko de zéros inutiles au début du fichier */

    .text : {
        *(.entry)
        *(.text*)
    }
    .rodata : { *(.rodata*) }
    .data : { *(.data*) }
    .bss : { *(.bss*) }


}