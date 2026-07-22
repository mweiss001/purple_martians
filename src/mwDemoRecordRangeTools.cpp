// mwDemoRecord.cpp
#include "pm.h"
#include "mwDemoRecord.h"
#include "mwDemoMode.h"
#include "mwColor.h"
#include "mwFont.h"
#include "mwGameMoves.h"
#include "mwGmInfo.h"
#include "mwInput.h"
#include "mwLoop.h"
#include "mwMenu.h"
#include "mwWidget.h"

void mwDemoRecord::range_tools_set_fire_moves(int f, int clear_set)
{
   int p = mGmInfo.gmPlayerInfo[current_section].playerNum;

   // look for an existing game move for this frame and player
   int existing_move = 0;
   for (int x=0; x<mGameMoves.entry_pos; x++)
      if ((mGameMoves.arr[x][0] == f) && (mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_PLAYER_MOVE) && (mGameMoves.arr[x][2] == p)) existing_move = x;

   if (existing_move)
   {
      if (clear_set) mGameMoves.arr[existing_move][3] |= PM_COMPMOVE_FIRE;
      else           mGameMoves.arr[existing_move][3] &= ~PM_COMPMOVE_FIRE;
   }
   else
   {
      // find previous game move for this player
      int prev_move_index = 0;
      int prev_move_frame = 0;

      // look for largest frame num that is less than f
      for (int x=0; x<mGameMoves.entry_pos; x++)
         if ((mGameMoves.arr[x][0] < f) && (mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_PLAYER_MOVE) && (mGameMoves.arr[x][2] == p))
         {
            if (mGameMoves.arr[x][0] > prev_move_frame)
            {
               prev_move_frame = mGameMoves.arr[x][0];
               prev_move_index = x;
            }
         }

      int cm = 0;
      if (prev_move_index) cm = mGameMoves.arr[prev_move_index][3];

      if (clear_set) cm |= PM_COMPMOVE_FIRE;
      else           cm &= ~PM_COMPMOVE_FIRE;

      mGameMoves.add_game_move2(f, PM_GAMEMOVE_TYPE_PLAYER_MOVE, p, cm);
   }
}


void mwDemoRecord::range_tools_do(int action)
{
   int p = mGmInfo.gmPlayerInfo[current_section].playerNum;
   for (int x=0; x<mGameMoves.entry_pos; x++)
      if ((mGameMoves.arr[x][0] >= rt_start_frame) && (mGameMoves.arr[x][0] <= rt_end_frame) && ((rt_all_players) || (mGameMoves.arr[x][2] == p)) )
      {
         if (action == 1) mGameMoves.clear_single(x);
         if (action == 2) mGameMoves.arr[x][0]++;
         if (action == 3) mGameMoves.arr[x][0]--;
         if (action == 4)
         {
            if (mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_PLAYER_MOVE) mGameMoves.arr[x][3] &= ~ PM_COMPMOVE_FIRE;
         }
      }
   mGameMoves.gm_sort();
   refresh();
}



void mwDemoRecord::proc_range_tools_menu(void)
{
   if (mInput.mouse_b[2][0])
   {
      sprintf(mMenu.menu_string[0],"Range Tools Menu");
      sprintf(mMenu.menu_string[1],"-----------------");
      sprintf(mMenu.menu_string[2],"Set Start to Current Time");
      sprintf(mMenu.menu_string[3],"Set Start to Frame 1");
      sprintf(mMenu.menu_string[4],"Set End to Current Time");
      sprintf(mMenu.menu_string[5],"Set End to Last Frame");
      sprintf(mMenu.menu_string[6],"-----------------");
      sprintf(mMenu.menu_string[7],"Delete Range");
      sprintf(mMenu.menu_string[8],"Shift Range +1");
      sprintf(mMenu.menu_string[9],"Shift Range -1");
      sprintf(mMenu.menu_string[10],"Remove FIRE from Range");
      sprintf(mMenu.menu_string[11],"Add FIRE to Range");
      sprintf(mMenu.menu_string[12],"Add single FIRE to Start");
      sprintf(mMenu.menu_string[13],"end");
      int mp = mMenu.pmenu(5, 13, -12, 1);
      if (mp == 2) rt_start_frame = mLoop.frame_num;
      if (mp == 3) rt_start_frame = 1;
      if (mp == 4) rt_end_frame = mLoop.frame_num;
      if (mp == 5) rt_end_frame = mDemoMode.last_frame;
      if (mp == 7) range_tools_do(1);
      if (mp == 8) range_tools_do(2);
      if (mp == 9) range_tools_do(3);
      if (mp == 10) range_tools_do(4);
      if (mp == 11)
      {
         range_tools_do(4); // delete all fire

         for (int f=rt_start_frame; f<=rt_end_frame; f+=4)
         {
            range_tools_set_fire_moves(f, 1);
            range_tools_set_fire_moves(f+1, 0);
         }

         mGameMoves.gm_sort();
         refresh();
      }
      if (mp == 12)
      {
         range_tools_set_fire_moves(rt_start_frame, 1);
         range_tools_set_fire_moves(rt_start_frame+1, 0);

         mGameMoves.gm_sort();
         refresh();
      }
   }
}

void mwDemoRecord::draw_range_tools(mwWindow w)
{
   if (w.rect.contains(mInput.mouse_x, mInput.mouse_y)) proc_range_tools_menu();

   int x1 = w.rect.x1;
   int x2 = w.rect.x2;
   int y1 = w.rect.y1 + 14;
   int xa = w.rect.x1+4;
   int xb = w.rect.x2-4;
   int d = w.disable_input;
   int c = w.color;

   char m1[64];

   al_draw_textf(mFont.pr8, mColor.pc[15], xa, y1, 0, "Start:%s", gettf(rt_start_frame, m1));
   if (mWidget.buttont(xb-30, y1, xb, 10,  0,0,0,0,  0,c,15, 0,  1,0,0,d, "Set")) rt_start_frame = mLoop.frame_num;

   y1+=10; al_draw_line(x1, y1, x2, y1, mColor.pc[c], 1); y1+=2;

   al_draw_textf(mFont.pr8, mColor.pc[15], xa, y1, 0, "End  :%s", gettf(rt_end_frame, m1));
   if (mWidget.buttont(xb-30, y1, xb, 10,  0,0,0,0,  0,c,15, 0,  1,0,0,d, "Set")) rt_end_frame = mLoop.frame_num;

   y1+=10; al_draw_line(x1, y1, x2, y1, mColor.pc[c], 1);

   mWidget.togglec(xa, y1, xa+40, 16,  0,0,0,0,  0,0,0,0, 1,0,1,d, rt_all_players,  "All Players", 15, 15);
}
