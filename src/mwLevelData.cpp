// mwLevelData.cpp

#include "pm.h"
#include "mwLevel.h"
#include "mwConfig.h"
#include "mwLift.h"
#include "mwItem.h"
#include "mwEnemy.h"
#include "mwScreen.h"
#include "mwFont.h"
#include "mwInput.h"
#include "mwBitmap.h"
#include "mwColor.h"
#include "mwMiscFnx.h"
#include "mwDisplay.h"
#include "mwLoop.h"
#include "mwPlayer.h"
#include "mwVisualLevel.h"
#include "mwDemoMode.h"
#include "mwMain.h"
#include "mwNetgame.h"


void mwLevel::reset_level_data(void)
{
   clear_data();
   save_data();
   level_stats_bmp_msg_type = 0;        // to force recreation
   load_level(mLevel.play_level, 0, 0); // reload play level
}

void mwLevel::unlock_all_levels(void)
{
   for(int i=0; i<100; i++)
   {
      if (data[i].status == 0) data[i].status = 1;
      update_level_status(i);
   }
   for(int i=0; i<16; i++) area_locks[i] = 0;
   save_data();
   level_stats_bmp_msg_type = 0;        // to force recreation
   load_level(mLevel.play_level, 0, 0); // reload play level
}

void mwLevel::setup_data(void)
{
   load_data();
   load_level_icons();
}

void mwLevel::level_start_data(void)
{
//   printf("level_start_data() play_level:%d\n", play_level);
   level_data_purple_coins_collected = 0;
   level_data_player_respawns = 0;
   level_data_enemies_killed = 0;
}

void mwLevel::add_play_data_record(int lev, int type)
{
   int save_flag = 1;
   if (mPlayer.syn[mPlayer.active_local_player].control_method == PM_PLAYER_CONTROL_METHOD_DEMO_MODE) save_flag = 0; // if running demo mode, don't save data
   if (mLevel.skc_trigger_demo_cheat) save_flag = 1;                        // in cheat mode save data
   if (mDemoMode.demo_debug_running_demo_saves_level_data) save_flag = 1;   // or if this is option is set
   if (lev == 1) save_flag = 0;                                             // never save data for overworld


   if (save_flag)
   {
      mLevel.skc_trigger_demo_cheat = 0;

      int cmp = 1;                              // completed
      int ct = mPlayer.syn[0].level_done_frame; // completion time

      //   type 0 - level quit
      //   type 1 - level complete
      //   type 2 - debug completion

      if (type == 0) // level aborted
      {
         cmp = 0; // not completed
         ct = mLoop.frame_num;
      }

      if (type == 2) ct = data[lev].time_par + 1200; // fake time (par time + 30s)


      // add entry to play_data[] array
      int i = play_data_num;
      // printf("save level_complete_data(%d) %d\n", lev, i);
      play_data[i].level = lev;
      play_data[i].timer = ct;
      play_data[i].completed = cmp;
      play_data[i].player_respawns            = level_data_player_respawns;
      play_data[i].enemies_killed             = level_data_enemies_killed;
      play_data[i].purple_coins_collected     = level_data_purple_coins_collected;
      play_data[i].enemies_left_alive_at_exit = mEnemy.num_enemy;
      play_data_num++;

      calc_level_stats(lev);
   }

   check_achievments();
   save_data();
   level_stats_bmp_msg_type = 0;
}

void mwLevel::calc_level_stats(int lev)
{
   // iterates all play_data and sets these variables

   // data[lev].status = 0; // do not set this automatically

   data[lev].times_played = 0;
   data[lev].times_beat = 0;
   data[lev].times_quit = 0;

   data[lev].time_worst = 0;
   data[lev].time_average = 0;
   data[lev].time_total = 0;

   int time_played_completed_only = 0;

   for (int i=0; i<play_data_num; i++)
      if (play_data[i].level == lev)
      {
         data[lev].times_played++;
         data[lev].time_total += mLevel.play_data[i].timer;
         if (mLevel.play_data[i].timer > data[lev].time_worst) data[lev].time_worst = mLevel.play_data[i].timer;

         if (mLevel.play_data[i].completed)
         {
            data[lev].times_beat++;
            time_played_completed_only += mLevel.play_data[i].timer;


            if (data[lev].status < 2) // if locked or ready, then this is completed for the first time
            {
               data[lev].status = 2; // mark as complete and set initial values
               data[lev].time_best                  = play_data[i].timer;
               data[lev].min_respawns               = play_data[i].player_respawns;
               data[lev].max_enemies_killed         = play_data[i].enemies_killed;
               data[lev].max_purple_coins_collected = play_data[i].purple_coins_collected;
               data[lev].min_enemies_left           = play_data[i].enemies_left_alive_at_exit;

               // if all coins collected, set initial all coins time record
               if (play_data[i].purple_coins_collected >= data[lev].tot_purple_coins) data[lev].time_best_all_coins = play_data[i].timer;

            }
            else // check for new best values
            {
               if (play_data[i].timer                      < data[lev].time_best)                  data[lev].time_best                  = play_data[i].timer;
               if (play_data[i].player_respawns            < data[lev].min_respawns)               data[lev].min_respawns               = play_data[i].player_respawns;
               if (play_data[i].enemies_killed             > data[lev].max_enemies_killed)         data[lev].max_enemies_killed         = play_data[i].enemies_killed;
               if (play_data[i].purple_coins_collected     > data[lev].max_purple_coins_collected) data[lev].max_purple_coins_collected = play_data[i].purple_coins_collected;
               if (play_data[i].enemies_left_alive_at_exit < data[lev].min_enemies_left)           data[lev].min_enemies_left           = play_data[i].enemies_left_alive_at_exit;

               // if all coins collected, check for new best all coins time record
               if (play_data[i].purple_coins_collected >= data[lev].tot_purple_coins)
                  if ((data[lev].time_best_all_coins == 0) || (play_data[i].timer < data[lev].time_best_all_coins)) // no previous record or better than previous record
                     data[lev].time_best_all_coins = play_data[i].timer;

            }
         }
         else // non-completed record
         {
            data[lev].times_quit++;
            // only these can be updated by an incomplete level
            if (play_data[i].enemies_killed             > data[lev].max_enemies_killed)         data[lev].max_enemies_killed         = play_data[i].enemies_killed;
            if (play_data[i].purple_coins_collected     > data[lev].max_purple_coins_collected) data[lev].max_purple_coins_collected = play_data[i].purple_coins_collected;
         }
      }

   if (data[lev].times_beat) data[lev].time_average = time_played_completed_only/data[lev].times_beat; // average time (only count completed records)

   if ((data[lev].time_best_all_coins > 0) && (data[lev].time_best_all_coins < data[lev].time_par))  data[lev].status = 3; // perfect

}


