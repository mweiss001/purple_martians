// mwGameMoves.cpp

#include "pm.h"
#include "mwGameMoves.h"
#include "mwPlayer.h"
#include "mwNetgame.h"
#include "mwLog.h"
#include "mwLoop.h"
#include "mwLevel.h"
#include "mwGameEvent.h"
#include "mwInput.h"
#include "mwShot.h"
#include "mwDisplay.h"
#include "mwDemoMode.h"
#include "mwDemoRecord.h"
#include "mwScreen.h"
#include "mwMain.h"
#include "mwItem.h"
#include "mwMiscFnx.h"

#include "mwSql.h"


mwGameMoves mGameMoves;

mwGameMoves::mwGameMoves() { initialize(); }



void mwGameMoves::initialize(void)
{
   memset(arr, 0, sizeof arr);
   //for (int x=0; x<GAME_MOVES_SIZE; x++) clear_single(x);
   entry_pos = 0;
   current_pos = 0;

   HEADER_version = 0;
   HEADER_muid.clear();
   HEADER_create_timestamp.clear();
   HEADER_modify_timestamp.clear();
   HEADER_level = 0;
   HEADER_last_frame = 0;
   HEADER_num_entries = 0;

   sprintf(last_loaded_gm_filename, "%s", "");
   status = 0;
}






void mwGameMoves::new_level()
{
   initialize();
   HEADER_create_timestamp = mMiscFnx.get_timestamp();
   HEADER_muid = mMiscFnx.generate_muid();
   HEADER_level = mLevel.play_level;
   status = 1;

   create_gm_session_links();

}


void mwGameMoves::clear_single(int i)
{
   for (int y=0; y<4; y++) arr[i][y] = 0;
}

// removes entry i and slides all the rest down
void mwGameMoves::gm_remove(int i)
{
   if (entry_pos > 1)
   {
      for (int x=i; x<entry_pos-1; x++)
         for (int k=0; k<4; k++) arr[x][k] = arr[x+1][k];
      entry_pos--;
   }
}

void mwGameMoves::gm_swap(int i, int j)
{
   for (int k=0; k<4; k++)
   {
      int tmp = arr[i][k];
      arr[i][k] = arr[j][k];
      arr[j][k] = tmp;
   }
}

void mwGameMoves::gm_sort(void)
{
   // first sort by frame
   int swap_flag = 1;
   while (swap_flag)
   {
      swap_flag = 0;
      for (int x=0; x<entry_pos-1; x++)
      {
         if (arr[x][0] > arr[x+1][0])
         {
            gm_swap(x, x+1);
            swap_flag++; // if any swaps
         }
      }
   }

   // then sort by player num
   swap_flag = 1;
   while (swap_flag)
   {
      swap_flag = 0;
      for (int x=0; x<entry_pos-1; x++)
      {
         if (arr[x][0] == arr[x+1][0]) // same frame num
            if (arr[x][2] > arr[x+1][2]) // sort by player num
            {
               gm_swap(x, x+1);
               swap_flag++; // if any swaps
            }
      }
   }

   // then put shot config first of all in the same frame
   swap_flag = 1;
   while (swap_flag)
   {
      swap_flag = 0;
      for (int x=0; x<entry_pos-1; x++)
      {
         if (arr[x][0] == arr[x+1][0]) // same frame num
            if (arr[x][1] != PM_GAMEMOVE_TYPE_SHOT_CONFIG &&  arr[x+1][1] == PM_GAMEMOVE_TYPE_SHOT_CONFIG)
            {
               gm_swap(x, x+1);
               swap_flag++; // if any swaps
            }
      }
   }

   // now remove all zero entries from the start
   for (int x=0; x<entry_pos; x++)
      if ((entry_pos > 1) && (arr[x][0] == 0) && (arr[x][1] == 0) && (arr[x][2] == 0) && (arr[x][3] == 0) )
      {
         gm_remove(x);
         x--;
      }
}

void mwGameMoves::remove_doubled_moves(void)
{
   gm_sort();
   for (int p=0; p<NUM_PLAYERS; p++)
   {
      int cm = 0; // initial move
      for (int x=0; x<entry_pos; x++)
      {
         if ((arr[x][2] == p) && (arr[x][1] == PM_GAMEMOVE_TYPE_PLAYER_MOVE))
         {
            if (arr[x][3] != cm) cm = arr[x][3]; // different cm, update
            else clear_single(x);  // same cm, clear
         }
      }
   }
   gm_sort();
}



int mwGameMoves::has_player_acknowledged(int p)
{
   if (mPlayer.syn[p].paused_type == 3) return 1; // headless server player
   int start_pos = entry_pos;
   int end_pos = start_pos - 1000;
   if (end_pos < 0) end_pos = 0;
   for (int x=start_pos; x>end_pos; x--) // look back for ack
      if ((arr[x][1] == PM_GAMEMOVE_TYPE_LEVEL_DONE_ACK) && (arr[x][2] == p)) return 1;
   return 0;
}


