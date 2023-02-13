// mwGameMovesArray.cpp

#include "pm.h"
#include "mwGameMovesArray.h"
#include "z_player.h"
#include "n_netgame.h"
#include "z_log.h"
#include "mwProgramState.h"
#include "z_menu.h"
#include "z_level.h"
#include "z_control.h"
#include "z_fnx.h"
#include "z_screen_overlay.h"



mwGameMovesArray mwGMA;


mwGameMovesArray::mwGameMovesArray()
{
   initialize();
}

void mwGameMovesArray::initialize(void)
{
   for (int x=0; x<GAME_MOVES_SIZE; x++)
      for (int y=0; y<4; y++)
         game_moves[x][y] = 0;
   game_move_entry_pos = 0;
   game_move_current_pos = 0;
}


int mwGameMovesArray::has_player_acknowledged(int p)
{
   int start_pos = game_move_entry_pos;
   int end_pos = start_pos - 1000;
   if (end_pos < 0) end_pos = 0;
   for (int x=start_pos; x>end_pos; x--) // look back for ack
      if ((game_moves[x][1] == 8) && (game_moves[x][2] == p)) return 1;
   return 0;
}






// this function processes all entries in the game_moves array that match current mwPS.frame_num
void mwGameMovesArray::proc_game_moves_array(void)
{
   // search entire range
   int start_index = game_move_entry_pos-1;
   int end_index = 0;

   // reduce search range
   start_index = game_move_current_pos + 100;
   if (start_index > game_move_entry_pos-1) start_index = game_move_entry_pos-1;

   end_index = game_move_current_pos - 100;
   if (end_index < 0) end_index = 0;

   for (int x=start_index; x>=end_index; x--)  // search backwards from start_index to end_index
   {
      if (game_moves[x][0] == mwPS.frame_num) // found frame number that matches current frame
      {
         if (x > game_move_current_pos) game_move_current_pos = x; // index of last used gm - keep this at the most recent one, never go back
         switch (game_moves[x][1])
         {
            case 1: proc_player_active_game_move(x); break;
            case 2: proc_player_inactive_game_move(x); break;
            case 3: proc_player_client_join_game_move(x); break;
            case 4: proc_player_client_quit_game_move(x); break;
            case 5: set_controls_from_comp_move(game_moves[x][2], game_moves[x][3]); break;
         }
      }
   }
}




void mwGameMovesArray::add_game_move2(int frame, int type, int data1, int data2)
{
   game_moves[game_move_entry_pos][0] = frame;
   game_moves[game_move_entry_pos][1] = type;
   game_moves[game_move_entry_pos][2] = data1;
   game_moves[game_move_entry_pos][3] = data2;
   game_move_entry_pos++;
}

void mwGameMovesArray::add_game_move(int frame, int type, int data1, int data2)
{
   // ----------------------------------------------------------------------------------------
   // if we are in level_done_mode 5, all moves are converted to type 8, level done acknowledge
   // ----------------------------------------------------------------------------------------
   if ((players[0].level_done_mode == 5) && (type == 5) && (data2)) // all move type inputs are acks in this mode
   {
      if (!has_player_acknowledged(data1)) // to prevent multiple acks
         add_game_move2(frame, 8, data1, 0); // type 8 - acknowledge level done
      return; // to exit immediately
   }

   // ----------------------------------------------------------------------------------------
   // if menu key or escape was pressed, handle the quitting of the game for other players too
   // ----------------------------------------------------------------------------------------
   if ((type == 5) && (data2 & PM_COMPMOVE_MENU))  // menu key
   {
      // ----------------------------------------------------------------------------------------
      // single player mode quit
      // ----------------------------------------------------------------------------------------
      if ((active_local_player == 0) && (players[0].control_method == 0))
      {
         // do not enter game move, just exit to menu
         mwPS.new_program_state = 1;
         resume_allowed = 1;
         return;
      }
      // ----------------------------------------------------------------------------------------
      // local server quitting
      // ----------------------------------------------------------------------------------------
      if ((ima_server) && (data1 == 0))
      {
         // first set all connected clients to inactive
         for (int p=1; p<NUM_PLAYERS; p++)
            if (players[p].control_method == 2)
            {
               add_game_move2(frame + 2, 4, p, 0);   // type 4 - client quit
               add_game_move2(frame + 10, 2, p, 64); // type 2 - player inactive
            }
         // then set server to inactive in future
         add_game_move2(frame + 10, 2, 0, 64); // type 2 - player inactive
         return;
      }
      // ----------------------------------------------------------------------------------------
      // remote client quitting (server only)
      // ----------------------------------------------------------------------------------------
      if ((ima_server) && (data1 > 0))
      {
         add_game_move2(frame + 2, 2, data1, 64); // type 2 - player inactive
         return;
      }
      // ----------------------------------------------------------------------------------------
      // local client quitting (client only)
      // ----------------------------------------------------------------------------------------
      if (ima_client)
      {
         add_game_move2(frame + 4, 2, data1, 64); // type 2 - player inactive
         return;
      }

      sprintf(msg, "Error: menu key not handled...should never get here");
      m_err(msg);

      // ----------------------------------------------------------------------------------------
      // all other types of quitting
      // ----------------------------------------------------------------------------------------
//      add_game_move2(frame + 4, 2, data1, 64); // type 2 - player inactive
//      return;
   }

   // ----------------------------------------------------------------------------------------
   // the normal method of simply entering the game move
   // ----------------------------------------------------------------------------------------
   add_game_move2(frame, type, data1, data2);
}



