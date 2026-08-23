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
#include "mwGameEvent.h"
#include "mwGmInfo.h"
#include "mwMain.h"
#include "mwNetgame.h"
#include "mwSql.h"


void mwLevel::reset_level_data(void)
{
   clear_data();
   save_data();
   level_stats_bmp_msg_type = 0;        // to force recreation
   load_level(mLevel.play_level, 0, 0); // reload play level
}

void mwLevel::unlock_all_levels(void)
{
   // only change levels that were locked to start with
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

/*
int mwLevel::get_level_status(int level, int &status_color, char * status_text)
{
   char sql[1024];
   sprintf(sql, "SELECT status FROM level_play_data WHERE level=%d", level);

   int status = mSql.execute_sql_and_return_one_int(sql, mSql.db_level_play_stats);

   if (status == 0) { sprintf(status_text, "Locked");   status_color = 10; } // red
   if (status == 1) { sprintf(status_text, "Ready");    status_color = 13; } // lt blue
   if (status == 2) { sprintf(status_text, "Complete"); status_color = 12; } // dk blue
   if (status == 3) { sprintf(status_text, "Perfect");  status_color = 8;  } // purple

   return status;
}
*/

void mwLevel::set_status_text_and_color(int level)
{
   int i = level;
   if (data[i].status == 0) { sprintf(data[i].status_text, "Locked");   data[i].status_color = 10; } // red
   if (data[i].status == 1) { sprintf(data[i].status_text, "Ready");    data[i].status_color = 13; } // lt blue
   if (data[i].status == 2) { sprintf(data[i].status_text, "Complete"); data[i].status_color = 12; } // dk blue
   if (data[i].status == 3) { sprintf(data[i].status_text, "Perfect");  data[i].status_color = 8;  } // purple
}


void mwLevel::add_play_data_record(int lev, int type)
{
   // printf("void mwLevel::add_play_data_record(lev:%d, type:%d))\n", lev, type);

   int save_flag = 1;
   if (mPlayer.syn[mPlayer.active_local_player].control_method == PM_PLAYER_CONTROL_METHOD_DEMO_MODE) save_flag = 0; // if running demo mode, don't save data
   if (mLevel.skc_trigger_demo_cheat) save_flag = 1;                        // in cheat mode save data
   if (mDemoMode.demo_debug_running_demo_saves_level_data) save_flag = 1;   // or if this is option is set
   if (lev == 1) save_flag = 0;                                             // never save data for overworld
   if (save_flag)
   {
      mLevel.skc_trigger_demo_cheat = 0;

      if (type == 2)  // debug - mark level complete with UP on gate
      {
         char sql[2000];
         sprintf(sql, "INSERT INTO play_data (level, time, completed, num_players, player_respawns, enemies_killed, enemies_left, coins_collected) VALUES(%d, %d, %d, %d, %d, %d, %d, %d);", lev, data[lev].time_par + 1200,  1, 0, 0,0,0,0);
         mSql.execute_sql(sql, mSql.db_level_play_stats);
         calc_level_stats(lev);
      }

      else
      {
         mGmInfo.fill();
         mGmInfo.add();
      }

      check_achievements();
      save_data();
      level_stats_bmp_msg_type = 0;
   }
}



void mwLevel::calc_level_stats(int lev)
{
   // iterates all play_data and sets these variables

   data[lev].times_played = 0;
   data[lev].times_beat = 0;
   data[lev].times_quit = 0;

   data[lev].time_worst = 0;
   data[lev].time_average = 0;
   data[lev].time_total = 0;

   int time_played_completed_only = 0;

   char sql[4000];
   sprintf(sql, "SELECT time, completed, player_respawns, enemies_killed, enemies_left, coins_collected FROM play_data WHERE level=%d", lev );
   std::vector<std::vector<int>> matrix = {};
   mSql.execute_sql_and_return_2d_vector_int(sql, mSql.db_level_play_stats, matrix );

   // iterate
   for (auto m : matrix)
   {
      int time = m[0];
      int completed = m[1];
      int player_respawns = m[2];
      int enemies_killed = m[3];
      int enemies_left = m[4];
      int coins_collected = m[5];

      data[lev].times_played++;
      data[lev].time_total += time;

      if (time > data[lev].time_worst) data[lev].time_worst = time;


      if (completed)
      {
         data[lev].times_beat++;
         time_played_completed_only += time;

         if (data[lev].status < 2) // if locked or ready, then this is completed for the first time
         {
            data[lev].status = 2; // mark as complete and set initial values
            data[lev].time_best             = time;
            data[lev].min_respawns          = player_respawns;
            data[lev].max_enemies_killed    = enemies_killed;
            data[lev].max_coins_collected   = coins_collected;
            data[lev].min_enemies_left      = enemies_left;

            // if all coins collected, set initial all coins time record
            if (coins_collected >= data[lev].tot_coins) data[lev].time_best_all_coins = time;
         }
         else // check for new best values
         {
            if (time              < data[lev].time_best)              data[lev].time_best              = time;
            if (player_respawns   < data[lev].min_respawns)           data[lev].min_respawns           = player_respawns;
            if (enemies_killed    > data[lev].max_enemies_killed)     data[lev].max_enemies_killed     = enemies_killed;
            if (coins_collected   > data[lev].max_coins_collected)    data[lev].max_coins_collected    = coins_collected;
            if (enemies_left      < data[lev].min_enemies_left)       data[lev].min_enemies_left       = enemies_left;

            // if all coins collected, check for new best all coins time record
            if (coins_collected >= data[lev].tot_coins)
               if ((data[lev].time_best_all_coins == 0) || (time < data[lev].time_best_all_coins)) // no previous record or better than previous record
                  data[lev].time_best_all_coins = time;
         }
      }
      else // non-completed record
      {
         data[lev].times_quit++;
         // only these can be updated by an incomplete level
         if (enemies_killed  > data[lev].max_enemies_killed)   data[lev].max_enemies_killed  = enemies_killed;
         if (coins_collected > data[lev].max_coins_collected)  data[lev].max_coins_collected = coins_collected;
      }
   }

   if (data[lev].times_beat) data[lev].time_average = time_played_completed_only / data[lev].times_beat; // average time (only count completed records)
   if ((data[lev].time_best_all_coins > 0) && (data[lev].time_best_all_coins < data[lev].time_par)) data[lev].status = 3; // perfect
}





void mwLevel::check_achievements(void)
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
                      block = 1026 | PM_BTILE_ALL_SOLID; // thru
         if (x == x1) block = 1025 | PM_BTILE_ALL_SOLID;
         if (x == x2) block = 1027 | PM_BTILE_ALL_SOLID;
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
                      block = 1029 | PM_BTILE_ALL_SOLID; // thru
         if (y == y1) block = 1028 | PM_BTILE_ALL_SOLID;
         if (y == y2) block = 1030 | PM_BTILE_ALL_SOLID;
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
   if (msg_id != -1) mItem.clear_item(msg_id); // erase msg if it exists

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
         pct += data[l].tot_coins;
         pcc += data[l].max_coins_collected;
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
   else if (msg_id != -1) mItem.clear_item(msg_id); // erase msg if it exists



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
void mwLevel::fill_area_array()
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



   faa_helper(1000, 2000, 1160, 1700, 1, aai, 11); // area 11 (multiplayer bomb toss levels)
   faa_helper(0,    2000, 1840, 2000, 1, aai, 12); // area 12 (advanced info levels)
//   for(int i=0; i<aai; i++) printf("area:%d level:%d\n", area_array[i][1], area_array[i][0]);
}


