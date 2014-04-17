#include "particle.h"
#include <math.h>
#include "SDL.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

PARTICLE particle[P_MAX]; // Our particles
TEXTURE texture[2]; // We have two textures

bool Init()
{

    // Try and load the first texture
    if(InitTexture(texture[0],"P1.bmp") == false)
        return false;

    // Try and load the second texture
    if(InitTexture(texture[1],"P2.bmp") == false)
        return false;

    // Set second textures color to a "yellowish color"
    texture[1].red = 200 + rand()%25;
    texture[1].green = 200 + rand()%25;
    texture[1].blue = 0;

    GLfloat angle = 0; // A particle's angle
    GLfloat speed = 0; // A particle's speed

    // Create all the particles
    for(int i = 0; i < P_MAX; i++)
    {
        speed = float(rand()%50 + 50); // Make a random speed

        // Init the particle with a random speed
        InitParticle(particle[i],speed,angle);

        angle += 360 / (float)P_MAX; // Increment the angle so when all the particles are
                                    // initialized they will be equally positioned in a
                                   // circular fashion
    }

    return true; // Particles and textures have been initialized
}

// Initializes a particle
void InitParticle(PARTICLE &particle, GLfloat sss, GLfloat aaa)
{
    particle.speed = sss; // Set the particle's speed
    particle.angle = aaa; // Set the particle's current angle of rotation

    // Randomly set the particles color
    particle.red = rand()%255;
    particle.green = rand()%255;
    particle.blue = rand()%255;
}

static void swap(unsigned char & a, unsigned char & b)
{
    unsigned char temp;

    temp = a;
    a    = b;
    b    = temp;

    return;
}

bool InitTexture(TEXTURE &texture, char * fileName)
{

    if( fileName == NULL )                                // if the filename is not valid can't initialize texture
        return false;


    SDL_Surface * image = NULL;                           // This will hold the image we are holding
    
    image = SDL_LoadBMP(fileName);                        // Load the image

    if(image == NULL)                                     // error check
    {
        cerr << " Failed loading " << fileName << " : " << SDL_GetError() << endl;
        Quit(0);
    }

    // Generate a texture with the associative texture ID stored in the array
    glGenTextures(1, &(texture.tID));                     // create a texture
    glBindTexture(GL_TEXTURE_2D, texture.tID); // The texture is going to be a 
                                               // 2D texture so say so

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
    int width  = image -> w;
    int height = image -> h;
    unsigned char * data = (unsigned char *) (image -> pixels);         // the pixel data

    int BytesPerPixel = (image -> format -> BytesPerPixel) ; // the number of bytes used per pixel in our bitmap

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
    

    // Specify the 2D texture map
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image->w, image->h, 0, GL_RGB, 
                 GL_UNSIGNED_BYTE, image->pixels);

    // Make our textures look purty when we apply them to "objects" that are bigger OR
    // smaller than the texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    // Free Memory
    SDL_FreeSurface(image);
    return true;
}


// Draw a particle
void DrawParticle(const PARTICLE &particle)
{
    // First draw the FIRST part of our particle

    glBindTexture(GL_TEXTURE_2D, texture[0].tID); // Bind 1st texture
    glColor3ub(particle.red,particle.green,particle.blue); // Use particle's color
    
    // Calculate the current x any y positions of the particle based on the particle's
    // current angle -- This will make the particles move in a "circular pattern"
    GLfloat xPos = sinf(particle.angle); 
    GLfloat yPos = cosf(particle.angle);
    

    // Translate to the x and y position and the #defined PDEPTH (particle depth)
    glTranslatef(xPos,yPos,PDEPTH);
    
    // Draw the first quad
    glBegin(GL_QUADS);

        glTexCoord2f(0,0);
        glVertex3f(-1, 1, 0);
            
        glTexCoord2f(1,0);
        glVertex3f(1, 1, 0);
            
        glTexCoord2f(1,1);
        glVertex3f(1, -1, 0);
            
        glTexCoord2f(0,1);
        glVertex3f(-1, -1, 0);
                
    glEnd(); // Done drawing quad

    // Draw the SECOND part of our particle

    glBindTexture(GL_TEXTURE_2D, texture[1].tID); // Bind 2nd texture
    glColor3ub(texture[1].red,texture[1].green,texture[1].blue); // Use 2nd texture's color
    
    glRotatef(particle.angle,0,0,1); // Rotate around the z-axis (depth axis)

    // Draw the second quad
    glBegin(GL_QUADS);

        glTexCoord2f(0,0);
        glVertex3f(-1, 1, 0);
            
        glTexCoord2f(1,0);
        glVertex3f(1, 1, 0);
            
        glTexCoord2f(1,1);
        glVertex3f(1, -1, 0);
            
        glTexCoord2f(0,1);
        glVertex3f(-1, -1, 0);
                
    glEnd(); // Done drawing quad

    // Translate back to where we began
    glTranslatef(-xPos,-yPos,-PDEPTH);

}
                                                                 

// Simply draws/updates all the particles (ie effectively runs the application)
void RunApp()
{
    for(int i = 0; i < P_MAX; i++)
    {
        DrawParticle(particle[i]);
        
        // Increment the particle's angle
        particle[i].angle += ANGLE_INC;
    }
}

// Free up the memory allocated by the textures
void FreeTextures()
{
    if(texture[0].tID)
        glDeleteTextures(1,&(texture[0].tID));

    if(texture[1].tID)
        glDeleteTextures(1,&(texture[1].tID));
}


/*    Extra technical stuff...

    When you run the app you'll notice that the particles spin around either getting closer
    or farther away from the center of the screen.  This is simply a byproduct of using 
    glRotatef().  Every time you call glRotatef() the "current matrix" (in our case the model
    view matrix) will get multiplied by a rotation matrix, this resulting matrix becomes the
    new model view matrix so each call to glRotatef() is changing this matrix, thus changing
    where the particles    appear on the screen.

*/
