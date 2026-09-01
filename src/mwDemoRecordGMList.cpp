// mwDemoRecord.cpp
#include "pm.h"
#include "mwDemoMode.h"
#include "mwDemoRecord.h"

#include "mwColor.h"
#include "mwConfig.h"
#include "mwDisplay.h"
#include "mwEventQueue.h"
#include "mwFont.h"
#include "mwGameMoves.h"
#include "mwGmInfo.h"
#include "mwInput.h"
#include "mwLoop.h"
#include "mwMenu.h"
#include "mwMiscFnx.h"
#include "mwPlayer.h"
#include "mwWidget.h"



int mwDemoRecord::draw_GMList_controls(mwWindow &w)
{
   int x1 = w.rect.x1;
   int x2 = w.rect.x2;
   int xc = w.rect.XCenter();
   int d = w.disable_input;


   // show controls in title bar
   int ya = w.rect.y1 + 2;
   int tbh = 14; // title bar height

   int tbx = x1+120;
   mWidget.mCheckBoxWithToolTip(1, tbx, 10, 1, ya-1, 10, 0, gm_list_mono, "m", 15, 15, d, "mono color");

   tbx-=30;
   if (mWidget.mCheckBoxWithToolTip(1, tbx, 10, 1, ya-1, 10, 0, gm_list_all, "a", 15, 15, d, "all players")) load_lnk_arr();

   tbx-=30;
   if (mWidget.mCheckBoxWithToolTip(1, tbx, 10, 1, ya-1, 10, 0, gm_list_simple, "s", 15, 15, d, "simple")) load_lnk_arr();



   // show controls below title bar
   ya = w.rect.y1 + tbh;
   int bah = 49; // + mLoop.pct_y; // buttons area height

   // simpler variable names for local scope
   int f = mLoop.frame_num;
   int p = mPlayer.active_local_player;

   // if set, edit this gm_Index after displaying list
   gm_list_edit_index = -1;


   // finds the game move with the highest frame number equal or less than f
   int prev_move = mGameMoves.find_previous_move_for_player(p, f);

   int prev_move_type = 0; // by default - no move found
   if (prev_move != -1)
   {
      prev_move_type = 1; // prev move found
      if (mGameMoves.arr[prev_move][0] == f) prev_move_type = 2; // prev move is equal to current frame
   }


   int bcol = w.color + 192;
   int fcol = 15;

   char msg[256];
   sprintf(msg, "%s", "Add move");
   if (prev_move_type == 2) sprintf(msg, "%s", "Copy move");

   if (mWidget.mButton(0, x1+2, x2-2,  0,ya,ya+10,  0,  1,3,1,  bcol,fcol,15, 15,0, msg, d))
   {
      // default values for new move
      int nf = f;
      int nd = 0;

      // if move found, copy data
      if (prev_move_type) nd = mGameMoves.arr[prev_move][3];

      // move found and current frame
      if (prev_move_type == 2) nf = f + 1;


      if (gm_list_simple)
      {
         mGameMoves.add_game_move2(nf, PM_GAMEMOVE_TYPE_PLAYER_MOVE, p, nd);
         mGameMoves.gm_sort();
         refresh();
      }
      else
      {
         // add move with temp value '9999' then sort into place
         mGameMoves.add_game_move2(nf, PM_GAMEMOVE_TYPE_PLAYER_MOVE, p, 9999);
         mGameMoves.gm_sort();
         refresh();

         // clear, frame, title after refresh
         w.rect.clear_frame_title(mColor.pc[w.color+224], mColor.pc[w.color], mColor.White, 12, w.title_text_just, mFont.pr8, w.title);


         // find index of newly added move with d3 of 9999
         for (int i=0; i<mGameMoves.entry_pos; i++)
            if ((mGameMoves.arr[i][0] == nf) && (mGameMoves.arr[i][1] == 5) && (mGameMoves.arr[i][2] == p) && (mGameMoves.arr[i][3] == 9999))
            {
               // change from marker '9999' to actual data
               mGameMoves.arr[i][3] = nd;

               // mark for editing after drawing gm list
               gm_list_edit_index = i;
            }
      }
   }



   ya +=12;
   if (mWidget.mButton(0, x1+2, xc-2,  0,ya,ya+10,  0,  1,3,1,  bcol,fcol,15, 15,0, "slide -1", d))
   {
      for (int x=0; x<mGameMoves.entry_pos; x++)
         if ((mGameMoves.arr[x][2] == p) && (mGameMoves.arr[x][0] >= f)) mGameMoves.arr[x][0]--;
      mGameMoves.gm_sort();
      mLoop.frame_num--;
      refresh();
   }

   if (mWidget.mButton(0, xc+2, x2-2,  0,ya,ya+10,  0,  1,3,1,  bcol,fcol,15, 15,0, "slide +1", d))
   {
      for (int x=0; x<mGameMoves.entry_pos; x++)
         if ((mGameMoves.arr[x][2] == p) && (mGameMoves.arr[x][0] >= f)) mGameMoves.arr[x][0]++;
      mGameMoves.gm_sort();
      mLoop.frame_num++;
      refresh();
   }

   ya +=12;
   if (mWidget.mButton(0, x1+2, x2-2,  0,ya,ya+10,  0,  1,3,1,  bcol,fcol,15, 15,0, "Add fire", d))
   {
      mGameMoves.set_flag_for_player_at_frame(f+0, p, PM_COMPMOVE_FIRE, 1);
      mGameMoves.set_flag_for_player_at_frame(f+1, p, PM_COMPMOVE_FIRE, 0);
      mGameMoves.gm_sort();
      refresh();
   }

   ya +=12;
   if (mWidget.mButton(0, x1+2, x2-2,  0,ya,ya+10,  0,  1,3,1,  bcol,fcol,15, 15,0, "Add fire + 4", d))
   {
      mGameMoves.set_flag_for_player_at_frame(f+4, p, PM_COMPMOVE_FIRE, 1);
      mGameMoves.set_flag_for_player_at_frame(f+5, p, PM_COMPMOVE_FIRE, 0);
      mGameMoves.gm_sort();
      mLoop.frame_num+=4;
      refresh();
   }

   return bah;

}







