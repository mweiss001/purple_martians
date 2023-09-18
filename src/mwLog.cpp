// mwLog.cpp

#include "pm.h"
#include "mwLog.h"
#include "mwPlayer.h"
#include "mwGameMoves.h"
#include "mwLoop.h"
#include "mwLevel.h"
#include "mwInput.h"

#include "mwConfig.h"


mwLog mLog;

mwLog::mwLog()
{
   // init_log_types(); // now only done when recreating settings.pm
}

void mwLog::init_log_types(void)
{
   //printf("init log types\n");
   int i;
   for (i=0; i<100; i++)
   {
      log_types[i].group = 0;
      log_types[i].action = 0;
      strcpy(log_types[i].name, "");
   }

   i = LOG_error;                   log_types[i].group = 9;   strcpy(log_types[i].name, "LOG_error"); log_types[i].action = 7;   // always all three actions

   i = LOG_NET_network_setup;       log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_network_setup");
   i = LOG_NET_join_details;        log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_join_details");
   i = LOG_NET_ending_stats;        log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_ending_stats");
   i = LOG_NET_bandwidth;           log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_bandwidth");
   i = LOG_NET_player_array;        log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_player_array");
   i = LOG_NET_stdf;                log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_stdf");
   i = LOG_NET_stdf_packets;        log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_stdf_packets");
   i = LOG_NET_dif_applied;         log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_dif_applied");
   i = LOG_NET_dif_not_applied;     log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_dif_not_applied");
   i = LOG_NET_stak;                log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_stak");
   i = LOG_NET_cdat;                log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_cdat");
   i = LOG_NET_client_ping;         log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_client_ping");
   i = LOG_NET_timer_adjust;        log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_timer_adjust");

   i = LOG_TMR_cpu;                 log_types[i].group = 2;   strcpy(log_types[i].name, "LOG_TMR_cpu");
   i = LOG_TMR_rebuild_bitmaps;     log_types[i].group = 2;   strcpy(log_types[i].name, "LOG_TMR_rebuild_bitmaps");
   i = LOG_TMR_move_tot;            log_types[i].group = 2;   strcpy(log_types[i].name, "LOG_TMR_move_tot");
   i = LOG_TMR_move_all;            log_types[i].group = 2;   strcpy(log_types[i].name, "LOG_TMR_move_all");
   i = LOG_TMR_move_enem;           log_types[i].group = 2;   strcpy(log_types[i].name, "LOG_TMR_move_enem");
   i = LOG_TMR_draw_tot;            log_types[i].group = 2;   strcpy(log_types[i].name, "LOG_TMR_draw_tot");
   i = LOG_TMR_draw_all;            log_types[i].group = 2;   strcpy(log_types[i].name, "LOG_TMR_draw_all");
   i = LOG_TMR_bmsg_add;            log_types[i].group = 2;   strcpy(log_types[i].name, "LOG_TMR_bmsg_add");
   i = LOG_TMR_bmsg_draw;           log_types[i].group = 2;   strcpy(log_types[i].name, "LOG_TMR_bmsg_draw");
   i = LOG_TMR_scrn_overlay;        log_types[i].group = 2;   strcpy(log_types[i].name, "LOG_TMR_scrn_overlay");
   i = LOG_TMR_sdif;                log_types[i].group = 2;   strcpy(log_types[i].name, "LOG_TMR_sdif");
   i = LOG_TMR_cdif;                log_types[i].group = 2;   strcpy(log_types[i].name, "LOG_TMR_cdif");
   i = LOG_TMR_rwnd;                log_types[i].group = 2;   strcpy(log_types[i].name, "LOG_TMR_rwnd");
   i = LOG_TMR_client_timer_adj;    log_types[i].group = 2;   strcpy(log_types[i].name, "LOG_TMR_client_timer_adj");
   i = LOG_TMR_client_ping;         log_types[i].group = 2;   strcpy(log_types[i].name, "LOG_TMR_client_ping");

   i = LOG_OTH_program_state;       log_types[i].group = 2;   strcpy(log_types[i].name, "LOG_OTH_program_state");
   i = LOG_OTH_transitions;         log_types[i].group = 2;   strcpy(log_types[i].name, "LOG_OTH_transitions");
   i = LOG_OTH_level_done;          log_types[i].group = 2;   strcpy(log_types[i].name, "LOG_OTH_level_done");
   i = LOG_OTH_move;                log_types[i].group = 2;   strcpy(log_types[i].name, "LOG_OTH_move");
   i = LOG_OTH_draw;                log_types[i].group = 2;   strcpy(log_types[i].name, "LOG_OTH_draw");


}

