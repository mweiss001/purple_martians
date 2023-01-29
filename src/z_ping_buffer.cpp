// ping_buffer.cpp
#include "pm.h"
#include "z_log.h"
#include "z_player.h"
#include "z_ping_buffer.h"


double dsync_array[8] = {0};
int    dsync_num_filled = 0;
int    dsync_index = 0;

void dysnc_buffer_clear(void)
{
   for (int i=0; i<8; i++)
      dsync_array[i] = 0;;
   dsync_num_filled = 0;
   dsync_index = 0;
}



double ping_array[8] = {0};
int    ping_num_filled = 0;
int    ping_index = 0;

void ping_buffer_clear(void)
{
   for (int i=0; i<8; i++)
      ping_array[i] = 0;;
   ping_num_filled = 0;
   ping_index = 0;
}




void ping_array_add(double ping)
{
   int p = active_local_player;

   ping_array[ping_index] = ping; // always put it where ping_index points
   if (++ping_index > 7) ping_index = 0;
   ping_num_filled++;

   // find number of used entries
   int ul = 8; // default
   if (ping_num_filled < 8) ul = ping_num_filled;

   // add up the entries
   double tally = 0;
   for (int i=0; i<ul; i++)
      tally += ping_array[i];

   // get the average
   players1[p].ping_avg = tally / (ul);


   if (players1[p].client_chase_offset_mode) players1[p].client_chase_offset = - players1[p].ping_avg + players1[p].client_chase_offset_auto_offset;

   sprintf(msg, "ping [%3.2f] avg[%3.2f]\n", ping*1000, players1[p].ping_avg*1000);
   //printf(msg);
   if (LOG_NET_client_ping) add_log_entry2(37, p, msg);

   sprintf(msg, "tmst ping:[%5.2f] pavg:[%5.2f]\n", ping*1000, players1[p].ping_avg*1000);
   if (LOG_TMR_client_ping) if (frame_num) add_log_entry2(44, 0, msg);

}




void dsync_array_add(void)
{
   int p = active_local_player;

   dsync_array[dsync_index] = players1[p].dsync; // put it where dsync_index points
   if (++dsync_index > 7) dsync_index = 0;
   dsync_num_filled++;

   // find number of used entries
   int ul = 8; // default
   if (dsync_num_filled < 8) ul = dsync_num_filled;

   // add up the entries
   double tally = 0;
   for (int i=0; i<ul; i++)
      tally += dsync_array[i];

   // get the average
   players1[p].dsync_avg = tally / (ul);
}


