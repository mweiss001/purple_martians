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



mwLog mLog;

mwLog::mwLog()
{
   // init_log_types(); // now only done when recreating settings.pm
   erase_log();
   erase_log_net();
   erase_log_status();
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

   i = LOG_error;                   log_types[i].group = 9;   strcpy(log_types[i].name, "LOG_error"); log_types[i].action = 3;   // always both actions

   i = LOG_NET;                     log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET");

   i = LOG_NET_session;             log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_session");

   i = LOG_NET_ending_stats;        log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_ending_stats");
   i = LOG_NET_bandwidth;           log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_bandwidth");

   i = LOG_NET_stdf_rewind;         log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_stdf_rewind");
   i = LOG_NET_stdf_create;         log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_stdf_create");

   i = LOG_NET_stdf;                log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_stdf");
   i = LOG_NET_stdf_packets;        log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_stdf_packets");
   i = LOG_NET_dif_apply;           log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_dif_apply");

   i = LOG_NET_stak;                log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_stak");
   i = LOG_NET_cdat;                log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_cdat");
   i = LOG_NET_client_ping;         log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_client_ping");
   i = LOG_NET_timer_adjust;        log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_timer_adjust");
   i = LOG_NET_file_transfer;       log_types[i].group = 1;   strcpy(log_types[i].name, "LOG_NET_file_transfer");

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
   i = LOG_TMR_proc_rx_buffer;      log_types[i].group = 2;   strcpy(log_types[i].name, "LOG_TMR_proc_rx_buffer");

   i = LOG_OTH_program_state;       log_types[i].group = 3;   strcpy(log_types[i].name, "LOG_OTH_program_state");
   i = LOG_OTH_transitions;         log_types[i].group = 3;   strcpy(log_types[i].name, "LOG_OTH_transitions");
   i = LOG_OTH_level_done;          log_types[i].group = 3;   strcpy(log_types[i].name, "LOG_OTH_level_done");
   i = LOG_OTH_move;                log_types[i].group = 3;   strcpy(log_types[i].name, "LOG_OTH_move");
   i = LOG_OTH_draw;                log_types[i].group = 3;   strcpy(log_types[i].name, "LOG_OTH_draw");
}

void mwLog::set_log_type_action(int type, int action)
{
   log_types[type].action = action;
}

void mwLog::clear_all_log_actions(void)
{
   for (int i=0; i<100; i++)
      if (log_types[i].group != 9) // except for errors
         log_types[i].action = 0;

   autosave_log_on_level_done = 0;
   autosave_log_on_level_quit = 0;
   autosave_log_on_program_exit = 0;

   mConfig.save_config(0);
}



void mwLog::flush_logs(void)
{
   save_log_file();
   save_log_net_file();
   save_log_status_file();
}



void mwLog::erase_log(void)
{
   log_msg[0] = 0;
   log_msg_pos = 0;
}

void mwLog::erase_log_net(void)
{
   log_net_msg[0] = 0;
   log_net_msg_pos = 0;
   log_net_msg_num_lines = 0;
}

void mwLog::erase_log_status(void)
{
   log_status_msg[0] = 0;
   log_status_msg_pos = 0;
   log_status_msg_num_lines = 0;
}


void mwLog::save_log_file(void)
{
   if (strlen(log_msg) > 0)
   {
      al_make_directory("logs"); // create if not already created

      char filename[256];
      time_t now = time(NULL);
      struct tm *timenow = localtime(&now);
      strftime(filename, sizeof(filename), "logs/%Y%m%d-%H%M%S", timenow);

      char lh[16];
      strncpy(lh, mLoop.local_hostname, 16); // only get max 16 char of local_hostname

      char ph[80];
      sprintf(ph, "-[%d][%s].txt", mLevel.play_level, lh );

      strcat(filename, ph);

      FILE *filepntr = fopen(filename,"w");
      fprintf(filepntr, "%s", log_msg);
      fclose(filepntr);

      printf("%s saved \n", filename);
      erase_log();
   }
}



void mwLog::save_log_net_file(void)
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


void mwLog::save_log_status_file(void)
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



// appends passed text string to log array
// this is only function that actually prints to console or adds to log array
// also checks if log array is full and flushes to disk
void mwLog::log_append_text(int type, const char *txt)
{
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
   // skip these message types if server with no clients
   if ((mNetgame.ima_server) && (mNetgame.server_num_clients == 0))
   {
      if (type == LOG_NET_stdf_rewind) return;
      if (type == LOG_NET_stdf_create) return;
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
   char d[100];
   mMiscFnx.chr_dt(d);

   char txt[1000];
   sprintf(txt, "%d,%d,%s,%f,%d,%d,%d,%s\n", type, sub_type, d, agt, f, p, client, msg);
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

void mwLog::add_log_status_db_rows(void)
{
   // do not log status unless at least one client is connected
   if (mNetgame.server_num_clients == 0) return;

   char d[100];
   mMiscFnx.chr_dt(d);

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
                       d, f, p, r, cpu,  sync, ping, difs, lcor, rcor, txbf, rxbf, txpf, rxpf);

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
