// mwLog.cpp

#include "pm.h"
#include "mwLog.h"
#include "mwPlayer.h"
#include "mwGameMoves.h"
#include "mwLoop.h"
#include "mwLevel.h"
#include "mwInput.h"
#include "mwConfig.h"
#include "mwNetgame.h"
#include "mwMiscFnx.h"

#include "mwSql.h"


mwLog mLog;

mwLog::mwLog()
{
   init_log_types(); // now only done when recreating settings.pm
   erase_log();
   erase_log_net();
   erase_log_status();
}

void mwLog::init_log_types()
{
   // printf("init log types\n");
   int i;
   for (i=0; i<100; i++)
   {
      log_types[i].group = 0;
      log_types[i].action = 0;
      strcpy(log_types[i].name, "");
   }


   i = LOG_error;                  log_types[i].group = 9;   strcpy(log_types[i].name, "LOG_error"); log_types[i].action = 3;   // always both actions

   i = LOG_NET;                    log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET");
   i = LOG_NET_DIF_REWIND;         log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_DIF_REWIND");
   i = LOG_NET_DIF_CREATE;         log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_DIF_CREATE");
   i = LOG_NET_DIF_TRX;            log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_DIF_TRX");
   i = LOG_NET_DIF_TRX_PACKET;     log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_DIF_TRX_PACKET");
   i = LOG_NET_DIF_APPLY;          log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_DIF_APPLY");
   i = LOG_NET_DIF_ACK;            log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_DIF_ACK");
   i = LOG_NET_CDAT;               log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_CDAT");
   i = LOG_NET_TIMER_ADJUST;       log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_TIMER_ADJUST");
   i = LOG_NET_CLIENT_PING;        log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_CLIENT_PING");
   i = LOG_NET_FILE_TRANSFER;      log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_FILE_TRANSFER");
   i = LOG_NET_ENDING_STATS;       log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_ENDING_STATS");
   i = LOG_NET_BANDWIDTH;          log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_BANDWIDTH");
   i = LOG_NET_SESSION;            log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_SESSION");
   i = LOG_NET_CSV;                log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_CSV"); log_types[i].action = 0;


   i = LOG_OTH_PROGRAM_STATE;      log_types[i].group = 3;   strcpy(log_types[i].name, "LOG_OTH_PROGRAM_STATE");
   i = LOG_OTH_TRANSITIONS;        log_types[i].group = 3;   strcpy(log_types[i].name, "LOG_OTH_TRANSITIONS");
   i = LOG_OTH_LEVEL_DONE;         log_types[i].group = 3;   strcpy(log_types[i].name, "LOG_OTH_LEVEL_DONE");



   i = LOG_TMR_cpu;                log_types[i].group = 2;   strcpy(log_types[i].name, "LOG_TMR_cpu");
   i = LOG_TMR_rebuild_bitmaps;    log_types[i].group = 2;   strcpy(log_types[i].name, "LOG_TMR_rebuild_bitmaps");
   i = LOG_TMR_move_tot;           log_types[i].group = 2;   strcpy(log_types[i].name, "LOG_TMR_move_tot");
   i = LOG_TMR_move_all;           log_types[i].group = 2;   strcpy(log_types[i].name, "LOG_TMR_move_all");
   i = LOG_TMR_move_enem;          log_types[i].group = 2;   strcpy(log_types[i].name, "LOG_TMR_move_enem");
   i = LOG_TMR_draw_tot;           log_types[i].group = 2;   strcpy(log_types[i].name, "LOG_TMR_draw_tot");
   i = LOG_TMR_draw_all;           log_types[i].group = 2;   strcpy(log_types[i].name, "LOG_TMR_draw_all");
   i = LOG_TMR_bmsg_add;           log_types[i].group = 2;   strcpy(log_types[i].name, "LOG_TMR_bmsg_add");
   i = LOG_TMR_bmsg_draw;          log_types[i].group = 2;   strcpy(log_types[i].name, "LOG_TMR_bmsg_draw");
   i = LOG_TMR_scrn_overlay;       log_types[i].group = 2;   strcpy(log_types[i].name, "LOG_TMR_scrn_overlay");
   i = LOG_TMR_sdif;               log_types[i].group = 2;   strcpy(log_types[i].name, "LOG_TMR_sdif");
   i = LOG_TMR_cdif;               log_types[i].group = 2;   strcpy(log_types[i].name, "LOG_TMR_cdif");
   i = LOG_TMR_rwnd;               log_types[i].group = 2;   strcpy(log_types[i].name, "LOG_TMR_rwnd");
   i = LOG_TMR_client_timer_adj;   log_types[i].group = 2;   strcpy(log_types[i].name, "LOG_TMR_client_timer_adj");
   i = LOG_TMR_client_ping;        log_types[i].group = 2;   strcpy(log_types[i].name, "LOG_TMR_client_ping");
   i = LOG_TMR_proc_rx_buffer;     log_types[i].group = 2;   strcpy(log_types[i].name, "LOG_TMR_proc_rx_buffer");

}



