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

#define SPRITE3_DATA    0x0400
#define SPRITE3_PTR     0x07FB

#define SPRITE4_DATA    0x0440
#define SPRITE4_PTR     0x07FC

/*#define SPRITE5_DATA    0x0340
#define SPRITE5_PTR     0x07F

#define SPRITE6_DATA    0x0340
#define SPRITE6_PTR     0x07F

#define SPRITE7_DATA    0x0340
#define SPRITE7_PTR     0x07F
*/

#define step_frame 10

#define DRIVER          "c64-ptvjoy.joy"

int (*SPRITE_DATA);

unsigned int x_pos;
unsigned int y_pos;

unsigned char p1_step=1;
unsigned char p_throw_step=0;
unsigned char p1_pick_step=0;
unsigned char p1_snow=0;
unsigned char p1_vec=0;
unsigned char p1_trigger=0;
unsigned char p_throw_trig=0;

unsigned char player =0;
unsigned char joystat;

static char shapes[7][4] = {
  {0,1,40,41},   //square
  {0,40,80,120}, //vertical line
  {0,1,2,3},     //horizontal line
  {0,1,41,81},   //L1
  {0,1,2,40},    //L2
  {1,41,80,81},  //L3
  {0,40,41,42}   //L4
};


void p_up()
{

  y_pos = --y_pos;

  if(p1_vec!=0){
    p1_vec=0;
    p1_trigger=step_frame;
  }

  if(p1_trigger == step_frame){

    switch( p1_step )
    {
      //Move cursor down
      case 0:
        memcpy ((void*) SPRITE_DATA, p_sprites[player][36], sizeof (p_sprites[player][36]));
        break;
      case 1:
        memcpy ((void*) SPRITE_DATA, p_sprites[player][37], sizeof (p_sprites[player][37]));
        break;
    }
    ++p1_step;
    if(p1_step>1){p1_step=0;}
  }
  ++p1_trigger;
  if(p1_trigger>step_frame){p1_trigger=0;}

}

void p_down()
{

  y_pos = ++y_pos;
  
  if(p1_vec!=1){
    p1_vec=1;
    p1_trigger=step_frame;
  }

  if(p1_trigger == step_frame){

    switch( p1_step )
    {
      //Move cursor down
      case 0:
        memcpy ((void*) SPRITE_DATA, p_sprites[player][33], sizeof (p_sprites[player][33]));
        break;
      case 1:
        memcpy ((void*) SPRITE_DATA, p_sprites[player][34], sizeof (p_sprites[player][34]));
        break;
    }
    ++p1_step;
    if(p1_step>1){p1_step=0;}
  }
  ++p1_trigger;
  if(p1_trigger>step_frame){p1_trigger=0;}

}


void p_left()
{

  x_pos = --x_pos;
  
  if(p1_vec!=2){
    p1_vec=2;
    p1_trigger=step_frame;
  }

  if(p1_trigger == step_frame){

    switch( p1_step )
    {
      case 0:
        memcpy ((void*) SPRITE_DATA, p_sprites[player][16], sizeof (p_sprites[player][16]));
        break;
      case 1:
        memcpy ((void*) SPRITE_DATA, p_sprites[player][18], sizeof (p_sprites[player][18]));
        break;
    }

    ++p1_step;
    if(p1_step>1){p1_step=0;}
  }
  ++p1_trigger;
  if(p1_trigger>step_frame){p1_trigger=0;}
}

void p_right()
{

  x_pos = ++x_pos;

  if(p1_vec!=3){
    p1_vec=3;
    p1_trigger=step_frame;
  }

  if(p1_trigger == step_frame){

    switch( p1_step )
    {
      case 0:
        memcpy ((void*) SPRITE_DATA, p_sprites[player][0], sizeof (p_sprites[player][0]));
        break;
      case 1:
        memcpy ((void*) SPRITE_DATA, p_sprites[player][2], sizeof (p_sprites[player][2]));
        break;

    }
    ++p1_step;
    if(p1_step>1){p1_step=0;}
  }
  ++p1_trigger;
  if(p1_trigger>step_frame){p1_trigger=0;}
}

