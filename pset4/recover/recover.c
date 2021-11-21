// Detects if a file is a JPEG

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 512

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
     // Check usage
     if (argc != 2)
     {
          return 1;
     }


     // Open file
     FILE *file = fopen(argv[1], "r");
     if (!file)
     {
          return 1;
     }

     int filecount = 0;
     BYTE bytes[BUFFER_SIZE];
     FILE *jpeg = NULL;


     while(fread(bytes, BUFFER_SIZE, 1, file) == 1){

          // Check first three bytes
          if (bytes[0] == 0xff && bytes[1] == 0xd8 && bytes[2] == 0xff && (bytes[3] & 0xf0) == 0xe0)
          {
               if (jpeg != NULL)
               {
                    fclose(jpeg);
               }
               char filename[8];
               sprintf(filename, "%03i.jpg", filecount++);

               jpeg = fopen(filename, "w");


          }

          if (jpeg != NULL)
          {
               fwrite(bytes, BUFFER_SIZE, 1, jpeg);
          }
     }


     // Close file
     fclose(file);
     fclose(jpeg);
}