// processes all entries in the game_moves array that match current frame_num
void mwGameMoves::proc(void)
{
   // search entire range
   int start_index = entry_pos-1;
   int end_index = 0;

   if (!(mLoop.state[1] == PM_PROGRAM_STATE_DEMO_RECORD))
   {
      // reduce search range
      start_index = current_pos + 100;
      if (start_index > entry_pos-1) start_index = entry_pos-1;

      end_index = current_pos - 100;
      if (end_index < 0) end_index = 0;
   }

   for (int x=start_index; x>=end_index; x--)  // search backwards from start_index to end_index
   {
      if (arr[x][0] == mLoop.frame_num) // found frame number that matches current frame
      {
         int t = arr[x][1];

         if (!(mLoop.state[1] == PM_PROGRAM_STATE_DEMO_RECORD))
            if (x > current_pos) current_pos = x; // index of last used gm - keep this at the most recent one, never go back

         if (t & PM_GAMEMOVE_TYPE_PLAYER_ACTIVE_FLAG) // new version with embedded name
         {
            // decode and call original function
            int p, c;
            char name[9] = { 0 };
            mMiscFnx.gma_to_val(arr[x][1], arr[x][2], arr[x][3], p, c, name);
            snprintf(mPlayer.syn[p].name, 9, "%s", name);

            //printf("proc gm p:%d c:%d name:%s nt:%d d1:%d d2:%d \n", p, c, name, arr[x][1], arr[x][2], arr[x][3]);
            proc_game_move_player_active(p, c);
         }

         if (t == PM_GAMEMOVE_TYPE_PLAYER_ACTIVE)   {  proc_game_move_player_active(        arr[x][2], arr[x][3] ); }
         if (t == PM_GAMEMOVE_TYPE_PLAYER_INACTIVE) {  proc_game_move_player_inactive(      arr[x][2], arr[x][3] ); }
         if (t == PM_GAMEMOVE_TYPE_PLAYER_HIDDEN)   {  proc_game_move_player_hidden(        arr[x][2]            ); }
         if (t == PM_GAMEMOVE_TYPE_SHOT_CONFIG)     {  proc_game_move_shot_config(          arr[x][2], arr[x][3] ); }
         if (t == PM_GAMEMOVE_TYPE_PLAYER_MOVE)     {  mPlayer.set_controls_from_comp_move( arr[x][2], arr[x][3] ); }
      }
   }

   // at this point all the players' controls are set for the frame - save them in bitf
   for (int p=0; p<NUM_PLAYERS; p++)
      if (mPlayer.syn[p].active)
      {
         mPlayer.syn[p].old_ctrl_bitf = mPlayer.syn[p].ctrl_bitf; // move current to old
         mPlayer.syn[p].ctrl_bitf = mPlayer.comp_move_from_players_current_controls(p); // get new
      }
}

void mwGameMoves::add_game_move2(int frame, int type, int data1, int data2)
{
   arr[entry_pos][0] = frame;
   arr[entry_pos][1] = type;
   arr[entry_pos][2] = data1;
   arr[entry_pos][3] = data2;
   entry_pos++;
}






