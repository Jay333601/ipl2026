#include <stdio.h>
#include <stdint.h>

union decision {
    unsigned char flags;
    struct field {
        unsigned int your_choice:1;        /* to be computed */
        unsigned int your_mothers_choice:1;
        unsigned int your_fathers_choice:1;
        unsigned int socially_acceptable:1;
        unsigned int financially_viable:1;
        unsigned int do_you_aptitude:1;
        unsigned int do_you_likeit:1;
        unsigned int decision:1;           /* to be computed */
    } field;
};

union decision input(void);
void make_decision(union decision *d);
void print_decsion(union decision d);
void print_conclusion_based_on_flags(union decision d);

static int count_set_bits(unsigned char value)
{
    int count = 0;
    while (value) {
        count += value & 1;
        value >>= 1;
    }
    return count;
}

union decision input(void)
{
    union decision d = { .flags = 0 };

    /* Set the inputs that matter when making a choice. */
    d.field.your_mothers_choice = 1;
    d.field.your_fathers_choice = 0;
    d.field.socially_acceptable = 1;
    d.field.financially_viable = 1;
    d.field.do_you_aptitude = 1;
    d.field.do_you_likeit = 0;

    make_decision(&d);
    return d;
}

void make_decision(union decision *d)
{
    /*
     * This expression reflects a simple decision process:
     * - Prefer something you like and are able to do if it is viable.
     * - Fall back to social / family support when personal preference is weak.
     */
    d->field.your_choice =
        (d->field.do_you_likeit && d->field.do_you_aptitude && d->field.financially_viable) ||
        (d->field.socially_acceptable && d->field.your_mothers_choice && d->field.your_fathers_choice);

    d->field.decision = d->field.your_choice;
}

void print_decsion(union decision d)
{
    puts("Decision narrative:");
    printf("- Your mother thinks it is a good idea: %s\n",
           d.field.your_mothers_choice ? "yes" : "no");
    printf("- Your father thinks it is a good idea: %s\n",
           d.field.your_fathers_choice ? "yes" : "no");
    printf("- It is socially acceptable: %s\n",
           d.field.socially_acceptable ? "yes" : "no");
    printf("- It is financially viable: %s\n",
           d.field.financially_viable ? "yes" : "no");
    printf("- You have the aptitude: %s\n",
           d.field.do_you_aptitude ? "yes" : "no");
    printf("- You personally like it: %s\n",
           d.field.do_you_likeit ? "yes" : "no");
    printf("- Your computed choice: %s\n",
           d.field.your_choice ? "accept" : "decline");
    printf("- Final decision: %s\n",
           d.field.decision ? "go ahead" : "pass" );
}

void print_conclusion_based_on_flags(union decision d)
{
    int score = count_set_bits(d.flags);
    printf("\nFlags value = 0x%02X, interpreted as %d positive signals.\n",
           d.flags, score);

    if (score >= 5) {
        puts("Conclusion: Strong consensus. This choice is well supported by the flag state.");
    } else if (score >= 3) {
        puts("Conclusion: Mixed signals. There is enough support to explore it carefully.");
    } else {
        puts("Conclusion: Weak signal. The decision is likely not the right fit yet.");
    }
}

int main(void)
{
    union decision d = input();
    print_decsion(d);
    print_conclusion_based_on_flags(d);
    return 0;
}
