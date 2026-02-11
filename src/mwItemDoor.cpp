// mwItemDoor.cpp

#include "pm.h"
#include "mwItem.h"
#include "mwPlayer.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwColor.h"
#include "mwLoop.h"
#include "mwGameEvent.h"
#include "mwEventQueue.h"

// item[x][1]  draw type (0=hidden, 1=static door, 2=animated warp)
// item[x][6]  color
// item[x][7]  move type (0 = auto, 1 = force instant, 2 = force move
// item[x][8]  type (0 = no dest (exit only), 1 = linked dest
// item[x][9]  linked destination item
// item[x][10] key held flag
// item[x][11] door entry type (0 0-immed, 1 = up 2 = down)
// item[x][12] draw lines always, never

void mwItem::change_linked_door_color_and_draw_type(int door)
{
   // changes all linked doors to match door color and shape
   // first do the linked door if there is one...
   if (item[door][8] == 1)
   {
      int di = item[door][9];      // dest item
      item[di][1] = item[door][1]; // set shape
      item[di][6] = item[door][6]; // set color
   }

   // then look for any doors that link to this door
   for (int i=0; i<500; i++)
      if ((item[i][0] == 1) && (item[i][9] == door))
      {
         item[i][1] = item[door][1]; // set shape
         item[i][6] = item[door][6]; // set color
      }
}

int mwItem::draw_door(int i, int x, int y, int custom)
{
   ALLEGRO_BITMAP *tmp = NULL;
   int col = item[i][6];
   int drawn = 0;

   if (mItem.item[i][1] == 1) tmp = mBitmap.door_tile[1][col][0]; // old style door static shape

   else if (mItem.item[i][1] == 2) // new style door animation sequence
   {
      int tmr = mLoop.frame_num;
      if (mLoop.level_editor_running) tmr = al_get_timer_count(mEventQueue.fps_timer);
      int an = (tmr % 16) / 2;
      if (item[i][8] == 0) an = 7-an;  // exit only, run the sequence backwards
      tmp = mBitmap.door_tile[0][col][an];
   }
   else if (item[i][1] == 0) drawn = 1;


   if ((item[i][8] == 1) && (!custom)) // linked item destination
   {
      // linked destination item position
      int dx = itemf[item[i][9]][0];
      int dy = itemf[item[i][9]][1];
      int line_color = item[item[i][9]][6];

      if (item[i][12] == 1) // always draw line
         al_draw_line(x+10, y+10, dx+10, dy+10, mColor.pc[line_color], 1);  // draw a line connecting them

      if (!drawn)
      {
         for (int p=0; p<NUM_PLAYERS; p++) // is player touching door
            if ((mPlayer.syn[p].active) && (mPlayer.syn[p].marked_door == i))
            {
               if (item[i][12] > 0)  // always draw or only draw when touching ( 1 or 2)
                  al_draw_line(x+10, y+10, dx+10, dy+10, mColor.pc[line_color], 1);  // draw a line connecting them

               // bigger door when player touching it
               al_draw_scaled_bitmap(tmp, 0, 0, 20, 20, x-5, y-6, 30, 26, 0 );

               if (item[i][8] == 0) al_draw_scaled_bitmap(mBitmap.tile[1015], 0, 0, 20, 20, x-5, y-6, 30, 26, 0); // OUT
               else                 al_draw_scaled_bitmap(mBitmap.tile[1014], 0, 0, 20, 20, x-5, y-6, 30, 26, 0); // IN

               if (item[i][11] == 1) // enter with <up>
                  al_draw_text(mFont.pr8, mColor.pc[15],  x+3, y-14, 0, "up");

               if (item[i][11] == 2) // enter with <down>
                  al_draw_text(mFont.pr8, mColor.pc[15],  x-5, y-14, 0, "down");
               drawn = 1;
            }
      }
   }
   if (!drawn)
   {
      al_draw_bitmap(tmp, x, y, 0); // if not drawn yet
      if (item[i][8] == 0) al_draw_bitmap(mBitmap.tile[1015], x, y, 0); // OUT
      else                 al_draw_bitmap(mBitmap.tile[1014], x, y, 0); // IN
   }
   return 1;
}

