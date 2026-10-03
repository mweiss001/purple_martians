// mwNetgameClient.cpp

#include "pm.h"
#include "mwNetgame.h"
#include "mwPacketBuffer.h"
#include "mwLog.h"
#include "mwPlayer.h"
#include "mwDisplay.h"
#include "mwScreen.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwColor.h"
#include "mwInput.h"
#include "mwEventQueue.h"
#include "mwLevel.h"
#include "mwConfig.h"
#include "mwLoop.h"
#include "mwQuickGraph2.h"
#include "mwGameMoves.h"
#include "mwMiscFnx.h"


int mwNetgame::clientInitNetwork()
{
   char target[300];
   sprintf(target, "%s", "");

   int error = 0;
   if (networkInit())
   {
      //mLog.log_error("failed to initialize network");
      error = 1;
   }
   if (!error)
   {
      Channel = net_openchannel(networkDriver, NULL); // dynamic port
      if (Channel == NULL)
      {
         //mLog.log_error("failed to create NetChannel");
         error = 2;
      }
      if (!error)
      {
         sprintf(target, "%s:%d", server_address, server_port);
         if (net_assigntarget(Channel, target))
         {
            //char msg[512];
            //sprintf(msg, "client failed to set NetChannel target:[%s]", target);
            //mLog.log_error(msg);
            error = 3;
         }
      }
   }

   mLog.add(LOG_NET, LOG_NET_SUBTYPE_CLIENT_INIT, -1, error,0,0,0,0,0,0,0,0,0, target, net_getlocaladdress(Channel));




   if (error) return 0;
   return 1;
}

int mwNetgame::clientJoin()
{
   // Check for reply from server
   int tries = 1;        // number of times to try
   float try_delay = 1;  // delay between tries
   int got_reply = 0;
   while (!got_reply)
   {
      //mLog.add_fwf(LOG_NET, -1, 76, 10, "|", " ", "ClientCheckResponse %d", tries);
      int ccr = clientCheckResponse();
      if (ccr == 1)
      {
         got_reply = 1;
         // mLog.add_fw(LOG_NET, -1, 76, 10, "|", " ", "Got 'join accepted' reply from server");
      }
      if (ccr == 0)
      {
         al_rest(try_delay);
         if (++tries > 2) // "no reply from server"
         {
            mLog.add(LOG_NET, LOG_NET_SUBTYPE_CLIENT_WAIT, -1, 1);
            return 0;
         }
      }
      if (ccr == -1) // "'server full' reply from server"
      {
         mLog.add(LOG_NET, LOG_NET_SUBTYPE_CLIENT_WAIT, -1, 2);
         return 0;
      }
      if (ccr == -2) // "Cancelled"
      {
         mLog.add(LOG_NET, LOG_NET_SUBTYPE_CLIENT_WAIT, -1, 3);
         return 0;
      }
   }

   ima_client = 1;
   return 1;
}

int mwNetgame::clientCheckResponse() // check for a response from the server
{
   client_send_cjon_packet();
   mLog.add(LOG_NET, LOG_NET_SUBTYPE_CLIENT_CJON, -1);

   al_set_target_backbuffer(mDisplay.display);
   al_clear_to_color(al_map_rgb(0,0,0));
   mScreen.rtextout_centref(mFont.pr8, NULL, mDisplay.SCREEN_W/2, 2*mDisplay.SCREEN_H/4, 15, -2, 1, "Waiting for Server:[%s]", server_address);
   mScreen.rtextout_centre( mFont.pr8, NULL, mDisplay.SCREEN_W/2, 3*mDisplay.SCREEN_H/4, 15, -2, 1, "             ESC to cancel             ");
   al_flip_display();

   char prog[41];
   sprintf(prog, "                                        ");


   char data[PACKET_BUFFER_SIZE];
   char address[32];
   int done = 0;
   while (done < 40)
   {
      mEventQueue.proc(1);
      if (mInput.key[ALLEGRO_KEY_ESCAPE][1]) return -2; // cancelled

      if (net_receive(Channel, data, PACKET_BUFFER_SIZE, address))
      {
         if (mPacketBuffer.PacketRead(data, "sjon"))
         {
            return client_proc_sjon_packet(data); // 1=join, -1=full
         }
      }
      prog[done] = '.';
      mScreen.rtextout_centref(mFont.pr8, NULL, mDisplay.SCREEN_W/2, 5*mDisplay.SCREEN_H/8, 15, -2, 1, "%s", prog);
      al_flip_display();
      al_rest(.1);
      done++;
   }
   return 0; // no response
}

