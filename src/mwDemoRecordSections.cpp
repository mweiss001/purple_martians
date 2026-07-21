// mwDemoRecord.cpp
#include "pm.h"
#include "mwDemoRecord.h"
#include "mwDemoMode.h"
#include "mwBitmap.h"
#include "mwColor.h"
#include "mwDisplay.h"
#include "mwFont.h"
#include "mwGameMoves.h"
#include "mwGmInfo.h"
#include "mwInput.h"
#include "mwLoop.h"
#include "mwMenu.h"
#include "mwMiscFnx.h"
#include "mwPlayer.h"
#include "mwWidget.h"


void mwDemoRecord::mark_player_carry(int p)
{
   for (int i=0; i<fhs_indx; i++)
      if ((mLoop.frame_num >= fhs[i][1]) && (mLoop.frame_num <= fhs[i][2]) && (fhs[i][0] == p)) fhs[i][3] = 5;
}

void mwDemoRecord::mark_player_shot_used(int p, int f, int type)
{
   // mark the section that was active when this shot was fired
   for (int i=0; i<fhs_indx; i++)
      if ((p == fhs[i][0]) && (f >= fhs[i][1]) && (f <= fhs[i][2])) fhs[i][3] = type;
}

void mwDemoRecord::remove_unused_fire_held_sections(int test)
{
   int sections_removed = 0;

   mGmInfo.fill();

   int aild = mGmInfo.levelDoneFrame;

// exceptions
//   fhs[27][3] = 99;
//   fhs[76][3] = 99;
//   fhs[78][3] = 99;
//   fhs[184][3] = 99;
//   fhs[186][3] = 99;


   for (int i=0; i<fhs_indx; i++)
      if (fhs[i][3] == 0)
      {
         sections_removed++;
         int ild = mGmInfo.levelDoneFrame;

         int p = fhs[i][0];
         int f1 = fhs[i][1];
         int f2 = fhs[i][2];
         for (int x=0; x<mGameMoves.entry_pos; x++)
            if ((mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_PLAYER_MOVE) && (mGameMoves.arr[x][0] >= f1) && (mGameMoves.arr[x][0] < f2) && (mGameMoves.arr[x][2] == p)) mGameMoves.arr[x][3] &= ~PM_COMPMOVE_FIRE;

         if (test)
         {
            mGmInfo.fill();
            if (ild != mGmInfo.levelDoneFrame)
            {
               printf("Removing section:%d  p:%d  %d-%d   ild:%d ld:%d ----  Error!\n", i, p, f1, f2, ild, mGmInfo.levelDoneFrame);
            }
            printf("Removing section:%d  p:%d  %d-%d ld:%d\n", i, p, f1, f2, mGmInfo.levelDoneFrame);
         }
         else printf("Removing section:%d  p:%d  %d-%d ld:%d\n", i, p, f1, f2, mGmInfo.levelDoneFrame);

      }
   mGmInfo.fill();

   if (aild != mGmInfo.levelDoneFrame)
   {
      printf("Error! - After removing all sections, level done has changed from %d to %d\n", aild, mGmInfo.levelDoneFrame);
//      reload();
   }
   else printf("%d Sections removed\n", sections_removed);
}



