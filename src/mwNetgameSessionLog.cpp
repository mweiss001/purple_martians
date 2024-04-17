// mwNetgameSessionLog.cpp

#include "pm.h"
#include "mwNetgame.h"
#include "mwPlayer.h"
#include "mwMiscFnx.h"
#include "mwLog.h"


void mwNetgame::session_clear_entry(int i)
{
   client_sessions[i].active = 0;
   client_sessions[i].inactive_reason = 0;
   client_sessions[i].player_num = 0;

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
   strcpy(client_sessions[i].host_name, "");
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
         client_sessions[p].inactive_reason = 8;
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

void mwNetgame::session_add_entry(const char* address, const char* hostname, int p, int active, int inactive_reason)
{
   client_sessions[p].active = active;
   client_sessions[p].inactive_reason = inactive_reason;
   client_sessions[p].player_num = p;

   sprintf(client_sessions[p].host_name, "%s", hostname);

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

   // this will be a little different

   // files are used as temp persistant storage, so I really can't just print to console
   // if file is not on, nothing will be saved
   // print is optional if file is on


/*
   if (log_types[type].action & LOG_ACTION_PRINT) printf("%s", txt);
   if (log_types[type].action & LOG_ACTION_LOG)
*/

   if (mLog.log_types[LOG_NET_session].action & LOG_ACTION_LOG)
   {

      al_make_directory("logs/session"); // create if not already created
      char filename[256];
      sprintf(filename, "logs/session/%s.txt", client_sessions[i].session_name);


      FILE *filepntr;
      filepntr = fopen(filename,"w");

      fprintf(filepntr, "Timestamp:%s\n",     client_sessions[i].timestamp);
      fprintf(filepntr, "Duration:%0.1fs\n",  client_sessions[i].duration);
      fprintf(filepntr, "IP:%s\n",            client_sessions[i].ip);
      fprintf(filepntr, "Port:%d\n",          client_sessions[i].port);
      fprintf(filepntr, "Host Name:%s\n",     client_sessions[i].host_name);

      fprintf(filepntr, "\n");



      if (client_sessions[i].active) fprintf(filepntr, "Session not closed\n");
      else
      {
         fprintf(filepntr, "Session closed because ");
         if (client_sessions[i].inactive_reason == 1 ) fprintf(filepntr, "client never joined due to server full\n");
         if (client_sessions[i].inactive_reason == 4 ) fprintf(filepntr, "server dropped client due to loss of communication\n");
         if (client_sessions[i].inactive_reason == 6 ) fprintf(filepntr, "server dropped client due to session_check found player inactive\n");
         if (client_sessions[i].inactive_reason == 8 ) fprintf(filepntr, "server quit\n");

      }

      fprintf(filepntr, "\n");

      fprintf(filepntr, "cdat's Received:%d\n",  client_sessions[i].cdats_rx);

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

      char msg[80];

      fprintf(filepntr, "\nBytes Sent\n----------\n" );
      fprintf(filepntr, "%6s  Total\n", mMiscFnx.chrsi(client_sessions[i].tx_total_bytes, msg));
      fprintf(filepntr, "%6s  Avg per second\n", mMiscFnx.chrsi((int)tb_avg, msg));
      fprintf(filepntr, "%6s  Max per frame\n", mMiscFnx.chrsi(client_sessions[i].tx_max_bytes_per_frame, msg));

      fprintf(filepntr, "\nBytes Received\n--------------\n" );
      fprintf(filepntr, "%6s  Total\n", mMiscFnx.chrsi(client_sessions[i].rx_total_bytes, msg));
      fprintf(filepntr, "%6s  Avg per second\n", mMiscFnx.chrsi((int)rb_avg, msg));
      fprintf(filepntr, "%6s  Max per frame\n", mMiscFnx.chrsi(client_sessions[i].rx_max_bytes_per_frame, msg));

      fprintf(filepntr, "\nPackets Sent\n------------\n" );
      fprintf(filepntr, "%6s  Total\n", mMiscFnx.chrsi(client_sessions[i].tx_total_packets, msg));
      fprintf(filepntr, "%6s  Avg per second\n", mMiscFnx.chrsi((int)tp_avg, msg));
      fprintf(filepntr, "%6s  Max per frame\n", mMiscFnx.chrsi(client_sessions[i].tx_max_packets_per_frame, msg));

      fprintf(filepntr, "\nPackets Received\n----------------\n" );
      fprintf(filepntr, "%6s  Total\n", mMiscFnx.chrsi(client_sessions[i].rx_total_packets, msg));
      fprintf(filepntr, "%6s  Avg per second\n", mMiscFnx.chrsi((int)rp_avg, msg));
      fprintf(filepntr, "%6s  Max per frame\n", mMiscFnx.chrsi(client_sessions[i].rx_max_packets_per_frame, msg));


      fprintf(filepntr, "\n");

      fprintf(filepntr, "Player Num:%d\n",       client_sessions[i].player_num);
      fprintf(filepntr, "Next Levels:%d\n",      client_sessions[i].next_levels);
      fprintf(filepntr, "Exits Activated:%d\n",  client_sessions[i].exits);
      fprintf(filepntr, "Respawns:%d\n",         client_sessions[i].respawns);
      fprintf(filepntr, "Shots Fired:%d\n",      client_sessions[i].shots_fired);
      fprintf(filepntr, "Enemy Hits:%d\n",       client_sessions[i].enemy_hits);
      fprintf(filepntr, "Player Hits:%d\n",      client_sessions[i].player_hits);
      fprintf(filepntr, "Self Hits:%d\n",        client_sessions[i].self_hits);
      fprintf(filepntr, "Purple Coins:%d\n",     client_sessions[i].purple_coins);

      fprintf(filepntr, "\n");

      fclose(filepntr);

      if (mLog.log_types[LOG_NET_session].action & LOG_ACTION_PRINT) // open the file and print it
      {
         printf("\nAdded Log Entry:%s\n", filename);

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
            printf("%s\n", buff);
            if (ch == EOF) done = 1;
         }
         fclose(filepntr);
      }

      // erase if inactive
      if (client_sessions[i].active == 0) session_clear_entry(i);
   }
}

void mwNetgame::session_drop_player(int p)
{
   client_sessions[p].active = 0;
   client_sessions[p].inactive_reason = 4;
   client_sessions[p].duration = al_get_time() - client_sessions[p].start_time;
   session_add_log(p);
}



// this is called every frame from server_control() to check and clean up active sessions
void mwNetgame::session_check_active(void)
{
   for (int p=1; p<8; p++)
      if (client_sessions[p].active)
      {
         if (!mPlayer.syn[p].active) // client is not active anymore
         {
            client_sessions[p].active = 0;

            client_sessions[p].inactive_reason = 6;


            session_tally(p);
            session_add_log(p);
         }
      }
}

