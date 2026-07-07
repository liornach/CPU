#ifndef LN_MEMORY_MEMORY_HPP
#define LN_MEMORY_MEMORY_HPP

#include <cstddef>
#include <cstdint>
#include <functional>
#include <vector>

namespace ln
{

// Flat byte-addressable RAM with little-endian multi-byte access.
//
// kUartAddress is not backed by real storage: a byte store there is forwarded to an
// output sink instead of being written into RAM, and a byte load from there always
// reads 0 (the UART is treated as write-only).
//
// Assumption: this emulator does not model alignment faults. Multi-byte loads/stores
// are always composed byte-by-byte, so unaligned accesses silently work by wrapping
// across whatever bytes happen to sit at addr, addr+1, ... rather than trapping.
// Out-of-range addresses (outside both RAM and the UART address) throw std::out_of_range,
// via std::vector::at.
class Memory
{
public:
    static constexpr std::uint32_t kUartAddress = 0x10000000u;

    explicit Memory(std::size_t sizeBytes) : _ram(sizeBytes, 0) {}

    void SetUartSink(std::function<void(char)> sink) { _uartSink = std::move(sink); }

    std::uint8_t LoadByte(std::uint32_t addr) const
    {
        if (addr == kUartAddress) return 0;
        return _ram.at(addr);
    }

    void StoreByte(std::uint32_t addr, std::uint8_t value)
    {
        if (addr == kUartAddress)
        {
            if (_uartSink) _uartSink(static_cast<char>(value));
            return;
        }
        _ram.at(addr) = value;
    }

    std::uint16_t LoadHalf(std::uint32_t addr) const
    {
        return static_cast<std::uint16_t>(
            (static_cast<std::uint16_t>(LoadByte(addr))) |
            (static_cast<std::uint16_t>(LoadByte(addr + 1)) << 8));
    }

    void StoreHalf(std::uint32_t addr, std::uint16_t value)
    {
        StoreByte(addr, static_cast<std::uint8_t>(value & 0xFFu));
        StoreByte(addr + 1, static_cast<std::uint8_t>((value >> 8) & 0xFFu));
    }

    std::uint32_t LoadWord(std::uint32_t addr) const
    {
        return static_cast<std::uint32_t>(LoadByte(addr)) |
               (static_cast<std::uint32_t>(LoadByte(addr + 1)) << 8) |
               (static_cast<std::uint32_t>(LoadByte(addr + 2)) << 16) |
               (static_cast<std::uint32_t>(LoadByte(addr + 3)) << 24);
    }

    void StoreWord(std::uint32_t addr, std::uint32_t value)
    {
        StoreByte(addr, static_cast<std::uint8_t>(value & 0xFFu));
        StoreByte(addr + 1, static_cast<std::uint8_t>((value >> 8) & 0xFFu));
        StoreByte(addr + 2, static_cast<std::uint8_t>((value >> 16) & 0xFFu));
        StoreByte(addr + 3, static_cast<std::uint8_t>((value >> 24) & 0xFFu));
    }

    std::size_t Size() const { return _ram.size(); }

private:
    std::vector<std::uint8_t> _ram;
    std::function<void(char)> _uartSink;
};

}

#endif
