/*
 * Linker Script for stm32f051c6t6
 * Can be modified on SRAM LENGTH and FLASH RAM for other stm32f series mcu
 *  Created on: 23 Jul 2023
 *      Author: Malefetsane Lenka
 */
ENTRY(Reset_Handler)

/*>VMA AT>LMA for memeory specification*/
MEMORY
{
    FLASH(rx) : ORIGIN = 0x08000000, LENGTH = 516K
    SRAM(rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}
/*Location  counter always tracks VMA!!, ie .*/
SECTIONS
{
    .isr_vector :
    {
        *(.isr_vector)   /* ISR Vector comes first Not relocated to SRAM*/

    }>FLASH /*LMA and VMA are the same*/
    
    .text :
    {
        _stext = .;
        *(.text)
        *(.text.*)
        *(.init)
        *(.fini)
        *(.rodata)
        *(.rodata.*)
        . = ALIGN(4);
        _etext = .;

    }>FLASH /*>VMA AT>LMA*/

    _la_data = LOADADDR(.data);
    .data :
    {
        _sdata = .;    /*VMA is SRAM , then _sdata = 0x20000000*/
        *(.data)
        *(.data.*)
         . = ALIGN(4);
        _edata = .;

    }>SRAM AT>FLASH
    .bss :
    {
        _sbss = .;
        __bss_start__ = _sbss;
        *(.bss)
        *(.bss.*)
        *(COMMON)
         . = ALIGN(4);
        _ebss = .;
        __bss_end__ = _ebss;
        . = ALIGN(4);
        end = .;
        __end__ = end;

    }>SRAM
}