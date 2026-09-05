// mwGmInfo.cpp

#include "pm.h"
#include "mwGmInfo.h"
#include "mwDemoMode.h"
#include "mwEnemy.h"
#include "mwGameEvent.h"
#include "mwGameMoves.h"
#include "mwLevel.h"
#include "mwLoop.h"
#include "mwMiscFnx.h"
#include "mwPlayer.h"
#include "mwSound.h"
#include "mwSql.h"

mwGmInfo mGmInfo;

void mwGmInfo::clear()
{
   // clear game_event vector
   mGameEvent.game_events.clear();

   // clear player info vector
   gmPlayerInfo.clear();

   lastFrame = -1;
   levelDoneFrame = -1;
   levelDonePlayer = -1;

   completed = false;

   coinsCollected = 0;
   playerDeaths = 0;
   enemiesKilled = 0;

}


/*
plays back the current level with the current game moves array and gathers statistics
reloads the current level to seek back to start
*/

void mwGmInfo::fill()
{
   //printf("mwGmInfo::fill()\n");

   clear();

   // get last frame from last game move
   lastFrame = mGameMoves.arr[mGameMoves.entry_pos-1][0];

   // save current control method to restore later
   int old_mPlayer0_control_method = mPlayer.syn[0].control_method;

   // load level and seek start
   mDemoMode.seek_to_frame(0, 0);


   // double t0 = al_get_time();

   mSound.mute = 1;

   // playback level to fill game_event vector and find level done if exists
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
         completed = true;
         done = 1;
      }

      // if 10s past last game move and level not done, stop and leave completed = false and last frame as last game move frame
      if (mLoop.frame_num > lastFrame + 400) done = 1;
   }

   mSound.mute = 0;

   // restore old control method
   mPlayer.syn[0].control_method = old_mPlayer0_control_method;

   //printf("t1: %f\n", (al_get_time() - t0)*1000);

   findPlayerTracks();
   findPlayerTracksLastMoves();
   findDeaths();
   findPurpleCoins();
   findEnemyHits();
}




// adds the current data stored in gmInfo to database
// called after fill
void mwGmInfo::add()
{
   char sql[500];


   sprintf(sql, "INSERT INTO play_data (level,                          time,           completed, num_players,        player_respawns,         enemies_killed,     enemies_left,        coins_collected) VALUES(%d, %d, %d, %d, %d, %d, %d, %d);" ,
                                        mLevel.play_level, mGmInfo.lastFrame,   mGmInfo.completed,           0,   mGmInfo.playerDeaths,  mGmInfo.enemiesKilled, mEnemy.num_enemy, mGmInfo.coinsCollected);

   printf("sql:%s \n", sql);

   mSql.execute_sql(sql, mSql.db_level_play_stats);

   mLevel.calc_level_stats(mLevel.play_level);
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
         gmPlayerInfo.push_back({ p, c, name, f, 0, false, f, {}, {} });
      }

      if (t == PM_GAMEMOVE_TYPE_PLAYER_INACTIVE)
      {
         int p = mGameMoves.arr[x][2]; // player number
         // find existing that matches player num and has endFrame set to 0
         for (auto& r : gmPlayerInfo)
            if (r.playerNum == p && r.endFrame == 0) r.endFrame = f;
      }
   }

   // if end frame not set, set to lastFrame
   for (auto& r : gmPlayerInfo)
      if (r.endFrame == 0)
      {
         r.endFrame = lastFrame;
         r.noEnd = true;
      }

   // if p0_hidden erase all player 0 tracks (this is only for headless server mode)
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


void mwGmInfo::findPlayerTracksLastMoves()
{
   for (auto& r : gmPlayerInfo)
   {
      int p = r.playerNum;
      int f1 = r.startFrame;
      int f2 = r.endFrame;
      for (int x=0; x<mGameMoves.entry_pos; x++)
      {
         int f = mGameMoves.arr[x][0];
         if (f >=f1 && f < f2 && mGameMoves.arr[x][2] == p) r.lastMoveFrame = f;
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



void mwGmInfo::findEnemyHits()
{
   enemiesShot = 0;
   for (auto& gm : mGameEvent.game_events)
      if (gm.ev == 43)
      {
         enemiesShot++;
         int f  = gm.frame;
         int p  = gm.z1; // player num
         int e  = gm.z2; // enemy num
         int sf = gm.z4; // shot fired frame
         int hc = gm.z3; // enemy extra hits to kill

         int index = getGmPlayerInfoIndex(p, f);
         if (index != -1) gmPlayerInfo[index].enemyHits.push_back({f, e, sf, hc });

      }
}


int mwGmInfo::testFire(int f, int p)
{
   int index = getGmPlayerInfoIndex(p, f);
   if (index != -1)
   {
      for (auto eh : gmPlayerInfo[index].enemyHits)
         if (eh.shotFiredFrame == f)
         {
            if (eh.hitsLeft) return 1; // hit, but not killed
            else return 2; // killed
         }
   }
   return 0;
}

void mwGmInfo::findDeaths()
{
   playerDeaths = 0;
   for (auto& gm : mGameEvent.game_events)
      if (gm.ev == 8)
      {
         playerDeaths++;
         int p = gm.z1;
         int f = gm.frame;
         int index = getGmPlayerInfoIndex(p, f);
         if (index != -1) gmPlayerInfo[index].playerDeaths.push_back(f);
      }
}

void mwGmInfo::findPurpleCoins()
{
   coinsCollected = 0;
   for (auto& gm : mGameEvent.game_events)
      if (gm.ev == 27)
      {
         coinsCollected++;
         int p = gm.z1;
         int f = gm.frame;
         int index = getGmPlayerInfoIndex(p, f);
         if (index != -1) gmPlayerInfo[index].coinsCollected.push_back(f);
      }
}

int mwGmInfo::countTotalPurpleCoinsCollected()
{
   coinsCollected = 0;
   for (auto& gm : mGameEvent.game_events)
      if (gm.ev == 27) coinsCollected++;
   return coinsCollected;
}


