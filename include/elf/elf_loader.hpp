#ifndef LN_ELF_ELF_LOADER_HPP
#define LN_ELF_ELF_LOADER_HPP

#include "memory/memory.hpp"

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

namespace ln
{

struct ElfLoadResult
{
    std::uint32_t entryPoint;
    std::uint32_t stackTop; // suggested initial stack pointer: top of memory, 16-byte aligned
};

// Minimal ELF32 little-endian loader for bare-metal RISC-V executables.
// Only PT_LOAD segments are honored; symbol tables, relocations, dynamic linking,
// and everything else in the ELF are ignored, since a bare-metal program doesn't
// need them to run.
class ElfLoader
{
public:
    static ElfLoadResult Load(const std::vector<std::uint8_t>& file, Memory& memory)
    {
        constexpr std::size_t kEhdrSize = 52;
        if (file.size() < kEhdrSize) throw std::runtime_error("ELF file too small for a 32-bit header");

        if (!(file[0] == 0x7F && file[1] == 'E' && file[2] == 'L' && file[3] == 'F'))
        {
            throw std::runtime_error("missing ELF magic");
        }
        if (file[4] != 1) throw std::runtime_error("not a 32-bit ELF (EI_CLASS != ELFCLASS32)");
        if (file[5] != 1) throw std::runtime_error("not little-endian (EI_DATA != ELFDATA2LSB)");

        constexpr std::uint16_t kEmRiscV = 0xF3;
        std::uint16_t machine = ReadU16(file, 18);
        if (machine != kEmRiscV) throw std::runtime_error("not a RISC-V ELF (e_machine != EM_RISCV)");

        std::uint32_t entry = ReadU32(file, 24);
        std::uint32_t phoff = ReadU32(file, 28);
        std::uint16_t phentsize = ReadU16(file, 42);
        std::uint16_t phnum = ReadU16(file, 44);

        constexpr std::uint32_t kPtLoad = 1;

        for (std::uint16_t i = 0; i < phnum; ++i)
        {
            std::size_t base = static_cast<std::size_t>(phoff) + static_cast<std::size_t>(i) * phentsize;

            std::uint32_t type = ReadU32(file, base + 0);
            if (type != kPtLoad) continue;

            std::uint32_t offset = ReadU32(file, base + 4);
            std::uint32_t vaddr = ReadU32(file, base + 8);
            std::uint32_t filesz = ReadU32(file, base + 16);
            std::uint32_t memsz = ReadU32(file, base + 20);

            for (std::uint32_t b = 0; b < filesz; ++b)
            {
                memory.StoreByte(vaddr + b, file.at(offset + b));
            }
            for (std::uint32_t b = filesz; b < memsz; ++b)
            {
                memory.StoreByte(vaddr + b, 0);
            }
        }

        std::uint32_t stackTop = static_cast<std::uint32_t>(memory.Size()) & ~0xFu;
        return ElfLoadResult{ entry, stackTop };
    }

    static std::vector<std::uint8_t> ReadFile(const std::string& path)
    {
        std::ifstream in(path, std::ios::binary);
        if (!in) throw std::runtime_error("failed to open ELF file: " + path);
        return std::vector<std::uint8_t>(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
    }

private:
    static std::uint16_t ReadU16(const std::vector<std::uint8_t>& file, std::size_t offset)
    {
        return static_cast<std::uint16_t>(
            static_cast<std::uint16_t>(file.at(offset)) |
            (static_cast<std::uint16_t>(file.at(offset + 1)) << 8));
    }

    static std::uint32_t ReadU32(const std::vector<std::uint8_t>& file, std::size_t offset)
    {
        return static_cast<std::uint32_t>(file.at(offset)) |
               (static_cast<std::uint32_t>(file.at(offset + 1)) << 8) |
               (static_cast<std::uint32_t>(file.at(offset + 2)) << 16) |
               (static_cast<std::uint32_t>(file.at(offset + 3)) << 24);
    }
};

}

#endif
