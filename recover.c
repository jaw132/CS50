/* Script to recover jpegs from a forensic image. It takes one parameter - the forensic image */

#include <stdio.h>
#include <stdlib.h>

// number of images we want to recover +1
#define MAXIMAGE 11


int main(int argc, char *argv[])
{
    // Check only one paramter is provided.
    if (argc != 2){
        printf("Wrong number of parameters entered, program takes one.\n");
        return 1;
    }

    char *infile = argv[1];
    
    // Check we can read the forensic image
    FILE *input = fopen(infile, "r");
    if (input == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 1;
    }

    int imageno = 1;
    int counter = 0, passed = 0;
    unsigned char *jpeg;

    // nested loop, first opens file to write jpeg bytes too, the second checks if the image is a jpeg and then write to the aforementioned file
    while (1){
        char image_file[15];
        sprintf(image_file, "image%i.jpg", imageno);

        FILE *out = fopen(image_file, "w");

        if (passed > 0){
            fwrite(jpeg, 512 , 1 , out);
            free(jpeg);
        }

        while (1){
            
            //allocate memory to read forensic image to - 512 bytes as digital cameras tend to store bytes in 512B blocks (FAT)
            jpeg = calloc(1, 512);
            fread(jpeg, 512, 1, input);
            
            // Check if we found the start of a new jpeg
            if (jpeg[0] == 0xff && jpeg[1] == 0xd8 && jpeg[2] == 0xff){
                counter++;
                passed++;
            }

            if (counter == 2){
                counter--;
                imageno++;
                break;
            }
            
            // The forensic image provided has jpegs stored contigously so once we find the one we can just write to the new file
            if (passed > 0){
                fwrite(jpeg, 512 , 1 , out);
            }

            free(jpeg);
        }

        fclose(out);
        if (imageno == MAXIMAGE){
            break;
        }

    }

    fclose(input);

}