void mwDemoRecord::draw_GMList(mwWindow &w)
{
   if (gm_list_simple) w.rect.setWidth(172);
   else w.rect.setWidth(329);

   w.set_resizeable(w.rect.w, w.rect.w, 200, 1600);




   int tbh = 14; // title bar height
   int bah = draw_GMList_controls(w); // button area height

   int x1 = w.rect.x1;
   int x2 = w.rect.x2;
   int d = w.disable_input;


   // start of list and running y position
   int ry = w.rect.y1 + tbh + bah;
   al_draw_line(x1, ry-1, x2, ry-1, mColor.pc[w.color], 1);



   // set num_lines from available space
   int num_lines = (w.rect.h-tbh-bah-1)/8;


   // starting index
   int gm1 = gm_list_start_index;

   // set end of range
   int gm2 = gm1 + num_lines;

   // don't let end be after the end of the array
   if (gm2 > lnk_entry_pos) gm2 = lnk_entry_pos;


   // is current frame position visible on list?
   int f = mLoop.frame_num;
   int gm1_frame = mGameMoves.arr[lnk_arr[gm1]][0];
   int gm2_frame = mGameMoves.arr[lnk_arr[gm2]][0];
   if ((gm1_frame > f) || (gm2_frame < f))
   {
      // find new starting gm index to show
      // ideally the current frame position should be near the middle of the list
      // find game move index closest to current frame

      int i_closest = 0;
      int min_dist = 999999;
      for (int i=0; i<lnk_entry_pos; i++)
      {
         // translate from link array index to game move index;
         int gmi = lnk_arr[i];

         // get frame number
         int gmf = mGameMoves.arr[gmi][0];

         // get distance from current frame
         int dist = abs(gmf - mLoop.frame_num);

         if (dist < min_dist)
         {
            min_dist = dist;
            i_closest = i;
         }
      }

      // go back half of num_lines
      gm1 = i_closest - num_lines/2;

      // don't let start be before than the start of the array
      if (gm1 < 0) gm1 = 0;

      // save for next time
      gm_list_start_index = gm1;

      // set end of range
      gm2 = gm1 + num_lines;

      // don't let end be after the end of the array
      if (gm2 > lnk_entry_pos) gm2 = lnk_entry_pos;
   }


   // y positions for current gm frame
   int cy1 = -1;
   int cy2 = -1;

   // y positions for mouseover gm frame
   int my1 = -1;
   int my2 = -1;


   // iterate the link array
   for (int i=gm1; i<gm2; i++)
   {
      // get actual game move index from link translation table
      int gi = lnk_arr[i];

      // get the 4 values from game move
      int gif = mGameMoves.arr[gi][0];
      int git = mGameMoves.arr[gi][1];
      int gip = mGameMoves.arr[gi][2];
      int giv = mGameMoves.arr[gi][3];

      // extract player number, color, and name from ACTIVE game move
      if (git & PM_GAMEMOVE_TYPE_PLAYER_ACTIVE_FLAG)
      {
         char name[9] = { 0 };
         mMiscFnx.gma_to_val(git, gip, giv, gip, giv, name);

         // why? // so that we use player name from demo file instead of local player name? ???
         // snprintf(mPlayer.syn[gip].name, 9, "%s", name);
      }


      // fill the text line
      char msg[256];
      if (gm_list_simple) sprintf(msg, "%s", mGameMoves.get_gm_text3(gi, msg));
      else                sprintf(msg, "%s", mGameMoves.get_gm_text2(gi, gif, git, gip, giv, msg));

      // set line color from player color
      int text_col = mPlayer.syn[gip].color;
      if (git == PM_GAMEMOVE_TYPE_SHOT_CONFIG) text_col = 15;
      if (gm_list_mono) text_col = 15;

      // rect with line dimensions for background draw and mouse detection
      mwRect<int> line_rect = mwRect<int>::fromX1Y1X2Y2(x1, ry, x1+strlen(msg)*8, ry+7);


      // if this move is an exact match of current frame, set start and end cy values
      if (gif == mLoop.frame_num)
      {
         // set start y pos of current frame rect only if not set yet
         if (cy1 == -1) cy1 = ry;

         // set end y pos of current frame rect
         cy2 = ry + line_rect.h;
      }

      // this move is not an exact match with current frame, check if immediately adjacent to current frame
      else
      {
         // check if frame number of this move is less than current frame, and next move is greater than current frame

         // this move has a frame number less than current frame
         if (gif < mLoop.frame_num)
         {
            // get next move in list
            int ni = i+1;

            // make sure not past end of list
            if (ni >= lnk_entry_pos-1) ni = lnk_entry_pos-1;

            // get the frame number of the next move in the list
            int nf = mGameMoves.arr[lnk_arr[ni]][0];

            // set start y pos of current frame rect
            if (nf > mLoop.frame_num) cy1 = ry;
         }

         // check if frame number of this move is more than current frame, and previous move is less than current frame

         // this move has a frame number more than current frame
         if (gif > mLoop.frame_num)
         {
            // get prev move in list
            int pi = i-1;

            // make sure not before beginning of list
            if (pi < 0) pi = 0;

            // get the frame number of the previous move in the list
            int pf = mGameMoves.arr[lnk_arr[pi]][0];

            // set end y pos of current frame rect
            if (pf < mLoop.frame_num) cy2 = ry + line_rect.h;
         }
      }


      // draw line background
      line_rect.draw_filled_rectangle(mColor.Black);

      // draw line text
      al_draw_text(mFont.pr8, mColor.pc[text_col], x1, ry, 0, msg);


      // process mouse input (this is purposely here, after drawing the line)

      // determine if mouse is on the line, and set my1, my2
      if ((!d) && (line_rect.contains(mInput.mouse_x, mInput.mouse_y)))
      {
         my1 = ry;
         my2 = ry + line_rect.h;


         // right click context menu
         if (mInput.mouse_b[2][0])
         {
            // to line up context menu nicely
            int yp = line_rect.y2 + line_rect.h + 1;

            mInput.mouse_y = yp;
            al_set_mouse_xy(mDisplay.display, mInput.mouse_x * mDisplay.display_transform_double, mInput.mouse_y * mDisplay.display_transform_double);
            proc_gm_list_menu(gi, x1, yp);
         }


         // edit in place
         if (gm_list_simple)
         {
            int c_x1 = x1+72;
            int c_x2 = c_x1+48;
            if ((mInput.mouse_x > c_x1) && (mInput.mouse_x < c_x2))
            {
               int cn = (mInput.mouse_x - c_x1) / 8;
               int cnx = c_x1 + cn*8;
               al_draw_rectangle(cnx-1, my1-1, cnx+9, my2+1, mColor.pc[10], 1);

               //al_draw_textf(mFont.pixl, mColor.pc[15],  cnx, my1-10, 0, "%d", cn);

               // show the letter
               const char m2[8] = "LRUDJF";
               if ((cn >=0) && (cn <=5))
               {
                  int cfsc = 12; // not set color
                  if (mGameMoves.arr[gi][3] & (int) pow(2, cn)) cfsc = 15; // set color
                  al_draw_textf(mFont.pr8, mColor.pc[cfsc],  cnx, my1, 0, "%c", m2[cn]);
               }


               if (mInput.mouse_b[1][0])
               {
                  while (mInput.mouse_b[1][0]) mEventQueue.proc(1);
                  mGameMoves.arr[gi][3] ^= (int) pow(2, cn);
                  refresh();
               }
            }

            // left click to the left of edit in place sets current frame
            if ((mInput.mouse_x < c_x1) && (mInput.mouse_b[1][0]))
            {
               while (mInput.mouse_b[1][0]) mEventQueue.proc(1);
               mLoop.frame_num = gif;
               refresh();
            }

         }
         else
         {
            // left click sets current frame
            if (mInput.mouse_b[1][0])
            {
               while (mInput.mouse_b[1][0]) mEventQueue.proc(1);
               mLoop.frame_num = gif;
               refresh();
            }
         }
      }

      // advance y pos for next line
      ry += line_rect.h + 1;

      // edit in place
      if (gi == gm_list_edit_index) edit_gm(gi, x1, ry + line_rect.h + 1);

   } // all lines drawn

   // draw rect for current move outline
   if ((cy1 != -1) && (cy2 != -1))
   {
      mwRect<int> rect = mwRect<int>::fromX1Y1X2Y2(x1+2, cy1, x2-2, cy2);
      rect.draw_rectangle(mColor.White, 0, +1);
   }

   // draw rect for mouse position outline
   if ((my1 != -1) && (my2 != -1))
   {
      mwRect<int> rect = mwRect<int>::fromX1Y1X2Y2(x1+2, my1, x2-2, my2);
      rect.draw_rectangle(mColor.Yellow, 0, +1);
   }
}





