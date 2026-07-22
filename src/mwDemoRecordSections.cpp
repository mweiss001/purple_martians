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
   // get a copy of levelDoneFrame to compare later
   mGmInfo.fill();
   int aild = mGmInfo.levelDoneFrame;
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
            if (ild != mGmInfo.levelDoneFrame) printf("Removing section:%d  p:%d  %d-%d   ild:%d ld:%d ----  Error!\n", i, p, f1, f2, ild, mGmInfo.levelDoneFrame);
            printf("Removing section:%d  p:%d  %d-%d ld:%d\n", i, p, f1, f2, mGmInfo.levelDoneFrame);
         }
         else printf("Removing section:%d  p:%d  %d-%d ld:%d\n", i, p, f1, f2, mGmInfo.levelDoneFrame);
      }
   mGmInfo.fill();
   if (aild != mGmInfo.levelDoneFrame)
   {
      printf("Error! - After removing all sections, level done has changed from %d to %d\n", aild, mGmInfo.levelDoneFrame);
      //reload();
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
            if ((mGameMoves.arr[x][2] == p) && (mGameMoves.arr[x][0] >= mLoop.frame_num)) mGameMoves.clear_single(x);

         mGameMoves.add_game_move2(mLoop.frame_num, PM_GAMEMOVE_TYPE_PLAYER_INACTIVE, p, 0);
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
         range_tools_set_fire_moves(mLoop.frame_num,     1);
         range_tools_set_fire_moves(mLoop.frame_num + 1, 0);

         mGameMoves.gm_sort();
         refresh();

      }
   }
}


void mwDemoRecord::set_active_section(int i)
{
   // check if the index is valid
   if ((i < 0) || (i >= (int) mGmInfo.gmPlayerInfo.size())) return;
   current_section = i;

   mPlayer.active_local_player = mGmInfo.gmPlayerInfo[i].playerNum;
   mPlayer.syn[mPlayer.active_local_player].color = mGmInfo.gmPlayerInfo[i].playerCol;

   load_lnk_arr();    // load games moves into link array
}

void mwDemoRecord::draw_current_section(mwWindow w)
{
   if (w.rect.contains(mInput.mouse_x, mInput.mouse_y)) proc_section_details_menu();

   int ya = w.rect.y1 + 4;
   int c = w.color;
   int d = w.disable_input;

   if (mWidget.mButton(1, w.rect.x1+22, 8,   1, w.rect.y1, 12,  0,0,0,3,  0,0,15,14,   0, "-", d)) set_active_section(current_section-1);
   if (mWidget.mButton(1, w.rect.x2-30, 8,   1, w.rect.y1, 12,  0,0,0,3,  0,0,15,14,   0, "+", d)) set_active_section(current_section+1);

   int i = current_section;
   int p   = mGmInfo.gmPlayerInfo[i].playerNum;
   int col = mGmInfo.gmPlayerInfo[i].playerCol;
   int f1  = mGmInfo.gmPlayerInfo[i].startFrame;
   int f2  = mGmInfo.gmPlayerInfo[i].endFrame;

   // draw player tile in upper right corner
   al_draw_bitmap(mBitmap.player_tile[col][1], w.rect.x2-28, ya+13, ALLEGRO_FLIP_HORIZONTAL );

   int x = w.rect.x1+4;
   ya+=11;
   int line_space = 12;

   al_draw_textf(mFont.pr8, mColor.pc[15], x, ya, 0, "Player Number:%d", p);

   ya+=line_space+2;
   al_draw_text(mFont.pr8, mColor.pc[15], x, ya, 0, "Player Color:");
   al_draw_text(mFont.pr8, mColor.pc[col], w.rect.x1+110 /*+mLoop.pct_x */, ya, 0, mColor.color_name[col]);

   ya+=line_space+2;
   al_draw_text( mFont.pr8, mColor.pc[15], x, ya, 0, "Player Name:");
   ya-=2;

   if (mWidget.buttontcb(x+98, ya, 0, 13,  0,0,0,0,  0,15,15,10,  1,0,0,d, mPlayer.syn[p].name))
   {
      mMiscFnx.edit_player_name(x+134, ya, p);
      int gi = mGameMoves.find_first_active_game_move_for_player(p, f1, f2);
      if (gi) mGameMoves.gma_change_name(gi, mPlayer.syn[p].name);
      refresh();
   }

   ya+=line_space+1;
   al_draw_line(w.rect.x1, ya, w.rect.x2, ya, mColor.pc[c], 1);
   ya+=+3;

   al_draw_text(mFont.pr8, mColor.pc[15], w.rect.XCenter(), ya, ALLEGRO_ALIGN_CENTER, "Change Player Color");
   ya+=line_space;
   int cl = mWidget.colsel(x, ya, x+200, 16,  10,0,0,0,  0,12,15,15,  0,0,1,d);
   if (cl != -1)
   {
      int gi = mGameMoves.find_first_active_game_move_for_player(p, f1, f2);
      if (gi) mGameMoves.gma_change_color(gi, cl);
      refresh();
   }

   ya+= 2; al_draw_line(w.rect.x1, ya, w.rect.x2, ya, mColor.pc[c], 1); ya+=3;

   char m1[64], m2[64];
   al_draw_textf(mFont.pr8, mColor.pc[15], x, ya, 0, "Range:%s to %s", gettf(f1, m1), gettf(f2, m2));

   ya+=line_space;
   al_draw_textf(mFont.pr8, mColor.pc[15], x, ya, 0, "Deaths:%d", (int)mGmInfo.gmPlayerInfo[i].deaths.size());

   ya+=line_space;
   al_draw_textf(mFont.pr8, mColor.pc[15], x, ya, 0, "Purple Coins:%d", (int)mGmInfo.gmPlayerInfo[i].purpleCoins.size());
}

