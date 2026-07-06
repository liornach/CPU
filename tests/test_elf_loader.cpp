#include "elf/elf_loader.hpp"
#include "memory/memory.hpp"
#include "test_util.hpp"

#include <cstdint>
#include <vector>

namespace
{

void AppendU16(std::vector<std::uint8_t>& out, std::uint16_t v)
{
    out.push_back(static_cast<std::uint8_t>(v & 0xFF));
    out.push_back(static_cast<std::uint8_t>((v >> 8) & 0xFF));
}

void AppendU32(std::vector<std::uint8_t>& out, std::uint32_t v)
{
    out.push_back(static_cast<std::uint8_t>(v & 0xFF));
    out.push_back(static_cast<std::uint8_t>((v >> 8) & 0xFF));
    out.push_back(static_cast<std::uint8_t>((v >> 16) & 0xFF));
    out.push_back(static_cast<std::uint8_t>((v >> 24) & 0xFF));
}

// Hand-builds a minimal, valid-enough ELF32 LE RISC-V file with a single PT_LOAD
// segment, so the loader can be tested without depending on an external toolchain.
std::vector<std::uint8_t> BuildSyntheticElf(std::uint32_t entry, std::uint32_t vaddr,
                                             const std::vector<std::uint8_t>& segmentData,
                                             std::uint32_t memsz)
{
    std::vector<std::uint8_t> file;

    // e_ident
    file.push_back(0x7F);
    file.push_back('E');
    file.push_back('L');
    file.push_back('F');
    file.push_back(1); // EI_CLASS = ELFCLASS32
    file.push_back(1); // EI_DATA = ELFDATA2LSB
    file.push_back(1); // EI_VERSION
    for (int i = 0; i < 9; ++i) file.push_back(0); // EI_OSABI..EI_PAD

    AppendU16(file, 2);      // e_type = ET_EXEC
    AppendU16(file, 0xF3);   // e_machine = EM_RISCV
    AppendU32(file, 1);      // e_version
    AppendU32(file, entry);  // e_entry
    AppendU32(file, 52);     // e_phoff (right after the 52-byte header)
    AppendU32(file, 0);      // e_shoff
    AppendU32(file, 0);      // e_flags
    AppendU16(file, 52);     // e_ehsize
    AppendU16(file, 32);     // e_phentsize
    AppendU16(file, 1);      // e_phnum
    AppendU16(file, 0);      // e_shentsize
    AppendU16(file, 0);      // e_shnum
    AppendU16(file, 0);      // e_shstrndx

    std::uint32_t segmentOffset = 52 + 32; // header + one program header

    // Program header (PT_LOAD)
    AppendU32(file, 1);                                  // p_type = PT_LOAD
    AppendU32(file, segmentOffset);                      // p_offset
    AppendU32(file, vaddr);                               // p_vaddr
    AppendU32(file, vaddr);                               // p_paddr
    AppendU32(file, static_cast<std::uint32_t>(segmentData.size())); // p_filesz
    AppendU32(file, memsz);                               // p_memsz
    AppendU32(file, 5);                                   // p_flags (R+X)
    AppendU32(file, 0x1000);                              // p_align

    file.insert(file.end(), segmentData.begin(), segmentData.end());

    return file;
}

}

int main()
{
    using ln::ElfLoader;
    using ln::Memory;
    using namespace ln::test;

    std::vector<std::uint8_t> segment = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 };
    std::uint32_t vaddr = 0x1000;
    std::uint32_t entry = 0x1000;
    std::uint32_t memsz = 10; // 4 bytes beyond filesz should be zero-filled

    std::vector<std::uint8_t> file = BuildSyntheticElf(entry, vaddr, segment, memsz);

    Memory memory(0x2000);
    auto result = ElfLoader::Load(file, memory);

    Check(result.entryPoint == entry, "entry point parsed correctly");

    for (std::size_t i = 0; i < segment.size(); ++i)
    {
        Check(memory.LoadByte(static_cast<std::uint32_t>(vaddr + i)) == segment[i], "segment byte loaded at expected address");
    }
    for (std::uint32_t i = static_cast<std::uint32_t>(segment.size()); i < memsz; ++i)
    {
        Check(memory.LoadByte(vaddr + i) == 0, "memsz beyond filesz is zero-filled");
    }

    Check(result.stackTop == (0x2000u & ~0xFu), "stack top derived from memory size, 16-byte aligned");

    return g_failures == 0 ? 0 : 1;
}
