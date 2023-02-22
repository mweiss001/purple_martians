// mwBottomMessage.cpp

#include "pm.h"
#include "mwBottomMessage.h"
#include "mwDisplay.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwTimeStamp.h"
#include "mwColor.h"
#include "mwProgramState.h"
#include "mwItems.h"
#include "mwEnemy.h"


mwBottomMessage mwBM;

void mwBottomMessage::create_bitmaps(void)
{
   for (int x=0; x<20; x++)
   {
      bmsg_bmp[x] = mwB.create_and_clear_bitmap(800, 20);
      bmsg_bmp2[x] = bmsg_bmp[x]; // pointers only
   }
   bmsg_temp = mwB.create_and_clear_bitmap(800, 20); // temp bitmap for building new lines
}

void mwBottomMessage::initialize(void)
{
   for (int c=0; c<20; c++)
   {
      al_set_target_bitmap(bmsg_bmp[c]);
      al_clear_to_color(al_map_rgba(0,0,0,0));
   }
   bmsg_index = 0;
   bottom_msg_timer = 0;

  for (int i=0; i<10; i++)
     game_event_retrigger_holdoff[i] = 0;
}


int mwBottomMessage::draw_text(const char *txt, int col, int bmsg_length)
{
   al_draw_text(mF.pr16, mC.pc[col], bmsg_length, 2, ALLEGRO_ALIGN_INTEGER, txt);
   //printf("%d %s\n",mwPS.frame_num, txt);
   return (strlen(txt)*16);
}

int mwBottomMessage::draw_tile(int tn, int bmsg_length)
{
   al_draw_bitmap(mwB.tile[tn], bmsg_length, 0, 0);
   return 20;
}

int mwBottomMessage::draw_tile2(int tn, int bmsg_length, int xo, int yo)
{
   al_draw_bitmap(mwB.tile[tn], bmsg_length+xo, +yo, 0);
   return 20;
}

int mwBottomMessage::draw_player(int p, int bmsg_length)
{
   char msg[256];
   sprintf(msg, "Player %d", p);
   int len = 0;
   if (0) // 'Player x'
   {
      len += draw_text(msg, players[p].color, bmsg_length);
   }

   if (1) // player tile
   {
      al_draw_bitmap(mwB.player_tile[players[p].color][1], bmsg_length, 0, 0); // draw tile
      len += 16;
   }
   if (0) // 'Player x' and tile
   {
      len += draw_text(msg, players[p].color, bmsg_length) + 12;
      al_draw_bitmap(mwB.player_tile[players[p].color][1], bmsg_length + len, 0, 0); // draw tile
      len += 20;
   }
   return len;
}

int mwBottomMessage::draw_enemy(int e_type, int bmsg_length)
{
   int len = 0;
   if (0) // enemy name
   {
      len += draw_text(mEnemy.enemy_name[e_type][0], 15, bmsg_length);
   }
   if (1) // enemy tile
   {
      len += draw_tile(mEnemy.enemy_tile[e_type], bmsg_length);
   }
   if (0) // name and tile
   {
      len += draw_text(mEnemy.enemy_name[e_type][0], 15, bmsg_length) + 8;
      len += draw_tile(mEnemy.enemy_tile[e_type], bmsg_length+len);
   }
   if (0) // tile and name
   {
      len += draw_tile(mEnemy.enemy_tile[e_type], bmsg_length-4) + 8;
      len += draw_text(mEnemy.enemy_name[e_type][0], 15, bmsg_length+len);
   }
   return len;
}

int mwBottomMessage::draw_health(int h, int bmsg_length)
{
   char msg[256];
   int col = 9; // green
   if (h < 0) col = 10; // red
   if (h == 0) col = 13; // blue
   sprintf(msg, " Health %+d", h);
   bmsg_length += draw_text(msg, col, bmsg_length);
   return (strlen(msg)*16);
}

