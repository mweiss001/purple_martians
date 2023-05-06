// mwGameEvent.cpp

#include "pm.h"
#include "mwGameEvent.h"
#include "mwBottomMessage.h"
#include "mwSound.h"
#include "mwLoop.h"


mwGameEvent mGameEvent;

void mwGameEvent::add(int ev, int x, int y, int z1, int z2, int z3, int z4)
{
   if (!mLoop.ff_state)
   {
      if (ev == 11) // tally raw damage
      {
         int p = z1;
         float damage = (float)z4 / 100; // damage
         mPlayer.loc[p].damage_type = z2;
         if (mPlayer.loc[p].damage_holdoff > mLoop.frame_num+20) mPlayer.loc[p].damage_holdoff = mLoop.frame_num + 20; // if holdoff is too far in future (this is where it is reset)
         if (mPlayer.loc[p].damage_holdoff < mLoop.frame_num) // triggered and not in holdoff
         {
            mPlayer.loc[p].damage_holdoff = mLoop.frame_num + 20; // set holdoff
            mPlayer.loc[p].damage_tally = damage; // init tally with current damage
         }
         if (mPlayer.loc[p].damage_holdoff > mLoop.frame_num) // triggered and in holdoff
         {
            mPlayer.loc[p].damage_tally += damage; // inc tally with current damage
         }
      }

      if (mBottomMessage.bottom_msg_on) mBottomMessage.add(ev, x, y, z1, z2, z3, z4); // send event to bmsg add
      if (mSound.sound_on)
      {
            /*  sample numbers
            0 - player shoots
            1 - d'OH
            2 - bonus
            3 - hiss
            4 - la dee dah  door, key, exit
            5 - explosion
            6 - grunt 1 shot
            7 - grunt 2 hit
            8 - enemy killed  */
         switch (ev)
         {
            // al_play_sample(ALLEGRO_SAMPLE *spl, float gain, float pan, float speed, ALLEGRO_PLAYMODE loop, ALLEGRO_SAMPLE_ID *ret_id)
            case  1: // player shoots
               //al_play_sample(snd[0], 0.71, 0, .8, ALLEGRO_PLAYMODE_ONCE, NULL);
               al_play_sample(mSound.snd[0], 0.81, 0, .7, ALLEGRO_PLAYMODE_ONCE, NULL);
            break;

            case 2: // explosion
               al_play_sample(mSound.snd[5], .78, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            break;

            case 20: case 22: case 4: // la dee dah (key, door, unlocked exit, warp)
               if (mSound.sample_delay[4]+30 < mLoop.frame_num)
               {
                  mSound.sample_delay[4] = mLoop.frame_num;
                  al_play_sample(mSound.snd[4], 0.78, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
               }
            break;

            case 24: // sproingy
               al_play_sample(mSound.snd[9], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            break;

            case 27: // purple coin
              if (mSound.sample_delay[2]+30 < mLoop.frame_num)
               {
                  mSound.sample_delay[2] = mLoop.frame_num;
                  al_play_sample(mSound.snd[2], 0.78, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
               }
            break;

            case 40: case 41: // player got hurt
               al_play_sample(mSound.snd[6], 0.5, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            break;

            case 12: // player took damage
               if (mSound.sample_delay[7]+14 < mLoop.frame_num)
               {
                  mSound.sample_delay[7] = mLoop.frame_num;
                  al_play_sample(mSound.snd[7], 0.5, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
               }
            break;
            case 42: // enemy killed
               al_play_sample(mSound.snd[8], 0.5, 0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
            break;
            case 8: // d'Oh (player died)
               al_play_sample(mSound.snd[1], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            break;

         }
      }
   }
}
/*

master list of events

num snd msg description
 1   0   -  player fires shot
 2   5   -  explosion
 4   4   -  unlocked exit
 5   -   -  jump

 6   -  yes player joined
 7   -  yes player quit
 8   1  yes player died

11   -   -  tally raw damage
12   7  yes show damage

20   4  yes key
21   -  yes switch
22   4  yes door
23   -  yes locked exit
24   9  yes spring
25   -  yes bomb
26   -  yes rocket
27   2  yes coin
28   -  yes bonus

40   6  yes player hurt by player
41   6  yes player hurt by enemy
42   8  yes player killed enemy

*/