void mwLog::erase_log(void)
{
   log_msg[0] = 0;
   log_msg_pos = 0;
}

void mwLog::save_log_file(void)
{
   al_make_directory("logs"); // create if not already created
   FILE *filepntr;

   char filename[256];
   struct tm *timenow;
   time_t now = time(NULL);
   timenow = localtime(&now);
   strftime(filename, sizeof(filename), "logs/%Y%m%d-%H%M%S", timenow);

   char lh[16];
   strncpy(lh, mLoop.local_hostname, 16); // to remove compiler error in case local_hostname is too long

   char ph[80];
   sprintf(ph, "-[%d][%s].txt", mLevel.play_level, lh );

   strcat(filename, ph);

   if (strlen(log_msg) > 0)
   {
      filepntr = fopen(filename,"w");
      fprintf(filepntr, "%s", log_msg);
      fclose(filepntr);
      printf("%s saved \n", filename);
   }
   erase_log();
}





// these are for appending text to lines

// appends printf style text string
void mwLog::appf(int type, const char *format, ...)
{
   char smsg[200];
   va_list args;
   va_start(args, format);
   vsprintf(smsg, format, args);
   va_end(args);
   app(type, smsg);
}

// appends text string
void mwLog::app(int type, const char *txt)
{
   if (log_types[type].action & LOG_ACTION_PRINT) printf("%s", txt);
   if (log_types[type].action & LOG_ACTION_ERROR) mInput.m_err(txt);
   if (log_types[type].action & LOG_ACTION_LOG)
   {
      if ((log_msg_pos + strlen(txt)) >= NUM_LOG_CHAR) printf("log array full, > %d char\n", NUM_LOG_CHAR);
      else
      {
         memcpy(log_msg + log_msg_pos, txt, strlen(txt));
         log_msg_pos += strlen(txt);
         log_msg[log_msg_pos+1] = 0; // NULL terminate
      }
   }
}







// these are for adding lines with the prefix...

// adds printf style text string
void mwLog::addf(int type, int player, const char *format, ...)
{
   char smsg[200];
   va_list args;
   va_start(args, format);
   vsprintf(smsg, format, args);
   va_end(args);
   add(type, player, smsg);
}

// adds text string
void mwLog::add(int type, int player, const char *txt)
{
   char tmsg[500];
   sprintf(tmsg, "[%2d][%d][%d]%s", type, player, mLoop.frame_num, txt);


   if (log_types[type].action & LOG_ACTION_PRINT) printf("%s", tmsg);
   if (log_types[type].action & LOG_ACTION_ERROR) mInput.m_err(txt);
   if (log_types[type].action & LOG_ACTION_LOG)
   {
      if ((log_msg_pos + strlen(tmsg)) >= NUM_LOG_CHAR) printf("log array full, > %d char\n", NUM_LOG_CHAR);
      else
      {
         memcpy(log_msg + log_msg_pos, tmsg, strlen(tmsg));
         log_msg_pos += strlen(tmsg);
         log_msg[log_msg_pos+1] = 0; // NULL terminate
      }
   }
}




// adds fixed width formatted printf style text string
void mwLog::add_fwf(int type, int player, int width, int pos, const char *border, const char *fill, const char *format, ...)
{
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
   add(type, player, ftxt);
}


