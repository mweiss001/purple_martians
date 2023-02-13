// z_control.cpp
#include "pm.h"
#include "z_control.h"
#include "z_log.h"
#include "z_settings.h"
#include "z_player.h"
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
#include "z_menu.h"
#include "z_config.h"


void clear_controls(int p)
{
   players[p].left  = 0;
   players[p].right = 0;
   players[p].up    = 0;
   players[p].down  = 0;
   players[p].jump  = 0;
   players[p].fire  = 0;
   players[p].menu  = 0;
}

void set_controls_from_comp_move(int p, int comp_move)
{
   clear_controls(p);
   if (comp_move & PM_COMPMOVE_LEFT)  players[p].left  = 1;
   if (comp_move & PM_COMPMOVE_RIGHT) players[p].right = 1;
   if (comp_move & PM_COMPMOVE_UP)    players[p].up    = 1;
   if (comp_move & PM_COMPMOVE_DOWN)  players[p].down  = 1;
   if (comp_move & PM_COMPMOVE_JUMP)  players[p].jump  = 1;
   if (comp_move & PM_COMPMOVE_FIRE)  players[p].fire  = 1;
   if (comp_move & PM_COMPMOVE_MENU)  players[p].menu  = 1;
}

void set_comp_move_from_player_key_check(int p) // doesn't set controls
{
   int cm = 0;
   if (mI.key[players1[p].left_key][0])  cm |= PM_COMPMOVE_LEFT;
   if (mI.key[players1[p].right_key][0]) cm |= PM_COMPMOVE_RIGHT;
   if (mI.key[players1[p].up_key][0])    cm |= PM_COMPMOVE_UP;
   if (mI.key[players1[p].down_key][0])  cm |= PM_COMPMOVE_DOWN;
   if (mI.key[players1[p].jump_key][0])  cm |= PM_COMPMOVE_JUMP;
   if (mI.key[players1[p].fire_key][0])  cm |= PM_COMPMOVE_FIRE;
   if (mI.key[players1[p].menu_key][0])  cm |= PM_COMPMOVE_MENU;
   if (mI.key[ALLEGRO_KEY_ESCAPE][0])    cm |= PM_COMPMOVE_MENU;
   players1[p].comp_move = cm;
}

void set_controls_from_player_key_check(int p) // used only in menu
{
   if (mI.key[players1[p].left_key][0])  players[p].left  = 1;
   if (mI.key[players1[p].right_key][0]) players[p].right = 1;
   if (mI.key[players1[p].up_key][0])    players[p].up    = 1;
   if (mI.key[players1[p].down_key][0])  players[p].down  = 1;
   if (mI.key[players1[p].jump_key][0])  players[p].jump  = 1;
   if (mI.key[players1[p].fire_key][0])  players[p].fire  = 1;
   if (mI.key[players1[p].menu_key][0])  players[p].menu  = 1;
   if (mI.key[ALLEGRO_KEY_ESCAPE][0])    players[p].menu  = 1;
}

void rungame_key_check(int p)
{
   if (mI.key[ALLEGRO_KEY_0][0]) active_local_player = 0;
   if (mI.key[ALLEGRO_KEY_1][0]) active_local_player = 1;
   if (mI.key[ALLEGRO_KEY_2][0]) active_local_player = 2;
   if (mI.key[ALLEGRO_KEY_3][0]) active_local_player = 3;
   if (mI.key[ALLEGRO_KEY_4][0]) active_local_player = 4;
   if (mI.key[ALLEGRO_KEY_5][0]) active_local_player = 5;
   if (mI.key[ALLEGRO_KEY_6][0]) active_local_player = 6;
   if (mI.key[ALLEGRO_KEY_7][0]) active_local_player = 7;

   // dont let alp be an inactive player
   while (!players[active_local_player].active) // if alp not active
      if (++active_local_player > 7) active_local_player = 0;

   // if games_moves doesn't end with level_done kill it after 4 seconds
   if (mwPS.frame_num > mwDM.demo_mode_last_frame + 160) mwPS.new_program_state = 1;

   if ((mI.key[ALLEGRO_KEY_ESCAPE][0]) || (mI.key[ALLEGRO_KEY_ENTER][0]) || (mI.key[ALLEGRO_KEY_SPACE][0]))
   {
      mwDM.demo_mode_on = 0;

      // set all players inactive
      for (int p=0; p<NUM_PLAYERS; p++) players[p].active = 0;

      // except for local player
      players[0].active = 1;
      load_config();

      while (mI.key[ALLEGRO_KEY_ESCAPE][0]) mwEQ.proc_event_queue();

      mwPS.new_program_state = 1;
      if (mwPS.old_program_state == 2) mwPS.old_program_state = mwPS.older_program_state; // don't send back to demo mode

   }
}


void proc_player_input(void)
{
   for (int p=0; p<NUM_PLAYERS; p++)
      if (players[p].active) // cycle all active players
      {
         int cm = players[p].control_method;
         if (cm == 1) rungame_key_check(p); // run game from file
         if ((cm == 0) || (cm == 3) || (cm == 4)) // single player, server, client
         {
            if ((players[0].level_done_mode == 0) || (players[0].level_done_mode == 5)) // only allow player input in these modes
            {
               if (players1[p].fake_keypress_mode) players1[p].comp_move = rand() % 64;
               else set_comp_move_from_player_key_check(p);
               if (players1[p].old_comp_move != players1[p].comp_move)  // player's controls have changed
               {
                  players1[p].old_comp_move = players1[p].comp_move;

                  // in single player, client and server mode, add to game moves array
                  if ((cm == 0) || (cm == 3) || (cm == 4)) mwGMA.add_game_move(mwPS.frame_num, 5, p, players1[p].comp_move);

                  // in client mode, send cdat packet, and apply move directly to controls
                  if (cm == 4)
                  {
                     Packet("cdat");
                     PacketPut1ByteInt(p);
                     PacketPut4ByteInt(mwPS.frame_num);
                     PacketPut1ByteInt(players1[p].comp_move);
                     ClientSend(packetbuffer, packetsize);

                     set_controls_from_comp_move(p, players1[p].comp_move);

                     if (players[p].menu) mwPS.new_program_state = 25;
                     players1[p].client_cdat_packets_tx++;
                     sprintf(msg,"tx cdat - move:%d\n", players1[p].comp_move);
                     if (LOG_NET_cdat) add_log_entry2(35, p, msg);

                  }
               }
            }
         }
      }
      else if (players[p].control_method == 4) // not active and control method 4 is a client waiting for server to make it active
      {
         if (mI.key[ALLEGRO_KEY_ESCAPE][1]) mwPS.new_program_state = 25; // give them an escape option
      }
}
