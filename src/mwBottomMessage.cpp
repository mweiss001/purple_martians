// mwBottomMessage.cpp

#include "pm.h"
#include "mwBottomMessage.h"
#include "mwDisplay.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwTimeStamp.h"
#include "mwColor.h"
#include "mwLoop.h"
#include "mwItems.h"
#include "mwEnemy.h"
#include "mwSound.h"



mwBottomMessage mBottomMessage;



void mwBottomMessage::create_test_mode_list(void)
{
   initialize();


//   add(int ev, int x, int y, int z1, int z2, int z3, int z4);

//   add(5,  0, 0, 7, 0, 0,  0);  // p0 went through door
//   add(24, 0, 0, 1, 0, 10, 0);  // p1 lit bomb with 10 sec fuse
//   add(25, 0, 0, 2, 0, 0,  0);  // p2 armed bomb with remote detonator
//   add(26, 0, 0, 3, 0, 0,  0);  // p3 lit rocket
//
//   add(2,  0, 0, 4, 1039, 0, 0);  // p4 got a red key
//   add(2,  0, 0, 4, 1040, 0, 0);  // p4 got a green key
//   add(2,  0, 0, 4, 1041, 0, 0);  // p4 got a blue key
//   add(2,  0, 0, 4, 1042, 0, 0);  // p4 got a purple key
//
//   add(30,  0, 0, 4, 97, 0, 0);   // p4 flipped a switch (green)
//   add(30,  0, 0, 4, 98,  0, 0);  // p4 flipped a switch (red)
//   add(30,  0, 0, 4, 99,  0, 0);  // p4 flipped a switch (blue)
//   add(30,  0, 0, 4, 100, 0, 0);  // p4 flipped a switch (purple)

//   add(40,  0, 0, 4, 2, 0, 25);   // p4 got shot by another player p2
//   add(41,  0, 0, 4, 0, 0, 25);   // p4 shot themself
   add(43,  0, 0, 5, 4, 0, 25);   // p5 player got shot by enemy (bouncer)
   add(44,  0, 0, 5, 6, 0, 15);   // p6 player got shot by enemy (cannon)
//
   // need to fix holdoff for these
//   game_event_retrigger_holdoff[1] = mLoop.frame_num-1;
//   add(3,  0, 0, 4, 0, 12, 0);  // p4 tried to exit with 12 enemies left
//
//
//   game_event_retrigger_holdoff[1] = mLoop.frame_num-1;
//   add(3,  0, 0, 4, 0, 1, 0);  // p4 tried to exit with 1 enemies left
//
//
//   game_event_retrigger_holdoff[2] = mLoop.frame_num-1;
//   add(50,  0, 0, 3, 0, 0, 12);  // p3 hit a mine

//   int p = 4;
//   mPlayer.loc[p].block_damage_tally = 23;
//   mPlayer.loc[p].block_damage_type = 2;
//   add(58, 0, 0, p, 0, 0, 0);  // p4 took block damage (spikey floor)
//
//   p = 5;
//   mPlayer.loc[p].block_damage_tally = 16;
//   mPlayer.loc[p].block_damage_type = 0;
//   add(58, 0, 0, p, 0, 0, 0);  // p5 took block damage


//   add(52, 0, 0, 2, 1, 0, 33);  // p2 exploded p1
//   add(53, 0, 0, 0, 0, 0, 54);  // p0 exploded self
//
//   add(54, 0, 0, 3, 0, 0, 11);  // p3 got squished
//   add(56, 0, 0, 6, 0, 0, 13);  // p6 got stuck
//
   add(60, 0, 0, 6, 0, 4, 0);  // p6  killed enemy with a shot
   add(62, 0, 0, 5, 0, 7, 0);  // p5  killed enemy with a explosion

//   add(71, 0, 0, 1, 0, 0, 0);  // p1 got a purple coin

//   add(72, 0, 0, 2, 0, 1023, 26);  // p2 got a health bonus

//   for (int a=0; a<8; a++)
//     add(72, 0, 0, 2, 0, 928+a, 26);  // p2 got a health bonus
//
//   for (int a=0; a<8; a++)
//     add(72, 0, 0, 2, 0, 960+a, 26);  // p2 got a health bonus
//
//   add(72, 0, 0, 2, 0, 1023, 26);  // p2 got a health bonus
//   add(72, 0, 0, 2, 0, 1027, 26);  // p2 got a health bonus
//   add(72, 0, 0, 2, 0, 1035, 26);  // p2 got a health bonus



   add(80, 0, 0, 3, 0, 0, 0);  // p3 joined
//   add(81, 0, 0, 4, 0, 0, 0);  // p4 quit
//   add(90, 0, 0, 5, 0, 0, 0);  // p5 died

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








void mwBottomMessage::draw_text(const char *txt, int col, int &xpos)
{
   al_draw_text(mFont.pr16, mColor.pc[col], xpos, 2, ALLEGRO_ALIGN_INTEGER, txt);
   xpos += (strlen(txt)*16);
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
   //sprintf(msg, "Player %d", p);
   sprintf(msg, "P%d", p);
   int dp = disp_player;
   if (dp == 0) draw_bmp(tmp, xpos, 0, 0); // tile only
   if (dp == 1) draw_text(msg, mPlayer.syn[p].color, xpos); // text only
   if (dp == 2) // tile + text
   {
      draw_bmp(tmp, xpos, 0, 0);
      xpos += 6;
      draw_text(msg, mPlayer.syn[p].color, xpos);
   }
   if (dp == 3) // text + tile
   {
      draw_text(msg, mPlayer.syn[p].color, xpos);
      xpos += 4;
      draw_bmp(tmp, xpos, 0, 0);
   }
}

void mwBottomMessage::draw_enemy(int e_type, int &xpos)
{
   int de = disp_enemy;

   ALLEGRO_BITMAP *tmp = mBitmap.tile[mEnemy.enemy_tile[e_type]];

   if (de == 0) draw_bmp(tmp, xpos, 0, 0); // tile only
   if (de == 1) draw_text(mEnemy.enemy_name[e_type][0], 15, xpos); // text only
   if (de == 2) // tile + text
   {
      draw_bmp(tmp, xpos, 0, 0);
      xpos += 16;
      draw_text(mEnemy.enemy_name[e_type][0], 15, xpos);
   }
   if (de == 3) // text + tile
   {
      draw_text(mEnemy.enemy_name[e_type][0], 15, xpos);
      xpos += 16;
      draw_bmp(tmp, xpos, 0, 0);
   }
}

void mwBottomMessage::draw_item(ALLEGRO_BITMAP *tmp, const char *txt, int col, int &xpos, int xo, int yo)
{
   int di = disp_item;
   if (di == 0) draw_bmp(tmp, xpos, xo, yo); // tile only
   if (di == 1) draw_text(txt, col, xpos);   // text only
   if (di == 2) // tile + text
   {
      draw_bmp(tmp, xpos, xo, yo);
      xpos += 16;
      draw_text(txt, col, xpos);
   }
   if (di == 3) // text + tile
   {
      draw_text(txt, col, xpos);
      xpos += 16;
      draw_bmp(tmp, xpos, xo, yo);
   }
}

void mwBottomMessage::draw_health(int h, int &xpos)
{
   char msg[256];
   int col = 9; // green
   if (h < 0) col = 10; // red
   if (h == 0) col = 13; // blue
   sprintf(msg, " Health %+d", h);
   //sprintf(msg, " %+d", h);
   draw_text(msg, col, xpos);
}

void mwBottomMessage::add(int ev, int x, int y, int z1, int z2, int z3, int z4)
{
   char msg[256];
   double t0;
   if (mLog.LOG_TMR_bmsg_add) t0 = al_get_time();

   // event retrigger holdoff for these events that can repeat every frame
   if (ev == 3) // exit
   {
      if (game_event_retrigger_holdoff[1] < mLoop.frame_num) game_event_retrigger_holdoff[1] = mLoop.frame_num + 60;
      else ev = 0;
   }
   if (ev == 50) // mine
   {
      if (game_event_retrigger_holdoff[2] < mLoop.frame_num) game_event_retrigger_holdoff[2] = mLoop.frame_num + 20;
      else ev = 0;
   }
   if (ev == 59) // raw damage that needs to be tallied
   {
      int p = z1;
      float damage = (float)z4 / 100; // damage

      mPlayer.loc[p].block_damage_type = z2;

      if (mPlayer.loc[p].block_damage_holdoff < mLoop.frame_num) // triggered and not in holdoff
      {
         mPlayer.loc[p].block_damage_holdoff = mLoop.frame_num + 20; // set holdoff
         mPlayer.loc[p].block_damage_tally = damage; // init tally with current damage
      }
      if (mPlayer.loc[p].block_damage_holdoff > mLoop.frame_num) // triggered and in holdoff
      {
         mPlayer.loc[p].block_damage_tally += damage; // inc tally with current damage
      }
     ev = 0; // this is handled by player health that emits event 58
   }

   if ((ev != 0) && (ev != 1) && (ev != 4) && (ev != 15) && (ev != 22) && (ev != 31) && (ev != 31) ) // events that don't have bmsg handler
   {
      int xpos = 0;
      int custom_drawn = 0;

      al_set_target_bitmap(bmsg_temp);
      al_clear_to_color(al_map_rgba(0, 0, 0, 0));

      draw_player(z1, xpos); // all bmsg start with player

      if (ev == 5) // player went through a door
      {
         custom_drawn = 1;
         draw_text(" went through a ", 15, xpos);
         draw_item(mBitmap.door_tile[z4][z3][0], "door", 15, xpos, 0, 0);
      }
      if (ev == 24) // player lit bomb
      {
         custom_drawn = 1;
         draw_text(" lit a ", 15, xpos);
         draw_item(mBitmap.tile[465], "romb", 15, xpos, 0, 0);
         sprintf(msg, " with %d sec fuse",  z3);
         draw_text(msg, 15, xpos);
      }
      if (ev == 25) // player armed a bomb with a remote detonator
      {
         custom_drawn = 1;
         draw_text(" got a ", 15, xpos);
         draw_item(mBitmap.tile[539], "detonator", 15, xpos, 0, 4);
         draw_text(" for ", 15, xpos);
         draw_item(mBitmap.tile[538], "bomb", 15, xpos, 0, -4);

//         draw_text(" armed a ", 15, xpos);
//         draw_item(mBitmap.tile[538], "bomb", 15, xpos, 0, -4);
//         draw_text(" with ", 15, xpos);
//         draw_item(mBitmap.tile[539], "remote detonator", 15, xpos, 0, 4);
      }
      if (ev == 26) // player lit rocket
      {
         custom_drawn = 1;
         draw_text(" lit a ", 15, xpos);
         draw_item(mBitmap.tile[249], "rocket", 15, xpos, 0, 0);
      }
      if (ev == 3) // player tried to exit
      {
         custom_drawn = 1;
         draw_text(" found ", 15, xpos);

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

//         if (z3 == 1) sprintf(msg, " tried to exit with 1 enemy left");
//         else sprintf(msg, " tried to exit with %d enemies left",  z3);
//         draw_text(msg, 15, xpos);
      }
      if (ev == 2) // key
      {
         custom_drawn = 1;
         draw_text(" got a ", 15, xpos);

         int c1 = 0;  // color
         int tn = 0;  // tile_numer
         int k = z2 - 1039;

//         char tmsg[20] = {0};
//         if (k == 0) { sprintf(tmsg, "Red Key");    c1 = 10; tn = 272; } // red
//         if (k == 1) { sprintf(tmsg, "Green Key");  c1 = 11; tn = 279; } // green
//         if (k == 2) { sprintf(tmsg, "Blue Key");   c1 = 13; tn = 288; } // blue
//         if (k == 3) { sprintf(tmsg, "Purple Key"); c1 = 8;  tn = 295; } // purple
//         draw_item(mBitmap.tile[tn], tmsg, c1, xpos, 0, 0);

         if (k == 0) { c1 = 10; tn = 272; } // red
         if (k == 1) { c1 = 11; tn = 279; } // green
         if (k == 2) { c1 = 13; tn = 288; } // blue
         if (k == 3) { c1 = 8;  tn = 295; } // purple
         draw_item(mBitmap.tile[tn], "key", c1, xpos, 0, 0);
      }

      if (ev == 30) // switch
      {
         custom_drawn = 1;
         draw_text(" flipped a ", 15, xpos);
         draw_item(mBitmap.tile[z2], "switch", 15, xpos, 0, 0);
      }

      if (ev == 40) // player got shot by another player
      {
         custom_drawn = 1;
         draw_text(" got shot by ", 15, xpos);
         draw_player(z2, xpos);
         draw_health(-z4, xpos);
      }
      if (ev == 41) // player shot themself
      {
         custom_drawn = 1;
         draw_text(" shot themself!", 15, xpos);
         draw_health(-z4, xpos);
      }
      if (ev == 43) // player got shot by enemy
      {
         custom_drawn = 1;
         draw_text(" got shot by ", 15, xpos);
         draw_enemy(z2, xpos);
         draw_health(-z4, xpos);
      }
      if (ev == 44) // player got hit by enemy
      {
         custom_drawn = 1;
         draw_text(" got hit by ", 15, xpos);
         draw_enemy(z2, xpos);
         draw_health(-z4, xpos);
      }
      if (ev == 50) // player hit a mine
      {
         custom_drawn = 1;
         draw_text(" hit a ", 15, xpos);
         draw_item(mBitmap.tile[456], "mine", 15, xpos, 0, 0);
         draw_health(-z4, xpos);
      }
      if (ev == 52) // player exploded another player
      {
         custom_drawn = 1;
         draw_text(" exploded ", 15, xpos);
         draw_player(z2, xpos);
         draw_health(-z4, xpos);
      }
      if (ev == 53) // player exploded themself
      {
         custom_drawn = 1;
         draw_text(" exploded themself ", 15, xpos);
         draw_health(-z4, xpos);
      }
      if (ev == 54) // player got squished
      {
         custom_drawn = 1;
         draw_text(" got squished ", 15, xpos);
         draw_health(-z4, xpos);
      }
      if (ev == 56) // player got stuck
      {
         custom_drawn = 1;
         draw_text(" got stuck ", 15, xpos);
         draw_health(-z4, xpos);
      }

      if (ev == 58) // player lost health from block damage
      {
         custom_drawn = 1;
         int damage = (int) mPlayer.loc[z1].block_damage_tally;
         int type = mPlayer.loc[z1].block_damage_type;
         if (type == 2) draw_text(" was hurt by spikey floor ", 15, xpos);
         else           draw_text(" was hurt by block damage ", 15, xpos);
         draw_health(-damage, xpos);
      }

      if (ev == 60) // player killed enemy with a shot
      {
         custom_drawn = 1;
         draw_text(" killed ", 15, xpos);
         draw_enemy(z3, xpos);
         draw_text(" with a shot.", 15, xpos);
      }
      if (ev == 62) // player killed enemy with explosion
      {
         custom_drawn = 1;
         draw_text(" killed ", 15, xpos);
         draw_enemy(z3, xpos);
         draw_text(" with explosion!", 15, xpos);
      }

      if (ev == 71) // player got a purple coin
      {
         custom_drawn = 1;
         draw_text(" got a ", 15, xpos);
         draw_item(mBitmap.tile[197], "coin", 15, xpos, 0, 0);
      }

      if (ev == 72) // player got a health bonus
      {
         custom_drawn = 1;
         draw_text(" got a ", 15, xpos);

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
         draw_health(z4, xpos);
      }

      if (ev == 80) // player joined
      {
         custom_drawn = 1;
         draw_text(" joined!", 15, xpos);
      }
      if (ev == 81) // player quit
      {
         custom_drawn = 1;
         draw_text(" quit!", 15, xpos);
      }
      if (ev == 90) // player died
      {
         custom_drawn = 1;
         draw_text(" died!", 15, xpos);
      }
      if (custom_drawn) // caught by one of the handlers here
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
      else
      {
         printf(" no bmsg handler for event:%d\n",ev);
        // 1-shoot 4-exit 15-jump 22-explosion 31-sproingy
      }
   }
   if (mLog.LOG_TMR_bmsg_add) mLog.add_log_TMR(al_get_time() - t0, "bmsg_add4", 0);
}

void mwBottomMessage::draw()
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

         float oss = (io - fo) / (float) nb;  // opacity step
         float co = io; // current opacity

         float vss = (ivs - fvs) / (float) nb;  // v size step
         float cvs = ivs; // current v size
         float y = mDisplay.SCREEN_H - ivs*20 - 1;

         float hss = (ihs - fhs) / (float) nb;  // h size step
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
         }
         // draw bounding box to show what size it is
         //float tvs = (sw/2)*ivs;
         //al_draw_rectangle(x-tvs, mDisplay.SCREEN_H,  x+tvs, y, mColor.pc[15], 1);

      }
   }
   if (mLog.LOG_TMR_bmsg_draw) mLog.add_log_TMR(al_get_time() - t0, "bmsg_draw", 0);
}