void mwLog::add_headerf(int type, int player, int blank_lines, const char *format, ...)
{
   char smsg[200];
   va_list args;
   va_start(args, format);
   vsprintf(smsg, format, args);
   va_end(args);
   add_header(type, player, blank_lines, smsg);
}

void mwLog::add_header(int type, int player, int blank_lines, const char *txt)
{
   add_fw(type, player, 76, 10, "+", "-", "");
   for (int i=0; i<blank_lines; i++) add_fw(type, player, 76, 10, "|", " ", "");
   add_fw(type, player, 76, (76 - strlen(txt))/2, "|", " ", txt);
   for (int i=0; i<blank_lines; i++) add_fw(type, player, 76, 10, "|", " ", "");
   add_fw(type, player, 76, 10, "+", "-", "");
}

void mwLog::log_time_date_stamp(void)
{
   char tmsg[80];
   struct tm *timenow;
   time_t now = time(NULL);
   timenow = localtime(&now);
   strftime(tmsg, sizeof(tmsg), "%Y-%m-%d  %H:%M:%S", timenow);
   add_fwf(10, 0, 76, 10, "|", " ", "Date and time: %s",tmsg);
}

void mwLog::log_versions(void)
{
   add_fw (10, 0, 76, 10, "+", "-", "");
   add_fwf(10, 0, 76, 10, "|", " ", "Purple Martians Version %s", mLoop.pm_version_string);
   add_fw (10, 0, 76, 10, "|", " ", mLoop.al_version_string);
   log_time_date_stamp();
   add_fw (10, 0, 76, 10, "+", "-", "");
}



void mwLog::log_ending_stats_client(int type, int p)
{
   add_headerf(type, 0, 0, "Client %d (%s) ending stats", p, mPlayer.loc[p].hostname);

   add_fwf(type, p, 76, 10, "|", " ", "total game frames.........[%d]", mLoop.frame_num);
   add_fwf(type, p, 76, 10, "|", " ", "frame when client joined..[%d]", mPlayer.loc[p].join_frame);

   if (mPlayer.loc[p].quit_frame == 0) mPlayer.loc[p].quit_frame = mLoop.frame_num;
   add_fwf(type, p, 76, 10, "|", " ", "frame when client quit....[%d]", mPlayer.loc[p].quit_frame);

   log_reason_for_player_quit(type, p);

   add_fwf(type, p, 76, 10, "|", " ", "frames client was active..[%d]", mPlayer.loc[p].quit_frame - mPlayer.loc[p].join_frame);
   add_fwf(type, p, 76, 10, "|", " ", "cdat packets total........[%d]", mPlayer.loc[p].client_cdat_packets_tx);
   add_fwf(type, p, 76, 10, "|", " ", "cdat packets late.........[%d]", mPlayer.syn[p].late_cdats);

   log_bandwidth_stats(type, p);
   add_fwf(type, 0, 76, 10, "+", "-", "");
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
      if ((mPlayer.syn[p].control_method == 2) || (mPlayer.syn[p].control_method == 8))
      {
         add_fwf(type, p, 76, 10, "|", " ", "Player:%d (%s)", p, mPlayer.loc[p].hostname);
         add_fwf(type, p, 76, 10, "|", " ", "frame when client joined..[%d]", mPlayer.loc[p].join_frame);

         if (mPlayer.loc[p].quit_frame == 0) mPlayer.loc[p].quit_frame = mLoop.frame_num;
         add_fwf(type, p, 76, 10, "|", " ", "frame when client quit....[%d]", mPlayer.loc[p].quit_frame);

         log_reason_for_player_quit(type, p);

         add_fwf(type, p, 76, 10, "|", " ", "frames client was active..[%d]", mPlayer.loc[p].quit_frame - mPlayer.loc[p].join_frame);
         add_fwf(type, p, 76, 10, "|", " ", "cdat packets total........[%d]", mPlayer.loc[p].client_cdat_packets_tx);
         add_fwf(type, p, 76, 10, "|", " ", "cdat packets late.........[%d]", mPlayer.syn[p].late_cdats);

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

      if ((mPlayer.syn[p].active) && (mPlayer.syn[p].control_method == 2))
         sprintf(ms, " <-- active client");

      if ((!mPlayer.syn[p].active) && (mPlayer.syn[p].control_method == 2))
         sprintf(ms, " <-- syncing client");

      if (mPlayer.syn[p].control_method == 9) sprintf(ms, " <-- used client");

      if (p == mPlayer.active_local_player) sprintf(ms, " <-- active local player (me!)");
      if (p == 0) sprintf(ms, " <-- server");

      sprintf(msg, "[%d][%2d][%d][%2d][%d] - %s %s",
                                              p,
                                              mPlayer.loc[p].who,
                                              mPlayer.syn[p].active,
                                              mPlayer.syn[p].color,
                                              mPlayer.syn[p].control_method,
                                              mPlayer.loc[p].hostname,
                                              ms );
      add_fw(type, 0, 76, 10, "|", " ", msg);
   }
   add_fw(type, 0, 76, 10, "+", "-", "");
}

