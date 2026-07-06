# RV32I emulator — design notes and assumptions

A minimal bare-metal RV32I emulator that loads a GCC-produced ELF and runs it, with a
single memory-mapped UART for output. No libc, no OS, no syscalls beyond `ecall`/`ebreak`
acting as halts.

## Layout

- `include/gates/` — primitive logic gates (NAND, NOT, AND, OR, NOR, XOR).
- `include/components/` — gate-composed building blocks: half/full adder, 2:1 mux,
  32-bit adder/subtractor, signed/unsigned comparators, 32-bit bitwise ops, sign extension.
- `include/memory/memory.hpp` — flat byte-addressable RAM with little-endian access and
  the memory-mapped UART.
- `include/elf/elf_loader.hpp` — minimal ELF32 LE loader (PT_LOAD segments only).
- `include/cpu/decoder.hpp` — RV32I instruction decoder.
- `include/cpu/cpu.hpp` — the fetch/decode/execute loop.
- `tools/rv32i_emulator.cpp` — CLI: `rv32i_emulator <elf> [max-instructions]`.
- `fixtures/hello_world/` — the target program, a hand-written `_start`, and a linker
  script; built automatically by CMake when a RISC-V bare-metal GCC is on `PATH`.

## Memory map

- `0x00000000` and up: flat RAM (size is caller-configured; the CLI tool and the
  integration test both use 1 MiB).
- `0x10000000`: UART. A byte **store** here is forwarded to an output sink instead of
  being written into RAM. A byte **load** from here always returns 0 (the UART is
  treated as write-only; nothing in the target program reads it).
- Nothing else is memory-mapped. There's no interrupt controller, timer, or second UART
  register (status/ready bits) — output is modeled as unconditionally accepting a byte.

## Stack

`ElfLoader::Load` returns a suggested initial stack pointer (top of the emulator's RAM,
16-byte aligned), which `Cpu::Reset` writes into `x2` before execution starts. In
practice the fixture's own `_start` (`fixtures/hello_world/start.s`) sets `sp` itself via
`lui sp, 0x20`, so the loader's suggested value is a fallback for programs that don't set
up their own stack, not something this particular program relies on.

## ELF loader limitations

- ELF32, little-endian only (`EI_CLASS == ELFCLASS32`, `EI_DATA == ELFDATA2LSB`,
  `e_machine == EM_RISCV`).
- Only `PT_LOAD` program headers are honored. Section headers, symbol tables,
  relocations, dynamic linking, and `.eh_frame`/`.riscv.attributes`-style metadata
  sections are all ignored — a statically linked bare-metal `EXEC` is all this needs to
  support.
- No relocation processing. The fixture's linker script places everything at fixed
  addresses starting at 0, so the GCC-produced `EXEC` needs none.

## Unaligned memory access

Not modeled as a fault. `Memory::LoadHalf/LoadWord/StoreHalf/StoreWord` always decompose
into individual byte accesses, so an unaligned load/store silently "works" by reading or
writing whatever bytes happen to sit at `addr, addr+1, ...`. Real RV32I hardware may trap
on unaligned accesses; this emulator deliberately doesn't, to keep the memory model simple.

## RV32I coverage / unsupported instructions

Implemented: `LUI`, `AUIPC`, `JAL`, `JALR`, all six branches (`BEQ`/`BNE`/`BLT`/`BGE`/
`BLTU`/`BGEU`), all five loads (`LB`/`LH`/`LW`/`LBU`/`LHU`), all three stores (`SB`/`SH`/
`SW`), all nine immediate ALU ops (`ADDI`/`SLTI`/`SLTIU`/`XORI`/`ORI`/`ANDI`/`SLLI`/`SRLI`/
`SRAI`), and all ten register-register ALU ops (`ADD`/`SUB`/`SLL`/`SLT`/`SLTU`/`XOR`/
`SRL`/`SRA`/`OR`/`AND`). `ECALL`/`EBREAK` are recognized and treated as a clean halt.

Not implemented: `FENCE` (a no-op on a single-hart in-order emulator; simply not decoded)
and anything outside the RV32I base — no M (multiply/divide), A (atomics), F/D
(floating point), or C (compressed) extension instructions. Any word that doesn't decode
to a known RV32I encoding halts the CPU with `HaltReason::UnknownInstruction` rather than
executing something incorrect.

## Gate-level vs. native scope decision

Per the plan, `ADD`/`SUB`/`ADDI`/`AND`/`ANDI`/`OR`/`ORI`/`XOR`/`XORI`/`SLT`/`SLTI`/
`SLTU`/`SLTIU` all route through the gate-composed `Adder32`, `Bitwise32`, and
`Comparator32` components (`include/components/`), not native C++ `+`/`-`/`&`/`|`/`^`/
comparison operators. Shifts (`SLL`/`SRL`/`SRA` and their immediate forms) use native
C++ shift operators — no gate-level barrel shifter is implemented, since none of the
required components list called for one and no target program's control flow depends on
shift semantics being gate-composed. This is a deliberate scope cut, not an oversight.
