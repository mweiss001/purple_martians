// mwNetgame.cpp

#include "pm.h"
#include "mwNetgame.h"
#include "mwGameState.h"
#include "mwTriggerEvent.h"
#include "mwLevel.h"
#include "mwShot.h"
#include "mwLog.h"
#include "mwLoop.h"
#include "mwInput.h"

mwNetgame mNetgame;

mwNetgame::mwNetgame()
{
   networkDriver = -1;
   ima_server = 0;
   ima_client = 0;
   sprintf(server_address, "purplemartians.org");
   server_port = 24785;
   zlib_cmp = 7;

   mGameState.server_force_fakekey = 0;
   mGameState.server_force_client_offset = 0;
   mGameState.client_chase_offset = 0;

   for (int i=0; i<20; i++) files_to_send[i].active = 0;
   for (int i=0; i<8; i++) clear_channel(i);
   Channel = NULL;
}

// initialize libnet and setup a driver to use. Returns 0 on success.
int mwNetgame::networkInit()
{
   if (net_init()) // initialize libnet
   {
      mLog.log_error("Couldn't initialize libnet\n");
      return -1;
   }

   // detect drivers in the internet class
   NET_DRIVERLIST drivers = net_detectdrivers(net_classes[NET_CLASS_INET].drivers);
   NET_DRIVERNAME *drivernames = net_getdrivernames(drivers);
   networkDriver = drivernames[0].num; // use first detected driver
   free(drivernames);

   if (networkDriver >= 0)
   {
      if (!net_initdriver(networkDriver))
      {
         mLog.log_error("Couldn't initialize network driver\n");
         return -1;
      }
   }
   else
   {
      mLog.log_error("Found no driver in the internet network class\n");
      return -1;
   }
   return 0;
}

void mwNetgame::networkExit()
{
   if (Channel) net_closechannel(Channel);
   Channel = NULL;
   net_shutdown();
}

// read and discard all waiting packets
void mwNetgame::channelFlush()
{
   char data[PACKET_BUFFER_SIZE] = {0};
   while (net_receive(Channel, data, PACKET_BUFFER_SIZE, NULL));
}

void mwNetgame::clear_channel(int c)
{
   mwChannels[c].channel_active = 0;
   strcpy(mwChannels[c].channel_address, "");
}


bool mwNetgame::active()
{
   if (ima_server || ima_client) return true;
   return false;
}



void mwNetgame::game_vars_to_state(char * b)
{
   int sz = 0, offset = 0;
   offset += sz; sz = sizeof(mLevel.mPlayer.syn);              memcpy(b+offset, mLevel.mPlayer.syn,              sz);
   offset += sz; sz = sizeof(mLevel.mEnemy.Ei);                memcpy(b+offset, mLevel.mEnemy.Ei,                sz);
   offset += sz; sz = sizeof(mLevel.mEnemy.Ef);                memcpy(b+offset, mLevel.mEnemy.Ef,                sz);
   offset += sz; sz = sizeof(mLevel.mItem.item);               memcpy(b+offset, mLevel.mItem.item,               sz);
   offset += sz; sz = sizeof(mLevel.mItem.itemf);              memcpy(b+offset, mLevel.mItem.itemf,              sz);
   offset += sz; sz = sizeof(mLevel.mLift.cur);                memcpy(b+offset, mLevel.mLift.cur,                sz);
   offset += sz; sz = sizeof(mLevel.l);                        memcpy(b+offset, mLevel.l,                        sz);
   offset += sz; sz = sizeof(mShot.p);                         memcpy(b+offset, mShot.p,                         sz);
   offset += sz; sz = sizeof(mShot.e);                         memcpy(b+offset, mShot.e,                         sz);
   offset += sz; sz = sizeof(mTriggerEvent.event);             memcpy(b+offset, mTriggerEvent.event,             sz);
   offset += sz; sz = sizeof(mGameState);                      memcpy(b+offset, &mGameState,                     sz);
   offset += sz; sz = sizeof(mLevel.area_array);               memcpy(b+offset, mLevel.area_array,               sz);
   offset += sz; sz = sizeof(mLevel.data);                     memcpy(b+offset, mLevel.data,                     sz);
   offset += sz; sz = sizeof(mLevel.overworld_area_array);     memcpy(b+offset, mLevel.overworld_area_array,     sz);
   mLevel.mItem.mMessageText.VtoA();
   offset += sz; sz = sizeof(mLevel.mItem.mMessageText.messageTextA); memcpy(b+offset, mLevel.mItem.mMessageText.messageTextA, sz);
}

