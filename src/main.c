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


#define heightMin 50
#define heightMax 230
#define widthMin 24 
#define widthMax 254

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

unsigned char p1_step=1;
unsigned char p1_throw_step=0;
unsigned char p1_pick_step=0;
unsigned char p1_snow=0;
unsigned char p1_vec=0;

static char shapes[7][4] = {
  {0,1,40,41},   //square
  {0,40,80,120}, //vertical line
  {0,1,2,3},     //horizontal line
  {0,1,41,81},   //L1
  {0,1,2,40},    //L2
  {1,41,80,81},  //L3
  {0,40,41,42}   //L4
};


void p1_up()
{
  switch( p1_step )
  {
    //Move cursor down
    case 0:
      memcpy ((void*) SPRITE0_DATA, p1_run36, sizeof (p1_run36));
      break;
    case 1:
      memcpy ((void*) SPRITE0_DATA, p1_run37, sizeof (p1_run37));
      break;
  }
  VIC.spr0_y = --VIC.spr0_y;
  ++p1_step;
  if(p1_step>1){p1_step=0;}
  p1_vec=0;
}

void p1_down()
{
  switch( p1_step )
  {
    //Move cursor down
    case 0:
      memcpy ((void*) SPRITE0_DATA, p1_run33, sizeof (p1_run33));
      break;
    case 1:
      memcpy ((void*) SPRITE0_DATA, p1_run34, sizeof (p1_run34));
      break;
  }
  VIC.spr0_y = ++VIC.spr0_y;
  ++p1_step;
  if(p1_step>1){p1_step=0;}
  p1_vec=1;
}


void p1_left()
{
  switch( p1_step )
  {
    //Move cursor down
    case 0:
      memcpy ((void*) SPRITE0_DATA, p1_run16, sizeof (p1_run16));
      break;
    case 1:
      memcpy ((void*) SPRITE0_DATA, p1_run18, sizeof (p1_run18));
      break;
  }
  VIC.spr0_x = --VIC.spr0_x;
  ++p1_step;
  if(p1_step>1){p1_step=0;}
  p1_vec=2;

}

void p1_right()
{
  switch( p1_step )
  {
    //Move cursor down
    case 0:
      memcpy ((void*) SPRITE0_DATA, p1_run0, sizeof (p1_run0));
      break;
    case 1:
      memcpy ((void*) SPRITE0_DATA, p1_run2, sizeof (p1_run2));
      break;

  }
  VIC.spr0_x = ++VIC.spr0_x;
  ++p1_step;
  if(p1_step>1){p1_step=0;}
  p1_vec=3;
}

void p1_still()
{
  switch( p1_vec )
  {
    case 0:
      memcpy ((void*) SPRITE0_DATA, p1_run35, sizeof (p1_run35));
      break;
    case 1:
      memcpy ((void*) SPRITE0_DATA, p1_run32, sizeof (p1_run32));
      break;
    case 2:
      memcpy ((void*) SPRITE0_DATA, p1_run17, sizeof (p1_run17));
      break;
    case 3:
      memcpy ((void*) SPRITE0_DATA, p1_run1, sizeof (p1_run1));
      break;
  }
}

void p1_throw()
{
  switch( p1_throw_step )
  {
    //Move cursor down
    case 0:
      memcpy ((void*) SPRITE0_DATA, p1_img7, sizeof (p1_run0));
      break;
    case 1:
      memcpy ((void*) SPRITE0_DATA, p1_img8, sizeof (p1_run2));
      break;
    case 2:
      memcpy ((void*) SPRITE0_DATA, p1_img9, sizeof (p1_run0));
      break;
    case 3:
      memcpy ((void*) SPRITE0_DATA, p1_img10, sizeof (p1_run2));
      break;
    case 4:
      memcpy ((void*) SPRITE0_DATA, p1_img11, sizeof (p1_run0));
      break;
    case 5:
      memcpy ((void*) SPRITE0_DATA, p1_img12, sizeof (p1_run2));
      break;
    case 6:
      memcpy ((void*) SPRITE0_DATA, p1_img13, sizeof (p1_run0));
      break;
  }
  ++p1_throw_step;
  if(p1_throw_step>6)
    {
      p1_throw_step=0;
      p1_snow=0;
    }
}