void mwGameMoves::add_game_move(int frame, int type, int data1, int data2)
{
   char msg[1024];
   int p = data1; // alias to improve readability
   int c = data2;

   if (type == PM_GAMEMOVE_TYPE_PLAYER_ACTIVE)
   {
      // convert to new player active type with embedded name
      int t, d1, d2;
      mMiscFnx.val_to_gma(t, d1, d2, p, c, mPlayer.syn[p].name);
      add_game_move2(frame, t, d1, d2);
      //printf("add gm p:%d c:%d name:%s nt:%d d1:%d d2:%d \n", p, c, mPlayer.syn[p].name, t, d1, d2);
      return;
   }

   if (type == PM_GAMEMOVE_TYPE_SHOT_CONFIG)
   {
      int sc = 0;
      if (mPlayer.syn[0].player_vs_player_shots) sc |= 0b01;
      if (mPlayer.syn[0].player_vs_self_shots)   sc |= 0b10;

      add_game_move2(frame, type, sc, mPlayer.syn[0].player_vs_player_shot_damage);
      return; // to exit immediately
   }

   // -----------------------------------------------------------------------------------------------------------------
   // if we are in level_done_mode 5, all PM_GAMEMOVE_TYPE_PLAYER_MOVE are converted to PM_GAMEMOVE_TYPE_LEVEL_DONE_ACK
   // -----------------------------------------------------------------------------------------------------------------
   if ((mPlayer.syn[0].level_done_mode == 5) && (type == PM_GAMEMOVE_TYPE_PLAYER_MOVE) && (data2))
   {
      if (!has_player_acknowledged(p)) // to prevent multiple acks
         add_game_move2(frame, PM_GAMEMOVE_TYPE_LEVEL_DONE_ACK, p, 0);
      return; // exit immediately
   }



   // --------------------------------------------------------------------------------------
   // PM_COMPMOVE_MENU was pressed - enter PM_GAMEMOVE_TYPE_PLAYER_INACTIVE
   // --------------------------------------------------------------------------------------
   if ((type == PM_GAMEMOVE_TYPE_PLAYER_MOVE) && (data2 & PM_COMPMOVE_MENU))
   {



      // ----------------------------------------------------------------------------------------
      // single player mode quit - do not enter inactive game move (so game can be resumed)
      // ----------------------------------------------------------------------------------------
      if ((mPlayer.syn[0].control_method == PM_PLAYER_CONTROL_METHOD_SINGLE_PLAYER) && (mPlayer.active_local_player == 0))
      {
         mLoop.state[0] = PM_PROGRAM_STATE_SINGLE_PLAYER_EXIT;
         return;
      }

      // ----------------------------------------------------------------------------------------
      // demo record player quit
      // ----------------------------------------------------------------------------------------
      if ((mLoop.state[1] == PM_PROGRAM_STATE_DEMO_RECORD) && (mPlayer.syn[p].control_method == PM_PLAYER_CONTROL_METHOD_SINGLE_PLAYER))
      {
         add_game_move2(frame, PM_GAMEMOVE_TYPE_PLAYER_INACTIVE, p, PM_PLAYER_QUIT_REASON_MENU_KEY);
         return;
      }


      // ----------------------------------------------------------------------------------------
      // local server
      // ----------------------------------------------------------------------------------------
      if (mNetgame.ima_server)
      {
         if (p == 0) // server quitting
         {
            // first set all connected clients to inactive
            for (int p=1; p<NUM_PLAYERS; p++)
               if (mPlayer.syn[p].control_method == PM_PLAYER_CONTROL_METHOD_NETGAME_REMOTE)
               {
                  mPlayer.syn[p].control_method = PM_PLAYER_CONTROL_METHOD_CLIENT_ORPHAN; // this lets the clients quit gracefully instead of timing out with lost server connection
                  add_game_move2(frame + 10, PM_GAMEMOVE_TYPE_PLAYER_INACTIVE, p, PM_PLAYER_QUIT_REASON_SERVER_ENDED_GAME);
               }

            // then set server to inactive in future
            add_game_move2(frame + 10, PM_GAMEMOVE_TYPE_PLAYER_INACTIVE, 0, PM_PLAYER_QUIT_REASON_SERVER_ENDED_GAME);
            return;
         }
         else // client quitting
         {
            add_game_move2(frame + 2, PM_GAMEMOVE_TYPE_PLAYER_INACTIVE, data1, PM_PLAYER_QUIT_REASON_CLIENT_ENDED_GAME);
            mNetgame.session_close(p, 2); // reason 2 - client quit
            return;
         }
      }

      // ----------------------------------------------------------------------------------------
      // local client quitting
      // ----------------------------------------------------------------------------------------
      if (mNetgame.ima_client)
      {
         // should I do this? or should I just wait for the server to do it??
         add_game_move2(frame + 4, PM_GAMEMOVE_TYPE_PLAYER_INACTIVE, p, PM_PLAYER_QUIT_REASON_CLIENT_ENDED_GAME);
         return;
      }

      sprintf(msg, "Error: menu key not handled...should never get here");
      mInput.m_err(msg);

      // ----------------------------------------------------------------------------------------
      // all other types of quitting
      // ----------------------------------------------------------------------------------------
//      add_game_move2(frame + 4, PM_GAMEMOVE_TYPE_PLAYER_INACTIVE, data1, 64);
//      return;

   }

   // ----------------------------------------------------------------------------------------
   // the normal method of simply entering the game move
   // ----------------------------------------------------------------------------------------
   add_game_move2(frame, type, data1, data2);
}


void mwGameMoves::proc_game_move_shot_config(int shot, int damg)
{
   mPlayer.syn[0].player_vs_player_shots       = shot & 0b01;
   mPlayer.syn[0].player_vs_self_shots         = shot & 0b10;
   mPlayer.syn[0].player_vs_player_shot_damage = damg;
}

void mwGameMoves::proc_game_move_player_hidden(int p)
{
   mPlayer.syn[p].paused = 1;
   mPlayer.syn[p].paused_type = 3;
}



void mwGameMoves::proc_game_move_player_active(int p, int color)
{
   mPlayer.syn[p].color = color;

   // player was inactive before and just now changes to active
   if (mPlayer.syn[p].active == 0)
   {
      mItem.set_player_start_pos(p);
      mPlayer.syn[p].active = 1;
      mPlayer.loc[p].join_frame = mLoop.frame_num;

      if ((mNetgame.ima_server) || (mNetgame.ima_client))
         if (p != mPlayer.active_local_player) mPlayer.syn[p].control_method = PM_PLAYER_CONTROL_METHOD_NETGAME_REMOTE;

      // if player 0 is file play all added players will be too
      if (mPlayer.syn[0].control_method == PM_PLAYER_CONTROL_METHOD_DEMO_MODE) mPlayer.syn[p].control_method = PM_PLAYER_CONTROL_METHOD_DEMO_MODE;

      if (mLoop.frame_num > 1) // don't show notifications if player was made active at the start of the level
      {
         mScreen.add_player_text_overlay(p, 1);
         mGameEvent.add(6, 0, 0, p, 0, 0, 0);
         if (!mLoop.ff_state)
         {
            mLog.add_headerf(LOG_NET, -1, 0, "Player:%d became ACTIVE!                                ", p);
            mLog.add_log_net_db_row(LOG_NET, 0, p, "Player:%d became ACTIVE!", p);
         }
      }
   }
}


