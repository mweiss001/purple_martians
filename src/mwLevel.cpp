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
#include "mwVisualLevel.h"
#include "mwNetgame.h"
#include "mwShot.h"
#include "mwTriggerEvent.h"



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
   mConfig.save_config(PM_CFG_SAVE_START_LEVEL);
}

int level_exists(int level)
{
   char fn[256];
   sprintf(fn, "levels/level%03d.pml", level);
   if (al_filename_exists(fn)) return 1;
   return 0;
}


// returns the next level that exists after 'lev'
// if greater than max_lev:
// if wrap return 1
// if not wrap (clamp) return highest level less than max
int mwLevel::get_next_level(int lev, int max_lev, int wrap)
{
   int done = 0;
   while (!done)
   {
      if (level_exists(++lev)) done = 1;
      if (lev > max_lev)
      {
         if (wrap)
         {
            lev = 1;
            done = 1;
         }
         else
         {
            lev = get_prev_level(lev, max_lev, wrap);
            done = 1;
         }
      }
   }
   return lev;
}

// returns the first prev level that exists before 'lev'
// if less than 1
// if wrap return highest level less than max
// if not wrap (clamp) return level 1
int mwLevel::get_prev_level(int lev, int max_lev, int wrap)
{
   int done = 0;
   while (!done)
   {
      if (level_exists(--lev)) done = 1;
      if (lev < 1)
      {
         if (wrap)
         {
            lev = max_lev;
            lev = get_prev_level(lev, max_lev, wrap);
            done = 1;
         }
         else
         {
            lev = 1;
            done = 1;
         }
      }
   }
   return lev;
}

















void mwLevel::change_block(int x, int y, int block)
{
   if ((x >= 0) && (x < 100) & (y >= 0) && (y < 100))
   {
      l[x][y] = block;
      if (!mDisplay.no_display)
      {
         al_set_target_bitmap(mBitmap.level_background);
         al_draw_filled_rectangle(x*20, y*20, x*20+20, y*20+20, mColor.pc[0]);
         al_draw_bitmap(mBitmap.btile[block & 1023], x*20, y*20, 0);
      }
   }
}



int mwLevel::is_block_empty(int x, int y, int test_block, int test_item, int test_enemy)
{
   int mpty = 0; // default is not empty (because if x and y not valid, I don't want to use it)
   if ((x > -1) && (x < 100) & (y > -1) && (y < 100)) // if valid x and y
   {
      mpty = 1; // default is empty
      if (test_block) if (mLevel.l[x][y] & PM_BTILE_ALL_SOLID) mpty = 0; // if any of the solids match, it is not empty
      if (test_enemy)
         for (int c=0; c<100; c++)
            if (mEnemy.Ei[c][0])
               if ((mEnemy.Ef[c][0] == x*20) && (mEnemy.Ef[c][1] == y*20)) mpty = 0;
      if (test_item)
         for (int c=0; c<500; c++)
            if (mItem.item[c][0])
               if ((mItem.item[c][4] == (x*20)) && (mItem.item[c][5] == (y*20))) mpty = 0;
   }
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

   prev_level_loaded = last_level_loaded;
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

         if (mLoop.level_editor_running)
         {
            mItem.sort_item(1);
            mEnemy.sort_enemy();
         }

         level_check();
         level_start_data();

         mShot.clear_shots();

         mTriggerEvent.initialize();

         if (!mDisplay.no_display) mScreen.init_level_background(); // draw blocks on level_background


         mPlayer.init_player(0, 2);

         mLoop.eco_draw = 0;
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

   char msg[256];
   int error = 0;

   // count number of starts
   number_of_starts = 0;
   int s[8] = {0};

   for (int i=0; i<500; i++)
      if (mItem.item[i][0] == 5)
      {
         int x = mItem.item[i][7]; // start index
         if (x < 0)
         {
            sprintf(msg, "Level %d has a start index less than 0 (%d)", lev, x);
            mInput.m_err(msg);
            error = 1;
         }
         if (x > 7)
         {
            sprintf(msg, "Level %d has a start index greater than 70 (%d)", lev, x);
            mInput.m_err(msg);
            error = 1;
         }
         if ((x >=0) && (x<8))
         {
            number_of_starts++;
            s[x]++; // save index of this start
         }
      }

   if (number_of_starts == 0)
   {
      sprintf(msg, "Level %d has no start block", lev);
      mInput.m_err(msg);
      error = 1;
   }

   if (number_of_starts > 8 )
   {
      sprintf(msg, "Level %d has more than 8 start blocks (%d)", lev, number_of_starts);
      mInput.m_err(msg);
      error = 1;
   }
   for (int i=0; i<8; i++)
      if (s[i] > 1)
      {
         sprintf(msg, "Level %d has duplicate start blocks with index %d", lev, i);
         mInput.m_err(msg);
         error = 1;
      }

   if (number_of_starts > 1)
      for (int i=1; i<number_of_starts; i++)
         if (s[i] == 0)
         {
            sprintf(msg, "Level %d has %d starts but no start with index %d", lev, number_of_starts, i);
            mInput.m_err(msg);
            error = 1;
         }
   if (error)
   {
      printf("Level Check Error: %s\n", msg);
      printf("number_of_starts:%d\n", number_of_starts);
      for (int i=0; i<8; i++)
         printf("%d %d\n", i, s[i]);
   }

}