void mwDemoRecord::proc_gm_list_menu(int gi, int sx, int sy)
{
   if (mInput.mouse_b[2][0])
   {
      int f = mGameMoves.arr[gi][0];
      int t = mGameMoves.arr[gi][1];
      int p = mGameMoves.get_player_num_from_game_move(gi);
      int m = mGameMoves.arr[gi][3];


      int mi=0;
      sprintf(mMenu.menu_string[mi++],"Edit");
      sprintf(mMenu.menu_string[mi++],"Copy");
      sprintf(mMenu.menu_string[mi++],"Delete");
      sprintf(mMenu.menu_string[mi++],"Dump");
      sprintf(mMenu.menu_string[mi++],"Erase After");
      sprintf(mMenu.menu_string[mi++],"Copy all moves to active player");
      sprintf(mMenu.menu_string[mi++],"Insert new at current pos");
      sprintf(mMenu.menu_string[mi++],"slide +1 at current pos");
      sprintf(mMenu.menu_string[mi++],"slide -1 at current pos");

      sprintf(mMenu.menu_string[mi++],"Add fire 4 frames after current pos");
      sprintf(mMenu.menu_string[mi++],"Add fire at current pos");


      sprintf(mMenu.menu_string[mi++],"end");
      int mr = mMenu.pmenu(5, 13, -4, 99);

      mi=0;

      if (mr == mi++) // edit
      {
         edit_gm(gi, sx, sy);
      }
      if (mr == mi++) // copy
      {
         mGameMoves.add_game_move2(f, t, p, m);
         mGameMoves.gm_sort();
         refresh();
         set_active_section(current_section);
      }
      if (mr == mi++) // delete
      {
         mGameMoves.gm_remove(gi);
         mGameMoves.gm_sort();
         refresh();
         set_active_section(current_section);
      }
      if (mr == mi++) // dump
      {
         //draw_gm_txt_lines(0, 0, 1);
      }
      if (mr == mi++) // erase after
      {
         for (int x=0; x<mGameMoves.entry_pos; x++)
            if ((mGameMoves.arr[x][2] == p) && (mGameMoves.arr[x][0] > f)) mGameMoves.clear_single(x);

         mGameMoves.gm_sort();
         refresh();
      }


      if (mr == mi++) // copy to rec_player
      {
         int sp = p;
         int dp = mPlayer.active_local_player;

         //printf("copy_ptp(sp:%d, dp:%d)\n", sp,  dp);

         // erase all moves for destination player
         for (int x=0; x<mGameMoves.entry_pos; x++)
            if ((mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_PLAYER_MOVE) && (mGameMoves.arr[x][2] == dp)) mGameMoves.clear_single(x);

         // copy all moves from source player to destination player
         for (int x=0; x<mGameMoves.entry_pos; x++)
            if ((mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_PLAYER_MOVE) && (mGameMoves.arr[x][2] == sp))
               mGameMoves.add_game_move2(mGameMoves.arr[x][0], mGameMoves.arr[x][1], dp, mGameMoves.arr[x][3]);

         mGameMoves.gm_sort();
         refresh();
      }


      if (mr == mi++) // add at current pos
      {
         mGameMoves.add_game_move2(mLoop.frame_num, t, p, m);
         mGameMoves.gm_sort();
         refresh();
         set_active_section(current_section);
      }
      if (mr == mi++) // slide +1 at current pos
      {
         for (int x=0; x<mGameMoves.entry_pos; x++)
            if ((mGameMoves.arr[x][2] == p) && (mGameMoves.arr[x][0] >= f)) mGameMoves.arr[x][0]++;
         mGameMoves.gm_sort();
         refresh();
         set_active_section(current_section);
      }
      if (mr == mi++) // slide -1 at current pos
      {
         for (int x=0; x<mGameMoves.entry_pos; x++)
            if ((mGameMoves.arr[x][2] == p) && (mGameMoves.arr[x][0] >= f)) mGameMoves.arr[x][0]--;
         mGameMoves.gm_sort();
         refresh();
         set_active_section(current_section);
      }


      if (mr == mi++) // add fire at pos+4
      {
         // uses 'm' as the fire move

         //printf("add fire at pos+4 f:%d p:%d m:%d\n", f, p, m);

         // add fire
         mGameMoves.add_game_move2(f+4, PM_GAMEMOVE_TYPE_PLAYER_MOVE, p, m);

         // release fire
         int mfo = m & ~PM_COMPMOVE_FIRE;
         mGameMoves.add_game_move2(f+5, PM_GAMEMOVE_TYPE_PLAYER_MOVE, p, mfo);

         mGameMoves.gm_sort();
         refresh();
         set_active_section(current_section);
      }

      if (mr == mi++) // add fire at pos
      {

         int cf = mLoop.frame_num;

         //printf("add fire at pos f:%d p:%d m:%d\n", cf, p, m);

         m |=  PM_COMPMOVE_FIRE; // set flag

         // press fire
         mGameMoves.add_game_move2(cf, PM_GAMEMOVE_TYPE_PLAYER_MOVE, p, m);

         // release fire
         int mfo = m & ~PM_COMPMOVE_FIRE;
         mGameMoves.add_game_move2(cf+1, PM_GAMEMOVE_TYPE_PLAYER_MOVE, p, mfo);

         mGameMoves.gm_sort();
         refresh();
         set_active_section(current_section);
      }






   }
}

