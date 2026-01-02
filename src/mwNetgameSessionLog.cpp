// mwNetgameSessionLog.cpp

#include "pm.h"
#include "mwNetgame.h"
#include "mwPlayer.h"
#include "mwMiscFnx.h"
#include "mwLog.h"


void mwNetgame::session_clear_entry(int i)
{
   client_sessions[i].active = 0;
   client_sessions[i].endreason = 0;
   client_sessions[i].player_num = 0;
   client_sessions[i].player_color = 0;

   client_sessions[i].start_time = 0;
   client_sessions[i].end_time = 0;
   client_sessions[i].duration = 0;

   client_sessions[i].next_levels = 0;
   client_sessions[i].exits = 0;

   client_sessions[i].cdats_rx = 0;

   client_sessions[i].respawns = 0;
   client_sessions[i].shots_fired = 0;
   client_sessions[i].enemy_hits = 0;

   client_sessions[i].player_hits = 0;
   client_sessions[i].self_hits = 0;
   client_sessions[i].purple_coins = 0;

   client_sessions[i].tx_total_bytes = 0;
   client_sessions[i].tx_total_packets = 0;
   client_sessions[i].tx_max_bytes_per_frame = 0;
   client_sessions[i].tx_max_packets_per_frame = 0;

   client_sessions[i].rx_total_bytes = 0;
   client_sessions[i].rx_total_packets = 0;
   client_sessions[i].rx_max_bytes_per_frame = 0;
   client_sessions[i].rx_max_packets_per_frame = 0;

   strcpy(client_sessions[i].session_name, "");
   strcpy(client_sessions[i].player_name, "");
   strcpy(client_sessions[i].hostname, "");
   strcpy(client_sessions[i].timestamp, "");
   strcpy(client_sessions[i].ip, "");

   client_sessions[i].port = 0;
}


// adds current player tally things to session tally
void mwNetgame::session_tally(int p)
{
   client_sessions[p].cdats_rx     += mPlayer.loc[p].client_cdat_packets_tx;
   client_sessions[p].respawns     += mPlayer.syn[p].stat_respawns;
   client_sessions[p].shots_fired  += mPlayer.syn[p].stat_shots_fired;
   client_sessions[p].enemy_hits   += mPlayer.syn[p].stat_enemy_hits;

   client_sessions[p].player_hits  += mPlayer.syn[p].stat_player_hits;
   client_sessions[p].self_hits    += mPlayer.syn[p].stat_self_hits;
   client_sessions[p].purple_coins += mPlayer.syn[p].stat_purple_coins;

   client_sessions[p].tx_total_bytes           += mPlayer.loc[p].tx_total_bytes;
   client_sessions[p].tx_total_packets         += mPlayer.loc[p].tx_total_packets;
   client_sessions[p].rx_total_bytes           += mPlayer.loc[p].rx_total_bytes;
   client_sessions[p].rx_total_packets         += mPlayer.loc[p].rx_total_packets;

   if (mPlayer.loc[p].tx_max_bytes_per_frame   > client_sessions[p].tx_max_bytes_per_frame)   client_sessions[p].tx_max_bytes_per_frame   = mPlayer.loc[p].tx_max_bytes_per_frame;
   if (mPlayer.loc[p].rx_max_bytes_per_frame   > client_sessions[p].rx_max_bytes_per_frame)   client_sessions[p].rx_max_bytes_per_frame   = mPlayer.loc[p].rx_max_bytes_per_frame;
   if (mPlayer.loc[p].tx_max_packets_per_frame > client_sessions[p].tx_max_packets_per_frame) client_sessions[p].tx_max_packets_per_frame = mPlayer.loc[p].tx_max_packets_per_frame;
   if (mPlayer.loc[p].rx_max_packets_per_frame > client_sessions[p].rx_max_packets_per_frame) client_sessions[p].rx_max_packets_per_frame = mPlayer.loc[p].rx_max_packets_per_frame;

   // save duration up to this point
   client_sessions[p].duration = al_get_time() - client_sessions[p].start_time;
}


