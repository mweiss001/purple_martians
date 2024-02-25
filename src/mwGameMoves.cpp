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




mwGameMoves mGameMoves;

mwGameMoves::mwGameMoves()
{
   initialize();
}

void mwGameMoves::initialize(void)
{
   for (int x=0; x<GAME_MOVES_SIZE; x++) clear_single(x);
   entry_pos = 0;
   current_pos = 0;
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
         if (!(mLoop.state[1] == PM_PROGRAM_STATE_DEMO_RECORD))
            if (x > current_pos) current_pos = x; // index of last used gm - keep this at the most recent one, never go back

         switch (arr[x][1])
         {
            case PM_GAMEMOVE_TYPE_PLAYER_ACTIVE:    proc_game_move_player_active(x); break;
            case PM_GAMEMOVE_TYPE_PLAYER_INACTIVE:  proc_game_move_player_inactive(x); break;
            case PM_GAMEMOVE_TYPE_PLAYER_HIDDEN:    proc_game_move_player_hidden(x); break;
            case PM_GAMEMOVE_TYPE_SHOT_CONFIG:      proc_game_move_shot_config(x); break;
            case PM_GAMEMOVE_TYPE_PLAYER_MOVE:      mPlayer.set_controls_from_comp_move(arr[x][2], arr[x][3]); break;
         }
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
      if ((mPlayer.active_local_player == 0) && (mPlayer.syn[0].control_method == PM_PLAYER_CONTROL_METHOD_SINGLE_PLAYER))
      {
         mLoop.state[0] = PM_PROGRAM_STATE_MENU;
         mLevel.resume_allowed = 1;
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
                  mPlayer.syn[p].control_method = PM_PLAYER_CONTROL_METHOD_CLIENT_THAT_SERVER_QUIT_ON; // this lets the clients quit gracefully instead of timing out with lost server connection
                  add_game_move2(frame + 10, PM_GAMEMOVE_TYPE_PLAYER_INACTIVE, p, PM_PLAYER_QUIT_REASON_SERVER_ENDED_GAME);
               }

            // then set server to inactive in future
            add_game_move2(frame + 10, PM_GAMEMOVE_TYPE_PLAYER_INACTIVE, 0, PM_PLAYER_QUIT_REASON_SERVER_ENDED_GAME);
            return;
         }
         else // client quitting
         {
            add_game_move2(frame + 2, PM_GAMEMOVE_TYPE_PLAYER_INACTIVE, data1, PM_PLAYER_QUIT_REASON_SERVER_ENDED_GAME);
            return;
         }
      }

      // ----------------------------------------------------------------------------------------
      // local client quitting
      // ----------------------------------------------------------------------------------------
      if (mNetgame.ima_client)
      {
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


void mwGameMoves::proc_game_move_shot_config(int i)
{
   mPlayer.syn[0].player_vs_player_shots       = arr[i][2] & 0b01;
   mPlayer.syn[0].player_vs_self_shots         = arr[i][2] & 0b10;
   mPlayer.syn[0].player_vs_player_shot_damage = arr[i][3];
}

void mwGameMoves::proc_game_move_player_hidden(int x)
{
   int p = arr[x][2];  // player number
   mPlayer.syn[p].paused = 1;
   mPlayer.syn[p].paused_type = 3;
}

void mwGameMoves::proc_game_move_player_active(int x)
{
   int p = arr[x][2];  // player number
   int c = arr[x][3];  // color

   // if player was already active, just change the color
   mPlayer.syn[p].color = c; // color

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
         if (!mLoop.ff_state) mLog.add_headerf(LOG_NET, p, 0, "Player:%d became ACTIVE!                                ", p);
      }
   }
}

void mwGameMoves::proc_game_move_player_inactive(int x)
{
   int p = arr[x][2]; // player number
   mPlayer.loc[p].quit_reason = arr[x][3];
   mPlayer.loc[p].quit_frame  = mLoop.frame_num;

   if (mPlayer.syn[p].active)
   {
      if (!mLoop.ff_state) mLog.add_headerf(LOG_NET, p, 0, "Player:%d became INACTIVE!                              ", p);
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
      mScreen.add_player_text_overlay(p, 0);
      mGameEvent.add(7, 0, 0, p, 0, 0, 0);
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
   char tmp[100];

   char fname[128];
   sprintf(fname, "%s", sfname);

   // change extension to .txt
   ALLEGRO_PATH *path = al_create_path(fname);
   al_set_path_extension(path, ".txt");
   sprintf(fname, "%s", al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP));

   FILE *filepntr = fopen(fname,"w");
   fprintf(filepntr,"number of entries %d\n", entry_pos);
   fprintf(filepntr,"[ gm][frame][t][p][cm]\n");
   for (int x=0; x<entry_pos; x++)
      fprintf(filepntr, "%s\n", get_gm_text(x, tmp));
   fclose(filepntr);

   if (mNetgame.ima_server) // if server, send to all active clients
   {
      for (int p=1; p<NUM_PLAYERS; p++)
         if ((mPlayer.syn[p].active) && (mPlayer.syn[p].control_method == PM_PLAYER_CONTROL_METHOD_NETGAME_REMOTE))
            mNetgame.server_add_file_to_send(fname, mPlayer.loc[p].who);
   }
}