void mwLog::add(int log_type, int sub_type, int p, float v0, float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8, float v9, const char* t1, const char* t2)
{
   if (p == -1) p = mPlayer.active_local_player;

//   int entered = 0;

   char msg[1024];
   sprintf(msg, "%s", "");

   if (log_type == LOG_OTH_LEVEL_DONE)
   {
      if (sub_type == 5) log_append_textf(log_type, "[%4d] Level Done Mode:%d - %s [%d]\n", mLoop.frame_num, sub_type, t1, (int)v0);
      else               log_append_textf(log_type, "[%4d] Level Done Mode:%d - %s\n", mLoop.frame_num, sub_type, t1);
   }


   if (log_type == LOG_OTH_TRANSITIONS)
   {
      if (sub_type == 1) log_add_prefixed_text(LOG_OTH_TRANSITIONS, p, "Transitions: Next level to overworld\n");
      if (sub_type == 2) log_add_prefixed_text(LOG_OTH_TRANSITIONS, p, "Transitions: Next level from overworld\n");
      if (sub_type == 3) log_add_prefixed_text(LOG_OTH_TRANSITIONS, p, "Transitions: Level to level (no overworld)\n");

      if (sub_type == 5) log_add_prefixed_text(LOG_OTH_TRANSITIONS, p, "Transitions: Pre-load\n");
      if (sub_type == 6) log_add_prefixed_text(LOG_OTH_TRANSITIONS, p, "Transitions: Post-load\n");

      if (sub_type == 0)
      {
         int i = (int) v0;
         int f = (int) v1;
         const char* tcn[5] = {"nothing", "game", "menu", "gate"};
         mLog.log_add_prefixed_textf(LOG_OTH_TRANSITIONS, 0, "Transition from %s to %s\n", tcn[i], tcn[f]);
      }


   }


   if (log_type == LOG_OTH_PROGRAM_STATE)
   {
      if (sub_type == 0)
      {
         int s0 = (int) v0;
         int s1 = (int) v1;

         // log_add_prefixed_textf(LOG_OTH_PROGRAM_STATE, 0, "State change from [%2d]:%s to [%2d]:%s  ----  ", s0, mLoop.state_names[s0], s1, mLoop.state_names[s1]);
         // for (int i=0; i<8; i++) log_append_textf(LOG_OTH_PROGRAM_STATE, "%2d ", mLoop.state[i]);
         // log_append_textf(LOG_OTH_PROGRAM_STATE, " --- [qa:%d] [da:%d]\n", mLoop.quit_action, mLoop.done_action);
         // log_add_prefixed_textf(LOG_OTH_PROGRAM_STATE, 0, "------  State Change --------------\n");


         // log_append_textf(LOG_OTH_PROGRAM_STATE, "\n[%d]---------  State Change --------------\n", mLoop.frame_num);
         // log_append_textf(LOG_OTH_PROGRAM_STATE, "From [%2d]:%s\n", s0, mLoop.state_names[s0]);
         // log_append_textf(LOG_OTH_PROGRAM_STATE, "To   [%2d]:%s\n", s1, mLoop.state_names[s1]);
         // for (int i=0; i<8; i++) log_append_textf(LOG_OTH_PROGRAM_STATE, "%2d ", mLoop.state[i]);
         // log_append_textf(LOG_OTH_PROGRAM_STATE, "\n----- [qa:%d] [da:%d]  -----------\n", mLoop.quit_action, mLoop.done_action);

         // log_append_textf(LOG_OTH_PROGRAM_STATE, "\n-------------------  State Change ------------------------\n");
         // log_append_textf(LOG_OTH_PROGRAM_STATE, "[%d] from: [%2d]:%s to: [%2d]:%s\n", mLoop.frame_num, s0, mLoop.state_names[s0], s1, mLoop.state_names[s1]);
         // log_append_textf(LOG_OTH_PROGRAM_STATE, "quit action:%d   done action:%d\n", mLoop.quit_action, mLoop.done_action);
         // log_append_textf(LOG_OTH_PROGRAM_STATE, "----------------------------------------------------------\n");

         log_append_textf(LOG_OTH_PROGRAM_STATE, "\n-------------------  State Change ------------------------\n");
         log_append_textf(LOG_OTH_PROGRAM_STATE, "Frame:[%d]\n", mLoop.frame_num, s0, mLoop.state_names[s0], s1, mLoop.state_names[s1]);
         log_append_textf(LOG_OTH_PROGRAM_STATE, "From: [%2d]-%s\n", s0, mLoop.state_names[s0]);
         log_append_textf(LOG_OTH_PROGRAM_STATE, "To:   [%2d]-%s\n", s1, mLoop.state_names[s1]);
         log_append_textf(LOG_OTH_PROGRAM_STATE, "Quit action:[%d]-%s\n", mLoop.quit_action, mLoop.quit_action_names[mLoop.quit_action]);
         log_append_textf(LOG_OTH_PROGRAM_STATE, "Done action:[%d]-%s\n", mLoop.done_action, mLoop.done_action_names[mLoop.done_action]);
         // log_append_textf(LOG_OTH_PROGRAM_STATE, "History:\n");
         // log_append_textf(LOG_OTH_PROGRAM_STATE, "--------\n");
         // for (int i=0; i<8; i++) log_append_textf(LOG_OTH_PROGRAM_STATE, "[%d][%2d]-%s\n", i, mLoop.state[i], mLoop.state_names[mLoop.state[i]]);
         log_append_textf(LOG_OTH_PROGRAM_STATE, "----------------------------------------------------------\n");
      }


      if (sub_type == 2) log_add_prefixed_text(LOG_OTH_PROGRAM_STATE, 0, "PROGRAM STATE - exit program at level done because done_action = 0");
      if (sub_type == 3) log_add_prefixed_text(LOG_OTH_PROGRAM_STATE, 0, "PROGRAM STATE - instead of menu, go to overworld because quit_action = 2");
      if (sub_type == 4) log_add_prefixed_text(LOG_OTH_PROGRAM_STATE, 0, "PROGRAM STATE - instead of menu, go to settings because quit_action = 3");


   }



   if (log_type == LOG_NET_ENDING_STATS)
   {
      if (sub_type == 0) log_ending_stats_server(log_type);
      if (sub_type == 1) log_ending_stats_client(log_type, p);
   }


   if (log_type == LOG_NET_BANDWIDTH)
   {
      sprintf(msg, "bandwidth txb:[%d] rxb:[%d] txp:[%d] rxp:[%d]\n", (int) v0, (int) v1, (int) v2, (int) v3);
      log_add_prefixed_textf(log_type, p, "%s\n", msg);
   }


   if (log_type == LOG_NET_DIF_REWIND)
   {
      if (sub_type == 0) sprintf(msg, "stdf rewind [%d] [%d]", (int)v0, -(int)v1);
      if (sub_type == 1) sprintf(msg, "stdf rewind [%d] not found - oldest frame not valid", (int)v0);
      if (sub_type == 2) sprintf(msg, "stdf rewind [%d] not found - using oldest frame [%d]", (int)v0, (int)v1);
      log_add_prefixed_textf(log_type, p, "%s\n", msg);
   }


   if (log_type == LOG_NET_DIF_CREATE)
   {
      sprintf(msg, "stdf create:%d", (int) v0);
      log_add_prefixed_textf(log_type, p, "%s\n", msg);
      //add_log_net_db_row(log_type, 0, 0,   "%s",   msg);
   }


   if (log_type == LOG_NET_DIF_TRX)
   {
      if (sub_type == 0) sprintf(msg, "tx stdf p:%d [s:%d d:%d] cmp:%d ratio:%3.2f [%d packets needed]", p, (int)v0, (int)v1, (int)v2, v3, (int)v4);
      if (sub_type == 1) sprintf(msg, "rx dif complete [%d to %d] - uncompressed",   (int)v0, (int)v1);
      if (sub_type == 2) sprintf(msg, "rx dif complete [%d to %d] - bad uncompress", (int)v0, (int)v1);
      log_add_prefixed_textf(log_type, p, "%s\n", msg);
      //add_log_net_db_row(log_type, 0, 0,   "%s",   msg);

   }


   if (log_type == LOG_NET_DIF_TRX_PACKET)
   {
      if (sub_type == 0) sprintf(msg, "tx stdf p:%d piece [%d of %d] [%d to %d] st:%4d sz:%4d slsn:%d", p, (int)v0, (int)v1, (int)v2, (int)v3, (int)v4, (int)v5, (int)v6);
      char msg1[256];
      sprintf(msg1, "rx stdf piece [%d of %d] [%d to %d] st:%4d sz:%4d sdln:%d slsn:%d", (int)v0, (int)v1, (int)v2, (int)v3, (int)v4, (int)v5, (int)v6, (int)v7);
      if (sub_type == 1) sprintf(msg, "%s", msg1);
      if (sub_type == 2) sprintf(msg, "%s  -  slsn is from next level - setting next level:%d", msg1, (int) v6);
      if (sub_type == 3) sprintf(msg, "%s  -  sdln bad!", msg1);

      log_add_prefixed_textf(log_type, p, "%s\n", msg);
      //add_log_net_db_row(log_type, 0, 0,   "%s",   msg);
   }



   if (log_type == LOG_NET_DIF_APPLY)
   {
      char msg1[64];
      sprintf(msg1, "----- Apply dif [%d to %d]", (int)v0, (int)v1);
      if (sub_type == 0) sprintf(msg, "%s [not applied] [dif not valid]", msg1);
      if (sub_type == 1) sprintf(msg, "%s [not applied] [not newer than last dif applied]", msg1);
      if (sub_type == 2) sprintf(msg, "%s [not applied] [no bases found] - resending stak [%d]", msg1, (int)v2);
      if (sub_type == 3) sprintf(msg, "%s [not applied] [base not found] - resending stak [%d]", msg1, (int)v2);
      if (sub_type == 4)
      {
         int mlfn = v3; // mLoop.frame_num
         int ff = v4; // fast forward
         char msg2[64];
         if (ff == 0) sprintf(msg2, "exact frame match [%d]", mlfn);
         if (ff > 0)  sprintf(msg2, "rewound [%d] frames", ff);
         if (ff < 0)
         {
            if (mlfn == 0) sprintf(msg2, "initial state");
            else           sprintf(msg2, "jumped ahead %d frames", -ff);
         }
         sprintf(msg, "%s [applied] [%s]", msg1, msg2);
      }
      log_add_prefixed_textf(log_type, p, "%s\n", msg);
      //add_log_net_db_row(log_type, 0, 0,   "%s",   msg);

   }

   if (log_type == LOG_NET_DIF_ACK)
   {
      if (sub_type == 0) sprintf(msg, "tx stak p:%d ack:[%d] cur:[%d]", (int)v0, (int)v1, (int)v2);
      if (sub_type == 1) sprintf(msg, "rx stak p:%d ack:[%d] cur:[%d] slsn:[%d] - dsync:[%4.1f] chase:[%4.1f]", (int)v0, (int)v1, (int)v2, (int)v3, v4*1000, v5);
      if (sub_type == 2) sprintf(msg, "rx stak p:%d ack:[%d] cur:[%d] slsn:[%d] - wrong slsn, ignoring", (int)v0, (int)v1, (int)v2, (int)v3);
      log_add_prefixed_textf(log_type, p, "%s\n", msg);
//      add_log_net_db_row(log_type, 0, 0,   "%s",   msg);
   }



   if (log_type == LOG_NET_CDAT)
   {
      if (sub_type == 0) sprintf(msg, "tx cdat - move:%d",                                                    (int)v0);
      if (sub_type == 1) sprintf(msg, "rx cdat p:%d fn:[%d] sync:[%d] slsn:[%d] - entered gmep:[%d]",         p, (int)v0, (int)v1, (int)v2, (int)v3);
      if (sub_type == 2) sprintf(msg, "rx cdat p:%d fn:[%d] sync:[%d] slsn:[%d] - late - dropped",            p, (int)v0, (int)v1, (int)v2);
      if (sub_type == 3) sprintf(msg, "rx cdat p:%d fn:[%d] sync:[%d] slsn:[%d] - wrong slsn:[%d] - dropped", p, (int)v0, (int)v1, (int)v2, (int)v3);
      log_add_prefixed_textf(log_type, p, "%s\n", msg);
      //add_log_net_db_row(log_type, 0, 0,   "%s",   msg);
   }



   if (log_type == LOG_NET_TIMER_ADJUST)
   {
      sprintf(msg, "timer adjust dsc[%5.1f] dsa[%5.1f] off[%3.1f] chs[%3.3f]", v0*1000, v1*1000, v2*1000, v3);
      log_add_prefixed_textf(log_type, p, "%s\n", msg);
      //add_log_net_db_row(log_type, 0, 0,   "%s",   msg);
   }




   if (log_type == LOG_NET_CLIENT_PING)
   {
      float ping = v0;
      float ping_avg = v1;

      sprintf(msg, "client ping[%5.1f] avg[%5.1f]\n", ping*1000, ping_avg*1000);
      log_add_prefixed_textf(log_type, p, "%s\n", msg);
      //add_log_net_db_row(log_type, 0, p,   "%s",   msg);
   }









   if (log_type == LOG_NET_FILE_TRANSFER)
   {

      if (sub_type == 1) sprintf(msg, "save:txt:%s", t1); // not saved
      if (sub_type == 2) sprintf(msg, "saved:%s", t1); // saved

      if (sub_type == 4) sprintf(msg, "%s%s", t1, t2); // rx srrf -
      if (sub_type == 5) sprintf(msg, "%s", t1); // rx %s size:[%d] id:[%d]
      if (sub_type == 6) sprintf(msg, "%s", t1); // tx clrf - client request file");

      if (sub_type == 7)  sprintf(msg, "%s", t1);            // rx clrf - client request file");
      if (sub_type == 8)  sprintf(msg, "%s%d", t1, (int)v0); // rx sfak - client acknowledged getting file id:
      if (sub_type == 9)  sprintf(msg, "%s%s", t1, t2);      // starting file transfer - ", files_to_send[i].name);
      if (sub_type == 10) sprintf(msg, "file:%s does not exist", t1);      // file:%s does not exist
      if (sub_type == 11) sprintf(msg, "file:%s is too large - %d > 200k", t1, (int)v0);      // file:%s too large


      log_add_prefixed_textf(log_type, p, "%s\n", msg);
//      add_log_net_db_row(log_type, 0, p,   "%s",   msg);

   }




   if (log_type == LOG_NET)
   {
      if (sub_type == LOG_NET_SUBTYPE_PLAYER_ACTIVE)
      {
         sprintf(msg, "Player:%d became ACTIVE!", p);
         add_header(log_type, p, 0, msg);
      }
      if (sub_type == LOG_NET_SUBTYPE_PLAYER_INACTIVE)
      {
         sprintf(msg, "Player:%d became INACTIVE!", p);
         add_header(log_type, p, 0, msg);
      }
      if (sub_type == LOG_NET_SUBTYPE_PLAYER_DIED)
      {
         sprintf(msg, "Player:%d DIED!", p);
         add_header(log_type, p, 0, msg);
      }

      if (sub_type == LOG_NET_SUBTYPE_NEXT_LEVEL)
      {
         int l = v0;
         sprintf(msg, "NEXT LEVEL:%d", l);
         add_header(log_type, p, 1, msg);
      }

      if (sub_type == LOG_NET_SUBTYPE_LEVEL_STARTED)
      {
         int l = v0;
         sprintf(msg, "LEVEL %d STARTED", l);
         add_header(log_type, p, 1, msg);
      }


      if (sub_type == LOG_NET_SUBTYPE_SERVER_START)
      {
         int error = v0;
         log_versions();
                         add_fw (log_type, -1, 76, 10, "+", "-", "");
                         add_fw (log_type, -1, 76, 10, "|", " ", "Server mode started");
                         add_fwf(log_type, -1, 76, 10, "|", " ", "Server hostname:    [%s]", mLoop.local_hostname);
                         add_fwf(log_type, -1, 76, 10, "|", " ", "Level:              [%d]", mLevel.play_level);
         if (error == 0) add_fw( log_type, -1, 76, 10, "|", " ", "Server network initialized successfully");
         else            add_fw( log_type, -1, 76, 10, "|", " ", "Server network initialization failed");
         if (error == 1) add_fw( log_type, -1, 76, 10, "|", " ", "Failed to initialize network");
         if (error == 2) add_fw( log_type, -1, 76, 10, "|", " ", "Failed to open server channel");
         add_fw( log_type, p, 76, 10, "+", "-", "");

         //add_log_net_db_row(log_type, 0, 0, "Server mode started - hostname:%s", mLoop.local_hostname);
      }

      if (sub_type == LOG_NET_SUBTYPE_SERVER_STOP)
      {
         sprintf(msg, "Shutting down the server network");
         add_header(log_type, p, 0, msg);
         //add_log_net_db_row(log_type, 0, 0, t1);
      }


      if (sub_type == LOG_NET_SUBTYPE_SERVER_CJON)
      {
         int full = v0;
         int requested_color = v1;
         int color = v2;
         int lev = v3;
         int cp = v4;
         int server_frame = v5;
         int slsn = v6;

         add_fwf(log_type, p, 76, 10, "+", "-", "");
         add_fwf(log_type, p, 76, 10, "|", " ",        "Server received join request from %s at %s (color:%d)", t1, t2, requested_color);
         add_log_net_db_row(log_type, sub_type, p,     "Server received join request from %s at %s (color:%d)", t1, t2, requested_color);

         if (full)
         {
            add_fwf(log_type, p, 76, 10, "|", " ",    "Reply sent: 'SERVER FULL'");
            add_log_net_db_row(log_type, sub_type, p,  "Reply sent: 'SERVER FULL'");
         }
         else
         {
            add_fwf(log_type, p, 76, 10, "|", " ", "Server replied with join invitation:");
            add_fwf(log_type, p, 76, 10, "|", " ", "Level:%d", lev);
            add_fwf(log_type, p, 76, 10, "|", " ", "Player Number:%d", cp);
            add_fwf(log_type, p, 76, 10, "|", " ", "Player Color:%d", color);
            add_fwf(log_type, p, 76, 10, "|", " ", "Server Frame:%d", server_frame);
            add_fwf(log_type, p, 76, 10, "|", " ", "Server Level Sequence Num:%d", slsn);
            add_fwf(log_type, p, 76, 10, "+", "-", "");

            add_log_net_db_row(log_type, sub_type, p,       "Server replied with join invitation:");
            add_log_net_db_row(log_type, sub_type, p,       "Level:[%d] Frame:[%d] SLSN:%d", lev, server_frame, slsn);
            add_log_net_db_row(log_type, sub_type, p,       "Player Number:[%d] Player Color:%d", cp, color);
         }
      }


      if (sub_type == LOG_NET_SUBTYPE_CLIENT_START) // Client mode started on localhost:[%s]
      {
         sprintf(msg, "Client mode started on localhost:%s", t1);
         add_fw(log_type, p, 76, 10, "+", "-", "");
         add_fw(log_type, p, 76, 10, "|", " ", msg);
      }

      if (sub_type == LOG_NET_SUBTYPE_CLIENT_INIT)
      {
         log_versions();

         int error = v0;
         if (error == 0)
         {
            add_fwf(log_type, p, 76, 10, "|", " ", "Client network initialized");
            add_fwf(log_type, p, 76, 10, "|", " ", "Local address:[%s]", t2);
            add_fwf(log_type, p, 76, 10, "|", " ", "Server target:[%s]", t1);
         }
         if (error == 1) add_fwf(log_type, p, 76, 10, "|", " ", "Failed to initialize network");
         if (error == 2) add_fwf(log_type, p, 76, 10, "|", " ", "Failed to create NetChannel");
         if (error == 3) add_fwf(log_type, p, 76, 10, "|", " ", "Failed to set NetChannel target:[%s]", t1);
      }

      if (sub_type == LOG_NET_SUBTYPE_CLIENT_INIT)
      {
         sprintf(msg, "Sent 'cjon' packet to server, waiting for reply...");
         add_fw(log_type, p, 76, 10, "|", " ", msg);
      }

      if (sub_type == LOG_NET_SUBTYPE_CLIENT_WAIT)
      {
         if (v1 == 1) add_fw(log_type, p, 76, 10, "|", " ", "no reply from server");
         if (v1 == 2) add_fw(log_type, p, 76, 10, "|", " ", "'server full' reply from server");
         if (v1 == 3) add_fw(log_type, p, 76, 10, "|", " ", "cancelled");
         add_fw(log_type, p, 76, 10, "+", "-", "");
      }

      if (sub_type == LOG_NET_SUBTYPE_CLIENT_SJON)
      {
         int lev = v0;
         int color = v1;
         int sfnum = v2;
         int slsn = v3;

         add_fwf(log_type,  p, 76, 10, "|", " ", "Client received join invitation from server");
         add_fwf(log_type,  p, 76, 10, "|", " ", "Level:[%d]", lev);
         add_fwf(log_type,  p, 76, 10, "|", " ", "Player Number:[%d]", p);
         add_fwf(log_type,  p, 76, 10, "|", " ", "Player Color:[%d]", color);
         add_fwf(log_type,  p, 76, 10, "|", " ", "Server Frame Num:[%d]", sfnum);
         add_fwf(log_type,  p, 76, 10, "|", " ", "Server Level Sequence Num:[%d]", slsn);
         add_fwf(log_type,  p, 76, 10, "+", "-", "");

         add_log_net_db_row(log_type, 0, p, "Client received join invitation from server");
         add_log_net_db_row(log_type, 0, p, "Player Number:[%d] - Player Color:[%d]", p, color);
         add_log_net_db_row(log_type, 0, p, "Lev:[%d] Frame:[%d] slsn:[%d]", lev, sfnum, slsn);
      }

      if (sub_type == LOG_NET_SUBTYPE_CLIENT_STOP)
      {
         sprintf(msg, "Shutting down the client network");
         add_header(log_type, p, 0, msg);
      }


      if (sub_type == LOG_NET_SUBTYPE_SERVER_CJRC)
      {
         add_fwf(log_type, p, 76, 10, "|", " ", "Server received remote control request");
         add_fwf(log_type, p, 76, 10, "|", " ", "Server replied with sjrc packet");
      }

      if (sub_type == LOG_NET_SUBTYPE_SERVER_DROP)
      {
         int server_last_stak_rx_frame_num = v0;
         add_headerf(log_type, p, 1,        "Server dropped player:%d (last stak rx:%d)", p, server_last_stak_rx_frame_num);
         //add_log_net_db_row(log_type, 0, p,  "Server dropped player:%d (last stak rx:%d)", p, server_last_stak_rx_frame_num);
      }


      if (sub_type == LOG_NET_SUBTYPE_CLIENT_DROP)
      {
         int last_dif_applied = v0;
         add_fwf(log_type, p, 76, 10, "+", "-", "");
         add_fwf(log_type, p, 76, 10, "|", " ", "Local Client Player %d Lost Server Connection!", p);
         add_fwf(log_type, p, 76, 10, "|", " ", "last_dif_applied:[%d]", last_dif_applied);
         add_fwf(log_type, p, 76, 10, "+", "-", "");
         //add_log_net_db_row(log_type, 0, 0, "Lost Server Connection! - last dif applied:[%d]", last_dif_applied);
      }


      if (sub_type == LOG_NET_SUBTYPE_SERVER_RELOAD)
      {
         if (v0 == 0) sprintf(msg, "Headless Server with no clients! - Reload");
         if (v0 == 1) sprintf(msg, "Server Approaching %d Game Moves! - Reload", (int) v1);
         if (v0 == 2) sprintf(msg, "Server Approaching %d Frames! - Reload", (int) v1);
         add_header(log_type, p, 1, msg);
         add_log_net_db_row(log_type, sub_type, p, msg);
      }

      if (sub_type == LOG_NET_SUBTYPE_PACKET_BUF_FULL)
      {
         log_add_prefixed_textf(log_type, p, "rx packet buffer full\n");
         log_add_prefixed_textf(log_type, p, "[%d] cdat\n", v0);
         log_add_prefixed_textf(log_type, p, "[%d] stak\n", v1);
         log_add_prefixed_textf(log_type, p, "[%d] rctl\n", v2);
         log_add_prefixed_textf(log_type, p, "[%d] stdf\n", v3);
         log_add_prefixed_textf(log_type, p, "[%d] snfo\n", v4);
         log_add_prefixed_textf(log_type, p, "[%d] sfil\n", v5);
         log_add_prefixed_textf(log_type, p, "[%d] srrf\n", v6);
         log_add_prefixed_textf(log_type, p, "[%d] crfl\n", v7);
         log_add_prefixed_textf(log_type, p, "[%d] all \n", v8);
      }
   }




   // if (!entered)
   // {
   //    add_header(log_type, p, 0, msg);
   //    add_log_net_db_row(log_type, sub_type, p, "%s", msg);
   // }
   //





}
   

   








