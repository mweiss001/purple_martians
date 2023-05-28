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
         mScreen.init_level_background(0); // draw blocks on level_background
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


void mwLevel::setup_data(void)
{
//   reset_level_data();
   load_data();
   load_level_icons();
}

void mwLevel::level_start_data(void)
{
//   printf("level_start_data() play_level:%d\n", play_level);

   if (play_level > 1) overworld_level = play_level;


   level_data_purple_coins_collected = 0;
   level_data_player_respawns = 0;
   level_data_enemies_killed = 0;
}

void mwLevel::level_complete_data(void)
{
   if (mPlayer.syn[mPlayer.active_local_player].control_method != 1) // don't count anything done in demo mode
   {


      int lev = play_level;

   //   printf("level_complete_data() play_level:%d\n", play_level);

      // add entry to play_data[] array
      int i = play_data_num;
      play_data[i].level = lev;
      play_data[i].timer = mLoop.frame_num;
      play_data[i].completed = 1;
      play_data[i].player_respawns = level_data_player_respawns;
      play_data[i].enemies_killed = level_data_enemies_killed;
      play_data[i].purple_coins_collected = level_data_purple_coins_collected;
      play_data[i].enemies_left_alive_at_exit = mEnemy.num_enemy;
      play_data_num++;


      // was the level just completed for the first time
      if (!data[lev].completed)
      {
         data[lev].completed = 1;
         data[lev].best_time                  = mLoop.frame_num;
         data[lev].min_respawns               = level_data_player_respawns;
         data[lev].max_enemies_killed         = level_data_enemies_killed;
         data[lev].max_purple_coins_collected = level_data_purple_coins_collected;
         data[lev].min_enemies_left           = mEnemy.num_enemy;
      }

      // check for new best values
      if (mLoop.frame_num                   < data[lev].best_time)                  data[lev].best_time                  = mLoop.frame_num;
      if (level_data_player_respawns        < data[lev].min_respawns)               data[lev].min_respawns               = level_data_player_respawns;
      if (level_data_enemies_killed         > data[lev].max_enemies_killed)         data[lev].max_enemies_killed         = level_data_enemies_killed;
      if (level_data_purple_coins_collected > data[lev].max_purple_coins_collected) data[lev].max_purple_coins_collected = level_data_purple_coins_collected;
      if (mEnemy.num_enemy                  < data[lev].min_enemies_left)           data[lev].min_enemies_left           = mEnemy.num_enemy;

      // calculate achievements

      // check for levels to unlock

   //   if (data[2].completed) data[3].unlocked = 1;
   //   if (data[2].completed) data[3].unlocked = 1;

      for (i=2; i<100; i++)
      {
         if (data[i].completed) data[i+1].unlocked = 1;
      }


      // check for other things to alter on the ovrworld level


      save_data();

   }



}



void mwLevel::clear_data(void)
{
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
   data[i].par_time = 4000;
   data[i].unlocked = 1;

   i = 3;
   strcpy(data[i].level_name, "Blue Key Fall");
   data[i].par_time = 1600;

   i = 4;
   strcpy(data[i].level_name, "Switch Puzzle");
   data[i].par_time = 500;

   i = 5;
   strcpy(data[i].level_name, "Kill Kill Kill");
   data[i].par_time = 500;

   i = 10;
   strcpy(data[i].level_name, "The Dead Zone");
   data[i].par_time = 500;

   i = 31;
   strcpy(data[i].level_name, "testy");
   data[i].par_time = 500;


   i = 70;
   strcpy(data[i].level_name, "Test Level 1");
   data[i].par_time = 200;
   data[i].unlocked = 1;

   i = 71;
   strcpy(data[i].level_name, "Test Level 2");
   data[i].par_time = 240;



   i = 80;
   strcpy(data[i].level_name, "Training Level 1");
   data[i].par_time = 500;


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

}


void mwLevel::load_data(void)
{
   FILE *fp =fopen("bitmaps/level_data.pm","rb");
   if (fp)
   {
      fread(data,           sizeof(data),          1, fp);
      fread(play_data,      sizeof(play_data),     1, fp);
      fread(&play_data_num, sizeof(play_data_num), 1, fp);
      fclose(fp);
      return;
   }
   mInput.m_err("Error loading level_date.pm");
}

void mwLevel::save_data(void)
{
   FILE *fp =fopen("bitmaps/level_data.pm","wb");
   if (fp)
   {
      fwrite(data,           sizeof(data),          1, fp);
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

   int x=0;
   int y=0;
   for (int i=0; i<100; i++)
   {
      al_set_target_bitmap(tmp);
      if (mLevel.load_level(i, 0, 1)) mScreen.draw_level2(tmp, x*sz, y*sz, sz, 1, 1, 1, 1, 0);

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
}



