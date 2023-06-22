// mwLevel.cpp

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


mwLevel mLevel;

void mwLevel::set_start_level(void)
{
   set_start_level(start_level);
}

void mwLevel::set_start_level(int s)
{
   if (s < 1  ) s = 1;
   if (s > 399) s = 399;
   play_level = start_level = s;
   load_level(s, 0, 0);
   resume_allowed = 0;
   mConfig.save();
}

int level_exists(int level)
{
   char fn[256];
   sprintf(fn, "levels/level%03d.pml", level);
   if (al_filename_exists(fn)) return 1;
   return 0;
}

void mwLevel::next_level(void)
{
   int done = 0;
   while (!done)
   {
      if (level_exists(++start_level)) done = 1;
      if (start_level > 199)
      {
         prev_level();
         done = 1;
      }
   }
   set_start_level(start_level);
}

int mwLevel::get_next_level(int lev)
{
   int done = 0;
   while (!done)
   {
      if (level_exists(++lev)) done = 1;
      if (lev > 199)
      {
         lev = 1;
         done = 1;
      }
   }
   return lev;
}




void mwLevel::prev_level(void)
{
   int done = 0;
   while (!done)
   {
      if (level_exists(--start_level)) done = 1;
      if (start_level < 1)
      {
         start_level = 1;
         done = 1;
      }
   }
   set_start_level(start_level);
}


void mwLevel::change_block(int x, int y, int block)
{
   l[x][y] = block;
   al_set_target_bitmap(mBitmap.level_background);
   al_draw_filled_rectangle(x*20, y*20, x*20+20, y*20+20, mColor.pc[0]);
   al_draw_bitmap(mBitmap.btile[block & 1023], x*20, y*20, 0);
}

int mwLevel::is_block_empty(int x, int y, int test_block, int test_item, int test_enemy)
{
   int mpty = 1; // default is empty

   if (test_block) if (mLevel.l[x][y] > 31) mpty = 0;

   if (test_enemy)
      for (int c=0; c<100; c++)
         if (mEnemy.Ei[c][0])
            if ((mEnemy.Ef[c][0] == x*20) && (mEnemy.Ef[c][1] == y*20)) mpty = 0;

   if (test_item)
      for (int c=0; c<500; c++)
         if (mItem.item[c][0])
            if ((mItem.item[c][4] == (x*20)) && (mItem.item[c][5] == (y*20))) mpty = 0;

   return mpty;
}








#define PML_SIZE 384080

void pml_to_var(char * b) // for load level
{
   int sz = 0, offset = 0;
   sz = sizeof(mLevel.l);       memcpy(mLevel.l,       b+offset, sz); offset += sz;
   sz = sizeof(mItem.item);     memcpy(mItem.item,     b+offset, sz); offset += sz;
   sz = sizeof(mEnemy.Ei);      memcpy(mEnemy.Ei,      b+offset, sz); offset += sz;
   sz = sizeof(mEnemy.Ef);      memcpy(mEnemy.Ef,      b+offset, sz); offset += sz;
   sz = sizeof(mLift.cur);      memcpy(mLift.cur,      b+offset, sz); offset += sz;
   sz = sizeof(mLift.stp);      memcpy(mLift.stp,      b+offset, sz); offset += sz;
   sz = sizeof(mItem.pmsgtext); memcpy(mItem.pmsgtext, b+offset, sz); offset += sz;
}

void var_to_pml(char * b) // for save level
{
   int sz = 0, offset = 0;
   offset += sz; sz = sizeof(mLevel.l);       memcpy(b+offset, mLevel.l,       sz);
   offset += sz; sz = sizeof(mItem.item);     memcpy(b+offset, mItem.item,     sz);
   offset += sz; sz = sizeof(mEnemy.Ei);      memcpy(b+offset, mEnemy.Ei,      sz);
   offset += sz; sz = sizeof(mEnemy.Ef);      memcpy(b+offset, mEnemy.Ef,      sz);
   offset += sz; sz = sizeof(mLift.cur);      memcpy(b+offset, mLift.cur,      sz);
   offset += sz; sz = sizeof(mLift.stp);      memcpy(b+offset, mLift.stp,      sz);
   offset += sz; sz = sizeof(mItem.pmsgtext); memcpy(b+offset, mItem.pmsgtext, sz);
}



