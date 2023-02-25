// mwGameEvent.cpp

#include "pm.h"
#include "mwGameEvent.h"
#include "mwBottomMessage.h"
#include "mwSound.h"
#include "mwLoop.h"


mwGameEvent mGameEvent;


void mwGameEvent::add(int ev, int x, int y, int z1, int z2, int z3, int z4)
{
   if (mwBM.bottom_msg_on) add(ev, x, y, z1, z2, z3, z4); // send event to bmsg add
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
         case  2: case  4: case  5: // la dee dah (key, exit, door)
            if (mSound.sample_delay[4]+30 < mLoop.frame_num)
            {
               mSound.sample_delay[4] = mLoop.frame_num;
               al_play_sample(mSound.snd[4], 0.78, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
         break;
         case 70: case 71: // free man and purple coin
           if (mSound.sample_delay[2]+30 < mLoop.frame_num)
            {
               mSound.sample_delay[2] = mLoop.frame_num;
               al_play_sample(mSound.snd[2], 0.78, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
         break;
         case 40: case 41: case 43:// player got shot
            al_play_sample(mSound.snd[6], 0.5, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
         break;
         case 44: case 50: case 52: case 54: case 56: case 57: case 59: // player got hit (bomb, mine, enemy collision, squished, stuck)
            if (mSound.sample_delay[7]+14 < mLoop.frame_num)
            {
               mSound.sample_delay[7] = mLoop.frame_num;
               al_play_sample(mSound.snd[7], 0.5, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
         break;
         case 60: case 62: // enemy killed
            al_play_sample(mSound.snd[8], 0.5, 0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
         break;
         case 90: // d'Oh (player died)
            al_play_sample(mSound.snd[1], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
         break;
         case 22: // explosion
            al_play_sample(mSound.snd[5], .78, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
         break;

         case 31: // sproingy
            al_play_sample(mSound.snd[9], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
         break;
      }
   }
}


