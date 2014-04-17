//***********************************************************************//
//                                                                       //
//        - "Talk to me like I'm a 3 year old!" Programming Lessons -    //
//                                                                       //
//        $Author:        DigiBen        digiben@gametutorials.com       //
//                                                                       //
//        $Program:        Sky Box                                       //
//                                                                       //
//        $Description:    This shows how to create a textured SkyBox    //
//                                                                       //
//        $Date:            11/02/01                                     //
//                                                                       //
//***********************************************************************//

#include "main.h"


///////////////////////////////// CCAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This is the class constructor
/////
///////////////////////////////// CCAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

CCamera::CCamera(float positionX, float positionY, float positionZ,
                 float viewX,      float viewY,       float viewZ,
                 float upVectorX, float upVectorY, float upVectorZ)
{
    // Pass in the initialization to our Positioning Function.
    // If nothing was passed in to our constructor it will default to some standard values.
    PositionCamera(positionX, positionY, positionZ,  
                   viewX,      viewY,     viewZ,  
                   upVectorX, upVectorY, upVectorZ);
}

///////////////////////////////// POSITION CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This function sets the camera's position and view and up vVector.
/////
///////////////////////////////// POSITION CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::PositionCamera(float positionX, float positionY, float positionZ,
                             float viewX,      float viewY,       float viewZ,
                             float upVectorX, float upVectorY, float upVectorZ)
{
    CVector3 vPosition    = {positionX, positionY, positionZ};
    CVector3 vView        = {viewX,     viewY,     viewZ};
    CVector3 vUpVector    = {upVectorX, upVectorY, upVectorZ};

    m_vPosition = vPosition;                    // Assign the position
    m_vView = vView;                            // Assign the view
    m_vUpVector = vUpVector;                    // Assign the up vVector
}


///////////////////////////////// MOVE CAMERA BY MOUSE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This allows us to look around using the mouse, like in most first person games.
/////   
/////
///////////////////////////////// MOVE CAMERA BY MOUSE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::MoveCameraByMouse()
{
    int mousePos_x,mousePos_y;
    int middleX = SCREEN_WIDTH  >> 1;                // This is a binary shift to get half the width
    int middleY = SCREEN_HEIGHT >> 1;                // This is a binary shift to get half the height
    float deltaY  = 0.0f;                            // This is the direction for looking up or down
    float rotateY = 0.0f;                            // This will be the value we need to rotate around the Y axis (Left and Right)
    
    // Get the mouse's current X,Y position
    SDL_GetMouseState(&mousePos_x,&mousePos_y);

    // If our cursor is still in the middle, we never moved... so don't update the screen
    if( (mousePos_x == middleX) && (mousePos_y == middleY) ) return;

    // Set the mouse position to the middle of our window
    SDL_WarpMouse(middleX, middleY);

    // Get the direction the mouse moved in, but bring the number down to a reasonable amount
    rotateY = (float)( (middleX - mousePos_x) ) / 1000;        
    deltaY  = (float)( (middleY - mousePos_y) ) / 1000;

    // Multiply the direction vVector for Y by an acceleration (The higher the faster is goes).
    m_vView.y += deltaY * 8;

    // Check if the distance of our view exceeds 10 from our position, if so, stop it. (UP)
    if( ( m_vView.y - m_vPosition.y ) >  10)  m_vView.y = m_vPosition.y + 10;

    // Check if the distance of our view exceeds -10 from our position, if so, stop it. (DOWN)
    if( ( m_vView.y - m_vPosition.y ) < -10)  m_vView.y = m_vPosition.y - 10;

    // Here we rotate the view along the X avis depending on the direction (Left of Right)
    RotateView(0, -rotateY, 0);
}

///////////////////////////////// ROTATE VIEW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This rotates the view around the position
/////
///////////////////////////////// ROTATE VIEW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::RotateView(float X, float Y, float Z)
{
    CVector3 vVector;                            // Vector for the position/view.

    // Get our view vVector (The direciton we are facing)
    vVector.x = m_vView.x - m_vPosition.x;        // This gets the direction of the X    
    vVector.y = m_vView.y - m_vPosition.y;        // This gets the direction of the Y
    vVector.z = m_vView.z - m_vPosition.z;        // This gets the direction of the Z

    // Rotate the view along the desired axis
    if(X) {
        // Rotate the view vVector up or down, then add it to our position
        m_vView.z = (float)(m_vPosition.z + sin(X)*vVector.y + cos(X)*vVector.z);
        m_vView.y = (float)(m_vPosition.y + cos(X)*vVector.y - sin(X)*vVector.z);
    }
    if(Y) {
        // Rotate the view vVector right or left, then add it to our position
        m_vView.z = (float)(m_vPosition.z + sin(Y)*vVector.x + cos(Y)*vVector.z);
        m_vView.x = (float)(m_vPosition.x + cos(Y)*vVector.x - sin(Y)*vVector.z);
    }
    if(Z) {
        // Rotate the view vVector diagnally right or diagnally down, then add it to our position
        m_vView.x = (float)(m_vPosition.x + sin(Z)*vVector.y + cos(Z)*vVector.x);        
        m_vView.y = (float)(m_vPosition.y + cos(Z)*vVector.y - sin(Z)*vVector.x);
    }
}


///////////////////////////////// MOVE CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This will move the camera forward or backward depending on the speed
/////
///////////////////////////////// MOVE CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::MoveCamera(float speed)
{
    CVector3 vVector={0};                        // Init a vVector for our view

    // Get our view vVector (The direciton we are facing)
    vVector.x = m_vView.x - m_vPosition.x;        // This gets the direction of the X    
    vVector.y = m_vView.y - m_vPosition.y;        // This gets the direction of the Y
    vVector.z = m_vView.z - m_vPosition.z;        // This gets the direction of the Z

    m_vPosition.x += vVector.x * speed;        // Add our acceleration to our position's X
    m_vPosition.z += vVector.z * speed;        // Add our acceleration to our position's Z
    m_vView.x += vVector.x * speed;            // Add our acceleration to our view's X
    m_vView.z += vVector.z * speed;            // Add our acceleration to our view's Z
}


/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// Nothing has changed since the Camera3 tutorial, except the constructor
// was added to allow initialization of the camera in the beginning.
//
//
//
//
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
// 

