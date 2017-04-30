/**
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n  infile outfile\n");
        return 1;
    }
    int n=atoi(argv[1]);
    if(n<=1||n>100)
    {
    
        return 0;
    }
    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    BITMAPFILEHEADER bf_1;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    BITMAPINFOHEADER bi_1;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    
    
   


     bi_1.biWidth=bi.biWidth*n;
     bi_1.biHeight=bi.biHeight*n;
     
     int padding= (4 - ((bi.biWidth) * sizeof(RGBTRIPLE)) % 4) % 4;

     // determine padding for scanlines
    int padding_1 = (4 - ((bi_1.biWidth) * sizeof(RGBTRIPLE)) % 4) % 4;

     bi_1.biSizeImage=((sizeof(RGBTRIPLE)*bi_1.biWidth)+padding_1)*abs(bi_1.biHeight);
     bf_1.bfSize=bi_1.biSizeImage  + 54;
    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf_1, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi_1, sizeof(BITMAPINFOHEADER), 1, outptr);

    
    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;
           
            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            
        
        
        for(int q=0;q<n;q++)
            {    
                 
                 fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
                    
        }        
            for (int p = 0; p < padding_1; p++)
            {
               fputc(0x00, outptr);
            }
           fseek(inptr, padding, SEEK_CUR); 
        
    }
            
            
    
    
     for(int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        
          // temporary storage
            RGBTRIPLE triple;
            RGBTRIPLE array[bi_1.biWidth];
            
         for (int j = 0; j < bi.biWidth; j++)
        {
            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            
            
            
            for(int r=0;r<n;r++)
            {
              array[j*n+r]= triple;
            }
            
            for(int m=0;m<n;m++)
            {
                 triple=array[m];
                 fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            
            }
        }
            for (int l= 0; l < padding_1; l++)
            {
               fputc(0x00, outptr);
            }
            
            
            
            fseek(inptr, padding, SEEK_CUR);
          
    }

        // skip over padding, if any
        

        // then add it back (to demonstrate how)
       
    

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