int mwNetgame::remoteJoin()
{
   client_send_cjrc_packet();
   int reply = 0;
   while (!reply)
   {
      char data[PACKET_BUFFER_SIZE];
      if (net_receive(Channel, data, PACKET_BUFFER_SIZE, NULL))
         if (mPacketBuffer.PacketRead(data, "sjrc")) reply = 1;

      mEventQueue.proc(1);
      al_set_target_backbuffer(mDisplay.display);
      al_clear_to_color(al_map_rgb(0,0,0));
      al_draw_textf(mFont.pr8, mColor.pc[10], 100, 80,  0, "Remote Control - Waiting for server reply");
      al_flip_display();

      if (mInput.key[ALLEGRO_KEY_ESCAPE][0])
      {
         while (mInput.key[ALLEGRO_KEY_ESCAPE][0]) mEventQueue.proc(1);
         return 0;
      }
   }
   return 1;
}

void mwNetgame::clientExitNetwork()
{
   mLog.add(LOG_NET, LOG_NET_SUBTYPE_CLIENT_STOP, -1);

   networkExit();

   ima_client = 0;
   // reset player data
   for (int p=0; p<NUM_PLAYERS; p++) mPlayer.init_player(p, 1);
   mPlayer.syn[0].active = 1; // local_control
   mPlayer.active_local_player = 0;
   mConfig.load_config(); // to restore local player color, last gate touched, and other settings
}

void mwNetgame::clientSend(void *data, int len)
{
   net_send(Channel, data, len);
   mPlayer.loc[mPlayer.active_local_player].tx_current_bytes_for_this_frame+= (len + 42);
   mPlayer.loc[mPlayer.active_local_player].tx_current_packets_for_this_frame++;
}


void mwNetgame::client_send_cjon_packet()
{
   char data[PACKET_BUFFER_SIZE] = {0}; int pos;
   mPacketBuffer.PacketName(data, pos, "cjon");
   mPacketBuffer.PacketPutInt32(  data, pos, mPlayer.syn[0].color); // requested color
   mPacketBuffer.PacketAddStringN(data, pos, mPlayer.syn[0].name);  // player name
   mPacketBuffer.PacketAddStringN(data, pos, mLoop.local_hostname); // hostname
   clientSend(data, pos);
}

void mwNetgame::client_send_cjrc_packet()
{
   char data[PACKET_BUFFER_SIZE] = {0}; int pos;
   mPacketBuffer.PacketName(data, pos, "cjrc");
   clientSend(data, pos);
}

void mwNetgame::client_send_rctl_packet(int type, double val)
{
   mLoop.remote_frames_since_last_rctl_sent = 0;

   char data[PACKET_BUFFER_SIZE] = {0}; int pos;
   mPacketBuffer.PacketName(data, pos, "rctl");
   mPacketBuffer.PacketPutInt32(data, pos, type);
   mPacketBuffer.PacketPutDouble(data, pos, val);
   clientSend(data, pos);
}


void mwNetgame::client_proc_pong_packet(char *data)
{
   int p = mPlayer.active_local_player;
   int pos = 4;
   double t0 = mPacketBuffer.PacketGetDouble(data, pos);
   double t1 = mPacketBuffer.PacketGetDouble(data, pos);
   double ping = al_get_time() - t0;

   mPacketBuffer.RA[p].add_data(ping);
   mPacketBuffer.PacketName(data, pos, "pang");
   mPacketBuffer.PacketPutDouble(data, pos, t1);
   clientSend(data, pos);

   mLog.add(LOG_NET_CLIENT_PING, 0, p, ping, mPlayer.loc[p].ping_avg);

   mLog.add_tmrf(LOG_TMR_client_ping, "ping:[%5.1f] avg:[%5.1f]\n", ping*1000, mPlayer.loc[p].ping_avg*1000);
}



void mwNetgame::client_proc_srrf_packet(int i)
{
   int val = mPacketBuffer.PacketGetInt32(i);
   char msg[256];
//   mLog.log_add_prefixed_textf(LOG_NET_FILE_TRANSFER, -1, "rx srrf - %s\n", mGameMoves.get_save_txt(val, msg)) ;
//   mLog.add(LOG_NET_FILE_TRANSFER, 4, 0,0,0,0,0,0,0,0,0, "rx srrf - ", mGameMoves.get_save_txt(val, msg)) ;

   mLog.add(LOG_NET_FILE_TRANSFER, 4, -1, 0,0,0,0,0,0,0,0,0,0, mGameMoves.get_save_txt(val, msg));



   // if blocking in CLIENT_PREEXIT2 state, advance to next state
   if ((val) && (mLoop.state[1] == PM_PROGRAM_STATE_CLIENT_PREEXIT2)) mLoop.state[0] = PM_PROGRAM_STATE_CLIENT_EXIT;
}



