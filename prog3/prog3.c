#include <stdio.h>
#include <stdint.h>
#include <string.h>

static void print_bits_uint32(uint32_t value, int bits)
{
    for (int i = bits - 1; i >= 0; --i) {
        putchar((value >> i) & 1 ? '1' : '0');
        if (i % 8 == 0 && i != 0) {
            putchar(' ');
        }
    }
}

void print_int_bits(int32_t x)
{
    uint32_t bits = (uint32_t)x;
    printf("integer: %d\n", x);
    printf("hex: 0x%08X\n", bits);
    printf("bits: ");
    print_bits_uint32(bits, 32);
    putchar('\n');
}

int count_one_bits(int32_t x)
{
    uint32_t u = (uint32_t)x;
    int count = 0;
    while (u) {
        count += u & 1u;
        u >>= 1;
    }
    return count;
}

void print_endianness(void)
{
    uint16_t value = 0x0102u;
    uint8_t *bytes = (uint8_t *)&value;

    printf("System endianness: ");
    if (bytes[0] == 0x02 && bytes[1] == 0x01) {
        printf("little endian\n");
    } else if (bytes[0] == 0x01 && bytes[1] == 0x02) {
        printf("big endian\n");
    } else {
        printf("unknown\n");
    }

    printf("Memory bytes of 0x0102: %02X %02X\n", bytes[0], bytes[1]);
}

void print_float_bits(float f)
{
    uint32_t bits;
    memcpy(&bits, &f, sizeof(bits));

    printf("float: %f\n", f);
    printf("hex: 0x%08X\n", bits);
    printf("bits: ");
    print_bits_uint32(bits, 32);
    putchar('\n');
}

int main(void)
{
    printf("=== Integer bit printing ===\n");
    print_int_bits(5);
    printf("count of 1 bits: %d\n\n", count_one_bits(5));

    print_int_bits(-5);
    printf("count of 1 bits: %d\n\n", count_one_bits(-5));

    print_endianness();
    printf("\n");

    printf("=== Floating point bit printing ===\n");
    print_float_bits(3.14f);
    putchar('\n');
    print_float_bits(-3.14f);
    putchar('\n');

    printf("=== Negative integer details ===\n");
    printf("-5 in two's complement shows the sign bit plus the bitwise representation of the negative value.\n");
    printf("The same bit pattern is used by the CPU for signed integer arithmetic.\n");

    printf("\n=== Floating point sign and exponent ===\n");
    printf("For a float, the highest bit is the sign bit, followed by the exponent and fraction.\n");
    printf("Negative floats set the sign bit to 1 and keep the exponent/mantissa bits for the magnitude.\n");

    return 0;
}