void mwLevel::unlock_all_level_in_area(int area)
{
   for(int i=0; i<100; i++)
      if (area_array[i][1] == area)
         if (data[area_array[i][0]].status < 1) data[area_array[i][0]].status = 1;
}


void mwLevel::data_helper(int i, int time_par, const char *name)
{
   strcpy(data[i].level_name, name);
   data[i].time_par = time_par; // 4:00 demo 3:15


   // char sql[500];
   // sprintf(sql, "INSERT INTO level_play_data (level, level_name, time_par) VALUES (%d, '%s', %d)", i, name, time_par);
   // mSql.execute_sql(sql, mSql.db_level_play_stats);



}

void mwLevel::set_level_names_and_par_time()
{
   data_helper(1,   0,     "Overworld Map");
   data_helper(2,   4800,  "Switch Nest");         // 2:00 demo 1:35
   data_helper(3,   1400,  "Blue Key Fall");       // 0:35 demo 0:27.9
   data_helper(4,   7200,  "Blue Key Fall");       // 3:00 demo 2:20
   data_helper(5,   6400,  "Kill Kill Kill");      // 2:40 demo 2:18
   data_helper(6,   9600,  "Breakable");           // 4:00 demo 2:58
   data_helper(7,   2400,  "Ranger Bob");          // 1:00 demo 0:46.5
   data_helper(8,   8400,  "Switch Pit");          // 3:30 demo 2:52
   data_helper(10,  9600,  "The Dead Zone");       // 4:00 demo 3:15
   data_helper(11,  14400, "Good and Evil");       // 6:00 demo 4:45
   data_helper(12,  16800, "Bucket of Bad");       // 7:00 demo 6:10
   data_helper(13,  14400, "Falling Arrows");      // 6:00 demo 4:41
   data_helper(14,  600,   "Block Puzzle");        // 0:15 demo 0:08.8
   data_helper(15,  1800,  "Smash");               // 0:45 demo 0:34.1
   data_helper(16,  14400, "Big Apple");           // 6:00 demo 4:01
   data_helper(17,  16800, "Little Rocket");       // 7:00 demo 5:08
   data_helper(18,  12000, "Incinerator");         // 5:00 demo 4:04
   data_helper(19,  16800, "Crazy Fox");           // 7:00 demo 5.24
   data_helper(20,  7200,  "Strange Things");      // 3:00 demo 2:22
   data_helper(21,  4200,  "Long One");            // 1:45 demo 1:25
   data_helper(22,  4800,  "Rocket Stew");         // 2:00 demo 1:28
   data_helper(23,  19200, "Amazing");             // 8:00 demo 5:58
   data_helper(24,  7200,  "Wendy");               // 3:00 demo 2:26
   data_helper(26,  4800,  "Long Fall");           // 2:00 demo 1:06
   data_helper(27,  4000,  "Gauntlet");            // 1:40 demo 1:38
   data_helper(28,  3600,  "Nowhere to Stand");    // 1:30 demo 0:49.2
   data_helper(29,  13200, "Brain Trust");         // 5:30 demo 3:58
   data_helper(30,  7200,  "Coral Cave");          // 3:00 demo 2:30
   data_helper(31,  19200, "Beginnings");          // 8:00 demo 6:03
   data_helper(32,  9600,  "Yellow Cake");         // 4:00 demo 3:19
   data_helper(33,  6000,  "Kates Gate");          // 2:30 demo 2:06
   data_helper(34,  3600,  "Crush");               // 1:30 demo 1:04
   data_helper(35,  12000, "Bomb Toss");           // 5:00 demo 3:36
   data_helper(36,  4800,  "Spike Cave");          // 2:00 demo 1:38
   data_helper(38,  3200,  "Come Over");           // 1:20 demo 1:06
   data_helper(39,  3200,  "Bomb Run");            // 1:20 demo 0:55.7
   data_helper(40,  4200,  "Edward");              // 1:45 demo 1:05
   data_helper(41,  9600,  "Ender");               // 4:00 demo 3:27
   data_helper(42,  16800, "Cloner Chain");        // 7:00 demo 6:14
   data_helper(43,  4800,  "Cathedral");           // 2:00 demo 1:45
   data_helper(44,  9600,  "Too Many Lifts");      // 4:00 demo 2:59
   data_helper(45,  1800,  "Chimney of Death");    // 0:45 demo 0:28.6
   data_helper(46,  2400,  "Bucket List");         // 1:00 demo 0:38.8
   data_helper(47,  2400,  "Begin Again");         // 1:00 demo 0:27.1
   data_helper(48,  2400,  "Mayhem");              // 1:00 demo 0:41.8
   data_helper(49,  9600,  "Rocket Square");       // 4:00 demo 2:37
   data_helper(50,  1600,  "Welcome");             // 0:40 demo 0:30.2
   data_helper(51,  6000,  "Cannon Bees");         // 2:30 demo 2:21
   data_helper(52,  3600,  "Boules");              // 1:30 demo 1:07
   data_helper(53,  8400,  "Lifts With Names");    // 3:30 demo 2:59
   data_helper(54,  2400,  "Old Training Level");  // 1:00 demo 0:31.9
   data_helper(55,  7200,  "Ancient Ruins");       // 3:00 demo 1:59
   data_helper(56,  9600,  "Zaiden");              // 4:00 demo 2:44
   data_helper(57,  7200,  "Valentine");           // 3:00 demo 2:43
   data_helper(60,  2400,  "Bomb Toss");           // 2:00 demo not done
   data_helper(61,  2400,  "Bomb Toss 2");         // 2:00 demo not done
   data_helper(64,  12000, "Escape Rocket!");      // 5:00 demo 4:38

   // ---------------------------------------
   // Training Levels
   // ---------------------------------------
   data_helper(80,  1200,  "Training Level 1");    // 0:30 demo 0:21.2
   data_helper(81,  4800,  "Training Level 2");    // 2:00 demo 0:53.4
   data_helper(82,  8400,  "Training Level 3");    // 3:30 demo 2:12
   data_helper(83,  6000,  "Training Level 4");    // 2:30 demo 1:14
   data_helper(84,  3600,  "Training Level 5");    // 1:30 demo 0:52.8
   data_helper(9,   6000,  "Bomb Intro");          // 2:30 demo 1:35
   data_helper(25,  1000,  "One Cannon");          // 0:25 demo 0:21.9
   data_helper(37,  800,   "Sacrifice");           // 0:20 demo 0:13.3


   // ---------------------------------------
   // Advanced Information Levels
   // ---------------------------------------
   data_helper(85,  4800,  "Doors");                // 2:00 demo 1:48
   data_helper(86,  3600,  "Triggers and Timers");  // 1:30 demo 1:26
   data_helper(87,  2400,  "Block Manip");          // 1:00 demo 0:28.4
   data_helper(88,  3600,  "Block Damage");         // 1:30 demo 1:24
   data_helper(89,  2400,  "Orbs");                 // 1:00 demo 0:51.8
   data_helper(90,  4800,  "Archwagons");           // 2:00 demo 1:51
   data_helper(91,  800,   "Trakbots");             // 0:20 demo 0:18.9
   data_helper(92,  3600,  "Bouncers and Cannons"); // 1:30 demo 1:11
   data_helper(93,  1800,  "Cloners");              // 0:45 demo 0:35.8
   data_helper(94,  1800,  "Vinepods");             // 0:45 demo 0:37.7
   data_helper(95,  3600,  "Blokwalks");            // 1:30 demo 0:36.4
   data_helper(96,  600,   "Flappers");             // 0:15 demo 0:03.9
   data_helper(97,  3600,  "Jumpworms");            // 1:30 demo 1:18
   data_helper(98,  3600,  "Lifts");                // 1:30 demo 1:11





}





