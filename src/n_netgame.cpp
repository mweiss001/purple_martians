// n_netgame.cpp

#include "pm.h"
#include "mwPlayers.h"
#include "n_netgame.h"
#include "mwLift.h"
#include "mwPMEvent.h"
#include "mwItems.h"
#include "mwEnemy.h"
#include "mwLevel.h"
#include "mwShots.h"


// ------------------------------------------------
// ----------------- netgame ----------------------
// ------------------------------------------------
int ima_server = 0;
int ima_client = 0;
char m_serveraddress[256] = "192.168.1.2";
int TCP = 0;
int zlib_cmp = 7;


// server's copies of client states
char srv_client_state[8][2][STATE_SIZE];
int srv_client_state_frame_num[8][2];

// local client's states
char client_state_buffer[STATE_SIZE];  // buffer for building compressed dif from packet pieces
int  client_state_buffer_pieces[16];   // to mark packet pieces as received
char client_state_base[STATE_SIZE];    // last ack state
int  client_state_base_frame_num;      // last ack state mwPS.frame_num
char client_state_dif[STATE_SIZE];     // uncompressed dif
int  client_state_dif_src;             // uncompressed dif src mwPS.frame_num
int  client_state_dif_dst;             // uncompressed dif dst mwPS.frame_num

void game_vars_to_state(char * b)
{
   int sz = 0, offset = 0;
   offset += sz; sz = sizeof(mPlayer.syn); memcpy(b+offset, mPlayer.syn, sz);
   offset += sz; sz = sizeof(mEnemy.Ei);   memcpy(b+offset, mEnemy.Ei,   sz);
   offset += sz; sz = sizeof(mEnemy.Ef);   memcpy(b+offset, mEnemy.Ef,   sz);
   offset += sz; sz = sizeof(mItem.item);  memcpy(b+offset, mItem.item,  sz);
   offset += sz; sz = sizeof(mItem.itemf); memcpy(b+offset, mItem.itemf, sz);
   offset += sz; sz = sizeof(mLift.cur);   memcpy(b+offset, mLift.cur,   sz);
   offset += sz; sz = sizeof(mLevel.l);    memcpy(b+offset, mLevel.l,    sz);
   offset += sz; sz = sizeof(mShot.p);     memcpy(b+offset, mShot.p,     sz);
   offset += sz; sz = sizeof(mShot.e);     memcpy(b+offset, mShot.e,     sz);
   offset += sz; sz = sizeof(mwPME.event); memcpy(b+offset, mwPME.event, sz);
}

void state_to_game_vars(char * b)
{
   int sz = 0, offset = 0;
   sz = sizeof(mPlayer.syn); memcpy(mPlayer.syn, b+offset, sz); offset += sz;
   sz = sizeof(mEnemy.Ei);   memcpy(mEnemy.Ei,   b+offset, sz); offset += sz;
   sz = sizeof(mEnemy.Ef);   memcpy(mEnemy.Ef,   b+offset, sz); offset += sz;
   sz = sizeof(mItem.item);  memcpy(mItem.item,  b+offset, sz); offset += sz;
   sz = sizeof(mItem.itemf); memcpy(mItem.itemf, b+offset, sz); offset += sz;
   sz = sizeof(mLift.cur);   memcpy(mLift.cur,   b+offset, sz); offset += sz;
   sz = sizeof(mLevel.l);    memcpy(mLevel.l,    b+offset, sz); offset += sz;
   sz = sizeof(mShot.p);     memcpy(mShot.p,     b+offset, sz); offset += sz;
   sz = sizeof(mShot.e);     memcpy(mShot.e,     b+offset, sz); offset += sz;
   sz = sizeof(mwPME.event); memcpy(mwPME.event, b+offset, sz); offset += sz;
}

void get_state_dif(char *a, char *b, char *c, int size)
{
   for (int i=0; i<size; i++)
      c[i] = a[i] - b[i];
}

void apply_state_dif(char *a, char *c, int size)
{
   for (int i=0; i<size; i++)
      a[i] -= c[i];
}






void reset_states(void)
{
   // reset base state on client
   memset(client_state_base, 0, STATE_SIZE);
   client_state_base_frame_num = 0;  // mwPS.frame_num id

   // reset dif buffer and mwPS.frame_num
   memset(client_state_dif, 0, STATE_SIZE);
   client_state_dif_src = -1; // -1 will never match a mwPS.frame_num
   client_state_dif_dst = -1;

   // reset buffer and frame_nums used to build compressed state from packets
   memset(client_state_buffer, 0, STATE_SIZE);
   for (int i=0; i<16; i++)
      client_state_buffer_pieces[i] = -1;

   // reset server's client states
   for (int i=0; i<8; i++)
   {
      memset(srv_client_state[i][0], 0, STATE_SIZE);
      memset(srv_client_state[i][1], 0, STATE_SIZE);
      srv_client_state_frame_num[i][0] = 0;  // src
      srv_client_state_frame_num[i][1] = -3; // dst
   }
}


void reset_client_state(int p) // server only
{
   memset(srv_client_state[p][0], 0, STATE_SIZE);
   memset(srv_client_state[p][1], 0, STATE_SIZE);
   srv_client_state_frame_num[p][0] = 0;  // src
   srv_client_state_frame_num[p][1] = -3; // dst
}