void mwLog::clear_all_log_actions()
{
   for (int i=0; i<100; i++)
      if (log_types[i].group != 9) // except for errors
         log_types[i].action = 0;

   autosave_log_on_level_done = 0;
   autosave_log_on_level_quit = 0;
   autosave_log_on_program_exit = 0;

   mConfig.save_config();
}



void mwLog::flush_logs()
{
   save_log_file();
   save_log_net_file();
   save_log_status_file();
}



void mwLog::erase_log()
{
   log_msg[0] = 0;
   log_msg_pos = 0;
}

void mwLog::erase_log_net()
{
   log_net_msg[0] = 0;
   log_net_msg_pos = 0;
   log_net_msg_num_lines = 0;
}

void mwLog::erase_log_status()
{
   log_status_msg[0] = 0;
   log_status_msg_pos = 0;
   log_status_msg_num_lines = 0;
}


void mwLog::save_log_file()
{
   if (strlen(log_msg) > 0)
   {
      al_make_directory("logs"); // create if not already created

      // get timestamp
      char timestamp[256];
      time_t now = time(NULL);
      struct tm *timenow = localtime(&now);
      strftime(timestamp, sizeof(timestamp), "%Y%m%d-%H%M%S", timenow);

      // get hostname and limit to 16 char
      std::string hostnameFull = mLoop.local_hostname;
      std::string hostname16 = hostnameFull.substr(0, std::min(hostnameFull.length(), (size_t) 16));

      // build filename string
      std::string filename = "logs/" + std::string(timestamp) + "-[" + std::to_string(mLevel.play_level) + "][" + hostname16 + "].txt";

      FILE *filepntr = fopen(filename.c_str(),"w");
      fprintf(filepntr, "%s", log_msg);
      fclose(filepntr);

      printf("%s saved \n", filename.c_str());
      erase_log();
   }
}