void mwLevel::check_achievments(void)
{
   int strict = 0;
   // in strict mode all levels are locked until the previous one is completed
   // in non-strict mode you just need to beat > 50% of levels in an area to unlock the next area and levels in it

   // do the level unlocks by iterating levels with area_array, they are all in order
   // if a level is complete (status > 1) and the next level is locked (status < 1) unlock the level (status = 1)
   // do this in both strict and non strict mode
   for(int i=0; i<99; i++)
     if ((data[area_array[i][0]].status > 1) && (data[area_array[i+1][0]].status < 1)) data[area_array[i+1][0]].status = 1;


   // check for completed areas and do the area unlocks
   for (int a=1; a<15; a++) // iterate areas
   {
      int area_complete = 0;

      if (strict)
      {
         area_complete = 1; // area complete by default
         for(int j=0; j<100; j++)
            if ((area_array[j][1] == a) && (data[area_array[j][0]].status < 2)) area_complete = 0; // not complete
      }
      else
      {
         int tot = 0; // count number of levels in area
         int cmp = 0; // count number of completed levels in area
         for(int j=0; j<100; j++)
            if (area_array[j][1] == a)
            {
               tot++;
               if (data[area_array[j][0]].status > 1) cmp++;
            }
         float d = (float)cmp / (float)tot;
         if (d > .5) area_complete = 1;
      }

      if (area_complete)
      {
         area_locks[a+1] = 0; // open barrier next area
         if (!strict) unlock_all_level_in_area(a+1);

         if (a == 14) // training level 2 (14) opens area 1
         {
            area_locks[1] = 0;
            if (!strict) unlock_all_level_in_area(1);
         }

         if (a == 9) // final area (9) also opens demo
         {
            area_locks[12] = 0;
            if (!strict) unlock_all_level_in_area(12);
         }

         if (!strict) if (a == 13) // training level 1 (13) also opens area 1 (non strict mode only)
         {
            area_locks[1] = 0;
            if (!strict) unlock_all_level_in_area(1);
         }
      }
   }

   for (int i=0; i<100; i++) update_level_status(i);
}

void mwLevel::update_level_status(int lev)
{
   if (data[lev].status == 0) { sprintf(data[lev].status_text, "Locked");   data[lev].status_color = 10; } // red
   if (data[lev].status == 1) { sprintf(data[lev].status_text, "Ready");    data[lev].status_color = 13; } // lt blue
   if (data[lev].status == 2) { sprintf(data[lev].status_text, "Complete"); data[lev].status_color = 12; } // dk blue
   if (data[lev].status == 3) { sprintf(data[lev].status_text, "Perfect");  data[lev].status_color = 8;  } // purple
}



void mwLevel::sob_hline(int x1, int x2, int y, int a)
{
   for(int x=x1; x<x2+1; x++)
   {
      int block = 0;
      if (a)
      {
                      block = 582 | PM_BTILE_ALL_SOLID; // thru
         if (x == x1) block = 590 | PM_BTILE_ALL_SOLID;
         if (x == x2) block = 588 | PM_BTILE_ALL_SOLID;
      }
      mLevel.l[x][y] = block;
   }
}

void mwLevel::sob_vline(int x, int y1, int y2, int a)
{
   for(int y=y1; y<y2+1; y++)
   {
      int block = 0;
      if (a)
      {
                      block = 580 | PM_BTILE_ALL_SOLID; // thru
         if (y == y1) block = 591 | PM_BTILE_ALL_SOLID;
         if (y == y2) block = 589 | PM_BTILE_ALL_SOLID;
      }
      mLevel.l[x][y] = block;
   }
}