void mwLevel::clear_data()
{
   mSql.execute_sql("DELETE FROM play_data", mSql.db_level_play_stats);


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
      data[i].max_coins_collected = 0;
      data[i].max_enemies_killed = 0;
      data[i].tot_coins = 0;
      data[i].min_enemies_left = 0;
      data[i].min_enemies_left_par = 0;
   }

   set_level_names_and_par_time();

   // unlock first training level
   data[80].status = 1;



   unlock_all_level_in_area(10);
   unlock_all_level_in_area(11);
   unlock_all_level_in_area(12);

   double t0 = al_get_time();

   // load all levels to get purple coin counts, and update status color and text
   for (int p=0; p<100; p++)
      if (mLevel.load_level(p, 1, 1))
      {
         update_level_status(p);

         data[p].tot_coins = 0;
         for (int i=0; i<500; i++)
            if ((mItem.item[i][0] == 2) && (mItem.item[i][6] == 3)) data[p].tot_coins++;

         char sql[1024];
         sprintf(sql, "UPDATE level_play_data SET tot_coins=%d WHERE level=%d ", data[p].tot_coins, p);
         mSql.execute_sql(sql, mSql.db_level_play_stats);

      }

   printf("clear load levels time:%f\n", al_get_time() - t0);

   save_data();
   level_stats_bmp_msg_type = 0;

}