void mwGameMoves::save_gm(const char *fname)
{
   int print_level = 1;

   int skip = 0;
   if (mNetgame.ima_client)     { skip = 1;  if (print_level > 1) printf("Never save demo for client\n");        } // do not save for client, the gm is missing data and is not playable
   if (entry_pos == 0)          { skip = 1;  if (print_level > 1) printf("No game moves to save\n");             }
   if (mLevel.play_level == 1)  { skip = 1;  if (print_level > 1) printf("Never save demo for overworld\n");     }
   if (mDemoMode.mode)          { skip = 1;  if (print_level > 1) printf("Never save demo when in demo mode\n"); }

   if (!skip)
   {
      gm_sort();

      FILE *filepntr = fopen(fname,"w");
      if (filepntr == NULL) printf("Error opening file:%s\n", fname);
      else
      {
         fprintf(filepntr,"%d\n", entry_pos);  // num_entries

         for (int x=0; x<entry_pos; x++)
            for (int y=0; y<4; y++)
               fprintf(filepntr,"%d\n", arr[x][y]);
         fclose(filepntr);

         if (print_level) printf("saved:%s\n", fname);

         if ((mNetgame.ima_server) && (mGameMoves.server_send_gm_to_clients)) // if server, send to all active clients
         {
            for (int p=1; p<NUM_PLAYERS; p++)
               if ((mPlayer.syn[p].active) && (mPlayer.syn[p].control_method == PM_PLAYER_CONTROL_METHOD_NETGAME_REMOTE))
                  mNetgame.server_add_file_to_send(fname, mPlayer.loc[p].who);
         }

         sprintf(mDemoRecord.current_loaded_demo_file, "%s", fname); // update the name

         if (0) save_gm_txt(fname); // also save as a human readable text file
      }
   }
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
         save_gm(fname);
      }
   }
   else printf("file select cancelled\n");
   al_destroy_native_file_dialog(afc);
}

void mwGameMoves::save_gm_make_fn(const char* description)
{
   char timestamp[40];
   struct tm *timenow;
   time_t now = time(NULL);
   timenow = localtime(&now);
   strftime(timestamp, sizeof(timestamp), "%Y%m%d-%H%M%S", timenow);
   char filename[120];
   sprintf(filename, "savegame/%s-[%02d]-%s.gm", timestamp, mLevel.play_level, description);
   save_gm(filename);
}


int mwGameMoves::load_demo_level(int lev)
{
   char msg[256];
   sprintf(msg, "savegame/demo/lev%03d.gm", lev);
   return load_gm(msg);
}

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



int mwGameMoves::load_gm(const char *sfname)
{
   // convert to 'ALLEGRO_FS_ENTRY' (also makes fully qualified path)
   ALLEGRO_FS_ENTRY *FS_fname = al_create_fs_entry(sfname);
   if (!al_fs_entry_exists(FS_fname))
   {
      printf("%s does not exist\n", al_get_fs_entry_name(FS_fname));
      return 0;
   }
   else
   {
      int loop, ch;
      char buff[2000];

      initialize();

      char fname[256];
      sprintf(fname, "%s", al_get_fs_entry_name(FS_fname));

      FILE *filepntr = fopen(fname, "r");
      if (filepntr == NULL)
      {
         printf("Error opening file:%s\n", fname);
         return 0;
      }
      else
      {
         // first get number of entries
         loop = 0;
         ch = fgetc(filepntr);
         while((ch != '\n') && (ch != EOF))
         {
            buff[loop] = ch;
            loop++;
            ch = fgetc(filepntr);
         }
         buff[loop] = 0;
         entry_pos = atoi(buff);

         // then get all the entries
         for (int x=0; x<entry_pos; x++)
            for (int y=0; y<4; y++)
            {
               loop = 0;
               ch = fgetc(filepntr);
               while((ch != '\n') && (ch != EOF))
               {
                  buff[loop] = ch;
                  loop++;
                  ch = fgetc(filepntr);
               }
               buff[loop] = 0;
               arr[x][y] = atoi(buff);
            }
         fclose(filepntr);

         mLevel.play_level = arr[0][3]; // set play level
         mDemoMode.last_frame = arr[entry_pos-1][0];
         //printf("dmlf:%d\n", mDemoMode.last_frame );
         //printf("fname:%s\n", fname);
         sprintf(mDemoRecord.current_loaded_demo_file, "%s", fname);
         return 1;
      }
   }
   return 0;
}
