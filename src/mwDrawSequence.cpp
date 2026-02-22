// mwDrawSequence.cpp

#include "pm.h"
#include "mwLog.h"
#include "mwPlayer.h"
#include "mwDrawSequence.h"
#include <limits>
#include "mwFont.h"
#include "mwLift.h"
#include "mwColor.h"
#include "mwLoop.h"
#include "mwItem.h"
#include "mwEnemy.h"
#include "mwScreen.h"
#include "mwShot.h"
#include "mwMiscFnx.h"
#include "mwPacketBuffer.h"
#include "mwDisplay.h"


mwDrawSequence mDrawSequence;

mwDrawSequence::mwDrawSequence()
{
   // clear all
   for (int i=0; i<20; i++)
   {
      RA[i].initialize(8);
      ts[i] = 0;
   }
   // setup names
   ds_draw(1, 0);
}


void mwDrawSequence::ds_add_names(int &i, const char* n0, const char* n1)
{
   strcpy(name[0][i], n0);
   strcpy(name[1][i], n1);
   i++;
}

void mwDrawSequence::ds_add(int &i)
{
   ts[i] = al_get_time();
   RA[i].add_data(ts[i] - ts[i-1]);
   i++;
}

void mwDrawSequence::ds_add_total(int &i)
{
   ts[i] = al_get_time();
   double total = ts[i] - ts[0];
   RA[i].add_data(total);
   mLog.add_tmr1(LOG_TMR_draw_tot, "draw", total);
   i++;
}