int mwNetgame::client_proc_sjon_packet(char * data)
{
   int pos = 4;
   int pl     = mPacketBuffer.PacketGetInt32(data, pos);  // play level
   int sfnum  = mPacketBuffer.PacketGetInt32(data, pos);  // server join frame number
   int p      = mPacketBuffer.PacketGetInt32(data, pos);  // client player number
   int color  = mPacketBuffer.PacketGetInt32(data, pos);  // client player color
   int slsn   = mPacketBuffer.PacketGetInt32(data, pos);  // server level sequence number

   if (p == 99) return -1; // server full, join denied
   else // join allowed
   {
      mPlayer.active_local_player = p;
      mPlayer.syn[p].active = 1;
      mPlayer.syn[p].control_method = PM_PLAYER_CONTROL_METHOD_CLIENT_LOCAL;
      mPlayer.syn[p].color = color;
      mNetgame.server_lev_seq_num = slsn;
      mMiscFnx.mw_strncpy(mPlayer.loc[0].hostname, server_address, 15);
      mMiscFnx.mw_strncpy(mPlayer.loc[p].hostname, mLoop.local_hostname, 15);
      mLevel.play_level = pl;
      mLog.add(LOG_NET, LOG_NET_SUBTYPE_CLIENT_SJON, -1, pl, color, sfnum, slsn);
      return 1;
   }
}


void mwNetgame::client_send_ping_packet()
{
   char data[PACKET_BUFFER_SIZE] = {0}; int pos;
   mPacketBuffer.PacketName(data, pos, "ping");
   mPacketBuffer.PacketPutDouble(data, pos, al_get_time());
   clientSend(data, pos);
}