void mwLog::save_log_net_file()
{
   if (strlen(log_net_msg) > 0)
   {
      al_make_directory("logs/net"); // create if not already created

      char filename[256];
      time_t now = time(NULL);
      struct tm *timenow = localtime(&now);
      strftime(filename, sizeof(filename), "logs/net/%Y%m%d-%H%M%S", timenow);

      auto currentDateTime = std::chrono::system_clock::now();
      int ms = std::chrono::time_point_cast<std::chrono::milliseconds>(currentDateTime).time_since_epoch().count() % 1000;

      char sms[64];
      sprintf(sms, ".%d.csv", ms);

      strcat(filename, sms);

      FILE *filepntr = fopen(filename,"w");
      fprintf(filepntr, "%s", log_net_msg);
      fclose(filepntr);

      printf("%s saved \n", filename);
      erase_log_net();
   }
}


void mwLog::save_log_status_file()
{
   if (strlen(log_status_msg) > 0)
   {
      al_make_directory("logs/status"); // create if not already created

      char filename[256];
      time_t now = time(NULL);
      struct tm *timenow = localtime(&now);
      strftime(filename, sizeof(filename), "logs/status/%Y%m%d-%H%M%S", timenow);

      auto currentDateTime = std::chrono::system_clock::now();
      int ms = std::chrono::time_point_cast<std::chrono::milliseconds>(currentDateTime).time_since_epoch().count() % 1000;

      char sms[64];
      sprintf(sms, ".%d.csv", ms);

      strcat(filename, sms);

      FILE *filepntr = fopen(filename,"w");
      fprintf(filepntr, "%s", log_status_msg);
      fclose(filepntr);

      printf("%s saved \n", filename);
//      printf("%s\n", log_status_msg);
      erase_log_status();
   }
}


