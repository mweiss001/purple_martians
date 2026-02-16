// mwGmInfo.cpp

#include "pm.h"
#include "mwGmInfo.h"
#include "mwDemoMode.h"
#include "mwGameMoves.h"
#include "mwItem.h"
#include "mwLevel.h"
#include "mwLoop.h"
#include "mwMiscFnx.h"
#include "mwPlayer.h"
#include "mwSql.h"

mwGmInfo mGmInfo;

void mwGmInfo::clear()
{
   // clear game_event table
   mSql.execute_sql("DELETE FROM game_events", mSql.db_game_events);

   gmPlayerInfo.clear();
   lastFrame = -1;
   levelDoneFrame = -1;
   levelDonePlayer = -1;
}



// called at the end of load_gm
void mwGmInfo::fill()
{
   clear();

   // set lastFrame from last gm entry frame num
   lastFrame = mDemoMode.last_frame;

   // run through the game to fill game_event table and find level done if it exists
   mDemoMode.seek_to_frame(0, 0);
   // double t0 = al_get_time();

   mLoop.ff_state = 1;
   addEventsToDatabase = true;
   int done = 0;
   while (!done)
   {
      mLoop.frame_num++;
      mGameMoves.proc();
      mLoop.move_frame();

      if (mPlayer.syn[0].level_done_mode)
      {
         levelDoneFrame  = mPlayer.syn[0].level_done_frame;
         levelDonePlayer = mPlayer.syn[0].level_done_player;
         lastFrame = levelDoneFrame;
         // printf("1 - levelDoneFrame = %d player:%d\n", levelDoneFrame, levelDonePlayer);
         done = 1;
      }
      if (mLoop.frame_num > lastFrame + 400) done = 1;
   }
   addEventsToDatabase = false;
   mLoop.ff_state = 0;

   //printf("t1: %f\n", (al_get_time() - t0)*1000);

   mDemoMode.seek_to_frame(0, 0);
   findPlayerTracks();
   findDeaths();
   findPurpleCoins();
}



// iterate game moves, use player active and inactive to create gmPlayerInfo entries
void mwGmInfo::findPlayerTracks()
{
   int p0_hidden = 0;
   for (int x=0; x<mGameMoves.entry_pos; x++)
   {
      int f = mGameMoves.arr[x][0]; // frame
      int t = mGameMoves.arr[x][1]; // game move type

      // this will only ever be player 0 in headless server mode
      if (t == PM_GAMEMOVE_TYPE_PLAYER_HIDDEN) p0_hidden = 1;

      if (t & PM_GAMEMOVE_TYPE_PLAYER_ACTIVE_FLAG)
      {
         int p, c;
         char name[9] = { 0 };
         mMiscFnx.gma_to_val(mGameMoves.arr[x][1], mGameMoves.arr[x][2], mGameMoves.arr[x][3], p, c, name);
         gmPlayerInfo.push_back({ p, c, name, f, 0, {}, {} });
      }

      if (t == PM_GAMEMOVE_TYPE_PLAYER_INACTIVE)
      {
         int p = mGameMoves.arr[x][2]; // player number
         // find existing that matches player num and has endFrame set to 0
         for (auto& r : gmPlayerInfo)
            if (r.playerNum == p && r.endFrame == 0) r.endFrame = f;
      }
   }

   // if last frame not set, set to lastFrame
   for (auto& r : gmPlayerInfo)
      if (r.endFrame == 0) r.endFrame = lastFrame;


   if (p0_hidden)
   {
      for (int i=0; i<(int)gmPlayerInfo.size(); i++)
      {
         if (gmPlayerInfo[i].playerNum == 0)
         {
            gmPlayerInfo.erase(gmPlayerInfo.begin() + i);
            break;
         }
      }
   }
}

// returns index into gmPlayerInfo where p == playerNum and f is between startFrame and endFrame
int mwGmInfo::getGmPlayerInfoIndex(int p, int frame)
{
   for (int i=0; i<(int)gmPlayerInfo.size(); i++)
      if (p == gmPlayerInfo[i].playerNum && frame >= gmPlayerInfo[i].startFrame && frame <= gmPlayerInfo[i].endFrame) return i;
   return -1;
}

void mwGmInfo::findDeaths()
{
   std::vector<std::vector<int>> matrix;
   mSql.execute_sql_and_return_2d_vector_int("SELECT p, frame FROM game_events WHERE ev=8", mSql.db_game_events, matrix);
   for (int i=0; i<(int)matrix.size(); i++)
   {
      int index = getGmPlayerInfoIndex(matrix[i][0], matrix[i][1]);
      if (index != -1) gmPlayerInfo[index].deaths.push_back(matrix[i][1]);
   }
}

void mwGmInfo::findPurpleCoins()
{
   std::vector<std::vector<int>> matrix;
   mSql.execute_sql_and_return_2d_vector_int("SELECT p, frame FROM game_events WHERE ev=27", mSql.db_game_events, matrix);
   for (int i=0; i<(int)matrix.size(); i++)
   {
      int index = getGmPlayerInfoIndex(matrix[i][0], matrix[i][1]);
      if (index != -1) gmPlayerInfo[index].purpleCoins.push_back(matrix[i][1]);
   }
}

