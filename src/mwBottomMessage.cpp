// mwBottomMessage.cpp

#include "pm.h"
#include "mwBottomMessage.h"
#include "mwDisplay.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwTimeStamp.h"
#include "mwColor.h"
#include "mwLoop.h"
#include "mwItem.h"
#include "mwEnemy.h"
#include "mwSound.h"
#include "mwScreen.h"
#include "mwPlayer.h"

mwBottomMessage mBottomMessage;


void mwBottomMessage::init_filter_events(void)
{
   // all off
   for (int i=0; i<100; i++) filter_event[i] = 0;

   filter_event[6] = 1;  // player join
   filter_event[7] = 1;  // player quit
   filter_event[8] = 1;  // player died
   filter_event[12] = 1; // show damage
   filter_event[20] = 1; // key
   filter_event[21] = 1; // switch
   filter_event[22] = 1; // door
   filter_event[23] = 1; // exit
   filter_event[24] = 1; // spring
   filter_event[25] = 1; // bomb
   filter_event[26] = 1; // rocket
   filter_event[27] = 1; // coin
   filter_event[28] = 1; // bonus
   filter_event[40] = 1; // player hurt player
   filter_event[41] = 1; // player hurt by enemy
   filter_event[42] = 1; // player killed enemy
}





void mwBottomMessage::create_test_mode_list(void)
{
   initialize();

//   add(int ev, int x, int y, int z1, int z2, int z3, int z4);

   add(20,  0, 0, 4, 1039, 0, 0);  // p4 got a red key
//   add(20,  0, 0, 4, 1040, 0, 0);  // p4 got a green key
//   add(20,  0, 0, 4, 1041, 0, 0);  // p4 got a blue key
//   add(20,  0, 0, 4, 1042, 0, 0);  // p4 got a purple key

   add(21,  0, 0, 4, 97, 0, 0);   // p4 flipped a switch (green)
//   add(21,  0, 0, 4, 98,  0, 0);  // p4 flipped a switch (red)
//   add(21,  0, 0, 4, 99,  0, 0);  // p4 flipped a switch (blue)
//   add(21,  0, 0, 4, 100, 0, 0);  // p4 flipped a switch (purple)

   add(22,  0, 0, 7, 0, 10,  1);  // p0 went through door

   // need to set holdoff for exit
   game_event_retrigger_holdoff[1] = mLoop.frame_num-1;
   add(23,  0, 0, 4, 0, 12, 0);  // p4 tried to exit with 12 enemies left

   add(25, 0, 0, 1, 0, 10, 0);    // p1 lit bomb with 10 sec fuse
   add(25, 0, 0, 2, 0, 0,  1);    // p2 armed bomb with remote detonator
   add(26, 0, 0, 3, 0, 0,  0);    // p3 lit rocket

   add(27, 0, 0, 1, 0, 0, 0);     // p1 got a purple coin

//   for (int a=0; a<8; a++)
//     add(28, 0, 0, 2, 0, 928+a, 26);  // p2 got a health bonus
//
//   for (int a=0; a<8; a++)
//     add(28, 0, 0, 2, 0, 960+a, 26);  // p2 got a health bonus
//
   add(28, 0, 0, 2, 0, 1023, 10);  // p2 got a health bonus
   add(28, 0, 0, 3, 0, 1027, 50);  // p3 got a health bonus
   add(28, 0, 0, 4, 0, 1035, 75);  // p4 got a health bonus

   add(28, 0, 0, 6, 0, 930,  56);  // p6 got a health bonus
   add(28, 0, 0, 7, 0, 961,  77);  // p7 got a health bonus
   add(28, 0, 0, 7, 0, 963,  87);  // p7 got a health bonus

   add(40,  0, 0, 4, 2, 1, 25);   // p4 got shot by another player p2
   add(40,  0, 0, 4, 4, 1, 25);   // p4 shot themself

   add(40, 0, 0, 2, 1, 2, 33);    // p2 exploded p1
   add(40, 0, 0, 3, 3, 2, 54);    // p3 exploded self


//   add(43,  0, 0, 5, 4, 0, 25);   // p5 player got shot by enemy (bouncer)
//   add(44,  0, 0, 5, 6, 0, 15);   // p6 player got shot by enemy (cannon)

   add(41,  0, 0, 5, 4, 1, 25);   // p5 player got hit by enemy (bouncer)
   add(41,  0, 0, 6, 6, 2, 15);   // p6 player got shot by enemy (cannon)



   add(42, 0, 0, 6, 0, 4, 1);  // p6  killed enemy with a shot
   add(42, 0, 0, 5, 0, 7, 2);  // p5  killed enemy with a explosion

//   for (int a=3; a<14; a++)
//      if (a != 10)
//         add(42, 0, 0, 5, 0, a, 2);  // p5  killed enemy with a explosion
//
//   for (int a=3; a<14; a++)
//      if (a != 10)
//         add(42, 0, 0, 6, 0, a, 1);  // p6  killed enemy with a shot


   add(6, 0, 0, 3, 0, 0, 0);  // p3 joined
   add(7, 0, 0, 4, 0, 0, 0);  // p4 quit
   add(8, 0, 0, 5, 0, 0, 0);  // p5 died

   add(12, 0, 0, 4, 2, 0, 23);  // p4 took block damage (spikey floor)
   add(12, 0, 0, 5, 1, 0, 16);  // p5 took block damage
   add(12, 0, 0, 3, 5, 0, 12);  // p3 hit a mine
   add(12, 0, 0, 3, 7, 0, 11);  // p3 got squished
   add(12, 0, 0, 6, 9, 0, 13);  // p6 got stuck


   test_mode_list_created = 1;
}