void mwLog::log_error(const char *txt, bool dialog)
{
   char msg[256];
   sprintf(msg, "Error: %s", txt);
   add_fwf(LOG_error, 0, 76, 10, "|", "-", msg);
   if (dialog) mInput.m_err(txt);
}


void mwLog::log_append_text_to_db(int type, const char *txt)
{
//    if (mSql.db_logs == NULL)
//    {
//       printf("Error! Cannot insert log into database. Database not open\n");
//       printf("log:%s\n", txt);
//       return;
//    }
//    char sql[1024];
//
// //   double agt = al_get_time();
//    double agt = 0;
//
//    char ts[256];
//    sprintf(ts, "%s", mMiscFnx.get_timestamp());
//    sprintf(sql, "INSERT INTO logs ( message, created, agt ) VALUES('%s', '%s', %f)" , txt, ts, agt);
//    printf("sql:%s\n", sql);
//    mSql.execute_sql(sql, mSql.db_logs);
}


// appends passed text string to log array
// this is only function that actually prints to console or adds to log array
// also checks if log array is full and flushes to disk
void mwLog::log_append_text(int type, const char *txt)
{
   // add to db...

   // just add created, frame, agt, and text line

   // log_append_text_to_db(type, txt);




   if (log_types[type].action & LOG_ACTION_PRINT) printf("%s", txt);
   if (log_types[type].action & LOG_ACTION_LOG)
   {
      if ((log_msg_pos + strlen(txt)) >= NUM_LOG_CHAR)
      {
         printf("log array full, > %d char ... saving\n", NUM_LOG_CHAR);
         save_log_file();
      }
      memcpy(log_msg + log_msg_pos, txt, strlen(txt));
      log_msg_pos += strlen(txt);
      log_msg[log_msg_pos] = 0; // NULL terminate
   }
}

