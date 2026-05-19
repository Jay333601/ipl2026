#include <stdio.h>
#include <stdint.h>

/*
 * Example of a structure with bitfields.
 * We define a bitfield `Flags` with several narrow fields.
 */

typedef struct {
    unsigned int firstbit  : 1;  /* 1 bit */
    unsigned int secondbit : 2;  /* 2 bits */
    unsigned int thirdbit  : 3;  /* 3 bits */
    unsigned int reserved  : 2;  /* 2 bits reserved for future use */
} Flags;

void print_flags(const char *label, Flags f)
{
    printf("%s:\n", label);
    printf("  firstbit  = %u\n", f.firstbit);
    printf("  secondbit = %u\n", f.secondbit);
    printf("  thirdbit  = %u\n", f.thirdbit);
    printf("  reserved  = %u\n", f.reserved);
    printf("  raw bytes = 0x%02x\n", *(unsigned char *)&f);
}

int main(void)
{
    Flags a = {0};
    Flags b = {0};

    a.firstbit = 1;
    a.secondbit = 3; /* max value 3 for 2 bits */
    a.thirdbit = 5;  /* max value 7 for 3 bits */

    b.firstbit = 0;
    b.secondbit = 1;
    b.thirdbit = 2;
    b.reserved = 1;

    print_flags("Record A", a);
    print_flags("Record B", b);

    /* Demonstrate that bitfields are stored in the same memory as an integer
     * and that only the specified width is used. */
    printf("\nBitfield sizes:\n");
    printf("  sizeof(Flags) = %zu\n", sizeof(Flags));
    printf("  total bits in Flags = %zu\n", sizeof(Flags) * 8);

    return 0;
}