void p_still()
{
  switch( p1_vec )
  {
    case 0:
      memcpy ((void*) SPRITE_DATA, p_sprites[player][35], sizeof (p_sprites[player][35]));
      break;
    case 1:
      memcpy ((void*) SPRITE_DATA, p_sprites[player][32], sizeof (p_sprites[player][32]));
      break;
    case 2:
      memcpy ((void*) SPRITE_DATA, p_sprites[player][17], sizeof (p_sprites[player][17]));
      break;
    case 3:
      memcpy ((void*) SPRITE_DATA, p_sprites[player][1], sizeof (p_sprites[player][1]));
      break;
  }
}

void p_throw()
{

  if(p1_vec!=3){
    p1_vec=3;
    p_throw_trig=20;
  }

  if(p_throw_trig == 20){
    switch( p_throw_step )
    {
      case 0:
        memcpy ((void*) SPRITE_DATA, p_sprites[player][7], sizeof (p_sprites[player][0]));
        break;
      case 1:
        memcpy ((void*) SPRITE_DATA, p_sprites[player][8], sizeof (p_sprites[player][2]));
        break;
      case 2:
        memcpy ((void*) SPRITE_DATA, p_sprites[player][9], sizeof (p_sprites[player][0]));
        break;
      case 3:
        memcpy ((void*) SPRITE_DATA, p_sprites[player][10], sizeof (p_sprites[player][2]));
        break;
      case 4:
        memcpy ((void*) SPRITE_DATA, p_sprites[player][11], sizeof (p_sprites[player][0]));
        break;
      case 5:
        memcpy ((void*) SPRITE_DATA, p_sprites[player][12], sizeof (p_sprites[player][2]));
        break;
      case 6:
        memcpy ((void*) SPRITE_DATA, p_sprites[player][13], sizeof (p_sprites[player][0]));
        break;
    }
    ++p_throw_step;
    if(p_throw_step>6)
    {
      p_throw_step=0;
      p1_snow=0;
    }
  }
  ++p_throw_trig;
  if(p_throw_trig>20){p_throw_trig=0;}
}

void p_pickup()
{
  switch( p1_pick_step )
  {
    //Move cursor down
    case 0:
      memcpy ((void*) SPRITE_DATA, p_sprites[player][3], sizeof (p_sprites[player][0]));
      break;
    case 1:
      memcpy ((void*) SPRITE_DATA, p_sprites[player][4], sizeof (p_sprites[player][2]));
      break;
    case 2:
      memcpy ((void*) SPRITE_DATA, p_sprites[player][5], sizeof (p_sprites[player][0]));
      break;
    case 3:
      memcpy ((void*) SPRITE_DATA, p_sprites[player][6], sizeof (p_sprites[player][2]));
      break;
    case 4:
      memcpy ((void*) SPRITE_DATA, p_sprites[player][5], sizeof (p_sprites[player][0]));
      break;
    case 5:
      memcpy ((void*) SPRITE_DATA, p_sprites[player][4], sizeof (p_sprites[player][2]));
      break;
    case 6:
      memcpy ((void*) SPRITE_DATA, p_sprites[player][3], sizeof (p_sprites[player][0]));
      break;
  }
  ++p1_pick_step;
  if(p1_pick_step>6)
    {
      p1_pick_step=0;
      p1_snow=1;
    }
}

void p_move()
{

  //  j2=joy_read (JOY_2);
  if(JOY_BTN_1 (joystat))
  {
    if(JOY_DOWN (joystat))
    {
      p_pickup();
    }
    else
    {
      p_throw();
    }
  }

  else if(JOY_DOWN (joystat) && JOY_LEFT (joystat))
  {
    if(y_pos < heightMax && x_pos > widthMin)
    {
        p_left();
        y_pos = ++y_pos;
    }
  }
  else if(JOY_DOWN (joystat) && JOY_RIGHT (joystat))
  {
    if(y_pos < heightMax && x_pos < widthMax)
    {
        p_right();
        y_pos = ++y_pos;
    }
  }
  else if(JOY_UP (joystat) && JOY_RIGHT (joystat))
  {
    if(y_pos > heightMin && x_pos < widthMax)
    {
        p_right();
        y_pos = --y_pos;
    }
  }
  else if(JOY_UP (joystat) && JOY_LEFT (joystat))
  {
    if(y_pos > heightMin && x_pos > widthMin)
    {
        p_left();
        y_pos = --y_pos;
    }
  }

  else if(JOY_DOWN (joystat))
  {
    if(y_pos < heightMax){
        p_down();
    }
  }

  else if(JOY_UP (joystat))
  {
    if(y_pos > heightMin){
       p_up();
    }
  }

  else if(JOY_LEFT (joystat))
  {
    if(x_pos > widthMin){
      p_left();
    }
  }
  else if(JOY_RIGHT (joystat))
  {
    if(x_pos < widthMax){
      p_right();
    }
  }
  else
  {
    p_still();
  }

  if(p_throw_step>0)
  {
    p_throw();
  }
  else if(p1_pick_step>0)
  {
    p_pickup();
  } 
}