void mwLevel::save_level_data_to_db()
{
   mSql.execute_sql("DELETE FROM level_data", mSql.db_level_play_stats);
   for(int i=0; i<100; i++)
   {
      char sql[2000];
      sprintf(sql, "INSERT INTO level_data (level, level_name, status, status_color, status_text, \
                                            min_respawns, max_coins_collected, tot_coins, \
                                            min_enemies_left, min_enemies_left_par, max_enemies_killed, \
                                            times_played, times_beat, times_quit, time_par, time_best, \
                                            time_best_all_coins, time_worst, time_average, time_total ) \
                                            VALUES(%d, '%s', %d, %d, '%s',    %d, %d, %d,   %d, %d, %d,    %d, %d, %d, %d, %d,   %d, %d, %d, %d)",
                                            i, data[i].level_name, data[i].status, data[i].status_color, data[i].status_text,
                                            data[i].min_respawns, data[i].max_coins_collected, data[i].tot_coins,
                                            data[i].min_enemies_left, data[i].min_enemies_left_par, data[i].max_enemies_killed,
                                            data[i].times_played, data[i].times_beat, data[i].times_quit, data[i].time_par, data[i].time_best,
                                            data[i].time_best_all_coins, data[i].time_worst, data[i].time_average, data[i].time_total );

      mSql.execute_sql(sql, mSql.db_level_play_stats);
   }
}