void mwLevel::sob_create_msg(const char* txt, int col, int x, int y, int w, int h)
{
   // check and delete any existing message with first 7 char matching txt 'Area xx'
   char msg[80];
   sprintf(msg, "%.7s", txt);

   int msg_id = find_msg(msg);
   if (msg_id != -1) mItem.erase_item(msg_id); // erase msg if it exists

   // create new message
   int i = mItem.get_empty_item(10);
   mItem.item[i][0] = 10;
   mItem.item[i][2] = 65;
   mItem.item[i][4] = x-10;
   mItem.item[i][5] = y-10;
   mItem.item[i][6] = x;
   mItem.item[i][7] = y;
   mItem.item[i][8] = w;
   mItem.item[i][9] = h;
   mMiscFnx.set_int_3216(mItem.item[i][13], 15, col); // set text and frame colors
   strcpy(mItem.pmsgtext[i], txt);
}


void mwLevel::sob_area_msg(int area, int x, int y)
{
   int w = 95;
   int h = 57;

   char msg1[20];
   char msg2[20];
   char msg3[20];
   char msg4[20];


   sprintf(msg1, "Area %d", area);

   if (area == 13) { sprintf(msg1, "Basic Training"); w = 130; }
   if (area == 14) { sprintf(msg1, "More Training");  w = 130; }
   if (area == 9)  { sprintf(msg1, "Final Area");     w = 110; }
   if (area == 16) { sprintf(msg1, "All Levels");     w = 110; }
   if (area == 17) { sprintf(msg1, "Game Levels");    w = 110; }
   if (area == 10) { sprintf(msg1, "Extra Levels");   w = 120; }
   if (area == 12) { sprintf(msg1, "Demo Levels");    w = 120; }


   int cmp = 0; // count how many levels in this area are complete
   int pct = 0; // count total purple coins in this area
   int pcc = 0; // count max purple coins collected in this area
   int tmr = 0; // count how many levels in this area have below par time

   int nl = 0;
   for (int i=0; i<100; i++)
      if ((area_array[i][1] == area) || ((area == 16) && (area_array[i][1]) && (area_array[i][1] != 11)) || ((area == 17) && (area_array[i][1] > 0) && (area_array[i][1] < 10)  ))
      {
         nl++;
         int l = area_array[i][0];
         if (data[l].status > 1) cmp++;
         pct += data[l].tot_purple_coins;
         pcc += data[l].max_purple_coins_collected;
         if ((data[l].time_best > 0) && (data[l].time_best < data[l].time_par) && (data[l].status > 1)) tmr++;
      }

   int col = 10;
   if ((area_locks[area]) && (area != 16) && (area != 17) && (!mNetgame.ima_client)) // locked
   {
      sprintf(msg2, "(Locked)");
      col = 10;
   }
   else
   {
      sprintf(msg2, "Done:%d/%d", cmp, nl);
      col = 13;

      if (cmp == nl)
      {
         sprintf(msg2, "Complete");
         col = 12;
      }
      if ((pcc == pct) && (tmr == nl))
      {
         sprintf(msg2, "Perfect!");
         col = 8;
      }
   }

   sprintf(msg3, "Coin:%d/%d", pcc, pct);
   //sprintf(msg3, "Coin:15/15");
   sprintf(msg4, "Time:%d/%d", tmr, nl);

   char msg[256];
   sprintf(msg, "%s\n%s\n%s\n%s", msg1, msg2, msg3, msg4);
   sob_create_msg(msg, col, x, y, w, h);
}

int mwLevel::find_msg(const char* str)
{
   int msg_id = -1;
   for(int i=0; i<500; i++)
      if ((mItem.item[i][0] == 10) && (!strncmp(mItem.pmsgtext[i], str, strlen(str)))) msg_id = i;
   return msg_id;
}