void mwGameMovesArray::proc_player_client_join_game_move(int x)
{
   if (ima_server)
   {
      int p = game_moves[x][2];  // player number
      int c = game_moves[x][3];  // color
      players[p].control_method = 2;
      players[p].color = c;
   }
}

void mwGameMovesArray::proc_player_client_quit_game_move(int x)
{
   if (ima_server)
   {
      int p = game_moves[x][2];  // player number
      players[p].control_method = 8;
   }
}

void mwGameMovesArray::proc_player_active_game_move(int x)
{
   int p            = game_moves[x][2]; // player number
   players[p].color = game_moves[x][3]; // color

   // player was inactive before and just now changes to active
   if (players[p].active == 0)
   {
      set_player_start_pos(p, 0); // set starting position
      players[p].active = 1;
      players1[p].join_frame = mwPS.frame_num;

      if ((ima_server) || (ima_client))
         if (p != active_local_player) players[p].control_method = 2;

      // if player 0 is file play all added players will be too
      if (players[0].control_method == 1) players[p].control_method = 1;

      set_player_join_quit_display(p, 1, 60);

      game_event(80, 0, 0, p, 0, 0, 0);

      if (LOG_NET)
      {
         sprintf(msg,"PLAYER:%d became ACTIVE color:%d", p, players[p].color);
         add_log_entry_header(10, p, msg, 1);
      }
   }
}

void mwGameMovesArray::proc_player_inactive_game_move(int x)
{
   int p   = game_moves[x][2]; // player number
   int val = game_moves[x][3]; // reason

   players1[p].quit_frame = mwPS.frame_num;

   // ------------------------------------
   // player never became active
   // ------------------------------------
   if ((players[p].active == 0) && (players[p].control_method == 2))
   {
      players1[p].join_frame = mwPS.frame_num;
      players1[p].quit_reason = 74;
//      players[p].control_method = 9; // prevent re-use of this player number in this level
      players[p].control_method = 0;
      players1[p].who = 99;
   }

   if (players[p].active)
   {
      if (LOG_NET)
      {
         sprintf(msg,"PLAYER:%d became INACTIVE", p);
         add_log_entry_header(10, p, msg, 1);
      }

      // ------------------------------------
      // player in run demo mode became inactive
      // ------------------------------------
      if (players[p].control_method == 1)
      {
         players[p].active = 0;
         // only quit if no players left active
         int still_active = 0;
         for (int p=0; p<NUM_PLAYERS; p++)
            if (players[p].active) still_active = 1;
         if (!still_active) mwPS.new_program_state = 1;
      }

      // ------------------------------------
      // local server player quit
      // ------------------------------------
      if (players[p].control_method == 3)
      {
         // printf("Local Server Player Quit :%d\n", mwPS.frame_num);
         players1[p].quit_reason = 91;

         // set quit reason for all active clients on server
         for (int pp=1; pp<NUM_PLAYERS; pp++)
            if ((players[pp].active) && (players[pp].control_method == 2))
               players1[pp].quit_reason = 91;
         if (LOG_NET) log_ending_stats_server();
         mwPS.new_program_state = 1;
      }

      // ------------------------------------
      // remote server quit
      // ------------------------------------
      if ((ima_client) && (p == 0))
      {
         // printf("Remote Server Quit :%d\n", mwPS.frame_num);
         if (val == 64) players1[active_local_player].quit_reason = 92;
         mwPS.new_program_state = 1;
      }

      // ------------------------------------
      // remote player quit (server only)
      // ------------------------------------
      if (players[p].control_method == 2)
      {
         // printf("Remote Player Quit :%d\n", mwPS.frame_num);
         players1[p].quit_reason = 93;
         if (LOG_NET) log_ending_stats(p);
         init_player(p, 1);

         reset_client_state(p);

//         players[p].active = 0;
//         players[p].control_method = 9; // prevent re-use of this player number in this level
//         players1[p].who = 99;
      }
      set_player_join_quit_display(p, 0, 60);
      game_event(81, 0, 0, p, 0, 0, 0);
   }
}

