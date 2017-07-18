/**
 * recover.c
 *
 * Author: Baibhav Vatsa
 * 
 * Recovers JPEGs from a forensic image.
 */
 
//don't wanna miss the headers
#include<stdio.h>
#include<stdlib.h>


//the worst way to define all possible jpeg signatures
#define JS01 0xe0ffd8ff
#define JS02 0xe1ffd8ff
#define JS03 0xe2ffd8ff
#define JS04 0xe3ffd8ff
#define JS05 0xe4ffd8ff
#define JS06 0xe5ffd8ff
#define JS07 0xe6ffd8ff
#define JS08 0xe7ffd8ff
#define JS09 0xe8ffd8ff
#define JS10 0xe9ffd8ff
#define JS11 0xeaffd8ff
#define JS12 0xebffd8ff
#define JS13 0xecffd8ff
#define JS14 0xedffd8ff
#define JS15 0xeeffd8ff
#define JS16 0xefffd8ff

//the block size of 512
#define BLOCK 512

int main(int argc, char* argv[])
{
    //opening a file
    FILE* f_in = fopen("card.raw", "r");
    
    //creating a file pointer
    FILE* f_out = NULL;
    
    //allocating space to byte, unsigned so that more values can be represented
    unsigned char* byte = malloc(sizeof(char) * BLOCK);
    
    //the filename
    char filename[8];
    
    //the variable to name the filenumbers
    int count = 0;
    
    //the loop which will execute till the end of the file
    while(fread(byte, sizeof(char), BLOCK, f_in) == BLOCK)
    {
        
        //transferring the data to buffer temporarily
        unsigned int* buffer = (unsigned int *)byte;
        
        //checking if it is among any of the jpgsignatures
        if(*buffer == JS01 || *buffer == JS02 || *buffer == JS03 || *buffer == JS04 || *buffer == JS05 || *buffer == JS06 || *buffer == JS07 || *buffer == JS08 || *buffer == JS09 || *buffer == JS10 || *buffer == JS11 || *buffer == JS12 || *buffer == JS13 || *buffer == JS14 || *buffer == JS15 || *buffer == JS16)
        {
            //voila! it is
            
            //close the old file, if any
            if (f_out != NULL)
            {
                fclose(f_out);
            }
            
            //name the new file
            sprintf(filename, "%03d.jpg", count);
            f_out = fopen(filename, "w+");
            
            //write to the new file
            fwrite(byte, sizeof(char), BLOCK, f_out);
            
            //get ready for the next file
            count++;
        }
        
        //if it doesn't match any jpg signatures and a file is already running
        else if (f_out != NULL)
        {
            //continue writing
            fwrite(byte, sizeof(char), BLOCK, f_out);
        }
        //if neither a jpg starts or nor a file is open
        else
        {
            //nothing for us here
            continue;
        }
    }
    
    //close the files and make sure that no memory leaks are possible
    fclose(f_out);
    free(byte);
    fclose(f_in);
}