// wrapper for 'log_append_text'  that takes a printf style format
void mwLog::log_append_textf(int type, const char *format, ...)
{
   char smsg[1000];
   va_list args;
   va_start(args, format);
   vsprintf(smsg, format, args);
   va_end(args);
   log_append_text(type, smsg);
}

// adds text string with prefix
// [%2d][%d][%d]%s", type, player, mLoop.frame_num, txt);
void mwLog::log_add_prefixed_text(int type, int player, const char *txt)
{
   if (player == -1) player = mPlayer.active_local_player;
   log_append_textf(type, "[%2d][%d][%d]%s", type, player, mLoop.frame_num, txt);
}



// wrapper for 'log_add_prefixed_text' that takes a printf style format
void mwLog::log_add_prefixed_textf(int type, int player, const char *format, ...)
{
   if (player == -1) player = mPlayer.active_local_player;
   char smsg[1000];
   va_list args;
   va_start(args, format);
   vsprintf(smsg, format, args);
   va_end(args);
   log_add_prefixed_text(type, player, smsg);
}







// adds text string with profile timer prefix prefix
// [%2d][%d][%d]tmst %s", type, player, mLoop.frame_num, txt);
void mwLog::add_tmr(int type, const char *txt)
{
   log_append_textf(type, "[%2d][%d][%d]tmst %s", LOG_TMR, mPlayer.active_local_player, mLoop.frame_num, txt);
}

// wrapper for 'add_tmr' that takes a printf style format
void mwLog::add_tmrf(int type, const char *format, ...)
{
   char msg[500];
   va_list args;
   va_start(args, format);
   vsprintf(msg, format, args);
   va_end(args);
   add_tmr(type, msg);
}


// adds a single profile timer log entry
void mwLog::add_tmr1(int type, const char *tag, double dt)
{
   char msg[500];
   sprintf(msg, "%s:[%0.4f]\n", tag, dt*1000);
   add_tmr(type, msg);
}
//
// // adds a single profile timer log entry and optionally terminate LF
// void mwLog::add_tmr2(int type, const char *tag, double dt)
// {
//    char msg[500];
//    sprintf(msg, "%s:[%0.4f]\n", tag, dt*1000);
//    add_tmr(type, msg);
// }
//














































// adds fixed width formatted printf style text string
void mwLog::add_fwf(int type, int player, int width, int pos, const char *border, const char *fill, const char *format, ...)
{
   if (player == -1) player = mPlayer.active_local_player;

   char smsg[200];
   va_list args;
   va_start(args, format);
   vsprintf(smsg, format, args);
   va_end(args);



   add_fw(type, player, width, pos, border, fill, smsg);
}

// adds fixed width formatted text string
void mwLog::add_fw(int type, int player, int width, int pos, const char *border, const char *fill, const char *txt)
{
   if (player == -1) player = mPlayer.active_local_player;
   int l = strlen(txt);
   int j1 = pos-2;
   int j2 = width - l - pos;
   char ftxt[200];

   if (pos > 1)
   {
      strcpy(ftxt, border);
      for (int i=0; i<j1; i++) strcat(ftxt, fill);
      strcat(ftxt, txt);
      for (int i=0; i<j2; i++) strcat(ftxt, fill);
      strcat(ftxt, border);
      strcat(ftxt, "\n");
   }

   if (pos == 1)
   {
      j2--;
      strcpy(ftxt, border);
      for (int i=0; i<j1; i++) strcat(ftxt, fill);
      strcat(ftxt, txt);
      for (int i=0; i<j2; i++) strcat(ftxt, fill);
      strcat(ftxt, border);
      strcat(ftxt, "\n");
   }

   if (pos == 0)
   {
      strcpy(ftxt, "");
      j2--;
      for (int i=0; i<j1; i++) strcat(ftxt, fill);
      strcat(ftxt, txt);
      for (int i=0; i<j2; i++) strcat(ftxt, fill);
      strcat(ftxt, border);
      strcat(ftxt, "\n");
   }
   log_add_prefixed_text(type, player, ftxt);
}


void mwLog::add_headerf(int type, int player, int blank_lines, const char *format, ...)
{
   if (player == -1) player = mPlayer.active_local_player;
   char smsg[200];
   va_list args;
   va_start(args, format);
   vsprintf(smsg, format, args);
   va_end(args);
   add_header(type, player, blank_lines, smsg);
}

void mwLog::add_header(int type, int player, int blank_lines, const char *txt)
{
   if (player == -1) player = mPlayer.active_local_player;
   add_fw(type, player, 76, 10, "+", "-", "");
   for (int i=0; i<blank_lines; i++) add_fw(type, player, 76, 10, "|", " ", "");
   add_fw(type, player, 76, (76 - strlen(txt))/2, "|", " ", txt);
   for (int i=0; i<blank_lines; i++) add_fw(type, player, 76, 10, "|", " ", "");
   add_fw(type, player, 76, 10, "+", "-", "");
}

void mwLog::log_time_date_stamp()
{
   char tmsg[80];
   struct tm *timenow;
   time_t now = time(NULL);
   timenow = localtime(&now);
   strftime(tmsg, sizeof(tmsg), "%Y-%m-%d  %H:%M:%S", timenow);
   add_fwf(10, 0, 76, 10, "|", " ", "Date and time: %s",tmsg);
}

void mwLog::log_versions()
{
   add_fw (10, 0, 76, 10, "+", "-", "");
   add_fwf(10, 0, 76, 10, "|", " ", "Purple Martians Version %s", mLoop.pm_version_string);
   add_fw (10, 0, 76, 10, "|", " ", mLoop.al_version_string);
   log_time_date_stamp();
   add_fw (10, 0, 76, 10, "+", "-", "");
}



void mwLog::log_ending_stats_client(int type, int p)
{
   if (p == -1) p = mPlayer.active_local_player;
   add_headerf(type, p, 0, "Client %d (%s) ending stats", p, mPlayer.loc[p].hostname);

   add_fwf(type, p, 76, 10, "|", " ", "total game frames.........[%d]", mLoop.frame_num);
   add_fwf(type, p, 76, 10, "|", " ", "frame when client joined..[%d]", mPlayer.loc[p].join_frame);

   if (mPlayer.loc[p].quit_frame == 0) mPlayer.loc[p].quit_frame = mLoop.frame_num;
   add_fwf(type, p, 76, 10, "|", " ", "frame when client quit....[%d]", mPlayer.loc[p].quit_frame);

   log_reason_for_player_quit(type, p);

   add_fwf(type, p, 76, 10, "|", " ", "frames client was active..[%d]", mPlayer.loc[p].quit_frame - mPlayer.loc[p].join_frame);
   add_fwf(type, p, 76, 10, "|", " ", "cdat packets total........[%d]", mPlayer.loc[p].client_cdat_packets_tx);
   add_fwf(type, p, 76, 10, "|", " ", "cdat packets late.........[%d]", mPlayer.syn[p].late_cdats);

   log_bandwidth_stats(type, p);
   add_fwf(type, p, 76, 10, "+", "-", "");
}