void mwDemoRecord::find_fire_held_sections(int p)
{
   // clear array
   for (int i=0; i<10000; i++)
      for (int j=0; j<4; j++)
         fhs[i][j] = 0;
   fhs_indx = 0;


   // setup player range
   int sp = p;
   int ep = p+1;
   if (p == -1) // all players
   {
      sp = 0;
      ep = NUM_PLAYERS;
   }


   for (p=sp; p<ep; p++) // cycle player range
   {
      int cf = 0;
      for (int x=0; x<mGameMoves.entry_pos; x++)
         if ((mGameMoves.arr[x][2] == p) && (mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_PLAYER_MOVE))
         {
            int f = mGameMoves.arr[x][0]; // frame
            int c = mGameMoves.arr[x][3] & PM_COMPMOVE_FIRE; // comp move fire detect

            if ((cf == 0) && (c)) // fire section starts
            {
               cf = 1;
               fhs[fhs_indx][0] = p;
               fhs[fhs_indx][1] = f;
            }
            if ((cf == 1) && (!c)) // fire section ends
            {
               cf = 0;
               fhs[fhs_indx][2] = f;
               fhs_indx++;
            }
         }
   }


   // then play the level and detect when fire held sections are used
   seek_to_frame(0, 0);
   mLoop.ff_state = 1;
   int done = 0;
   while (!done)
   {
      mLoop.frame_num++;
      mGameMoves.proc();
      mLoop.move_frame();
      if (mLoop.frame_num > mDemoMode.last_frame + 100) done = 1;
   }
   mLoop.ff_state = 0;

   if (1)
   {
      int us = 0;
      int eh = 0;
      int ph = 0;
      int od = 0;
      int td = 0;
      int bb = 0;
      int pc = 0;

      for (int i=0; i<fhs_indx; i++)
      {
         if (fhs[i][3] == 0) us++;
         if (fhs[i][3] == 1) eh++;
         if (fhs[i][3] == 2) ph++;
         if (fhs[i][3] == 3) od++;
         if (fhs[i][3] == 4) td++;
         if (fhs[i][3] == 5) pc++;
         if (fhs[i][3] == 6) bb++;
      }

      printf("%d - total fire held sections ----------------\n", fhs_indx);
      printf("%d - unused fire held sections\n", us);
      printf("%d - enemy hits\n", eh);
      printf("%d - player hits\n", ph);
      printf("%d - breakable blocks\n", bb);
      printf("%d - orb detect\n", od);
      printf("%d - trg detect\n", td);
      printf("%d - player carry\n", pc);
   }
}



void mwDemoRecord::proc_section_details_menu(void)
{
   if (mInput.mouse_b[2][0])
   {
      int i = current_section;
      int p   = mGmInfo.gmPlayerInfo[i].playerNum;
      int f1  = mGmInfo.gmPlayerInfo[i].startFrame;
      int f2  = mGmInfo.gmPlayerInfo[i].endFrame;




      sprintf(mMenu.menu_string[0],"Section Details Menu");
      sprintf(mMenu.menu_string[1],"--------------------");
      sprintf(mMenu.menu_string[2],"Delete Inactive Game Move");
      sprintf(mMenu.menu_string[3],"Add Inactive Game Move");
      sprintf(mMenu.menu_string[4],"Delete Section");
      sprintf(mMenu.menu_string[5],"Find Unused Fire Sections");
      sprintf(mMenu.menu_string[6],"Remove Unused Fire Sections");
      sprintf(mMenu.menu_string[7],"Remove Unused Fire Sections test");
      sprintf(mMenu.menu_string[8],"Remove Duplicate Game Move");
      sprintf(mMenu.menu_string[9],"Add FIRE at current pos");
      sprintf(mMenu.menu_string[10],"end");
      int mp = mMenu.pmenu(5, 13, -12, 1);

      if (mp == 2)
      {
         for (int x=0; x<mGameMoves.entry_pos; x++)
            if ((mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_PLAYER_INACTIVE) && (mGameMoves.arr[x][2] == p))
               if ((mGameMoves.arr[x][0] >= f1) && (mGameMoves.arr[x][0] <= f2)) mGameMoves.gm_remove(x);
         refresh();
      }
      if (mp == 3)
      {
         for (int x=0; x<mGameMoves.entry_pos; x++)
            if ((mGameMoves.arr[x][2] == p) && (mGameMoves.arr[x][0] >= current_frame_num)) mGameMoves.clear_single(x);

         mGameMoves.add_game_move2(current_frame_num, PM_GAMEMOVE_TYPE_PLAYER_INACTIVE, p, 0);
         mGameMoves.gm_sort();
         refresh();

      }
      if (mp == 4 && p != 0) // delete section -- but don't delete player 0
      {
         for (int x=0; x<mGameMoves.entry_pos; x++)
            if (mGameMoves.does_game_move_contain_player(x, p)) mGameMoves.clear_single(x);
         mGameMoves.gm_sort();
         refresh();
      }
      if (mp == 5)
      {
         find_fire_held_sections(p);
         refresh();
      }

      if (mp == 6)
      {
         remove_unused_fire_held_sections(0);
         refresh();
      }

      if (mp == 7)
      {
         remove_unused_fire_held_sections(1);
         refresh();
      }
      if (mp == 8)
      {
         mGameMoves.remove_doubled_moves();
         refresh();
      }

      if (mp == 9)
      {
         range_tools_set_fire_moves(current_frame_num, 1);
         range_tools_set_fire_moves(current_frame_num+1, 0);

         mGameMoves.gm_sort();
         refresh();

      }
   }
}