void mwItem::proc_door_collision(int p, int i)
{
   if ((mPlayer.syn[p].marked_door == -1)  // player has no marked door yet
     && (mPlayer.syn[p].carry_item != i+1)) // player is not carrying this door
   {


      if (item[i][8]) // do nothing if exit only
      {
         mPlayer.syn[p].marked_door = i;
         int do_entry = 0;
         if (item[i][11] == 0) do_entry = 1; // enter immed
         if (mPlayer.syn[p].carry_item-1 != i) // cant trigger entry if carrying this door
         {
            if (item[i][11] == 1) // enter with <up>
            {
               // to prevent immediate triggering when destination door, wait for release and re-press

               // if key held is old, ignore
               if (item[i][10] && item[i][10] < mLoop.frame_num-1) item[i][10] = 0;

               // up pressed and !pressed last frame
               if ((mPlayer.syn[p].up) && (!item[i][10])) do_entry = 1;

               if (mPlayer.syn[p].up) item[i][10] = mLoop.frame_num;
               else item[i][10] = 0;

            }

            if (item[i][11] == 2) // enter with <down>
            {
               // to prevent immediate triggering when destination door, wait for release and re-press

               // if key held is old, ignore
               if (item[i][10] && item[i][10] < mLoop.frame_num-1) item[i][10] = 0;

               // down pressed and !pressed last frame
               if ((mPlayer.syn[p].down) && (!item[i][10])) do_entry = 1;

               if (mPlayer.syn[p].down) item[i][10] = mLoop.frame_num;
               else item[i][10] = 0;
            }
         }

         if (do_entry)
         {
            int bad_exit = 0;
            item[i][10] = 0; // clear the key hold for the door you just left

           // check if dest item is same as source item
            if (item[i][9] == i) bad_exit = 1;

            // is player carrying an item ?
            if (mPlayer.syn[p].carry_item)
            {
                int ci = mPlayer.syn[p].carry_item - 1;
                //printf("do entry, player is carrying item:%d\n", ci);

               // check to see if player is carrying this door
                if (ci == i) player_drop_item(p, ci);

               // check to see if player is carrying an item without the carry through door flag set
               if (item[ci][3] != -2)  player_drop_item(p, ci);
            }

            // get the destination
            float dx=0, dy=0;
            int li = item[i][9]; // linked item number
            if ((li > -1) && (li < 500))
            {
               dx = itemf[li][0];
               dy = itemf[li][1];
            }
            else bad_exit = 1;

            if ((dx < 0) || (dx > 1980)) bad_exit = 1;
            if ((dy < 0) || (dy > 1980)) bad_exit = 1;

            if (!bad_exit)
            {
               int col = item[i][6];
               int type = 1;
               if (item[i][13] == 1083) type = 0;

               if (item[i][13]) mGameEvent.add(22, 0, 0, p, i, col, type); // no event if door is invisible

               int instant_move = 0;

               if (item[i][7] == 1) instant_move = 1; // 1 = force instant
               if (item[i][7] == 2) instant_move = 0; // 2 = force move

               if (mPlayer.is_player_riding_rocket(p)) instant_move = 1; // 1 = force instant if riding rocket

               if (instant_move)
               {
                  mPlayer.syn[p].x = itemf[li][0];
                  mPlayer.syn[p].y = itemf[li][1];

                  if (mPlayer.is_player_riding_rocket(p))
                  {
                     int c = mPlayer.syn[p].carry_item-1;
                     itemf[c][0] = mPlayer.syn[p].x;
                     itemf[c][1] = mPlayer.syn[p].y;
                  }

                  item[li][10] = mLoop.frame_num;
               }
               else
               {
                  // snap player to the source door
                  mPlayer.syn[p].x = itemf[i][0];
                  mPlayer.syn[p].y = itemf[i][1];

                  mPlayer.syn[p].right_xinc = 0;
                  mPlayer.syn[p].left_xinc  = 0;

                  // set player's xinc and yinc
                  float xlen = mPlayer.syn[p].x - dx;                 // get the x distance between player and exit
                  float ylen = mPlayer.syn[p].y - dy;                 // get the y distance between player and exit
                  float hy_dist = sqrt(pow(xlen, 2) + pow(ylen, 2));  // hypotenuse distance
                  float speed = 15;                                   // speed
                  float scaler = hy_dist / speed;                     // get scaler (time)
                  mPlayer.syn[p].door_xinc = xlen / scaler;           // calc xinc
                  mPlayer.syn[p].door_yinc = ylen / scaler;           // calc yinc

                  // set players rotation from xinc, yinc
                  mPlayer.syn[p].door_draw_rot = atan2(mPlayer.syn[p].door_yinc, mPlayer.syn[p].door_xinc) - ALLEGRO_PI/2;

                  int num_steps = scaler;

                  if ((num_steps > 0) && (num_steps < 2000))
                  {
                     int ddrns = mPlayer.syn[p].door_draw_rot_num_steps = 12;
                     mPlayer.syn[p].door_num_steps = num_steps;
                     mPlayer.syn[p].paused = 1;
                     mPlayer.syn[p].paused_type = 2;
                     mPlayer.syn[p].paused_mode = 1;
                     mPlayer.syn[p].paused_mode_count = ddrns;
                     mPlayer.syn[p].door_item = i;
                     mPlayer.syn[p].door_draw_rot_inc = mPlayer.syn[p].door_draw_rot / ddrns;
                  }
                  // printf("ns:%d xinc:%3.2f yinc:%3.2f \n", num_steps, mPlayer.syn[p].xinc, mPlayer.syn[p].yinc);
               } // end of if not instant move
            }  // end of if not bad exit
         } // end of do entry
      } // end of if not exit only
   } // end of if not first door touched
}


