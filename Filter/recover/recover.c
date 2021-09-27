#include <stdio.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover image/n");
        return 1;
    }

    //Open the file entered into the command line
    FILE *file = fopen(argv[1], "r");
    
    // Create pointer for the new image
    FILE *img = NULL;

    // Checks if prompted image is valid
    if (file == NULL)
    {
        return 1;
    }

    // Array for buffer & filename
    unsigned char buffer[512];
    char filename[8];

    // Counter for each created file
    int counter = 0;

    //Set flag
    bool check_newfile = false;

    //Read the file
    while (fread(buffer, 512, 1, file) == 1)
    {
        //Check if we are at the beginning of a JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //Close current JPEG, so we can start reading the next
            if (check_newfile == true)
            {
                fclose(img);
            }
            //Condition for found JPEG
            else
            {
                check_newfile = true;
            }

            sprintf(filename, "%03i.jpg", counter);
            img = fopen(filename, "w");
            counter++;
        }

        if (check_newfile == true)
        {
            fwrite(&buffer, 512, 1, img);
        }
    }

    // Close files
    fclose(file);
    fclose(img);

    // Return 0 if all correct
    return 0;

}