// only ever called from load level after loading level 1
void mwLevel::set_overworld_barriers(void)
{

   // --------------------------------
   // area 0
   // --------------------------------
   int msg_id = find_msg("Complete Basic");
   if (area_locks[1])   // locked between training levels and area 1,2
   {
      if (msg_id == -1) // create msg if it does not exist
      {
         int i = mItem.get_empty_item(10);
         mItem.item[i][0] = 10;
         mItem.item[i][2] = 11;
         mItem.item[i][4] = 980;
         mItem.item[i][5] = 160;
         mItem.item[i][6] = 867;
         mItem.item[i][7] = 183;
         mItem.item[i][8] = 266;
         mItem.item[i][9] = 15;
         mItem.item[i][13] = 851980;
         sprintf(mItem.pmsgtext[i], "Complete Basic Training");
      }
   }
   else if (msg_id != -1) mItem.erase_item(msg_id); // erase msg if it exists


   sob_area_msg(13, 625,  40); // basic training levels
   sob_area_msg(14, 1250, 40); // advanced training levels

   sob_area_msg(1,  745,  220);
   sob_area_msg(2,  1161, 220);
   sob_area_msg(3,  745,  400);
   sob_area_msg(4,  1161, 400);
   sob_area_msg(5,  745,  580);
   sob_area_msg(6,  1161, 580);
   sob_area_msg(7,  745,  760);
   sob_area_msg(8,  1161, 760);

   sob_area_msg(9,  725,  940);  // final area


   sob_area_msg(17, 1161, 940);  // main area levels
   sob_area_msg(16, 1161, 1010); // all levels

   sob_area_msg(10, 745,  1410); // extra levels
   sob_area_msg(12, 940,  1750); // demo levels


   // blocking lines of blocks
   sob_vline(60, 1,  8,  area_locks[14]);  // blocking area 14 (advanced training)

   sob_vline(56, 10, 17, area_locks[2]);   // blocking area 2
   sob_vline(56, 19, 26, area_locks[4]);   // blocking area 4
   sob_vline(56, 28, 35, area_locks[6]);   // blocking area 6
   sob_vline(56, 37, 44, area_locks[8]);   // blocking area 8

   sob_hline(45, 54, 9,  area_locks[1]);   // blocking between training levels and area 1, 2
   sob_hline(45, 54, 18, area_locks[3]);   // blocking between area 1,2 and area 3,4
   sob_hline(45, 54, 27, area_locks[5]);   // blocking between area 3,4 and area 5,6
   sob_hline(45, 54, 36, area_locks[7]);   // blocking between area 5,6 and area 7,8
   sob_hline(45, 54, 45, area_locks[9]);   // blocking between area 7,8 and area 9

   sob_vline(43, 46, 53, area_locks[10]);   // blocking between area 9 and the rest
   sob_vline(56, 46, 53, area_locks[10]);   // blocking between area 9 and the rest

   sob_vline(0,  46, 57, !area_locks[10]);   // edge pieces that I want invisible until 10 done
   sob_vline(99, 46, 84, !area_locks[10]);   // edge pieces that I want invisible until 10 done


   // how much of overworld level to hide
   int lowest_visible_y = 200; // default only top training row visible
   if (!area_locks[1]) lowest_visible_y = 380; // area 1 and 2 visible
   if (!area_locks[3]) lowest_visible_y = 560; // area 3 and 4 visible
   if (!area_locks[5]) lowest_visible_y = 740; // area 5 and 6 visible
   if (!area_locks[7]) lowest_visible_y = 920; // area 7 visible
   if (!area_locks[9]) lowest_visible_y = 1100; // area 9 visible
   if (!area_locks[10]) lowest_visible_y = 2000; // area 9 visible

   // remove all hiders
   for(int i=0; i<500; i++) if (mItem.item[i][0] == 19) mItem.item[i][0] = 0;

   // create hider
   int i = mItem.get_empty_item(19);
   mItem.item[i][0] = 19;
   mItem.item[i][1] = 0;
   mItem.item[i][2] = 1;
   mItem.item[i][3] = 1;
   mItem.item[i][4] = 40;
   mItem.item[i][5] = 40;
   mItem.item[i][7] = lowest_visible_y;
   mItem.item[i][8] = 2000;
   mItem.item[i][9] = 2000 - lowest_visible_y;

   save_level(1); // save overworld level
}



void mwLevel::faa_helper(int x1, int x2, int y1, int y2, int xasc, int &aai, int area)
{
   int res[50][3] = { 0 }; // results array
   int resi = 0;

   // find all gates in the area
   for(int i=0; i<100; i++)
      if ((mItem.item[i][0] == 18) && (mItem.item[i][4] >= x1) && (mItem.item[i][4] <= x2) && (mItem.item[i][5] >= y1) && (mItem.item[i][5] <= y2))
      {
         res[resi][0] = mItem.item[i][6]; // level num
         res[resi][1] = mItem.item[i][4]; // x
         res[resi][2] = mItem.item[i][5]; // y
         resi++;
      }

   // sort results by x position
   int swap_flag = 1;
   while (swap_flag)
   {
      swap_flag = 0;
      for(int i=0; i<resi-1; i++)
         if ( ((xasc == 1) && (res[i][1] > res[i+1][1])) || ((xasc == 0) && (res[i][1] < res[i+1][1])) )  // sort by x position, ascending or desc
         {
            swap_flag = 1;
            for (int k=0; k<3; k++)
            {
               res[49 ][k]  = res[i  ][k];  // use 49 for temp swap
               res[i  ][k]  = res[i+1][k];
               res[i+1][k]  = res[49 ][k];
            }
         }
   }

   // add to area array
   for (int i=0; i<resi; i++)
   {
      area_array[aai][0] = res[i][0];
      area_array[aai][1] = area;
      aai++;
   }
}