void mwDrawSequence::ds_draw(int setup_only, int flip)
{
   bool packet_check = 1;

   int skip_draw = 0;
   if (mPlayer.syn[0].level_done_mode == 27) skip_draw = 1;
   if (mDisplay.no_display) skip_draw = 1;
   if (!skip_draw)
   {
      int i=0;

      if (!setup_only) ts[i] = al_get_time(); // get starting timestamp

      i++;

      mLog.log_add_prefixed_textf(LOG_OTH_draw, 0, "[%4d]Draw - Get new background\n", mLoop.frame_num);
      if (setup_only) ds_add_names(i, "d-bkgr", "get_new_background");
      else { mScreen.get_new_background(1);  ds_add(i); }

      if (packet_check) mPacketBuffer.check_for_packets();

      mLog.log_add_prefixed_textf(LOG_OTH_draw, 0, "[%4d]Draw - Lifts\n", mLoop.frame_num);
      if (setup_only) ds_add_names(i, "d-lift", "draw_lifts");
      else { mLift.draw_lifts();  ds_add(i); }

//      if (RA[i-1].avg*1000 > 5) mLoop.eco_draw = 1;

      if (packet_check) mPacketBuffer.check_for_packets();

      mLog.log_add_prefixed_textf(LOG_OTH_draw, 0, "[%4d]Draw - Items\n", mLoop.frame_num);
      if (setup_only) ds_add_names(i, "d-item", "draw_items");
      else { mItem.draw_items(); ds_add(i); }



//   if (setup_only) ds_add_names(i, "d-gate", "draw_gates");
//   else
//   {
//      for (int i=0; i<500; i++)
//         if (mItem.item[i][0] == 18) mItem.draw_gate(i, mItem.item[i][4], mItem.item[i][5], 0);
//
//      add(i);
//   }

//   if (setup_only) ds_add_names(i, "d-mssg", "draw_message");
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


      if (packet_check) mPacketBuffer.check_for_packets();


      mLog.log_add_prefixed_textf(LOG_OTH_draw, 0, "[%4d]Draw - Enemies\n", mLoop.frame_num);
      if (setup_only) ds_add_names(i, "d-enem", "draw_enemies");
      else { mEnemy.draw_enemies(); ds_add(i); }

      if (packet_check) mPacketBuffer.check_for_packets();

      mLog.log_add_prefixed_textf(LOG_OTH_draw, 0, "[%4d]Draw - eshots\n", mLoop.frame_num);
      if (setup_only) ds_add_names(i, "d-esht", "draw_eshots");
      else { mShot.draw_eshots(); ds_add(i); }

      mLog.log_add_prefixed_textf(LOG_OTH_draw, 0, "[%4d]Draw - pshots\n", mLoop.frame_num);
      if (setup_only) ds_add_names(i, "d-psht", "draw_pshots");
      else { mShot.draw_pshots(); ds_add(i); }

      mLog.log_add_prefixed_textf(LOG_OTH_draw, 0, "[%4d]Draw - players\n", mLoop.frame_num);
      if (setup_only) ds_add_names(i, "d-plyr", "draw_players");
      else { mPlayer.draw_players(); ds_add(i); }

      if (packet_check) mPacketBuffer.check_for_packets();

      mLog.log_add_prefixed_textf(LOG_OTH_draw, 0, "[%4d]Draw - Erase Hidden\n", mLoop.frame_num);

      if (setup_only) ds_add_names(i, "d-erhd", "erase hidden");
      else
      {
          if (!mLoop.level_editor_running) mItem.erase_hider_areas();
          ds_add(i);
      }

      if (packet_check) mPacketBuffer.check_for_packets();

      mLog.log_add_prefixed_textf(LOG_OTH_draw, 0, "[%4d]Draw - Gate Info\n", mLoop.frame_num);
      if (setup_only) ds_add_names(i, "d-gnfo", "gate info");
      else
      {
         // draw gate info
         for (int p=0; p<NUM_PLAYERS; p++)
            if ((mPlayer.syn[p].active) && (mPlayer.syn[p].marked_gate != -1))
               mItem.draw_gate_info(mPlayer.syn[p].marked_gate);
          ds_add(i);
      }

      if (packet_check) mPacketBuffer.check_for_packets();


      mLog.log_add_prefixed_textf(LOG_OTH_draw, 0, "[%4d]Draw - Level Stats\n", mLoop.frame_num);
      if (setup_only) ds_add_names(i, "d-lsta", "level stats");
      else
      {
         for (int i=0; i<500; i++)
            if ((mItem.item[i][0] == 10) && (!strncmp(mItem.pmsgtext[i], "Level Statistics", 16)))
               mItem.draw_message(i, 0, 0, 0);
          ds_add(i);
      }

      if (packet_check) mPacketBuffer.check_for_packets();

      mLog.log_add_prefixed_textf(LOG_OTH_draw, 0, "[%4d]Draw - Draw scaled level region to display\n", mLoop.frame_num);
      if (setup_only) ds_add_names(i, "d-buff", "scale_buff_to_display");
      else { mScreen.draw_scaled_level_region_to_display(); ds_add(i); }

      if (packet_check) mPacketBuffer.check_for_packets();


      mLog.log_add_prefixed_textf(LOG_OTH_draw, 0, "[%4d]Draw - Draw purple coins direct to screen\n", mLoop.frame_num);
      // draw purple coins directly on the screen, so they scale nicely
      if (setup_only) ds_add_names(i, "d-pcds", "purple coins direct to screen");
      else
      {
         for (int c=0; c<500; c++)
            if ((mItem.item[c][0] == 2) && (mItem.item[c][6] == 3)) mItem.draw_purple_coin_screen_direct(c);
         ds_add(i);
      }


   //   if (setup_only) ds_add_names(i, "vpod", "vinepods direct to screen");
   //   else
   //   {
   //      for (int e=0; e<100; e++)
   //         if (mEnemy.Ei[e][0] == 7) mEnemy.draw_vinepod_screen_direct(e); // vinepod
   //      add(i);
   //   }

      if (packet_check) mPacketBuffer.check_for_packets();

      mLog.log_add_prefixed_textf(LOG_OTH_draw, 0, "[%4d]Draw - Draw players direct to screen\n", mLoop.frame_num);
      if (setup_only) ds_add_names(i, "d-plyr", "draw_players");
      else { mPlayer.draw_players_direct_to_screen(); ds_add(i); }

      if (packet_check) mPacketBuffer.check_for_packets();

      mLog.log_add_prefixed_textf(LOG_OTH_draw, 0, "[%4d]Draw - Draw npc direct to screen\n", mLoop.frame_num);
      // draw npc directly on the screen, so they scale nicely
      if (setup_only) ds_add_names(i, "d-npcd", "npc direct to screen");
      else
      {
         for (int e=0; e<100; e++)
            if (mEnemy.Ei[e][0] == 19) mEnemy.draw_crew_screen_direct(e);
         ds_add(i);
      }

      if (packet_check) mPacketBuffer.check_for_packets();

      mLog.log_add_prefixed_textf(LOG_OTH_draw, 0, "[%4d]Draw - Draw screen overlay\n", mLoop.frame_num);
      if (setup_only) ds_add_names(i, "d-ovrl", "draw_screen_overlay");
      else { mScreen.draw_screen_overlay(); ds_add(i); }

      if (packet_check) mPacketBuffer.check_for_packets();

      mLog.log_add_prefixed_textf(LOG_OTH_draw, 0, "[%4d]Draw - Flip display\n", mLoop.frame_num);
      if (setup_only) ds_add_names(i, "d-flip", "al_flip_display");
      else { if (flip) al_flip_display(); ds_add(i); }

      if (setup_only) ds_add_names(i, "d-totl", "total draw time");
      else ds_add_total(i);

      ns = i; // number of items

      if (mLog.log_types[LOG_TMR_draw_all].action) // profile draw all
      {
         mLog.add_tmr(LOG_TMR_draw_all, ""); // header
         for (int i=1; i<ns-1; i++)
            mLog.log_append_textf(LOG_TMR_draw_all, "%s:[%0.4f] " , name[0][i], (ts[i] - ts[i-1]) * 1000);

         if (mLog.log_types[LOG_TMR_draw_tot].action) mLog.log_append_text(LOG_TMR_draw_all, "\n"); // total has already been logged, end line here
         else                                         mLog.log_append_textf(LOG_TMR_draw_all, "%s:[%0.4f]\n " , name[0][ns-1], (ts[ns-1] - ts[0]) * 1000); // add total and end line

      }
   }
}

void mwDrawSequence::ds_show_text(int x, int y)
{
   al_draw_filled_rectangle(x, y, x+240, y+ns*9, mColor.pc[0]);
   for (int i=1; i<ns; i++)
      al_draw_textf(mFont.pr8, mColor.pc[15], 1+x, 1+y+i*9, 0, "%3.3f - %s", RA[i].avg*1000, name[1][i]);
}
