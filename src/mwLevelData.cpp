// mwLevelData.cpp

#include "pm.h"
#include "mwLevel.h"
#include "mwConfig.h"
#include "mwItem.h"
#include "mwInput.h"
#include "mwMiscFnx.h"
#include "mwPlayer.h"
#include "mwDemoMode.h"
#include "mwEditSelection.h"
#include "mwGmInfo.h"
#include "mwLevelIcons.h"
#include "mwLoop.h"
#include "mwMain.h"
#include "mwNetgame.h"
#include "mwSql.h"
#include "mwTriggerEvent.h"
#include "pm.h"
#include "pm.h"
#include "pm.h"
#include "pm.h"
#include "pm.h"

void mwLevel::reset_level_data()
{
   clear_data();
   save_data();
   level_stats_bmp_msg_type = 0;                  // to force recreation
   load_level_by_number(mLevel.play_level, 0, 0); // reload play level
}

void mwLevel::unlock_all_levels_and_areas()
{
   // only change levels that were locked to start with
   for(int i=0; i<100; i++)
      if (data[i].status < 1) data[i].status = 1;

   // make all areas visible
   for(int i=1; i<8; i++)
      overworld_area_array[i].visible = 1;

   save_data();
   level_stats_bmp_msg_type = 0;                  // to force recreation
   load_level_by_number(mLevel.play_level, 0, 0); // reload play level
}



void mwLevel::add_play_data_record(int lev, int type)
{
   //printf("void mwLevel::add_play_data_record(lev:%d, type:%d))\n", lev, type);

   if (mNetgame.ima_client) return; // clients do not save locally

   int save_flag = 1;
   if (mPlayer.syn[mPlayer.active_local_player].control_method == PM_PLAYER_CONTROL_METHOD_DEMO_MODE) save_flag = 0; // if running demo mode, don't save data
   if (mLevel.skc_trigger_demo_cheat) save_flag = 1;                        // in cheat mode save data
   if (mDemoMode.demo_debug_running_demo_saves_level_data) save_flag = 1;   // or if this is option is set
   if (lev == 1) save_flag = 0;                                             // never save data for overworld
   if (save_flag)
   {
      mLevel.skc_trigger_demo_cheat = 0;
      if (type == 2)  // debug - mark level complete with FIRE on gate
      {
         char sql[2000];
         sprintf(sql, "INSERT INTO play_data (level, time, completed, num_players, player_respawns, enemies_killed, enemies_left, coins_collected) VALUES(%d, %d, %d, %d, %d, %d, %d, %d);", lev, data[lev].time_par + 1200,  1, 0, 0,0,0,0);
         mSql.execute_sql(sql, mSql.db_play_data);
      }
      else
      {
         mGmInfo.calc();
         mGmInfo.add();
      }
      mLevel.calc_level_stats(mLevel.play_level);
      check_achievements();
      save_data();
      level_stats_bmp_msg_type = 0;
   }
}

