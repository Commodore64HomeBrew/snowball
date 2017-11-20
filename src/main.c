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
#include "vickers.h"

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

// ** My VIC Memory Map ** // 
// 0x0000 - VRAM Map (1st half only) // 
// 0x0400 - (unused) 
// 0x0800 - VRAM Tileset 
// 0x1000 - ROM Upper Case 
// 0x1800 - ROM Lower Case 
// 0x2000 - Sprites (128 total, 63+1 bytes each) 
// 0x2800 - Sprites 
// 0x3000 - Sprites 
// 0x3800 - Sprites 

//#define VICBase        (0x8000) 
//#define ScreenBase    (VICBase+(0x0)) 
//#define CharBase        (VICBase+(0x800)) 
//#define SpriteBase    (VICBase+(0x2000)) 




#define SPRITE0_DATA    0x0340
//#define SPRITE0_DATA    (MySpriteBase)
#define SPRITE0_PTR     0x07F8

#define SPRITE1_DATA    0x0380
//#define SPRITE1_DATA    (MySpriteBase+(0x80))
#define SPRITE1_PTR     0x07F9

#define SPRITE2_DATA    0x03C0
//#define SPRITE2_DATA    (MySpriteBase+(0xC0))
#define SPRITE2_PTR     0x07FA

#define SPRITE3_DATA    0x0400
//#define SPRITE3_DATA    (MySpriteBase+(0x100))
#define SPRITE3_PTR     0x07FB

#define SPRITE4_DATA    0x0440
//#define SPRITE4_DATA    (MySpriteBase+(0x140))
#define SPRITE4_PTR     0x07FC

#define SPRITE5_DATA    0x0480
#define SPRITE5_PTR     0x07FD

#define SPRITE6_DATA    0x04C0
#define SPRITE6_PTR     0x07FE

#define SPRITE7_DATA    0x0500
#define SPRITE7_PTR     0x07FF

//#define SPRITE0_DATA    0x0801
//#define SPRITE1_DATA    0x0841
//#define SPRITE2_DATA    0x0881
//#define SPRITE3_DATA    0x08C1
//#define SPRITE4_DATA    0x0901


#define step_frame 10
#define b_speed 2
#define fall_limit 100

#define DRIVER          "c64-ptvjoy.joy"

extern char SpriteColor[2]; 
extern char SpriteData[]; 

int (*SPRITE_DATA);
int (*BALL_DATA);


unsigned char joystat;


unsigned char p0_hit=0;
unsigned char p1_hit=0;
unsigned char p2_hit=0;
unsigned char p3_hit=0;

struct p_state {
  unsigned char player;
  unsigned char p_step;
  unsigned char p_throw_step;
  unsigned char p_pick_step;
  unsigned char p_snow;
  unsigned char p_vec;
  unsigned char p_trigger;
  unsigned char p_hit;
  unsigned char p_fall_time;
  unsigned char p_action;
  unsigned char (*x_pos);
  unsigned char (*y_pos);
  unsigned char (*x_ball);
  unsigned char (*y_ball);
  unsigned char b_vec;
  unsigned char b_active;
};



extern char Music[]; 

extern void SIDInit(); 
extern void SIDStep(); 
/* 
//#define SIDPlay() { 
void SIDInit( const char Song ) { 
    __asm__("lda #0"); 
    __asm__("tax"); 
    __asm__("tay"); 
    __asm__("jsr #$1000"); 
} 
//_Music+$00");//,Music+0x7C+2);//$400");//%w", 0x40D);//0x1000); 
//} 

void SIDStep() { 
    __asm__("jsr #$1012");//, 0x1013);// 0x1006); 
}*/ 



static char shapes[7][4] = {
  {0,1,40,41},   //square
  {0,40,80,120}, //vertical line
  {0,1,2,3},     //horizontal line
  {0,1,41,81},   //L1
  {0,1,2,40},    //L2
  {1,41,80,81},  //L3
  {0,40,41,42}   //L4
};

