// mwGameMovesArray.cpp

#include "pm.h"
#include "mwGameMovesArray.h"
#include "mwPlayers.h"
#include "mwNetgame.h"
#include "mwLog.h"
#include "mwLoop.h"
#include "mwLevel.h"
#include "mwGameEvent.h"
#include "mwInput.h"
#include "mwShots.h"
#include "mwDisplay.h"
#include "mwDemoMode.h"
#include "mwScreen.h"


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






// this function processes all entries in the game_moves array that match current mLoop.frame_num
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
      if (game_moves[x][0] == mLoop.frame_num) // found frame number that matches current frame
      {
         if (x > game_move_current_pos) game_move_current_pos = x; // index of last used gm - keep this at the most recent one, never go back
         switch (game_moves[x][1])
         {
            case 1: proc_player_active_game_move(x); break;
            case 2: proc_player_inactive_game_move(x); break;
            case 3: proc_player_client_join_game_move(x); break;
            case 4: proc_player_client_quit_game_move(x); break;
            case 5: mPlayer.set_controls_from_comp_move(game_moves[x][2], game_moves[x][3]); break;
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
   char msg[1024];

   // ----------------------------------------------------------------------------------------
   // if we are in level_done_mode 5, all moves are converted to type 8, level done acknowledge
   // ----------------------------------------------------------------------------------------
   if ((mPlayer.syn[0].level_done_mode == 5) && (type == 5) && (data2)) // all move type inputs are acks in this mode
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
      if ((mPlayer.active_local_player == 0) && (mPlayer.syn[0].control_method == 0))
      {
         // do not enter game move, just exit to menu
         mLoop.new_program_state = 1;
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
            if (mPlayer.syn[p].control_method == 2)
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
      if ((mNetgame.ima_server) && (data1 > 0))
      {
         add_game_move2(frame + 2, 2, data1, 64); // type 2 - player inactive
         return;
      }
      // ----------------------------------------------------------------------------------------
      // local client quitting (client only)
      // ----------------------------------------------------------------------------------------
      if (mNetgame.ima_client)
      {
         add_game_move2(frame + 4, 2, data1, 64); // type 2 - player inactive
         return;
      }

      sprintf(msg, "Error: menu key not handled...should never get here");
      mI.m_err(msg);

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
   if (mNetgame.ima_server)
   {
      int p = game_moves[x][2];  // player number
      int c = game_moves[x][3];  // color
      mPlayer.syn[p].control_method = 2;
      mPlayer.syn[p].color = c;
   }
}

void mwGameMovesArray::proc_player_client_quit_game_move(int x)
{
   if (mNetgame.ima_server)
   {
      int p = game_moves[x][2];  // player number
      mPlayer.syn[p].control_method = 8;
   }
}

void mwGameMovesArray::proc_player_active_game_move(int x)
{
   char msg[1024];

   int p            = game_moves[x][2]; // player number
   mPlayer.syn[p].color = game_moves[x][3]; // color

   // player was inactive before and just now changes to active
   if (mPlayer.syn[p].active == 0)
   {
      mPlayer.set_player_start_pos(p, 0); // set starting position
      mPlayer.syn[p].active = 1;
      mPlayer.loc[p].join_frame = mLoop.frame_num;

      if ((mNetgame.ima_server) || (mNetgame.ima_client))
         if (p != mPlayer.active_local_player) mPlayer.syn[p].control_method = 2;

      // if player 0 is file play all added players will be too
      if (mPlayer.syn[0].control_method == 1) mPlayer.syn[p].control_method = 1;

      mScreen.set_player_join_quit_display(p, 1, 60);

      mGameEvent.add(80, 0, 0, p, 0, 0, 0);

      if (mLog.LOG_NET)
      {
         sprintf(msg,"PLAYER:%d became ACTIVE color:%d", p, mPlayer.syn[p].color);
         mLog.add_log_entry_header(10, p, msg, 1);
      }
   }
}

void mwGameMovesArray::proc_player_inactive_game_move(int x)
{
   char msg[1024];
   int p   = game_moves[x][2]; // player number
   int val = game_moves[x][3]; // reason

   mPlayer.loc[p].quit_frame = mLoop.frame_num;

   // ------------------------------------
   // player never became active
   // ------------------------------------
   if ((mPlayer.syn[p].active == 0) && (mPlayer.syn[p].control_method == 2))
   {
      mPlayer.loc[p].join_frame = mLoop.frame_num;
      mPlayer.loc[p].quit_reason = 74;
//      mPlayer.syn[p].control_method = 9; // prevent re-use of this player number in this level
      mPlayer.syn[p].control_method = 0;
      mPlayer.loc[p].who = 99;
   }

   if (mPlayer.syn[p].active)
   {
      if (mLog.LOG_NET)
      {
         sprintf(msg,"PLAYER:%d became INACTIVE", p);
         mLog.add_log_entry_header(10, p, msg, 1);
      }

      // ------------------------------------
      // player in run demo mode became inactive
      // ------------------------------------
      if (mPlayer.syn[p].control_method == 1)
      {
         mPlayer.syn[p].active = 0;
         // only quit if no players left active
         int still_active = 0;
         for (int p=0; p<NUM_PLAYERS; p++)
            if (mPlayer.syn[p].active) still_active = 1;
         if (!still_active) mLoop.new_program_state = 1;
      }

      // ------------------------------------
      // local server player quit
      // ------------------------------------
      if (mPlayer.syn[p].control_method == 3)
      {
         // printf("Local Server Player Quit :%d\n", mLoop.frame_num);
         mPlayer.loc[p].quit_reason = 91;

         // set quit reason for all active clients on server
         for (int pp=1; pp<NUM_PLAYERS; pp++)
            if ((mPlayer.syn[pp].active) && (mPlayer.syn[pp].control_method == 2))
               mPlayer.loc[pp].quit_reason = 91;
         if (mLog.LOG_NET) mLog.log_ending_stats_server();
         mLoop.new_program_state = 1;
      }

      // ------------------------------------
      // remote server quit
      // ------------------------------------
      if ((mNetgame.ima_client) && (p == 0))
      {
         // printf("Remote Server Quit :%d\n", mLoop.frame_num);
         if (val == 64) mPlayer.loc[mPlayer.active_local_player].quit_reason = 92;
         mLoop.new_program_state = 1;
      }

      // ------------------------------------
      // remote player quit (server only)
      // ------------------------------------
      if (mPlayer.syn[p].control_method == 2)
      {
         // printf("Remote Player Quit :%d\n", mLoop.frame_num);
         mPlayer.loc[p].quit_reason = 93;
         if (mLog.LOG_NET) mLog.log_ending_stats(p);
         mPlayer.init_player(p, 1);

         mNetgame.reset_client_state(p);

//         mPlayer.syn[p].active = 0;
//         mPlayer.syn[p].control_method = 9; // prevent re-use of this player number in this level
//         mPlayer.loc[p].who = 99;
      }
      mScreen.set_player_join_quit_display(p, 0, 60);
      mGameEvent.add(81, 0, 0, p, 0, 0, 0);
   }
}







char* cmtos(int cm, char* tmp)
{
   sprintf(tmp, " ");
   if (cm > 31) // fire
   {
      cm -= 32;
      strcat(tmp, "[FIRE]");
   }
   else strcat(tmp, "[    ]");
   if (cm > 15) // jump
   {
      cm -= 16;
      strcat(tmp, "[JUMP]");
   }
   else strcat(tmp, "[    ]");
   if (cm > 7) // down
   {
      cm -= 8;
      strcat(tmp, "[DOWN]");
   }
   else strcat(tmp, "[    ]");
   if (cm > 3) // up
   {
      cm -= 4;
      strcat(tmp, "[UP]");
   }
   else strcat(tmp, "[  ]");
   if (cm > 1) // right
   {
      cm -= 2;
      strcat(tmp, "[RIGHT]");
   }
   else strcat(tmp, "[     ]");
   if (cm > 0) // left
   {
      cm -= 1;
      strcat(tmp, "[LEFT]");
   }
   else strcat(tmp, "[    ]");
   return tmp;
}

void mwGameMovesArray::save_gm_txt(char *sfname)
{
   char tmp[100];
   FILE *filepntr;
   char fname[80];
   sprintf(fname, "savegame/%s.txt", sfname);

   printf("%s - saved\n", fname);


   //replace_extension(fname, fname, "txt", sizeof(fname) );

   filepntr = fopen(fname,"w");

   fprintf(filepntr,"number of entries %d\n", mwGMA.game_move_entry_pos);
   fprintf(filepntr,"deathmatch_shots %d\n", mShot.deathmatch_shots);
   fprintf(filepntr,"deathmatch_shot_damage %d\n", mShot.deathmatch_shot_damage);
   fprintf(filepntr,"suicide_shots %d\n", mShot.suicide_shots);

   fprintf(filepntr,"[ gm][frame][t][p][cm]\n");

   for (int x=0; x<mwGMA.game_move_entry_pos; x++)
   {
      int f = mwGMA.game_moves[x][0]; // frame
      int t = mwGMA.game_moves[x][1]; // type
      int p = mwGMA.game_moves[x][2]; // player
      int v = mwGMA.game_moves[x][3]; // value

      fprintf(filepntr,"[%3d][%5d][%d][%d][%2d]", x, f, t, p, v);

      if (t == 0) fprintf(filepntr,"-------------START (level:%d)------------- ", v);
      if (t == 1) fprintf(filepntr,"-------------PLAYER %d ACTIVE (color:%d)-- ", p, v);
      if (t == 2) fprintf(filepntr,"-------------PLAYER %d INACTIVE------------", p);
      if (t == 3) fprintf(filepntr,"-------------CLIENT %d JOIN!-------------- ", p);
      if (t == 4) fprintf(filepntr,"-------------CLIENT %d QUIT!-------------- ", p);
      if (t == 5) fprintf(filepntr,"%s", cmtos(mwGMA.game_moves[x][3], tmp));
      if (t == 8) fprintf(filepntr,"-------------PLAYER %d ACKNOWLEDGE---------", p);


      fprintf(filepntr,"\n");
   }
   fclose(filepntr);
}

void mwGameMovesArray::save_gm_gm(char *sfname)
{
   FILE *filepntr;
   char fname[80];
   sprintf(fname, "savegame/%s.gm", sfname);
   filepntr = fopen(fname,"w");

   fprintf(filepntr,"%d\n", mwGMA.game_move_entry_pos);  // num_entries

   fprintf(filepntr,"%d\n", mShot.deathmatch_shots);
   fprintf(filepntr,"%d\n", mShot.deathmatch_shot_damage );
   fprintf(filepntr,"%d\n", mShot.suicide_shots);

   for (int x=0; x<mwGMA.game_move_entry_pos; x++)
      for (int y=0; y<4; y++)
         fprintf(filepntr,"%d\n", mwGMA.game_moves[x][y]);
   fclose(filepntr);
}


void mwGameMovesArray::save_gm()
{
   char fname[1024];
   sprintf(fname, "savegame/");
   //printf("fname:%s\n", fname);
   // convert to 'ALLEGRO_FS_ENTRY' (also makes fully qualified path)
   ALLEGRO_FS_ENTRY *FS_fname = al_create_fs_entry(fname);
   sprintf(fname, "%s\\", al_get_fs_entry_name(FS_fname));
   //printf("FS_fname:%s\n", fname);
   ALLEGRO_FILECHOOSER *afc = al_create_native_file_dialog(fname, "Save Demo Filename", "*.gm", ALLEGRO_FILECHOOSER_SAVE);

   if (al_show_native_file_dialog(display, afc))
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
         save_gm_gm(fname);
         save_gm_txt(fname);
      }
   }
   else printf("file select cancelled\n" );
   al_destroy_native_file_dialog(afc);
}



