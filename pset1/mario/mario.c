#include <stdio.h>
#include <cs50.h>

void draw_pyramid(int height);

int main(void){
    int height;
    do
    {
        height = get_int("Height:");
    }while (height < 1 || height >8);
    draw_pyramid(height);
}

void draw_pyramid(int height)
{   
    int hashes = 0;

    for(int i = 0; i < height; i++)
    {   
        hashes++;
        int white_space = height - hashes;

        // print white space
        for(int j = 0; j < white_space; j++)
        {
            printf(" ");
        }
        
        // print hashes
        for(int x = 0; x < hashes; x++)
        {
            printf("#");
        }

        // print gap
        printf("  ");

        //print hashes
        for(int y = 0; y < hashes; y++)
        {
            printf("#");
        }       
        printf("\n");
        
    }
}