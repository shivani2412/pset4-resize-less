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
    int k=atoi(argv[1]);
    if(k<1||k>100)
    {
        fprintf(stderr,"enter n less than 100\n");
        return 1;
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
    
    // determine padding for scanlines
    int padding = (4 - ((bi.biWidth)* sizeof(RGBTRIPLE)) % 4) % 4;


     bi_1.biwidth=bi.biwidth*n;
     bi_1.biheight=bi.biheight*n;
     
     // determine padding for scanlines
    int padding_1 = (4 - ((bi_1.biWidth) * sizeof(RGBTRIPLE)) % 4) % 4;

     bi_1.biSizeImage=((sizeof(RGBTRIPLE)*bi_1.biwidth)+padding_1)*abs(bi_1.biHeight);
     bf_1.bfsize=bi_1.biSizeImage + sizeof(BITMAPFILEHEADER) +  sizeof(BITMAPINFOHEADER);
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
    for (int i = 0, biHeight = abs(bi_1.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi_1.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            
        }
     for (int i = 0, biHeight = abs(bi_1.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi_1.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            for(int i=0;i<n;i++)
            {
                 fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
                    
                    
            for (int k = 0; k < padding_1; k++)
            {
               fputc(0x00, outptr);
            }
            
            fseek(inptr, padding, SEEK_CUR);
        }    
        

        // skip over padding, if any
        

        // then add it back (to demonstrate how)
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
