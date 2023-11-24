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
#include "mwScreen.h"
#include "mwMain.h"



mwGameMoves mGameMoves;

mwGameMoves::mwGameMoves()
{
   initialize();
}

void mwGameMoves::initialize(void)
{
   for (int x=0; x<GAME_MOVES_SIZE; x++)
      for (int y=0; y<4; y++)
         arr[x][y] = 0;
   entry_pos = 0;
   current_pos = 0;
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

// this function processes all entries in the game_moves array that match current frame_num
void mwGameMoves::proc(void)
{
   // search entire range
   int start_index = entry_pos-1;
   int end_index = 0;

   // reduce search range
   start_index = current_pos + 100;
   if (start_index > entry_pos-1) start_index = entry_pos-1;

   end_index = current_pos - 100;
   if (end_index < 0) end_index = 0;

   for (int x=start_index; x>=end_index; x--)  // search backwards from start_index to end_index
   {
      if (arr[x][0] == mLoop.frame_num) // found frame number that matches current frame
      {
         if (x > current_pos) current_pos = x; // index of last used gm - keep this at the most recent one, never go back
         switch (arr[x][1])
         {
            case PM_GAMEMOVE_TYPE_PLAYER_ACTIVE:    proc_player_active_game_move(x); break;
            case PM_GAMEMOVE_TYPE_PLAYER_INACTIVE:  proc_player_inactive_game_move(x); break;

            case PM_GAMEMOVE_TYPE_PLAYER_HIDDEN:    proc_player_hidden_game_move(x); break;

            case PM_GAMEMOVE_TYPE_MOVE:             mPlayer.set_controls_from_comp_move(arr[x][2], arr[x][3]); break;
         }
      }
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

   // ----------------------------------------------------------------------------------------
   // if we are in level_done_mode 5, all moves are converted to type 8, level done acknowledge
   // ----------------------------------------------------------------------------------------
   if ((mPlayer.syn[0].level_done_mode == 5) && (type == 5) && (data2)) // all move type inputs are acks in this mode
   {
      if (!has_player_acknowledged(data1)) // to prevent multiple acks
         add_game_move2(frame, PM_GAMEMOVE_TYPE_LEVEL_DONE_ACK, data1, 0); // type 8 - acknowledge level done
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
      if ((mPlayer.active_local_player == 0) && (mPlayer.syn[0].control_method == PM_PLAYER_CONTROL_METHOD_SINGLE_PLAYER))
      {
         // do not enter game move, just exit to menu
         mLoop.state[0] = PM_PROGRAM_STATE_MENU;
         mLevel.resume_allowed = 1;
         return;
      }
      // ----------------------------------------------------------------------------------------
      // local server quitting
      // ----------------------------------------------------------------------------------------
      if ((mNetgame.ima_server) && (data1 == 0))
      {
         // first set all connected clients to inactive
         for (int p=1; p<NUM_PLAYERS; p++)
            if (mPlayer.syn[p].control_method == PM_PLAYER_CONTROL_METHOD_NETGAME_REMOTE)
            {
               // mPlayer.syn[p].control_method = PM_PLAYER_CONTROL_METHOD_CLIENT_THAT_SERVER_QUIT_ON; // what does this even do?
               add_game_move2(frame + 10, PM_GAMEMOVE_TYPE_PLAYER_INACTIVE, p, 64);
            }
         // then set server to inactive in future
         add_game_move2(frame + 10, PM_GAMEMOVE_TYPE_PLAYER_INACTIVE, 0, 64);
         return;
      }
      // ----------------------------------------------------------------------------------------
      // remote client quitting (server only)
      // ----------------------------------------------------------------------------------------
      if ((mNetgame.ima_server) && (data1 > 0))
      {
         add_game_move2(frame + 2, PM_GAMEMOVE_TYPE_PLAYER_INACTIVE, data1, 64);
         return;
      }
      // ----------------------------------------------------------------------------------------
      // local client quitting (client only)
      // ----------------------------------------------------------------------------------------
      if (mNetgame.ima_client)
      {
         add_game_move2(frame + 4, PM_GAMEMOVE_TYPE_PLAYER_INACTIVE, data1, 64);
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


void mwGameMoves::proc_player_hidden_game_move(int x)
{
   int p = arr[x][2];  // player number

   mPlayer.syn[p].paused = 1;
   mPlayer.syn[p].paused_type = 3;

}

void mwGameMoves::proc_player_active_game_move(int x)
{
   int p = arr[x][2];  // player number
   int c = arr[x][3];  // color

   mPlayer.syn[p].color = c; // color

   // player was inactive before and just now changes to active
   if (mPlayer.syn[p].active == 0)
   {
      mPlayer.set_player_start_pos(p, 0); // set starting position
      mPlayer.syn[p].active = 1;
      mPlayer.loc[p].join_frame = mLoop.frame_num;

      if ((mNetgame.ima_server) || (mNetgame.ima_client))
         if (p != mPlayer.active_local_player) mPlayer.syn[p].control_method = PM_PLAYER_CONTROL_METHOD_NETGAME_REMOTE;

      // if player 0 is file play all added players will be too
      if (mPlayer.syn[0].control_method == 1) mPlayer.syn[p].control_method = PM_PLAYER_CONTROL_METHOD_DEMO_MODE;

      mScreen.set_player_text_overlay(p, 1);

      mGameEvent.add(6, 0, 0, p, 0, 0, 0);

      if (!mLoop.ff_state)
      {
         //if ((mMain.headless_server) && (p)) printf("Player:%d joined\n", p);
         mLog.add_headerf(LOG_NET, p, 0, "Player:%d became ACTIVE!                                ", p);
      }
   }
}

void mwGameMoves::proc_player_inactive_game_move(int x)
{
   int p   = arr[x][2]; // player number
   int val = arr[x][3]; // reason

   mPlayer.loc[p].quit_frame = mLoop.frame_num;

   // ------------------------------------
   // player never became active
   // ------------------------------------
   if ((mPlayer.syn[p].active == 0) && (mPlayer.syn[p].control_method == PM_PLAYER_CONTROL_METHOD_NETGAME_REMOTE))
   {
      mPlayer.loc[p].join_frame = mLoop.frame_num;
      mPlayer.loc[p].quit_reason = 74;
//      mPlayer.syn[p].control_method = PM_PLAYER_CONTROL_METHOD_CLIENT_USED; // prevent re-use of this player number in this level
      mPlayer.syn[p].control_method = PM_PLAYER_CONTROL_METHOD_SINGLE_PLAYER;
      mPlayer.loc[p].who = 99;
   }

   if (mPlayer.syn[p].active)
   {
      //if (mMain.headless_server) printf("Player:%d quit\n", p);

      if (!mLoop.ff_state) mLog.add_headerf(LOG_NET, p, 0, "Player:%d became INACTIVE!                              ", p);

      // ------------------------------------
      // player in run demo mode became inactive
      // ------------------------------------
      if (mPlayer.syn[p].control_method == PM_PLAYER_CONTROL_METHOD_DEMO_MODE)
      {
         mPlayer.syn[p].active = 0;
         // only quit if no players left active
         int still_active = 0;
         for (int p=0; p<NUM_PLAYERS; p++)
            if (mPlayer.syn[p].active) still_active = 1;
         if (!still_active) mLoop.state[0] = PM_PROGRAM_STATE_MENU;
      }

      // ------------------------------------
      // local server player quit
      // ------------------------------------
      if (mPlayer.syn[p].control_method == PM_PLAYER_CONTROL_METHOD_SERVER_LOCAL)
      {
         // printf("Local Server Player Quit :%d\n", mLoop.frame_num);
         mPlayer.loc[p].quit_reason = 91;

         // set quit reason for all active clients on server
         for (int pp=1; pp<NUM_PLAYERS; pp++)
            if ((mPlayer.syn[pp].active) && (mPlayer.syn[pp].control_method == PM_PLAYER_CONTROL_METHOD_NETGAME_REMOTE))
               mPlayer.loc[pp].quit_reason = 91;

         if (!mLoop.ff_state) mLog.log_ending_stats_server(LOG_NET_ending_stats);
         mLoop.state[0] = PM_PROGRAM_STATE_MENU;
      }

      // ------------------------------------
      // remote server quit
      // ------------------------------------
      if ((mNetgame.ima_client) && (p == 0))
      {
         // printf("Remote Server Quit :%d\n", mLoop.frame_num);
         if (val == 64) mPlayer.loc[mPlayer.active_local_player].quit_reason = 92;
         mLoop.state[0] = PM_PROGRAM_STATE_MENU;
      }

      // ------------------------------------
      // remote player quit (server only)
      // ------------------------------------
      if (mPlayer.syn[p].control_method == PM_PLAYER_CONTROL_METHOD_NETGAME_REMOTE)
      {
         // printf("Remote Player Quit :%d\n", mLoop.frame_num);
         mPlayer.loc[p].quit_reason = 93;
         if (!mLoop.ff_state) mLog.log_ending_stats_client(LOG_NET_ending_stats, p);
         mPlayer.init_player(p, 1);
         mNetgame.mStateHistory[p].initialize();  // reset client states

//         mPlayer.syn[p].active = 0;
//         mPlayer.syn[p].control_method = PM_PLAYER_CONTROL_METHOD_CLIENT_USED; // prevent re-use of this player number in this level
//         mPlayer.loc[p].who = 99;
      }
      mScreen.set_player_text_overlay(p, 0);
      mGameEvent.add(7, 0, 0, p, 0, 0, 0);
   }
}

char* cmtos(int cm, char* tmp)
{
   sprintf(tmp, " ");
   if (cm > 31) { cm -= 32; strcat(tmp, "[FIRE]");  }
   else                     strcat(tmp, "[    ]");
   if (cm > 15) { cm -= 16; strcat(tmp, "[JUMP]");  }
   else                     strcat(tmp, "[    ]");
   if (cm > 7)  { cm -= 8;  strcat(tmp, "[DOWN]");  }
   else                     strcat(tmp, "[    ]");
   if (cm > 3)  { cm -= 4;  strcat(tmp, "[UP]");    }
   else                     strcat(tmp, "[  ]");
   if (cm > 1)  { cm -= 2;  strcat(tmp, "[RIGHT]"); }
   else                     strcat(tmp, "[     ]");
   if (cm > 0)  { cm -= 1;  strcat(tmp, "[LEFT]");  }
   else                     strcat(tmp, "[    ]");
   return tmp;
}

void mwGameMoves::save_gm_txt(char *sfname)
{
   char tmp[100];
   FILE *filepntr;
   char fname[80];
   sprintf(fname, "savegame/%s.txt", sfname);

   printf("%s - saved\n", fname);

   //replace_extension(fname, fname, "txt", sizeof(fname) );

   filepntr = fopen(fname,"w");

   fprintf(filepntr,"number of entries %d\n", entry_pos);
   fprintf(filepntr,"player_vs_player_shots %d\n",       mPlayer.syn[0].player_vs_player_shots);
   fprintf(filepntr,"player_vs_player_shot_damage %d\n", mPlayer.syn[0].player_vs_player_shot_damage);
   fprintf(filepntr,"player_vs_self_shots %d\n",         mPlayer.syn[0].player_vs_self_shots);
   fprintf(filepntr,"[ gm][frame][t][p][cm]\n");
   for (int x=0; x<entry_pos; x++)
   {
      int f = arr[x][0]; // frame
      int t = arr[x][1]; // type
      int p = arr[x][2]; // player
      int v = arr[x][3]; // value

      fprintf(filepntr,"[%3d][%5d][%d][%d][%2d]", x, f, t, p, v);

      if (t == PM_GAMEMOVE_TYPE_LEVEL_START)     fprintf(filepntr,"-------------START (level:%d)------------- ", v);
      if (t == PM_GAMEMOVE_TYPE_PLAYER_ACTIVE)   fprintf(filepntr,"-------------PLAYER %d ACTIVE (color:%d)-- ", p, v);
      if (t == PM_GAMEMOVE_TYPE_PLAYER_INACTIVE) fprintf(filepntr,"-------------PLAYER %d INACTIVE------------", p);
      if (t == PM_GAMEMOVE_TYPE_MOVE)            fprintf(filepntr,"%s", cmtos(arr[x][3], tmp));
      if (t == PM_GAMEMOVE_TYPE_LEVEL_DONE_ACK)  fprintf(filepntr,"-------------PLAYER %d ACKNOWLEDGE---------", p);

      fprintf(filepntr,"\n");
   }
   fclose(filepntr);
}

void mwGameMoves::save_gm(char *sfname)
{
   if (entry_pos)
   {
      FILE *filepntr;
      char fname[80];
      sprintf(fname, "savegame/%s.gm", sfname);
      filepntr = fopen(fname,"w");

      fprintf(filepntr,"%d\n", entry_pos);  // num_entries

      fprintf(filepntr,"%d\n", mPlayer.syn[0].player_vs_player_shots);
      fprintf(filepntr,"%d\n", mPlayer.syn[0].player_vs_player_shot_damage );
      fprintf(filepntr,"%d\n", mPlayer.syn[0].player_vs_self_shots);

      for (int x=0; x<entry_pos; x++)
         for (int y=0; y<4; y++)
            fprintf(filepntr,"%d\n", arr[x][y]);
      fclose(filepntr);

      save_gm_txt(sfname); // then as a human readable text file
   }
}

void mwGameMoves::save_gm_file_select()
{
   char fname[1024];
   sprintf(fname, "savegame/");
   //printf("fname:%s\n", fname);
   // convert to 'ALLEGRO_FS_ENTRY' (also makes fully qualified path)
   ALLEGRO_FS_ENTRY *FS_fname = al_create_fs_entry(fname);
   sprintf(fname, "%s\\", al_get_fs_entry_name(FS_fname));
   //printf("FS_fname:%s\n", fname);
   ALLEGRO_FILECHOOSER *afc = al_create_native_file_dialog(fname, "Save Demo Filename", "*.gm", ALLEGRO_FILECHOOSER_SAVE);

   if (al_show_native_file_dialog(mDisplay.display, afc))
   {
      if (al_get_native_file_dialog_count(afc) == 1)
      {
         const char * r = al_get_native_file_dialog_path(afc, 0);
         sprintf(fname, "%s", r);
         //printf("file and path selected:%s\n", fname);
         // after all this i am going to strip away the fully qualified path and get just the filename
         ALLEGRO_PATH *p = al_create_path(fname);
         sprintf(fname, "%s", al_get_path_filename(p));
         //printf("file selected:%s\n", fname);
         save_gm(fname);
      }
   }
   else printf("file select cancelled\n" );
   al_destroy_native_file_dialog(afc);
}

void mwGameMoves::autosave_gm(int d)
{
   char description[40];
   if (d == 1) sprintf(description, "level_done_");
   if (d == 2) sprintf(description, "level_quit_");
   if (d == 3) sprintf(description, "program_exit_");
   if (d == 4) sprintf(description, "manual_save_");

   char timestamp[40];
   struct tm *timenow;
   time_t now = time(NULL);
   timenow = localtime(&now);
   strftime(timestamp, sizeof(timestamp), "%Y%m%d-%H%M%S", timenow);

   char lev[20];
   sprintf(lev, "-lev%d", mLevel.play_level);

   char filename[120];
   sprintf(filename, "%s%s%s", description, timestamp, lev);
   save_gm(filename);
}


int mwGameMoves::load_gm(int lev)
{
   char msg[256];
   sprintf(msg, "savegame/demo/lev%03d.gm", lev);
   return load_gm(msg);
}

int mwGameMoves::load_gm(const char *sfname )
{
   int debug_print = 0;

   if (debug_print) printf("sfname:%s\n", sfname);

   int bad_filename = 0;

   FILE *filepntr;
   char fname[1024];

   if (strlen(sfname) >= 0)
   {

      sprintf(fname, "%s", sfname);

      if (debug_print) printf("fname:%s\n", fname);

      // convert to 'ALLEGRO_FS_ENTRY' (also makes fully qualified path)
      ALLEGRO_FS_ENTRY *FS_fname = al_create_fs_entry(fname);
      sprintf(fname, "%s", al_get_fs_entry_name(FS_fname));

      if (debug_print) printf("FS_fname:%s\n", fname);

      // check if the passed filename exists
      if (!al_fs_entry_exists(FS_fname))
      {
         if (debug_print) printf("%s does not exist\n", al_get_fs_entry_name(FS_fname));
         bad_filename = 1;
      }
   }
   else
   {
      if (debug_print) printf("No filename passed, staring file select\n");
      bad_filename = 1;
   }


   if (!bad_filename)
   {
      // check the filename and extension
      ALLEGRO_PATH * path = al_create_path(fname);
      const char *tmp = al_get_path_filename(path);
      if (strlen(tmp)<1) bad_filename = 1;
      const char *tme = al_get_path_extension(path);
      if (strcmp(tme, ".gm") !=0) bad_filename = 1;
      al_destroy_path(path);
   }

   // prompt to select file
   if (bad_filename)
   {
      sprintf(fname, "savegame/");

      if (debug_print) printf("fname:%s\n", fname);

      // convert to 'ALLEGRO_FS_ENTRY' (also makes fully qualified path)
      ALLEGRO_FS_ENTRY *FS_fname = al_create_fs_entry(fname);
      sprintf(fname, "%s\\", al_get_fs_entry_name(FS_fname));

      if (debug_print) printf("FS_fname:%s\n", fname);

      ALLEGRO_FILECHOOSER *afc = al_create_native_file_dialog(fname, "Run Demo Filename", "*.gm", 0);

      if (al_show_native_file_dialog(mDisplay.display, afc))
      {
         if (al_get_native_file_dialog_count(afc) == 1)
         {
            sprintf(fname, "%s", al_get_native_file_dialog_path(afc, 0));
            if (debug_print) printf("file selected:%s\n", fname);
         }

         // check to see if we selected a good filename

         // start by assuming its good

         bad_filename = 0;
         // convert to 'ALLEGRO_FS_ENTRY' (also makes fully qualified path)
         ALLEGRO_FS_ENTRY *FS_fname = al_create_fs_entry(fname);
         sprintf(fname, "%s", al_get_fs_entry_name(FS_fname));

         // check if it exists
         if (!al_fs_entry_exists(FS_fname))
         {
            //printf("%s does not exist\n", al_get_fs_entry_name(FS_fname));
            bad_filename = 1;
         }
         if (!bad_filename)
         {
            // check the filename and extension
            ALLEGRO_PATH * path = al_create_path(fname);
            const char *tmp = al_get_path_filename(path);
            if (strlen(tmp)<1) bad_filename = 1;
            const char *tme = al_get_path_extension(path);
            if (strcmp(tme, ".gm") !=0) bad_filename = 1;
            al_destroy_path(path);
         }
      }
      else if (debug_print) printf("file select cancelled\n" );
      al_destroy_native_file_dialog(afc);
   }




   // after all that, now do we now have a valid filename?
   if (!bad_filename)
   {
      //printf("processing file %s\n", fname);

      initialize();

      int loop, ch;
      char buff[2000];
      if ((filepntr=fopen(fname, "r")))
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


         // then pvp_shots
         loop = 0;
         ch = fgetc(filepntr);
         while((ch != '\n') && (ch != EOF))
         {
            buff[loop] = ch;
            loop++;
            ch = fgetc(filepntr);
         }
         buff[loop] = 0;
         mPlayer.syn[0].player_vs_player_shots = atoi(buff);



         // then pvp_shot_damage
         loop = 0;
         ch = fgetc(filepntr);
         while((ch != '\n') && (ch != EOF))
         {
            buff[loop] = ch;
            loop++;
            ch = fgetc(filepntr);
         }
         buff[loop] = 0;
         mPlayer.syn[0].player_vs_player_shot_damage = atoi(buff);

         // then pvs_shots
         loop = 0;
         ch = fgetc(filepntr);
         while((ch != '\n') && (ch != EOF))
         {
            buff[loop] = ch;
            loop++;
            ch = fgetc(filepntr);
         }
         buff[loop] = 0;
         mPlayer.syn[0].player_vs_self_shots = atoi(buff);

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
         if (debug_print) printf("dmlf:%d\n", mDemoMode.last_frame );
         return 1;
      }
   }
   return 0;
}