void mwNetgame::client_proc_snfo_packet(int i)
{
   if (mLoop.state[1] == PM_PROGRAM_STATE_SERVER_REMOTE_CONTROL_RUN)
   {
      int seq     = mPacketBuffer.PacketGetInt32(i);
      int max_seq = mPacketBuffer.PacketGetInt32(i);
      int sb      = mPacketBuffer.PacketGetInt32(i);
      int sz      = mPacketBuffer.PacketGetInt32(i);
      int fn      = mPacketBuffer.PacketGetInt32(i); // frame_num

      //printf("rx snfo piece [%d of %d] frame:[%d] st:%4d sz:%4d\n", seq+1, max_seq, fn, sb, sz);

      // do some checks, because one time i received: rx snfo piece [121 of 218] frame:[994] st:1616929891 sz:1180829280
      int bad_data = 0;
      if ((max_seq < 1) || (max_seq > 6)) bad_data = 1;
      if ((seq < 0) || (seq > 6) || (seq > max_seq)) bad_data = 1;
      if ((sb < 0) || (sb > 5000)) bad_data = 1;
      if ((sz < 0) || (sz > PACKET_PAYLOAD_CHUNK_SIZE)) bad_data = 1;

      if (bad_data) printf("rx snfo piece [%d of %d] frame:[%d] st:%4d sz:%4d  --- Bad Data!\n", seq+1, max_seq, fn, sb, sz);
      else
      {
         //printf("rx snfo piece [%d of %d] frame:[%d] st:%4d sz:%4d\n", seq+1, max_seq, fn, sb, sz);

         memcpy(client_state_buffer + sb, mPacketBuffer.rx_buf[i].data+24, sz);    // put the piece of data in the buffer
         client_state_buffer_pieces[seq] = fn;                                     // mark it with frame_num

         int complete = 1;                                         // did we just get the last packet? (yes by default)
         for (int i=0; i< max_seq; i++)
            if (client_state_buffer_pieces[i] != fn) complete = 0; // no, if any piece not at latest frame_num

         if (complete)
         {
            //printf("rx snfo complete - frame:[%d]\n", fn);
            char dmp[5800]; //
            // uncompress
            uLongf destLen = sizeof(dmp);
            uncompress((Bytef*)dmp, (uLongf*)&destLen, (Bytef*)client_state_buffer, sizeof(client_state_buffer));

            // copy to variables
            int sz=0, offset=0;
            sz = sizeof(mPlayer.syn); memcpy(mPlayer.syn, dmp+offset, sz); offset += sz;
            sz = sizeof(mPlayer.loc); memcpy(mPlayer.loc, dmp+offset, sz); offset += sz;

            // dsync, ping graphs
            for (int i=1; i<8; i++) // cycle all clients
               if (mPlayer.syn[i].active)
               {
                  mQuickGraph2[1].series[i].active = 1;
                  mQuickGraph2[1].series[i].color = mPlayer.syn[i].color;
                  mQuickGraph2[1].add_data(i, mPlayer.loc[i].pdsync*1000, 0);

                  mQuickGraph2[2].series[i].active = 1;
                  mQuickGraph2[2].series[i].color = mPlayer.syn[i].color;
                  mQuickGraph2[2].add_data(i, mPlayer.loc[i].ping*1000, 0);

                  mQuickGraph2[3].series[i].active = 1;
                  mQuickGraph2[3].series[i].color = mPlayer.syn[i].color;
                  mQuickGraph2[3].add_data(i, mPlayer.loc[i].cmp_dif_size, 0);

                  mQuickGraph2[6].series[i].active = 1;
                  mQuickGraph2[6].series[i].color = mPlayer.syn[i].color;
                  mQuickGraph2[6].add_data(i, mPlayer.loc[i].client_loc_plr_cor, 0);

                  mQuickGraph2[7].series[i].active = 1;
                  mQuickGraph2[7].series[i].color = mPlayer.syn[i].color;
                  mQuickGraph2[7].add_data(i, mPlayer.loc[i].client_rmt_plr_cor, 0);
               }

            for (int i=0; i<8; i++) // cycle all players
               if (mPlayer.syn[i].active)
               {
                  mQuickGraph2[0].series[i].active = 1;
                  mQuickGraph2[0].series[i].color = mPlayer.syn[i].color;
                  mQuickGraph2[0].add_data(i, mPlayer.loc[i].cpu, 0);

                  mQuickGraph2[4].series[i].active = 1;
                  mQuickGraph2[4].series[i].color = mPlayer.syn[i].color;
                  mQuickGraph2[4].add_data(i, mPlayer.loc[i].tx_bytes_per_tally/1000, 0);

                  mQuickGraph2[5].series[i].active = 1;
                  mQuickGraph2[5].series[i].color = mPlayer.syn[i].color;
                  mQuickGraph2[5].add_data(i, mPlayer.loc[i].rewind, 0);
               }


            // if no clients are active, erase and scroll graph data (otherwise old data just keeps scrolling forever)
            int ca = 0;
            for (int i=1; i<8; i++)
               if (mPlayer.syn[i].active) ca++;

            if (ca == 0)
               for (int i=1; i<8; i++)
                  for (int j=0; j<8; j++) mQuickGraph2[j].add_data(i, 0, 1); // erase line only


            for (int i=0; i<8; i++) mQuickGraph2[i].new_entry_pos();
         }
      }
   }
}

void mwNetgame::client_proc_sfil_packet(int i)
{
   int seq     = mPacketBuffer.PacketGetInt32(i);
   int max_seq = mPacketBuffer.PacketGetInt32(i);
   int sb      = mPacketBuffer.PacketGetInt32(i);
   int sz      = mPacketBuffer.PacketGetInt32(i);
   int id      = mPacketBuffer.PacketGetInt32(i);
   int fsize   = mPacketBuffer.PacketGetInt32(i); // uncompressed file size

   //printf("rx sfil piece [%d of %d] id:[%d] st:%4d sz:%4d fsz:%4d\n", seq+1, max_seq, id, sb, sz, fsize);

   memcpy(client_sfil_buffer + sb, mPacketBuffer.rx_buf[i].data+28, sz);    // put the piece of data in the buffer
   client_sfil_buffer_pieces[seq] = id;                                     // mark it with id

   int complete = 1;                                         // did we just get the last piece? (yes by default)
   for (int i=0; i< max_seq; i++)
      if (client_sfil_buffer_pieces[i] != id) complete = 0;  // no, if any piece does not have latest id

   if (complete)
   {
      // uncompress
      //char dmp[fsize + 128];

      char dmp[200128];

      uLongf destLen = sizeof(dmp);
      uncompress((Bytef*)dmp, (uLongf*)&destLen, (Bytef*)client_sfil_buffer, sizeof(client_sfil_buffer));

      // copy to variables
      char fname[256];
      memcpy(fname, dmp, sizeof(fname));

      char msg[1024];
      sprintf(msg, "rx %s size:[%d] id:[%d]", fname, fsize, id);
      mLog.add(LOG_NET_FILE_TRANSFER, 5, -1, 0,0,0,0,0,0,0,0,0,0, msg);

      // write to file
      FILE *fp = fopen(fname, "wb");
      if (!fp)
      {
         printf("Error opening %s", fname);
         return;
      }
      fwrite(dmp + 128, fsize, 1, fp);
      fclose(fp);

      client_send_sfak_packet(id); // send ack

      if (mLoop.state[1] == PM_PROGRAM_STATE_CLIENT_PREEXIT2) mLoop.state[0] = PM_PROGRAM_STATE_CLIENT_EXIT;
   }
}



