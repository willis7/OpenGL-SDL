//***********************************************************************//
//                                                                       //
//      - "Talk to me like I'm a 3 year old!" Programming Lessons -      //
//                                                                       //
//      $Author:        DigiBen     DigiBen@GameTutorials.com            //
//                                                                       //
//      $Program:       BSP Loader                                       //
//                                                                       //
//      $Description:   Loads faces and textures from a Quake3 BSP file  //
//                                                                       //
//      $Date:          5/9/02                                           //
//                                                                       //
//***********************************************************************//

#include "main.h"
#include "image.h"


# include <stdio.h>
# include <sys/types.h>
# include <jpeglib.h>
# include "SDL.h"

# include <iostream>
# include <stdlib.h>


///////////////////////////////////////      SWAP      \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////  This swaps 2 elements we pass to it (swaps bytes)
/////
//////////////////////////////////////       SWAP      \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

static void swap(unsigned char & a, unsigned char & b)
{
    unsigned char temp;

    temp = a;
    a    = b;
    b    = temp;

    return;
}

/// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

///////////////////////////////// LOAD BMP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This loads the BMP file and returns it's data in a tImage struct
/////
///////////////////////////////// LOAD BMP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

tImage *LoadBMP(const char *strFileName)
{
    SDL_Surface * pBitmap = NULL;

    FILE * pfile = NULL;

    pBitmap = SDL_LoadBMP(strFileName);                 // Load the bitmap and store data

    if( pBitmap == NULL )                               // if we couldn't load it, print an error message and return NULL
    {
        cerr << " Failed loading " << strFileName << " : " << SDL_GetError() << endl;
        return NULL;
    }
    
    // WARNING   : GO THROUGH THESE FEW LINES FOR SWAPPING ROWS ONLY IF YOU REALLY NEED TO, OR ELSE SKIP
    // THE LINES MARKED BELOW. Just take it for granted that these lines of code swap rows in the texture 
    // as required by us.
    
    // <SKIP> <SKIP> <SKIP> <SKIP>   (FROM HERE)        -------------------
    //
    // IMPORTANT : SDL loads Bitmaps differently when compared to the default windows loader. The row 0
    // corresponds to the top row and NOT the bottom row. Therefore if we don't invert the order of the rows,
    // then the textures will appear (vertically) inverted.
    // Therefore we must reverse the ordering of the rows in the data of the loaded surface ( the member
    //  'pixels' of the structure)
    
    // Rearrange the pixelData 
    int width  = pBitmap -> w;
    int height = pBitmap -> h;
    unsigned char * data = (unsigned char *) (pBitmap -> pixels);         // the pixel data

    int BytesPerPixel = (pBitmap -> format -> BytesPerPixel) ; // the number of bytes used per pixel in our bitmap

    //////////// This is how we swap the rows :
    // For half the rows, we swap row 'i' with row 'height - i -1'. (if we swap all the rows
    // like this and not the first half or the last half, then we get the same texture again !
    //
    // Now these rows are not stored as 2D arrays, instead they are stored as a long 1D array.
    // So each row is concatenated after the previous one to make this long array. Our swap 
    // function swaps one byte at a time and therefore we swap BytesPerPixel (= total bits per pixel)
    // bytes succesively.
    //
    // so the three loops below are :
    // for the first half of the rows
    //   for all the width (which is width of image * BytesPerPixel, where BytesPerPixel = total bits per pixel).
    //   (Here, for each pixel, we have to increment j by total bits per pixel (to get to next pixel to swap))
    //      for(each byte in this pixel i.e k = 0 to BytesPerPixel - 1, i.e BytesPerPixel bytes.
    //        swap the byte with the corresponding byte in the 'height -i -1'th row ('i'th row from bottom)
    for( int i = 0 ; i < (height / 2) ; ++i )
        for( int j = 0 ; j < width * BytesPerPixel; j += BytesPerPixel )
            for(int k = 0; k < BytesPerPixel; ++k)
                swap( data[ (i * width * BytesPerPixel) + j + k], data[ ( (height - i - 1) * width * BytesPerPixel ) + j + k]);

    // </SKIP> </SKIP> </SKIP> </SKIP>    (TO HERE)   -------------------
    
    tImage * pImage = (tImage *) malloc( sizeof(tImage) );
    pImage -> channels = 3;                                       // bmps have no alpha, if it uses a palette we make it RGB
    pImage -> sizeX    = width;
    pImage -> sizeY    = height;
    pImage -> data     = (unsigned char *) malloc(sizeof(unsigned char) * width * height * pImage -> channels );

    // the following lines extract R,G and B values from any bitmap

    for(int i = 0; i < (width * height); ++i)
    {
        byte r,g,b;                                                // R,G and B that we will put into pImage

        Uint32 pixel_value = 0;                                    // 32 bit unsigned int (as dictated by SDL)

        // the following loop extracts the pixel (however wide it is 8,16,24 or 32) and 
        // creates a long with all these bytes taken together.
        
        for(int j = BytesPerPixel - 1 ; j >=0; --j)                // for each byte in the pixel (from the right)
        {
            pixel_value = pixel_value << 8;                        // left shift pixel value by 8 bits
            pixel_value = pixel_value | data[ (i * BytesPerPixel) + j ];  // then make the last 8 bits of pixel value  =
        }                                                                 // the byte that we extract from pBitmap's data

        SDL_GetRGB(pixel_value, pBitmap -> format, (Uint8 *)&r, (Uint8 *)&g, (Uint8 *)&b);     // here we get r,g,b from pixel_value which is stored in the form specified by pBitmap->format

        pImage -> data[(i * pImage -> channels) + 0] = r;          // in our tImage classes we store r first
        pImage -> data[(i * pImage -> channels) + 1] = g;          // then g
        pImage -> data[(i * pImage -> channels) + 2] = b;          // and finally b (for bmps - three channels only)

        pixel_value = 0;                                           // reset pixel , else we get incorrect values of r,g,b
    }

    free(pBitmap);                                                 // free memory used by pBitmap

    return pImage;                                                 // return the image that we have just created
}

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