void mwGameMoves::proc_game_move_player_inactive(int p, int reason)
{
   mPlayer.loc[p].quit_reason = reason;
   mPlayer.loc[p].quit_frame = mLoop.frame_num;

   if (mPlayer.syn[p].active)
   {
      if (!mLoop.ff_state)
      {
         mLog.add_headerf(LOG_NET, -1, 0, "Player:%d became INACTIVE!                              ", p);
         mLog.add_log_net_db_row(LOG_NET, 0, p, "Player:%d became INACTIVE!", p);

      }


      mScreen.add_player_text_overlay(p, 0);
      mGameEvent.add(7, 0, 0, p, 0, 0, 0);

      mPlayer.syn[p].active = 0;

      if (mNetgame.ima_server)
      {
         if (p == 0) // local server player quit
         {
            // printf("Local Server Player Quit:%d\n", mLoop.frame_num);
            if (!mLoop.ff_state) mLog.log_ending_stats_server(LOG_NET_ending_stats);
            mLoop.state[0] = PM_PROGRAM_STATE_SERVER_EXIT;
         }
         else // client quit on server
         {
            // printf("Client Quit on Server:%d\n", mLoop.frame_num);
            if (!mLoop.ff_state) mLog.log_ending_stats_client(LOG_NET_ending_stats, p);
         }
      }

      if (mNetgame.ima_client)
      {
         if (p == 0) // server quit on client
         {
            // printf("Remote Server Quit:%d\n", mLoop.frame_num);
            mLoop.state[0] = PM_PROGRAM_STATE_CLIENT_EXIT;
         }
         else // a client quit
         {

            if (mPlayer.syn[p].control_method == PM_PLAYER_CONTROL_METHOD_CLIENT_LOCAL)
            {
               // printf("Local Client Quit:%d\n", mLoop.frame_num);
               mLoop.state[0] = PM_PROGRAM_STATE_CLIENT_EXIT;
            }

            if (mPlayer.syn[p].control_method == PM_PLAYER_CONTROL_METHOD_NETGAME_REMOTE) // remote client quit
            {
               // printf("Remote Client Quit:%d\n", mLoop.frame_num);

            }
         }
      }
   }
}

char* mwGameMoves::get_gm_text2(int gm, int f, int t, int p, int v, char* tmp)
{
   char dsc[80] = {0};
   if (t == PM_GAMEMOVE_TYPE_LEVEL_START)     sprintf(dsc, " START LEVEL %d", v);
   if (t == PM_GAMEMOVE_TYPE_PLAYER_ACTIVE)   sprintf(dsc, " P%d ACTIVE", p);
   if (t == PM_GAMEMOVE_TYPE_PLAYER_HIDDEN)   sprintf(dsc, " P%d HIDDEN", p);
   if (t == PM_GAMEMOVE_TYPE_PLAYER_INACTIVE) sprintf(dsc, " P%d INACTIVE", p);
   if (t == PM_GAMEMOVE_TYPE_LEVEL_DONE_ACK)  sprintf(dsc, " P%d ACK", p);
   if (t == PM_GAMEMOVE_TYPE_PLAYER_MOVE)     sprintf(dsc, " P%d MOVE", p);

   if (t == PM_GAMEMOVE_TYPE_SHOT_CONFIG)
   {
      int pvp = 0;
      int pvs = 0;
      if (p & 0b01) pvp = 1;
      if (p & 0b10) pvs = 1;
      sprintf(dsc, " SHOTS P:%d S:%d D:%d", pvp, pvs, v);
   }

   if (t == PM_GAMEMOVE_TYPE_PLAYER_MOVE)
   {
      strcat(dsc, " [");
      if (v & PM_COMPMOVE_LEFT)  strcat(dsc, "L");
      else                       strcat(dsc, " ");
      if (v & PM_COMPMOVE_RIGHT) strcat(dsc, "R");
      else                       strcat(dsc, " ");
      if (v & PM_COMPMOVE_UP)    strcat(dsc, "U");
      else                       strcat(dsc, " ");
      if (v & PM_COMPMOVE_DOWN)  strcat(dsc, "D");
      else                       strcat(dsc, " ");
      if (v & PM_COMPMOVE_JUMP)  strcat(dsc, "J");
      else                       strcat(dsc, " ");
      if (v & PM_COMPMOVE_FIRE)  strcat(dsc, "F");
      else                       strcat(dsc, " ");
      strcat(dsc, "]");


//      if (v & PM_COMPMOVE_LEFT)  strcat(dsc, "[L]");
//      else                       strcat(dsc, "[ ]");
//      if (v & PM_COMPMOVE_RIGHT) strcat(dsc, "[R]");
//      else                       strcat(dsc, "[ ]");
//      if (v & PM_COMPMOVE_UP)    strcat(dsc, "[U]");
//      else                       strcat(dsc, "[ ]");
//      if (v & PM_COMPMOVE_DOWN)  strcat(dsc, "[D]");
//      else                       strcat(dsc, "[ ]");
//      if (v & PM_COMPMOVE_JUMP)  strcat(dsc, "[J]");
//      else                       strcat(dsc, "[ ]");
//      if (v & PM_COMPMOVE_FIRE)  strcat(dsc, "[F]");
//      else                       strcat(dsc, "[ ]");
//


//      if (v & PM_COMPMOVE_FIRE)  strcat(dsc, "[FIRE]");
//      else                       strcat(dsc, "[    ]");
//      if (v & PM_COMPMOVE_JUMP)  strcat(dsc, "[JUMP]");
//      else                       strcat(dsc, "[    ]");
//      if (v & PM_COMPMOVE_DOWN)  strcat(dsc, "[DOWN]");
//      else                       strcat(dsc, "[    ]");
//      if (v & PM_COMPMOVE_UP)    strcat(dsc, "[UP]");
//      else                       strcat(dsc, "[  ]");
//      if (v & PM_COMPMOVE_RIGHT) strcat(dsc, "[RIGHT]");
//      else                       strcat(dsc, "[     ]");
//      if (v & PM_COMPMOVE_LEFT)  strcat(dsc, "[LEFT]");
//      else                       strcat(dsc, "[    ]");
   }
   sprintf(tmp, "[%3d][%5d][%d][%d][%2d]%s", gm, f, t, p, v, dsc);
   return tmp;
}