// iterates all play_data for level and sets these variables
void mwLevel::calc_level_stats(int lev)
{

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
   mSql.execute_sql_and_return_2d_vector_int(sql, mSql.db_play_data, matrix );

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


void mwLevel::check_achievements()
{
   // check for completed areas


   // area 1 - training
   // if more than 2 levels done
   int done = 0;
   for (int l=0; l<99; l++)
      if ((data[l].area == 1) && (data[l].status > 1)) done++;

   if (done > 0) overworld_area_array[2].visible = 1; // make next area visible



   // main areas (2-5)
   for (int a=2; a<6; a++)
   {
      //int tot = overworld_area_array[a].num_levels_tot;

      int tot = 0;


      done = 0;
      for (int l=0; l<99; l++)
         if (data[l].area == a)
         {
            tot++;
            if (data[l].status > 1) done++;
         }

      float pd = 100 * (float)done / (float)tot;

      if (pd > 1) // greater than 1%
      {
         overworld_area_array[a+1].visible = 1; // make next area visible
      }
   }



}







/*
creates a message with internal scroll trigger
scroll x,y

msg x,y,w,h
msg frame color
msg flags
message text
*/

void mwLevel::create_msg_helper(int sx, int sy, int mx, int my, int mw, int mh, int mcol, int mflags, const char *msg )
{
   // create message
   int i = mItem.get_empty_item(10);
   mItem.item[i][2] = mflags;
   mItem.item[i][4] = sx;
   mItem.item[i][5] = sy;
   mItem.item[i][6] = mx;
   mItem.item[i][7] = my;
   mItem.item[i][8] = mw;
   mItem.item[i][9] = mh;
   mItem.item[i][12] = 2; // display time
   mMiscFnx.set_int_3216(mItem.item[i][13], 15, mcol); // set text and frame colors
//   strcpy(mItem.pmsgtext[i], msg);

   mItem.mMessageText.set_text(i, msg);



}



/*
creates a message and trigger item set
trigger x,y,w,h
tcol - trigger color (0 is no draw)
msg x,y,w,h
msg frame color
msg flags
message text
*/


void mwLevel::create_msg_helper_trigger(int tx, int ty, int tw, int th, int tcol, int mx, int my, int mw, int mh, int mcol, int mflags, const char *msg )
{
   // get an unused event
   int ev = mTriggerEvent.get_unused_pm_event();

   // create trigger
   int i = mItem.get_empty_item(9);
   mItem.item[i][3] |= PM_ITEM_TRIGGER_PLAYER;

   if (tcol)
   {
      mItem.item[i][3] |= PM_ITEM_TRIGGER_DRAW_ON;
      mItem.item[i][2] = tcol;
   }

   mItem.item[i][4] = tx-20;
   mItem.item[i][5] = ty;
   mItem.item[i][6] = tx;
   mItem.item[i][7] = ty;
   mItem.item[i][8] = tw;
   mItem.item[i][9] = th;
   mItem.item[i][11] = ev; // constant send when triggered

   // create message
   i = mItem.get_empty_item(10);
   mItem.item[i][1] = ev;
   mItem.item[i][2] = mflags;
   mItem.item[i][4] = mx-20;
   mItem.item[i][5] = my;
   mItem.item[i][6] = mx;
   mItem.item[i][7] = my;
   mItem.item[i][8] = mw;
   mItem.item[i][9] = mh;

   mItem.item[i][12] = 2; // display time
   mMiscFnx.set_int_3216(mItem.item[i][13], 15, mcol); // set text and frame colors

//   strcpy(mItem.pmsgtext[i], msg);

   mItem.mMessageText.set_text(i, msg);



}



void mwLevel::create_msg_helper(int area)
{
   int col = overworld_area_array[area].color;

   int ax1 = overworld_area_array[area].x;
   int aw = overworld_area_array[area].w;
   //int ax2 = ax1 + aw;
   int axc = ax1 + aw/2;

   int ay1 = overworld_area_array[area].y;
   int ah = overworld_area_array[area].h;
   int ay2 = ay1 + ah;
   //int ayc = ay1 + ah/2;




   // int ss_flags = 0;
   // ss_flags |= PM_BTILE_SOLID_PLAYER;
   // ss_flags |= PM_BTILE_SOLID_ENEMY;
   // ss_flags |= PM_BTILE_SOLID_ITEM;
   // ss_flags |= PM_BTILE_SEMISOLID_PLAYER;
   // ss_flags |= PM_BTILE_SEMISOLID_ENEMY;
   // ss_flags |= PM_BTILE_SEMISOLID_ITEM;

   // add semisolid platform of blocks below
   // int by = y/20+3;
   // for (int bx=x/20; bx<x/20+6; bx++) mLevel.l[bx][by] = 899 + ss_flags;
   // mLevel.l[x/20][by] = 898 + ss_flags;
   // mLevel.l[x/20+5][by] = 900 + ss_flags;

   // int bx = x/20+2;
   // int by = y/20+3;
   // mLevel.l[bx][by] = 910 + ss_flags;
   // mLevel.l[bx][by+3] = 910 + ss_flags;


   // create always on message with area name
   // ----------------------------------------------
   int msg_name_x = (axc-9)*20 + 3;
   int msg_name_y = ay1     *20 + 2;
   if (area == 1) msg_name_y+=130;
   create_msg_helper(msg_name_x-20, msg_name_y, msg_name_x, msg_name_y, 95, 23, col, PM_ITEM_PMSG_FRAME4 + PM_ITEM_PMSG_SHOW_ALWAYS, overworld_area_array[area].name);



   // create trigger and message for area stats
   // ----------------------------------------------
   // msg area stats scroll
   int msg_stats_scroll_x = msg_name_x+40;
   int msg_stats_scroll_y = msg_name_y+60;

   // msg area stats msg
   int msg_stats_msg_x = 1204;
   int msg_stats_msg_y = msg_name_y;
   if (area == 1) msg_stats_scroll_y-=10;
   char msg[80];
   sprintf(msg, "Level Statistics Area:%d", area);

   // scroll
   create_msg_helper(msg_stats_scroll_x, msg_stats_scroll_y, msg_stats_msg_x, msg_stats_msg_y, 0, 0, col, PM_ITEM_PMSG_FRAME0 + PM_ITEM_PMSG_SHOW_SCROLL, msg);

   // visible trigger
   // create_msg_helper_trigger(x, y+40, 20, 20, col, 1000, y+70, 0, 0, col, PM_ITEM_PMSG_FRAME0, msg);

   // hidden trigger with tile
   //create_msg_helper_trigger(x, y+40, 20, 20, 0, 1000, y+70, 0, 0, col, PM_ITEM_PMSG_FRAME0, msg);
   //mLevel.l[x/20][(y+40)/20] = 134;



   // create trigger and message for what needs to be completed to continue
   // ---------------------------------------------------------------------
   int msg_unlock_x = (axc-4) * 20;
   int msg_unlock_w = 160;
   int msg_unlock_y = ay2 * 20;
   int msg_unlock_h =  40;


   if (area == 1)//  && overworld_area_array[2].visible == 0)
   {
      create_msg_helper_trigger(msg_unlock_x, msg_unlock_y-40, msg_unlock_w, 20, 0, msg_unlock_x, msg_unlock_y+4, msg_unlock_w, msg_unlock_h, col, PM_ITEM_PMSG_FRAME4, "Complete two\nlevels to unlock");
   }

   if (area == 2 && overworld_area_array[3].visible == 0)
   {
      create_msg_helper_trigger(msg_unlock_x, msg_unlock_y-40, msg_unlock_w, 20, 0, msg_unlock_x, msg_unlock_y+4, msg_unlock_w, msg_unlock_h, col, PM_ITEM_PMSG_FRAME4, "Complete three\nlevels to unlock");
   }



}


// creates an invisible door set from x1,y1 to x2,x2
// tile is usually an arrow pointing in the direction of the move
// enter_type can be (0-immediate, 1 = up, 2 = down)
void mwLevel::create_door_helper(int x1, int y1, int x2, int y2, int enter_type, int tile)
{
   // add tile
   mLevel.l[x1][y1] = tile;

   // create 2 door items
   int i0 = mItem.get_empty_item(1);
   int i1 = mItem.get_empty_item(1);

   // main door
   //mItem.item[i0][1] = 0; // hidden
   mItem.item[i0][4] = x1*20; // x
   mItem.item[i0][5] = y1*20; // y
   mItem.item[i0][8] = 1;  // type - linked dest
   mItem.item[i0][9] = i1; // linked dest item
   mItem.item[i0][11] = enter_type; // door entry type

   // exit
   //mItem.item[i1][1]  = 0; // hidden
   mItem.item[i1][4]  = x2*20; // x
   mItem.item[i1][5]  = y2*20; // y
   //mItem.item[i1][8]  = 0; // type - exit only
}




// only ever called from load level after loading level 1
void mwLevel::set_overworld_barriers()
{
   // printf("void mwLevel::set_overworld_barriers()\n");

   //   double t = al_get_time();

   // debug all areas visible
   // overworld_area_array[2].visible = 1;
   // overworld_area_array[3].visible = 1;
   // overworld_area_array[4].visible = 1;
   // overworld_area_array[5].visible = 1;
   // overworld_area_array[6].visible = 1;
   // overworld_area_array[7].visible = 1;

   // // debug all areas visible
   // overworld_area_array[1].visible = 1;
   // overworld_area_array[2].visible = 0;
   // overworld_area_array[3].visible = 0;
   // overworld_area_array[4].visible = 0;
   // overworld_area_array[5].visible = 0;
   // overworld_area_array[6].visible = 0;
   // overworld_area_array[7].visible = 0;

   // always copy area 1 - clear all first
   int i = 1;
   int x = overworld_area_array[i].x;
   int y = overworld_area_array[i].y;
   int w = overworld_area_array[i].w;
   int h = overworld_area_array[i].h;


   mEditSelection.copy_sel_from_level(70, x, y, w, h, 1, x, y, 2);

   // copy other areas if visible
   for (i=2; i<8; i++)
   {
      x = overworld_area_array[i].x;
      y = overworld_area_array[i].y;
      w = overworld_area_array[i].w;
      h = overworld_area_array[i].h;
      if (overworld_area_array[i].visible) mEditSelection.copy_sel_from_level(70, x, y, w, h, 1, x, y, 1);
   }

   // modify visible areas
   for (i=1; i<8; i++)
      if (overworld_area_array[i].visible)
      {
         // create msg
         create_msg_helper(i);

         // add side to side doors
         if (i>0 && i<6)
         {
            y = overworld_area_array[i].y + overworld_area_array[i].h - 2;
            if (i==5) y-=4; // this area has a different height
            create_door_helper(0,  y,  97, y, 0, 11);
            create_door_helper(99, y,   2, y, 0, 12);
         }


         // remove blocks from bottom of area above and create doors from area above
         if (i>1 && i<6) // only for areas 2 - 5
         {
            // top y position of this area
            y = overworld_area_array[i].y;

            // remove blocks from bottom of area above
            for (x=46; x<54; x++) mLevel.l[x][y-1] = 0;

            // add doors from area above
            create_door_helper(1,  y-2, 1,  y+2, 2, 15);
            create_door_helper(98, y-2, 98, y+2, 2, 15);
         }
   }

   save_level(1); // save overworld level

   if (!mMain.headless_server) mLevelIcons.update_one(1, 1); //   update_one




   // printf("void mwLevel::set_overworld_barriers() -- complete\n");



//   printf("time to set overworld barriers: %f\n", al_get_time() - t);


//   mEditSelection.add_sel_to_level(1, "sel/test123.sel", 10, 2);

//   mEditSelection.copy_sel_from_level(70, 45, 48, 10, 10, 1, 70, 50, 2);

//   mEditSelection.copy_sel_from_level(70, 0, 0, 100, 100, 1, 0, 0, 2);

}


// fills the area array (used only to control the order of levels in each area for stat listings)
void mwLevel::fill_area_array(int area, int xasc, int &aai)
{
   int x1 = overworld_area_array[area].x * 20;
   int y1 = overworld_area_array[area].y * 20;
   int x2 = x1 + overworld_area_array[area].w * 20;
   int y2 = y1 + overworld_area_array[area].h * 20;

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



void mwLevel::overworld_area_array_add(int area, const char* name, int color, int visible, int x, int y, int w, int h)
{
   sprintf(overworld_area_array[area].name, "%s", name);
   overworld_area_array[area].color = color;
   overworld_area_array[area].visible = visible;
   overworld_area_array[area].x = x;
   overworld_area_array[area].y = y;
   overworld_area_array[area].w = w;
   overworld_area_array[area].h = h;

   // find all gates in the area
   for (int i=0; i<100; i++)
      if ((mItem.item[i][0] == 18) && (mItem.item[i][4] >= x*20) && (mItem.item[i][4] < (x+w)*20) && (mItem.item[i][5] >= y*20) && (mItem.item[i][5] <= (y+h)*20))
      {
         int lev = mItem.item[i][6];
         data[lev].area = area;
      }
}


// adds areas to overworld_area_array
// searches the base overworld level for gates in areas
void mwLevel::fill_overworld_area_array()
{
   // load base overworld level
   load_level_by_number(70, 1, 0);

   overworld_area_array_add(1, "Surface",     11, 1, 0, 0,  100, 14);
   overworld_area_array_add(2, "Outskirts",   13, 0, 0, 14, 100, 10);
   overworld_area_array_add(3, "Suburbs",      6, 0, 0, 24, 100, 10);
   overworld_area_array_add(4, "Downtown",     7, 0, 0, 34, 100, 10);
   overworld_area_array_add(5, "Dungeon",      8, 0, 0, 44, 100, 14);
   overworld_area_array_add(6, "Extra",        9, 0, 0, 74, 100, 6);
   overworld_area_array_add(7, "Dev Demo",    10, 0, 0, 85, 100, 15);

   // print all levels and areas they belong to
   //for (int i=0; i<100; i++) printf("level:%d - area:%d\n", i, data[i].area);

   // fills the area array (used only to control the order of levels in each area for stat listings)
   int aai = 0;
   for (int area=1; area<8; area++) fill_area_array(area, 1, aai);
}


void mwLevel::unlock_all_level_in_area(int area)
{
   for (int i=0; i<100; i++)
      if (data[i].area == area)
         if (data[i].status < 1) data[i].status = 1;
}

void mwLevel::data_helper(int i, int time_par, const char *name)
{
   strcpy(data[i].level_name, name);
   data[i].time_par = time_par; // 4:00 demo 3:15
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
   mSql.execute_sql("DELETE FROM play_data", mSql.db_play_data);

   mPlayer.syn[0].overworld_last_touched_gate = 0; // no gate will be found and player will start from start block
   mConfig.save_config(PM_CFG_SAVE_OVERWORLD_GATE_LAST_TOUCHED);


   for (int i=0; i<100; i++)
   {
      area_array[i][0] = 0;
      area_array[i][1] = 0;
   }

   for (int i=0; i<20; i++)
   {
      strcpy(overworld_area_array[i].name, "");
      overworld_area_array[i].color = 0;
      overworld_area_array[i].visible = 0;
      overworld_area_array[i].x = 0;
      overworld_area_array[i].y = 0;
      overworld_area_array[i].w = 0;
      overworld_area_array[i].h = 0;
   }

   for (int i=0; i<100; i++)
   {
      strcpy(data[i].level_name, "");
      data[i].status = 1;
      data[i].area = 0;
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
   fill_overworld_area_array();

   // load all levels to get purple coin counts
   for (int lv=0; lv<100; lv++)
      if (mLevel.load_level_by_number(lv, 1, 1))
      {
         for (int i=0; i<500; i++)
            if ((mItem.item[i][0] == 2) && (mItem.item[i][6] == 1)) data[lv].tot_coins++;
      }

   load_level_by_number(1, 1, 0); // load overworld level to force initial setup

   save_data();
   level_stats_bmp_msg_type = 0;
}

void mwLevel::load_data()
{
   FILE *fp =fopen("data/level_data.pm","rb");
   if (fp)
   {
      fread(data,                  sizeof(data),                   1, fp);
      fread(overworld_area_array,  sizeof(overworld_area_array),   1, fp);
      fread(area_array,            sizeof(area_array),             1, fp);
      fclose(fp);
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
   //save_level_data_to_db();
   FILE *fp =fopen("data/level_data.pm","wb");
   if (fp)
   {
      fwrite(data,                  sizeof(data),                   1, fp);
      fwrite(overworld_area_array,  sizeof(overworld_area_array),   1, fp);
      fwrite(area_array,            sizeof(area_array),             1, fp);
      fclose(fp);
      return;
   }
   mInput.m_err("Error saving level_data.pm");
}


int mwLevel::get_total_purple_coins(int level)
{
   if (level > 1 && level < 100) return data[mLevel.play_level].tot_coins;
   return 0;
}



void mwLevel::dump_level_data()
{
//   printf("Play Data:\n");
//   for (int i=0; i<play_data_num; i++)
//      printf("i:%d lev:%d tmr:%d cmp:%d ek:%d\n", i, play_data[i].level, play_data[i].timer, play_data[i].completed, play_data[i].enemies_killed);
//
//   printf("\nLevel Data:\n");
//   for(int i=80; i<85; i++)
//      printf("i:%d t_b:%d ek:%d st:%d stc:%d sts:%s \n", i, data[i].time_best, data[i].max_enemies_killed, data[i].status, data[i].status_color, data[i].status_text);
}

