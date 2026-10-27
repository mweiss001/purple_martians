// mw_ItemStartExitGate.cpp

#include "pm.h"
#include "mwItem.h"
#include "mwBitmap.h"
#include "mwColor.h"
#include "mwLoop.h"
#include "mwLevel.h"
#include "mwGameEvent.h"
#include "mwFont.h"
#include "mwGameMoves.h"
#include "mwMain.h"
#include "mwConfig.h"
#include "mwDemoMode.h"
#include "mwScreen.h"
#include "mwDisplay.h"
#include "mwGameState.h"
#include "mwInput.h"
#include "mwLevelIcons.h"
#include "mwNetgame.h"
#include "mwTransitions.h"
#include "mwTriggerEvent.h"

/*
// -----------------------------------------------------------------------
// Start
// -----------------------------------------------------------------------

item[][0]  = 5 - Start
item[][1] = bitmap or ans
item[][2] = draw type (not used)
item[][3] = (0=stat, 1=fall, -1=carry, -2=carry through door)
item[][4]  = x pos
item[][5]  = y pos

item[][6] mode
item[][7] start index
item[][8] no backsies
item[][9] trigger event number

*/


void mwItem::proc_start(int i)
{
   int et = item[i][9];                  // number of pm_event trigger we are looking for
   int trig = mTriggerEvent.event[et];   // is the trigger event set?
   if (et == 0) trig = 0;                // if event is zero, ignore
   if (trig) proc_start_collision(0, i); // force player 0
}


int mwItem::draw_start(int i, int x, int y, int shape)
{
   // is this start the spawn point for the active local player?
   if (item[i][7] == m_parent.mPlayer.syn[m_parent.mPlayer.active_local_player].spawn_point_index) shape = mBitmap.zz[0][21]; // show spinning
   else                                                                                            shape = mBitmap.zz[0][11]; // show static
   al_draw_bitmap(mBitmap.sprite[shape], x, y, 0);
   if (mLoop.level_editor_running) al_draw_textf(mFont.pixl, mColor.pc[12], x+10, y-4, ALLEGRO_ALIGN_CENTER, "%d", item[i][7]); // in lev editor, show start seq number
   return 1;
}


void mwItem::proc_start_collision(int p, int i)
{
   int cpi = m_parent.mPlayer.syn[p].spawn_point_index;  // current index for player
   int npi = item[i][7];                        // new spawn point index

   // count number of starts and put in array indexed by start index
   int ns = 0;
   int s[8] = {0};
   for (int i=0; i<500; i++)
      if (item[i][0] == 5)
      {
         ns++;
         s[item[i][7]] = i; // save index of this start
      }

   if (ns > 1)
   {
      // get these from start with index 0
      int mode = item[s[0]][6];
      int iio  = item[s[0]][8]; // increase index only

      if ((!iio) || ((iio) && (npi > cpi))) // if not iio, or iio and higher index
      {
         if (mode == 2) // check point common
         {
            for (p=0; p<8; p++)
               m_parent.mPlayer.syn[p].spawn_point_index = item[i][7]; // set new spawn point for all players
         }
         if (mode == 3) // check point individual
         {
            m_parent.mPlayer.syn[p].spawn_point_index = item[i][7]; // set new spawn point for this player
         }
      }
   }
}