void p1_pickup()
{
  switch( p1_pick_step )
  {
    //Move cursor down
    case 0:
      memcpy ((void*) SPRITE0_DATA, p1_img3, sizeof (p1_run0));
      break;
    case 1:
      memcpy ((void*) SPRITE0_DATA, p1_img4, sizeof (p1_run2));
      break;
    case 2:
      memcpy ((void*) SPRITE0_DATA, p1_img5, sizeof (p1_run0));
      break;
    case 3:
      memcpy ((void*) SPRITE0_DATA, p1_img6, sizeof (p1_run2));
      break;
    case 4:
      memcpy ((void*) SPRITE0_DATA, p1_img5, sizeof (p1_run0));
      break;
    case 5:
      memcpy ((void*) SPRITE0_DATA, p1_img4, sizeof (p1_run2));
      break;
    case 6:
      memcpy ((void*) SPRITE0_DATA, p1_img3, sizeof (p1_run0));
      break;
  }
  ++p1_pick_step;
  if(p1_pick_step>6)
    {
      p1_pick_step=0;
      p1_snow=1;
    }
}

int main( void )
{
  unsigned char j1;
  unsigned char j2;

  unsigned char sRunning =1;
  unsigned char a;

  memcpy ((void*) SPRITE0_DATA, p1_run1, sizeof (p1_run1));
  memcpy ((void*) SPRITE1_DATA, p2_run1, sizeof (p2_run1));

  memcpy ((void*) SPRITE2_DATA, sb_snowball, sizeof (sb_snowball));

  /* Load and install the mouse driver */
  joy_load_driver (DRIVER);
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
  VIC.spr_ena=2;
  
  VIC.spr0_x = 100;
  VIC.spr0_y = 100;

  VIC.spr1_x = 110;
  VIC.spr1_y = 110;

  VIC.spr1_x = 120;
  VIC.spr1_y = 120;

  j1=joy_read (JOY_1);
  j2=joy_read (JOY_2);

  while( sRunning )
  {

    j1 = joy_read (JOY_1);

      if(JOY_BTN_1 (j1))
      {
        if(JOY_DOWN (j1) && p1_snow==0)
        {
          p1_pickup();
        }
        else if(p1_throw_step==0 && p1_snow==1)
        {
          p1_throw();
        }
      }
    
      else if(JOY_DOWN (j1) && JOY_LEFT (j1))
      {
        if(VIC.spr0_y < heightMax && VIC.spr0_x > widthMin)
        {
            p1_left();
            VIC.spr0_y = ++VIC.spr0_y;
        }
      }
      else if(JOY_DOWN (j1) && JOY_RIGHT (j1))
      {
        if(VIC.spr0_y < heightMax && VIC.spr0_x < widthMax)
        {
            p1_right();
            VIC.spr0_y = ++VIC.spr0_y;
        }
      }
      else if(JOY_UP (j1) && JOY_RIGHT (j1))
      {
        if(VIC.spr0_y > heightMin && VIC.spr0_x < widthMax)
        {
            p1_right();
            VIC.spr0_y = --VIC.spr0_y;
        }
      }
      else if(JOY_UP (j1) && JOY_LEFT (j1))
      {
        if(VIC.spr0_y > heightMin && VIC.spr0_x > widthMin)
        {
            p1_left();
            VIC.spr0_y = --VIC.spr0_y;
        }
      }

      else if(JOY_DOWN (j1))
      {
        if(VIC.spr0_y < heightMax){
            p1_down();
        }
      }

      else if(JOY_UP (j1))
      {
        if(VIC.spr0_y > heightMin){
           p1_up();
        }
      }

      else if(JOY_LEFT (j1))
      {
        if(VIC.spr0_x > widthMin){
          p1_left();
        }
      }
      else if(JOY_RIGHT (j1))
      {
        if(VIC.spr0_x < widthMax){
          p1_right();
        }
      }
      else
      {
        p1_still();
      }

      if(p1_throw_step>0)
      {
        p1_throw();
      }

      if(p1_pick_step>0)
      {
        p1_pickup();
      }

   }
  




  return(0);
}
