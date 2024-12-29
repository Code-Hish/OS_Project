#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a Block structure that holds data and points to the next block
typedef struct Block {
    char data[256];            // Data stored in the block (limit of 256 characters)
    struct Block* next;        // Pointer to the next block (linked list)
} Block;

// Define a File structure that has a name and points to the first block
typedef struct File {
    char name[50];             // File name
    Block* start_block;       // Pointer to the first block of the file
} File;

// Define a FileSystem structure to hold the list of files
typedef struct FileSystem {
    File* files[100];         // Array of pointers to File structures (directory)
    int file_count;           // Counter to track the number of files
} FileSystem;

// Function to initialize a FileSystem
void init_file_system(FileSystem* fs) {
    fs->file_count = 0;
}

// Function to check if a file with the given name already exists
int file_exists(FileSystem* fs, const char* filename) {
    for (int i = 0; i < fs->file_count; i++) {
        if (strcmp(fs->files[i]->name, filename) == 0) {
            return 1;  // File with the same name exists
        }
    }
    return 0;  // File does not exist
}

// Function to create a new file with unique name
void create_file(FileSystem* fs, char* filename) {
    while (file_exists(fs, filename)) {
        printf("Error: File '%s' already exists.\nPlease choose a different name: ", filename);
        fgets(filename, 50, stdin);
        filename[strcspn(filename, "\n")] = '\0';  // Remove newline character from filename
    }

    if (fs->file_count >= 100) {
        printf("File system full! Cannot create more files.\n");
        return;
    }

    // Create a new file
    File* new_file = (File*)malloc(sizeof(File));
    strcpy(new_file->name, filename);
    new_file->start_block = NULL;  // No data in the file initially
    
    fs->files[fs->file_count] = new_file;
    fs->file_count++;
    printf("File '%s' created successfully.\n", filename);
}

// Function to add data to a file
void write_to_file(FileSystem* fs, const char* filename, const char* data) {
    for (int i = 0; i < fs->file_count; i++) {
        File* file = fs->files[i];
        
        if (strcmp(file->name, filename) == 0) {
            // Create a new block to store the data
            Block* new_block = (Block*)malloc(sizeof(Block));
            strcpy(new_block->data, data);
            new_block->next = NULL;  // Last block points to NULL
            
            if (file->start_block == NULL) {
                file->start_block = new_block;  // First block of the file
            } else {
                // Traverse to the last block and append the new block
                Block* current = file->start_block;
                while (current->next != NULL) {
                    current = current->next;
                }
                current->next = new_block;
            }
            printf("Data written to file '%s'.\n", filename);
            return;
        }
    }
    printf("File '%s' not found.\n", filename);
}

// Function to read data from a file
void read_from_file(FileSystem* fs, const char* filename) {
    for (int i = 0; i < fs->file_count; i++) {
        File* file = fs->files[i];
        
        if (strcmp(file->name, filename) == 0) {
            Block* current = file->start_block;
            printf("Data from file '%s':\n", filename);
            while (current != NULL) {
                printf("%s", current->data);
                current = current->next;
            }
            printf("\n");
            return;
        }
    }
    printf("File '%s' not found.\n", filename);
}

// Function to display all files in the directory
void show_directory(FileSystem* fs) {
    printf("Directory contents:\n");
    for (int i = 0; i < fs->file_count; i++) {
        printf("- %s\n", fs->files[i]->name);
    }
}

int main() {
    FileSystem fs;
    init_file_system(&fs);
    
    int num_files;
    char filename[50];
    char data[256];
    
    // Ask the user how many files they want to create
    printf("Enter the number of files you want to create: ");
    scanf("%d", &num_files);
    getchar();  // Consume newline character left by scanf
    
    // Create files and add data to them
    for (int i = 0; i < num_files; i++) {
        // Input file name
        printf("\n");
        printf("\nEnter the name of file %d: ", i + 1);
        fgets(filename, sizeof(filename), stdin);
        filename[strcspn(filename, "\n")] = '\0';  // Remove newline character from filename
        
        // Create the file
        create_file(&fs, filename);

        // Ask for the number of blocks of data to write
        int num_blocks;
        printf("Enter the number of blocks of data you want to write to '%s': ", filename);
        scanf("%d", &num_blocks);
        getchar();  // Consume newline character left by scanf
        
        // Input data for each block and write to file
        for (int j = 0; j < num_blocks; j++) {
            printf("Enter data for block %d of '%s': ", j + 1, filename);
            fgets(data, sizeof(data), stdin);
            data[strcspn(data, "\n")] = '\0';  // Remove newline character from data
            
            write_to_file(&fs, filename, data);
        }
    }
    
    // Show the directory contents
    printf("\n");
    show_directory(&fs);

    // Read data from files
    printf("\n");
    for (int i = 0; i < fs.file_count; i++) {
        read_from_file(&fs, fs.files[i]->name);
        printf("\n");
    }

    return 0;
}