void mwItem::set_player_start_pos(int p)
{
   //printf("set player:%d start pos\n", p);

   int item_to_get_start_from = -1;

   // in overworld level, player starts at the last gate they touched
   if (m_parent.play_level == 1)
   {
      for (int i=0; i<500; i++)
         if ((item[i][0] == 18) && (item[i][6] == m_parent.mPlayer.syn[p].overworld_last_touched_gate)) item_to_get_start_from = i;

      // if (item_to_get_start_from == -1)
      // {
      //    printf("Player:%d entering overworld, but failed to find gate for level:%d\n", p, m_parent.mPlayer.syn[p].overworld_last_touched_gate);
      // }
   }


   if (item_to_get_start_from == -1) // start item not found yet....
   {
      // count the starts and put them in an array
      int ns = 0;
      int s[8] = {0};
      for (int i=0; i<500; i++)
         if (item[i][0] == 5)
         {
            ns++;
            s[item[i][7]] = i; // save index of this start
         }
      if (ns == 0) printf("Lev:%d - Error setting player position: No start item found\n", mLevel.play_level);
      else
      {
         // syn[p].spawn_point_index is saved for every player
         // it is the current index into the array of starts

         // if only one start, that has to be the one we use
         if (ns == 1) m_parent.mPlayer.syn[p].spawn_point_index = 0;

         // if more than one start....
         if (ns > 1)
         {
            int mode = item[s[0]][6];
            if (mode == 0)
            {
               printf("Lev:%d - Error setting player position: In start mode:0 there should be only one start.. all other starts are ignored.\n", m_parent.play_level);
               m_parent.mPlayer.syn[p].spawn_point_index = 0;
            }
            if (mode == 1) // team start
            {
               if (p % 2) m_parent.mPlayer.syn[p].spawn_point_index = 1; // odd
               else       m_parent.mPlayer.syn[p].spawn_point_index = 0; // even
            }
            if ((mode == 2) || (mode == 3)) // check point common and individual
            {
               //if (!cont) syn[p].spawn_point_index = 0; // initial
            }
         }
         // translate spawn_point index into an item we can get a position from
         item_to_get_start_from = s[m_parent.mPlayer.syn[p].spawn_point_index];
      }
   }
   // if after all that we do not have an item, pick a default place to start the player
   if (item_to_get_start_from == -1)
   {
      m_parent.mPlayer.syn[p].x = 100;
      m_parent.mPlayer.syn[p].y = 100;
   }
   // set the player's position from the item position
   else
   {
      m_parent.mPlayer.syn[p].x = itemf[item_to_get_start_from][0];
      m_parent.mPlayer.syn[p].y = itemf[item_to_get_start_from][1];
   }
}









/*
// -----------------------------------------------------------------------
// Exit
// -----------------------------------------------------------------------

item[][0]  = 3 - Exit
item[][1] = bitmap or ans
item[][2] = draw type (not used)
item[][3] = (0=stat, 1=fall, -1=carry, -2=carry through door)
item[][4] = x pos
item[][5] = y pos

item[][8] exit with x enemies left

*/

int mwItem::draw_exit(int i, int x, int y, int shape)
{
   al_draw_bitmap(mBitmap.sprite[399], x, y, 0); // 'exit' text not shown
   if (mLoop.frame_num % 60 > 30)
      al_draw_text(mFont.pixl, mColor.pc[10], x+11, y-2, ALLEGRO_ALIGN_CENTER, "EXIT");

   int exit_enemys_left = m_parent.mEnemy.num_enemy - item[i][8];
   if (exit_enemys_left > 0) // locked
   {
      al_draw_bitmap(mBitmap.sprite[366], x, y, 0); // show lock
      if (mLoop.frame_num % 60 < 30)
         al_draw_textf(mFont.pixl, mColor.pc[14], x+11, y-2, ALLEGRO_ALIGN_CENTER, "%d", exit_enemys_left);
   }
   return 1;
}

void mwItem::proc_exit_collision(int p, int i)
{
   int exit_enemys_left = m_parent.mEnemy.num_enemy - item[i][8];
   if (exit_enemys_left <= 0)
   {
      if ((mGameState.level_done_mode == 0) && (!mNetgame.ima_client)) // only trigger from mode 0 and client can never locally exit
      {
         mGameState.level_done_mode = 9;
         mGameState.level_done_timer = 0;
         mGameState.level_done_x = itemf[i][0];
         mGameState.level_done_y = itemf[i][1];
         mGameState.level_done_player = p;
         mGameState.level_done_frame = mLoop.frame_num;

         if (!mMain.classic_mode) mGameState.level_done_next_level = 1;                             // in story mode all exits return to overworld
         else mGameState.level_done_next_level = m_parent.get_next_level(m_parent.play_level, 199, 1);  // otherwise do next chron level
         mGameEvent.add(4, 0, 0, p, 0, 0, 0);
      }
   }
   else mGameEvent.add(23, 0, 0, p, i, exit_enemys_left, 0); // not enough dead yet
}