// searches the overworld level for gates in certain areas, then adds their levels to area array
void mwLevel::fill_area_array(void)
{
   // clear area array and index
   for (int i=0; i<100; i++)
   {
      area_array[i][0] = 0;
      area_array[i][1] = 0;
   }
   int aai = 0;

   faa_helper(0,    700,  0,    180,  0, aai, 13); // basic training levels
   faa_helper(1000, 2000, 0,    180,  1, aai, 14); // advanced training levels
   faa_helper(0,    1000, 200,  360,  0, aai, 1);  // area 1
   faa_helper(1000, 2000, 200,  360,  1, aai, 2);  // area 2
   faa_helper(0,    1000, 380,  540,  0, aai, 3);  // area 3
   faa_helper(1000, 2000, 380,  540,  1, aai, 4);  // area 4
   faa_helper(0,    1000, 560,  720,  0, aai, 5);  // area 5
   faa_helper(1000, 2000, 560,  720,  1, aai, 6);  // area 6
   faa_helper(0,    1000, 740,  900,  0, aai, 7);  // area 7
   faa_helper(1000, 2000, 740,  900,  1, aai, 8);  // area 8
   faa_helper(900,  1100, 920,  1080, 1, aai, 9);  // area 9 (final boss)
   faa_helper(0,    1000, 1160, 1700, 1, aai, 10); // area 10 (extra levels)
   faa_helper(1000, 2000, 1160, 1700, 1, aai, 11); // area 11 (muliplayer bomb toss levels)
   faa_helper(0,    2000, 1840, 2000, 1, aai, 12); // area 12 (advanced info levels)
//   for(int i=0; i<aai; i++) printf("area:%d level:%d\n", area_array[i][1], area_array[i][0]);
}


void mwLevel::unlock_all_level_in_area(int area)
{
   for(int i=0; i<100; i++)
      if (area_array[i][1] == area)
         if (data[area_array[i][0]].status < 1) data[area_array[i][0]].status = 1;
}


