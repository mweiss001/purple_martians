// z_item_door.cpp
#include "pm.h"
#include "z_item.h"
#include "z_item_door.h"

//#include "z_sound.h"
#include "z_player.h"
#include "mwFont.h"
#include "mwBitmap.h"
//#include "mwLift.h"
//#include "mwWidgets.h"
#include "mwColor.h"
//#include "mwPMEvent.h"
//#include "mwDisplay.h"
#include "mwProgramState.h"
//#include "z_menu.h"
//#include "z_enemy.h"
//#include "mwLevel.h"
//#include "e_fnx.h"
//
//#include "z_screen.h"
#include "z_screen_overlay.h"
//#include "mwShots.h"
//#include "z_solid.h"
//#include "z_item_key.h"
//#include "z_item_bomb.h"
//#include "z_item_pmsg.h"


void change_linked_door_color_and_shape(int door)
{
   // changes all linked doors to match door color and shape
   // first do the linked door if there is one...
   if (item[door][8] == 1)
   {
      int di = item[door][9]; // dest item
      item[di][6] = item[door][6]; // set color
      item[di][13] = item[door][13]; // set shape
      item[di][1]  = item[door][13]; // also set in [1]
   }

   // then look for any doors that link to this door
   for (int i=0; i<500; i++)
      if ((item[i][0] == 1) && (item[i][9] == door))
      {
         item[i][6] = item[door][6]; // set color
         item[i][13] = item[door][13]; // set shape
         item[i][1]  = item[door][13]; // also set in [1]
      }
}

void draw_door(int i, int x, int y, int custom)
{
   ALLEGRO_BITMAP *tmp = NULL;
   int col = item[i][6];
   int drawn = 0;
   if (item[i][13] == 448) // old style door shape
   {
      int shape = item[i][1];       // get shape
      int si = shape-448;           // convert to index to bitmap sequence
      tmp = mwB.door_tile[1][col][si];
   }
   else if (item[i][13] == 1083) // new style doors
   {
      int an = mwB.zz[1][83];             // cheat and use shape index from base door animation sequence
      if (item[i][8] == 0) an = 7-an; // exit only, run the sequence backwards
      tmp = mwB.door_tile[0][col][an];
   }
   else if (item[i][13] == 0) drawn = 1;

   if ((item[i][8] == 1) && (!custom)) // linked item destination
   {
      // linked destination item position
      int dx = itemf[item[i][9]][0];
      int dy = itemf[item[i][9]][1];
      int line_color = item[item[i][9]][6];

      if (item[i][12] == 1) // if always draw lines (1)
         al_draw_line(x+10, y+10, dx+10, dy+10, mC.pc[line_color], 1);  // draw a line connecting them

      if (!drawn)
      {
         for (int p=0; p<NUM_PLAYERS; p++) // is player touching door
            if ((players[p].active) && (players[p].marked_door == i))
            {
               if (item[i][12] > 0)  // always draw or only draw when touching ( 1 or 2)
                  al_draw_line(x+10, y+10, dx+10, dy+10, mC.pc[line_color], 1);  // draw a line connecting them

               // bigger door when player touching it
               al_draw_scaled_bitmap(tmp, 0, 0, 20, 20, x-5, y-6, 30, 26, 0 );

               if (item[i][8] == 0) al_draw_scaled_bitmap(mwB.tile[1015], 0, 0, 20, 20, x-5, y-6, 30, 26, 0); // OUT
               else al_draw_scaled_bitmap(mwB.tile[1014], 0, 0, 20, 20, x-5, y-6, 30, 26, 0); // IN

               if (item[i][11] == 1) // enter with <up>
                  al_draw_text(mF.pr8, mC.pc[15],  x+3, y-14, 0, "up");

               if (item[i][11] == 2) // enter with <down>
                  al_draw_text(mF.pr8, mC.pc[15],  x-5, y-14, 0, "down");
               drawn = 1;
            }
      }
   }
   if (!drawn)
   {
      al_draw_bitmap(tmp, x, y, 0); // if not drawn yet

      if (item[i][8] == 0) al_draw_bitmap(mwB.tile[1015], x, y, 0); // OUT
      else al_draw_bitmap(mwB.tile[1014], x, y, 0); // IN
   }

}