///////////////////////////////// LOAD TGA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This loads the TGA file and returns it's data in a tImageTGA struct
/////
///////////////////////////////// LOAD TGA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

tImage *LoadTGA(const char *strFileName)
{
    tImage *pImageData = NULL;          // This stores our important image data
    WORD width = 0, height = 0;         // The dimensions of the image
    byte length = 0;                    // The length in bytes to the pixels
    byte imageType = 0;                 // The image type (RLE, RGB, Alpha...)
    byte bits = 0;                      // The bits per pixel for the image (16, 24, 32)
    FILE *pFile = NULL;                 // The file pointer
    int channels = 0;                   // The channels of the image (3 = RGA : 4 = RGBA)
    int stride = 0;                     // The stride (channels * width)
    int i = 0;                          // A counter

    // Open a file pointer to the targa file and check if it was found and opened 
    if((pFile = fopen(strFileName, "rb")) == NULL) 
    {
        // Display an error message saying the file was not found, then return NULL
        cerr << "Unable to load TGA File!" << endl;
        return NULL;
    }
        
    // Allocate the structure that will hold our eventual image data (must free it!)
    pImageData = (tImage*)malloc(sizeof(tImage));

    // Read in the length in bytes from the header to the pixel data
    fread(&length, sizeof(byte), 1, pFile);
    
    // Jump over one byte
    fseek(pFile,1,SEEK_CUR); 

    // Read in the imageType (RLE, RGB, etc...)
    fread(&imageType, sizeof(byte), 1, pFile);
    
    // Skip past general information we don't care about
    fseek(pFile, 9, SEEK_CUR); 

    // Read the width, height and bits per pixel (16, 24 or 32)
    fread(&width,  sizeof(WORD), 1, pFile);
    fread(&height, sizeof(WORD), 1, pFile);
    fread(&bits,   sizeof(byte), 1, pFile);
    // Now we move the file pointer to the pixel data
    fseek(pFile, length + 1, SEEK_CUR); 

    // Check if the image is RLE compressed or not
    if(imageType != TGA_RLE)
    {
        // Check if the image is a 24 or 32-bit image
        if(bits == 24 || bits == 32)
        {
            // Calculate the channels (3 or 4) - (use bits >> 3 for more speed).
            // Next, we calculate the stride and allocate enough memory for the pixels.
            channels = bits / 8;
            stride = channels * width;
            pImageData->data = ((unsigned char*)malloc(sizeof(unsigned char)*stride*height));

            // Load in all the pixel data line by line
            for(int y = 0; y < height; y++)
            {
                // Store a pointer to the current line of pixels
                unsigned char *pLine = &(pImageData->data[stride * y]);

                // Read in the current line of pixels
                fread(pLine, stride, 1, pFile);
            
                // Go through all of the pixels and swap the B and R values since TGA
                // files are stored as BGR instead of RGB (or use GL_BGR_EXT verses GL_RGB)
                for(i = 0; i < stride; i += channels)
                {
                    int temp     = pLine[i];
                    pLine[i]     = pLine[i + 2];
                    pLine[i + 2] = temp;
                }
            }
        }
        // Check if the image is a 16 bit image (RGB stored in 1 unsigned short)
        else if(bits == 16)
        {
            unsigned short pixels = 0;
            int r=0, g=0, b=0;

            // Since we convert 16-bit images to 24 bit, we hardcode the channels to 3.
            // We then calculate the stride and allocate memory for the pixels.
            channels = 3;
            stride = channels * width;
            pImageData->data = ((unsigned char*)malloc(sizeof(unsigned char)*stride*height));

            // Load in all the pixel data pixel by pixel
            for(int i = 0; i < width*height; i++)
            {
                // Read in the current pixel
                fread(&pixels, sizeof(unsigned short), 1, pFile);
                
                // Convert the 16-bit pixel into an RGB
                b = (pixels & 0x1f) << 3;
                g = ((pixels >> 5) & 0x1f) << 3;
                r = ((pixels >> 10) & 0x1f) << 3;
                
                // This essentially assigns the color to our array and swaps the
                // B and R values at the same time.
                pImageData->data[i * 3 + 0] = r;
                pImageData->data[i * 3 + 1] = g;
                pImageData->data[i * 3 + 2] = b;
            }
        }   
        // Else return a NULL for a bad or unsupported pixel format
        else
            return NULL;
    }
    // Else, it must be Run-Length Encoded (RLE)
    else
    {
        // Create some variables to hold the rleID, current colors read, channels, & stride.
        byte rleID = 0;
        int colorsRead = 0;
        channels = bits / 8;
        stride = channels * width;

        // Next we want to allocate the memory for the pixels and create an array,
        // depending on the channel count, to read in for each pixel.
        pImageData->data = ((unsigned char*)malloc(sizeof(unsigned char)*stride*height));
        byte *pColors = ((byte*)malloc(sizeof(byte)*channels));

        // Load in all the pixel data
        while(i < width*height)
        {
            // Read in the current color count + 1
            fread(&rleID, sizeof(byte), 1, pFile);
            
            // Check if we don't have an encoded string of colors
            if(rleID < 128)
            {
                // Increase the count by 1
                rleID++;

                // Go through and read all the unique colors found
                while(rleID)
                {
                    // Read in the current color
                    fread(pColors, sizeof(byte) * channels, 1, pFile);

                    // Store the current pixel in our image array
                    pImageData->data[colorsRead + 0] = pColors[2];
                    pImageData->data[colorsRead + 1] = pColors[1];
                    pImageData->data[colorsRead + 2] = pColors[0];

                    // If we have a 4 channel 32-bit image, assign one more for the alpha
                    if(bits == 32)
                        pImageData->data[colorsRead + 3] = pColors[3];

                    // Increase the current pixels read, decrease the amount
                    // of pixels left, and increase the starting index for the next pixel.
                    i++;
                    rleID--;
                    colorsRead += channels;
                }
            }
            // Else, let's read in a string of the same character
            else
            {
                // Minus the 128 ID + 1 (127) to get the color count that needs to be read
                rleID -= 127;

                // Read in the current color, which is the same for a while
                fread(pColors, sizeof(byte) * channels, 1, pFile);

                // Go and read as many pixels as are the same
                while(rleID)
                {
                    // Assign the current pixel to the current index in our pixel array
                    pImageData->data[colorsRead + 0] = pColors[2];
                    pImageData->data[colorsRead + 1] = pColors[1];
                    pImageData->data[colorsRead + 2] = pColors[0];

                    // If we have a 4 channel 32-bit image, assign one more for the alpha
                    if(bits == 32)
                        pImageData->data[colorsRead + 3] = pColors[3];

                    // Increase the current pixels read, decrease the amount
                    // of pixels left, and increase the starting index for the next pixel.
                    i++;
                    rleID--;
                    colorsRead += channels;
                }
                
            }
                
        }
    }

    // Close the file pointer that opened the file
    fclose(pFile);

    // Fill in our tImage structure to pass back
    pImageData->channels = channels;
    pImageData->sizeX    = width;
    pImageData->sizeY    = height;

    // Return the TGA data (remember, you must free this data after you are done)
    return pImageData;
}




