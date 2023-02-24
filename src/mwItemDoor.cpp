// mwItemDoor.cpp

#include "pm.h"
#include "mwItems.h"
#include "mwPlayers.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwColor.h"
#include "mwProgramState.h"
#include "z_screen_overlay.h"

void mwItems::change_linked_door_color_and_shape(int door)
{
   // changes all linked doors to match door color and shape
   // first do the linked door if there is one...
   if (mItem.item[door][8] == 1)
   {
      int di = mItem.item[door][9]; // dest item
      mItem.item[di][6] = mItem.item[door][6]; // set color
      mItem.item[di][13] = mItem.item[door][13]; // set shape
      mItem.item[di][1]  = mItem.item[door][13]; // also set in [1]
   }

   // then look for any doors that link to this door
   for (int i=0; i<500; i++)
      if ((mItem.item[i][0] == 1) && (mItem.item[i][9] == door))
      {
         mItem.item[i][6] = mItem.item[door][6]; // set color
         mItem.item[i][13] = mItem.item[door][13]; // set shape
         mItem.item[i][1]  = mItem.item[door][13]; // also set in [1]
      }
}

int mwItems::draw_door(int i, int x, int y, int custom)
{
   ALLEGRO_BITMAP *tmp = NULL;
   int col = mItem.item[i][6];
   int drawn = 0;
   if (mItem.item[i][13] == 448) // old style door shape
   {
      int shape = mItem.item[i][1];       // get shape
      int si = shape-448;           // convert to index to bitmap sequence
      tmp = mwB.door_tile[1][col][si];
   }
   else if (mItem.item[i][13] == 1083) // new style doors
   {
      int an = mwB.zz[1][83];             // cheat and use shape index from base door animation sequence
      if (mItem.item[i][8] == 0) an = 7-an; // exit only, run the sequence backwards
      tmp = mwB.door_tile[0][col][an];
   }
   else if (mItem.item[i][13] == 0) drawn = 1;

   if ((mItem.item[i][8] == 1) && (!custom)) // linked item destination
   {
      // linked destination item position
      int dx = itemf[mItem.item[i][9]][0];
      int dy = itemf[mItem.item[i][9]][1];
      int line_color = mItem.item[mItem.item[i][9]][6];

      if (mItem.item[i][12] == 1) // if always draw lines (1)
         al_draw_line(x+10, y+10, dx+10, dy+10, mC.pc[line_color], 1);  // draw a line connecting them

      if (!drawn)
      {
         for (int p=0; p<NUM_PLAYERS; p++) // is player touching door
            if ((mPlayer.syn[p].active) && (mPlayer.syn[p].marked_door == i))
            {
               if (mItem.item[i][12] > 0)  // always draw or only draw when touching ( 1 or 2)
                  al_draw_line(x+10, y+10, dx+10, dy+10, mC.pc[line_color], 1);  // draw a line connecting them

               // bigger door when player touching it
               al_draw_scaled_bitmap(tmp, 0, 0, 20, 20, x-5, y-6, 30, 26, 0 );

               if (mItem.item[i][8] == 0) al_draw_scaled_bitmap(mwB.tile[1015], 0, 0, 20, 20, x-5, y-6, 30, 26, 0); // OUT
               else al_draw_scaled_bitmap(mwB.tile[1014], 0, 0, 20, 20, x-5, y-6, 30, 26, 0); // IN

               if (mItem.item[i][11] == 1) // enter with <up>
                  al_draw_text(mF.pr8, mC.pc[15],  x+3, y-14, 0, "up");

               if (mItem.item[i][11] == 2) // enter with <down>
                  al_draw_text(mF.pr8, mC.pc[15],  x-5, y-14, 0, "down");
               drawn = 1;
            }
      }
   }
   if (!drawn)
   {
      al_draw_bitmap(tmp, x, y, 0); // if not drawn yet

      if (mItem.item[i][8] == 0) al_draw_bitmap(mwB.tile[1015], x, y, 0); // OUT
      else al_draw_bitmap(mwB.tile[1014], x, y, 0); // IN
   }
   return 1;
}

