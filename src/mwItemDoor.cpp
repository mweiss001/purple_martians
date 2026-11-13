// mwItemDoor.cpp

#include "pm.h"
#include "mwLevel.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwColor.h"
#include "mwLoop.h"
#include "mwGameEvent.h"
#include "mwEventQueue.h"

// item[x][1]  draw type (0=hidden, 1=static door, 2=animated warp, 3=portal)
// item[x][6]  color
// item[x][7]  move type (0 = force instant, 1 = force move
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



   if (item[i][1] == 1) tmp = mBitmap.door_tile[1][col][0]; // old style door static shape

   else if (item[i][1] == 2) // new style door animation sequence
   {
      int num_shapes = 8;
      int frames_per_cycle = 4;

      int tmr = mLoop.frame_num;
      if (mLoop.level_editor_running) tmr = al_get_timer_count(mEventQueue.fps_timer);
      int an = (tmr % (num_shapes*frames_per_cycle)) / frames_per_cycle;
      if (item[i][8] == 0) an = (num_shapes-1)-an;  // exit only, run the sequence backwards
      tmp = mBitmap.door_tile[0][col][an];
   }
   else if (item[i][1] == 0) drawn = 1;

   else if (item[i][1] == 3) // portal
   {
      // is a player currently using this door










      int num_shapes = 4;
      int frames_per_cycle = 4;

      int tmr = mLoop.frame_num;
      if (mLoop.level_editor_running) tmr = al_get_timer_count(mEventQueue.fps_timer);
      int an = (tmr % (num_shapes*frames_per_cycle)) / frames_per_cycle;
//      if (item[i][8] == 0) an = (num_shapes-1)-an;  // exit only, run the sequence backwards


      // center
      int cx = x + 10;
      int cy = y + 10;
      int sz = 64;
      int tx = cx - sz/2;
      int ty = cy - sz/2;

      al_draw_scaled_bitmap(mBitmap.portal_tile[an], 0,0,64,64, tx, ty, sz, sz, 0);


//      al_draw_bitmap(mBitmap.portal_tile[0], tx, ty, 0);

//      if (item[i][8] == 0) al_draw_bitmap(mBitmap.sprite[1015], x, y, 0); // OUT
//      else                 al_draw_bitmap(mBitmap.sprite[1014], x, y, 0); // IN


      drawn = 1;
      //return 1;
   }




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
            if ((m_parent.mPlayer.syn[p].active) && (m_parent.mPlayer.syn[p].marked_door == i))
            {
               if (item[i][12] > 0)  // always draw or only draw when touching ( 1 or 2)
                  al_draw_line(x+10, y+10, dx+10, dy+10, mColor.pc[line_color], 1);  // draw a line connecting them

               // bigger door when player touching it
               al_draw_scaled_bitmap(tmp, 0, 0, 20, 20, x-5, y-6, 30, 26, 0 );

               if (item[i][8] == 0) al_draw_scaled_bitmap(mBitmap.sprite[1015], 0, 0, 20, 20, x-5, y-6, 30, 26, 0); // OUT
               else                 al_draw_scaled_bitmap(mBitmap.sprite[1014], 0, 0, 20, 20, x-5, y-6, 30, 26, 0); // IN

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
      if (item[i][8] == 0) al_draw_bitmap(mBitmap.sprite[1015], x, y, 0); // OUT
      else                 al_draw_bitmap(mBitmap.sprite[1014], x, y, 0); // IN
   }
   return 1;
}

