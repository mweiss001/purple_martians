// mwNetgame.cpp

#include "pm.h"
#include "mwNetgame.h"
#include "mwPlayer.h"
#include "mwEnemy.h"
#include "mwItem.h"
#include "mwLift.h"
#include "mwTriggerEvent.h"
#include "mwLevel.h"
#include "mwShot.h"
#include "mwLog.h"
#include "mwLoop.h"


mwNetgame mNetgame;

mwNetgame::mwNetgame()
{
   NetworkDriver = -1;
   ima_server = 0;
   ima_client = 0;
   remote_join_reply = 0;

   sprintf(serveraddress, "192.168.1.2");
   zlib_cmp = 7;

   for (int i=0; i<20; i++)
      files_to_send[i].active = 0;

   ServerChannel = NULL;
   ClientNum = 0;
   ListenChannel = NULL;                   // listen channel
   ClientChannel[MAX_CLIENTS] = {NULL, };  // array of channels for each client
}



// initialize libnet and setup a driver to use. Returns 0 on success.
int mwNetgame::NetworkInit(void)
{
   NET_DRIVERLIST drivers;
   NET_DRIVERNAME *drivernames;

   net_init(); // initialize libnet
   net_loadconfig(NULL); // configure libnet

   // detect drivers in the internet class
   drivers = net_detectdrivers(net_classes[NET_CLASS_INET].drivers);
   drivernames = net_getdrivernames(drivers);

   NetworkDriver = drivernames[0].num; // use first detected driver

   free(drivernames);

   if (NetworkDriver >= 0)
   {
      if (net_initdriver(NetworkDriver)) return 0;
      else
      {
         // Error: Couldn't initialize network driver
         return -1;
      }
   }
   else
   {
      // Error: Found no driver in the internet network class
      return -1;
   }
}





void mwNetgame::game_vars_to_state(char * b)
{
   int sz = 0, offset = 0;
   offset += sz; sz = sizeof(mPlayer.syn);         memcpy(b+offset, mPlayer.syn,         sz);
   offset += sz; sz = sizeof(mEnemy.Ei);           memcpy(b+offset, mEnemy.Ei,           sz);
   offset += sz; sz = sizeof(mEnemy.Ef);           memcpy(b+offset, mEnemy.Ef,           sz);
   offset += sz; sz = sizeof(mItem.item);          memcpy(b+offset, mItem.item,          sz);
   offset += sz; sz = sizeof(mItem.itemf);         memcpy(b+offset, mItem.itemf,         sz);
   offset += sz; sz = sizeof(mLift.cur);           memcpy(b+offset, mLift.cur,           sz);
   offset += sz; sz = sizeof(mLevel.l);            memcpy(b+offset, mLevel.l,            sz);
   offset += sz; sz = sizeof(mShot.p);             memcpy(b+offset, mShot.p,             sz);
   offset += sz; sz = sizeof(mShot.e);             memcpy(b+offset, mShot.e,             sz);
   offset += sz; sz = sizeof(mTriggerEvent.event); memcpy(b+offset, mTriggerEvent.event, sz);
}

void mwNetgame::state_to_game_vars(char * b)
{
   int sz = 0, offset = 0;
   sz = sizeof(mPlayer.syn);         memcpy(mPlayer.syn,         b+offset, sz); offset += sz;
   sz = sizeof(mEnemy.Ei);           memcpy(mEnemy.Ei,           b+offset, sz); offset += sz;
   sz = sizeof(mEnemy.Ef);           memcpy(mEnemy.Ef,           b+offset, sz); offset += sz;
   sz = sizeof(mItem.item);          memcpy(mItem.item,          b+offset, sz); offset += sz;
   sz = sizeof(mItem.itemf);         memcpy(mItem.itemf,         b+offset, sz); offset += sz;
   sz = sizeof(mLift.cur);           memcpy(mLift.cur,           b+offset, sz); offset += sz;
   sz = sizeof(mLevel.l);            memcpy(mLevel.l,            b+offset, sz); offset += sz;
   sz = sizeof(mShot.p);             memcpy(mShot.p,             b+offset, sz); offset += sz;
   sz = sizeof(mShot.e);             memcpy(mShot.e,             b+offset, sz); offset += sz;
   sz = sizeof(mTriggerEvent.event); memcpy(mTriggerEvent.event, b+offset, sz); offset += sz;
}

void mwNetgame::get_state_dif(char *a, char *b, char *c, int size)
{
   for (int i=0; i<size; i++) c[i] = a[i] - b[i];
}

void mwNetgame::apply_state_dif(char *a, char *c, int size)
{
   for (int i=0; i<size; i++) a[i] -= c[i];
}

void mwNetgame::reset_states(void)
{
   // reset dif buffer and frame_nums
   memset(client_state_dif, 0, STATE_SIZE);
   client_state_dif_src = -1; // -1 will never match a frame_num
   client_state_dif_dst = -1;

   // reset buffer and frame_nums used to build compressed state from packets
   memset(client_state_buffer, 0, STATE_SIZE);
   for (int i=0; i<16; i++) client_state_buffer_pieces[i] = -1;

   for (int i=0; i<8; i++) mStateHistory[i].initialize();

   server_dirty_frame = -1;
}


