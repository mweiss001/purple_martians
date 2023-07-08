// mwLog.cpp

#include "pm.h"
#include "mwLog.h"
#include "mwPlayer.h"
#include "mwGameMoves.h"
#include "mwLoop.h"
#include "mwLevel.h"

mwLog mLog;

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


void mwLog::add_log_entry2(int type, int player, const char *txt)
{
   char tmsg[500];
   sprintf(tmsg, "[%2d][%d][%d]%s", type, player, mLoop.frame_num, txt);
   // strcat(log_msg, tmsg);

   if ((log_msg_pos + strlen(tmsg)) >= NUM_LOG_CHAR)
   {
      printf("log array full, > %d char\n", NUM_LOG_CHAR);
   }
   else
   {
      memcpy(log_msg + log_msg_pos, tmsg, strlen(tmsg));
      log_msg_pos += strlen(tmsg);
      log_msg[log_msg_pos+1] = 0; // NULL terminate
      //sprintf(log_msg, "%s", txt);
      //printf("%s", tmsg);
   }
}

void mwLog::add_log_entry_position_text(int type, int player, int width, int pos, const char *txt, const char *border, const char *fill)
{
   int l = strlen(txt);
   int j1 = pos-2;
   int j2 = width - l - pos;
   char p[200];

   if (pos > 1)
   {
      strcpy(p, border);
      for (int i=0; i<j1; i++) strcat(p, fill);
      strcat(p, txt);
      for (int i=0; i<j2; i++) strcat(p, fill);
      strcat(p, border);
      strcat(p, "\n");
   }

   if (pos == 1)
   {
      j2--;
      strcpy(p, border);
      for (int i=0; i<j1; i++) strcat(p, fill);
      strcat(p, txt);
      for (int i=0; i<j2; i++) strcat(p, fill);
      strcat(p, border);
      strcat(p, "\n");
   }

   if (pos == 0)
   {
      strcpy(p, "");
      j2--;
      for (int i=0; i<j1; i++) strcat(p, fill);
      strcat(p, txt);
      for (int i=0; i<j2; i++) strcat(p, fill);
      strcat(p, border);
      strcat(p, "\n");
   }
   add_log_entry2(type, player, p);


}

void mwLog::add_log_entry_centered_text(int type, int player, int width, const char *txt, const char *border, const char *fill)
{
   int l = strlen(txt);
   int j1 = (width-l)/2 - 1;
   int j2 = j1 + 1;
   if (l % 2 == 0) j2 = j1;

   char p[200];

   strcpy(p, border);
   for (int i=0; i<j1; i++) strcat(p, fill);
   strcat(p, txt);
   for (int i=0; i<j2; i++) strcat(p, fill);
   strcat(p, border);

   strcat(p, "\n");
   add_log_entry2(type, player, p);
}

void mwLog::add_log_entry_header(int type, int player, const char *txt, int blank_lines)
{
   char htext[80]; // make a copy so that doesn't get overwritten
   sprintf(htext, "%s", txt);
   add_log_entry_centered_text(type, player, 76, "", "+", "-");

   for (int i=0; i<blank_lines; i++)
      add_log_entry_centered_text(type, player, 76, "", "|", " ");

   add_log_entry_centered_text(type, player, 76, htext, "|", " ");

   for (int i=0; i<blank_lines; i++)
      add_log_entry_centered_text(type, player, 76, "", "|", " ");

   add_log_entry_centered_text(type, player, 76, "", "+", "-");
}

void mwLog::log_bandwidth_stats(int p)
{
   char msg[1024];
   sprintf(msg,"total tx bytes............[%d]", mPlayer.loc[p].tx_total_bytes);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   sprintf(msg,"max tx bytes per frame....[%d]", mPlayer.loc[p].tx_max_bytes_per_frame);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   sprintf(msg,"avg tx bytes per frame....[%d]", mPlayer.loc[p].tx_total_bytes / mLoop.frame_num);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   sprintf(msg,"max rx bytes per second...[%d]", mPlayer.loc[p].tx_max_bytes_per_tally);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   sprintf(msg,"avg tx bytes per sec......[%d]", (mPlayer.loc[p].tx_total_bytes *40)/ mLoop.frame_num);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   sprintf(msg,"total tx packets..........[%d]", mPlayer.loc[p].tx_total_packets);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   sprintf(msg,"max tx packets per frame..[%d]", mPlayer.loc[p].tx_max_packets_per_frame);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   sprintf(msg,"max tx packets per second.[%d]", mPlayer.loc[p].tx_max_packets_per_tally);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");


   sprintf(msg,"total rx bytes............[%d]", mPlayer.loc[p].rx_total_bytes);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   sprintf(msg,"max rx bytes per frame....[%d]", mPlayer.loc[p].rx_max_bytes_per_frame);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   sprintf(msg,"avg rx bytes per frame....[%d]", mPlayer.loc[p].rx_total_bytes / mLoop.frame_num);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   sprintf(msg,"max rx bytes per second...[%d]", mPlayer.loc[p].rx_max_bytes_per_tally);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   sprintf(msg,"avg rx bytes per sec......[%d]", (mPlayer.loc[p].rx_total_bytes *40)/ mLoop.frame_num);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   sprintf(msg,"total rx packets..........[%d]", mPlayer.loc[p].rx_total_packets);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   sprintf(msg,"max rx packets per frame..[%d]", mPlayer.loc[p].rx_max_packets_per_frame);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   sprintf(msg,"max rx packets per second.[%d]", mPlayer.loc[p].rx_max_packets_per_tally);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");
}