/*
// -----------------------------------------------------------------------
// Gate
// -----------------------------------------------------------------------

item[][0] = 18 - Gate
item[][1] = draw mode
0 Draw Mode:Legacy
1 Draw Mode:Gold Colored Door
2 Draw Mode:Wooden Door
3 Draw Mode:Metal Door
4 Draw Mode:Portcullis

item[][2] =
item[][3] = flags (not used)

item[][4] = x pos
item[][5] = y pos

item[][6] = level
item[][7] = current display page

item[][10] level icon size
item[][11] hover level icon size

*/


void mwItem::proc_gate_collision(int p, int i)
{
   m_parent.mPlayer.syn[p].marked_gate = i;
   set_gate_level_icon_position(i);
   int lev = item[i][6];

   if (m_parent.mPlayer.syn[p].overworld_last_touched_gate != lev)
   {
      m_parent.mPlayer.syn[p].overworld_last_touched_gate = lev;
      if (p == 0) mConfig.save_config(PM_CFG_SAVE_OVERWORLD_GATE_LAST_TOUCHED); // clients do not save to config
   }

    // debug set level complete
   if ((mDemoMode.demo_debug_complete_level_on_gate_with_fire) && (m_parent.mPlayer.syn[p].fire) && (mLevel.data[lev].status < 2))
   {
      //printf("debug set level complete \n");
      mLevel.add_play_data_record(lev, 2);
      mLevel.load_level_by_number(1, 0, 0);
   }


   // if local player is hovering gate, load l2
   if (p == mLevel.mPlayer.active_local_player) l2.load_level_l2(lev);



   // -------------------------------------------------------
   // enter
   // -------------------------------------------------------

   // if not already in gate_enter_mode
   // and not client (client can never locally enter gate)
   // and UP pressed:
   // start gate_enter_mode
   if (!mGameState.gate_enter_mode && !mNetgame.ima_client && m_parent.mPlayer.syn[p].up)
   {
      mGameState.gate_enter_mode = 9;
      mGameState.gate_enter_item = i;

      // force page 0 (level icon) for transition
      item[i][7] = 0;

      l2.load_level_l2(lev);

      // test drawing this
      // l2.draw_scaled_level_region_to_display();
      // al_flip_display();
      // mInput.tsw();

      // save start positions
      mTransitions.gate_enter_next_level_start_x = l2.mPlayer.syn[0].x;
      mTransitions.gate_enter_next_level_start_y = l2.mPlayer.syn[0].y;
      mTransitions.gate_enter_Level_icon_size = item[i][11];

      // printf("\n--------------------------------------------------------------------\n");
      // printf("Next Level (%d) l2 load at gate enter\n", lev);
      // printf("player pos x:%.0f  y:%.0f\n", l2.mPlayer.syn[0].x, l2.mPlayer.syn[0].y);
      // printf("ldx:%2.0f  ldy:%2.0f  ldw:%d  ldh:%d\n", l2.level_display_region_x, l2.level_display_region_y, l2.level_display_region_w, l2.level_display_region_h);
      // printf("sdx:%d  sdy:%d  sdw:%d  sdh:%d\n", l2.screen_display_x, l2.screen_display_y, l2.screen_display_w, l2.screen_display_h);

   }



   // if DOWN pressed, cycle display pages
   if (m_parent.mPlayer.if_players_ctrl_just_pressed(p, PM_COMPMOVE_DOWN)) item[i][7]++;


   // enforce page limits
   int status = mLevel.data[lev].status;

   if ((status == 0) && (item[i][7] > 1)) item[i][7] = 0;
   if ((status == 1) && (item[i][7] > 2)) item[i][7] = 0;
   if ((status > 1)  && (item[i][7] > 4)) item[i][7] = 0;

   // no demo page in netgame
   //if (((mNetgame.ima_server) || (mNetgame.ima_client)) && (item[i][7] == 4)) item[i][7] = 0;
   if (mNetgame.active() && item[i][7] == 4) item[i][7] = 0;




}