void mwLog::log_ending_stats_server(int type)
{
   add_headerf(type, 0, 0, "Server (%s) ending stats", mLoop.local_hostname);

   add_fw (type, 0, 76, 10, "+", "-", "");
   add_fwf(type, 0, 76, 10, "|", " ", "level.....................[%d]", mLevel.play_level);
   add_fwf(type, 0, 76, 10, "|", " ", "total frames..............[%d]", mLoop.frame_num);
   add_fwf(type, 0, 76, 10, "|", " ", "total moves...............[%d]", mGameMoves.entry_pos);
   add_fwf(type, 0, 76, 10, "|", " ", "total time (seconds)......[%d]", mLoop.frame_num/40);
   add_fwf(type, 0, 76, 10, "|", " ", "total time (minutes)......[%d]", mLoop.frame_num/40/60);
   log_bandwidth_stats(type, 0);
   add_fw (type, 0, 76, 10, "+", "-", "");

   for (int p=1; p<NUM_PLAYERS; p++)
   {
      if ((mPlayer.syn[p].control_method == PM_PLAYER_CONTROL_METHOD_NETGAME_REMOTE) || (mPlayer.syn[p].control_method == PM_PLAYER_CONTROL_METHOD_CLIENT_ORPHAN))
      {
         add_fwf(type, 0, 76, 10, "|", " ", "Player:%d (%s)", p, mPlayer.loc[p].hostname);
         add_fwf(type, 0, 76, 10, "|", " ", "frame when client joined..[%d]", mPlayer.loc[p].join_frame);

         if (mPlayer.loc[p].quit_frame == 0) mPlayer.loc[p].quit_frame = mLoop.frame_num;
         add_fwf(type, 0, 76, 10, "|", " ", "frame when client quit....[%d]", mPlayer.loc[p].quit_frame);

         log_reason_for_player_quit(type, p);

         add_fwf(type, 0, 76, 10, "|", " ", "frames client was active..[%d]", mPlayer.loc[p].quit_frame - mPlayer.loc[p].join_frame);
         add_fwf(type, 0, 76, 10, "|", " ", "cdat packets total........[%d]", mPlayer.loc[p].client_cdat_packets_tx);
         add_fwf(type, 0, 76, 10, "|", " ", "cdat packets late.........[%d]", mPlayer.syn[p].late_cdats);

         log_bandwidth_stats(type, p);

         add_fw(type, 0, 76, 10, "+", "-", "");
         add_fw(type, 0, 76, 10, "+", "-", "");
      }
   }
   log_player_array(type);
}


void mwLog::log_player_array(int type)
{
   char msg[1024];
   add_header(type, 0, 0, "Player Array");

   add_fw(type, 0, 76, 10, "|", " ", "[p][wh][a][co][m]");

   for (int p=0; p<NUM_PLAYERS; p++)
   {
      char ms[80];
      sprintf(ms, " ");

      if ((mPlayer.syn[p].active) && (mPlayer.syn[p].control_method == PM_PLAYER_CONTROL_METHOD_NETGAME_REMOTE))
         sprintf(ms, " <-- active client");

      if ((!mPlayer.syn[p].active) && (mPlayer.syn[p].control_method == PM_PLAYER_CONTROL_METHOD_NETGAME_REMOTE))
         sprintf(ms, " <-- syncing client");

      if (p == mPlayer.active_local_player) sprintf(ms, " <-- active local player (me!)");
      if (p == 0) sprintf(ms, " <-- server");

      sprintf(msg, "[%d][%d][%2d][%d] - %s %s",
                                              p,
                                              mPlayer.syn[p].active,
                                              mPlayer.syn[p].color,
                                              mPlayer.syn[p].control_method,
                                              mPlayer.loc[p].hostname,
                                              ms );
      add_fw(type, 0, 76, 10, "|", " ", msg);
   }
   add_fw(type, 0, 76, 10, "+", "-", "");
}
/*
void mwLog::log_player_array2(int type)
{
   add_fw(type, 0, 76, 10, "|", " ", "[p][a][m][sy]");
   for (int p=0; p<NUM_PLAYERS; p++)
   {
      float sy = mPlayer.loc[p].pdsync;
      if (p == 0) sy = 0;
      add_fwf(type, 0, 76, 10, "|", " ", "[%d][%d][%d][%3.2f]",   p, mPlayer.syn[p].active, mPlayer.syn[p].control_method, sy );
   }
}

void mwLog::log_player_array3(int type)
{
   add_fw(    type, 0, 76, 10, "|", " ", "[p][a][m][co][pa][pt][pm][pc]");
   for (int p=0; p<NUM_PLAYERS; p++)
      add_fwf(type, 0, 76, 10, "|", " ", "[%d][%d][%d][%2d][%2d][%2d][%2d][%2d]",
                                           p,
                                               mPlayer.syn[p].active,
                                                   mPlayer.syn[p].control_method,
                                                       mPlayer.syn[p].color,
                                                            mPlayer.syn[p].paused,
                                                                 mPlayer.syn[p].paused_type,
                                                                      mPlayer.syn[p].paused_mode,
                                                                           mPlayer.syn[p].paused_mode_count );
}

*/

void mwLog::log_reason_for_player_quit(int type, int p)
{
   if (p == -1) p = mPlayer.active_local_player;
   char tmsg[80];
   sprintf(tmsg,"unknown");
   int r = mPlayer.loc[p].quit_reason;
   if (r == PM_PLAYER_QUIT_REASON_MENU_KEY)                      sprintf(tmsg,"player pressed ESC or menu key");
   if (r == PM_PLAYER_QUIT_REASON_CLIENT_LOST_SERVER_CONNECTION) sprintf(tmsg,"client lost server connection");
   if (r == PM_PLAYER_QUIT_REASON_CLIENT_ENDED_GAME)             sprintf(tmsg,"client ended game");
   if (r == PM_PLAYER_QUIT_REASON_SERVER_ENDED_GAME)             sprintf(tmsg,"server ended game");
   add_fwf(type, p, 76, 10, "|", " ", "reason for quit...........[%s]", tmsg);
}