char* mwGameMoves::get_gm_text(int gm, char* tmp)
{
   int f = arr[gm][0]; // frame
   int t = arr[gm][1]; // type
   int p = arr[gm][2]; // player
   int v = arr[gm][3]; // value
   return get_gm_text2(gm, f, t, p, v, tmp);
}



void mwGameMoves::save_gm_txt(const char *sfname)
{
   // change extension to .txt
   char fname[128];
   sprintf(fname, "%s", sfname);
   ALLEGRO_PATH *path = al_create_path(fname);
   al_set_path_extension(path, ".txt");
   sprintf(fname, "%s", al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP));

   FILE *filepntr = fopen(fname, "w");
   fprintf(filepntr,"number of entries %d\n", entry_pos);
   fprintf(filepntr,"[ gm][frame][t][p][cm]\n");

   char tmp[100];
   for (int x=0; x<entry_pos; x++)
      fprintf(filepntr, "%s\n", get_gm_text(x, tmp));

   fclose(filepntr);
}





char * mwGameMoves::get_save_txt(int num, char *txt)
{
   if (num == 0) sprintf(txt, "File will be saved");
   if (num == 1) sprintf(txt, "No game moves to save");
   if (num == 2) sprintf(txt, "Never save demo for overworld");
   if (num == 3) sprintf(txt, "Never save demo in demo mode");
   if (num == 4) sprintf(txt, "Never save demo locally for client");
   if (num == 5) sprintf(txt, "Server file transfer disabled");
   return txt;
}


void mwGameMoves::save_gm_make_fn(const char* description, int sendto)
{
   char filename[120];
   sprintf(filename, "savegame/%s-[%02d]-%s.gm", mMiscFnx.get_timestamp(), mLevel.play_level, description);
   save_gm(filename, sendto);
}


void mwGameMoves::save_gm_file_select(void)
{
   char fname[1024];
   sprintf(fname, "savegame/");
   // convert to 'ALLEGRO_FS_ENTRY' (to make fully qualified path)
   ALLEGRO_FS_ENTRY *FS_fname = al_create_fs_entry(fname);
   sprintf(fname, "%s\\", al_get_fs_entry_name(FS_fname));
   //printf("FS_fname:%s\n", fname);
   ALLEGRO_FILECHOOSER *afc = al_create_native_file_dialog(fname, "Save Demo Filename", "*.gm", ALLEGRO_FILECHOOSER_SAVE);

   if (al_show_native_file_dialog(mDisplay.display, afc))
   {
      if (al_get_native_file_dialog_count(afc) == 1)
      {
         sprintf(fname, "%s.gm", al_get_native_file_dialog_path(afc, 0));
         printf("file and path selected:%s\n", fname);
         save_gm(fname, 0);
      }
   }
   else printf("file select cancelled\n");
   al_destroy_native_file_dialog(afc);
}









void mwGameMoves::find_player_info()
{
   printf("find_player_info()\n");
   // looks in the loaded gm file and find all player active and inactive moves

   int am_count = 0;
   int im_count = 0;

//   int par[10][10] = { 0 };


   // first count active and inactive game moves
   for (int x=0; x<entry_pos; x++)
   {
      if (mGameMoves.arr[x][1] &  PM_GAMEMOVE_TYPE_PLAYER_ACTIVE_FLAG) am_count++;
      if (mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_PLAYER_INACTIVE) im_count++;
   }
   printf("player active/inactive moves: %d %d\n", am_count, im_count);
//      if (mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_PLAYER_INACTIVE)
}


