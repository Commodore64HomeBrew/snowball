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

#define SPRITE1_DATA    0x0380
#define SPRITE1_PTR     0x07F9

#define SPRITE2_DATA    0x03C0
#define SPRITE2_PTR     0x07FA

/*#define SPRITE3_DATA    0x0340
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

char p1_step=1;

static char shapes[7][4] = {
  {0,1,40,41},   //square
  {0,40,80,120}, //vertical line
  {0,1,2,3},     //horizontal line
  {0,1,41,81},   //L1
  {0,1,2,40},    //L2
  {1,41,80,81},  //L3
  {0,40,41,42}   //L4
};


void runup()
{
  printf("%d",p1_step);
  switch( p1_step )
  {
    //Move cursor down
    case 0:
      memcpy ((void*) SPRITE0_DATA, p1_run35, sizeof (p1_run0));
      VIC.spr0_x = --VIC.spr0_y;
      break;

    case 1:
      memcpy ((void*) SPRITE0_DATA, p1_run36, sizeof (p1_run1));
      VIC.spr0_x = --VIC.spr0_y;
      break;

    case 2:
      memcpy ((void*) SPRITE0_DATA, p1_run37, sizeof (p1_run2));
      VIC.spr0_x = --VIC.spr0_y;
      break;
  }
  ++p1_step;
  if(p1_step>2){p1_step=0;}
}

void rundown()
{
  printf("%d",p1_step);
  switch( p1_step )
  {
    //Move cursor down
    case 0:
      memcpy ((void*) SPRITE0_DATA, p1_run32, sizeof (p1_run0));
      VIC.spr0_x = ++VIC.spr0_y;
      break;

    case 1:
      memcpy ((void*) SPRITE0_DATA, p1_run33, sizeof (p1_run1));
      VIC.spr0_x = ++VIC.spr0_y;
      break;

    case 2:
      memcpy ((void*) SPRITE0_DATA, p1_run34, sizeof (p1_run2));
      VIC.spr0_x = ++VIC.spr0_y;
      break;
  }
  ++p1_step;
  if(p1_step>2){p1_step=0;}
}


void runleft()
{
  printf("%d",p1_step);
  switch( p1_step )
  {
    //Move cursor down
    case 0:
      memcpy ((void*) SPRITE0_DATA, p1_run16, sizeof (p1_run0));
      VIC.spr0_x = --VIC.spr0_x;
      break;

    case 1:
      memcpy ((void*) SPRITE0_DATA, p1_run17, sizeof (p1_run1));
      VIC.spr0_x = --VIC.spr0_x;
      break;

    case 2:
      memcpy ((void*) SPRITE0_DATA, p1_run18, sizeof (p1_run2));
      VIC.spr0_x = --VIC.spr0_x;
      break;
  }
  ++p1_step;
  if(p1_step>2){p1_step=0;}
}

void runright()
{
  printf("%d",p1_step);
  switch( p1_step )
  {
    //Move cursor down
    case 0:
      memcpy ((void*) SPRITE0_DATA, p1_run0, sizeof (p1_run0));
      VIC.spr0_x = ++VIC.spr0_x;
      break;

    case 1:
      memcpy ((void*) SPRITE0_DATA, p1_run1, sizeof (p1_run1));
      VIC.spr0_x = ++VIC.spr0_x;
      break;

    case 2:
      memcpy ((void*) SPRITE0_DATA, p1_run2, sizeof (p1_run2));
      VIC.spr0_x = ++VIC.spr0_x;
      break;
  }
  ++p1_step;
  if(p1_step>2){p1_step=0;}
}



int main( void )
{

  struct mouse_info info;

  char sRunning =1;
  char pet;

  memcpy ((void*) SPRITE0_DATA, p1_run1, sizeof (p1_run1));
  memcpy ((void*) SPRITE1_DATA, p2_run1, sizeof (p2_run1));

  memcpy ((void*) SPRITE2_DATA, sb_snowball, sizeof (sb_snowball));

  /* Load and install the mouse driver */
  //joy_load_driver (DRIVER);
/* Set the VIC sprite pointer */
    *(unsigned char*)SPRITE0_PTR = SPRITE0_DATA / 64;
    *(unsigned char*)SPRITE1_PTR = SPRITE1_DATA / 64;
    *(unsigned char*)SPRITE2_PTR = SPRITE2_DATA / 64;

  clrscr ();
  bgcolor (1);
  bordercolor (1); 

  //joy_install (DRIVER);
  VIC.spr_mcolor = 1;

  VIC.spr_mcolor0 = COLOR_BLACK;
  VIC.spr_mcolor1 = COLOR_YELLOW;

  //Player 1 colour:
  VIC.spr0_color = COLOR_RED;
  //Player 2 colour:
  VIC.spr1_color = COLOR_BLUE;

  //Snowball colour:
  VIC.spr2_color = 12;

  VIC.spr_ena=1;

  VIC.spr0_x = 100;
  VIC.spr0_y = 100;

  VIC.spr1_x = 200;
  VIC.spr1_y = 100;


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
            runleft();
          }
          break;

        //Move cursor right
        case PETSCII_RIGHT:
          
          if(VIC.spr0_x < screenW){
            runright();
          }
          break;
        }

      }

    //printf(VIC.rasterline);
   
  }




  return(0);
}