void mwLevel::load_level_data_from_db()
{
   // clear everything first
   for(int i=0; i<100; i++)
   {
      strcpy(data[i].level_name, "");

      data[i].status = 0;
      data[i].status_color = 0;
      strcpy(data[i].status_text, "");

      data[i].min_respawns = 0;
      data[i].max_coins_collected = 0;
      data[i].tot_coins = 0;

      data[i].min_enemies_left = 0;
      data[i].min_enemies_left_par = 0;
      data[i].max_enemies_killed = 0;

      data[i].times_played = 0;
      data[i].times_beat = 0;
      data[i].times_quit = 0;
      data[i].time_par = 0;
      data[i].time_best = 0;

      data[i].time_best_all_coins = 0;
      data[i].time_worst = 0;
      data[i].time_average = 0;
      data[i].time_total = 0;
   }


   // get all int values from database
   char sql[2000];
   sprintf(sql, "SELECT level, status, status_color, \
                        min_respawns, max_coins_collected, tot_coins, \
                        min_enemies_left, min_enemies_left_par, max_enemies_killed, \
                        times_played, times_beat, times_quit, time_par, time_best, \
                        time_best_all_coins, time_worst, time_average, time_total FROM level_data");
   std::vector<std::vector<int>> matrix = {};
   mSql.execute_sql_and_return_2d_vector_int(sql, mSql.db_level_play_stats, matrix );

   // iterate
   for (auto m : matrix)
   {
      int i = m[0];
      data[i].status = m[1];
      data[i].status_color = m[2];
      data[i].min_respawns = m[3];
      data[i].max_coins_collected = m[4];
      data[i].tot_coins = m[5];
      data[i].min_enemies_left = m[6];
      data[i].min_enemies_left_par = m[7];
      data[i].max_enemies_killed = m[8];
      data[i].times_played = m[9];
      data[i].times_beat = m[10];
      data[i].times_quit = m[11];
      data[i].time_par = m[12];
      data[i].time_best = m[13];
      data[i].time_best_all_coins = m[14];
      data[i].time_worst = m[15];
      data[i].time_average = m[16];
      data[i].time_total = m[17];
   }

   // get level names and par time
   set_level_names_and_par_time();

   // set status text and color
   for(int i=0; i<100; i++)
      set_status_text_and_color(i);
}


void mwLevel::load_data()
{
   load_level_data_from_db();
   FILE *fp =fopen("data/level_data.pm","rb");
   if (fp)
   {
      fread(area_locks,     sizeof(area_locks),    1, fp);
      fread(area_array,     sizeof(area_array),    1, fp);
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

void mwLevel::save_data()
{
   save_level_data_to_db();
   FILE *fp =fopen("data/level_data.pm","wb");
   if (fp)
   {
      fwrite(area_locks,     sizeof(area_locks),    1, fp);
      fwrite(area_array,     sizeof(area_array),    1, fp);
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

