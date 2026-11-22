// mwItemDoor.cpp

#include "pm.h"
#include "mwItem.h"
#include "mwLevel.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwColor.h"
#include "mwLoop.h"
#include "mwGameEvent.h"
#include "mwEventQueue.h"


/*

item[][0] 1 - PM_ITEM_TYPE_DOOR

item[][1] draw type
#define PM_ITEM_DOOR_DRAW_HIDE      0
#define PM_ITEM_DOOR_DRAW_STATIC    1
#define PM_ITEM_DOOR_DRAW_WARP      2
#define PM_ITEM_DOOR_DRAW_PORTAL    3

item[][2] common item flags
item[][3] unused
item[][4] x
item[][5] y
item[][6] color


item[][7]  move type (0=INSTANT 1=TRAVEl)
item[][8]  unused
item[][9]  linked destination item (-1 is none)
item[][10] key held flag


item[][11] enter mode
#define PM_ITEM_DOOR_ENTER_IMMEDIATE  0
#define PM_ITEM_DOOR_ENTER_WITH_UP    1
#define PM_ITEM_DOOR_ENTER_WITH_DOWN  2

item[][12] draw connecting line
#define PM_ITEM_DOOR_LINE_NEVER   0
#define PM_ITEM_DOOR_LINE_ALWAYS  1
#define PM_ITEM_DOOR_LINE_TOUCH   2

item[][13] scale (100 = 1.0)



*/


void mwItem::change_linked_door_color_and_draw_type(int door)
{
   // changes all linked doors to match door color, shape and size
   // first do the linked door if there is one...
   if (item[door][9] != -1)
   {
      int di = item[door][9];        // dest item
      item[di][1]  = item[door][1];  // copy draw type
      item[di][6]  = item[door][6];  // copy color
      item[di][13] = item[door][13]; // copy size

   }

   // then look for any doors that link to this door
   for (int i=0; i<500; i++)
      if ((item[i][0] == PM_ITEM_TYPE_DOOR) && (item[i][9] == door))
      {
         item[i][1]  = item[door][1];  // copy type
         item[i][6]  = item[door][6];  // copy color
         item[i][13] = item[door][13]; // copy size
      }
}


int mwItem::find_source_door(int search_index)
{
   for (int i=0; i<500; i++)
      if ((item[i][0] == PM_ITEM_TYPE_DOOR) && (item[i][9] == search_index)) return i;
   return -1;
}



void mwItem::draw_door_tile(int x, int y, int draw_type, int col, int ans, float scale, float rot)
{
   float yo = ((20*scale) - 20)/2;
   if (draw_type == PM_ITEM_DOOR_DRAW_STATIC) al_draw_scaled_rotated_bitmap(mBitmap.sprite[159+col], 10, 10, x+10, y+10-yo, scale, scale, 0, 0 );
   if (draw_type == PM_ITEM_DOOR_DRAW_WARP)   al_draw_scaled_rotated_bitmap(mBitmap.door_tile[col][ans], 10, 10, x+10, y+10-yo, scale, scale, 0, 0 );
   if (draw_type == PM_ITEM_DOOR_DRAW_PORTAL) mBitmap.draw_portal(col, ans, x+10, y+10, scale, rot);
}