void mwBottomMessage::create_bitmaps(void)
{
   for (int x=0; x<BMSG_MAX_LINES; x++)
   {
      bmsg_bmp[x] = mBitmap.create_and_clear_bitmap(800, 20);
      bmsg_bmp2[x] = bmsg_bmp[x]; // pointers only
   }
   bmsg_temp = mBitmap.create_and_clear_bitmap(800, 20); // temp bitmap for building new lines
}

void mwBottomMessage::initialize(void)
{
   if (!mDisplay.no_display)
   {
      for (int c=0; c<BMSG_MAX_LINES; c++)
      {
         al_set_target_bitmap(bmsg_bmp[c]);
         al_clear_to_color(al_map_rgba(0,0,0,0));
      }
      bmsg_index = 0;
      bottom_msg_timer = 0;

      test_mode_list_created = 0;

      for (int i=0; i<10; i++)
         game_event_retrigger_holdoff[i] = 0;
   }
}



void mwBottomMessage::draw_text(int &xpos, int col, const char *txt)
{
   al_draw_text(mFont.pr16, mColor.pc[col], xpos, 2, ALLEGRO_ALIGN_INTEGER, txt);
   xpos += (strlen(txt)*16);
}

void mwBottomMessage::draw_textf(int &xpos, int col, const char *format, ...)
{
   char msg[1024];
   va_list args;
   va_start(args, format);
   vsprintf(msg, format, args);
   va_end(args);
   draw_text(xpos, col, msg);
}


void mwBottomMessage::draw_bmp(ALLEGRO_BITMAP *tmp, int &xpos, int xo, int yo)
{
   al_draw_bitmap(tmp, xpos+xo, yo, 0);
   xpos += 20;
}


void mwBottomMessage::draw_player(int p, int &xpos)
{
   ALLEGRO_BITMAP *tmp = mBitmap.player_tile[mPlayer.syn[p].color][1];

   char msg[256];
   sprintf(msg, "P%d", p);
   if (disp_player_text_long) sprintf(msg, "Player %d", p);

   //sprintf(msg, "Player %d", p);
   int dp = disp_player;
   if (dp == 0) draw_bmp(tmp, xpos, 0, 0); // tile only
   if (dp == 1) draw_text(xpos, mPlayer.syn[p].color, msg); // text only
   if (dp == 2) // tile + text
   {
      draw_bmp(tmp, xpos, 0, 0);
      xpos += 6;
      draw_text(xpos, mPlayer.syn[p].color, msg);
   }
   if (dp == 3) // text + tile
   {
      draw_text(xpos, mPlayer.syn[p].color, msg);
      xpos += 4;
      draw_bmp(tmp, xpos, 0, 0);
   }
}