void mwNetgame::state_to_game_vars(char * b)
{
   int sz = 0, offset = 0;
   sz = sizeof(mLevel.mPlayer.syn);              memcpy(mLevel.mPlayer.syn,              b+offset, sz); offset += sz;
   sz = sizeof(mLevel.mEnemy.Ei);                memcpy(mLevel.mEnemy.Ei,                b+offset, sz); offset += sz;
   sz = sizeof(mLevel.mEnemy.Ef);                memcpy(mLevel.mEnemy.Ef,                b+offset, sz); offset += sz;
   sz = sizeof(mLevel.mItem.item);               memcpy(mLevel.mItem.item,               b+offset, sz); offset += sz;
   sz = sizeof(mLevel.mItem.itemf);              memcpy(mLevel.mItem.itemf,              b+offset, sz); offset += sz;
   sz = sizeof(mLevel.mLift.cur);                memcpy(mLevel.mLift.cur,                b+offset, sz); offset += sz;
   sz = sizeof(mLevel.l);                        memcpy(mLevel.l,                        b+offset, sz); offset += sz;
   sz = sizeof(mShot.p);                         memcpy(mShot.p,                         b+offset, sz); offset += sz;
   sz = sizeof(mShot.e);                         memcpy(mShot.e,                         b+offset, sz); offset += sz;
   sz = sizeof(mTriggerEvent.event);             memcpy(mTriggerEvent.event,             b+offset, sz); offset += sz;
   sz = sizeof(mGameState);                      memcpy(&mGameState,                     b+offset, sz); offset += sz;
   sz = sizeof(mLevel.area_array);               memcpy(mLevel.area_array,               b+offset, sz); offset += sz;
   sz = sizeof(mLevel.data);                     memcpy(mLevel.data,                     b+offset, sz); offset += sz;
   sz = sizeof(mLevel.overworld_area_array);     memcpy(mLevel.overworld_area_array,     b+offset, sz); offset += sz;
   sz = sizeof(mLevel.mItem.mMessageText.messageTextA); memcpy(mLevel.mItem.mMessageText.messageTextA, b+offset, sz); offset += sz;
   mLevel.mItem.mMessageText.AtoV();
}

void mwNetgame::get_state_dif(char *a, char *b, char *c, int size)
{
   for (int i=0; i<size; i++) c[i] = a[i] - b[i];
}

void mwNetgame::apply_state_dif(char *a, char *c, int size)
{
   for (int i=0; i<size; i++) a[i] -= c[i];
}

