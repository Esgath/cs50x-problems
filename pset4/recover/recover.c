#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// Prototypes
void create_new_file(FILE **fileptr, int file_counter);

int main(int argc, char *argv[])
{
    // Define BYTE type
    typedef uint8_t BYTE;

    // Check if correct usage
    if (argc != 2)
    {
        printf("How to use: recover name_of_forensic_image\n");
        return 1;
    }
    
    // Pointer to a file
    FILE *infile = fopen(argv[1], "r");

    // Check if can open for reading
    if (infile == NULL)
    {
        printf("Could not open %s.\n", argv[1]);
        return 1;
    }
    
    // Buffer
    BYTE buffer[512];

    // File number used in naming output files
    int file_counter = 0;
    
    // Pointer to outputfile
    FILE *outptr = NULL;
    
    // Boolean value used to check if writing file or still looking for header 
    bool file_found = false;

    // While not end of file:
    int a = 512;
    while (a == 512)
    {
        // Store first block in buffer
        a = fread(buffer, sizeof(BYTE), 512, infile);
        
        // Don't write to file if lower than 512 bytes found ---> EOF
        if (a != 512)
        {
            break;
        }
        
        // Check if block beggins with JPEG header
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && ((buffer[3] & 0xf0) == 0xe0))
        {
            // If first JPEG
            if (!file_found)
            {
                // Create new file
                create_new_file(&outptr, file_counter);
                fwrite(buffer, sizeof(BYTE), 512, outptr);
                
                // Update file_found and file_counter
                file_found = true;
                file_counter++;
            }
            // If already writing to file
            else if (file_found)
            {   
                // If header found, close current file
                fclose(outptr);
                
                // Create new file
                create_new_file(&outptr, file_counter);
                fwrite(buffer, sizeof(BYTE), 512, outptr);

                // Update file_found and file_counter
                file_found = true;
                file_counter++;
            }
        }
        else
        {
            // If already found JPEG, keep writing
            if (file_found)
            {
                fwrite(buffer, sizeof(BYTE), 512, outptr);
            }
        }
    }

    // Close file
    fclose(infile);
    
    // If didn't find any files, close outputfile just in case
    if (file_counter == 0)
    {
        fclose(outptr);
    }
}

void create_new_file(FILE **fileptr, int file_counter)
{
    // Filename
    char filename[8];
    sprintf(filename, "%03i.jpg", file_counter);                
                
    // Open new file
    *fileptr = fopen(filename, "w");
}