// checks a single gma for old player active type and converts if found
void mwGameMoves::check_gma(int x)
{
   if (mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_PLAYER_ACTIVE)
   {
      // old type only has player and color
      int p = arr[x][2];
      int c = arr[x][3];

      // new type also encodes player name
      char name[16];
      sprintf(name, "Player %d", p);

      // convert to new variables
      int type, d1, d2;
      mMiscFnx.val_to_gma(type, d1, d2, p, c, name);
      arr[x][1] = type;
      arr[x][2] = d1;
      arr[x][3] = d2;

      printf("found old gma -- p:%d c:%d -- new: name:%s type:%d d1:%d d2:%d \n", p, c, name, type, d1, d2);
   }
}




int mwGameMoves::save_gm(const char *fname, int sendto)
{
   char msg[256];
   int ret = 0; // good return by default
   if (entry_pos == 0)          ret = 1; // No game moves to save
   if (mLevel.play_level == 1)  ret = 2; // Never save demo for overworld
   if (mDemoMode.mode)          ret = 3; // Never save demo when in demo mode
   if (mNetgame.ima_client)     ret = 4; // Never save demo locally for client

   if ((sendto) && (!server_send_files_to_clients)) ret = 5; // trying to send to client and file transfer disabled

   // only log here if file will not be saved
   if (ret) mLog.log_add_prefixed_textf(LOG_NET_file_transfer, sendto, "save:txt:%s\n", get_save_txt(ret, msg));

   if (sendto) mNetgame.server_send_srrf_packet(sendto, ret);

   // actually do the save
   if (!ret)
   {
      gm_sort();
      save_gm(fname);

      mLog.log_add_prefixed_textf(LOG_NET_file_transfer, sendto, "saved:%s\n", fname);
      sprintf(mDemoRecord.current_loaded_demo_file, "%s", fname); // update current loaded demo filename

      //save_gm_txt(fname); // also save as a human readable text file
      if (sendto) mNetgame.server_add_file_to_send(fname, sendto);
   }
   return ret;
}









bool mwGameMoves::save_gm(const char *fname)
{
   printf("\nSaving gm:%s\n", fname);

   // try to open file for writing
   FILE *filepntr = fopen(fname,"w");
   if (filepntr == NULL)
   {
      printf("Error opening file:%s\n", fname);
      return 0;
   }

   // set version
   HEADER_version = 3;

   // set modified to now
   HEADER_modify_timestamp = mMiscFnx.get_timestamp();

   // if HEADER_create_timestamp is not set, set it here
   if (HEADER_create_timestamp.empty())
   {
      printf("HEADER_create_timestamp was not set. Setting to '20260101-000000'\n");
      HEADER_create_timestamp = "20260101-000000";
   }

   // if HEADER_muid is not set, create one here
   if (HEADER_muid.empty())
   {
      HEADER_muid = mMiscFnx.generate_muid();
      printf("HEADER_muid was not set. Setting to '%s'\n", HEADER_muid.c_str());
   }

   if (status == 1) // new level
   {
      HEADER_num_entries = entry_pos;
      HEADER_last_frame = arr[entry_pos-1][0];
   }

   fprintf(filepntr, "%s", "PMSAVEGAME\n");
   fprintf(filepntr,       "MUID:%s\n",                 HEADER_muid.c_str());
   fprintf(filepntr,       "VERSION:%d\n",              HEADER_version);
   fprintf(filepntr,       "CREATE_TIMESTAMP:%s\n",     HEADER_create_timestamp.c_str());
   fprintf(filepntr,       "MODIFY_TIMESTAMP:%s\n",     HEADER_modify_timestamp.c_str());
   fprintf(filepntr,       "LEVEL:%d\n",                HEADER_level);
   fprintf(filepntr,       "LAST_FRAME:%d\n",           HEADER_last_frame);
   fprintf(filepntr,       "GAMEMOVE_NUM_ENTRIES:%d\n", HEADER_num_entries);
   fprintf(filepntr, "%s", "GAMEMOVE_DATA_START:\n");

   for (int x=0; x<entry_pos; x++)
   {
      check_gma(x); // check for old type player active game moves
      fprintf(filepntr,"%d,%d,%d,%d\n", arr[x][0], arr[x][1], arr[x][2], arr[x][3] );
   }

   fclose(filepntr);


   add_gm_to_db();


   return 1;
}











































void mwGameMoves::print_header(void)
{
   printf("Filename            :%s\n",   last_loaded_gm_filename);
   printf("MUID                :%s\n",   HEADER_muid.c_str());
   printf("VERSION             :%d\n",   HEADER_version);
   printf("CREATE_TIMESTAMP    :%s\n",   HEADER_create_timestamp.c_str());
   printf("MODIFY_TIMESTAMP    :%s\n",   HEADER_modify_timestamp.c_str());
   printf("LEVEL               :%d\n",   HEADER_level);
   printf("LAST_FRAME          :%d\n",   HEADER_last_frame);
   printf("GAMEMOVE_NUM_ENTRIES:%d\n",   HEADER_num_entries);
}









// ------------------------------------------------------------------------------------------------------
// ---------- load file routines ------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------