/*bool isSet(unsigned char b, unsigned char n)
{ 
  return b & ( 1 << n);
}
*/
void b_move( struct p_state *s )
{

   unsigned char byte;
  //memcpy ((void*) SPRITE4_DATA, sb_snowball, 64);
 
  if(s->b_active==3){
    *(s->x_ball) = 1;
    *(s->y_ball) = 1;
    s->b_active=0;
    memcpy ((void*) BALL_DATA, sb_snowball, 64);
    return;
  }
  else if(s->b_active==1){
    *(s->x_ball) = *(s->x_pos);
    *(s->y_ball) = *(s->y_pos);
    s->b_active=2;
  }
  else if(s->b_active==2)
  {
    if(s->b_vec==2){*(s->x_ball)=*(s->x_ball)-b_speed;}
    else if(s->b_vec==3){*(s->x_ball)=*(s->x_ball)+b_speed;}

    if(*(s->x_ball)<5){
      *(s->x_ball)=1;
      *(s->y_ball)=1;
      s->b_active=0;
    }
  }

  //Check the value of (1 << bit) & byte. If it is nonzero, the bit is set.
  byte=VIC.spr_coll;

  if(byte & (1<<(s->player+4)))
  {

    if(!(byte & (1<<s->player)))
    {
      memcpy ((void*) BALL_DATA, sb_explode, 64);
      s->b_active=3;

      if (byte & (1<<0)){p0_hit=s->b_vec;}
      if (byte & (1<<1)){p1_hit=s->b_vec;}
      if (byte & (1<<2)){p2_hit=s->b_vec;}
      if (byte & (1<<3)){p3_hit=s->b_vec;}
    }
  }
}

void p_up( struct p_state *s )
{

  *(s->y_pos) = --*(s->y_pos);

  if(s->p_vec!=0){
    s->p_vec=0;
    s->p_trigger=step_frame;
  }

  if(s->p_trigger == step_frame){

    switch( s->p_step )
    {
      //Move cursor down
      case 0:
        memcpy ((void*) SPRITE_DATA, p_sprites[s->player][36], 64);
        break;
      case 1:
        memcpy ((void*) SPRITE_DATA, p_sprites[s->player][37], 64);
        break;
    }
    ++s->p_step;
    if(s->p_step>1){s->p_step=0;}
  }
  ++s->p_trigger;
  if(s->p_trigger>step_frame){s->p_trigger=0;}

}

void p_down( struct p_state *s )
{

  *(s->y_pos) = ++*(s->y_pos);
  
  if(s->p_vec!=1){
    s->p_vec=1;
    s->p_trigger=step_frame;
  }

  if(s->p_trigger == step_frame){

    switch( s->p_step )
    {
      //Move cursor down
      case 0:
        memcpy ((void*) SPRITE_DATA, p_sprites[s->player][33], 64);
        break;
      case 1:
        memcpy ((void*) SPRITE_DATA, p_sprites[s->player][34], 64);
        break;
    }
    ++s->p_step;
    if(s->p_step>1){s->p_step=0;}
  }
  ++s->p_trigger;
  if(s->p_trigger>step_frame){s->p_trigger=0;}

}


void p_left( struct p_state *s )
{

  *(s->x_pos) = --*(s->x_pos);
  
  if(s->p_vec!=2){
    s->p_vec=2;
    s->p_trigger=step_frame;
  }

  if(s->p_trigger == step_frame){

    switch( s->p_step )
    {
      case 0:
        memcpy ((void*) SPRITE_DATA, p_sprites[s->player][16], 64);
        break;
      case 1:
        memcpy ((void*) SPRITE_DATA, p_sprites[s->player][18], 64);
        break;
    }

    ++s->p_step;
    if(s->p_step>1){s->p_step=0;}
  }
  ++s->p_trigger;
  if(s->p_trigger>step_frame){s->p_trigger=0;}
}

void p_right( struct p_state *s )
{

  *(s->x_pos) = ++*(s->x_pos);

  if(s->p_vec!=3){
    s->p_vec=3;
    s->p_trigger=step_frame;
  }

  if(s->p_trigger == step_frame){

    switch( s->p_step )
    {
      case 0:
        memcpy ((void*) SPRITE_DATA, p_sprites[s->player][0], 64);
        break;
      case 1:
        memcpy ((void*) SPRITE_DATA, p_sprites[s->player][2], 64);
        break;

    }
    ++s->p_step;
    if(s->p_step>1){s->p_step=0;}
  }
  ++s->p_trigger;
  if(s->p_trigger>step_frame){s->p_trigger=0;}
}

