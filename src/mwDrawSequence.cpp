// mwDrawSequence.cpp

#include "pm.h"
#include "mwLog.h"
#include "mwPlayer.h"
#include "mwDrawSequence.h"
#include <limits>
#include "mwFont.h"
#include "mwLift.h"
#include "mwTimeStamp.h"
#include "mwColor.h"
#include "mwLoop.h"
#include "mwItem.h"
#include "mwEnemy.h"
#include "mwScreen.h"
#include "mwShot.h"
#include "mwMiscFnx.h"


mwDrawSequence mDrawSequence;

mwDrawSequence::mwDrawSequence()
{
   initialize();
   draw(1);
}

void mwDrawSequence::initialize(void)
{
   for (int i=0; i<20; i++)
   {
      RA[i].initialize();
      ts[i] = 0;
   }
}


void mwDrawSequence::add_names(int &i, const char* n0, const char* n1)
{
   strcpy(name[0][i], n0);
   strcpy(name[1][i], n1);
   i++;
}


void mwDrawSequence::add(int &i)
{
   ts[i] = al_get_time();
   RA[i].add_data(ts[i] - ts[i-1]);
   i++;
}

void mwDrawSequence::draw(int setup_only)
{
   int i=0;

   if (!setup_only) ts[i] = al_get_time(); // get starting timestamp
   i++;

   float totl = 0;


   if (setup_only) add_names(i, "d-bkgr", "get_new_background");
   else { mScreen.get_new_background(1);  add(i); }

   if (setup_only) add_names(i, "d-lift", "draw_lifts");
   else { mLift.draw_lifts();  add(i); }

   if (RA[i-1].avg*1000 > 5) mLoop.eco_draw = 1;


   if (setup_only) add_names(i, "d-item", "draw_items");
   else { mItem.draw_items(); add(i); }



//   if (setup_only) add_names(i, "d-gate", "draw_gates");
//   else
//   {
//      for (int i=0; i<500; i++)
//         if (mItem.item[i][0] == 18) mItem.draw_gate(i, mItem.item[i][4], mItem.item[i][5], 0);
//
//      add(i);
//   }

//   if (setup_only) add_names(i, "d-mssg", "draw_message");
//   else
//   {
//      // draw msg last so they are on top
//      for (int c=0; c<500; c++)
//         if (mItem.item[c][0] == 10)
//         {
//            mItem.draw_message(c, mItem.itemf[c][0], mItem.itemf[c][1], 0);
//
//
////            char msg[256];
////            int timer_count=0, timer_val=0;
////            mMiscFnx.get_int_3216(mItem.item[c][12], timer_count, timer_val);
////            // if timer running and not always show, draw the message
////            // if always show, it has been drawn on background
////            if ((timer_count) && (!(mItem.item[c][2] & PM_ITEM_PMSG_SHOW_ALWAYS))) mItem.draw_pop_message(c, 0, 0, 0, 0, 0, msg);
//         }
//      add(i);
//   }
//




   if (setup_only) add_names(i, "d-enem", "draw_enemies");
   else { mEnemy.draw_enemies(); add(i); }

   if (setup_only) add_names(i, "d-esht", "draw_eshots");
   else { mShot.draw_eshots(); add(i); }

   if (setup_only) add_names(i, "d-psht", "draw_pshots");
   else { mShot.draw_pshots(); add(i); }

   if (setup_only) add_names(i, "d-plyr", "draw_players");
   else { mPlayer.draw_players(); add(i); }






   if (setup_only) add_names(i, "d-erhd", "erase hidden");
   else
   {
       if (!mLoop.level_editor_running) mItem.erase_hider_areas();
       add(i);
   }

   if (setup_only) add_names(i, "d-gnfo", "gate info");
   else
   {
      // draw gate info
      for (int p=0; p<NUM_PLAYERS; p++)
         if ((mPlayer.syn[p].active) && (mPlayer.syn[p].marked_gate != -1))
            mItem.draw_gate_info(mPlayer.syn[p].marked_gate);
       add(i);
   }

   if (setup_only) add_names(i, "d-lsta", "level stats");
   else
   {
      for (int i=0; i<500; i++)
         if ((mItem.item[i][0] == 10) && (!strncmp(mItem.pmsgtext[i], "Level Statistics", 16)))
            mItem.draw_message(i, 0, 0, 0);
       add(i);
   }

   if (setup_only) add_names(i, "d-buff", "scale_buff_to_display");
   else { mScreen.draw_scaled_level_region_to_display(0); add(i); }


   // draw purple coins directly on the screen, so they scale nicely
   for (int c=0; c<500; c++)
      if ((mItem.item[c][0] == 2) && (mItem.item[c][6] == 3)) mItem.draw_purple_coin_screen_direct(c);



//   if (setup_only) add_names(i, "vpod", "vinepods direct to screen");
//   else
//   {
//      for (int e=0; e<100; e++)
//         if (mEnemy.Ei[e][0] == 7) mEnemy.draw_vinepod_screen_direct(e); // vinepod
//      add(i);
//   }


   if (setup_only) add_names(i, "plyr", "players direct to screen");
   else
   {
      for (int p=0; p<NUM_PLAYERS; p++)
         if (mPlayer.syn[p].active) mPlayer.draw_player_direct_to_screen(p);

      // do this so that that local player is always drawn on top
      mPlayer.draw_player_direct_to_screen(mPlayer.active_local_player);
      add(i);
   }

   // draw npc directly on the screen, so they scale nicely
   for (int e=0; e<100; e++)
      if (mEnemy.Ei[e][0] == 19) mEnemy.draw_crew_screen_direct(e);


















   if (setup_only) add_names(i, "d-ovrl", "draw_screen_overlay");
   else { mScreen.draw_screen_overlay(); add(i); }

   if (setup_only) add_names(i, "d-flip", "al_flip_display");
   else { al_flip_display(); add(i); }

   if (setup_only) add_names(i, "d-totl", "total draw time");
   else
   {
      ts[i] = al_get_time(); // set final timestamp
      totl = ts[i] - ts[0];  // get total time
      RA[i].add_data(totl);  // add total time
      i++;
   }

   ns = i; // number of items
   if (mLog.LOG_TMR_draw_all)
   {
      char msg[1024];
      char msg2[64];
      sprintf(msg, "tmst");
      for (int i=1; i<ns-1; i++)
      {
         sprintf(msg2, " %s:[%0.4f]", name[0][i], (ts[i] - ts[i-1]) * 1000 );
         strcat(msg, msg2);
      }
      sprintf(msg2, " %s:[%0.4f]\n", name[0][ns-1], totl * 1000);  // total
      strcat(msg, msg2);
      //printf("\n%s\n", msg);
      mLog.add_log_entry2(44, 0, msg);
   }
   if (mLog.LOG_TMR_draw_tot) mLog.add_log_TMR(totl, "draw", 0);
}


char * mwDrawSequence::get_line(int s, char* msg)
{
   sprintf(msg, "%3.3f - %s", RA[s].avg*1000, name[1][s]);
   return msg;
}

void mwDrawSequence::show_text(int x, int y)
{
   char msg[1024];
   al_draw_filled_rectangle(x, y, x+240, y+ns*9, mColor.pc[0]);
   for (int i=1; i<ns; i++)
      al_draw_textf(mFont.pr8, mColor.pc[15], 1+x, 1+y+i*9, 0, "%s", get_line(i, msg));
}