///////////////////////////////// DECODE JPG \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This decodes the jpeg and fills in the tImage structure
/////
///////////////////////////////// DECODE JPG \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void DecodeJPG(jpeg_decompress_struct* cinfo, tImage *pImageData)
{
    // Read in the header of the jpeg file
    jpeg_read_header(cinfo, TRUE);
    
    // Start to decompress the jpeg file with our compression info
    jpeg_start_decompress(cinfo);

    // Get the image dimensions and channels to read in the pixel data
    pImageData->channels = cinfo->num_components;
    pImageData->sizeX    = cinfo->image_width;
    pImageData->sizeY    = cinfo->image_height;

    // Get the row span in bytes for each row
    int rowSpan = cinfo->image_width * cinfo->num_components;
    
    // Allocate memory for the pixel buffer
    pImageData->data = ((unsigned char*)malloc(sizeof(unsigned char)*rowSpan*pImageData->sizeY));
            
    // Create an array of row pointers
    unsigned char** rowPtr = new unsigned char*[pImageData->sizeY];

    for (int i = 0; i < pImageData->sizeY; i++)
        rowPtr[i] = &(pImageData->data[i * rowSpan]);

    // Now comes the juice of our work, here we extract all the pixel data
    int rowsRead = 0;
    while (cinfo->output_scanline < cinfo->output_height) 
    {
        // Read in the current row of pixels and increase the rowsRead count
        rowsRead += jpeg_read_scanlines(cinfo, 
                                        &rowPtr[rowsRead], cinfo->output_height - rowsRead);
    }
    
    // Delete the temporary row pointers
    delete [] rowPtr;

    // Finish decompressing the data
    jpeg_finish_decompress(cinfo);
}


