#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DISK_SIZE 1000
#define MAX_FILES 100

typedef struct {
    char name[50];
    int start_block;
    int size;
} File;

File files[MAX_FILES];
int disk[DISK_SIZE] = {0};
int total_wasted_blocks = 0;
int total_frag = 0;

void initialize_disk() {
    for (int i = 0; i < DISK_SIZE; i++) {
        disk[i] = 0;
    }
}

int find_contiguous_blocks(int size) {
    int consecutive_free_blocks = 0;
    int start_block = -1;

    for (int i = 0; i < DISK_SIZE; i++) {
        if (disk[i] == 0) {
            if (consecutive_free_blocks == 0) {
                start_block = i;
            }
            consecutive_free_blocks++;
            if (consecutive_free_blocks == size) {
                return start_block;
            }
        } else {
            consecutive_free_blocks = 0;
        }
    }
    return -1; // Not enough contiguous space
}

void create_file(char* name, int size) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (files[i].size == 0) {
            strcpy(files[i].name, name);
            files[i].size = size;

            int start_block = find_contiguous_blocks(size);

            if (start_block != -1) {
                files[i].start_block = start_block;
                for (int j = start_block; j < start_block + size; j++) {
                    disk[j] = 1;
                }
                printf("File %s created\n", name);
            } else {
                total_wasted_blocks += size;
                printf("Error: Not enough contiguous space for file %s\n", name);
                files[i].size = 0;
            }
            return;
        }
    }
    printf("Error: Maximum number of files reached\n");
}

void delete_file(char* name) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (strcmp(files[i].name, name) == 0) {
            for (int j = files[i].start_block; j < files[i].start_block + files[i].size; j++) {
                disk[j] = 0;
            }
            total_wasted_blocks -= files[i].size;
            files[i].size = 0;
            printf("File %s deleted\n", name);
            return;
        }
    }
    printf("Error: File not found\n");
}

void rename_file(char* old_name, char* new_name) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (strcmp(files[i].name, old_name) == 0) {
            strcpy(files[i].name, new_name);
            printf("File %s renamed to %s\n", old_name, new_name);
            return;
        }
    }
    printf("Error: File not found\n");
}

void defrag_disk() {
    int empty_block = -1;
    for (int i = 0; i < DISK_SIZE; i++) {
        if (disk[i] == 0 && empty_block == -1) {
            empty_block = i;
        } else if (disk[i] == 1 && empty_block != -1) {
            total_frag += (i - empty_block);
            disk[empty_block] = 1;
            disk[i] = 0;
            empty_block = -1;
        }
    }
}

void display_disk_status() {
    int used_blocks = 0;
    for (int i = 0; i < DISK_SIZE; i++) {
        if (disk[i] == 1) {
            used_blocks++;
        }
    }

    int free_blocks = DISK_SIZE - used_blocks;

    printf("Disk Status:\n");
    printf("Total blocks: %d\n", DISK_SIZE);
    printf("Used blocks: %d\n", used_blocks);
    printf("Free blocks: %d\n", free_blocks);
    printf("Total wasted blocks: %d\n", total_wasted_blocks);
    printf("Fragmentation: %f%%\n", (float)total_frag / (used_blocks + total_frag) * 100);
}

int main() {
    char choice;
    char name[50];
    int size;

    initialize_disk();

    while (1) {
        printf("\nOptions:\n");
        printf("1. Create a file\n");
        printf("2. Delete a file\n");
        printf("3. Rename a file\n");
        printf("4. Defragment disk\n");
        printf("5. Display disk status\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                printf("Enter the name of the file: ");
                scanf("%s", name);
                printf("Enter the size of the file: ");
                scanf("%d", &size);
                create_file(name, size);
                break;
            case '2':
                printf("Enter the name of the file to delete: ");
                scanf("%s", name);
                delete_file(name);
                break;
            case '3':
                printf("Enter the name of the file to rename: ");
                scanf("%s", name);
                char new_name[50];
                printf("Enter the new name: ");
                scanf("%s", new_name);
                rename_file(name, new_name);
                break;
            case '4':
                defrag_disk();
                printf("Disk defragmented\n");
                break;
            case '5':
                display_disk_status();
                break;
            case '6':
                exit(0);
            default:
                printf("Invalid choice, please try again.\n");
        }
    }

    return 0;
}