void mwNetgame::process_bandwidth_counters(int p)
{
   // get max per frame
   if (mPlayer.loc[p].tx_current_packets_for_this_frame > mPlayer.loc[p].tx_max_packets_per_frame) mPlayer.loc[p].tx_max_packets_per_frame = mPlayer.loc[p].tx_current_packets_for_this_frame;
   if (mPlayer.loc[p].rx_current_packets_for_this_frame > mPlayer.loc[p].rx_max_packets_per_frame) mPlayer.loc[p].rx_max_packets_per_frame = mPlayer.loc[p].rx_current_packets_for_this_frame;
   if (mPlayer.loc[p].tx_current_bytes_for_this_frame   > mPlayer.loc[p].tx_max_bytes_per_frame)   mPlayer.loc[p].tx_max_bytes_per_frame   = mPlayer.loc[p].tx_current_bytes_for_this_frame;
   if (mPlayer.loc[p].rx_current_bytes_for_this_frame   > mPlayer.loc[p].rx_max_bytes_per_frame)   mPlayer.loc[p].rx_max_bytes_per_frame   = mPlayer.loc[p].rx_current_bytes_for_this_frame;

   // get totals
   mPlayer.loc[p].tx_total_bytes   += mPlayer.loc[p].tx_current_bytes_for_this_frame;
   mPlayer.loc[p].rx_total_bytes   += mPlayer.loc[p].rx_current_bytes_for_this_frame;
   mPlayer.loc[p].tx_total_packets += mPlayer.loc[p].tx_current_packets_for_this_frame;
   mPlayer.loc[p].rx_total_packets += mPlayer.loc[p].rx_current_packets_for_this_frame;

   // add to tallies
   mPlayer.loc[p].tx_bytes_tally   += mPlayer.loc[p].tx_current_bytes_for_this_frame;
   mPlayer.loc[p].rx_bytes_tally   += mPlayer.loc[p].rx_current_bytes_for_this_frame;
   mPlayer.loc[p].tx_packets_tally += mPlayer.loc[p].tx_current_packets_for_this_frame;
   mPlayer.loc[p].rx_packets_tally += mPlayer.loc[p].rx_current_packets_for_this_frame;

   // reset counts for this frame
   mPlayer.loc[p].tx_current_bytes_for_this_frame   = 0;
   mPlayer.loc[p].rx_current_bytes_for_this_frame   = 0;
   mPlayer.loc[p].tx_current_packets_for_this_frame = 0;
   mPlayer.loc[p].rx_current_packets_for_this_frame = 0;

   if (mLoop.frame_num % 40 == 0) // tally freq = 40 frames = 1s
   {
      // get maximums per tally
      if (mPlayer.loc[p].tx_bytes_per_tally   > mPlayer.loc[p].tx_max_bytes_per_tally)   mPlayer.loc[p].tx_max_bytes_per_tally   = mPlayer.loc[p].tx_bytes_per_tally;
      if (mPlayer.loc[p].rx_bytes_per_tally   > mPlayer.loc[p].rx_max_bytes_per_tally)   mPlayer.loc[p].rx_max_bytes_per_tally   = mPlayer.loc[p].rx_bytes_per_tally;
      if (mPlayer.loc[p].tx_packets_per_tally > mPlayer.loc[p].tx_max_packets_per_tally) mPlayer.loc[p].tx_max_packets_per_tally = mPlayer.loc[p].tx_packets_per_tally;
      if (mPlayer.loc[p].rx_packets_per_tally > mPlayer.loc[p].rx_max_packets_per_tally) mPlayer.loc[p].rx_max_packets_per_tally = mPlayer.loc[p].rx_packets_per_tally;

      // copy to display variables
      mPlayer.loc[p].tx_bytes_per_tally   = mPlayer.loc[p].tx_bytes_tally;
      mPlayer.loc[p].rx_bytes_per_tally   = mPlayer.loc[p].rx_bytes_tally;
      mPlayer.loc[p].tx_packets_per_tally = mPlayer.loc[p].tx_packets_tally;
      mPlayer.loc[p].rx_packets_per_tally = mPlayer.loc[p].rx_packets_tally;

      mLog.addf(LOG_NET_bandwidth, p, "bandwidth txb:[%d] rxb:[%d] txp:[%d] rxp:[%d]\n", mPlayer.loc[p].tx_bytes_per_tally, mPlayer.loc[p].rx_bytes_per_tally, mPlayer.loc[p].tx_packets_per_tally, mPlayer.loc[p].rx_packets_per_tally);

      // reset tallies
      mPlayer.loc[p].tx_bytes_tally   = 0;
      mPlayer.loc[p].rx_bytes_tally   = 0;
      mPlayer.loc[p].tx_packets_tally = 0;
      mPlayer.loc[p].rx_packets_tally = 0;
   }
}