void mwItem::draw_level_icon(int i)
{
   int x = item[i][4];
   int y = item[i][5];

   int xc = x + 10;            // x center of gate tile
   int ls = item[i][10];       // level icon size
   float lx = -0.5 + xc-ls/2;  // level icon x pos
   float ly = y+20-ls;         // level icon y pos

   int level = item[i][6];
   int status = mLevel.data[level].status;
   int color = mLevel.level_status_color[status];

   al_draw_filled_rectangle(   lx,   ly,   lx+ls+1, ly+ls+1, mColor.pc[0]);        // clear background
   mLevelIcons.draw_level_icon(level, lx+1, ly+1, ls);                             // draw level icon
   al_draw_rectangle(          lx,   ly,   lx+ls+1, ly+ls+1, mColor.pc[color], 1); // draw frame

   // draw and frame the level name
   mScreen.draw_framed_text(xc, ly-9, 0, mFont.pixl, color, 15, mLevel.data[level].level_name);
}




void mwItem::draw_gate_tile(int i)
{
   int lev = item[i][6];
   int status = mLevel.data[lev].status;

   int gate_tile_index = 0;
   int draw_mode = item[i][1];
   if (draw_mode == 1) gate_tile_index = status;
   if (draw_mode == 2) gate_tile_index = 4;
   if (draw_mode == 3) gate_tile_index = 5;
   if (draw_mode == 4) gate_tile_index = 6;


   // set the gate tile sequence
   int seq = 0; // closed door
   // only if gate_enter_mode is active and this door is gate_enter_item
   if ((mGameState.gate_enter_mode) && (mGameState.gate_enter_item == i))
   {
      if (mGameState.gate_enter_mode)     seq = 2; // fully open door
      if (mGameState.gate_enter_mode > 7) seq = 1; // partially open door
   }

   // draw the gate tile
   int x = item[i][4];
   int y = item[i][5];
   al_draw_bitmap(mBitmap.gate_tile[gate_tile_index][seq], x-10, y-39, 0);

   // draw lock overlay
   int xc = x+10; // center of tile
   int yc = y-5;  // center of tile
   if (status == 0) al_draw_scaled_bitmap(mBitmap.tile[153], 7, 6, 6, 8, xc-3, yc+7, 6, 8, 0);
}



int mwItem::draw_gate(int i, int x, int y, int custom)
{
   // to fix default shape from selection window PDE
   if (item[i][1] == 940) item[i][1] = 0;

   if (custom) al_draw_bitmap(mBitmap.sprite[139], x, y, 0 );
   else
   {
      bool player_on_gate = false;
      for (int p=0; p<NUM_PLAYERS; p++)
         if ((m_parent.mPlayer.syn[p].active) && (m_parent.mPlayer.syn[p].marked_gate == i)) player_on_gate = true;

      // draw level icon with frame and title
      if (!player_on_gate)
      {
         draw_level_icon(i);
         draw_gate_tile(i);
      }
   }
   return 1;
}