int mwLevel::load_level(int level_num, int load_only, int fail_silently)
{
   char msg[1024];
   zero_level_data();
   last_level_loaded = level_num;
   valid_level_loaded = 0;
   resume_allowed = 0;

   char lf[255];
   sprintf(lf, "levels/level%03d.pml", level_num);
   //printf("loading: %s\n", lf);

   FILE *fp = fopen(lf, "rb");
   if (!fp)
   {
      sprintf(msg, "Error opening %s", lf);
      if (!fail_silently) mInput.m_err(msg);
      return 0;
   }
   else  // file open !
   {
      // read the compressed data
      char cmp[PML_SIZE];
      fread(cmp, sizeof(cmp), 1, fp);
      fclose(fp);

      // decompress cmp to pml
      char pml[PML_SIZE];
      uLongf destLen = sizeof(pml);
      uncompress((Bytef*)pml, (uLongf*)&destLen, (Bytef*)cmp, sizeof(cmp));

      // copy to game variables
      pml_to_var(pml);

      if (level_num == 1) set_overworld_barriers();

      if (!load_only)
      {
         mLevel.valid_level_loaded = 1;
         mLift.lift_setup();
         for (int x=0; x<500; x++)
            if (mItem.item[x][0]) // only if active set x y
            {
               mItem.itemf[x][0] = mItem.item[x][4];
               mItem.itemf[x][1] = mItem.item[x][5];
            }
         level_check();
         level_start_data();
         mScreen.init_level_background(); // draw blocks on level_background
         //set_player_start_pos(0, 0);
      }

      return 1;
   }
}

void mwLevel::save_level(int level_num)
{
   last_level_loaded = level_num;
   level_check();

   mItem.sort_item(1);
   mEnemy.sort_enemy();
   mLift.lift_setup();

   char lf[255];
   sprintf(lf, "levels/level%03d.pml", level_num);
   //printf("saving: %s\n", lf);


   // put variables in pml
   char pml[PML_SIZE];
   var_to_pml(pml);

   // compress pml to cmp
   char cmp[PML_SIZE];
   uLongf destLen= sizeof(cmp);
   compress2((Bytef*)cmp, (uLongf*)&destLen, (Bytef*)pml, sizeof(pml), -1);
   int cmp_size = destLen;

   // write cmp to file
   FILE *fp = fopen(lf,"wb");
   fwrite(cmp, cmp_size, 1, fp);
   fclose(fp);
}

int mwLevel::load_level_prompt(void)
{
   char lf[256];
   sprintf(lf,"levels\\");
   if (mMiscFnx.mw_file_select("Load Selection", lf, ".pml", 0))
   {
      int len = strlen(lf);
      char g[10];
      g[0] = lf[len-7];
      g[1] = lf[len-6];
      g[2] = lf[len-5];
      g[3] = 0;
      int num = atoi(g);
      load_level(num, 0, 0);
      return 1;
   }
   return 0; // user pressed cancel
}

int mwLevel::save_level_prompt(void)
{
   int num = last_level_loaded;

   char title[80];
   sprintf(title,"Save Level %d ",num);

   char lf[256];
   sprintf(lf,"levels\\level%03d.pml", num);
   if (mMiscFnx.mw_file_select(title, lf, ".pml", 1))
   {
      int len = strlen(lf);
      char g[10];
      g[0] = lf[len-7];
      g[1] = lf[len-6];
      g[2] = lf[len-5];
      g[3] = 0;
      num = atoi(g);
      save_level(num);
      return 1;
   }
   return 0; // user pressed cancel
}


int mwLevel::show_level_data(int x_pos, int y_pos, int type)
{
   if (type == 1) al_clear_to_color(al_map_rgb(0,0,0));

   int ey_pos = mEnemy.show_enemy_data(x_pos, y_pos);
   int iy_pos = mItem.item_data(x_pos+135, y_pos);
   ey_pos = ey_pos + 8;
   al_draw_textf(mFont.pr8, mColor.pc[15], x_pos, ey_pos, 0,"%d Lifts  ", mLift.get_num_lifts());
   ey_pos += 8;
   al_draw_text(mFont.pr8, mColor.pc[15], x_pos, ey_pos, 0, "-------");
   ey_pos += 8;

   if (type == 1)
   {
      al_flip_display();
      mInput.tsw();
   }

   if (iy_pos > ey_pos) return iy_pos;
   else return ey_pos;
}