void mwDemoRecord::set_active_section(int i)
{
   if ((i < 0) || (i >= (int) mGmInfo.gmPlayerInfo.size())) return;
   current_section = i;

   int p = mGmInfo.gmPlayerInfo[i].playerNum;
   int c = mGmInfo.gmPlayerInfo[i].playerCol;

   record_player_number = mPlayer.active_local_player = p;
   record_player_color = c;
   load_lnk_arr();    // load games moves into link array

}


int mwDemoRecord::draw_section_details(int x1, int y1)
{
   int i = current_section;

   int p   = mGmInfo.gmPlayerInfo[i].playerNum;
   int col = mGmInfo.gmPlayerInfo[i].playerCol;
   int f1  = mGmInfo.gmPlayerInfo[i].startFrame;
   int f2  = mGmInfo.gmPlayerInfo[i].endFrame;


   int w = 208;
   //w +=  mLoop.pct_x;
   int x2 = x1 + w;

   int h = 109;
   //h +=  mLoop.pct_y;
   int y2 = y1 + h;

   if ((mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2)) proc_section_details_menu();

   al_draw_filled_rectangle(x1, y1, x2, y2, mColor.pc[0]);
   al_draw_rectangle(x1, y1, x2, y2, mColor.pc[col], 1);
   al_draw_line(x1, y1+12, x2, y1+12, mColor.pc[col], 1);
   al_draw_textf(mFont.pr8, mColor.pc[15], (x1+w/2), y1+2, ALLEGRO_ALIGN_CENTER, "Current Section");
   y1+=4;
   if (mWidget.buttont(x1+4,  y1, x1+12, 8,  0,0,0,0,  0,-1,15, 0,  1,0,0,0, "-")) set_active_section(current_section-1);
   if (mWidget.buttont(x2-12, y1, x2-4,  8,  0,0,0,0,  0,-1,15, 0,  1,0,0,0, "+")) set_active_section(current_section+1);
   y1+=10;

   int line_space = 10;

   int x = x1+4;
   int y = y1;

   al_draw_bitmap(mBitmap.player_tile[col][1], x+124, y1, 0 );
   al_draw_text(mFont.pr8, mColor.pc[col], x+146, y1+7, 0, mColor.color_name[col]);

   al_draw_textf(mFont.pr8, mColor.pc[15], x, y+1, 0, "Player Number:%d", p);

   y+=line_space+1;
   al_draw_text( mFont.pr8, mColor.pc[15], x, y, 0, "Player Color:");

   y+=line_space+1;
   al_draw_text( mFont.pr8, mColor.pc[15], x, y, 0, "Player Name:");
   int ya = y-2;
   if (mWidget.buttontcb(x+98, ya, 0, 13,  0,0,0,0,  0,15,15,10,  1,0,0,0, mPlayer.syn[p].name))
   {
      ya = y-17;
      mMiscFnx.edit_player_name(x+134, ya, p);
      int gi = mGameMoves.find_first_active_game_move_for_player(p, f1, f2);
      if (gi) mGameMoves.gma_change_name(gi, mPlayer.syn[p].name);
      refresh();
   }
   y+=line_space+1;
   al_draw_line(x1, y, x2, y, mColor.pc[col], 1);
   y+=3;

   al_draw_text(mFont.pr8, mColor.pc[15], (x1+w/2), y, ALLEGRO_ALIGN_CENTER, "Change Player Color");
   y+=line_space;
   int cl = mWidget.colsel(x, y, x+200, 16,  10,0,0,0,  0,12,15,15,  0,0,1,0);
   if (cl != -1)
   {
      int gi = mGameMoves.find_first_active_game_move_for_player(p, f1, f2);
      if (gi) mGameMoves.gma_change_color(gi, cl);
      refresh();
   }
   y+=2;
   al_draw_line(x1, y, x2, y, mColor.pc[col], 1);
   y+=2;

   char m1[64], m2[64];
   al_draw_textf(mFont.pr8, mColor.pc[15], x, y, 0, "Range:%s to %s", gettf(f1, m1), gettf(f2, m2));

   y+=line_space;
   al_draw_textf(mFont.pr8, mColor.pc[15], x, y, 0, "Deaths:%d", (int)mGmInfo.gmPlayerInfo[i].deaths.size());

   y+=line_space;
   al_draw_textf(mFont.pr8, mColor.pc[15], x, y, 0, "Purple Coins:%d", (int)mGmInfo.gmPlayerInfo[i].purpleCoins.size());

   return y2;
}