void p_still( struct p_state *s )
{
  if(s->p_trigger<=step_frame)
  {
    switch( s->p_vec )
    {
      case 0:
        memcpy ((void*) SPRITE_DATA, p_sprites[s->player][35], 64);
        break;
      case 1:
        memcpy ((void*) SPRITE_DATA, p_sprites[s->player][32], 64);
        break;
      case 2:
        memcpy ((void*) SPRITE_DATA, p_sprites[s->player][17], 64);
        break;
      case 3:
        memcpy ((void*) SPRITE_DATA, p_sprites[s->player][1], 64);
        break;
    }
  }
  else
  {
    s->p_trigger=step_frame+1;
  }
}

void p_fall( struct p_state *s )
{
  if(s->p_trigger<=step_frame)
  {
    switch( s->p_hit )
    {
      case 0:
        memcpy ((void*) SPRITE_DATA, p_sprites[s->player][15], 64);
        break;
      case 1:
        memcpy ((void*) SPRITE_DATA, p_sprites[s->player][31], 64);
        break;
      case 2:
        memcpy ((void*) SPRITE_DATA, p_sprites[s->player][15], 64);
        break;
      case 3:
        memcpy ((void*) SPRITE_DATA, p_sprites[s->player][31], 64);
        break;
    }
  }
  else
  {
    s->p_trigger=step_frame+1;
  }
  if(s->p_fall_time>fall_limit)
  {
    s->p_fall_time=0;
    s->p_hit=0;
  }
  else
  {
    ++s->p_fall_time;
  }
}

void p_throw_left( struct p_state *s )
{

  if(s->p_throw_step<7)
  {
    if(s->p_trigger == 0)
    {
      switch( s->p_throw_step )
      {
        case 0:
          memcpy ((void*) SPRITE_DATA, p_sprites[s->player][23], 64);
          break;
        case 1:
          memcpy ((void*) SPRITE_DATA, p_sprites[s->player][24], 64);
          break;
        case 2:
          memcpy ((void*) SPRITE_DATA, p_sprites[s->player][25], 64);
          break;
        case 3:
          memcpy ((void*) SPRITE_DATA, p_sprites[s->player][26], 64);
          break;
        case 4:
          memcpy ((void*) SPRITE_DATA, p_sprites[s->player][27], 64);
          break;
        case 5:
          memcpy ((void*) SPRITE_DATA, p_sprites[s->player][28], 64);
          if(s->p_snow==1)
          {
            s->b_active=1;
            s->b_vec=2;
            b_move(s);
            //s->p_snow=0;
            s->p_snow=1;
          }
          break;
        case 6:
          memcpy ((void*) SPRITE_DATA, p_sprites[s->player][29], 64);
          break;
      }
      ++s->p_throw_step;
    }
  }
  else
  {
    s->p_throw_step=0;
    //s->p_snow=0;
    s->p_snow=1;
  }
  ++s->p_trigger;
  if(s->p_trigger>step_frame){s->p_trigger=0;}
}

void p_throw_right( struct p_state *s )
{

  if(s->p_throw_step<7)
  {
    if(s->p_trigger == 0)
    {
      switch( s->p_throw_step )
      {
        case 0:
          memcpy ((void*) SPRITE_DATA, p_sprites[s->player][7], 64);
          break;
        case 1:
          memcpy ((void*) SPRITE_DATA, p_sprites[s->player][8], 64);
          break;
        case 2:
          memcpy ((void*) SPRITE_DATA, p_sprites[s->player][9], 64);
          break;
        case 3:
          memcpy ((void*) SPRITE_DATA, p_sprites[s->player][10], 64);
          break;
        case 4:
          memcpy ((void*) SPRITE_DATA, p_sprites[s->player][11], 64);
          break;
        case 5:
          memcpy ((void*) SPRITE_DATA, p_sprites[s->player][12], 64);
          if(s->p_snow==1)
          {
            s->b_active=1;
            s->b_vec=3;
            b_move(s);
            //s->p_snow=0;
            s->p_snow=1;
          }
          break;
        case 6:
          memcpy ((void*) SPRITE_DATA, p_sprites[s->player][13], 64);
          break;
      }
      ++s->p_throw_step;
    }
  }
  else
  {
    s->p_throw_step=0;
    //s->p_snow=0;
    s->p_snow=1;
  }
  ++s->p_trigger;
  if(s->p_trigger>step_frame){s->p_trigger=0;}
}