void mwLevel::zero_level_data(void)
{
   for (int c=0; c<100; c++)    // blocks
      for (int x=0; x<100; x++)
           l[c][x] = 0;

   for (int c=0; c<500; c++)  // items
   {
      for (int x=0; x<500; x++) mItem.pmsgtext[c][x] = 0;
      for (int x=0; x<16; x++)  mItem.item[c][x] = 0;
      for (int x=0; x<4; x++)   mItem.itemf[c][x] = 0;
   }
   mItem.sort_item(1);

   for (int c=0; c<100; c++)
   {
      for (int x=0; x<16; x++)  mEnemy.Ef[c][x] = 0; // enemy floats
      for (int x=0; x<32; x++)  mEnemy.Ei[c][x] = 0; // enemy ints
   }
   mEnemy.sort_enemy();

   for (int c=0; c<40; c++) // lifts
   {
      mLift.clear_lift(c);
      for (int x=0; x<40; x++)
         mLift.clear_lift_step(c,x);
   }
}

void mwLevel::level_check(void)
{
   int lev = last_level_loaded;

   // set number of starts - used for drawing, calculating spawn points, etc
   number_of_starts = 0;

   // number of purple coins
   data[lev].tot_purple_coins = 0;

   // min val for exit enemy lock
   data[lev].min_enemies_left_par = 100;


   for (int i=0; i<500; i++)
   {
      if (mItem.item[i][0] == 5) number_of_starts++;
      if ((mItem.item[i][0] == 2) && (mItem.item[i][6] == 3)) data[lev].tot_purple_coins++;
      if ((mItem.item[i][0] == 3) && (mItem.item[i][8] < data[lev].min_enemies_left_par)) data[lev].min_enemies_left_par = mItem.item[i][8];
   }


/*
   int error = 0;

   // count number of starts
   number_of_starts = 0;
   int s[8] = {0};


   for (int i=0; i<500; i++)
      if (mItem.item[i][0] == 5)
      {
         int x = mItem.item[i][7]; // start index
         if (x < 0) mInput.m_err("Level has a start index less than 0");
         if (x > 7) mInput.m_err("Level has a start index more than 7");
         if ((x >=0) && (x<8))
         {
            number_of_starts++;
            s[x]++; // save index of this start
         }
      }

   if (number_of_starts == 0)
   {
      mInput.m_err("Level has no start block");
      error = 1;
   }

   if (number_of_starts > 8 )
   {
      mInput.m_err("Level has more than 8 start blocks");
      error = 1;
   }
   for (int i=0; i<8; i++)
      if (s[i] > 1)
      {
         sprintf(msg, "Level has duplicate start blocks with index %d", i);
         mInput.m_err(msg);
         error = 1;
      }

   if (number_of_starts > 1)
      for (int i=1; i<number_of_starts; i++)
         if (s[i] == 0)
         {
            sprintf(msg, "Level has %d starts but no start with index %d", number_of_starts, i);
            mInput.m_err(msg);
            error = 1;
         }
   if (error)
   {
      printf("number_of_starts:%d\n", number_of_starts);
      for (int i=0; i<8; i++)
         printf("%d %d\n", i, s[i]);
   }

   */

}


void mwLevel::reset_level_data(void)
{
   clear_data();
   create_level_icons();
   save_data();
}