void mwLog::log_bandwidth_stats(int type, int p)
{
   if (p == -1) p = mPlayer.active_local_player;
   add_fwf(type, p, 76, 10, "|", " ", "total tx bytes............[%d]", mPlayer.loc[p].tx_total_bytes);
   add_fwf(type, p, 76, 10, "|", " ", "max tx bytes per frame....[%d]", mPlayer.loc[p].tx_max_bytes_per_frame);
   add_fwf(type, p, 76, 10, "|", " ", "avg tx bytes per frame....[%d]", mPlayer.loc[p].tx_total_bytes / mLoop.frame_num);
   add_fwf(type, p, 76, 10, "|", " ", "max rx bytes per second...[%d]", mPlayer.loc[p].tx_max_bytes_per_tally);
   add_fwf(type, p, 76, 10, "|", " ", "avg tx bytes per sec......[%d]", (mPlayer.loc[p].tx_total_bytes *40)/ mLoop.frame_num);
   add_fwf(type, p, 76, 10, "|", " ", "total tx packets..........[%d]", mPlayer.loc[p].tx_total_packets);
   add_fwf(type, p, 76, 10, "|", " ", "max tx packets per frame..[%d]", mPlayer.loc[p].tx_max_packets_per_frame);
   add_fwf(type, p, 76, 10, "|", " ", "max tx packets per second.[%d]", mPlayer.loc[p].tx_max_packets_per_tally);
   add_fwf(type, p, 76, 10, "|", " ", "total rx bytes............[%d]", mPlayer.loc[p].rx_total_bytes);
   add_fwf(type, p, 76, 10, "|", " ", "max rx bytes per frame....[%d]", mPlayer.loc[p].rx_max_bytes_per_frame);
   add_fwf(type, p, 76, 10, "|", " ", "avg rx bytes per frame....[%d]", mPlayer.loc[p].rx_total_bytes / mLoop.frame_num);
   add_fwf(type, p, 76, 10, "|", " ", "max rx bytes per second...[%d]", mPlayer.loc[p].rx_max_bytes_per_tally);
   add_fwf(type, p, 76, 10, "|", " ", "avg rx bytes per sec......[%d]", (mPlayer.loc[p].rx_total_bytes *40)/ mLoop.frame_num);
   add_fwf(type, p, 76, 10, "|", " ", "total rx packets..........[%d]", mPlayer.loc[p].rx_total_packets);
   add_fwf(type, p, 76, 10, "|", " ", "max rx packets per frame..[%d]", mPlayer.loc[p].rx_max_packets_per_frame);
   add_fwf(type, p, 76, 10, "|", " ", "max rx packets per second.[%d]", mPlayer.loc[p].rx_max_packets_per_tally);
}




// this will be called directly from all the places I want to log from
// server will add directly and client will send packet
void mwLog::add_log_net_db_row(int type, int sub_type, int client, const char *format, ...)
{
   if (!mLog.log_types[LOG_NET_CSV].action) return;

   // skip these message types if server with no clients
   if ((mNetgame.ima_server) && (mNetgame.server_num_clients == 0))
   {
      if (type == LOG_NET_DIF_REWIND) return;
      if (type == LOG_NET_DIF_CREATE) return;
   }

   if ((mNetgame.ima_server) || (mNetgame.ima_client))
   {
      double agt = al_get_time();
      int f = mLoop.frame_num;
      int p = mPlayer.active_local_player;

      // build the actual message from variable args
      char smsg[800];
      va_list args;
      va_start(args, format);
      vsprintf(smsg, format, args);
      va_end(args);

      if (mNetgame.ima_server) add_log_net_db_row2(type, sub_type, agt, f, p, client, smsg);

      // I don't need to send player num or client, server will know who it came from
      if (mNetgame.ima_client) mNetgame.client_send_clog_packet(type, sub_type, f, agt, smsg);
   }
}


// actually adds to char array
// called both by server directly and when server rx's clog
void mwLog::add_log_net_db_row2(int type, int sub_type, double agt, int f, int p, int client, const char* msg)
{
   if (!mLog.log_types[LOG_NET_CSV].action) return;


   std::string ts = mMiscFnx.timestamp_UTC_ISO8601();

//   char d[100];
//   mMiscFnx.chr_dt(d);

   char txt[1000];
   sprintf(txt, "%d,%d,%s,%f,%d,%d,%d,%s\n", type, sub_type, ts.c_str(), agt, f, p, client, msg);
//         printf("%s", txt);

   int size_limit = NUM_LOG_CHAR;
   if ((log_net_msg_pos + (int)strlen(txt)) >= size_limit)
   {
      printf("log_net array size > %d char ... dumping to file\n", size_limit);
      save_log_net_file();
   }

   int line_limit = 1000;
   if (log_net_msg_num_lines > line_limit)
   {
      printf("log_net array lines > %d ... dumping to file\n", line_limit);
      save_log_net_file();
   }

   memcpy(log_net_msg + log_net_msg_pos, txt, strlen(txt));
   log_net_msg_pos += strlen(txt);
   log_net_msg[log_net_msg_pos] = 0; // NULL terminate
   log_net_msg_num_lines++;
}

void mwLog::add_log_status_db_rows()
{
   if (!mLog.log_types[LOG_NET_CSV].action) return;

   // do not log status unless at least one client is connected
   if (mNetgame.server_num_clients == 0) return;

   std::string ts = mMiscFnx.timestamp_UTC_ISO8601();


//   char d[100];
//   mMiscFnx.chr_dt(d);

   int f = mLoop.frame_num;

   char txt[1000];

   for (int p=0; p<NUM_PLAYERS; p++)
      if (mPlayer.syn[p].active)
      {
         int r = mPlayer.loc[p].rewind;
         float cpu  = mPlayer.loc[p].cpu;
         float sync = mPlayer.loc[p].pdsync*1000;
         float ping = mPlayer.loc[p].ping*1000;
         float difs = mPlayer.loc[p].cmp_dif_size;
         float lcor = mPlayer.loc[p].client_loc_plr_cor;
         float rcor = mPlayer.loc[p].client_rmt_plr_cor;

         float txbf = mPlayer.loc[p].tx_current_bytes_for_this_frame;
         float rxbf = mPlayer.loc[p].rx_current_bytes_for_this_frame;
         float txpf = mPlayer.loc[p].tx_current_packets_for_this_frame;
         float rxpf = mPlayer.loc[p].rx_current_packets_for_this_frame;

         // these values have no meaning for server
         if (p == 0)
         {
            sync = 0;
            ping = 0;
            difs = 0;
            lcor = 0;
            rcor = 0;
         }

         sprintf(txt, "%s,%d,%d,%d,%3.1f,%3.1f,%3.1f,%3.0f,%3.1f,%3.1f,%3.0f,%3.0f,%3.0f,%3.0f\n",
                       ts.c_str(), f, p, r, cpu,  sync, ping, difs, lcor, rcor, txbf, rxbf, txpf, rxpf);

         // printf("added log_status line: %d  %s\n", log_status_msg_num_lines,  txt);

         int size_limit = NUM_LOG_CHAR;
         if ((log_status_msg_pos + (int)strlen(txt)) >= size_limit)
         {
            printf("log_status array size > %d char ... dumping to file\n", size_limit);
            save_log_status_file();
         }

         int line_limit = 1000; // 25 seconds
         if (log_status_msg_num_lines > line_limit)
         {
            printf("log status array lines > %d ... dumping to file\n", line_limit);
            save_log_status_file();
         }

         memcpy(log_status_msg + log_status_msg_pos, txt, strlen(txt));
         log_status_msg_pos += strlen(txt);
         log_status_msg[log_status_msg_pos] = 0; // NULL terminate
         log_status_msg_num_lines++;
      }
}