void mwLevel::clear_data(void)
{
   for(int i=0; i<16; i++) area_locks[i] = 1; // set all locks
   area_locks[13] = 0; // basic training area unlocked

   load_level(1, 1, 0); // load overworld level to set overworld barriers
   fill_area_array();

   mPlayer.syn[0].overworld_last_touched_gate = 0; // no gate will be found and player will start from start block
   mConfig.save_config(PM_CFG_SAVE_OVERWORLD_GATE_LAST_TOUCHED);


   for(int i=0; i<100; i++)
   {
      strcpy(data[i].level_name, "");

      data[i].status = 0;
      data[i].status_color = 0;
      strcpy(data[i].status_text, "");

      data[i].time_par = 0;
      data[i].time_best = 0;
      data[i].time_best_all_coins = 0;
      data[i].time_worst = 0;
      data[i].time_average = 0;
      data[i].time_total = 0;

      data[i].times_played = 0;
      data[i].times_beat = 0;
      data[i].times_quit = 0;

      data[i].min_respawns = 0;
      data[i].max_purple_coins_collected = 0;
      data[i].max_enemies_killed = 0;
      data[i].tot_purple_coins = 0;
      data[i].min_enemies_left = 0;
      data[i].min_enemies_left_par = 0;
   }

   int i = 1;
   strcpy(data[i].level_name, "Overworld Map");
   data[i].time_par = 0;


   i = 2;
   strcpy(data[i].level_name, "Switch Nest");
   data[i].time_par = 4800; // 2:00 demo 1:35

   i = 3;
   strcpy(data[i].level_name, "Blue Key Fall");
   data[i].time_par = 1400; // 35s demo 27.9

   i = 4;
   strcpy(data[i].level_name, "Switch Puzzle");
   data[i].time_par = 7200; // 3:00 demo 2:20

   i = 5;
   strcpy(data[i].level_name, "Kill Kill Kill");
   data[i].time_par = 6400; // 2:40 demo 2:18

   i = 6;
   strcpy(data[i].level_name, "Breakable");
   data[i].time_par = 9600; // 4:00 demo 2:58

   i = 7;
   strcpy(data[i].level_name, "Ranger Bob");
   data[i].time_par = 2400; // 1:00 demo 46.5

   i = 8;
   strcpy(data[i].level_name, "Switch Pit");
   data[i].time_par = 8400; // 3:30 demo 2:52

   i = 10;
   strcpy(data[i].level_name, "The Dead Zone");
   data[i].time_par = 9600; // 4:00 demo 3:15

   i = 11;
   strcpy(data[i].level_name, "Good and Evil");
   data[i].time_par = 14400; // 6:00 demo 4:45

   i = 12;
   strcpy(data[i].level_name, "Bucket of Bad");
   data[i].time_par = 16800; // 7:00 demo 6:10

   i = 13;
   strcpy(data[i].level_name, "Falling Arrows");
   data[i].time_par = 14400; // 6:00 demo 4:41

   i = 14;
   strcpy(data[i].level_name, "Block Puzzle");
   data[i].time_par = 600; // 15s demo 8.8

   i = 15;
   strcpy(data[i].level_name, "Smash");
   data[i].time_par = 1600; // 40s demo 35.0

   i = 16;
   strcpy(data[i].level_name, "Big Apple");
   data[i].time_par = 14400; // 6:00 demo 4:01

   i = 17;
   strcpy(data[i].level_name, "Little Rocket");
   data[i].time_par = 16800; // 7:00 demo 5:08

   i = 18;
   strcpy(data[i].level_name, "Incinerator");
   data[i].time_par = 12000; // 5:00 demo 4:04

   i = 19;
   strcpy(data[i].level_name, "Crazy Fox");
   data[i].time_par = 16800; // 7:00 demo 5:24

   i = 20;
   strcpy(data[i].level_name, "Strange Things");
   data[i].time_par = 7200; // 3:00 demo 2:22

   i = 21;
   strcpy(data[i].level_name, "Long One");
   data[i].time_par = 4200; // 1:45 demo 1:25

   i = 22;
   strcpy(data[i].level_name, "Rocket Stew");
   data[i].time_par = 4800; // 2:00 demo 1:28

   i = 23;
   strcpy(data[i].level_name, "Amazing");
   data[i].time_par = 19200; // 8:00 demo 5:58

   i = 24;
   strcpy(data[i].level_name, "Wendy");
   data[i].time_par = 7200; // 3:00 demo 2:26

   i = 26;
   strcpy(data[i].level_name, "Long Fall");
   data[i].time_par = 4800; // 2:00 demo 1:06

   i = 27;
   strcpy(data[i].level_name, "Gauntlet");
   data[i].time_par = 3600; // 1:30 demo 1:23

   i = 28;
   strcpy(data[i].level_name, "Nowhere to Stand");
   data[i].time_par = 3600; // 1:30 demo 49.2

   i = 29;
   strcpy(data[i].level_name, "Brain Trust");
   data[i].time_par = 13200; // 5:30 demo 3:58

   i = 30;
   strcpy(data[i].level_name, "Coral Cave");
   data[i].time_par = 7200; // 3:00 demo 2:30

   i = 31;
   strcpy(data[i].level_name, "Beginnings");
   data[i].time_par = 19200; // 8:00 demo 6:03

   i = 32;
   strcpy(data[i].level_name, "Yellow Cake");
   data[i].time_par = 9600; // 4:00 demo 3:19

   i = 33;
   strcpy(data[i].level_name, "Kate's Gate");
   data[i].time_par = 6000; // 2:30 demo 2:06

   i = 34;
   strcpy(data[i].level_name, "Crush");
   data[i].time_par = 3600; // 1:30 demo 1:04

   i = 35;
   strcpy(data[i].level_name, "Bomb Toss");
   data[i].time_par = 12000; // 5:00 demo 3:36

   i = 38;
   strcpy(data[i].level_name, "Come Over");
   data[i].time_par = 3200; // 1:20 demo 1:06

   i = 39;
   strcpy(data[i].level_name, "Bomb Run");
   data[i].time_par = 3200; // 1:20 demo 55.7

   i = 40;
   strcpy(data[i].level_name, "Edward");
   data[i].time_par = 4200; // 1:45 demo 1:05

   i = 41;
   strcpy(data[i].level_name, "Ender");
   data[i].time_par = 9600; // 4:00 demo 3:27

   i = 42;
   strcpy(data[i].level_name, "Cloner Chain");
   data[i].time_par = 16800; // 7:00 demo 6:14

   i = 43;
   strcpy(data[i].level_name, "Cathedral");
   data[i].time_par = 4800; // 2:00 demo 1:45

   i = 44;
   strcpy(data[i].level_name, "Too Many Lifts");
   data[i].time_par = 9600; // 4:00 demo 2:59

   i = 45;
   strcpy(data[i].level_name, "Chimney of Death");
   data[i].time_par = 1800; // :45 demo 28.6

   i = 46;
   strcpy(data[i].level_name, "Bucket List");
   data[i].time_par = 2400; // 1:00 demo 38.8

   i = 47;
   strcpy(data[i].level_name, "Begin Again");
   data[i].time_par = 2400; // 1:00 demo 27.1

   i = 48;
   strcpy(data[i].level_name, "Mayhem");
   data[i].time_par = 2400; // 1:00 demo 41.8

   i = 49;
   strcpy(data[i].level_name, "Rocket Square");
   data[i].time_par = 9600; // 4:00 demo 2:37

   i = 50;
   strcpy(data[i].level_name, "Welcome");
   data[i].time_par = 1600; // :40 demo 30.2

   i = 51;
   strcpy(data[i].level_name, "Cannon Bees");
   data[i].time_par = 6000; // 2:30 demo 2:21

   i = 52;
   strcpy(data[i].level_name, "Boules");
   data[i].time_par = 3600; // 1:30 demo 1:07

   i = 53;
   strcpy(data[i].level_name, "Lifts With Names");
   data[i].time_par = 8400; // 3:30 demo 2:59

   i = 54;
   strcpy(data[i].level_name, "Old Training Level");
   data[i].time_par = 2400; // 1:00 demo 31.9

   i = 55;
   strcpy(data[i].level_name, "Ancient Ruins");
   data[i].time_par = 9600; // 4:00 demo 2:33

   i = 56;
   strcpy(data[i].level_name, "Zaiden");
   data[i].time_par = 9600; // 4:00 demo 2:44

   i = 57;
   strcpy(data[i].level_name, "Valentine");
   data[i].time_par = 7200; // 3:00 demo 2:43


   i = 60;
   strcpy(data[i].level_name, "Bomb Toss"); // same name as level 35...
   data[i].time_par = 2400; // 2:00 demo // not done

   i = 61;
   strcpy(data[i].level_name, "Bomb Toss 2");
   data[i].time_par = 2400; // 2:00 demo

   i = 64;
   strcpy(data[i].level_name, "Escape Rocket!");
   data[i].time_par = 12000; // 5:00 demo 4:30


   // ---------------------------------------
   // Test Levels
   // ---------------------------------------

//   i = 70;
//   strcpy(data[i].level_name, "Test Level 1");
//   data[i].time_par = 200;
//   data[i].unlocked = 1;
//
//   i = 71;
//   strcpy(data[i].level_name, "Test Level 2");
//   data[i].time_par = 240;



   // ---------------------------------------
   // Training Levels
   // ---------------------------------------

   i = 80;
   strcpy(data[i].level_name, "Training Level 1");
   data[i].time_par = 1200; // 30s demo 21.2
   data[i].status = 1;

   i = 81;
   strcpy(data[i].level_name, "Training Level 2");
   data[i].time_par = 4800; // 2:00 demo 53.4

   i = 82;
   strcpy(data[i].level_name, "Training Level 3");
   data[i].time_par = 8400; // 3:30 demo 2:12

   i = 83;
   strcpy(data[i].level_name, "Training Level 4");
   data[i].time_par = 6000; // 2:30 demo 1:14

   i = 84;
   strcpy(data[i].level_name, "Training Level 5");
   data[i].time_par = 3600; // 1:30 demo 52.8

   i = 9;
   strcpy(data[i].level_name, "Bomb Intro");
   data[i].time_par = 6000; // 2:30 demo 1:35

   i = 25;
   strcpy(data[i].level_name, "One Cannon");
   data[i].time_par = 1000; // 25s demo 21.9

   i = 37;
   strcpy(data[i].level_name, "Sacrifice");
   data[i].time_par = 800; // :20 demo 13.3


   // ---------------------------------------
   // Advanced Information Levels
   // ---------------------------------------
   i = 85;
   strcpy(data[i].level_name, "Doors");
   data[i].time_par = 4800; // 2:00 demo 1:48

   i = 86;
   strcpy(data[i].level_name, "Triggers and Timers");
   data[i].time_par = 3600; // 1:30 demo 1:26

   i = 87;
   strcpy(data[i].level_name, "Block Manip");
   data[i].time_par = 2400; // 1:00 demo 28.4

   i = 88;
   strcpy(data[i].level_name, "Block Damage");
   data[i].time_par = 3600; // 1:30 demo 1:24

   i = 89;
   strcpy(data[i].level_name, "Orbs");
   data[i].time_par = 2400; // 1:00 demo 51.8

   i = 90;
   strcpy(data[i].level_name, "Archwagons");
   data[i].time_par = 4800; // 2:00 demo 1:51

   i = 91;
   strcpy(data[i].level_name, "Trakbots");
   data[i].time_par = 800; // :20 demo 18.9

   i = 92;
   strcpy(data[i].level_name, "Bouncers and Cannons");
   data[i].time_par = 3600; // 1:30 demo 1:11

   i = 93;
   strcpy(data[i].level_name, "Cloners");
   data[i].time_par = 1800; // :45 demo 35.5

   i = 94;
   strcpy(data[i].level_name, "Vinepods");
   data[i].time_par = 1800; // :45 demo 37.7

   i = 95;
   strcpy(data[i].level_name, "Blokwalks");
   data[i].time_par = 3600; // 1:30 demo 36.4

   i = 96;
   strcpy(data[i].level_name, "Flappers");
   data[i].time_par = 600; // :15 demo 3.9

   i = 97;
   strcpy(data[i].level_name, "Jumpworms");
   data[i].time_par = 3600; // 1:30 demo 1:18

   i = 98;
   strcpy(data[i].level_name, "Lifts");
   data[i].time_par = 3600; // 1:30 demo 1:11


   for (int i=0; i<100000; i++)
   {
      play_data[i].level = 0;
      play_data[i].start_timestamp=0;
      play_data[i].timer=0;
      play_data[i].completed = 0;
      play_data[i].enemies_killed = 0;
      play_data[i].enemies_left_alive_at_exit = 0;
      play_data[i].player_respawns = 0;
      play_data[i].purple_coins_collected = 0;
   }
   play_data_num = 0;

   unlock_all_level_in_area(10);
   unlock_all_level_in_area(11);
   unlock_all_level_in_area(12);


   double t0 = al_get_time();

   // load all levels to get purple coin counts
   for (int p=0; p<100; p++)
      if (mLevel.load_level(p, 1, 1))
      {
         update_level_status(p);

         data[p].tot_purple_coins = 0;
         for (int i=0; i<500; i++)
            if ((mItem.item[i][0] == 2) && (mItem.item[i][6] == 3)) data[p].tot_purple_coins++;
      }

   printf("clear load levels time:%f\n", al_get_time() - t0);

   save_data();
   level_stats_bmp_msg_type = 0;

}