void mwItem::proc_player_door_move(int p)
{
   int x = mPlayer.syn[p].door_item;
   int ddrns = mPlayer.syn[p].door_draw_rot_num_steps;
   float amount_to_shrink = 0.5;
   float sa = amount_to_shrink / ddrns;  // shrink and grow player inc

   if (mPlayer.syn[p].paused_mode == 1) // mode 1: enter door
   {
      mPlayer.syn[p].paused_mode_count--;
      if (mPlayer.syn[p].paused_mode_count)
      {
         mPlayer.syn[p].draw_scale -= sa; // shrink player
         mPlayer.syn[p].draw_rot += mPlayer.syn[p].door_draw_rot_inc; // rotate player
      }
      else // next mode
      {
         mPlayer.syn[p].paused_mode = 2;
         mPlayer.syn[p].paused_mode_count = mPlayer.syn[p].door_num_steps;
         mPlayer.syn[p].xinc = mPlayer.syn[p].door_xinc;
         mPlayer.syn[p].yinc = mPlayer.syn[p].door_yinc;
         mPlayer.syn[p].draw_rot = mPlayer.syn[p].door_draw_rot;
      }
   }

   if (mPlayer.syn[p].paused_mode == 2) // mode 2: player move
   {
      mPlayer.syn[p].paused_mode_count--;
      if (mPlayer.syn[p].paused_mode_count)
      {
         mPlayer.syn[p].x -= mPlayer.syn[p].xinc;
         mPlayer.syn[p].y -= mPlayer.syn[p].yinc;
      }
      else // mode 2 done
      {
         mPlayer.syn[p].paused_mode = 3;
         mPlayer.syn[p].paused_mode_count = ddrns;

         mPlayer.syn[p].xinc=0;
         mPlayer.syn[p].yinc=0;


         // snap to dest...
         if (item[x][8] == 0) // regular dest
         {
            mPlayer.syn[p].x  = item[x][6] * 20;
            mPlayer.syn[p].y  = item[x][7] * 20;
         }
         if (item[x][8] == 1) // linked item dest
         {
            int li = item[x][9]; // linked item number
            mPlayer.syn[p].x  = itemf[li][0];
            mPlayer.syn[p].y  = itemf[li][1];
            // set destination key held to prevent immediate retriggering
            item[li][10] = mLoop.frame_num;
         }
       }
   } // end of mode 2

   if (mPlayer.syn[p].paused_mode == 3) // mode 3: exit
   {
      mPlayer.syn[p].paused_mode_count--;
      if (mPlayer.syn[p].paused_mode_count)
      {
         mPlayer.syn[p].draw_scale += sa;                             // un-shrink player
         mPlayer.syn[p].draw_rot -= mPlayer.syn[p].door_draw_rot_inc; // un-rotate player
      }
      else // next mode (done)
      {
         // if linked item exit
         if (item[x][8] == 1) // liked item dest
         {
            int li = item[x][9]; // linked item number
            item[li][10] = mLoop.frame_num;   // key hold off
         }
         mPlayer.syn[p].paused = 0;  // the entire thing is done
      }
   }
   proc_player_carry(p);
}