bool mwGameMoves::parse_header_line(const char * buf)
{
   // convert to string
   string line(buf);

   // remove any CR or LF from end of line
   while (!line.empty() && (line.back() == '\r' || line.back() == '\n')) line.pop_back();

   // Find the position of the token
   size_t pos = line.find(':');

   // Check if the token was found
   if (pos != std::string::npos)
   {
      // Extract the part before the token
      std::string key = line.substr(0, pos);

      // Extract the part after the token ( adding 1 to pos skips the token itself)
      std::string val = line.substr(pos + 1);

      // printf("key:'%s' val:'%s'\n", key.c_str(), val.c_str());

      if (!val.empty())
      {
         if (key == "VERSION")              HEADER_version = std::stoi(val);
         if (key == "MUID")                 HEADER_muid = val;
         if (key == "CREATE_TIMESTAMP")     HEADER_create_timestamp = val;
         if (key == "MODIFY_TIMESTAMP")     HEADER_modify_timestamp = val;
         if (key == "LEVEL")                HEADER_level = std::stoi(val);
         if (key == "LAST_FRAME")           HEADER_last_frame = std::stoi(val);
         if (key == "GAMEMOVE_NUM_ENTRIES") HEADER_num_entries = std::stoi(val);

         // for old v2
         if (key == "GAME_DATA_NUM_ENTRIES") HEADER_num_entries = std::stoi(val);

      }
      else
      {
         if (key == "GAMEMOVE_DATA_START") return false;

         // for old v2
         if (key == "GAME_DATA_START") return false;

      }
   }
   return true;
}


// load level from integer level number
int mwGameMoves::load_demo_level(int lev)
{
   char msg[256];
   sprintf(msg, "savegame/demo/lev%03d.gm", lev);
   return load_gm(msg);
}

// prompts for filename
int mwGameMoves::load_gm_file_select(void)
{
   int good_load = 0;

   char fname[1024];
   sprintf(fname, "savegame/");
   // convert to 'ALLEGRO_FS_ENTRY' (to make fully qualified path)
   ALLEGRO_FS_ENTRY *FS_fname = al_create_fs_entry(fname);
   sprintf(fname, "%s\\", al_get_fs_entry_name(FS_fname));
   //printf("FS_fname:%s\n", fname);

   ALLEGRO_FILECHOOSER *afc = al_create_native_file_dialog(fname, "Run Demo Filename", "*.gm", 0);

   if (al_show_native_file_dialog(mDisplay.display, afc))
   {
      if (al_get_native_file_dialog_count(afc) == 1)
      {
         sprintf(fname, "%s", al_get_native_file_dialog_path(afc, 0));
         printf("file and path selected:%s\n", fname);
         if (load_gm(fname)) good_load = 1;
      }
   }
   else printf("file select cancelled\n" );
   al_destroy_native_file_dialog(afc);
   return good_load;
}

// this is the function that actual loads the gm from the the full file path
// -------------------------------------------------------------------------


int mwGameMoves::load_gm(const char *sfname)
{
   // convert to 'ALLEGRO_FS_ENTRY' (also makes fully qualified path)
   ALLEGRO_FS_ENTRY *FS_fname = al_create_fs_entry(sfname);
   if (!al_fs_entry_exists(FS_fname))
   {
      printf("%s does not exist\n", al_get_fs_entry_name(FS_fname));
      return 0;
   }

   char fname[256];
   sprintf(fname, "%s", al_get_fs_entry_name(FS_fname));
   FILE *file = fopen(fname, "r");
   if (file == NULL)
   {
      printf("Error opening file:%s\n", fname);
      return 0;
   }

   printf("\nLoading gm: '%s'\n", fname);




   // clear the game moves array
   initialize();

   // save filename
   sprintf(last_loaded_gm_filename, "%s", fname);


   int read_last_frame = 0;

   char buffer[1024];
   fgets(buffer, 100, file);
   if (strncmp(buffer, "PMSAVEGAME", 10) == 0)
   {
      // read lines up to end of header
      bool read_header = 1;
      while ((read_header) && (fgets(buffer, 100, file)))
         read_header = parse_header_line(buffer);

      if (HEADER_num_entries)
         for (int x=0; x<HEADER_num_entries; x++)
            if (fscanf(file, "%d,%d,%d,%d\n", &arr[x][0], &arr[x][1], &arr[x][2], &arr[x][3]) > 0) read_last_frame = arr[x][0];
   }
   else
   {
      // did not find "PMSAVEGAME" at start of file, this is the old legacy version

      // get number entries from first line (already in buffer)
      HEADER_num_entries = atoi(buffer);
      if (HEADER_num_entries)
         for (int x=0; x<HEADER_num_entries; x++)
            for (int y=0; y<4; y++)
               if (fgets(buffer, 100, file))
               {
                  arr[x][y] = atoi(buffer);
                  if (y == 0) read_last_frame = arr[x][y];
               }
   }

   fclose(file);

   if (HEADER_num_entries == 0)
   {
      printf("Error! No game move entries found\n");
      print_header();
      initialize();
      return 0;
   }

   entry_pos = HEADER_num_entries;


   int trigger_resave = 0;

   // if HEADER_last_frame was not set from header, set it here form the last frame read
   if (HEADER_last_frame == 0)
   {
      printf("Last frame not set from header, setting to:%d\n", read_last_frame);
      HEADER_last_frame = read_last_frame;
      if (HEADER_last_frame == 0)
      {
         printf("Error! HEADER_last_frame:0\n");
         print_header();
         initialize();
         return 0;
      }
      else trigger_resave = 1;
   }

   // if HEADER_level was not set from header, set it here from the first entry
   if (HEADER_level == 0)
   {
      printf("Level number not set from header, setting to:%d\n", arr[0][3]);
      HEADER_level = arr[0][3];
      if (HEADER_level == 0)
      {
         printf("Error! LEVEL:0\n");
         print_header();
         initialize();
         return 0;
      }
      else trigger_resave = 1;
   }

   // if HEADER_create_timestamp was not set from header, set it here
   if (HEADER_create_timestamp.empty())
   {
      printf("HEADER_create_timestamp was not set. Setting to '20260101-000000'\n");
      HEADER_create_timestamp = "20260101-000000";
      trigger_resave = 1;
   }

   // if HEADER_muid was not set from header, create one here
   if (HEADER_muid.empty())
   {
      HEADER_muid = mMiscFnx.generate_muid();
      printf("HEADER_muid was not set. Setting to '%s'\n", HEADER_muid.c_str());
      trigger_resave = 1;
   }

   if (HEADER_version < 3)
   {
      printf("Loaded old version of file: %s  --  resaving as new version\n", fname);
      trigger_resave = 1;
   }

   if (trigger_resave)
   {
      printf("Resave triggered, resaving:%s\n", fname);
      print_header();
      save_gm(fname);
   }


   mLevel.play_level = HEADER_level; // set play level
   mDemoMode.last_frame = HEADER_last_frame;
   sprintf(mDemoRecord.current_loaded_demo_file, "%s", fname);

//   find_player_info();

//   printf("Loaded gm file:%s\n", fname);

   status = 2;

   add_gm_to_db();

   return 1;
}