void mwItem::draw_gate_hover_info(int i, int page)
{
   int x = item[i][4];
   int y = item[i][5];
   int xc = x + 10; // center of tile

   if (page == -1) page = item[i][7];

   int bs = 200; //  + mLoop.pct_y; // page size

   // if level icon page, get page size from hover level icon size
   if (page == 0) bs = item[i][11];

   int by = y+20; // y start pos

   int level = item[i][6];
   int status = mLevel.data[level].status;
   int color = mLevel.level_status_color[status];

   if (status == 0) // locked
   {
      mScreen.draw_framed_text(xc, y+20, 0, mFont.pixl, color, 15, " DOWN - Cycle Info ");
      if (page == 0) show_page(i, 0, xc, bs, by); // level icon map
      if (page == 1) show_page(i, 7, xc, bs, by); // not completed general
   }
   if (status == 1) // ready (not completed)
   {
      mScreen.draw_framed_text(xc, y+20, 0, mFont.pixl, color, 15, " UP - Start Level ");
      mScreen.draw_framed_text(xc, y+30, 0, mFont.pixl, color, 15, " DOWN - Cycle Info ");
      if (page == 0) show_page(i,  0, xc, bs, by); // level icon map
      if (page == 1) show_page(i,  7, xc, bs, by); // not completed general
      if (page == 2) show_page(i, 12, xc, bs, by); // demo
   }
   if (status > 1) // completed
   {
      mScreen.draw_framed_text(xc, y+20, 0, mFont.pixl, color, 15, " UP - Start Level ");
      mScreen.draw_framed_text(xc, y+30, 0, mFont.pixl, color, 15, " DOWN - Cycle Info ");
      if (page == 0) show_page(i,  0, xc, bs, by); // level icon map
      if (page == 1) show_page(i,  1, xc, bs, by); // general
      if (page == 2) show_page(i,  2, xc, bs, by); // times
      if (page == 3) show_page(i,  3, xc, bs, by); // purple coins
      if (page == 4) show_page(i, 12, xc, bs, by); // demo
   }

   // draw the gate tile on top
   draw_gate_tile(i);


}


void mwItem::draw_level_icon_direct_to_screen(int i)
{
   // only draw for page 0
   if (item[i][7] != 0) return;


   // gate screen position
   int x = item[i][4];
   int y = item[i][5];
   float size = item[i][11];

   // level icon origin
   float lix1 = x-size/2 + 11;
   float liy1 = y-size   + 20;


   // find actual screen position
   float ex1=0, ey1=0;
   mScreen.calc_actual_screen_position(lix1, liy1, ex1, ey1);
   float ex2=0, ey2=0;
   mScreen.calc_actual_screen_position(lix1+size, liy1+size, ex2, ey2);

   // get new scaled size
   float esize = ex2 - ex1;

   // test draw
   //al_draw_rectangle(ex1, ey1, ex2, ey2, mColor.Red, 1);

   int level = item[i][6];

   if (l2.valid_level_loaded && l2.last_level_loaded == level) l2.draw_level3(NULL, level, ex1, ey1, esize);
   else                                                 mLevelIcons.draw_level_icon(level, ex1, ey1, esize);



}