void mwItem::proc_door_collision(int p, int i)
{
   // do nothing if exit only
   if (item[i][8] == 0) return;

   // this is to make sure player can only be on one door
   if (m_parent.mPlayer.syn[p].marked_door != -1) return;

   // this is to prevent player from entering door they are carrying
   if (m_parent.mPlayer.syn[p].carry_item == i+1) return;

   // mark this door at the one door the player is on
   m_parent.mPlayer.syn[p].marked_door = i;


   int do_entry = 0;

   // enter immediately
   if (item[i][11] == 0) do_entry = 1;


   // enter with <up>
   if (item[i][11] == 1)
   {
      // to prevent immediate triggering when destination door, wait for release and re-press

      // if key held is old, ignore
      if (item[i][10] && item[i][10] < mLoop.frame_num-1) item[i][10] = 0;

      // up pressed and !pressed last frame
      if ((m_parent.mPlayer.syn[p].up) && (!item[i][10])) do_entry = 1;

      if (m_parent.mPlayer.syn[p].up) item[i][10] = mLoop.frame_num;
      else item[i][10] = 0;

   }

   // enter with <down>
   if (item[i][11] == 2)
   {
      // to prevent immediate triggering when destination door, wait for release and re-press

      // if key held is old, ignore
      if (item[i][10] && item[i][10] < mLoop.frame_num-1) item[i][10] = 0;

      // down pressed and !pressed last frame
      if ((m_parent.mPlayer.syn[p].down) && (!item[i][10])) do_entry = 1;

      if (m_parent.mPlayer.syn[p].down) item[i][10] = mLoop.frame_num;
      else item[i][10] = 0;
   }



   if (do_entry)
   {
      // clear the key hold for the door you just left
      item[i][10] = 0;

      // check if dest item is same as source item
      if (item[i][9] == i) return;

      // send event unless door is invisible
      if (item[i][1] != 0) mGameEvent.add(22, 0, 0, p, i, item[i][6], 1);


      // check if player needs to drop carried item
      if (m_parent.mPlayer.syn[p].carry_item)
      {
         // index of the carried item
         int ci = m_parent.mPlayer.syn[p].carry_item - 1;

         // drop if player is carrying this door
         if (ci == i) player_drop_item(p, ci);

         // drop if carry through door flag is not set
         if (!is_item_carryable_through_door(ci)) player_drop_item(p, ci);
      }


      // linked door item number
      int li = item[i][9];
      if ((li < 0) || (li > 499)) return;

      // destination
      float dx=0, dy=0;
      dx = itemf[li][0];
      dy = itemf[li][1];
      if ((dx < 0) || (dx > 1980)) return;
      if ((dy < 0) || (dy > 1980)) return;




      // instant move or travel
      int travel = item[i][7];

      // force instant if riding rocket
      if (m_parent.mPlayer.is_player_riding_rocket(p)) travel = 0;

      if (travel)
      {
         // snap player to the source door
         m_parent.mPlayer.syn[p].x = itemf[i][0];
         m_parent.mPlayer.syn[p].y = itemf[i][1];

         // stop player's motion
         m_parent.mPlayer.syn[p].right_xinc = 0;
         m_parent.mPlayer.syn[p].left_xinc  = 0;

         // mark source door item in player struct
         m_parent.mPlayer.syn[p].door_item = i;

         // number of steps in enter and exit modes
         m_parent.mPlayer.syn[p].door_draw_rot_num_steps = 12;

         // set paused and paused type 2 (door move)
         m_parent.mPlayer.syn[p].paused = 1;
         m_parent.mPlayer.syn[p].paused_type = PM_PLAYER_PAUSED_TYPE_DOOR_MOVE;

         // set initial mode
         m_parent.mPlayer.syn[p].paused_mode = 1;

         // initial mode num steps
         m_parent.mPlayer.syn[p].paused_mode_count = m_parent.mPlayer.syn[p].door_draw_rot_num_steps;


         // get the x and y distances between player and exit
         float xlen = m_parent.mPlayer.syn[p].x - dx;
         float ylen = m_parent.mPlayer.syn[p].y - dy;

         // hypotenuse distance
         float hy_dist = sqrt(pow(xlen, 2) + pow(ylen, 2));

         // number of steps is distance / speed
         float num_steps = hy_dist / 15;

         // save integer number of steps for travel mode
         m_parent.mPlayer.syn[p].door_num_steps = num_steps;

         // calc x and y incs
         m_parent.mPlayer.syn[p].door_xinc = xlen / num_steps;
         m_parent.mPlayer.syn[p].door_yinc = ylen / num_steps;

         // set players rotation from xinc, yinc
         m_parent.mPlayer.syn[p].door_draw_rot = atan2(m_parent.mPlayer.syn[p].door_yinc, m_parent.mPlayer.syn[p].door_xinc) - ALLEGRO_PI/2;

         // initial mode rotation increment
         m_parent.mPlayer.syn[p].door_draw_rot_inc = m_parent.mPlayer.syn[p].door_draw_rot / m_parent.mPlayer.syn[p].door_draw_rot_num_steps;

      }
      else // instant move
      {
         // immediately set the player's position to the exit door
         m_parent.mPlayer.syn[p].x = itemf[li][0];
         m_parent.mPlayer.syn[p].y = itemf[li][1];

         // if riding rocket, set the rocket to the new position also
         if (m_parent.mPlayer.is_player_riding_rocket(p))
         {
            int c = m_parent.mPlayer.syn[p].carry_item-1;
            itemf[c][0] = m_parent.mPlayer.syn[p].x;
            itemf[c][1] = m_parent.mPlayer.syn[p].y;
         }

         // reset the ket held flag
         item[li][10] = mLoop.frame_num;
      }
   }
}