int mwItem::draw_door(int i, int x, int y, int custom)
{
   float scale = (float)item[i][13] / 100;

   int col = item[i][6];
   int draw_type = item[i][1];

   // this is for drawing in the point item and draw item boxes only
   if (custom)
   {
      // if invisible, show static shape
      if (draw_type == PM_ITEM_DOOR_DRAW_HIDE) draw_type = PM_ITEM_DOOR_DRAW_STATIC;

      // draw at scale 1, except for portals that are too big and scale .4 works better
      scale = 1;
      if (draw_type == PM_ITEM_DOOR_DRAW_PORTAL) scale = .4;

      draw_door_tile(x, y, draw_type, col, 0, scale, 0);

      // I want to also draw here if orphan

      if (item[i][9] == -1)
      {
         if (find_source_door(i) == -1)
         {
            al_draw_line(x, y, x+20, y+20, mColor.Red, 2);
            al_draw_line(x+20, y, x, y+20, mColor.Red, 2);
            al_draw_text(mFont.pixl, mColor.White, x+10, y,   ALLEGRO_ALIGN_CENTER, "no");
            al_draw_text(mFont.pixl, mColor.White, x+10, y+8, ALLEGRO_ALIGN_CENTER, "src");
         }
      }

      // if linked door is not valid, draw and mark with red X, then return
      if (item[i][9] != -1)
      {
         int linked_door = item[i][9];

         if (item[linked_door][0] != 1)
         {
            al_draw_text(mFont.pixl, mColor.White, x+10, y,   ALLEGRO_ALIGN_CENTER, "no");
            al_draw_text(mFont.pixl, mColor.White, x+10, y+8, ALLEGRO_ALIGN_CENTER, "dst");
            al_draw_line(x, y, x+20, y+20, mColor.Red, 2);
            al_draw_line(x+20, y, x, y+20, mColor.Red, 2);
            return 1;
         }
      }
      return 1;
   }


   // do not draw exit only doors, they will be drawn when source door is drawn
   // unless door is an orphan exit (no source door) in which case, draw and mark with red X, then return
   if (item[i][9] == -1)
   {
      if (find_source_door(i) == -1)
      {
         // if invisible, show static shape
         if (draw_type == PM_ITEM_DOOR_DRAW_HIDE) draw_type = PM_ITEM_DOOR_DRAW_STATIC;

         draw_door_tile(x, y, draw_type, col, 0, scale, 0);

         al_draw_line(x, y, x+20, y+20, mColor.Red, 2);
         al_draw_line(x+20, y, x, y+20, mColor.Red, 2);

         al_draw_text(mFont.pixl, mColor.White, x+10, y,   ALLEGRO_ALIGN_CENTER, "no");
         al_draw_text(mFont.pixl, mColor.White, x+10, y+8, ALLEGRO_ALIGN_CENTER, "src");

      }
      return 1;
   }

   // if linked door is not valid, draw and mark with red X, then return
   if (item[i][9] != -1)
   {
      int linked_door = item[i][9];

      if (item[linked_door][0] != 1)
      {
         // if invisible, show static shape
         if (draw_type == PM_ITEM_DOOR_DRAW_HIDE) draw_type = PM_ITEM_DOOR_DRAW_STATIC;
         draw_door_tile(x, y, draw_type, col, 0, scale, 0);
         al_draw_text(mFont.pixl, mColor.White, x+10, y,   ALLEGRO_ALIGN_CENTER, "no");
         al_draw_text(mFont.pixl, mColor.White, x+10, y+8, ALLEGRO_ALIGN_CENTER, "dst");
         al_draw_line(x, y, x+20, y+20, mColor.Red, 2);
         al_draw_line(x+20, y, x, y+20, mColor.Red, 2);
         return 1;
      }
   }

   // If I get here, I have 2 valid doors, if I didn't I should have quit by this point

   // source door
   int si = i;
   float sx = itemf[si][0];
   float sy = itemf[si][1];
   int scol = item[si][6];
   int s_draw_type = item[si][1];
   float s_scale = scale;

   int door_entry_mode = item[si][11];

   // destination door
   int di = item[i][9];
   float dx = itemf[di][0];
   float dy = itemf[di][1];
   int dcol = item[di][6];
   int d_draw_type = item[di][1];

   float d_scale = (float)item[di][13] / 100;


   // get rotation from source and destination door positions
   float rot = atan2(dy-sy, dx-sx);


   // determine if a player is hovering the source door
   bool player_hover = 0;
   for (int p=0; p<NUM_PLAYERS; p++)
      if ((m_parent.mPlayer.syn[p].active) && (m_parent.mPlayer.syn[p].door_hover == si)) player_hover = 1;

   // increase scale of source door if hovered
   if (player_hover) s_scale *= door_hover_scale_multiplier;


   // -------------------------------------------------------
   // static door
   // -------------------------------------------------------
   if (s_draw_type == PM_ITEM_DOOR_DRAW_STATIC) draw_door_tile(sx, sy, s_draw_type, scol, 0, s_scale, rot); // source door

   // only draw if the destination door has no exit link
   if (item[di][9] == -1) if (d_draw_type == PM_ITEM_DOOR_DRAW_STATIC) draw_door_tile(dx, dy, d_draw_type, dcol, 0, d_scale, rot); // dest door

   // -------------------------------------------------------
   // animated warp
   // -------------------------------------------------------
   if ((s_draw_type == PM_ITEM_DOOR_DRAW_WARP) || (d_draw_type == PM_ITEM_DOOR_DRAW_WARP))
   {
      int num_tiles = 10;
      int frames_per_cycle = 2;

      // get timer when game running
      int tmr = mLoop.frame_num;

      // get timer in level editor
      if (mLoop.level_editor_running) tmr = al_get_timer_count(mEventQueue.fps_timer);

      // animation seq indexes
      int san = (tmr % (num_tiles*frames_per_cycle)) / frames_per_cycle;
      int dan = (num_tiles-1)-san;  // run the sequence backwards

      if (s_draw_type == PM_ITEM_DOOR_DRAW_WARP) draw_door_tile(sx, sy, s_draw_type, scol, san, s_scale, rot);  // source door

      // only draw if the destination door has no exit link
      if (item[di][9] == -1) if (d_draw_type == PM_ITEM_DOOR_DRAW_WARP) draw_door_tile(dx, dy, d_draw_type, dcol, dan, d_scale, rot); // dest door








   }

   // -------------------------------------------------------
   // portal
   // -------------------------------------------------------
   if ((s_draw_type == PM_ITEM_DOOR_DRAW_PORTAL) || (d_draw_type == PM_ITEM_DOOR_DRAW_PORTAL))
   {

      // default animation seq indexes
      int san = 0;
      int dan = 0;

      // is any player currently using this door for a door move
      int p = m_parent.mPlayer.find_player_using_door(i);

      // idle
      if (p == -1)
      {
         int idle_start   =  0;
         int idle_length  =  4;

         // how many frames before next tile
         int frames_per_cycle = 4;

         // get timer when game running
         int tmr = mLoop.frame_num;

         // get timer in level editor
         if (mLoop.level_editor_running) tmr = al_get_timer_count(mEventQueue.fps_timer);

         // set both source and destination animation sequence based on timer mod
         san = dan = idle_start + (tmr % (idle_length*frames_per_cycle)) / frames_per_cycle;

      }
      else
      {
         int main_start   =  4;
         int main_length  = 11;

         // get the number of tiles in the main sequence
         float num_tiles = main_length-4;

         // by default use the last shape in the sequence
         san = main_start + num_tiles * 1; // source animation seq
         dan = main_start + num_tiles * 1; // dest animation seq

         // get the ratio of the progress in the mode
         float ratio = (float)m_parent.mPlayer.syn[p].paused_mode_count / (float)m_parent.mPlayer.syn[p].door_draw_rot_num_steps;

         // set the source and destination animation sequence based on the ratio
         if (m_parent.mPlayer.syn[p].paused_mode == 1) san = main_start + num_tiles * (1-ratio); // rotate in
         if (m_parent.mPlayer.syn[p].paused_mode == 3) dan = main_start + num_tiles * ratio;     // rotate out
      }

      // draw the source bitmap
      if (s_draw_type == PM_ITEM_DOOR_DRAW_PORTAL) draw_door_tile(sx, sy, s_draw_type, scol, san, s_scale, rot);

      // draw the destination bitmap
      if (item[di][9] == -1) if (d_draw_type == PM_ITEM_DOOR_DRAW_PORTAL) draw_door_tile(dx, dy, d_draw_type, dcol, dan, d_scale, rot + ALLEGRO_PI);

   }


   // increase y offset if door scale is larger
   int syo = ((20*s_scale) - 20)/2;
   int dyo = ((20*d_scale) - 20)/2;


   // draw a line connecting the doors
   bool draw_line = 0;
   if (item[i][12] == PM_ITEM_DOOR_LINE_ALWAYS) draw_line = 1; // always draw
   if (item[i][12] == PM_ITEM_DOOR_LINE_TOUCH && player_hover) draw_line = 1; // only draw when player touches
   if (item[i][12] == PM_ITEM_DOOR_LINE_NEVER) draw_line = 0; // never draw
   if (custom) draw_line = 0; // never draw in custom mode
   if (draw_line) al_draw_line(sx+10, sy+10-syo, dx+10, dy+10-dyo, mColor.pc[scol], 1);


   // draw the text last
   if ((player_hover) && (s_draw_type != PM_ITEM_DOOR_DRAW_HIDE))
   {
      if (door_entry_mode == PM_ITEM_DOOR_ENTER_WITH_UP)   al_draw_text(mFont.pr8, mColor.pc[15], sx+3, sy-14-syo, 0, "up");
      if (door_entry_mode == PM_ITEM_DOOR_ENTER_WITH_DOWN) al_draw_text(mFont.pr8, mColor.pc[15], sx-5, sy-14-syo, 0, "down");
   }


   if (s_draw_type != PM_ITEM_DOOR_DRAW_HIDE)
   {
      if (find_source_door(si) == -1) al_draw_scaled_rotated_bitmap(mBitmap.sprite[1014], 10, 10, sx+10, sy+10-syo, s_scale, s_scale, 0, 0 ); // IN

      // if this source door if also an exit for something else, draw I/O
      else al_draw_scaled_rotated_bitmap(mBitmap.sprite[1016], 10, 10, sx+10, sy+10-syo, s_scale, s_scale, 0, 0 ); // I/O
   }

   if (d_draw_type != PM_ITEM_DOOR_DRAW_HIDE)
   {
      // only draw 'OUT' if this destination door is NOT also a source door for something else (has an exit link)
      if (item[di][9] == -1) al_draw_scaled_rotated_bitmap(mBitmap.sprite[1015], 10, 10, dx+10, dy+10-dyo, d_scale, d_scale, 0, 0 ); // OUT
   }


   return 1;
}

