/**
* 
* Copyright (c) 2017 Snowball Fight - Kevin Casteels
*
*/

//
// Includes
#include <cbm.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <mouse.h>
#include <joystick.h>
//#include "RsTypes.h"
#include <peekpoke.h>

#include "spritedata.h"

#define PETSCII_STOP  0x03
//#define PETSCII_RUN   0x83

#define PETSCII_SPACE  0x20

#define PETSCII_F1  0x85
//#define PETSCII_F2  0x89
#define PETSCII_F3  0x86
//#define PETSCII_F4  0x8a
#define PETSCII_F5  0x87
//#define PETSCII_F6  0x8b
#define PETSCII_F7  0x88
//#define PETSCII_F8  0x8c

#define PETSCII_DOWN  0x11
#define PETSCII_UP    0x91
#define PETSCII_RIGHT 0x1D
#define PETSCII_LEFT  0x9D

#define screenH 200
#define screenW 320

#define SPRITE0_DATA    0x0340
#define SPRITE0_PTR     0x07F8

/*#define SPRITE1_PTR     0x07F8

#define SPRITE2_DATA    0x0340
#define SPRITE2_PTR     0x07F8

#define SPRITE3_DATA    0x0340
#define SPRITE3_PTR     0x07F

#define SPRITE4_DATA    0x0340
#define SPRITE4_PTR     0x07F

#define SPRITE5_DATA    0x0340
#define SPRITE5_PTR     0x07F

#define SPRITE6_DATA    0x0340
#define SPRITE6_PTR     0x07F

#define SPRITE7_DATA    0x0340
#define SPRITE7_PTR     0x07F
*/
#define DRIVER          "c64-stdjoy.joy"



static char shapes[7][4] = {
  {0,1,40,41},   //square
  {0,40,80,120}, //vertical line
  {0,1,2,3},     //horizontal line
  {0,1,41,81},   //L1
  {0,1,2,40},    //L2
  {1,41,80,81},  //L3
  {0,40,41,42}   //L4
};



int main( void )
{

  struct mouse_info info;

  char sRunning =1;
  char pet;

  memcpy ((void*) SPRITE0_DATA, spr_img1, sizeof (spr_img1));



  /* Load and install the mouse driver */
  //joy_load_driver (DRIVER);
/* Set the VIC sprite pointer */
    *(unsigned char*)SPRITE0_PTR = SPRITE0_DATA / 64;

  clrscr ();
  bgcolor (1);
  bordercolor (1); 

  //joy_install (DRIVER);
  VIC.spr_mcolor = 1;

  VIC.spr_mcolor0 = COLOR_BLACK;
  VIC.spr_mcolor1 = COLOR_YELLOW;

  VIC.spr0_color = COLOR_RED;

  VIC.spr_ena=1;
  VIC.spr0_x = 20;
  VIC.spr0_y = 40;

  while( sRunning )
  {

    if( kbhit() )
    {

      //uint8_t pet = cgetc();
      pet = cgetc();

      switch( pet )
      {
        //Move cursor down
        case PETSCII_DOWN:

          if(VIC.spr0_y < screenH){
            VIC.spr0_y = ++VIC.spr0_y;
          }
          break;

        //Move cursor up
        case PETSCII_UP:

          if(VIC.spr0_y > 0){
            VIC.spr0_y = --VIC.spr0_y;
          }
          break;

        //Move cursor left
        case PETSCII_LEFT:

          if(VIC.spr0_x > 0){
            VIC.spr0_x = --VIC.spr0_x;
          }
          break;

        //Move cursor right
        case PETSCII_RIGHT:

          if(VIC.spr0_x < screenW){
            VIC.spr0_x = ++VIC.spr0_x;
          }
          break;
        }

      }

    //printf(VIC.rasterline);
   
  }




  return(0);
}