void mwBottomMessage::add(int ev, int x, int y, int z1, int z2, int z3, int z4)
{
   char msg[256];
   if (LOG_TMR_bmsg_add) t0 = al_get_time();

   // event retrigger holdoff for these events that can repeat every frame
   if (ev == 3) // exit
   {
      if (game_event_retrigger_holdoff[1] < mwPS.frame_num) game_event_retrigger_holdoff[1] = mwPS.frame_num + 60;
      else ev = 0;
   }
   if (ev == 50) // mine
   {
      if (game_event_retrigger_holdoff[2] < mwPS.frame_num) game_event_retrigger_holdoff[2] = mwPS.frame_num + 20;
      else ev = 0;
   }
   if (ev == 59) // raw damage that needs to be tallied
   {
      int p = z1;
      float damage = (float)mItem.item[z2][15] / 100; // damage from item 17 - block damage

      if (players1[p].block_damage_holdoff < mwPS.frame_num) // triggered and not in holdoff
      {
         players1[p].block_damage_holdoff = mwPS.frame_num + 20; // set holdoff
         players1[p].block_damage_tally = damage; // init tally with current damage
      }
      if (players1[p].block_damage_holdoff > mwPS.frame_num) // triggered and in holdoff
      {
         players1[p].block_damage_tally += damage; // inc tally with current damage
      }
     ev = 0; // this is handled by player health that emits event 58
   }

   if ((ev != 0) && (ev != 1) && (ev != 4) && (ev != 15) && (ev != 22) && (ev != 31) && (ev != 31) ) // events that don't have bmsg handler
   {
      int bmsg_length = 0; // keep a running total
      int custom_drawn = 0;

      al_set_target_bitmap(bmsg_temp);
      al_clear_to_color(al_map_rgba(0, 0, 0, 0));

      bmsg_length += draw_player(z1, bmsg_length); // all bmsg start with player

      if (ev == 5) // player went through a door
      {
         custom_drawn = 1;
         bmsg_length += draw_text(" went through a door", 15, bmsg_length);
      }
      if (ev == 24) // player lit bomb
      {
         custom_drawn = 1;
         bmsg_length += draw_text(" lit a bomb ", 15, bmsg_length);
         bmsg_length += draw_tile(465, bmsg_length);
         sprintf(msg, " with %d sec fuse",  z3);
         bmsg_length += draw_text(msg, 15, bmsg_length);
      }
      if (ev == 25) // player armed a bomb with a remote detonator
      {
         custom_drawn = 1;
         bmsg_length += draw_text(" armed a bomb ", 15, bmsg_length);
         bmsg_length += draw_tile(538, bmsg_length);
         bmsg_length += draw_text(" with remote detonator ", 15, bmsg_length);
         bmsg_length += draw_tile(539, bmsg_length);
      }
      if (ev == 26) // player lit rocket
      {
         custom_drawn = 1;
         bmsg_length += draw_text(" lit a rocket ", 15, bmsg_length);
         bmsg_length += draw_tile(249, bmsg_length);
      }
      if (ev == 3) // player tried to exit
      {
         custom_drawn = 1;
         if (z3 == 1) sprintf(msg, " tried to exit with 1 enemy left");
         else sprintf(msg, " tried to exit with %d enemies left",  z3);
         bmsg_length += draw_text(msg, 15, bmsg_length);
      }
      if (ev == 2) // key
      {
         custom_drawn = 1;
         bmsg_length += draw_text(" got a ", 15, bmsg_length);

         int c1 =0;  // color
         int tn = 0; // tile_numer
         int k = mItem.item[z2][1] - 1039;
         char tmsg[20] = {0};

         if (k == 0) { sprintf(tmsg, "red");    c1 = 10; tn = 272; } // red
         if (k == 1) { sprintf(tmsg, "green");  c1 = 11; tn = 279; } // green
         if (k == 2) { sprintf(tmsg, "blue");   c1 = 13; tn = 288; } // blue
         if (k == 3) { sprintf(tmsg, "purple"); c1 = 8;  tn = 295; } // purple

         bmsg_length += draw_text(tmsg, c1, bmsg_length);
         bmsg_length += draw_text(" key ", 15, bmsg_length);
         bmsg_length += draw_tile2(tn, bmsg_length, -10, 0);
      }

      if (ev == 30) // switch
      {
         custom_drawn = 1;
         bmsg_length += draw_text(" flipped a switch ", 15, bmsg_length);
         bmsg_length += draw_tile2(mItem.item[z2][1], bmsg_length, -10, -2);
      }

      if (ev == 40) // player got shot by another player
      {
         custom_drawn = 1;
         bmsg_length += draw_text(" got shot by ", 15, bmsg_length);
         bmsg_length += draw_player(z2, bmsg_length);
         bmsg_length += draw_health(-z4, bmsg_length);
      }
      if (ev == 41) // player shot themself
      {
         custom_drawn = 1;
         bmsg_length += draw_text(" shot themself!", 15, bmsg_length);
         bmsg_length += draw_health(-z4, bmsg_length);
      }
      if (ev == 43) // player got shot by enemy
      {
         custom_drawn = 1;
         bmsg_length += draw_text(" got shot by ", 15, bmsg_length);
         bmsg_length += draw_enemy(z2, bmsg_length);
         bmsg_length += draw_health(-z4, bmsg_length);
      }
      if (ev == 44) // player got hit by enemy
      {
         custom_drawn = 1;
         bmsg_length += draw_text(" got hit by ", 15, bmsg_length);
         bmsg_length += draw_enemy(mEnemy.Ei[z2][0], bmsg_length);
         bmsg_length += draw_health(-z4, bmsg_length);
      }
      if (ev == 50) // player hit a mine
      {
         custom_drawn = 1;
         bmsg_length += draw_text(" hit a mine ", 15, bmsg_length);
         bmsg_length += draw_tile(456, bmsg_length);
         bmsg_length += draw_health(-z4, bmsg_length);
      }
      if (ev == 52) // player exploded another player
      {
         custom_drawn = 1;
         bmsg_length += draw_text(" exploded ", 15, bmsg_length);
         bmsg_length += draw_player(z2, bmsg_length);
         bmsg_length += draw_health(-z4, bmsg_length);
      }
      if (ev == 53) // player exploded themself
      {
         custom_drawn = 1;
         bmsg_length += draw_text(" exploded themself ", 15, bmsg_length);
         bmsg_length += draw_health(-z4, bmsg_length);
      }
      if (ev == 54) // player got squished
      {
         custom_drawn = 1;
         bmsg_length += draw_text(" got squished ", 15, bmsg_length);
         bmsg_length += draw_health(-z4, bmsg_length);
      }
      if (ev == 56) // player got stuck
      {
         custom_drawn = 1;
         bmsg_length += draw_text(" got stuck ", 15, bmsg_length);
         bmsg_length += draw_health(-z4, bmsg_length);
      }

      if (ev == 58) // player lost health from block damage
      {
         custom_drawn = 1;
         int damage = (int) players1[z1].block_damage_tally;
         int type = mItem.item[players1[z1].block_damage_item_number][2];
         if (type == 2) bmsg_length += draw_text(" was hurt by spikey floor ", 15, bmsg_length);
         else           bmsg_length += draw_text(" was hurt by block damage ", 15, bmsg_length);
         bmsg_length += draw_health(-damage, bmsg_length);
      }

      if (ev == 60) // player killed enemy with a shot
      {
         custom_drawn = 1;
         bmsg_length += draw_text(" killed ", 15, bmsg_length);
         bmsg_length += draw_enemy(z3, bmsg_length);
         bmsg_length += draw_text(" with a shot.", 15, bmsg_length);
      }
      if (ev == 62) // player killed enemy with explosion
      {
         custom_drawn = 1;
         bmsg_length += draw_text(" killed ", 15, bmsg_length);
         bmsg_length += draw_enemy(z3, bmsg_length);
         bmsg_length += draw_text(" with explosion!", 15, bmsg_length);
      }

      if (ev == 71) // player got a purple coin
      {
         custom_drawn = 1;
         bmsg_length += draw_text(" got a purple coin! ", 15, bmsg_length);
         bmsg_length += draw_tile(265, bmsg_length);
      }

      if (ev == 72) // player got a health bonus
      {
         custom_drawn = 1;
         bmsg_length += draw_text(" picked up a Health Bonus! ", 15, bmsg_length);
         int bt = 320;
         if (z3 == 1023) bt = 304;
         if (z3 == 1027) bt = 240;
         if (z3 == 1035) bt = 320;
         bmsg_length += draw_tile2(bt, bmsg_length, -8, -3) -8;
         bmsg_length += draw_health(z4, bmsg_length);
      }
      if (ev == 80) // player joined
      {
         custom_drawn = 1;
         bmsg_length += draw_text(" joined!", 15, bmsg_length);
      }
      if (ev == 81) // player quit
      {
         custom_drawn = 1;
         bmsg_length += draw_text(" quit!", 15, bmsg_length);
      }
      if (ev == 90) // player died
      {
         custom_drawn = 1;
         bmsg_length += draw_text(" died!", 15, bmsg_length);
      }
      if (custom_drawn) // caught by one of the handlers here
      {
         bottom_msg_timer = 100; // start the timer
         if (++bmsg_index > 19) bmsg_index = 0;

         al_set_target_bitmap(bmsg_bmp[bmsg_index]);
         al_clear_to_color(al_map_rgba(0, 0, 0, 0));
         al_draw_bitmap(bmsg_temp, (400 - (bmsg_length/2)), 0, 0);
         al_set_target_backbuffer(display);

         // convert array to array2 to re-arrange order...this does not actually copy anything except pointers!
         int tc = 0;
         // cycle from 1-index to 1-min
         for (int c=bmsg_index; c>=0; c--)
         {
            bmsg_bmp2[tc] = bmsg_bmp[c];
            tc++;
         }
         // cycle from 1-max to 1-index
         for (int c=19; c>bmsg_index; c--)
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
   if (LOG_TMR_bmsg_add) add_log_TMR(al_get_time() - t0, "bmsg_add4", 0);
}

void mwBottomMessage::draw()
{
   if (LOG_TMR_bmsg_draw) t0 = al_get_time();
   if (bottom_msg_on)
   {
//      bottom_msg_timer = 100; // always draw
      if (bottom_msg_timer > 0)
      {
         bottom_msg_timer--;

         int nb = 8;  // number of bottom message lines to display (max 20)

         int dfb = 2000 - players[active_local_player].y; // player distance from bottom of level
         if (dfb < 300)  nb = 4;
         if (dfb < 200)  nb = 2;

         int sw = 800; // string length in pixels
         int sh = 20;  // string height in pixels
         float x = mwD.SCREEN_W/2 - 10;

         float io = 1.0; // initial opacity
         float fo = 0.1; // final opacity
         float oss = (io - fo) / (float) nb;  // opacity step
         float co = io; // current opacity

         float ivs = 0.5; // initial v size
         float fvs = 0.1; // final v size
         float vss = (ivs - fvs) / (float) nb;  // v size step
         float cvs = ivs; // current v size
         float y = mwD.SCREEN_H - ivs*20 - 1;

         float ihs = 0.5; // initial h size
         float fhs = 0.1; // final h size
         float hss = (ihs - fhs) / (float) nb;  // v size step
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
//         float tvs = (sw/2)*ivs;
//         al_draw_rectangle(x-tvs, mwD.SCREEN_H,  x+tvs, y, mC.pc[15], 1);

      }
   }
   if (LOG_TMR_bmsg_draw) add_log_TMR(al_get_time() - t0, "bmsg_draw", 0);
}