void mwLog::log_reason_for_player_quit(int p)
{
   char msg[1024];
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

   sprintf(msg,"reason for quit...........[%s]", tmsg);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");
}


void mwLog::add_log_TMR(double dt, const char *tag, int p)
{
   char msg[1024];
   sprintf(msg, "tmst %s:[%0.4f]\n", tag, dt*1000);
   add_log_entry2(44, p, msg);
}

void mwLog::log_time_date_stamp(void)
{
   char msg[1024];
   char tmsg[80];
   struct tm *timenow;
   time_t now = time(NULL);
   timenow = localtime(&now);
   strftime(tmsg, sizeof(tmsg), "%Y-%m-%d  %H:%M:%S", timenow);
   sprintf(msg, "Date and time: %s",tmsg);
   printf("%s\n", msg);
   add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");
}

void mwLog::log_versions(void)
{
   char msg[1024];
   add_log_entry_centered_text(10, 0, 76, "", "+", "-");
   sprintf(msg, "Purple Martians Version %s", mLoop.pm_version_string);
   add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");
   add_log_entry_position_text(10, 0, 76, 10, mLoop.al_version_string, "|", " ");
   log_time_date_stamp();
   add_log_entry_centered_text(10, 0, 76, "", "+", "-");
}

void mwLog::log_player_array(void)
{
   char msg[1024];
   add_log_entry_header(10, 0,  "Player Array", 0);
   add_log_entry_position_text(10, 0, 76, 10, "[p][wh][a][co][m]", "|", " ");
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
      add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");
   }
   add_log_entry_centered_text(10, 0, 76, "", "+", "-");
}

void mwLog::log_player_array2(void)
{
   char msg[1024];
   sprintf(msg, "[p][a][m][sy]");
   add_log_entry_position_text(26, 0, 76, 10, msg, "|", " ");
   for (int p=0; p<NUM_PLAYERS; p++)
   {
      float sy = mPlayer.loc[p].dsync;
      if (p == 0) sy = 0;
      sprintf(msg, "[%d][%d][%d][%3.2f]",   p, mPlayer.syn[p].active, mPlayer.syn[p].control_method, sy );
      add_log_entry_position_text(26, 0, 76, 10, msg, "|", " ");
   }
}

void mwLog::log_ending_stats(int p)
{
   char msg[1024];
   sprintf(msg,"Client %d (%s) ending stats", p, mPlayer.loc[p].hostname);
   add_log_entry_header(10, p, msg, 0);

   sprintf(msg,"total game frames.........[%d]", mLoop.frame_num);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");
   sprintf(msg,"frame when client joined..[%d]", mPlayer.loc[p].join_frame);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   if (mPlayer.loc[p].quit_frame == 0) mPlayer.loc[p].quit_frame = mLoop.frame_num;
   sprintf(msg,"frame when client quit....[%d]", mPlayer.loc[p].quit_frame);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   log_reason_for_player_quit(p);

   sprintf(msg,"frames client was active..[%d]", mPlayer.loc[p].quit_frame - mPlayer.loc[p].join_frame);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   sprintf(msg,"cdat packets total........[%d]", mPlayer.loc[p].client_cdat_packets_tx);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   sprintf(msg,"cdat packets late.........[%d]", mPlayer.loc[p].late_cdats);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   log_bandwidth_stats(p);
   add_log_entry_centered_text(10, p, 76, "", "+", "-");
}

void mwLog::log_ending_stats_server()
{
   char msg[1024];
   sprintf(msg,"Server (%s) ending stats", mLoop.local_hostname);
   add_log_entry_header(10, 0, msg, 0);

   add_log_entry_centered_text(10, 0, 76, "", "+", "-");

   sprintf(msg,"level.....................[%d]", mLevel.play_level);
   add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");

   sprintf(msg,"total frames..............[%d]", mLoop.frame_num);
   add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");

   sprintf(msg,"total moves...............[%d]", mGameMoves.entry_pos);
   add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");

   sprintf(msg,"total time (seconds)......[%d]", mLoop.frame_num/40);
   add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");

   sprintf(msg,"total time (minutes)......[%d]", mLoop.frame_num/40/60);
   add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");

   log_bandwidth_stats(0);

   add_log_entry_centered_text(10, 0, 76, "", "+", "-");

   for (int p=1; p<NUM_PLAYERS; p++)
   {
      if ((mPlayer.syn[p].control_method == 2) || (mPlayer.syn[p].control_method == 8))
      {
         sprintf(msg,"Player:%d (%s)", p, mPlayer.loc[p].hostname);
         add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

         sprintf(msg,"frame when client joined..[%d]", mPlayer.loc[p].join_frame);
         add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

         if (mPlayer.loc[p].quit_frame == 0) mPlayer.loc[p].quit_frame = mLoop.frame_num;
         sprintf(msg,"frame when client quit....[%d]", mPlayer.loc[p].quit_frame);
         add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

         log_reason_for_player_quit(p);

         sprintf(msg,"frames client was active..[%d]", mPlayer.loc[p].quit_frame - mPlayer.loc[p].join_frame);
         add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

         sprintf(msg,"cdat packets total........[%d]", mPlayer.loc[p].client_cdat_packets_tx);
         add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

         sprintf(msg,"cdat packets late.........[%d]", mPlayer.loc[p].late_cdats);
         add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

         log_bandwidth_stats(p);

         add_log_entry_centered_text(10, p, 76, "", "+", "-");
      }
   }
   log_player_array();
}