void mwLevel::load_data(void)
{
   FILE *fp =fopen("data/level_data.pm","rb");
   if (fp)
   {
      fread(data,           sizeof(data),          1, fp);
      fread(area_locks,     sizeof(area_locks),    1, fp);
      fread(area_array,     sizeof(area_array),    1, fp);
      fread(play_data,      sizeof(play_data),     1, fp);
      fread(&play_data_num, sizeof(play_data_num), 1, fp);
      fclose(fp);
      return;
   }
   else
   {
      printf("Error loading data/level_data.pm, recreating....\n");
      clear_data();
      save_data();
   }
}

void mwLevel::save_data(void)
{
   FILE *fp =fopen("data/level_data.pm","wb");
   if (fp)
   {
      fwrite(data,           sizeof(data),          1, fp);
      fwrite(area_locks,     sizeof(area_locks),    1, fp);
      fwrite(area_array,     sizeof(area_array),    1, fp);
      fwrite(play_data,      sizeof(play_data),     1, fp);
      fwrite(&play_data_num, sizeof(play_data_num), 1, fp);
      fclose(fp);
      return;
   }
   mInput.m_err("Error saving level_data.pm");
}





void mwLevel::dump_level_data(void)
{
//   printf("Play Data:\n");
//   for (int i=0; i<play_data_num; i++)
//      printf("i:%d lev:%d tmr:%d cmp:%d ek:%d\n", i, play_data[i].level, play_data[i].timer, play_data[i].completed, play_data[i].enemies_killed);
//
//   printf("\nLevel Data:\n");
//   for(int i=80; i<85; i++)
//      printf("i:%d t_b:%d ek:%d st:%d stc:%d sts:%s \n", i, data[i].time_best, data[i].max_enemies_killed, data[i].status, data[i].status_color, data[i].status_text);
//
//   printf("\nArea Array:\n");
//   for(int i=0; i<5; i++)
//      printf("i:%d %d %d\n", i, area_array[i][0], area_array[i][1]);


   printf("\nArea Locks:\n");
   for(int i=0; i<16; i++)
      printf("i:%d %d \n", i, area_locks[i]);


}