void mwNetgame::client_send_sfak_packet(int id)
{
   // printf("Client ack id:%d\n", id);
   char data[PACKET_BUFFER_SIZE] = {0}; int pos;
   mPacketBuffer.PacketName(data, pos, "sfak");
   mPacketBuffer.PacketPutInt32(data, pos, id);
   clientSend(data, pos);
}

void mwNetgame::client_send_crfl()
{
   mLog.add(LOG_NET_FILE_TRANSFER, 6, -1, 0,0,0,0,0,0,0,0,0,0, "tx clrf - client request file");
   char data[PACKET_BUFFER_SIZE] = {0}; int pos;
   mPacketBuffer.PacketName(data, pos, "crfl");
   clientSend(data, pos);
}





void mwNetgame::client_proc_stdf_packet(int i)
{
   double t0 = al_get_time();

   int seq     = mPacketBuffer.PacketGetInt32(i);
   int max_seq = mPacketBuffer.PacketGetInt32(i);
   int sb      = mPacketBuffer.PacketGetInt32(i);
   int sz      = mPacketBuffer.PacketGetInt32(i);
   int dst     = mPacketBuffer.PacketGetInt32(i);
   int src     = mPacketBuffer.PacketGetInt32(i);
   int slsn    = mPacketBuffer.PacketGetInt32(i); // server level sequence num
   int sdln    = mPacketBuffer.PacketGetInt32(i); // server last level loaded

   if (slsn != server_lev_seq_num)
   {
      if (slsn == server_lev_seq_num +1) // slsn is from next level - setting next level
      {
         mLog.add(LOG_NET_DIF_TRX_PACKET, 2, -1, seq+1, max_seq, src, dst, sb, sz, sdln, slsn);
         mPlayer.syn[0].level_done_next_level = sdln;
         mLoop.state[0] = PM_PROGRAM_STATE_NEXT_LEVEL;
      }
      else // slsn bad
      {
         mLog.add(LOG_NET_DIF_TRX_PACKET, 3, -1, seq+1, max_seq, src, dst, sb, sz, sdln, slsn);
      }
      return;
   }
   mLog.add(LOG_NET_DIF_TRX_PACKET, 1, -1, seq+1, max_seq, src, dst, sb, sz, sdln, slsn);


   memcpy(client_state_buffer + sb, mPacketBuffer.rx_buf[i].data+36, sz);   // put the piece of data in the buffer

   client_state_buffer_pieces[seq] = dst;                     // mark it with destination mLoop.frame_num
   int complete = 1;                                          // did we just get the last packet? (yes by default)
   for (int i=0; i< max_seq; i++)
      if (client_state_buffer_pieces[i] != dst) complete = 0; // no, if any piece not at latest frame_num

   if (complete)
   {
      // uncompress client_state_buffer to dif
      uLongf destLen = sizeof(client_state_dif);
      uncompress((Bytef*)client_state_dif, (uLongf*)&destLen, (Bytef*)client_state_buffer, sizeof(client_state_buffer));

      if (destLen == STATE_SIZE)
      {
         mLog.add(LOG_NET_DIF_TRX, 1, -1, src, dst);
         client_state_dif_src = src; // mark dif data with new src and dst
         client_state_dif_dst = dst;
      }
      else
      {
         mLog.add(LOG_NET_DIF_TRX, 2, -1, src, dst);
         client_state_dif_src = -1; // mark dif data as bad
         client_state_dif_dst = -1;
      }
   }

   if (mLoop.frame_num) mLog.add_tmr1(LOG_TMR_cdif, "stdf", al_get_time() - t0);
}


