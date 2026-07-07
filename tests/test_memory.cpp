#include "memory/memory.hpp"
#include "test_util.hpp"

#include <cstdint>
#include <string>

int main()
{
    using ln::Memory;
    using namespace ln::test;

    // Byte round trip.
    {
        Memory mem(64);
        mem.StoreByte(10, 0xAB);
        Check(mem.LoadByte(10) == 0xAB, "byte round trip");
    }

    // Half-word round trip and little-endian byte order.
    {
        Memory mem(64);
        mem.StoreHalf(4, 0xBEEF);
        Check(mem.LoadHalf(4) == 0xBEEF, "half round trip");
        Check(mem.LoadByte(4) == 0xEF, "half low byte little-endian");
        Check(mem.LoadByte(5) == 0xBE, "half high byte little-endian");
    }

    // Word round trip and little-endian byte order.
    {
        Memory mem(64);
        mem.StoreWord(8, 0xDEADBEEFu);
        Check(mem.LoadWord(8) == 0xDEADBEEFu, "word round trip");
        Check(mem.LoadByte(8) == 0xEF, "word byte 0 little-endian");
        Check(mem.LoadByte(9) == 0xBE, "word byte 1 little-endian");
        Check(mem.LoadByte(10) == 0xAD, "word byte 2 little-endian");
        Check(mem.LoadByte(11) == 0xDE, "word byte 3 little-endian");
    }

    // Unaligned access: documented as permitted, composed byte-by-byte.
    {
        Memory mem(64);
        mem.StoreWord(1, 0x11223344u);
        Check(mem.LoadWord(1) == 0x11223344u, "unaligned word round trip");
    }

    // UART: byte stores are forwarded to the sink, not written into RAM.
    {
        Memory mem(64);
        std::string captured;
        mem.SetUartSink([&captured](char c) { captured += c; });

        const std::string message = "Hi\n";
        for (char c : message)
        {
            mem.StoreByte(Memory::kUartAddress, static_cast<std::uint8_t>(c));
        }
        Check(captured == message, "UART sink receives stored bytes in order");
        Check(mem.LoadByte(Memory::kUartAddress) == 0, "UART reads back as 0 (write-only)");
    }

    return g_failures == 0 ? 0 : 1;
}