void mwItem::show_page(int i, int page, int xc, int bs, int by)
{
   if (page) by -=40;

   float o = 0.5;

   int pad_page = 4; // mLoop.pct_y;

   // page x1 and x2 from xc and level icon width
   float x1 = (float)(xc-bs/2 - 1) + o;
   float x2 = (float)(xc+bs/2 + 1) - o;

   float size = 0;

   // debug initial y pos
   //al_draw_line(x1-100, ya, x2+100, ya, mColor.Red, 1);

   // debug center
   // al_draw_line(xc, ya-100, xc, ya+bs+100, mColor.Red, 1);

   int level = item[i][6];
   int status = mLevel.data[level].status;
   int color = mLevel.level_status_color[status];



   if (page == 0) // level icon map
   {
      size = bs+1;
      float ya = by - size + o;

      // clear
      al_draw_filled_rectangle(x1-pad_page, ya-pad_page, x2+pad_page, by, mColor.pc[0]);

      // draw
      //mLevelIcons.draw_level_icon(x1+1, ya+1, bs, level);

      // frame
      al_draw_rectangle(x1, ya, x2, by, mColor.pc[color], 1);
   }

   char msg[80];
   int yi=10;

   if (page == 1) // general
   {
      size = (6*yi)+13;
      float ya = by - size + o;

      al_draw_filled_rectangle(x1, ya, x2, by, mColor.pc[0]);
      al_draw_rectangle(x1, ya, x2, by, mColor.pc[color], 1);

      al_draw_text(mFont.pr8, mColor.pc[15], xc, ya+1, ALLEGRO_ALIGN_CENTER | ALLEGRO_ALIGN_INTEGER, "General");
      al_draw_line(x1, ya+11, x2, ya+11, mColor.pc[color], 1);
      ya+=13.5;

      draw_line(x1+2, x2-1, ya, "Min Time Overall",   chrms(mLevel.data[level].time_best,           msg), 15); ya+=yi;
      draw_line(x1+2, x2-1, ya, "Min Time w/coins",   chrms(mLevel.data[level].time_best_all_coins, msg), 15); ya+=yi;
      draw_line(x1+2, x2-1, ya, "Min Player Deaths",  chrd(mLevel.data[level].min_respawns,         msg), 15); ya+=yi;
      draw_line(x1+2, x2-1, ya, "Max Purple Coins",   chrd(mLevel.data[level].max_coins_collected,  mLevel.data[level].tot_coins, msg), 15); ya+=yi;
      draw_line(x1+2, x2-1, ya, "Max Enemies Killed", chrd(mLevel.data[level].max_enemies_killed,   msg), 15); ya+=yi;
      draw_line(x1+2, x2-1, ya, "Min Enemies Left",   chrd(mLevel.data[level].min_enemies_left,     msg), 15); ya+=yi;
   }

   if (page == 2) // times
   {
      size = (9*yi)+13;
      float ya = by - size + o;

      al_draw_filled_rectangle(x1, ya, x2, by, mColor.pc[0]);
      al_draw_rectangle(x1, ya, x2, by, mColor.pc[color], 1);
      al_draw_text(mFont.pr8, mColor.pc[15], xc, ya+1, ALLEGRO_ALIGN_CENTER | ALLEGRO_ALIGN_INTEGER, "Time to Complete");
      al_draw_line(x1, ya+11, x2, ya+11, mColor.pc[color], 1);
      ya+=13.5;

      draw_line(x1+2, x2-1, ya, "Par Time",          chrms(mLevel.data[level].time_par,            msg), 15); ya+=yi;
      draw_line(x1+2, x2-1, ya, "Min Time Overall",  chrms(mLevel.data[level].time_best,           msg), 15); ya+=yi;
      draw_line(x1+2, x2-1, ya, "Min Time w/coins",  chrms(mLevel.data[level].time_best_all_coins, msg), 15); ya+=yi;
      draw_line(x1+2, x2-1, ya, "Max Time",          chrms(mLevel.data[level].time_worst,          msg), 15); ya+=yi;
      draw_line(x1+2, x2-1, ya, "Avg Time",          chrms(mLevel.data[level].time_average,        msg), 15); ya+=yi;
      draw_line(x1+2, x2-1, ya, "Total Play Time",   chrms(mLevel.data[level].time_total,          msg), 15); ya+=yi;
      draw_line(x1+2, x2-1, ya, "Times Played",      chrd(mLevel.data[level].times_played,         msg), 15); ya+=yi;
      draw_line(x1+2, x2-1, ya, "Times Completed",   chrd(mLevel.data[level].times_beat,           msg), 15); ya+=yi;
      draw_line(x1+2, x2-1, ya, "Times Quit",        chrd(mLevel.data[level].times_quit,           msg), 15); ya+=yi;
   }

   if (page == 3) // purple coins
   {
      size = (2*yi)+25;
      float ya = by - size + o;

      al_draw_filled_rectangle(x1, ya, x2, by, mColor.pc[0]);
      al_draw_rectangle(x1, ya, x2, by, mColor.pc[color], 1);
      al_draw_bitmap(mBitmap.sprite[197], x1+2, ya+2, 0); // show purple coin
      al_draw_text(mFont.pr8, mColor.pc[15], xc, ya+8, ALLEGRO_ALIGN_CENTER | ALLEGRO_ALIGN_INTEGER, "Purple Coins");
      al_draw_line(x1, ya+23, x2, ya+23, mColor.pc[color], 1);
      ya+=25.5;

      draw_line(x1+2, x2-1, ya, "Collected", chrd(mLevel.data[level].max_coins_collected, msg), 15); ya+=yi;
      draw_line(x1+2, x2-1, ya, "Total",     chrd(mLevel.data[level].tot_coins,           msg), 15); ya+=yi;
   }

   if (page == 7) // not completed general
   {
      size = (3*yi)+13;
      float ya = by - size + o;

      al_draw_filled_rectangle(x1, ya, x2, by, mColor.pc[0]);
      al_draw_rectangle(x1, ya, x2, by, mColor.pc[color], 1);
      al_draw_text(mFont.pr8, mColor.pc[15], xc, ya+1, ALLEGRO_ALIGN_CENTER | ALLEGRO_ALIGN_INTEGER, "General");
      al_draw_line(x1, ya+11, x2, ya+11, mColor.pc[color], 1);
      ya+=13.5;

      draw_line(x1+2, x2-1, ya, "Par Time",     chrms(mLevel.data[level].time_par,      msg), 15); ya+=yi;
      draw_line(x1+2, x2-1, ya, "Purple Coins", chrd(mLevel.data[level].tot_coins,      msg), 15); ya+=yi;
      draw_line(x1+2, x2-1, ya, "Times Played", chrd(mLevel.data[level].times_played,   msg), 15); ya+=yi;
   }

   if (page == 12) // run demo for level
   {
      if ((mLevel.data[level].times_beat == 0) && (mLevel.data[level].times_played > 2)) // if not beat and started > 2 time
      {
         size = (4*yi)+13;
         float ya = by - size + o;

         al_draw_filled_rectangle(x1, ya, x2, by, mColor.pc[0]);
         al_draw_rectangle(x1, ya, x2, by, mColor.pc[color], 1);
         al_draw_text(mFont.pr8, mColor.pc[15], xc, ya+1, ALLEGRO_ALIGN_CENTER | ALLEGRO_ALIGN_INTEGER, "Run Demo");
         al_draw_line(x1, ya+11, x2, ya+11, mColor.pc[color], 1);
         ya+=13.5;

         al_draw_textf(mFont.pr8, mColor.pc[15], xc, ya, ALLEGRO_ALIGN_CENTER | ALLEGRO_ALIGN_INTEGER, "type 'demo' to run demo");  ya+=yi*2;
         al_draw_textf(mFont.pr8, mColor.pc[15], xc, ya, ALLEGRO_ALIGN_CENTER | ALLEGRO_ALIGN_INTEGER, "type 'cheat' to let demo"); ya+=yi;
         al_draw_textf(mFont.pr8, mColor.pc[15], xc, ya, ALLEGRO_ALIGN_CENTER | ALLEGRO_ALIGN_INTEGER, "complete level for you");   ya+=yi;

         if (mLevel.skc_trigger_demo)
         {
            mLevel.skc_trigger_demo = 0;
            mDemoMode.run_single_from_gate(level);
         }
      }
      else
      {
         size = (1*yi)+13;
         float ya = by - size + o;
         al_draw_filled_rectangle(x1, ya, x2, by, mColor.pc[0]);
         al_draw_rectangle(x1, ya, x2, by, mColor.pc[color], 1);
         al_draw_text(mFont.pr8, mColor.pc[15], xc, ya+1, ALLEGRO_ALIGN_CENTER | ALLEGRO_ALIGN_INTEGER, "Run Demo");
         al_draw_line(x1, ya+11, x2, ya+11, mColor.pc[color], 1);
         ya+=13.5;

         al_draw_textf(mFont.pr8, mColor.pc[15], xc, ya, ALLEGRO_ALIGN_CENTER | ALLEGRO_ALIGN_INTEGER, "type 'demo' to run demo"); ya+=yi;

         if (mLevel.skc_trigger_demo)
         {
            mLevel.skc_trigger_demo = 0;
            if (mLevel.skc_trigger_demo_cheat) mLevel.skc_trigger_demo_cheat = 0; // do nothing if trying to cheat here
            else mDemoMode.run_single_from_gate(level);
         }
      }
   }

   // draw the framed level name
   float ya = by - size - 11 + o;
   mScreen.draw_framed_text(xc, ya, 0, mFont.pr8, color, 15, mLevel.data[level].level_name);

   // // draw badges
   // int badge_y = ya+14; // + mLoop.pct_y; // up
   // if (!dir) badge_y = ya-10; // down
   //
   // // show icon for purple coin achievement
   // if (mLevel.data[lev].max_coins_collected == mLevel.data[lev].tot_coins)
   //    mBitmap.spin_shape(197, xc-56, badge_y, 0, 0, 20, 20, 1.0, 0.4, 30);
   //
   // // show icon for par time achievement
   // if ((mLevel.data[lev].time_best_all_coins > 0) && (mLevel.data[lev].time_best_all_coins < mLevel.data[lev].time_par))
   //    mBitmap.spin_shape(198, xc+56-20, badge_y, 0, 0, 20, 20, 1.0, 0.8, 40);
   //

}