void mwNetgame::client_apply_dif()
{
   double t0 = al_get_time();
   int p = mPlayer.active_local_player;


   // check if dif is valid
   if ((client_state_dif_src == -1) || (client_state_dif_dst == -1))
   {
      mLog.add(LOG_NET_DIF_APPLY, 0, -1, client_state_dif_src, client_state_dif_dst);
      return;
   }

   // check if dif_dest has already been applied (check if dif_dest is less than or equal to newest_state_frame_num)
   if (client_state_dif_dst <= mStateHistory[p].newest_state_frame_num)
   {
      mLog.add(LOG_NET_DIF_APPLY, 1, -1, client_state_dif_src, client_state_dif_dst);
      return;
   }

   // if we get this far, we know that dif is valid and dif destination is newer than last applied dif


   // now check if we have a base state that matches dif source
   char base[STATE_SIZE] = {0};
   int base_frame_num = 0;

   if (client_state_dif_src == 0) mPlayer.loc[p].client_base_resets++; // server sent a dif based on zero, leave base as is (all zero)
   else
   {
      // finds and sets base matching 'client_state_dif_src' -- if not found, leaves base as is (all zero)
      mStateHistory[p].get_base_state(base, base_frame_num, client_state_dif_src);

      if (base_frame_num == 0) // base was not found in history
      {
         int fn = mStateHistory[p].newest_state_frame_num; // get newest base we do have
         if (fn == -1) // no valid base states at all
         {
            // do not sent stak with -1 send it with 0
            client_send_stak_packet(0);
            mLog.add(LOG_NET_DIF_APPLY, 2, -1, client_state_dif_src, client_state_dif_dst, 0);
            return;
         }
         else
         {
            client_send_stak_packet(fn);
            mLog.add(LOG_NET_DIF_APPLY, 3, -1, client_state_dif_src, client_state_dif_dst, fn);
            return;
         }
      }
   }

   // if we got this far, a valid base has been found and we will be applying the dif

   // ------------------------------------------------
   // save things before applying dif
   // ------------------------------------------------

   // make a copy of level array l[][]
   int old_l[100][100];
   memcpy(old_l, mLevel.l, sizeof(mLevel.l));

   // make a copy of players' pos
   for (int pp=0; pp<NUM_PLAYERS; pp++)
      if (mPlayer.syn[pp].active)
      {
         mPlayer.loc[pp].old_x = mPlayer.syn[pp].x;
         mPlayer.loc[pp].old_y = mPlayer.syn[pp].y;
      }


   // apply dif to base
   apply_state_dif(base, client_state_dif, STATE_SIZE);

   // copy to game vars and set new frame number
   state_to_game_vars(base);
   mLoop.frame_num = client_state_dif_dst;

   // client keeps track of last dif applied and uses that to quit if no dif applied for a period of time
   mPlayer.loc[p].client_last_dif_applied_frame_num = mLoop.frame_num;

   // save to history
   mStateHistory[p].add_state(mLoop.frame_num);
   //mStateHistory[p].show_states("save frame:%d to history\n", mLoop.frame_num);


   // compare dif destination to current frame number
   int ff = mPlayer.loc[p].rewind = mLoop.frame_num - client_state_dif_dst;

   // add log entry
   mLog.add(LOG_NET_DIF_APPLY, 4, -1, client_state_dif_src, client_state_dif_dst, mLoop.frame_num, ff);


   // ------------------------------------------------
   // restore things after applying dif
   // ------------------------------------------------

   // fix control methods

   // on client, server is PM_PLAYER_CONTROL_METHOD_NETGAME_REMOTE
   mPlayer.syn[0].control_method = PM_PLAYER_CONTROL_METHOD_NETGAME_REMOTE;

   // change active local player (p) to PM_PLAYER_CONTROL_METHOD_CLIENT_LOCAL
   if (mPlayer.syn
       [p].control_method == PM_PLAYER_CONTROL_METHOD_NETGAME_REMOTE) mPlayer.syn[p].control_method = PM_PLAYER_CONTROL_METHOD_CLIENT_LOCAL;

   // server quit
   if (mPlayer.syn[p].control_method == PM_PLAYER_CONTROL_METHOD_CLIENT_ORPHAN) mLoop.state[0] = PM_PROGRAM_STATE_MENU;


   // compare old_l to l and redraw changed tiles
   // double t0 = al_get_time();
   al_set_target_bitmap(mBitmap.level_background);
   for (int x=0; x<100; x++)
      for (int y=0; y<100; y++)
         if (mLevel.l[x][y] != old_l[x][y])
         {
            // printf("dif at x:%d y:%d\n", x, y);
            al_draw_filled_rectangle(x*20, y*20, x*20+20, y*20+20, mColor.pc[0]);
            al_draw_bitmap(mBitmap.tile[mLevel.l[x][y] & PM_BTILE_TILENUM_MASK], x*20, y*20, 0);
         }
   // mLog.add_log_TMR(al_get_time() - t0, "oldl", 0);

   // ------------------------------------------------
   // if we rewound time, play it back
   // ------------------------------------------------
   if (ff > 0) mLoop.loop_frame(ff);

   // send acknowledgment
   client_send_stak_packet(client_state_dif_dst);


   // ------------------------------------------------
   // calc players' corrections
   // ------------------------------------------------
   mPlayer.loc[p].client_rmt_plr_cor = 0; // reset max remote
   for (int pp=0; pp<NUM_PLAYERS; pp++)
      if (mPlayer.syn[pp].active)
      {
         float cor = sqrt(pow((mPlayer.loc[pp].old_x - mPlayer.syn[pp].x), 2) + pow((mPlayer.loc[pp].old_y - mPlayer.syn[pp].y), 2));  // hypotenuse distance
         if (p == pp) mPlayer.loc[p].client_loc_plr_cor = cor; // save local cor
         else if (cor > mPlayer.loc[p].client_rmt_plr_cor) mPlayer.loc[p].client_rmt_plr_cor = cor; // save max remote cor
      }

   // add data to tally
   mTally_client_loc_plr_cor_last_sec[p].add_data(mPlayer.loc[p].client_loc_plr_cor);
   mTally_client_rmt_plr_cor_last_sec[p].add_data(mPlayer.loc[p].client_rmt_plr_cor);

   mLog.add_tmr1(LOG_TMR_cdif, "cdif", al_get_time() - t0);

}