void mwLog::log_player_array2(int type)
{
   add_fw(type, 0, 76, 10, "|", " ", "[p][a][m][sy]");
   for (int p=0; p<NUM_PLAYERS; p++)
   {
      float sy = mPlayer.loc[p].dsync;
      if (p == 0) sy = 0;
      add_fwf(type, 0, 76, 10, "|", " ", "[%d][%d][%d][%3.2f]",   p, mPlayer.syn[p].active, mPlayer.syn[p].control_method, sy );
   }
}



void mwLog::log_reason_for_player_quit(int type, int p)
{
   char tmsg[80];
   sprintf(tmsg,"unknown");
   int r = mPlayer.loc[p].quit_reason;
   if (r == 64) sprintf(tmsg,"player pressed ESC or menu key");
   if (r == 70) sprintf(tmsg,"server drop (server sync > 100)");
   if (r == 71) sprintf(tmsg,"server drop (no stak for 100 frames)");
   if (r == 74) sprintf(tmsg,"client never became active");
   if (r == 75) sprintf(tmsg,"client lost server connection");
   if (r == 80) sprintf(tmsg,"level done");
   if (r == 90) sprintf(tmsg,"local client quit");
   if (r == 91) sprintf(tmsg,"local server quit");
   if (r == 92) sprintf(tmsg,"remote server quit");
   if (r == 93) sprintf(tmsg,"remote client quit");

   add_fwf(type, p, 76, 10, "|", " ", "reason for quit...........[%s]", tmsg);
}

void mwLog::log_bandwidth_stats(int type, int p)
{
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


void mwLog::add_log_TMR(double dt, const char *tag, int p)
{
   addf(44, p, "tmst %s:[%0.4f]\n", tag, dt*1000);
}


// for single tags
void mwLog::add_tmr1(int type, int p, const char *tag, double dt)
{
   char txt[500];
   sprintf(txt, "%s:[%0.4f]\n", tag, dt*1000);
   add_tmr(type, p, txt);
}


// takes a printf format
void mwLog::add_tmrf(int type, int p, const char *format, ...)
{
   char smsg[200];
   va_list args;
   va_start(args, format);
   vsprintf(smsg, format, args);
   va_end(args);
   add_tmr(type, p, smsg);
}

// takes an already formatted string
void mwLog::add_tmr(int type, int p, const char *txt)
{
   if (log_types[type].action & LOG_ACTION_PRINT) printf("%s", txt);
   if (log_types[type].action & LOG_ACTION_LOG)
   {
      char tmsg[500];
      sprintf(tmsg, "[%2d][%d][%d]tmst %s", 44, p, mLoop.frame_num, txt); // force type 44

      if ((log_msg_pos + strlen(tmsg)) >= NUM_LOG_CHAR) printf("log array full, > %d char\n", NUM_LOG_CHAR);
      else
      {
         memcpy(log_msg + log_msg_pos, tmsg, strlen(tmsg));
         log_msg_pos += strlen(tmsg);
         log_msg[log_msg_pos+1] = 0; // NULL terminate
      }
   }
}