void mwItem::proc_door_collision(int p, int i)
{
   // do nothing if exit only
   if (item[i][9] == -1) return;

   // this is to make sure player can only be hovering one door
   if (m_parent.mPlayer.syn[p].door_hover != -1) return;

   // this is to prevent player from entering door they are carrying
   if (m_parent.mPlayer.syn[p].carry_item == i+1) return;

   // mark this door as the one door the player is hovering
   m_parent.mPlayer.syn[p].door_hover = i;


   int do_entry = 0;

   if (item[i][11] == PM_ITEM_DOOR_ENTER_IMMEDIATE) do_entry = 1;

   if (item[i][11] == PM_ITEM_DOOR_ENTER_WITH_UP)
   {
      // to prevent immediate triggering when destination door, wait for release and re-press

      // if key held is old, ignore
      if (item[i][10] && item[i][10] < mLoop.frame_num-1) item[i][10] = 0;

      // up pressed and !pressed last frame
      if ((m_parent.mPlayer.syn[p].up) && (!item[i][10])) do_entry = 1;


      // set or clear key pressed last frame
      if (m_parent.mPlayer.syn[p].up) item[i][10] = mLoop.frame_num;
      else item[i][10] = 0;

   }

   if (item[i][11] == PM_ITEM_DOOR_ENTER_WITH_DOWN)
   {
      // to prevent immediate triggering when destination door, wait for release and re-press

      // if key held is old, ignore
      if (item[i][10] && item[i][10] < mLoop.frame_num-1) item[i][10] = 0;

      // down pressed and !pressed last frame
      if ((m_parent.mPlayer.syn[p].down) && (!item[i][10])) do_entry = 1;

      // set or clear key pressed last frame
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
      if (item[i][1] != PM_ITEM_DOOR_DRAW_HIDE) mGameEvent.add(22, 0, 0, p, i, item[i][6], 1);


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
      float dx = itemf[li][0];
      float dy = itemf[li][1];
      if ((dx < 0) || (dx > 1980)) return;
      if ((dy < 0) || (dy > 1980)) return;


      // instant move or travel
      int travel = item[i][7];

      // force instant if riding rocket
      if (m_parent.mPlayer.is_player_riding_rocket(p)) travel = 0;

      if (travel)
      {
         float s_scale = (float)item[i][13] / 100;

         // I would love to have this line here, things line up better, but it breaks demos...:(
         //s_scale *= door_hover_scale_multiplier;

         float syo = ((20*s_scale) - 20)/2;

         float d_scale = (float)item[li][13] / 100;
         float dyo = ((20*d_scale) - 20)/2;


         // snap player to the source door
         m_parent.mPlayer.syn[p].x = itemf[i][0];
         m_parent.mPlayer.syn[p].y = itemf[i][1];

         m_parent.mPlayer.syn[p].y = itemf[i][1]-syo;


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
         dy-=dyo;

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

         float d_scale = (float)item[door_item][13] / 100;
         float dyo = ((20*d_scale) - 20)/2;

         // snap to dest...
         if (item[door_item][9] == -1) // regular dest
         {
            m_parent.mPlayer.syn[p].x  = item[door_item][6] * 20;
            m_parent.mPlayer.syn[p].y  = item[door_item][7] * 20 - dyo;
         }
         if (item[door_item][9] != -1) // linked item dest
         {
            int li = item[door_item][9]; // linked item number
            m_parent.mPlayer.syn[p].x  = itemf[li][0];
            m_parent.mPlayer.syn[p].y  = itemf[li][1] - dyo;
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
         m_parent.mPlayer.syn[p].draw_scale += shrink_scale_inc;                        // un-shrink player
         m_parent.mPlayer.syn[p].draw_rot -= m_parent.mPlayer.syn[p].door_draw_rot_inc; // un-rotate player
      }
      else // next mode (done)
      {
         // if linked item exit
         if (item[door_item][9] != -1) // liked item dest
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