void mwNetgame::client_send_cdat_packet(int p)
{
   char data[PACKET_BUFFER_SIZE] = {0}; int pos;
   mPacketBuffer.PacketName(data, pos, "cdat");
   mPacketBuffer.PacketPutInt32(data, pos, server_lev_seq_num);
   mPacketBuffer.PacketPutInt32(data, pos, p);
   mPacketBuffer.PacketPutInt32(data, pos, mLoop.frame_num);
   mPacketBuffer.PacketPutInt32(data, pos, mPlayer.loc[p].comp_move);
   clientSend(data, pos);
   mPlayer.loc[p].client_cdat_packets_tx++;


//   mLog.log_add_prefixed_textf(LOG_NET_CDAT, p, "tx cdat - move:%d\n", mPlayer.loc[p].comp_move);
//   mLog.add_log_net_db_row(LOG_NET_CDAT, 0, 0, "tx cdat - move:%d", mPlayer.loc[p].comp_move);

//   mLog.add(LOG_NET_CDAT, 0, mPlayer.loc[p].comp_move);

   mLog.add(LOG_NET_CDAT, 0, p, mPlayer.loc[p].comp_move);


}












void mwNetgame::client_send_stak_packet(int ack_frame)
{
   int p = mPlayer.active_local_player;

   char data[PACKET_BUFFER_SIZE] = {0}; int pos;
   mPacketBuffer.PacketName(data, pos, "stak");
   mPacketBuffer.PacketPutInt32(data, pos, server_lev_seq_num);
   mPacketBuffer.PacketPutInt32(data, pos, p);
   mPacketBuffer.PacketPutInt32(data, pos, ack_frame);
   mPacketBuffer.PacketPutInt32(data, pos, mLoop.frame_num);
   mPacketBuffer.PacketPutDouble(data, pos, mPlayer.loc[p].client_chase_fps);
   mPacketBuffer.PacketPutDouble(data, pos, mPlayer.loc[p].pdsync_avg);
   mPacketBuffer.PacketPutInt32(data, pos, mPlayer.loc[p].rewind);
   mPacketBuffer.PacketPutDouble(data, pos, mPlayer.loc[p].client_loc_plr_cor);
   mPacketBuffer.PacketPutDouble(data, pos, mPlayer.loc[p].client_rmt_plr_cor);
   mPacketBuffer.PacketPutDouble(data, pos, mPlayer.loc[p].cpu);
   clientSend(data, pos);

   mLog.add(LOG_NET_DIF_ACK, 0, p, ack_frame, mLoop.frame_num);
}