void mwGameMovesArray::blind_save_game_moves(int d)
{
   int do_save = 0;
   if ((d == 1) && (mLog.autosave_game_on_level_done))    do_save = 1;
   if ((d == 2) && (mLog.autosave_game_on_game_exit))     do_save = 1;
   if ((d == 3) && (mLog.autosave_game_on_game_exit))     do_save = 1;
   if (do_save)
   {
      char lev[80];
      sprintf(lev, "-lev%d", mLevel.play_level);
      char filename[80];
      struct tm *timenow;
      time_t now = time(NULL);
      timenow = localtime(&now);
      if (d == 1) strftime(filename, sizeof(filename), "level_done_%Y%m%d-%H%M%S", timenow);
      if (d == 2) strftime(filename, sizeof(filename), "game_exit_%Y%m%d-%H%M%S",  timenow);
      if (d == 3) strftime(filename, sizeof(filename), "bad_exit_%Y%m%d-%H%M%S",   timenow);
      strcat(filename, lev);


      save_gm_gm(filename);  // first save as a playable *.gm
      save_gm_txt(filename); // then as a human readable text file
   }
}

int mwGameMovesArray::load_gm(const char *sfname )
{
   FILE *filepntr;
   char fname[1024];
   sprintf(fname, "%s", sfname);
   //printf("fname:%s\n", fname);

   // convert to 'ALLEGRO_FS_ENTRY' (also makes fully qualified path)
   ALLEGRO_FS_ENTRY *FS_fname = al_create_fs_entry(fname);
   sprintf(fname, "%s", al_get_fs_entry_name(FS_fname));

   //printf("FS_fname:%s\n", fname);

   int bad_filename = 0;
   int user_cancelled = 0;


   // check if the passed filename exists
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

   if (bad_filename)
   {
      sprintf(fname, "savegame/");

      //printf("fname:%s\n", fname);

      // convert to 'ALLEGRO_FS_ENTRY' (also makes fully qualified path)
      ALLEGRO_FS_ENTRY *FS_fname = al_create_fs_entry(fname);
      sprintf(fname, "%s\\", al_get_fs_entry_name(FS_fname));

      //printf("FS_fname:%s\n", fname);

      ALLEGRO_FILECHOOSER *afc = al_create_native_file_dialog(fname, "Run Demo Filename", "*.gm", 0);

      if (al_show_native_file_dialog(display, afc))
      {
         if (al_get_native_file_dialog_count(afc) == 1)
         {
            const char * r = al_get_native_file_dialog_path(afc, 0);
            sprintf(fname, "%s", r);
            //printf("file selected:%s\n", fname);
         }
      }
      else
      {
         user_cancelled = 1;
         //printf("file select cancelled\n" );
      }
      al_destroy_native_file_dialog(afc);
   }

   // check to see if we selected a good filename
   if ((!user_cancelled) && (bad_filename))
   {
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

   // after all that, now do we now have a valid filename?
   if ((!user_cancelled) && (!bad_filename))
   {
      //printf("processing file %s\n", fname);

      mwGMA.initialize();

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
         mwGMA.game_move_entry_pos = atoi(buff);

         loop = 0;
         ch = fgetc(filepntr);
         while((ch != '\n') && (ch != EOF))
         {
            buff[loop] = ch;
            loop++;
            ch = fgetc(filepntr);
         }
         buff[loop] = 0;
         mShot.deathmatch_shots= atoi(buff);

         loop = 0;
         ch = fgetc(filepntr);
         while((ch != '\n') && (ch != EOF))
         {
            buff[loop] = ch;
            loop++;
            ch = fgetc(filepntr);
         }
         buff[loop] = 0;
         mShot.deathmatch_shot_damage= atoi(buff);

         loop = 0;
         ch = fgetc(filepntr);
         while((ch != '\n') && (ch != EOF))
         {
            buff[loop] = ch;
            loop++;
            ch = fgetc(filepntr);
         }
         buff[loop] = 0;
         mShot.suicide_shots = atoi(buff);

         // then get all the entries
         for (int x=0; x<mwGMA.game_move_entry_pos; x++)
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
               mwGMA.game_moves[x][y] = atoi(buff);
            }
         fclose(filepntr);
         mLevel.play_level = mwGMA.game_moves[0][3]; // set play level
         mwDM.demo_mode_last_frame = mwGMA.game_moves[mwGMA.game_move_entry_pos-1][0];
         //printf("dmlf:%d\n", mwDM.demo_mode_last_frame );
         return 1;
      }
   }
   return 0;
}