void mwNetgame::reset_states()
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
   if (mLevel.mPlayer.loc[p].tx_current_packets_for_this_frame > mLevel.mPlayer.loc[p].tx_max_packets_per_frame) mLevel.mPlayer.loc[p].tx_max_packets_per_frame = mLevel.mPlayer.loc[p].tx_current_packets_for_this_frame;
   if (mLevel.mPlayer.loc[p].rx_current_packets_for_this_frame > mLevel.mPlayer.loc[p].rx_max_packets_per_frame) mLevel.mPlayer.loc[p].rx_max_packets_per_frame = mLevel.mPlayer.loc[p].rx_current_packets_for_this_frame;
   if (mLevel.mPlayer.loc[p].tx_current_bytes_for_this_frame   > mLevel.mPlayer.loc[p].tx_max_bytes_per_frame)   mLevel.mPlayer.loc[p].tx_max_bytes_per_frame   = mLevel.mPlayer.loc[p].tx_current_bytes_for_this_frame;
   if (mLevel.mPlayer.loc[p].rx_current_bytes_for_this_frame   > mLevel.mPlayer.loc[p].rx_max_bytes_per_frame)   mLevel.mPlayer.loc[p].rx_max_bytes_per_frame   = mLevel.mPlayer.loc[p].rx_current_bytes_for_this_frame;

   // get totals
   mLevel.mPlayer.loc[p].tx_total_bytes   += mLevel.mPlayer.loc[p].tx_current_bytes_for_this_frame;
   mLevel.mPlayer.loc[p].rx_total_bytes   += mLevel.mPlayer.loc[p].rx_current_bytes_for_this_frame;
   mLevel.mPlayer.loc[p].tx_total_packets += mLevel.mPlayer.loc[p].tx_current_packets_for_this_frame;
   mLevel.mPlayer.loc[p].rx_total_packets += mLevel.mPlayer.loc[p].rx_current_packets_for_this_frame;

   // add to tallies
   mLevel.mPlayer.loc[p].tx_bytes_tally   += mLevel.mPlayer.loc[p].tx_current_bytes_for_this_frame;
   mLevel.mPlayer.loc[p].rx_bytes_tally   += mLevel.mPlayer.loc[p].rx_current_bytes_for_this_frame;
   mLevel.mPlayer.loc[p].tx_packets_tally += mLevel.mPlayer.loc[p].tx_current_packets_for_this_frame;
   mLevel.mPlayer.loc[p].rx_packets_tally += mLevel.mPlayer.loc[p].rx_current_packets_for_this_frame;

   // reset counts for this frame
   mLevel.mPlayer.loc[p].tx_current_bytes_for_this_frame   = 0;
   mLevel.mPlayer.loc[p].rx_current_bytes_for_this_frame   = 0;
   mLevel.mPlayer.loc[p].tx_current_packets_for_this_frame = 0;
   mLevel.mPlayer.loc[p].rx_current_packets_for_this_frame = 0;

   if (mLoop.frame_num % 40 == 0) // tally freq = 40 frames = 1s
   {
      // get maximums per tally
      if (mLevel.mPlayer.loc[p].tx_bytes_per_tally   > mLevel.mPlayer.loc[p].tx_max_bytes_per_tally)   mLevel.mPlayer.loc[p].tx_max_bytes_per_tally   = mLevel.mPlayer.loc[p].tx_bytes_per_tally;
      if (mLevel.mPlayer.loc[p].rx_bytes_per_tally   > mLevel.mPlayer.loc[p].rx_max_bytes_per_tally)   mLevel.mPlayer.loc[p].rx_max_bytes_per_tally   = mLevel.mPlayer.loc[p].rx_bytes_per_tally;
      if (mLevel.mPlayer.loc[p].tx_packets_per_tally > mLevel.mPlayer.loc[p].tx_max_packets_per_tally) mLevel.mPlayer.loc[p].tx_max_packets_per_tally = mLevel.mPlayer.loc[p].tx_packets_per_tally;
      if (mLevel.mPlayer.loc[p].rx_packets_per_tally > mLevel.mPlayer.loc[p].rx_max_packets_per_tally) mLevel.mPlayer.loc[p].rx_max_packets_per_tally = mLevel.mPlayer.loc[p].rx_packets_per_tally;

      // copy to display variables
      mLevel.mPlayer.loc[p].tx_bytes_per_tally   = mLevel.mPlayer.loc[p].tx_bytes_tally;
      mLevel.mPlayer.loc[p].rx_bytes_per_tally   = mLevel.mPlayer.loc[p].rx_bytes_tally;
      mLevel.mPlayer.loc[p].tx_packets_per_tally = mLevel.mPlayer.loc[p].tx_packets_tally;
      mLevel.mPlayer.loc[p].rx_packets_per_tally = mLevel.mPlayer.loc[p].rx_packets_tally;

      mLog.add(LOG_NET_BANDWIDTH, 0, p, mLevel.mPlayer.loc[p].tx_bytes_per_tally, mLevel.mPlayer.loc[p].rx_bytes_per_tally, mLevel.mPlayer.loc[p].tx_packets_per_tally, mLevel.mPlayer.loc[p].rx_packets_per_tally);

      // reset tallies
      mLevel.mPlayer.loc[p].tx_bytes_tally   = 0;
      mLevel.mPlayer.loc[p].rx_bytes_tally   = 0;
      mLevel.mPlayer.loc[p].tx_packets_tally = 0;
      mLevel.mPlayer.loc[p].rx_packets_tally = 0;
   }
}

