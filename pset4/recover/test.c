#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

void create_new_file(FILE **ptr);
int main(void)
{
    FILE *fileptr = NULL;
    create_new_file(&fileptr);
    typedef uint8_t BYTE;
    BYTE buffer[10];
    fread(buffer, sizeof(BYTE), 10, fileptr);
}

void create_new_file(FILE **ptr)
{
   *ptr = fopen("card.raw", "r");
}
