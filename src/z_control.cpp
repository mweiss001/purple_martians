// z_control.cpp
#include "pm.h"
#include "z_control.h"
#include "z_log.h"
#include "mwSettings.h"
#include "mwPlayers.h"
#include "n_netgame.h"
#include "mwDemoMode.h"
#include "mwDisplay.h"
#include "mwFont.h"
#include "n_packet.h"
#include "n_client.h"
#include "mwGameMovesArray.h"
#include "mwColor.h"
#include "mwInput.h"
#include "mwEventQueue.h"
#include "mwProgramState.h"
#include "mwConfig.h"


void clear_controls(int p)
{
   mPlayer.syn[p].left  = 0;
   mPlayer.syn[p].right = 0;
   mPlayer.syn[p].up    = 0;
   mPlayer.syn[p].down  = 0;
   mPlayer.syn[p].jump  = 0;
   mPlayer.syn[p].fire  = 0;
   mPlayer.syn[p].menu  = 0;
}

void set_controls_from_comp_move(int p, int comp_move)
{
   clear_controls(p);
   if (comp_move & PM_COMPMOVE_LEFT)  mPlayer.syn[p].left  = 1;
   if (comp_move & PM_COMPMOVE_RIGHT) mPlayer.syn[p].right = 1;
   if (comp_move & PM_COMPMOVE_UP)    mPlayer.syn[p].up    = 1;
   if (comp_move & PM_COMPMOVE_DOWN)  mPlayer.syn[p].down  = 1;
   if (comp_move & PM_COMPMOVE_JUMP)  mPlayer.syn[p].jump  = 1;
   if (comp_move & PM_COMPMOVE_FIRE)  mPlayer.syn[p].fire  = 1;
   if (comp_move & PM_COMPMOVE_MENU)  mPlayer.syn[p].menu  = 1;
}

void set_comp_move_from_player_key_check(int p) // doesn't set controls
{
   int cm = 0;
   if (mI.key[mPlayer.loc[p].left_key][0])  cm |= PM_COMPMOVE_LEFT;
   if (mI.key[mPlayer.loc[p].right_key][0]) cm |= PM_COMPMOVE_RIGHT;
   if (mI.key[mPlayer.loc[p].up_key][0])    cm |= PM_COMPMOVE_UP;
   if (mI.key[mPlayer.loc[p].down_key][0])  cm |= PM_COMPMOVE_DOWN;
   if (mI.key[mPlayer.loc[p].jump_key][0])  cm |= PM_COMPMOVE_JUMP;
   if (mI.key[mPlayer.loc[p].fire_key][0])  cm |= PM_COMPMOVE_FIRE;
   if (mI.key[mPlayer.loc[p].menu_key][0])  cm |= PM_COMPMOVE_MENU;
   if (mI.key[ALLEGRO_KEY_ESCAPE][0])    cm |= PM_COMPMOVE_MENU;
   mPlayer.loc[p].comp_move = cm;
}

void set_controls_from_player_key_check(int p) // used only in menu
{
   if (mI.key[mPlayer.loc[p].left_key][0])  mPlayer.syn[p].left  = 1;
   if (mI.key[mPlayer.loc[p].right_key][0]) mPlayer.syn[p].right = 1;
   if (mI.key[mPlayer.loc[p].up_key][0])    mPlayer.syn[p].up    = 1;
   if (mI.key[mPlayer.loc[p].down_key][0])  mPlayer.syn[p].down  = 1;
   if (mI.key[mPlayer.loc[p].jump_key][0])  mPlayer.syn[p].jump  = 1;
   if (mI.key[mPlayer.loc[p].fire_key][0])  mPlayer.syn[p].fire  = 1;
   if (mI.key[mPlayer.loc[p].menu_key][0])  mPlayer.syn[p].menu  = 1;
   if (mI.key[ALLEGRO_KEY_ESCAPE][0])    mPlayer.syn[p].menu  = 1;
}