void mwDemoRecord::proc_edit_gm_type_menu(int & t)
{
   if (mInput.mouse_b[2][0])
   {
      mInput.mouse_y+=8;
      al_set_mouse_xy(mDisplay.display, mInput.mouse_x * mDisplay.display_transform_double, mInput.mouse_y * mDisplay.display_transform_double);

      sprintf(mMenu.menu_string[0],"PLAYER_MOVE");
      sprintf(mMenu.menu_string[1],"PLAYER_INACTIVE");
      sprintf(mMenu.menu_string[2],"SHOT_CONFIG");
      sprintf(mMenu.menu_string[3],"PLAYER_HIDDEN");
      sprintf(mMenu.menu_string[4],"LEVEL_DONE_ACK");
      sprintf(mMenu.menu_string[5],"end");
      switch (mMenu.pmenu(5, 13, -4, 99))
      {
         case 0: t = PM_GAMEMOVE_TYPE_PLAYER_MOVE; break;
         case 1: t = PM_GAMEMOVE_TYPE_PLAYER_INACTIVE; break;
         case 2: t = PM_GAMEMOVE_TYPE_SHOT_CONFIG; break;
         case 3: t = PM_GAMEMOVE_TYPE_PLAYER_HIDDEN; break;
         case 4: t = PM_GAMEMOVE_TYPE_LEVEL_DONE_ACK; break;
      }
      mInput.mouse_y-=8;
      al_set_mouse_xy(mDisplay.display, mInput.mouse_x * mDisplay.display_transform_double, mInput.mouse_y * mDisplay.display_transform_double);
   }
}

