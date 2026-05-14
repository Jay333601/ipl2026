#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char name[50];
} Record;

void storeRecords(int n) {
    FILE *fp = fopen("records.bin", "wb");
    if (!fp) {
        perror("Error opening file");
        return;
    }
    Record r;
    for (int i = 0; i < n; i++) {
        printf("Enter id and name for record %d: ", i + 1);
        scanf("%d %s", &r.id, r.name);
        fwrite(&r, sizeof(Record), 1, fp);
    }
    fclose(fp);
}

void getRecord(int m) {
    FILE *fp = fopen("records.bin", "rb");
    if (!fp) {
        perror("Error opening file");
        return;
    }
    fseek(fp, (m - 1) * sizeof(Record), SEEK_SET);
    Record r;
    if (fread(&r, sizeof(Record), 1, fp) == 1) {
        printf("Record %d: ID=%d, Name=%s\n", m, r.id, r.name);
    } else {
        printf("Record not found\n");
    }
    fclose(fp);
}

void deleteRecord(int id) {
    // Load all records into memory
    FILE *fp = fopen("records.bin", "rb");
    if (!fp) {
        perror("Error opening file");
        return;
    }
    Record *records = NULL;
    int count = 0;
    Record temp;
    while (fread(&temp, sizeof(Record), 1, fp) == 1) {
        records = realloc(records, (count + 1) * sizeof(Record));
        records[count] = temp;
        count++;
    }
    fclose(fp);

    // Find and remove the record
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (records[i].id == id) {
            for (int j = i; j < count - 1; j++) {
                records[j] = records[j + 1];
            }
            count--;
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Record with ID %d not found\n", id);
        free(records);
        return;
    }

    // Rewrite the file
    fp = fopen("records.bin", "wb");
    if (!fp) {
        perror("Error opening file");
        free(records);
        return;
    }
    for (int i = 0; i < count; i++) {
        fwrite(&records[i], sizeof(Record), 1, fp);
    }
    fclose(fp);
    free(records);
    printf("Record deleted\n");
}

int main() {
    int choice, n, m, id;
    while (1) {
        printf("1. Store records\n2. Get record\n3. Delete record\n4. Exit\n");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("Enter n: ");
                scanf("%d", &n);
                storeRecords(n);
                break;
            case 2:
                printf("Enter m: ");
                scanf("%d", &m);
                getRecord(m);
                break;
            case 3:
                printf("Enter id to delete: ");
                scanf("%d", &id);
                deleteRecord(id);
                break;
            case 4:
                exit(0);
        }
    }
    return 0;
}