void rungame_key_check(int p)
{
   if (mI.key[ALLEGRO_KEY_0][0]) mPlayer.active_local_player = 0;
   if (mI.key[ALLEGRO_KEY_1][0]) mPlayer.active_local_player = 1;
   if (mI.key[ALLEGRO_KEY_2][0]) mPlayer.active_local_player = 2;
   if (mI.key[ALLEGRO_KEY_3][0]) mPlayer.active_local_player = 3;
   if (mI.key[ALLEGRO_KEY_4][0]) mPlayer.active_local_player = 4;
   if (mI.key[ALLEGRO_KEY_5][0]) mPlayer.active_local_player = 5;
   if (mI.key[ALLEGRO_KEY_6][0]) mPlayer.active_local_player = 6;
   if (mI.key[ALLEGRO_KEY_7][0]) mPlayer.active_local_player = 7;

   // dont let alp be an inactive player
   while (!mPlayer.syn[mPlayer.active_local_player].active) // if alp not active
      if (++mPlayer.active_local_player > 7) mPlayer.active_local_player = 0;

   // if games_moves doesn't end with level_done kill it after 4 seconds
   if (mwPS.frame_num > mwDM.demo_mode_last_frame + 160) mwPS.new_program_state = 1;

   if ((mI.key[ALLEGRO_KEY_ESCAPE][0]) || (mI.key[ALLEGRO_KEY_ENTER][0]) || (mI.key[ALLEGRO_KEY_SPACE][0]))
   {
      mwDM.demo_mode_on = 0;

      // set all players inactive
      for (int p=0; p<NUM_PLAYERS; p++) mPlayer.syn[p].active = 0;

      // except for local player
      mPlayer.syn[0].active = 1;
      mConfig.load();

      while (mI.key[ALLEGRO_KEY_ESCAPE][0]) mwEQ.proc_event_queue();

      mwPS.new_program_state = 1;
      if (mwPS.old_program_state == 2) mwPS.old_program_state = mwPS.older_program_state; // don't send back to demo mode

   }
}


void proc_player_input(void)
{
   char msg[1024];
   for (int p=0; p<NUM_PLAYERS; p++)
      if (mPlayer.syn[p].active) // cycle all active players
      {
         int cm = mPlayer.syn[p].control_method;
         if (cm == 1) rungame_key_check(p); // run game from file
         if ((cm == 0) || (cm == 3) || (cm == 4)) // single player, server, client
         {
            if ((mPlayer.syn[0].level_done_mode == 0) || (mPlayer.syn[0].level_done_mode == 5)) // only allow player input in these modes
            {
               if (mPlayer.loc[p].fake_keypress_mode) mPlayer.loc[p].comp_move = rand() % 64;
               else set_comp_move_from_player_key_check(p);
               if (mPlayer.loc[p].old_comp_move != mPlayer.loc[p].comp_move)  // player's controls have changed
               {
                  mPlayer.loc[p].old_comp_move = mPlayer.loc[p].comp_move;

                  // in single player, client and server mode, add to game moves array
                  if ((cm == 0) || (cm == 3) || (cm == 4)) mwGMA.add_game_move(mwPS.frame_num, 5, p, mPlayer.loc[p].comp_move);

                  // in client mode, send cdat packet, and apply move directly to controls
                  if (cm == 4)
                  {
                     Packet("cdat");
                     PacketPut1ByteInt(p);
                     PacketPut4ByteInt(mwPS.frame_num);
                     PacketPut1ByteInt(mPlayer.loc[p].comp_move);
                     ClientSend(packetbuffer, packetsize);

                     set_controls_from_comp_move(p, mPlayer.loc[p].comp_move);

                     if (mPlayer.syn[p].menu) mwPS.new_program_state = 25;
                     mPlayer.loc[p].client_cdat_packets_tx++;
                     sprintf(msg,"tx cdat - move:%d\n", mPlayer.loc[p].comp_move);
                     if (LOG_NET_cdat) add_log_entry2(35, p, msg);

                  }
               }
            }
         }
      }
      else if (mPlayer.syn[p].control_method == 4) // not active and control method 4 is a client waiting for server to make it active
      {
         if (mI.key[ALLEGRO_KEY_ESCAPE][1]) mwPS.new_program_state = 25; // give them an escape option
      }
}