void mwNetgame::session_flush_active_at_server_exit(void)
{
   for (int p=1; p<8; p++)
      if (client_sessions[p].active)
      {
         client_sessions[p].active = 0;
         client_sessions[p].endreason = 8;
         session_tally(p);
         session_add_log(p);
      }
}

void mwNetgame::session_save_active_at_level_done(void)
{
   for (int p=1; p<8; p++)
      if (client_sessions[p].active)
      {
         client_sessions[p].next_levels++;
         if (p == mPlayer.syn[0].level_done_player) client_sessions[p].exits++;
         session_tally(p);
         session_add_log(p);
      }
}

void mwNetgame::session_add_entry(const char* address, const char* hostname, int p, int active, int endreason)
{
   if (!mLog.log_types[LOG_NET_session].action) return;

   session_clear_entry(p);

   client_sessions[p].active = active;
   client_sessions[p].endreason = endreason;
   client_sessions[p].player_num = p;
   client_sessions[p].player_color = mPlayer.syn[p].color;

   sprintf(client_sessions[p].hostname, "%s", hostname);
   sprintf(client_sessions[p].hostname, "%s", mPlayer.syn[p].name);

   client_sessions[p].start_time = al_get_time();
   client_sessions[p].end_time = al_get_time();

   // get timestamp
   struct tm *timenow;
   time_t now = time(NULL);
   timenow = localtime(&now);
   char ts[20];
   strftime(ts, sizeof(ts), "%Y%m%d-%H%M%S", timenow);
   strcpy(client_sessions[p].timestamp, ts);

   // make a copy of address so we can play with it
   char ad[32];
   strcpy(ad, address);

   char ip[16];

   // split address at ':'  everything before is ip, everything after is port
   int c;
   for (c=0; c<(int)strlen(ad); c++)
      if (ad[c] == ':')
      {
         strncpy(ip, ad, c);
         ip[c] = 0;
         break;
      }
   strcpy(client_sessions[p].ip, ip);

   mMiscFnx.chop_first_x_char(ad, c+1);
   int port = atoi(ad);
   client_sessions[p].port = atoi(ad);

   sprintf(client_sessions[p].session_name, "%s-%s-%d", ts, ip, port);

   session_add_log(p);
}


