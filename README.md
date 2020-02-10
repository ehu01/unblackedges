* COMP40: Machine Structure and Assembly-Language Programming
* Homework 2: Unblackedges - using an interface we created to remove black edges
*
* By: Emily Hu
* Date: 7 December 2019
*
* README

COMPILE/RUN:

    - Compile/link with:
        make unblackedges
    - Run executable with:
        ./unblackedges [Portable Bitmap file]


PURPOSE:

The goal of this assignment was first to create our own interface, Bit2, and then use it 
to create a program which removes the black edges from a given image. 


FILES:

bit2.h 
    - Interface of Bit2, a two-dimensional bit array. 
    
bit2.c
    - Implementation of Bit2. Uses David Hanson's 'bit' abstract data type.
    
unblackedges.c
    - Removes black edge pixels from a given Portable Bitmap (PBM) using the Bit2 interface.
    
    
TESTING:

Testing was done by providing both PBM images and Plain PBM images of different sizes. Edge
case testing was done by providing Plain PBM images with no dimensions, images with incorrect
dimension specifications, and images with black pixels in all sorts of configurations.
  
  
NOTES:

Unblackedges conducts a breadth-first search of the Bit2 structure from each black edge
pixel. A black edge pixel is defined as any black pixel which is either in row 0 or row h-1 or
in column 0 or column w-1 (for an image with height h and width w) OR a black pixel which 
neighbors another black pixel. 