void mwBottomMessage::bm_draw_enemy(int e_type, int &xpos)
{
   int de = disp_enemy;

   ALLEGRO_BITMAP *tmp = mBitmap.tile[mEnemy.enemy_tile[e_type]];

   if (de == 0) draw_bmp(tmp, xpos, 0, 0); // tile only
   if (de == 1) draw_text(xpos, 15, mEnemy.enemy_name[e_type][2]); // text only
   if (de == 2) // tile + text
   {
      draw_bmp(tmp, xpos, 0, 0);
      xpos += 16;
      draw_text(xpos, 15, mEnemy.enemy_name[e_type][2]);
   }
   if (de == 3) // text + tile
   {
      draw_text(xpos, 15, mEnemy.enemy_name[e_type][2]);
      xpos += 16;
      draw_bmp(tmp, xpos, 0, 0);
   }
}

void mwBottomMessage::draw_item(ALLEGRO_BITMAP *tmp, const char *txt, int col, int &xpos, int xo, int yo)
{
   int di = disp_item;
   if (di == 0) draw_bmp(tmp, xpos, xo, yo); // tile only
   if (di == 1) draw_text(xpos, col, txt);   // text only
   if (di == 2) // tile + text
   {
      draw_bmp(tmp, xpos, xo, yo);
      xpos += 16;
      draw_text(xpos, col, txt);
   }
   if (di == 3) // text + tile
   {
      draw_text(xpos, col, txt);
      xpos += 16;
      draw_bmp(tmp, xpos, xo, yo);
   }
}

void mwBottomMessage::draw_health(int p, int h, int &xpos, int xo, int yo)
{
   xpos += xo;
   int dh = disp_health;
   int col = 13; // blue
   if (h < 0) col = 10; // red
   if (h > 0) col = 9;  // green

   if (dh == 0) draw_textf(xpos, col, "Health %+d", h);
   if (dh == 1) draw_textf(xpos, col, "H%+d", h);
   if (dh == 2) draw_textf(xpos, col, "%+d", h);
   if (dh == 3)
   {
      int hbw = 24;
      int hbx = xpos + 2 + hbw/2;
      int hbh = 6;
      int hby = 4 + (20 - hbh)/2;
      mScreen.draw_percent_barc(hbx, hby, hbw, hbh, abs(h), 0, col, 15);
      al_draw_textf(mFont.pr8, mColor.pc[col], hbx, 3, ALLEGRO_ALIGN_CENTER, "%+d", h);
   }
}


