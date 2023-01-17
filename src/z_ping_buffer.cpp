// ping_buffer.cpp
#include "pm.h"

void ping_buffer_clear(void)
{
   for (int i=0; i<4; i++)
      ping_array[i] = 0;;
   ping_num_filled = 0;
   ping_index = 0;
}

void ping_array_add(double ping)
{
   // always put it where ping_index points
   ping_array[ping_index] = ping;

   // printf("adding ping:%f i:%d nf:%d \n", ping, ping_index, ping_num_filled);

   ping_index++;
   ping_num_filled++;

   if (ping_index > 7) ping_index = 0;

   // calculate average
   int ul = 8; // default
   if (ping_num_filled < 8) ul = ping_num_filled;

   //printf("ul:%d\n", ul);

   double tally = 0;
   for (int i=0; i<ul; i++)
   {
      tally += ping_array[i];
      //printf("i:%d %f \n", i, ping_array[i]);
   }
   int p = active_local_player;
   players1[p].ping_avg = tally / (ul);

   players1[p].client_chase_offset = - players1[p].ping_avg;

   sprintf(msg, "ping [%3.2f] avg[%3.2f]\n", ping*1000, players1[p].ping_avg*1000);
   //printf(msg);
   if (LOG_NET_client_ping) add_log_entry2(37, p, msg);

}

























// OK do it