// translation table from gm index to index in gm_txt_line array
void mwDemoRecord::load_lnk_arr(void)
{
   lnk_entry_pos = 0;
   for (int i=0; i<mGameMoves.entry_pos; i++)
   {
      int add = 0;
      if (gm_list_all) add = 1;
      else
      {
         // always add player active moves for all players
         if (mGameMoves.arr[i][1] & PM_GAMEMOVE_TYPE_PLAYER_ACTIVE_FLAG) add = 1;

         // always add shot config moves
         if (mGameMoves.arr[i][1] == PM_GAMEMOVE_TYPE_SHOT_CONFIG) add = 1;

         // add current player moves
         if (mGameMoves.arr[i][2] == mPlayer.active_local_player) add = 1;
      }

      // in simple mode add only 'PM_GAMEMOVE_TYPE_PLAYER_MOVE'
      if ((gm_list_simple) && (mGameMoves.arr[i][1] != PM_GAMEMOVE_TYPE_PLAYER_MOVE)) add = 0;

      if (add)
      {
         lnk_arr[lnk_entry_pos] = i;
         if (++lnk_entry_pos > 9999)
         {
            printf("lnk_arr ran out of room\n");
            return;
         }
      }
   }
}


void mwDemoRecord::edit_gm(int gi, int sx, int sy)
{
   int f = mGameMoves.arr[gi][0];
   int t = mGameMoves.arr[gi][1];
   int p = mGameMoves.arr[gi][2];
   int v = mGameMoves.arr[gi][3];
   char name[9] = { 0 };

   int x1 = sx;
   int y1 = sy-6;
   int x2 = x1 + 329;
   int y2 = y1 + 46;

   char msg[256];
   int col = 14;

   int quit = 0;
   while (!quit)
   {
      // erase and frame
      al_draw_filled_rectangle(x1, y1, x2, y2, mColor.pc[col+208]);
      al_draw_rectangle(x1, y1, x2, y2, mColor.pc[col], 1);

      int xa = x1+2;
      int ya = y1+2;

      // show text line of game move being edited
      sprintf(msg, "%s", mGameMoves.get_gm_text2(gi, f, t, p, v, msg));
      al_draw_text(mFont.pr8, mColor.pc[15], x1, ya, 0, msg);
      ya+=9;

      al_draw_line(x1, ya, x2, ya, mColor.pc[col], 1);
      ya+=2;

      // common y's for all title bar stuff
      int tb_y1 = y1+1;
      int tb_y2 = tb_y1+9;

      int tb_x1 = x1+184;
      if (gi>999) tb_x1 += 8;

      if ((mInput.mouse_y >= tb_y1) && (mInput.mouse_y < tb_y2))
      {
         // player num pop-up menu
         if (t != PM_GAMEMOVE_TYPE_SHOT_CONFIG)
         {
            int p_x1 = tb_x1;
            int p_x2 = p_x1+16;
            if ((mInput.mouse_x > p_x1) && (mInput.mouse_x < p_x2))
            {
               al_draw_rectangle(p_x1, tb_y1, p_x2, tb_y2, mColor.pc[10], 1);
               change_player_num_menu(p);
            }
         }

         // game move type pop-up menu
         int t_x1 = tb_x1 + 24;
         int t_x2 = 0;
         if (t == PM_GAMEMOVE_TYPE_PLAYER_MOVE)       { t_x2 = t_x1+34; }
//         if (t & PM_GAMEMOVE_TYPE_PLAYER_ACTIVE_FLAG) { t_x2 = t_x1+70; }
         if (t == PM_GAMEMOVE_TYPE_PLAYER_INACTIVE)   { t_x2 = t_x1+68; }
         if (t == PM_GAMEMOVE_TYPE_PLAYER_HIDDEN)     { t_x2 = t_x1+50; }
         if (t == PM_GAMEMOVE_TYPE_LEVEL_DONE_ACK)    { t_x2 = t_x1+26; }
         if (t == PM_GAMEMOVE_TYPE_SHOT_CONFIG)       { t_x1 -= 24; t_x2 = t_x1+42; }

         if ((mInput.mouse_x > t_x1) && (mInput.mouse_x < t_x2))
         {
            al_draw_rectangle(t_x1, tb_y1, t_x2, tb_y2, mColor.pc[10], 1);
            proc_edit_gm_type_menu(t);
         }

         // toggle player controls for game move type 'PM_GAMEMOVE_TYPE_PLAYER_MOVE'
         if (t == PM_GAMEMOVE_TYPE_PLAYER_MOVE)
         {
            int c_x1 = tb_x1+72;
            int c_x2 = c_x1+48;
            if ((mInput.mouse_x > c_x1) && (mInput.mouse_x < c_x2))
            {
               int cn = (mInput.mouse_x - c_x1) / 8;
               int cnx = c_x1 + cn*8;

               al_draw_rectangle(cnx-1, tb_y1, cnx+8+1, tb_y2, mColor.pc[10], 1);

               const char m2[8] = "LRUDJF";
               if ((cn >=0) && (cn <=5)) al_draw_textf(mFont.pr8, mColor.pc[15],  cnx, tb_y1+1, 0, "%c", m2[cn]);

               if (mInput.mouse_b[1][0])
               {
                  while (mInput.mouse_b[1][0]) mEventQueue.proc(1);
                  v ^= (int) pow(2, cn);
               }
            }
         }
      }

      // frame number display and adjustment

      al_draw_textf(mFont.pr8, mColor.pc[9], (x1+x2)/2, ya+4, ALLEGRO_ALIGN_CENTER,  "Frame:%d", f);
      al_draw_textf(mFont.pr8, mColor.pc[15], (x1+x2)/2, ya+4, ALLEGRO_ALIGN_CENTER, "      %d", f);

      int c = 9+128;

      int bp = 10; // button padding
      int bs = 2; // button spacing

      int x7 = xa;
      int x8 = x7 + bp + 8*4;

      if (mWidget.mButton(0, x7, x8,   1, ya, 14,    1, 2, 0, 1,    c, c, 15, 0, 0, "-100",    0)) f-=100;
      x7 = x8 + bs; x8 = x7 + bp + 8*3;
      if (mWidget.mButton(0, x7, x8,   1, ya, 14,    1, 2, 0, 1,    c, c, 15, 0, 0, "-10",     0)) f-=10;
      x7 = x8 + bs; x8 = x7 + bp + 8*2;
      if (mWidget.mButton(0, x7, x8,   1, ya, 14,    1, 2, 0, 1,    c, c, 15, 0, 0, "-1",      0)) f-=1;

      x8 = x2 - 2;  x7 = x8 - (bp + 8*4);
      if (mWidget.mButton(0, x7, x8,   1, ya, 14,    1, 2, 0, 1,    c, c, 15, 0, 0, "+100",    0)) f+=100;
      x8 = x7 - bs; x7 = x8 - (bp + 8*3);
      if (mWidget.mButton(0, x7, x8,   1, ya, 14,    1, 2, 0, 1,    c, c, 15, 0, 0, "+10",     0)) f+=10;
      x8 = x7 - bs; x7 = x8 - (bp + 8*2);
      if (mWidget.mButton(0, x7, x8,   1, ya, 14,    1, 2, 0, 1,    c, c, 15, 0, 0, "+1",      0)) f+=1;

      ya+=16;
      mWidget.mSliderInt(0, xa, x2-2,  1, ya, 14,  2, 2, 1, 1,  c, c, 15, 15, 15,0, 0,  f, mDemoMode.last_frame+1000, 0, 1, "", 0, 0);
      ya+=16;

      if (f < 0) f = 0;

      if (t & PM_GAMEMOVE_TYPE_PLAYER_ACTIVE_FLAG)
      {
         // erase and frame
         int yi = 31;
         al_draw_filled_rectangle(x1, ya, x2, ya+yi, mColor.pc[col+208]);
         al_draw_rectangle(x1, ya, x2, ya+yi, mColor.pc[col], 1);
         ya+=2;

         // extract values
         int pp, c;
         mMiscFnx.gma_to_val(mGameMoves.arr[gi][1], mGameMoves.arr[gi][2], mGameMoves.arr[gi][3], pp, c, name);

         int cl = mWidget.colsel(xa, ya, x2-2, 16,  10,0,0,0,  0,12,15,15,  0,0,1,0);
         if (cl != -1) v = cl;

         ya+=2;
         al_draw_text( mFont.pr8, mColor.pc[15], xa+2, ya, 0, "Player Name:");
         ya-=2;
         if (mWidget.mButton(3, xa+98, 0,   1, ya, 11,   0, 0, 3, 3,   0, 15, 15,  10, 0, name, 0))
         {
            if (mMiscFnx.edit_string_simple(xa+98, ya, mPlayer.syn[pp].name, 8)) mGameMoves.gma_change_name(gi, mPlayer.syn[pp].name);
            refresh();
         }
         ya+=13;
      }

      if (t == PM_GAMEMOVE_TYPE_SHOT_CONFIG)
      {
         // erase and frame
         int yi = 18;
         al_draw_filled_rectangle(x1, ya, x2, ya+yi, mColor.pc[col+208]);
         al_draw_rectangle(x1, ya, x2, ya+yi, mColor.pc[col], 1);
         ya+=2;
         mWidget.togglfc(xa,     ya, xa+20,  16, 0,0,0,0,  0,0,0,0,     1,0,0,0, p, 0b01, "Other Players", 15, 15);
         mWidget.togglfc(xa+128, ya, xa+148, 16, 0,0,0,0,  0,0,0,0,     1,0,0,0, p, 0b10, "Self", 15, 15);
         mWidget.mStepSliderInt(0, xa+180, x2-2,  1, ya, 14,  2, 2, 1, 1,  12, 12, 15, 15, 15,0, 0,  v, 100, -100, 1, 1, 0, "Damage:", 0, 0); ya+=16;
      }

      // erase and frame
      al_draw_filled_rectangle(x1, ya, x2, ya+24, mColor.pc[col+208]);
      al_draw_rectangle(x1, ya, x2, ya+24, mColor.pc[col], 1);
      ya+=3;

      int sp = 45;
      x7 = xa + sp;

      if (mWidget.mButton(0, x7, x7+60,   1, ya, 18,    1, 2, 0, 1,    6, 6, 15, 0, 0, "Reload",    0))
      {
         f = mGameMoves.arr[gi][0];
         t = mGameMoves.arr[gi][1];
         p = mGameMoves.arr[gi][2];
         v = mGameMoves.arr[gi][3];
      }
      x7 += 60 + sp;

      if (mWidget.mButton(0, x7, x7+60,   1, ya, 18,    1, 2, 0, 1,    14, 14, 15, 0, 0, "Commit",    0))
      {
         mGameMoves.arr[gi][0] = f;

         if (t & PM_GAMEMOVE_TYPE_PLAYER_ACTIVE_FLAG)
         {
            mGameMoves.gma_change_color(gi, v);
         }
         else
         {
            mGameMoves.arr[gi][1] = t;
            mGameMoves.arr[gi][2] = p;
            mGameMoves.arr[gi][3] = v;
         }

         mGameMoves.gm_sort();
         refresh();
         set_active_section(current_section);
         quit = 1;
      }
      x7 += 60 + sp;
      if (mWidget.mButton(0, x7, x7+60,   1, ya, 18,    1, 2, 0, 1,   10, 10, 15, 0, 0, "Cancel",    0)) quit = 1;

      // to hide remnants when height is bigger, erase area below
      al_draw_filled_rectangle(x1+1, ya+21, x2-1, ya+40, mColor.pc[0]);

      if (mInput.key[ALLEGRO_KEY_ESCAPE][0])
      {
         while (mInput.key[ALLEGRO_KEY_ESCAPE][0]) mEventQueue.proc(1);
         quit = 1;
      }
      al_flip_display();
      mEventQueue.proc(1);
   }
}
