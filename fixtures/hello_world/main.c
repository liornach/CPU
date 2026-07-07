#define UART ((volatile char*)0x10000000)

void main() {
    char* s = "Hello, World!\n";
    while (*s) {
        *UART = *s++;
    }
}