///////////////////////////////// LOAD JPG \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This loads the JPG file and returns it's data in a tImage struct
/////
///////////////////////////////// LOAD JPG \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

tImage *LoadJPG(const char *strFileName)
{
    struct jpeg_decompress_struct cinfo;
    tImage *pImageData = NULL;
    FILE *pFile;
        
    // Open a file pointer to the jpeg file and check if it was found and opened 
    if((pFile = fopen(strFileName, "rb")) == NULL) 
    {
        // Display an error message saying the file was not found, then return NULL
        cerr << "Unable to load JPG File!";
        return NULL;
    }
    
    // Create an error handler
    jpeg_error_mgr jerr;

    // Have our compression info object point to the error handler address
    cinfo.err = jpeg_std_error(&jerr);
    
    // Initialize the decompression object
    jpeg_create_decompress(&cinfo);
    
    // Specify the data source (Our file pointer)   
    jpeg_stdio_src(&cinfo, pFile);
    
    // Allocate the structure that will hold our eventual jpeg data (must free it!)
    pImageData = (tImage*)malloc(sizeof(tImage));

    // Decode the jpeg file and fill in the image data structure to pass back
    DecodeJPG(&cinfo, pImageData);
    
    // This releases all the stored memory for reading and decoding the jpeg
    jpeg_destroy_decompress(&cinfo);
    
    // Close the file pointer that opened the file
    fclose(pFile);

    // Return the jpeg data (remember, you must free this data after you are done)
    return pImageData;
}


/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// In this file we have all of our image loading code and made it so they
// each pass back a tImage object.  We use the glaux library to load the
// bitmap like we did before in all the previous tutorials, but convert the
// data to a tImage structure which is passed back.
//
//
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
//