void mwNetgame::client_timer_adjust()
{
   int p = mPlayer.active_local_player;

   float max_dsync = mPacketBuffer.get_max_dsync();    // iterates all stdf packets in buffer and gets max dysnc
   if (max_dsync > -1000)
   {
      mPlayer.loc[p].pdsync = max_dsync;
      mRollingAverage[2].add_data(mPlayer.loc[p].pdsync);    // send to rolling average
      mPlayer.loc[p].pdsync_avg = mRollingAverage[2].avg;    // get average

      // automatically adjust client_chase_offset based on ping time
      if (client_chase_offset_mode == 1) client_chase_offset = - mPlayer.loc[p].ping_avg + client_chase_offset_auto_offset;

      // overridden by server
      if (mPlayer.syn[0].server_force_client_offset) client_chase_offset = mPlayer.syn[0].client_chase_offset;

      // set point
      float sp = client_chase_offset;

      // error = set point - measured value
      float err = sp - mPlayer.loc[p].pdsync_avg;

      // instantaneous error adjustment (proportional)
      float p_adj = err * 80;

      // total adjust
      float t_adj = p_adj;


      // calc speed adjustment
      float fps_chase = mLoop.frame_speed - t_adj;

      // enforce limits
      if (fps_chase < 10) fps_chase = 10;
      if (fps_chase > 70) fps_chase = 70;

      // save value in player struct
      mPlayer.loc[p].client_chase_fps = fps_chase;

      // adjust the timer
      al_set_timer_speed(mEventQueue.fps_timer, (1 / fps_chase));


      mLog.add_tmrf(LOG_TMR_client_timer_adj, "dsc:[%5.2f] dsa:[%5.2f] sp:[%5.2f] er:[%6.2f] ta:[%6.2f]\n", mPlayer.loc[p].pdsync*1000, mPlayer.loc[p].pdsync_avg*1000, sp*1000, err*1000, t_adj);

      mLog.add(LOG_NET_TIMER_ADJUST, 0, p, mPlayer.loc[p].pdsync, mPlayer.loc[p].pdsync_avg, client_chase_offset, mPlayer.loc[p].client_chase_fps );
   }
   else mLog.add(LOG_NET_TIMER_ADJUST, 1, p, mPlayer.loc[p].pdsync, mPlayer.loc[p].pdsync_avg, client_chase_offset, mPlayer.loc[p].client_chase_fps ); // no stdf packets this frame
}

void mwNetgame::client_proc_player_drop()
{
   int p = mPlayer.active_local_player;
   if (mPlayer.syn[p].control_method == PM_PLAYER_CONTROL_METHOD_CLIENT_ORPHAN)
   {
      mPlayer.loc[p].quit_reason = PM_PLAYER_QUIT_REASON_SERVER_ENDED_GAME;

      //mLog.log_ending_stats_client(LOG_NET_ENDING_STATS, p);
      mLog.add(LOG_NET_ENDING_STATS, 1, p);



      mScreen.rtextout_centre(mFont.bltn, NULL, mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2, 10, -2, 1, "SERVER ENDED GAME!");
      al_flip_display();
      mInput.tsw();
      mLoop.state[0] = PM_PROGRAM_STATE_CLIENT_EXIT;
   }

   int lda = mPlayer.loc[p].client_last_dif_applied_frame_num;
   if ((mLoop.frame_num > 0) && (lda > 0)) // check to see if server connection is lost
   {
      int ss = mLoop.frame_num - lda;
      if (ss > 200)
      {
         mPlayer.loc[p].quit_reason = PM_PLAYER_QUIT_REASON_CLIENT_LOST_SERVER_CONNECTION;
         mLog.add(LOG_NET, LOG_NET_SUBTYPE_CLIENT_DROP, p, lda);
         //mLog.log_ending_stats_client(LOG_NET_ENDING_STATS, p);
         mLog.add(LOG_NET_ENDING_STATS, 1, p);
         mScreen.rtextout_centre(mFont.bltn, NULL, mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2, 10, -2, 1, "LOST SERVER CONNECTION!");
         al_flip_display();
         mInput.tsw();
         mLoop.state[0] = PM_PROGRAM_STATE_CLIENT_EXIT;
      }
   }
}

void mwNetgame::client_control()
{
   client_timer_adjust();
   mPacketBuffer.proc_rx_buffer();
   client_apply_dif();
   client_proc_player_drop();
   process_bandwidth_counters(mPlayer.active_local_player);
}