void p_pickup_right( struct p_state *s )
{
  switch( s->p_pick_step )
  {
    //Move cursor down
    case 0:
      memcpy ((void*) SPRITE_DATA, p_sprites[s->player][3], 64);
      break;
    case 1:
      memcpy ((void*) SPRITE_DATA, p_sprites[s->player][4], 64);
      break;
    case 2:
      memcpy ((void*) SPRITE_DATA, p_sprites[s->player][5], 64);
      break;
    case 3:
      memcpy ((void*) SPRITE_DATA, p_sprites[s->player][6], 64);
      break;
    case 4:
      memcpy ((void*) SPRITE_DATA, p_sprites[s->player][5], 64);
      break;
    case 5:
      memcpy ((void*) SPRITE_DATA, p_sprites[s->player][4], 64);
      break;
    case 6:
      memcpy ((void*) SPRITE_DATA, p_sprites[s->player][3], 64);
      break;
  }
  ++s->p_pick_step;
  if(s->p_pick_step>6)
    {
      s->p_pick_step=0;
      s->p_snow=1;
    }
}

void p_pickup_left( struct p_state *s )
{
  switch( s->p_pick_step )
  {
    //Move cursor down
    case 0:
      memcpy ((void*) SPRITE_DATA, p_sprites[s->player][19], 64);
      break;
    case 1:
      memcpy ((void*) SPRITE_DATA, p_sprites[s->player][20], 64);
      break;
    case 2:
      memcpy ((void*) SPRITE_DATA, p_sprites[s->player][21], 64);
      break;
    case 3:
      memcpy ((void*) SPRITE_DATA, p_sprites[s->player][22], 64);
      break;
    case 4:
      memcpy ((void*) SPRITE_DATA, p_sprites[s->player][21], 64);
      break;
    case 5:
      memcpy ((void*) SPRITE_DATA, p_sprites[s->player][20], 64);
      break;
    case 6:
      memcpy ((void*) SPRITE_DATA, p_sprites[s->player][19], 64);
      break;
  }
  ++s->p_pick_step;
  if(s->p_pick_step>6)
    {
      s->p_pick_step=0;
      s->p_snow=1;
    }
}


void p_move( struct p_state *s ){

  if(s->b_active>1){
    b_move(s);
  }

  if(s->p_action>0)
  {
    if(s->p_hit>0)
    {
      p_fall(s);
      return;
    }
    else if(s->p_pick_step>0)
    {
      if(s->p_vec==2){p_pickup_left(s);}
      else if(s->p_vec==3){p_pickup_right(s);}
      return;
    }
    else if(s->p_throw_step>0)
    {
      if(s->p_vec==2){p_throw_left(s);}
      else if(s->p_vec==3){p_throw_right(s);}
      return;
    }
  }

  joystat = joy_read (s->player);
  
  if(JOY_BTN_1 (joystat))
  {
    if(JOY_DOWN (joystat))
    {
      if(s->p_vec==2){p_pickup_left(s);}
      else if(s->p_vec==3){p_pickup_right(s);}
    }
    else
    {
      s->p_trigger=0;
      if(s->p_vec==2){
        p_throw_left(s);
      }
      else if(s->p_vec==3){
        p_throw_right(s);
      }
    }
  }

  else if(JOY_DOWN (joystat))
  {
    if(JOY_LEFT (joystat))
    {
      if(*(s->y_pos) < heightMax && *(s->x_pos) > widthMin)
      {
          p_left(s);
          *(s->y_pos) = ++*(s->y_pos);
      }
    }
    else if(JOY_RIGHT (joystat))
    {
      if(*(s->y_pos) < heightMax && *(s->x_pos) < widthMax)
      {
          p_right(s);
          *(s->y_pos) = ++*(s->y_pos);
      }
    }
    else if(*(s->y_pos) < heightMax){
        p_down(s);
    }
  }

  else if(JOY_UP (joystat))
  {
    if(JOY_RIGHT (joystat))
    {
      if(*(s->y_pos) > heightMin && *(s->x_pos) < widthMax)
      {
          p_right(s);
          *(s->y_pos) = --*(s->y_pos);
      }
    }
    else if(JOY_LEFT (joystat))
    {
      if(*(s->y_pos) > heightMin && *(s->x_pos) > widthMin)
      {
          p_left(s);
          *(s->y_pos) = --*(s->y_pos);
      }
    }
    else if(*(s->y_pos) > heightMin){
       p_up(s);
    }
  }
  else if(JOY_LEFT (joystat))
  {
    if(*(s->x_pos) > widthMin){
      p_left(s);
    }
  }
  else if(JOY_RIGHT (joystat))
  {
    if(*(s->x_pos) < widthMax){
      p_right(s);
    }
  }
  else
  {
    p_still(s);
  }


}