void mwLevel::unlock_all_levels(void)
{
   for(int i=0; i<100; i++) data[i].unlocked = 1;
   for(int i=0; i<16; i++) area_locks[i] = 0;
   save_data();
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


void mwLevel::check_achievments(void)
{
   // do the level unlocks by iterating levels with area_array, thay all should be in order
   for(int i=0; i<99; i++)
      if (data[area_array[i][0]].completed) data[area_array[i+1][0]].unlocked = 1;

   // do the area unlocks
   for(int a=1; a<10; a++) // iterate areas
   {
      int complete = 1; // area complete by default
      for(int j=0; j<100; j++)
         if ((area_array[j][1] == a) && (data[area_array[j][0]].completed == 0)) complete = 0;
      if (complete) area_locks[a+1] = 0; // open barrier next area
   }

   // special case for basic training levels
   if (data[83].completed)
   {
      area_locks[1] = 0;    // open barrier to next lower row
      data[2].unlocked = 1; // open first level in area 1


      area_locks[14] = 0;   // open barrier to area 14, advanced training
      unlock_all_level_in_area(14);
   }
   save_data();
}


void mwLevel::level_complete_data(void)
{

//   if ((mPlayer.syn[mPlayer.active_local_player].control_method != 1) || (mLevel.skc_trigger_demo_cheat))// don't count anything done in demo mode, unless cheat!
   {
      mLevel.skc_trigger_demo_cheat = 0;

      int lev = play_level;

      data[lev].unlocked = 1; // if for some reason it isn't already
      //   printf("level_complete_data() play_level:%d\n", play_level);
      int ct = mLoop.frame_num;
      printf("1 level_complete_data() mLoop.frame_num:%d\n", ct);

      // add entry to play_data[] array
      int i = play_data_num;
      play_data[i].level = lev;
      play_data[i].timer = ct;
      play_data[i].completed = 1;
      play_data[i].player_respawns = level_data_player_respawns;
      play_data[i].enemies_killed = level_data_enemies_killed;
      play_data[i].purple_coins_collected = level_data_purple_coins_collected;
      play_data[i].enemies_left_alive_at_exit = mEnemy.num_enemy;
      play_data_num++;


      // was the level just completed for the first time
      if (!data[lev].completed)
      {
         printf("first time:%d\n", ct);
         data[lev].completed = 1;
         data[lev].best_time                  = ct;
         data[lev].min_respawns               = level_data_player_respawns;
         data[lev].max_enemies_killed         = level_data_enemies_killed;
         data[lev].max_purple_coins_collected = level_data_purple_coins_collected;
         data[lev].min_enemies_left           = mEnemy.num_enemy;
      }

      // check for new best values
      if (ct                                < data[lev].best_time)                  data[lev].best_time                  = ct;
      if (level_data_player_respawns        < data[lev].min_respawns)               data[lev].min_respawns               = level_data_player_respawns;
      if (level_data_enemies_killed         > data[lev].max_enemies_killed)         data[lev].max_enemies_killed         = level_data_enemies_killed;
      if (level_data_purple_coins_collected > data[lev].max_purple_coins_collected) data[lev].max_purple_coins_collected = level_data_purple_coins_collected;
      if (mEnemy.num_enemy                  < data[lev].min_enemies_left)           data[lev].min_enemies_left           = mEnemy.num_enemy;

      check_achievments();
   }
}



void mwLevel::show_level_stats(int x, int y)
{
   al_draw_textf(mFont.pr8, mColor.White, x, y, 0, "%-10s num name", "Area");
   y+=10;


   for(int i=0; i<100; i++)
      if (area_array[i][1])
      {
         int lev = area_array[i][0];
         int area = area_array[i][1];

         char area_name[80];
         sprintf(area_name, "Area:%d", area);
         if ((area == 13) || (area == 14)) sprintf(area_name, "Training");
         if (area == 10) sprintf(area_name, "Extra");
         if (area == 12) sprintf(area_name, "Demo");

         char lev_name[80];
         sprintf(lev_name, data[lev].level_name);


//         al_draw_text(mFont.pr8, mColor.Red, x, y, 0, "TEST!!!");

         al_draw_textf(mFont.pr8, mColor.White, x, y, 0, "%-10s %-3d %s", area_name, lev, lev_name);

         y+=8;


        // printf("%s lev:%d name:%s\n", area_name, lev, lev_name);

      }
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
   sprintf(mItem.pmsgtext[i], txt);
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
   int pcc = 0; // count max purple coins clooected in this area
   int tmr = 0; // count how many levels in this area have below par time

   int nl = 0;
   for (int i=0; i<100; i++)
      if ((area_array[i][1] == area) || ((area == 16) && (area_array[i][1]) && (area_array[i][1] != 11)) || ((area == 17) && (area_array[i][1] > 0) && (area_array[i][1] < 10)  ))
      {
         nl++;
         int l = area_array[i][0];
         cmp += data[l].completed;
         pct += data[l].tot_purple_coins;
         pcc += data[l].max_purple_coins_collected;
         if ((data[l].best_time > 0) && (data[l].best_time < data[l].par_time) && (data[l].completed)) tmr++;
      }

   int col = 10;
   if ((area_locks[area]) && (area != 16) && (area != 17)) // locked
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


   // area messages
   sob_area_msg(13, 640,  40);
   sob_area_msg(14, 1250,  40);

   sob_area_msg(1, 745,  220);
   sob_area_msg(2, 1161, 220);
   sob_area_msg(3, 745,  400);
   sob_area_msg(4, 1161, 400);
   sob_area_msg(5, 745,  580);
   sob_area_msg(6, 1161, 580);
   sob_area_msg(7, 745,  760);
   sob_area_msg(8, 1161, 760);

   sob_area_msg(9, 725, 940);   // final area
   sob_area_msg(17, 1161, 940); // main area levels

   sob_area_msg(16, 1161, 1010); // all levels



   sob_area_msg(10, 745, 1410); // extra levels

   sob_area_msg(12, 940, 1750); // demo levels


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

      // sort by x
      for(int i=0; i<resi-1; i++)
      {
         if ( ((xasc == 1) && (res[i][1] > res[i+1][1])) || ((xasc == 0) && (res[i][1] < res[i+1][1])) )
         {
            swap_flag = 1;

            res[49][0] = res[i][0]; // temp
            res[49][1] = res[i][1];
            res[49][2] = res[i][2];

            res[i][0] = res[i+1][0];
            res[i][1] = res[i+1][1];
            res[i][2] = res[i+1][2];

            res[i+1][0] = res[49][0];
            res[i+1][1] = res[49][1];
            res[i+1][2] = res[49][2];

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
   for(int i=0; i<100; i++) if (area_array[i][1] == area) data[area_array[i][0]].unlocked = 1;
}



void mwLevel::clear_data(void)
{
   for(int i=0; i<16; i++) area_locks[i] = 1;

   area_locks[13] = 0; // basic training area unlocked

   load_level(1, 1, 0); // load overworld level (also sets overworld barriers)

   fill_area_array();

   overworld_level = 0; // no gate will be found and player will start from start block

   for(int i=0; i<100; i++)
   {
      strcpy(data[i].level_name, "");
      data[i].par_time = 0;
      data[i].unlocked = 0;
      data[i].completed = 0;
      data[i].best_time = 0;
      data[i].min_respawns = 0;
      data[i].max_purple_coins_collected = 0;
      data[i].max_enemies_killed = 0;
      data[i].tot_purple_coins = 0;
      data[i].min_enemies_left = 0;
      data[i].min_enemies_left_par = 0;
   }

   int i = 2;
   strcpy(data[i].level_name, "Switch Nest");
   data[i].par_time = 4000; // 1:40 demo 1:24

   i = 3;
   strcpy(data[i].level_name, "Blue Key Fall");
   data[i].par_time = 1400; // 35s demo 33.1

   i = 4;
   strcpy(data[i].level_name, "Switch Puzzle");
   data[i].par_time = 8400; // 3:30 demo 3:22

   i = 5;
   strcpy(data[i].level_name, "Kill Kill Kill");
   data[i].par_time = 6400; // 2:40 demo 2:30

   i = 6;
   strcpy(data[i].level_name, "Breakable");
   data[i].par_time = 9600; // 4:00 demo 3:41

   i = 7;
   strcpy(data[i].level_name, "Ranger Bob");
   data[i].par_time = 2000; // 50s demo 46.6

   i = 8;
   strcpy(data[i].level_name, "Switch Pit");
   data[i].par_time = 8800; // 3:40 demo 3:31

   i = 10;
   strcpy(data[i].level_name, "The Dead Zone");
   data[i].par_time = 11600; // 4:50 demo 4:45

   i = 11;
   strcpy(data[i].level_name, "Good and Evil");
   data[i].par_time = 19200; // 8:00 demo 7:11

   i = 12;
   strcpy(data[i].level_name, "Bucket of Bad");
   data[i].par_time = 16800; // 7:00 demo 6:19

   i = 13;
   strcpy(data[i].level_name, "Falling Arrows");
   data[i].par_time = 12400; // 5:10 demo 5:02

   i = 14;
   strcpy(data[i].level_name, "Block Puzzle");
   data[i].par_time = 400; // 10s demo 8.6

   i = 15;
   strcpy(data[i].level_name, "Smash");
   data[i].par_time = 1600; // 40s demo 33.8

   i = 16;
   strcpy(data[i].level_name, "Big Apple");
   data[i].par_time = 14400; // 6:00 demo 5:30

   i = 17;
   strcpy(data[i].level_name, "Little Rocket");
   data[i].par_time = 19200; // 8:00 demo 7:28

   i = 18;
   strcpy(data[i].level_name, "Incinerator");
   data[i].par_time = 16800; // 7:00 demo 6:41

   i = 19;
   strcpy(data[i].level_name, "Crazy Fox");
   data[i].par_time = 19200; // 8:00 demo 7:47

   i = 20;
   strcpy(data[i].level_name, "Strange Things");
   data[i].par_time = 7200; // 3:00 demo 2:24

   i = 21;
   strcpy(data[i].level_name, "Long One");
   data[i].par_time = 3600; // 1:30 demo 1:28

   i = 22;
   strcpy(data[i].level_name, "Rocket Stew");
   data[i].par_time = 4800; // 2:00 demo 1:58

   i = 23;
   strcpy(data[i].level_name, "Amazing");
   data[i].par_time = 21600; // 9:00 demo 8:27

   i = 24;
   strcpy(data[i].level_name, "Wendy");
   data[i].par_time = 6600; // 2:45 demo 2:30


   i = 26;
   strcpy(data[i].level_name, "Long Fall");
   data[i].par_time = 4800; // 2:00 demo 1:05

   i = 27;
   strcpy(data[i].level_name, "Gauntlet");
   data[i].par_time = 3600; // 1:30 demo 1:27

   i = 28;
   strcpy(data[i].level_name, "Nowhere to Stand");
   data[i].par_time = 3600; // 1:30 demo 1:08

   i = 29;
   strcpy(data[i].level_name, "Brain Trust");
   data[i].par_time = 13200; // 5:30 demo 5:03

   i = 30;
   strcpy(data[i].level_name, "Coral Cave");
   data[i].par_time = 7200; // 3:00 demo 2:55

   i = 31;
   strcpy(data[i].level_name, "Beginnings");
   data[i].par_time = 12000; // 5:00 demo 4:51

   i = 32;
   strcpy(data[i].level_name, "Yellow Cake");
   data[i].par_time = 9600; // 4:00 demo 3:39

   i = 33;
   strcpy(data[i].level_name, "Kate's Gate");
   data[i].par_time = 6000; // 2:30 demo 2:08

   i = 34;
   strcpy(data[i].level_name, "Crush");
   data[i].par_time = 3600; // 1:30 demo 1:12

   i = 35;
   strcpy(data[i].level_name, "Bomb Toss");
   data[i].par_time = 13200; // 5:30 demo 5:29

   i = 38;
   strcpy(data[i].level_name, "Come Over");
   data[i].par_time = 3200; // 1:20 demo 1:07

   i = 39;
   strcpy(data[i].level_name, "Bomb Run");
   data[i].par_time = 2400; // 1:00 demo 0:55.8

   i = 40;
   strcpy(data[i].level_name, "Edward");
   data[i].par_time = 4200; // 1:45 demo 1:36

   i = 41;
   strcpy(data[i].level_name, "Ender");
   data[i].par_time = 12000; // 5:00 demo 4:10

   i = 42;
   strcpy(data[i].level_name, "Cloner Chain");
   data[i].par_time = 16800; // 7:00 demo 6:55

   i = 43;
   strcpy(data[i].level_name, "Cathedral");
   data[i].par_time = 4800; // 2:00 demo 1:50

   i = 44;
   strcpy(data[i].level_name, "Too Many Lifts");
   data[i].par_time = 12000; // 5:00 demo 4:58

   i = 45;
   strcpy(data[i].level_name, "Chimney of Death");
   data[i].par_time = 1800; // :45 demo :37.9

   i = 46;
   strcpy(data[i].level_name, "Bucket List");
   data[i].par_time = 2400; // 1:00 demo 0:43.6

   i = 47;
   strcpy(data[i].level_name, "Begin Again");
   data[i].par_time = 2400; // 1:00 demo 27.8s

   i = 48;
   strcpy(data[i].level_name, "Mayhem");
   data[i].par_time = 2400; // 1:00 demo :43.7

   i = 49;
   strcpy(data[i].level_name, "Rocket Square");
   data[i].par_time = 9600; // 4:00 demo 3:55

   i = 50;
   strcpy(data[i].level_name, "Welcome");
   data[i].par_time = 1600; // :40 demo :33.5

   i = 51;
   strcpy(data[i].level_name, "Cannon Bees");
   data[i].par_time = 6000; // 2:30 demo 2:19

   i = 52;
   strcpy(data[i].level_name, "Boules");
   data[i].par_time = 3600; // 1:30 demo 1:15

   i = 53;
   strcpy(data[i].level_name, "Lifts With Names");
   data[i].par_time = 8400; // 3:30 demo 3:12

   i = 54;
   strcpy(data[i].level_name, "Old Training Level");
   data[i].par_time = 2400; // 1:00 demo :44.6

   i = 55;
   strcpy(data[i].level_name, "Ancient Ruins");
   data[i].par_time = 14400; // 6:00 demo 5:15

   i = 56;
   strcpy(data[i].level_name, "Zaiden");
   data[i].par_time = 12000; // 5:00 demo 4:40

   i = 57;
   strcpy(data[i].level_name, "Valentine");
   data[i].par_time = 12000; // 5:00 demo 4:27

   i = 60;
   strcpy(data[i].level_name, "Bomb Toss");
   data[i].par_time = 2400; // 2:00 demo

   i = 61;
   strcpy(data[i].level_name, "Bomb Toss 2");
   data[i].par_time = 2400; // 2:00 demo


   i = 64;
   strcpy(data[i].level_name, "Escape Rocket!");
   data[i].par_time = 16880; // 7:02 demo 7:01



   // ---------------------------------------
   // Test Levels
   // ---------------------------------------

   i = 70;
   strcpy(data[i].level_name, "Test Level 1");
   data[i].par_time = 200;
   data[i].unlocked = 1;

   i = 71;
   strcpy(data[i].level_name, "Test Level 2");
   data[i].par_time = 240;



   // ---------------------------------------
   // Training Levels
   // ---------------------------------------

   i = 80;
   strcpy(data[i].level_name, "Training Level 1");
   data[i].par_time = 1200; // 30s demo 25
   data[i].unlocked = 1;

   i = 81;
   strcpy(data[i].level_name, "Training Level 2");
   data[i].par_time = 4800; // 2:00 demo 1:30

   i = 82;
   strcpy(data[i].level_name, "Training Level 3");
   data[i].par_time = 8400; // 3:30 demo 3:10

   i = 83;
   strcpy(data[i].level_name, "Training Level 4");
   data[i].par_time = 6000; // 2:30 demo 2:22

   i = 84;
   strcpy(data[i].level_name, "Training Level 5");
   data[i].par_time = 4800; // 2:00 demo 1:24

   i = 9;
   strcpy(data[i].level_name, "Bomb Intro");
   data[i].par_time = 6400; // 2:40 demo 2:37

   i = 25;
   strcpy(data[i].level_name, "One Cannon");
   data[i].par_time = 1000; // 25s demo 19.7s

   i = 37;
   strcpy(data[i].level_name, "Sacrifice");
   data[i].par_time = 800; // :20 demo 19.0


   // ---------------------------------------
   // Advanced Information Levels
   // ---------------------------------------
   i = 85;
   strcpy(data[i].level_name, "Doors");
   data[i].par_time = 4800; // 2:00 demo 1:58

   i = 86;
   strcpy(data[i].level_name, "Triggers and Timers");
   data[i].par_time = 7200; // 3:00 demo 2:26

   i = 87;
   strcpy(data[i].level_name, "Block Manip");
   data[i].par_time = 2400; // 1:00 demo 0:57

   i = 88;
   strcpy(data[i].level_name, "Block Damage");
   data[i].par_time = 6000; // 2:30 demo 2:10

   i = 89;
   strcpy(data[i].level_name, "Orbs");
   data[i].par_time = 4800; // 2:00 demo 1:44

   i = 90;
   strcpy(data[i].level_name, "Archwagons");
   data[i].par_time = 4800; // 2:00 demo 1:50

   i = 91;
   strcpy(data[i].level_name, "Trakbots");
   data[i].par_time = 2400; // 1:00 demo 0:41.2

   i = 92;
   strcpy(data[i].level_name, "Bouncers and Cannons");
   data[i].par_time = 4800; // 2:00 demo 1:29

   i = 93;
   strcpy(data[i].level_name, "Cloners");
   data[i].par_time = 4800; // 2:00 demo 1:26

   i = 94;
   strcpy(data[i].level_name, "Vinepods");
   data[i].par_time = 4800; // 2:00 demo 1:23

   i = 95;
   strcpy(data[i].level_name, "Blokwalks");
   data[i].par_time = 4800; // 2:00 demo 1:05

   i = 96;
   strcpy(data[i].level_name, "Flappers");
   data[i].par_time = 2400; // 1:00 demo 0:46.9

   i = 97;
   strcpy(data[i].level_name, "Jumpworms");
   data[i].par_time = 4800; // 2:00 demo 1:18

   i = 98;
   strcpy(data[i].level_name, "Lifts");
   data[i].par_time = 7200; // 3:00 demo 2:25

   for(int i=0; i<10000; i++)
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

}


void mwLevel::load_data(void)
{
   FILE *fp =fopen("bitmaps/level_data.pm","rb");
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
   mInput.m_err("Error loading level_data.pm");
}

void mwLevel::save_data(void)
{
   FILE *fp =fopen("bitmaps/level_data.pm","wb");
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


void mwLevel::create_level_icons(void)
{
   int sz = 200;
   ALLEGRO_BITMAP *tmp = al_create_bitmap(sz*10, sz*10);
   al_set_target_bitmap(tmp);
   al_clear_to_color(al_map_rgba(0,0,0,0));

   int sz2 = level_icon2_size;
   ALLEGRO_BITMAP *tmp2 = al_create_bitmap(sz2*10, sz2*10);
   al_set_target_bitmap(tmp2);
   al_clear_to_color(al_map_rgba(0,0,0,0));


   int x=0;
   int y=0;
   for (int i=0; i<100; i++)
   {

      if (mLevel.load_level(i, 0, 1))
      {
         al_set_target_bitmap(tmp);
         mScreen.draw_level2(tmp, x*sz, y*sz, sz, 1, 1, 1, 1, 0);

         al_set_target_bitmap(tmp2);
         mScreen.draw_level2(tmp2, x*sz2, y*sz2, sz2, 1, 1, 1, 1, 0);


      }


      // show progress bar
      int pc = i*100 / 100;
      al_set_target_backbuffer(mDisplay.display);
      //al_clear_to_color(al_map_rgb(0,0,0));
      mScreen.draw_percent_bar(mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2, mDisplay.SCREEN_W-200, 20, pc );
      al_draw_text(mFont.pr8, mColor.pc[15], mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2+6, ALLEGRO_ALIGN_CENTER, "Creating Level Icons");
      al_flip_display();

      if (++x > 9)
      {
         x = 0;
         y++;
      }
   }







//   al_set_target_backbuffer(mDisplay.display);
//   al_draw_bitmap(tmp, 0,0,0);
//   al_flip_display();
//   mInput.tsw();

   al_save_bitmap("bitmaps\\level_icons.bmp", tmp);
   al_save_bitmap("bitmaps\\level_icons2.bmp", tmp2);

   al_destroy_bitmap(tmp);
}

void mwLevel::load_level_icons(void)
{
   int sz = 200;
   //double llt0 = al_get_time();
   ALLEGRO_BITMAP *tmp = al_load_bitmap("bitmaps\\level_icons.bmp");
   if (!tmp) mInput.m_err("Error loading tiles from:level_icons.bmp");
   else
   {
      int x=0;
      int y=0;
      for (int i=0; i<100; i++)
      {
         if (!mLevel.level_icon[i]) mLevel.level_icon[i] = al_create_bitmap(sz, sz);

         al_set_target_bitmap(mLevel.level_icon[i]);
         al_clear_to_color(al_map_rgba(0,0,0,0));

         al_draw_bitmap_region(tmp, x*sz, y*sz, sz, sz, 0, 0, 0);
         if (++x > 9)
         {
            x = 0;
            y++;
         }
     }
     //double llt1 = al_get_time() - llt0;
     //printf("Load level icons time:%f\n", llt1*1000);
     al_destroy_bitmap(tmp);
   }

   sz = level_icon2_size;
   //double llt0 = al_get_time();
   tmp = al_load_bitmap("bitmaps\\level_icons2.bmp");
   if (!tmp) mInput.m_err("Error loading tiles from:level_icons2.bmp");
   else
   {
      int x=0;
      int y=0;
      for (int i=0; i<100; i++)
      {
         if (!mLevel.level_icon2[i]) mLevel.level_icon2[i] = al_create_bitmap(sz, sz);

         al_set_target_bitmap(mLevel.level_icon2[i]);
         al_clear_to_color(al_map_rgba(0,0,0,0));

         al_draw_bitmap_region(tmp, x*sz, y*sz, sz, sz, 0, 0, 0);
         if (++x > 9)
         {
            x = 0;
            y++;
         }
     }
     //double llt1 = al_get_time() - llt0;
     //printf("Load level icons time:%f\n", llt1*1000);
     al_destroy_bitmap(tmp);




   }
}