void mwBottomMessage::add(int ev, int wx, int wy, int z1, int z2, int z3, int z4)
{
   char msg[256];
   double t0 = al_get_time();

   if (filter_event[ev] == 0) ev = 0;

   // event retrigger holdoff for events that can repeat every frame
   if (ev == 23) // exit
   {
      if (game_event_retrigger_holdoff[1] < mLoop.frame_num) game_event_retrigger_holdoff[1] = mLoop.frame_num + 60;
      else ev = 0;
   }

   if ((ev != 0) && (ev != 1) && (ev != 2) && (ev != 4) && (ev != 5) && (ev != 11) && (ev != 24) ) // events that don't have bmsg handler (1-shot 2-explosion, 4-unlocked exit, 5-jump, 24-spring
   {
      int xpos = 0;
      int custom_drawn = 0;

      al_set_target_bitmap(bmsg_temp);
      al_clear_to_color(al_map_rgba(0, 0, 0, 0));

      draw_player(z1, xpos); // all bmsg start with player


      if (ev == 20) // key
      {
         custom_drawn = 1;
         draw_text(xpos, 15, " got a ");
         int c1 = 0;  // color
         int tn = 0;  // tile_numer
         int k = z2 - 1039;
         if (k == 0) { c1 = 10; tn = 272; } // red
         if (k == 1) { c1 = 11; tn = 279; } // green
         if (k == 2) { c1 = 13; tn = 288; } // blue
         if (k == 3) { c1 = 8;  tn = 295; } // purple
         draw_item(mBitmap.tile[tn], "key", c1, xpos, 0, 0);
      }
      if (ev == 21) // switch
      {
         custom_drawn = 1;
         draw_text(xpos, 15, " flipped a ");
         draw_item(mBitmap.tile[z2], "switch", 15, xpos, 0, 0);
      }
      if (ev == 22) // door
      {
         custom_drawn = 1;
         draw_text(xpos, 15, " went through a ");
         draw_item(mBitmap.door_tile[z4][z3][0], "door", 15, xpos, 0, 0);
      }
      if (ev == 23) // locked exit
      {
         custom_drawn = 1;
         draw_text(xpos, 15, " found ");

         // construct the exit bitmap
         ALLEGRO_BITMAP *etmp = al_create_bitmap(20, 20);
         al_set_target_bitmap(etmp);
         al_clear_to_color(al_map_rgba(0, 0, 0, 0));
         al_draw_bitmap(mBitmap.tile[399], 0, 0, 0);  // base tile
         al_draw_bitmap(mBitmap.tile[366], 0, 0, 0);  // lock
         al_draw_text(mFont.pixl, mColor.pc[10], 11, -2, ALLEGRO_ALIGN_CENTER, "EXIT");
         al_draw_textf(mFont.pixl, mColor.pc[14], 11, -2, ALLEGRO_ALIGN_CENTER, "%d", z3);

         al_set_target_bitmap(bmsg_temp);
         draw_item(etmp, "locked exit", 15, xpos, 0, 0);
         al_destroy_bitmap(etmp);
      }
      if (ev == 25) // bomb
      {
         custom_drawn = 1;
         if (z4)
         {
            draw_text(xpos, 15, " got a ");
            draw_item(mBitmap.tile[539], "detonator", 15, xpos, 0, 4);
            draw_text(xpos, 15, " for ");
            draw_item(mBitmap.tile[538], "bomb", 15, xpos, 0, -4);
         }
         else
         {
            draw_text(xpos, 15, " lit a ");
            draw_item(mBitmap.tile[465], "bomb", 15, xpos, 0, 0);
            sprintf(msg, " with %d sec fuse",  z3);
            draw_text(xpos, 15, msg);
         }
      }
      if (ev == 26) // rocket
      {
         custom_drawn = 1;
         draw_text(xpos, 15, " lit a ");
         draw_item(mBitmap.tile[249], "rocket", 15, xpos, 0, 0);
      }
      if (ev == 27) // coin
      {
         custom_drawn = 1;
         draw_text(xpos, 15, " got a ");
         draw_item(mBitmap.tile[197], "coin", 15, xpos, 0, 0);
      }
      if (ev == 28) // bonus
      {
         custom_drawn = 1;
         draw_text(xpos, 15, " got a ");

         char tmsg[80] = {0};
         sprintf(tmsg, "potion");

         if ((z3 > 958) && (z3 < 968)) sprintf(tmsg, "diamond");
         if ((z3 == 928) || (z3 == 929) || (z3 == 932) ) sprintf(tmsg, "fruit");
         if ((z3 == 930) || (z3 == 931) || (z3 == 934) ) sprintf(tmsg, "flower");
         if (z3 == 933) sprintf(tmsg, "carrot");
         if (z3 == 935) sprintf(tmsg, "rainbow");

         int yo = 0;
         if (z3 == 935) yo = -4; // raise rainbow tile

         int bt = z3;
         if (z3 == 1023) bt = 304;
         if (z3 == 1027) bt = 240;
         if (z3 == 1035) bt = 320;
         draw_item(mBitmap.tile[bt], tmsg, 15, xpos, 0, yo);
         draw_health(z1, z4, xpos, 16, 0);
      }
      if (ev == 40) // player hurt player
      {
         custom_drawn = 1;
         if (z3 == 1) draw_text(xpos, 15, " shot ");
         if (z3 == 2) draw_text(xpos, 15, " exploded ");
         //if (z1 == z2) draw_text("themself!", 15, xpos); //else
         draw_player(z2, xpos);
         draw_health(z1, -z4, xpos, 16, 0);
      }
      if (ev == 42) // player killed enemy
      {
         custom_drawn = 1;
         if (z4 == 1) draw_text(xpos, 15, " shot ");
         if (z4 == 2) draw_text(xpos, 15, " exploded ");
         bm_draw_enemy(z3, xpos);
      }

      if (ev == 41) // player hurt by enemy
      {
         custom_drawn = 1;
         if (z3 == 1) draw_text(xpos, 15, " got hit by ");
         if (z3 == 2) draw_text(xpos, 15, " got shot by ");
         bm_draw_enemy(z2, xpos);
         draw_health(z1, -z4, xpos, 16, 0);
      }
      if (ev == 6) // player joined
      {
         custom_drawn = 1;
         draw_text(xpos, 15, " joined!");
      }
      if (ev == 7) // player quit
      {
         custom_drawn = 1;
         draw_text(xpos, 15, " quit!");
      }
      if (ev == 8) // player died
      {
         custom_drawn = 1;
         draw_text(xpos, 15, " died!");
      }

      if (ev == 12) // player took damage (lost health from block damage, mine, stuck...
      {
         custom_drawn = 1;
         if (z2 == 1) draw_text(xpos, 15, " was hurt by damage field");
         if (z2 == 2) draw_text(xpos, 15, " was hurt by spikey floor");
         if (z2 == 5)
         {
            draw_text(xpos, 15, " was hurt by ");
            draw_item(mBitmap.tile[456], "mine", 15, xpos, 0, 0);
         }
         if (z2 == 7) draw_text(xpos, 15, " got stuck in blocks");
         if (z2 == 9) draw_text(xpos, 15, " got squished by lift");
         draw_health(z1, -z4, xpos, 16, 0);
      }

      if (!custom_drawn) printf(" no bmsg handler for event:%d\n",ev); // not caught by one of the handlers here
      else // add to bmsg bitmap array
      {
         bottom_msg_timer = 100; // start the timer
         if (++bmsg_index > BMSG_MAX_LINES-1) bmsg_index = 0;
         al_set_target_bitmap(bmsg_bmp[bmsg_index]);
         al_clear_to_color(al_map_rgba(0, 0, 0, 0));
         al_draw_bitmap(bmsg_temp, (400 - (xpos/2)), 0, 0);
         al_set_target_backbuffer(mDisplay.display);

         // convert array to array2 to re-arrange order...this does not actually copy anything except pointers!
         int tc = 0;
         // cycle from 1-index to 1-min
         for (int c=bmsg_index; c>=0; c--)
         {
            bmsg_bmp2[tc] = bmsg_bmp[c];
            tc++;
         }
         // cycle from 1-max to 1-index
         for (int c=BMSG_MAX_LINES-1; c>bmsg_index; c--)
         {
            bmsg_bmp2[tc] = bmsg_bmp[c];
            tc++;
         }
      }
   }
   mLog.add_tmr1(LOG_TMR_bmsg_add, 0, "bmsg_add4", al_get_time() - t0);
}