int main( void )
{



  unsigned char sRunning =1;
/*
  unsigned char p0_step, p0_throw_step, p0_pick_step, p0_snow, p0_vec, p0_trigger, p0_throw_trig, p0_fall_time;
  unsigned char p1_step, p1_throw_step, p1_pick_step, p1_snow, p1_vec, p1_trigger, p1_throw_trig, p1_fall_time;
  unsigned char p2_step, p2_throw_step, p2_pick_step, p2_snow, p2_vec, p2_trigger, p2_throw_trig, p2_fall_time;
  unsigned char p3_step, p3_throw_step, p3_pick_step, p3_snow, p3_vec, p3_trigger, p3_throw_trig, p3_fall_time;
  
  unsigned char b0_active,b1_active,b2_active,b3_active;
  unsigned char b0_vec,b1_vec,b2_vec,b3_vec;
*/
  struct p_state p0;
  struct p_state p1;
  struct p_state p2;
  struct p_state p3;

  p0.player=0;
  p0.p_action=0;
  p0.p_vec=3;
  p0.x_pos = &VIC.spr0_x;
  p0.y_pos = &VIC.spr0_y;
  p0.x_ball = &VIC.spr4_x;
  p0.y_ball = &VIC.spr4_y;

  p1.player=1;
  p1.p_action=0;
  p1.p_vec=2;
  p1.x_pos = &VIC.spr1_x;
  p1.y_pos = &VIC.spr1_y;
  p1.x_ball = &VIC.spr5_x;
  p1.y_ball = &VIC.spr5_y;

  p2.player=2;
  p2.p_action=0;
  p2.p_vec=3;
  p2.x_pos = &VIC.spr2_x;
  p2.y_pos = &VIC.spr2_y;
  p2.x_ball = &VIC.spr6_x;
  p2.y_ball = &VIC.spr6_y;

  p3.player=3;
  p3.p_action=0;
  p3.p_vec=2;
  p3.x_pos = &VIC.spr3_x;
  p3.y_pos = &VIC.spr3_y;
  p3.x_ball = &VIC.spr7_x;
  p3.y_ball = &VIC.spr7_y;


  //memset((void*)MyScreenBase,0,1024); 
  //memset((void*)0xd800,1,1024);

  //VICSetAddr( 0, 2 );    // Fun Mode (shows neat garbage) // 

  //memcpy((void*)(0x4000),(void*)&Music,1024*8); 

  //SIDInit(); 

  //memcpy((void*)(MySpriteBase),(void*)&SpriteData,64); 

  //SprInit();

  //VICSetPage( 2 ); 
  //VICSetAddr( 0, 1 ); 
  //SprInit();




  /* Load and install the mouse driver */
  joy_load_driver (DRIVER);
/* Set the VIC sprite pointer */
    *(unsigned char*)SPRITE0_PTR = SPRITE0_DATA / 64;
    *(unsigned char*)SPRITE1_PTR = SPRITE1_DATA / 64;
    *(unsigned char*)SPRITE2_PTR = SPRITE2_DATA / 64;
    *(unsigned char*)SPRITE3_PTR = SPRITE3_DATA / 64;

    *(unsigned char*)SPRITE4_PTR = SPRITE4_DATA / 64;
    *(unsigned char*)SPRITE5_PTR = SPRITE5_DATA / 64;
    *(unsigned char*)SPRITE6_PTR = SPRITE6_DATA / 64;
    *(unsigned char*)SPRITE7_PTR = SPRITE7_DATA / 64;

  clrscr ();
  bgcolor (1);
  bordercolor (1);


  VIC.bgcolor[0] = 1;
  VIC.bgcolor[1] = 1;
  VIC.bgcolor[2] = 1;
  VIC.bgcolor[3] = 1;


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
  VIC.spr5_color = 12;
  VIC.spr6_color = 12;
  VIC.spr7_color = 12;


  VIC.spr_mcolor = Spr_EnableBit[0];
  VIC.spr_mcolor = Spr_EnableBit[1];
  VIC.spr_mcolor = Spr_EnableBit[2];
  VIC.spr_mcolor = Spr_EnableBit[3];

  VIC.spr_mcolor = Spr_EnableBit[4];
  VIC.spr_mcolor = Spr_EnableBit[5];
  VIC.spr_mcolor = Spr_EnableBit[6];
  VIC.spr_mcolor = Spr_EnableBit[8];

  VIC.spr_ena = Spr_EnableBit[0];
  VIC.spr_ena = Spr_EnableBit[1];
  VIC.spr_ena = Spr_EnableBit[2];
  VIC.spr_ena = Spr_EnableBit[3];

  VIC.spr_ena = Spr_EnableBit[4];
  VIC.spr_ena = Spr_EnableBit[5];
  VIC.spr_ena = Spr_EnableBit[6];
  VIC.spr_ena = Spr_EnableBit[8];

  VIC.spr0_x = widthMin;
  VIC.spr0_y = heightMin;

  VIC.spr1_x = widthMax;
  VIC.spr1_y = heightMin;

  VIC.spr2_x = widthMin;
  VIC.spr2_y = heightMax;

  VIC.spr3_x = widthMax;
  VIC.spr3_y = heightMax;

  VIC.spr4_x = 1;
  VIC.spr4_y = 1;

  VIC.spr5_x = 1;
  VIC.spr5_y = 1;

  VIC.spr6_x = 1;
  VIC.spr6_y = 1;

  VIC.spr7_x = 1;
  VIC.spr7_y = 1;

  memset((void*)0xd800,1,1024);

  memcpy ((void*) SPRITE0_DATA, p_sprites[0][1], 64);
  memcpy ((void*) SPRITE1_DATA, p_sprites[1][17], 64);
  memcpy ((void*) SPRITE2_DATA, p_sprites[2][1], 64);
  memcpy ((void*) SPRITE3_DATA, p_sprites[3][17], 64);

  memcpy ((void*) SPRITE4_DATA, sb_snowball, 64);
  memcpy ((void*) SPRITE5_DATA, sb_snowball, 64);
  memcpy ((void*) SPRITE6_DATA, sb_snowball, 64);
  memcpy ((void*) SPRITE7_DATA, sb_snowball, 64);
  textcolor (0);

  while( sRunning )
  {


    SPRITE_DATA = SPRITE0_DATA;
    BALL_DATA = SPRITE4_DATA;
    p_move(&p0);
    p0.p_hit = p0_hit;

    SPRITE_DATA = SPRITE1_DATA;
    BALL_DATA = SPRITE5_DATA;
    p_move(&p1);
    p1.p_hit = p1_hit;

    SPRITE_DATA = SPRITE2_DATA;
    BALL_DATA = SPRITE6_DATA;
    p_move(&p2);
    p2.p_hit = p2_hit;

    SPRITE_DATA = SPRITE3_DATA;
    BALL_DATA = SPRITE7_DATA;
    p_move(&p3);
    p3.p_hit = p3_hit;


    //gotoxy( 1, 21 ); cputs("coll: $"); cputhex8( VIC.spr_coll );
    //gotoxy( 1, 22 ); cputs("prio: $"); cputhex8( VIC.spr_prio );
    //gotoxy( 1, 23 ); cputs("   x: $"); cputc( VIC.spr_hi_x >> 0 & 0x1 ? '1' : '0'); cputhex8( VIC.spr0_x );
    //gotoxy( 1, 24 ); cputs("   y: $"); cputhex8( VIC.spr0_y );


    //for(a=0;a<100;a++){/*test*/};

  }

  return(0);
}