int main( void )
{

  unsigned char sRunning =1;
  unsigned char a;

  memcpy ((void*) SPRITE0_DATA, p_sprites[0][1], sizeof (p_sprites[0][1]));
  memcpy ((void*) SPRITE1_DATA, p_sprites[1][1], sizeof (p_sprites[1][1]));
  memcpy ((void*) SPRITE2_DATA, p_sprites[2][1], sizeof (p_sprites[2][1]));
  memcpy ((void*) SPRITE3_DATA, p_sprites[3][1], sizeof (p_sprites[3][1]));


  memcpy ((void*) SPRITE4_DATA, sb_snowball, sizeof (sb_snowball));

  /* Load and install the mouse driver */
  joy_load_driver (DRIVER);
/* Set the VIC sprite pointer */
    *(unsigned char*)SPRITE0_PTR = SPRITE0_DATA / 64;
    *(unsigned char*)SPRITE1_PTR = SPRITE1_DATA / 64;
    *(unsigned char*)SPRITE2_PTR = SPRITE2_DATA / 64;
    *(unsigned char*)SPRITE3_PTR = SPRITE3_DATA / 64;

    *(unsigned char*)SPRITE4_PTR = SPRITE4_DATA / 64;

  clrscr ();
  bgcolor (1);
  bordercolor (1); 

  //joy_install (DRIVER);
  VIC.spr_mcolor = 1;
  VIC.spr_mcolor =0x3; 
  VIC.spr_mcolor =0x7;
  VIC.spr_mcolor =0xf;
       
  VIC.spr_mcolor =0x1f;

  VIC.spr_mcolor0 = COLOR_BLACK;
  VIC.spr_mcolor1 = COLOR_YELLOW;

  //Player 1 colour:
  VIC.spr0_color = COLOR_RED;
  //Player 2 colour:
  VIC.spr1_color = COLOR_BLUE;
  //Player 2 colour:
  VIC.spr2_color = COLOR_GREEN;
  //Player 2 colour:
  VIC.spr3_color = COLOR_PURPLE;


  //Snowball colour:
  VIC.spr4_color = 12;

  VIC.spr_ena=0x1;
  VIC.spr_ena=0x3;
  VIC.spr_ena=0x7;
  VIC.spr_ena=0xf;

  VIC.spr_ena=0x1f;

  VIC.spr0_x = 100;
  VIC.spr0_y = 100;

  VIC.spr1_x = 110;
  VIC.spr1_y = 110;

  VIC.spr2_x = 120;
  VIC.spr2_y = 120;

  VIC.spr3_x = 130;
  VIC.spr3_y = 130;

  VIC.spr4_x = 140;
  VIC.spr4_y = 140;

  while( sRunning )
  {

    SPRITE_DATA = SPRITE0_DATA;
    player = 0;
    x_pos = VIC.spr0_x;
    y_pos = VIC.spr0_y;
    joystat = joy_read (0);
    p_move();
    VIC.spr0_x = x_pos;
    VIC.spr0_y = y_pos;



    SPRITE_DATA = SPRITE1_DATA;
    player = 1;
    x_pos = VIC.spr1_x;
    y_pos = VIC.spr1_y;
    joystat = joy_read (1);
    p_move();
    VIC.spr1_x = x_pos;
    VIC.spr1_y = y_pos;



    SPRITE_DATA = SPRITE2_DATA;
    player = 2;
    x_pos = VIC.spr2_x;
    y_pos = VIC.spr2_y;
    joystat = joy_read (2);
    p_move();
    VIC.spr2_x = x_pos;
    VIC.spr2_y = y_pos;


    SPRITE_DATA = SPRITE3_DATA;
    player = 3;
    x_pos = VIC.spr3_x;
    y_pos = VIC.spr3_y;
    joystat = joy_read (3);
    p_move();
    VIC.spr3_x = x_pos;
    VIC.spr3_y = y_pos;



    for(a=0;a<100;a++){/*test*/};
  }

  return(0);
}

