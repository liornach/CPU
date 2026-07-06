# Minimal bare-metal entry point: no libc, no crt0. Sets up the stack and calls
# main(), then halts with ebreak (rather than looping forever) so the emulator's
# clean-halt path is what ends execution.
.section .text.start
.global _start
_start:
    lui sp, 0x20
    call main
    ebreak
