#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Check that the argument count is 2
    if(argc != 2)
    {
        printf("Usage: .recover IMAGE\n");
        return 1;
    }

    // Open file for reading
    FILE *input_file = fopen(argv[1], "r");

    // Check that the input_file is valid
    if(input_file == NULL)
    {
         printf("Could not open the file\n");
          return 2;
    }

    //File pointer for recovered images
    FILE *output_file = NULL;

    //create an array of 512 elements to store 512 bytes from the memory card
    BYTE buffer[512];

    //count amount of jpeg files found
    int count_image=0;

    //string to hold a filename
    char filename[8]={0};

    //read memory card until the end of file
    while (fread(buffer, sizeof(BYTE)*512, 1, input_file)==1)
    {
        //check if jpeg is found
        if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {

            //close outptr if jpeg was found before and written into ###.jpg

            if(output_file != NULL)
            {
                fclose(output_file);
            }

            sprintf(filename, "%03i.jpg", count_image++);

            // Open output_file for writing
            output_file = fopen(filename, "w");

        }
        //Check if output has been used for valid input
        if(output_file != NULL)
        {
            fwrite(buffer, sizeof(BYTE)*512, 1, output_file);
        }
    }
    // close last opened outptr
    if (output_file != NULL)
    {
        fclose(output_file);
    }
    //close input file (forensic image)
    fclose(input_file);

    return 0;
}