#include <iomanip>

// call from load...or save..or somewhere we know HEADER is valid

void mwGameMoves::add_gm_to_db(void)
{
   printf("\nDatabase add or update\n");

   // first convert HEADER_create_timestamp and HEADER_last_frame to dt_start, dt_end, duration
   //printf("HEADER_create_timestamp:'%s'\n", HEADER_create_timestamp.c_str());
   //printf("HEADER_last_frame:'%d'\n", HEADER_last_frame);

   char dts[32] = {0};
   char dte[32] = {0};

   // use HEADER_create_timestamp as is for dts
   sprintf(dts, "%s", HEADER_create_timestamp.c_str());
   int dur = HEADER_last_frame / 40;

   // create stringstream 'ss' from dts
   std::stringstream ss(dts);

   // create struct tm 'timestart'
   struct tm timestart = {0};

   // push ss into get_time, which will convert based on format and put in timestart
   ss >> std::get_time(&timestart, "%Y%m%d-%H%M%S");
   if (ss.fail()) printf("Error parsing time\n");

   // add duration to timestart
   timestart.tm_sec += dur;

   // normalize the struct
   mktime(&timestart);

   // put the result in dte
   strftime(dte, sizeof(dte), "%Y%m%d-%H%M%S", &timestart);

//   printf("dts:'%s'\n", dts);
//   printf("dte:'%s'\n", dte);
//   printf("dur:%d\n", dur);

   // check if muid already exists
   char sql[500];
   sprintf(sql, "SELECT COUNT(*) FROM gm WHERE muid='%s'", HEADER_muid.c_str());
   if (mSql.execute_sql_and_return_one_int(sql)) printf("muid: '%s' exists\n", HEADER_muid.c_str());
   else
   {
      printf("muid: '%s' does not exist  --  inserting\n", HEADER_muid.c_str());
      sprintf(sql, "INSERT INTO gm ( muid, filename, dt_start, dt_end, duration, level, num_entries ) \
                             VALUES( '%s', '%s',     '%s',     '%s',   %d,       %d,    %d)" ,
                                     HEADER_muid.c_str(),
                                           last_loaded_gm_filename,
                                                     dts,      dte,    dur,      HEADER_level,
                                                                                        HEADER_num_entries);
      // printf("sql:%s\n", sql);
      mSql.execute_sql(sql);

      create_gm_session_links();
   }
}


void mwGameMoves::create_gm_session_links()
{
   if (mNetgame.ima_server)
      for (int p=1; p<8; p++)
         if (mPlayer.loc[p].session_id) create_gm_session_link(mPlayer.loc[p].session_id);
}


void mwGameMoves::create_gm_session_link(int session_id)
{
   char sql[500];
   sprintf(sql, "SELECT COUNT(*) FROM gm_sessions WHERE muid='%s' AND session_id=%d", HEADER_muid.c_str(), session_id);
   if (mSql.execute_sql_and_return_one_int(sql)) printf("link exists: muid: '%s' session_d: %d\n", HEADER_muid.c_str(), session_id);
   else
   {
      sprintf(sql, "INSERT INTO gm_sessions (id, gm_muid, session_id) VALUES(NULL, '%s', %d)", HEADER_muid.c_str(), session_id);
      printf("sql:%s\n", sql);
      mSql.execute_sql(sql);
   }
}























