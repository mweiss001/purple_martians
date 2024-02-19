// mwNetgameSessionLog.cpp

#include "pm.h"
#include "mwNetgame.h"
#include "mwPlayer.h"
#include "mwMiscFnx.h"
#include "mwLog.h"

void mwNetgame::session_clear_entry(int i)
{
   client_sessions[i].active = 0;
   client_sessions[i].status = 0;
   client_sessions[i].who = 0;
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
void mwNetgame::session_tally(int i)
{
   int p = client_sessions[i].player_num;

   client_sessions[i].cdats_rx     += mPlayer.loc[p].client_cdat_packets_tx;
   client_sessions[i].respawns     += mPlayer.syn[p].stat_respawns;
   client_sessions[i].shots_fired  += mPlayer.syn[p].stat_shots_fired;
   client_sessions[i].enemy_hits   += mPlayer.syn[p].stat_enemy_hits;

   client_sessions[i].player_hits  += mPlayer.syn[p].stat_player_hits;
   client_sessions[i].self_hits    += mPlayer.syn[p].stat_self_hits;
   client_sessions[i].purple_coins += mPlayer.syn[p].stat_purple_coins;

   client_sessions[i].tx_total_bytes           += mPlayer.loc[p].tx_total_bytes;
   client_sessions[i].tx_total_packets         += mPlayer.loc[p].tx_total_packets;
   client_sessions[i].rx_total_bytes           += mPlayer.loc[p].rx_total_bytes;
   client_sessions[i].rx_total_packets         += mPlayer.loc[p].rx_total_packets;

   if (mPlayer.loc[p].tx_max_bytes_per_frame > client_sessions[i].tx_max_bytes_per_frame) client_sessions[i].tx_max_bytes_per_frame = mPlayer.loc[p].tx_max_bytes_per_frame;
   if (mPlayer.loc[p].rx_max_bytes_per_frame > client_sessions[i].rx_max_bytes_per_frame) client_sessions[i].rx_max_bytes_per_frame = mPlayer.loc[p].rx_max_bytes_per_frame;
   if (mPlayer.loc[p].tx_max_packets_per_frame > client_sessions[i].tx_max_packets_per_frame) client_sessions[i].tx_max_packets_per_frame = mPlayer.loc[p].tx_max_packets_per_frame;
   if (mPlayer.loc[p].rx_max_packets_per_frame > client_sessions[i].rx_max_packets_per_frame) client_sessions[i].rx_max_packets_per_frame = mPlayer.loc[p].rx_max_packets_per_frame;

   // save duration up to this point
   client_sessions[i].duration = al_get_time() - client_sessions[i].start_time;
}








void mwNetgame::session_flush_active_at_server_exit(void)
{
   for (int i=0; i<16; i++)
      if (client_sessions[i].active)
      {
         client_sessions[i].active = 0;
         client_sessions[i].status = 40; // reason = server exit
         session_tally(i);
         session_add_log(i);
      }
}



void mwNetgame::session_save_active_at_level_done(void)
{
   for (int i=0; i<16; i++)
      if (client_sessions[i].active)
      {
         client_sessions[i].next_levels++;

         int p = client_sessions[i].player_num;
         if (p == mPlayer.syn[0].level_done_player) client_sessions[i].exits++;

         session_tally(i);
         session_add_log(i);
      }
}

int mwNetgame::session_get_empty(void)
{
   int ei = -1;
   for (int i=0; i<16; i++)
      if (!client_sessions[i].active)
      {
         ei = i;
         break;
      }
   return ei;
}


int mwNetgame::session_get_index_from_who(int who)
{
   int ei = -1;
   for (int i=0; i<16; i++)
      if ((client_sessions[i].active) && (client_sessions[i].who == who))
      {
         ei = i;
         break;
      }
   return ei;
}


void mwNetgame::session_add_entry(const char* address, int who)
{
   int i = session_get_empty();
   if (i != -1)
   {
      client_sessions[i].active = 1;
      client_sessions[i].status = 1;
      client_sessions[i].who = who;

      client_sessions[i].start_time = al_get_time();
      client_sessions[i].end_time = al_get_time();

      // get timestamp
      struct tm *timenow;
      time_t now = time(NULL);
      timenow = localtime(&now);
      char ts[16];
      strftime(ts, sizeof(ts), "%Y%m%d%H%M%S", timenow);
      strcpy(client_sessions[i].timestamp, ts);

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
      strcpy(client_sessions[i].ip, ip);

      mMiscFnx.chop_first_x_char(ad, c+1);
      int port = atoi(ad);
      client_sessions[i].port = atoi(ad);

      sprintf(client_sessions[i].session_name, "%s_%s_%d", ts, ip, port);

      session_add_log(i);
   }
}


void mwNetgame::session_update_entry(int who, int status, const char* hostname, int player_num)
{
   int i = session_get_index_from_who(who);
   if (i != -1)
   {
      client_sessions[i].status = status;
      client_sessions[i].player_num = player_num;
      sprintf(client_sessions[i].host_name, "%s", hostname);
      session_add_log(i);
   }
}


// this will replace any exsiting log with the exact name
void mwNetgame::session_add_log(int i)
{

   // this will be a little differenr

   // files are used as temp persistant storage, so i cant really just print to console
   // if file is not on, nothing will be saved
   // print is optional is file is on



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

      if (client_sessions[i].active) fprintf(filepntr, "Session Open\n");
      else                           fprintf(filepntr, "Session Closed\n");

      fprintf(filepntr, "Status:%d - ", client_sessions[i].status);
      if (client_sessions[i].status == 1 ) fprintf(filepntr, "Waiting\n");
      if (client_sessions[i].status == 2 ) fprintf(filepntr, "Running\n");
      if (client_sessions[i].status == 10) fprintf(filepntr, "Client did not join due to server full\n");
      if (client_sessions[i].status == 20) fprintf(filepntr, "Server did not receive cjon from client after 10s\n");
      if (client_sessions[i].status == 30) fprintf(filepntr, "Player became inactive\n");
      if (client_sessions[i].status == 40) fprintf(filepntr, "Player became inactive (server quit)\n");
      if (client_sessions[i].status == 50) fprintf(filepntr, "Player became inactive (server dropped player)\n");

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
   for (int i=0; i<16; i++)
      if ((client_sessions[i].active) && (client_sessions[i].player_num == p) && (client_sessions[i].status == 2))
      {
         client_sessions[i].active = 0;
         client_sessions[i].status = 50;
         client_sessions[i].duration = al_get_time() - client_sessions[i].start_time;
         session_add_log(i);
      }
}

void mwNetgame::session_check_active(void)
{
   for (int i=0; i<16; i++)
      if (client_sessions[i].active)
      {
         if (client_sessions[i].status == 1) // waiting for client cjon
         {
            // how long has it been in waiting for reply
            double wait = al_get_time() - client_sessions[i].start_time;
            if (wait > 10)
            {
               client_sessions[i].active = 0;
               client_sessions[i].status = 20;
               session_add_log(i);
            }
         }

         if (client_sessions[i].status == 10) // never started due to server full
         {
            client_sessions[i].active = 0;
            session_add_log(i);
         }

         if (client_sessions[i].status == 2) // active client
         {
            int p = client_sessions[i].player_num;
            if (!mPlayer.syn[p].active)
            {
               client_sessions[i].active = 0;
               client_sessions[i].status = 30;

               session_tally(i);
               session_add_log(i);
            }
         }
      }
}

