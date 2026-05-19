#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RECORD_FILE "records.txt"
#define MAX_NAME_LEN 50
#define MAX_LINE_LEN 128

typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    double salary;
} Employee;

void write_records_to_file(const char *filename, const Employee records[], size_t count)
{
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < count; ++i) {
        fprintf(fp, "%d;%s;%.2f\n", records[i].id, records[i].name, records[i].salary);
    }

    fclose(fp);
}

long *create_seek_index(const char *filename, size_t *out_count)
{
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("fopen");
        return NULL;
    }

    long *positions = NULL;
    size_t capacity = 0;
    size_t count = 0;
    char buffer[MAX_LINE_LEN];

    while (fgets(buffer, sizeof(buffer), fp)) {
        if (count >= capacity) {
            capacity = capacity ? capacity * 2 : 8;
            long *next = realloc(positions, capacity * sizeof(long));
            if (!next) {
                perror("realloc");
                free(positions);
                fclose(fp);
                return NULL;
            }
            positions = next;
        }

        positions[count++] = ftell(fp) - (long)strlen(buffer);
    }

    fclose(fp);
    *out_count = count;
    return positions;
}

int display_record_at_position(const char *filename, long position)
{
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("fopen");
        return 0;
    }

    if (fseek(fp, position, SEEK_SET) != 0) {
        perror("fseek");
        fclose(fp);
        return 0;
    }

    char line[MAX_LINE_LEN];
    if (!fgets(line, sizeof(line), fp)) {
        fprintf(stderr, "Unable to read record at position %ld\n", position);
        fclose(fp);
        return 0;
    }

    Employee record;
    if (sscanf(line, "%d;%49[^;];%lf", &record.id, record.name, &record.salary) != 3) {
        fprintf(stderr, "Invalid record format at position %ld\n", position);
        fclose(fp);
        return 0;
    }

    printf("Record at position %ld:\n", position);
    printf("  ID: %d\n", record.id);
    printf("  Name: %s\n", record.name);
    printf("  Salary: %.2f\n", record.salary);

    fclose(fp);
    return 1;
}

int main(void)
{
    Employee employees[] = {
        {101, "Alice", 55000.0},
        {102, "Bob", 62000.5},
        {103, "Carol", 71000.0},
        {104, "Dave", 48250.75}
    };
    size_t record_count = sizeof(employees) / sizeof(employees[0]);

    write_records_to_file(RECORD_FILE, employees, record_count);

    size_t index_count;
    long *seek_positions = create_seek_index(RECORD_FILE, &index_count);
    if (!seek_positions) {
        return EXIT_FAILURE;
    }

    printf("Stored %zu records in '%s'.\n", record_count, RECORD_FILE);
    printf("Seek positions:\n");
    for (size_t i = 0; i < index_count; ++i) {
        printf("  record %zu starts at %ld\n", i + 1, seek_positions[i]);
    }

    puts("\nDisplay records by seek position:");
    for (size_t i = 0; i < index_count; ++i) {
        display_record_at_position(RECORD_FILE, seek_positions[i]);
        putchar('\n');
    }

    free(seek_positions);
    return EXIT_SUCCESS;
}