void proc_door_collision(int p, int i)
{

   if ((players[p].marked_door == -1)  // player has no marked door yet
     && (players[p].carry_item != i+1)) // player is not carrying this door
   {

      // item[x][6]  color
      // item[x][7]  move type (0 = auto, 1 = force instant, 2 = force move
      // item[x][8]  type (0 = no dest (exit only), 1 = linked dest
      // item[x][9]  linked destination item
      // item[x][10] key held flag
      // item[x][11] door entry type (0 0-immed, 1 = up 2 = down)
      // item[x][12] draw lines always, never
      // item[x][13] base animation shape

      if (item[i][8]) // do nothing if exit only
      {
         players[p].marked_door = i;
         int do_entry = 0;
         if (item[i][11] == 0) do_entry = 1; // enter immed
         if (players[p].carry_item-1 != i) // cant trigger entry if carrying this door
         {
            if (item[i][11] == 1) // enter with <up>
            {
               // to prevent immediate triggering when destination door, wait for release and re-press

               // if key held is old, ignore
               if (item[i][10] && item[i][10] < mwPS.frame_num-1) item[i][10] = 0;

               // up pressed and !pressed last frame
               if ((players[p].up) && (!item[i][10])) do_entry = 1;

               if (players[p].up) item[i][10] = mwPS.frame_num;
               else item[i][10] = 0;

            }

            if (item[i][11] == 2) // enter with <down>
            {
               // to prevent immediate triggering when destination door, wait for release and re-press

               // if key held is old, ignore
               if (item[i][10] && item[i][10] < mwPS.frame_num-1) item[i][10] = 0;

               // down pressed and !pressed last frame
               if ((players[p].down) && (!item[i][10])) do_entry = 1;

               if (players[p].down) item[i][10] = mwPS.frame_num;
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
            if (players[p].carry_item)
            {
                int ci = players[p].carry_item - 1;
                //printf("do entry, player is carrying item:%d\n", ci);

               // check to see if player is carrying this door
                if (ci == i) player_drop_item(p);

               // check to see if player is carrying an item without the carry through door flag set
               if (item[ci][3] != -2)  player_drop_item(p);
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
               if (item[i][13]) game_event(5, 0, 0, p, i, 0, 0); // no event if door is invisible

               int instant_move = 0;
               if (item[i][7] == 0) // 0 = auto
                  if (item[li][3]) // if dest is not stat
                     instant_move = 1;

               if (item[i][7] == 1) instant_move = 1; // 1 = force instant
               if (item[i][7] == 2) instant_move = 0; // 2 = force move

               if (is_player_riding_rocket(p)) instant_move = 1; // 1 = force instant if riding rocket

               if (instant_move)
               {
                  players[p].x = itemf[li][0];
                  players[p].y = itemf[li][1];

                  if (is_player_riding_rocket(p))
                  {
                     int c = players[p].carry_item-1;
                     itemf[c][0] = players[p].x;
                     itemf[c][1] = players[p].y;
                  }

                  item[li][10] = mwPS.frame_num;
               }
               else
               {
                  // snap player to the source door
                  players[p].x = itemf[i][0];
                  players[p].y = itemf[i][1];

                  players[p].right_xinc = 0;
                  players[p].left_xinc  = 0;

                  // set player's xinc and yinc
                  float xlen = players[p].x - dx;     // get the x distance between player and exit
                  float ylen = players[p].y - dy;     // get the y distance between player and exit
                  float hy_dist = sqrt(pow(xlen, 2) + pow(ylen, 2));  // hypotenuse distance
                  float speed = 15;                        // speed
                  float scaler = hy_dist / speed;          // get scaler
                  players[p].door_xinc = xlen / scaler;    // calc xinc
                  players[p].door_yinc = ylen / scaler;    // calc yinc

                  // set players rotation from xinc, yinc
                  players[p].door_draw_rot = atan2(players[p].door_yinc, players[p].door_xinc) - ALLEGRO_PI/2;

                  int num_steps = scaler;

                  if ((num_steps > 0) && (num_steps < 2000))
                  {
                     int ddrns = players[p].door_draw_rot_num_steps = 12;
                     players[p].door_num_steps = num_steps;
                     players[p].paused = 1;
                     players[p].paused_type = 2;
                     players[p].paused_mode = 1;
                     players[p].paused_mode_count = ddrns;
                     players[p].door_item = i;
                     players[p].door_draw_rot_inc = players[p].door_draw_rot / ddrns;
                  }
                  // printf("ns:%d xinc:%3.2f yinc:%3.2f \n", num_steps, players[p].xinc, players[p].yinc);
               } // end of if not instant move
            }  // end of if not bad exit
         } // end of do entry
      } // end of if not exit only
   } // end of if not first door touched
}


void proc_player_door_move(int p)
{
   int x = players[p].door_item;
   int ddrns = players[p].door_draw_rot_num_steps;
   float amount_to_shrink = 0.5;
   float sa = amount_to_shrink / ddrns;  // shrink and grow player inc
   int as = 7; // door open/close animation speed

   if (players[p].paused_mode == 1) // mode 1: enter door
   {
      players[p].paused_mode_count--;
      if (players[p].paused_mode_count)
      {
         players[p].draw_scale -= sa; // shrink player
         players[p].draw_rot += players[p].door_draw_rot_inc; // rotate player

         // open door
         float ratio = 1 - (float) players[p].paused_mode_count / ddrns;
         float shape_shift = ratio * as;

         if (item[x][13] == 448)
            item[x][1] = 448 + (int)shape_shift;
//            printf("ratio:%f scaled_ratio%f int_sr:%d shape:%d \n", ratio, shape_shift, int(shape_shift), item[x][1] );

      }
      else // next mode
      {
         if (item[x][13] == 448) item[x][1] = 448; // restore door shape
         players[p].paused_mode = 2;
         players[p].paused_mode_count = players[p].door_num_steps;
         players[p].xinc = players[p].door_xinc;
         players[p].yinc = players[p].door_yinc;
         players[p].draw_rot = players[p].door_draw_rot;
      }
   }

   if (players[p].paused_mode == 2) // mode 2: player move
   {
      players[p].paused_mode_count--;
      if (players[p].paused_mode_count)
      {
         players[p].x -= players[p].xinc;
         players[p].y -= players[p].yinc;
      }
      else // mode 2 done
      {
         players[p].paused_mode = 3;
         players[p].paused_mode_count = ddrns;

         players[p].xinc=0;
         players[p].yinc=0;


         // snap to dest...
         if (item[x][8] == 0) // regular dest
         {
            players[p].x  = item[x][6] * 20;
            players[p].y  = item[x][7] * 20;
         }
         if (item[x][8] == 1) // linked item dest
         {
            int li = item[x][9]; // linked item number
            players[p].x  = itemf[li][0];
            players[p].y  = itemf[li][1];
            // set destination key held to prevent immediate retriggering
            item[li][10] = mwPS.frame_num;
         }
       }
   } // end of mode 2

   if (players[p].paused_mode == 3) // mode 3: exit
   {
      players[p].paused_mode_count--;
      if (players[p].paused_mode_count)
      {
         players[p].draw_scale += sa;                         // un-shrink player
         players[p].draw_rot -= players[p].door_draw_rot_inc; // un-rotate player
         // close door (only if linked item exit)
         if (item[x][8] == 1)    // if linked item exit
         {
            float ratio = 1 - (float) players[p].paused_mode_count / ddrns;
            float shape_shift = ratio * 6 ;
            int li = item[x][9]; // linked item number
            if (item[li][13] == 448)
               item[li][1] = 454 - (int)shape_shift;
//                 printf("ratio:%f scaled_ratio%f int_sr:%d shape:%d \n", ratio, shape_shift, int(shape_shift), item[li][1] );

         }
      }
      else // next mode (done)
      {
         // if linked item exit
         if (item[x][8] == 1) // liked item dest
         {
            int li = item[x][9]; // linked item number
            if (item[li][13] == 448)
               item[li][1] = 448;   // restore door shape
            item[li][10] = mwPS.frame_num;   // key hold off

         }
         players[p].paused = 0;  // the entire thing is done
      }
   }
   proc_player_carry(p);
}