// this will replace any existing log with the exact name
void mwNetgame::session_add_log(int i)
{
   // this is different from other log tyeps
   // files are used as temp persistant storage until the session is closed
   // if LOG_ACTION_LOG is not on, nothing will be saved
   // if only LOG_ACTION_PRINT is on, nothing will happen
   // if both are on, the file will be printed to console after it is saved

   if (mLog.log_types[LOG_NET_session].action & LOG_ACTION_LOG)
   {
      al_make_directory("logs/session"); // create if not already created
      char filename[256];
      sprintf(filename, "logs/session/%s.txt", client_sessions[i].session_name);

      FILE *filepntr;
      filepntr = fopen(filename,"w");

      fprintf(filepntr, "dt_start:%s\n",      client_sessions[i].timestamp);
      fprintf(filepntr, "duration:%d\n",      (int) client_sessions[i].duration);
      fprintf(filepntr, "ip:%s\n",            client_sessions[i].ip);
      fprintf(filepntr, "port:%d\n",          client_sessions[i].port);
      fprintf(filepntr, "hostname:%s\n",      client_sessions[i].hostname);

      int er = client_sessions[i].endreason;
      char endreason[256];
      if (er == 0) sprintf(endreason, "unknown");
      if (er == 1) sprintf(endreason, "server_full");
      if (er == 2) sprintf(endreason, "client_quit");
      if (er == 4) sprintf(endreason, "comm_loss");
      if (er == 6) sprintf(endreason, "inactive");


      if (er == 8) sprintf(endreason, "server quit");
      fprintf(filepntr, "endreason:%s\n", endreason);

      fprintf(filepntr, "cdat_rx:%d\n",          client_sessions[i].cdats_rx);
      fprintf(filepntr, "player_num:%d\n",       client_sessions[i].player_num);
      fprintf(filepntr, "player_name:%s\n",      client_sessions[i].player_name);
      fprintf(filepntr, "player_color:%d\n",     client_sessions[i].player_color);

      fprintf(filepntr, "next_levels:%d\n",      client_sessions[i].next_levels);
      fprintf(filepntr, "exits_activated:%d\n",  client_sessions[i].exits);
      fprintf(filepntr, "respawns:%d\n",         client_sessions[i].respawns);
      fprintf(filepntr, "shots_fired:%d\n",      client_sessions[i].shots_fired);
      fprintf(filepntr, "enemy_hits:%d\n",       client_sessions[i].enemy_hits);
      fprintf(filepntr, "player_hits:%d\n",      client_sessions[i].player_hits);
      fprintf(filepntr, "self_hits:%d\n",        client_sessions[i].self_hits);
      fprintf(filepntr, "purple_coins:%d\n",     client_sessions[i].purple_coins);

      float dur = client_sessions[i].duration;
      float tb_avg = 0;
      float rb_avg = 0;
      float tp_avg = 0;
      float rp_avg = 0;

      if (dur > 0)
      {
         tb_avg = (float)client_sessions[i].tx_total_bytes / dur;
         rb_avg = (float)client_sessions[i].rx_total_bytes / dur;
         tp_avg = (float)client_sessions[i].tx_total_packets / dur;
         rp_avg = (float)client_sessions[i].rx_total_packets / dur;
      }

      fprintf(filepntr, "tx_bytes_total:%" PRId64 "\n",    client_sessions[i].tx_total_bytes);
      fprintf(filepntr, "tx_bytes_avg_per_sec:%d\n",       (int) tb_avg);
      fprintf(filepntr, "tx_bytes_max_per_frame:%d\n",     client_sessions[i].tx_max_bytes_per_frame);

      fprintf(filepntr, "rx_bytes_total:%" PRId64 "\n",    client_sessions[i].rx_total_bytes);
      fprintf(filepntr, "rx_bytes_avg_per_sec:%d\n",       (int) rb_avg);
      fprintf(filepntr, "rx_bytes_max_per_frame:%d\n",     client_sessions[i].rx_max_bytes_per_frame);

      fprintf(filepntr, "tx_packets_total:%" PRId64 "\n",  client_sessions[i].tx_total_packets);
      fprintf(filepntr, "tx_packets_avg_per_sec:%d\n",     (int) tp_avg);
      fprintf(filepntr, "tx_packets_max_per_frame:%d\n",   client_sessions[i].tx_max_packets_per_frame);

      fprintf(filepntr, "rx_packets_total:%" PRId64 "\n",  client_sessions[i].rx_total_packets);
      fprintf(filepntr, "rx_packets_avg_per_sec:%d\n",     (int) rp_avg);
      fprintf(filepntr, "rx_packets_max_per_frame:%d\n",   client_sessions[i].rx_max_packets_per_frame);

      fclose(filepntr);

      if (mLog.log_types[LOG_NET_session].action & LOG_ACTION_PRINT) // open the file and print it
      {
         printf("filename:%s", filename);

         filepntr = fopen(filename,"r");
         int done = 0;
         while (!done)
         {
            int ch = fgetc(filepntr);
            int loop = 0;
            char buff[200];
            while((ch != '\n') && (ch != EOF))
            {
               if (ch != 13) buff[loop++] = ch;
               ch = fgetc(filepntr);
            }
            buff[loop] = 0; // terminate the string

            if (ch == EOF) done = 1;
            else
            {
               printf("%s\n", buff);
            }
         }
         fclose(filepntr);
      }

      // erase if inactive
      if (client_sessions[i].active == 0) session_clear_entry(i);
   }
}

void mwNetgame::session_close(int p, int reason)
{
   if (!mLog.log_types[LOG_NET_session].action) return;
   client_sessions[p].active = 0;
   client_sessions[p].endreason = reason;
   client_sessions[p].duration = al_get_time() - client_sessions[p].start_time;
   session_tally(p);
   session_add_log(p);
}


// this is called every frame from server_control() to check and clean up active sessions
void mwNetgame::session_check_active(void)
{
   if (!mLog.log_types[LOG_NET_session].action) return;
   for (int p=1; p<8; p++)
      if ((client_sessions[p].active) && (!mPlayer.syn[p].active)) session_close(p, 6); // session still active, but player is not active anymore
}

