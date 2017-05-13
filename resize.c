
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
    if ((n < 1) || (n > 100))
    {
    
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
    
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    
    
    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    BITMAPFILEHEADER bf_1 = bf;
    BITMAPINFOHEADER bi_1 = bi; 
    
    bi_1.biWidth *= n;
    bi_1.biHeight *= n;
    
    int  padding= (4 - ((bi.biWidth) * sizeof(RGBTRIPLE)) % 4) % 4;

     // determine padding for scanlines
    int padding_1 = (4 - ((bi_1.biWidth) * sizeof(RGBTRIPLE)) % 4) % 4;

    bi_1.biSizeImage = (sizeof(RGBTRIPLE) * abs(bi_1.biHeight) * bi_1.biWidth) + (bi_1.biWidth * padding_1);
   
    // update file size
    bf_1.bfSize = bi_1.biSizeImage + sizeof (BITMAPFILEHEADER) + sizeof (BITMAPINFOHEADER); 


    // write outfile's BITMAPFILEHEADER
    fwrite(&bf_1,sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi_1, sizeof(BITMAPINFOHEADER), 1, outptr);
     
    int ini_pointer = bi.biWidth * sizeof(RGBTRIPLE) + padding;
    // ini_pointer indicates an offset

    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    // iterate over infile's scanlines
    {
        for(int j=0;j < n;j++)
        // for vertical copy
        {
         // iterate over pixels in scanline
            for (int k = 0; k < bi.biWidth; k++)
            {  
            // temporary storage
                RGBTRIPLE triple;

            // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // write RGB triple to outfile
                for(int l_1=0;l_1 < n;l_1++)
                {
                // for horizontal  copy
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }
            fseek(inptr,padding,SEEK_CUR);
            for(int m_1=0;m_1 < padding_1;m_1++)
        // adding padding to outifile
            {
                fputc(0x00, outptr);

            }
            fseek(inptr, -ini_pointer, SEEK_CUR); 
        // to point to the starting point and skipping padding

        }
        fseek(inptr,ini_pointer, SEEK_CUR); 
        // to point to the starting point and skipping padding of other line

    }
     
     
    

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}