void mwItem::proc_player_door_move(int p)
{
   int door_item = m_parent.mPlayer.syn[p].door_item;
   
   float amount_to_shrink = 0.5;
   float shrink_scale_inc = amount_to_shrink / m_parent.mPlayer.syn[p].door_draw_rot_num_steps;  // shrink and grow player inc

   if (m_parent.mPlayer.syn[p].paused_mode == 1) // mode 1: enter door
   {
      m_parent.mPlayer.syn[p].paused_mode_count--;
      if (m_parent.mPlayer.syn[p].paused_mode_count)
      {
         m_parent.mPlayer.syn[p].draw_scale -= shrink_scale_inc; // shrink player
         m_parent.mPlayer.syn[p].draw_rot += m_parent.mPlayer.syn[p].door_draw_rot_inc; // rotate player
      }
      else // next mode
      {
         m_parent.mPlayer.syn[p].paused_mode = 2;
         m_parent.mPlayer.syn[p].paused_mode_count = m_parent.mPlayer.syn[p].door_num_steps;
         m_parent.mPlayer.syn[p].xinc = m_parent.mPlayer.syn[p].door_xinc;
         m_parent.mPlayer.syn[p].yinc = m_parent.mPlayer.syn[p].door_yinc;
         m_parent.mPlayer.syn[p].draw_rot = m_parent.mPlayer.syn[p].door_draw_rot;
      }
   }

   if (m_parent.mPlayer.syn[p].paused_mode == 2) // mode 2: player move
   {
      m_parent.mPlayer.syn[p].paused_mode_count--;
      if (m_parent.mPlayer.syn[p].paused_mode_count)
      {
         m_parent.mPlayer.syn[p].x -= m_parent.mPlayer.syn[p].xinc;
         m_parent.mPlayer.syn[p].y -= m_parent.mPlayer.syn[p].yinc;
      }
      else // mode 2 done
      {
         m_parent.mPlayer.syn[p].paused_mode = 3;
         m_parent.mPlayer.syn[p].paused_mode_count = m_parent.mPlayer.syn[p].door_draw_rot_num_steps;

         m_parent.mPlayer.syn[p].xinc=0;
         m_parent.mPlayer.syn[p].yinc=0;


         // snap to dest...
         if (item[door_item][8] == 0) // regular dest
         {
            m_parent.mPlayer.syn[p].x  = item[door_item][6] * 20;
            m_parent.mPlayer.syn[p].y  = item[door_item][7] * 20;
         }
         if (item[door_item][8] == 1) // linked item dest
         {
            int li = item[door_item][9]; // linked item number
            m_parent.mPlayer.syn[p].x  = itemf[li][0];
            m_parent.mPlayer.syn[p].y  = itemf[li][1];
            // set destination key held to prevent immediate retriggering
            item[li][10] = mLoop.frame_num;
         }
       }
   } // end of mode 2

   if (m_parent.mPlayer.syn[p].paused_mode == 3) // mode 3: exit
   {
      m_parent.mPlayer.syn[p].paused_mode_count--;
      if (m_parent.mPlayer.syn[p].paused_mode_count)
      {
         m_parent.mPlayer.syn[p].draw_scale += shrink_scale_inc;                             // un-shrink player
         m_parent.mPlayer.syn[p].draw_rot -= m_parent.mPlayer.syn[p].door_draw_rot_inc; // un-rotate player
      }
      else // next mode (done)
      {
         // if linked item exit
         if (item[door_item][8] == 1) // liked item dest
         {
            int li = item[door_item][9]; // linked item number
            item[li][10] = mLoop.frame_num;   // key hold off
         }
         m_parent.mPlayer.syn[p].paused = 0;  // the entire thing is done
      }
   }

   // player can still carry and drop item while in door move
   // can't throw or pick up, just drop
   proc_player_carry(p);
}