void mwLevel::create_level_icons(void)
{
   //double t0 = al_get_time();

   int sz1 = 100;
   ALLEGRO_BITMAP *tmp_100 = al_create_bitmap(sz1*10, sz1*10);
   al_set_target_bitmap(tmp_100);
   al_clear_to_color(al_map_rgba(0,0,0,0));

   int sz2 = 200;
   ALLEGRO_BITMAP *tmp_200 = al_create_bitmap(sz2*10, sz2*10);
   al_set_target_bitmap(tmp_200);
   al_clear_to_color(al_map_rgba(0,0,0,0));

   int x=0;
   int y=0;

   level_icons_loaded = 1; // this has to be here to prevent a recursive loop when load level gets called 100 times

   for (int i=0; i<100; i++)
   {
      if (mLevel.load_level(i, 0, 1))
      {
         mScreen.draw_level2(tmp_100,  x*sz1, y*sz1, sz1, 1, 1, 1, 1, 0);
         mScreen.draw_level2(tmp_200,  x*sz2, y*sz2, sz2, 1, 1, 1, 1, 0);
      }

      // show progress bar
      int pc = (i*100 + 1) / 100;
      al_set_target_backbuffer(mDisplay.display);
      //al_clear_to_color(al_map_rgb(0,0,0));
      mScreen.draw_percent_bar(mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2, mDisplay.SCREEN_W-200, 20, pc );
      al_draw_text(mFont.pr8, mColor.pc[15], mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2+6, ALLEGRO_ALIGN_CENTER, "Creating Level Icons");
      al_flip_display();

      // iterate through the rows and columns
      if (++x > 9)
      {
         x = 0;
         y++;
      }
   }

   al_save_bitmap("data/level_icons_100.bmp", tmp_100);
   al_save_bitmap("data/level_icons_200.bmp", tmp_200);

   al_destroy_bitmap(tmp_100);
   al_destroy_bitmap(tmp_200);

   //printf("create_level_icons_time:%f\n", al_get_time() - t0);
   load_level_icons();
}

void mwLevel::load_level_icons(void)
{
   if (!mDisplay.no_display)
   {
      ALLEGRO_BITMAP *tmp_100 = al_load_bitmap("data/level_icons_100.bmp");
      ALLEGRO_BITMAP *tmp_200 = al_load_bitmap("data/level_icons_200.bmp");
      if ((!tmp_100) || (!tmp_200))
      {
         printf("Error loading level_icon bitmaps - recreating\n");
         create_level_icons();
      }
      else
      {
         int x=0, y=0;
         for (int i=0; i<100; i++)
         {
            al_set_target_bitmap(level_icon_100[i]);
            al_clear_to_color(al_map_rgba(0,0,0,0));
            al_draw_bitmap_region(tmp_100, x*100, y*100, 100, 100, 0, 0, 0);

            al_set_target_bitmap(level_icon_200[i]);
            al_clear_to_color(al_map_rgba(0,0,0,0));
            al_draw_bitmap_region(tmp_200, x*200, y*200, 200, 200, 0, 0, 0);

            // iterate through the rows and columns
            if (++x > 9)
            {
               x = 0;
               y++;
            }
         }
         al_destroy_bitmap(tmp_100);
         al_destroy_bitmap(tmp_200);
         level_icons_loaded = 1;
      }
   }
}