void mwItems::proc_door_collision(int p, int i)
{

   if ((mPlayer.syn[p].marked_door == -1)  // player has no marked door yet
     && (mPlayer.syn[p].carry_item != i+1)) // player is not carrying this door
   {

      // mItem.item[x][6]  color
      // mItem.item[x][7]  move type (0 = auto, 1 = force instant, 2 = force move
      // mItem.item[x][8]  type (0 = no dest (exit only), 1 = linked dest
      // mItem.item[x][9]  linked destination item
      // mItem.item[x][10] key held flag
      // mItem.item[x][11] door entry type (0 0-immed, 1 = up 2 = down)
      // mItem.item[x][12] draw lines always, never
      // mItem.item[x][13] base animation shape

      if (mItem.item[i][8]) // do nothing if exit only
      {
         mPlayer.syn[p].marked_door = i;
         int do_entry = 0;
         if (mItem.item[i][11] == 0) do_entry = 1; // enter immed
         if (mPlayer.syn[p].carry_item-1 != i) // cant trigger entry if carrying this door
         {
            if (mItem.item[i][11] == 1) // enter with <up>
            {
               // to prevent immediate triggering when destination door, wait for release and re-press

               // if key held is old, ignore
               if (mItem.item[i][10] && mItem.item[i][10] < mwPS.frame_num-1) mItem.item[i][10] = 0;

               // up pressed and !pressed last frame
               if ((mPlayer.syn[p].up) && (!mItem.item[i][10])) do_entry = 1;

               if (mPlayer.syn[p].up) mItem.item[i][10] = mwPS.frame_num;
               else mItem.item[i][10] = 0;

            }

            if (mItem.item[i][11] == 2) // enter with <down>
            {
               // to prevent immediate triggering when destination door, wait for release and re-press

               // if key held is old, ignore
               if (mItem.item[i][10] && mItem.item[i][10] < mwPS.frame_num-1) mItem.item[i][10] = 0;

               // down pressed and !pressed last frame
               if ((mPlayer.syn[p].down) && (!mItem.item[i][10])) do_entry = 1;

               if (mPlayer.syn[p].down) mItem.item[i][10] = mwPS.frame_num;
               else mItem.item[i][10] = 0;
            }
         }

         if (do_entry)
         {
            int bad_exit = 0;
            mItem.item[i][10] = 0; // clear the key hold for the door you just left

           // check if dest item is same as source item
            if (mItem.item[i][9] == i) bad_exit = 1;

            // is player carrying an item ?
            if (mPlayer.syn[p].carry_item)
            {
                int ci = mPlayer.syn[p].carry_item - 1;
                //printf("do entry, player is carrying item:%d\n", ci);

               // check to see if player is carrying this door
                if (ci == i) player_drop_item(p, ci);

               // check to see if player is carrying an item without the carry through door flag set
               if (mItem.item[ci][3] != -2)  player_drop_item(p, ci);
            }

            // get the destination
            float dx=0, dy=0;
            int li = mItem.item[i][9]; // linked item number
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
               if (mItem.item[i][13]) game_event(5, 0, 0, p, i, 0, 0); // no event if door is invisible

               int instant_move = 0;
               if (mItem.item[i][7] == 0) // 0 = auto
                  if (mItem.item[li][3]) // if dest is not stat
                     instant_move = 1;

               if (mItem.item[i][7] == 1) instant_move = 1; // 1 = force instant
               if (mItem.item[i][7] == 2) instant_move = 0; // 2 = force move

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

                  mItem.item[li][10] = mwPS.frame_num;
               }
               else
               {
                  // snap player to the source door
                  mPlayer.syn[p].x = itemf[i][0];
                  mPlayer.syn[p].y = itemf[i][1];

                  mPlayer.syn[p].right_xinc = 0;
                  mPlayer.syn[p].left_xinc  = 0;

                  // set player's xinc and yinc
                  float xlen = mPlayer.syn[p].x - dx;     // get the x distance between player and exit
                  float ylen = mPlayer.syn[p].y - dy;     // get the y distance between player and exit
                  float hy_dist = sqrt(pow(xlen, 2) + pow(ylen, 2));  // hypotenuse distance
                  float speed = 15;                        // speed
                  float scaler = hy_dist / speed;          // get scaler
                  mPlayer.syn[p].door_xinc = xlen / scaler;    // calc xinc
                  mPlayer.syn[p].door_yinc = ylen / scaler;    // calc yinc

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


void mwItems::proc_player_door_move(int p)
{
   int x = mPlayer.syn[p].door_item;
   int ddrns = mPlayer.syn[p].door_draw_rot_num_steps;
   float amount_to_shrink = 0.5;
   float sa = amount_to_shrink / ddrns;  // shrink and grow player inc
   int as = 7; // door open/close animation speed

   if (mPlayer.syn[p].paused_mode == 1) // mode 1: enter door
   {
      mPlayer.syn[p].paused_mode_count--;
      if (mPlayer.syn[p].paused_mode_count)
      {
         mPlayer.syn[p].draw_scale -= sa; // shrink player
         mPlayer.syn[p].draw_rot += mPlayer.syn[p].door_draw_rot_inc; // rotate player

         // open door
         float ratio = 1 - (float) mPlayer.syn[p].paused_mode_count / ddrns;
         float shape_shift = ratio * as;

         if (mItem.item[x][13] == 448)
            mItem.item[x][1] = 448 + (int)shape_shift;
//            printf("ratio:%f scaled_ratio%f int_sr:%d shape:%d \n", ratio, shape_shift, int(shape_shift), mItem.item[x][1] );

      }
      else // next mode
      {
         if (mItem.item[x][13] == 448) mItem.item[x][1] = 448; // restore door shape
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
         if (mItem.item[x][8] == 0) // regular dest
         {
            mPlayer.syn[p].x  = mItem.item[x][6] * 20;
            mPlayer.syn[p].y  = mItem.item[x][7] * 20;
         }
         if (mItem.item[x][8] == 1) // linked item dest
         {
            int li = mItem.item[x][9]; // linked item number
            mPlayer.syn[p].x  = itemf[li][0];
            mPlayer.syn[p].y  = itemf[li][1];
            // set destination key held to prevent immediate retriggering
            mItem.item[li][10] = mwPS.frame_num;
         }
       }
   } // end of mode 2

   if (mPlayer.syn[p].paused_mode == 3) // mode 3: exit
   {
      mPlayer.syn[p].paused_mode_count--;
      if (mPlayer.syn[p].paused_mode_count)
      {
         mPlayer.syn[p].draw_scale += sa;                         // un-shrink player
         mPlayer.syn[p].draw_rot -= mPlayer.syn[p].door_draw_rot_inc; // un-rotate player
         // close door (only if linked item exit)
         if (mItem.item[x][8] == 1)    // if linked item exit
         {
            float ratio = 1 - (float) mPlayer.syn[p].paused_mode_count / ddrns;
            float shape_shift = ratio * 6 ;
            int li = mItem.item[x][9]; // linked item number
            if (mItem.item[li][13] == 448)
               mItem.item[li][1] = 454 - (int)shape_shift;
//                 printf("ratio:%f scaled_ratio%f int_sr:%d shape:%d \n", ratio, shape_shift, int(shape_shift), mItem.item[li][1] );

         }
      }
      else // next mode (done)
      {
         // if linked item exit
         if (mItem.item[x][8] == 1) // liked item dest
         {
            int li = mItem.item[x][9]; // linked item number
            if (mItem.item[li][13] == 448)
               mItem.item[li][1] = 448;   // restore door shape
            mItem.item[li][10] = mwPS.frame_num;   // key hold off

         }
         mPlayer.syn[p].paused = 0;  // the entire thing is done
      }
   }
   proc_player_carry(p);
}