// used to save absolute screen position of gate, so transition returning to overworld lines up
void mwItem::set_gate_level_icon_position(int i)
{
   // gate item location
   int x = item[i][4];
   int y = item[i][5];
   int size = item[i][11];


   // offset from gate item to level icon
   x += (10 - size/2);
   y -= (size-19);

   // convert to actual screen pos
   mTransitions.gate_transition_x = (x - m_parent.level_display_region_x) * mDisplay.scale_factor_current + m_parent.screen_display_x;
   mTransitions.gate_transition_y = (y - m_parent.level_display_region_y) * mDisplay.scale_factor_current + m_parent.screen_display_y;
   mTransitions.gate_transition_wx = m_parent.level_display_region_x;
   mTransitions.gate_transition_wy = m_parent.level_display_region_y;
   mTransitions.gate_transition_scale = mDisplay.scale_factor_current;
}

// draws left justified txt1, right justified txt2, and '...' in between
void mwItem::draw_line(int x1, int x2, int y, const char * txt1, const char * txt2, int col)
{
   // make my own copy of txt1 to append "."
   char lt[200];
   strcpy(lt, txt1);

   int mtl = (x2-x1)/8; // max text length
   int tl = strlen(txt1);
   int bs = mtl-tl; // blank spaces to fill
   for (int i=0; i<bs; i++) strcat(lt, ".");

   al_draw_text(mFont.pr8, mColor.pc[15], x1+1, y, 0, lt);

   tl = strlen(txt2)*8;
   al_draw_filled_rectangle(x2-tl-4, y, x2, y+8, mColor.pc[0]);

   al_draw_text(mFont.pr8, mColor.pc[col], x2-tl-1, y, 0, txt2);
}


// format time from frames to seconds or minutes
char * mwItem::chrms(int time, char* ft)
{
   // less than 1 minute
   if (time < 2400) sprintf(ft, "%0.1fs", (float)time/40);

   // less than 1 hour
   else if (time < 144000)
   {
      int m = time / 2400;    // minutes
      int rt = time - m*2400; // remaining portion that is less than 1 minute
      sprintf(ft, "%d:%02d", m, rt/40);
   }
   else
   {
      int h = time / 144000;    // hours
      int rt = time - h*144000; // remaining portion that is less than 1 hour
      int m = rt / 2400;        // minutes
      rt -= m*2400;             // remaining portion that is less than 1 minute
      sprintf(ft, "%d:%02d:%02d", h, m, rt/40);
   }
   return ft;
}


char * mwItem::chrd(int v, char* ft)
{
   sprintf(ft, "%d", v);
   return ft;
}

char * mwItem::chrd(int v1, int v2, char* ft)
{
   sprintf(ft, "%d/%d", v1, v2);
   return ft;
}