void mwBottomMessage::draw(int outline)
{
   double t0 = al_get_time();
   if (bottom_msg_on)
   {
      //bottom_msg_timer = 100; // always draw
      if (bottom_msg_timer > 0)
      {
         bottom_msg_timer--;

         int nb = num_lines;  // number of bottom message lines to display (max 20)

         int dfb = 2000 - mPlayer.syn[mPlayer.active_local_player].y; // player distance from bottom of level
         if (dfb < 300)  nb = 4;
         if (dfb < 200)  nb = 2;

         int sw = 800; // string length in pixels
         int sh = 20;  // string height in pixels
         float x = mDisplay.SCREEN_W/2 - 10;

         float oss = (io - fo) / (float)(nb-1);  // opacity step
         float co = io; // current opacity

         float vss = (ivs - fvs) / (float)(nb-1);  // v size step
         float cvs = ivs; // current v size
         float y = mDisplay.SCREEN_H - ivs*20 - 1;

         float hss = (ihs - fhs) / (float)(nb-1);  // h size step
         float chs = ihs; // current v size

         for (int m=0; m<nb; m++)
         {
            float dw = chs * sw;
            float dh = cvs * sh;
            ALLEGRO_COLOR col = al_map_rgba_f(co, co, co, co);
            al_draw_tinted_scaled_bitmap(bmsg_bmp2[m], col, 0, 0, sw, sh, x-dw/2, y, dw, dh, 0);
            co -= oss;
            cvs -= vss;
            chs -= hss;
            y -= dh;

            //printf("m:%d oss:%f co:%f\n", m, oss, co);

         }
         if (outline)
         {
            // draw bounding box to show what size it is
            float tvs = (sw/2)*ivs;
            al_draw_rectangle(x-tvs, mDisplay.SCREEN_H,  x+tvs, y+(cvs*sh), mColor.pc[15], 1);
         }
      }
   }
   mLog.add_tmr1(LOG_TMR_bmsg_draw, 0, "bmsg_draw", al_get_time() - t0);
}

