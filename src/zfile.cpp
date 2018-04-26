#include "pm.h"

extern FILE *filepntr;

char tmp[100];

char* cmtos(int cm)
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

void save_gm_txt(char *sfname)
{
   FILE *filepntr;
   char fname[80];
   sprintf(fname, "savegame/%s.txt", sfname);

   //replace_extension(fname, fname, "txt", sizeof(fname) );

   filepntr = fopen(fname,"w");

   fprintf(filepntr,"number of entries %d\n", game_move_entry_pos);
   fprintf(filepntr,"deathmatch_pbullets %d\n", deathmatch_pbullets);
   fprintf(filepntr,"deathmatch_pbullets_damage %d\n", deathmatch_pbullets_damage);
   fprintf(filepntr,"suicide_pbullets %d\n", suicide_pbullets);

   fprintf(filepntr,"[ gm][  pc][p][cm]\n");

   for (int x=0; x<game_move_entry_pos; x++)
   {
      fprintf(filepntr,"[%3d][%4d]", x, game_moves[x][0]);
      {
         if (game_moves[x][1] == 0)
            fprintf(filepntr,"-------------START (level:%d)------------- ", game_moves[x][2]);

         if (game_moves[x][1] == 1)
         {
            int val = game_moves[x][3];
            int p = game_moves[x][2];
            if ((val > 0) && (val < 16)) fprintf(filepntr,"-------------PLAYER %d ACTIVE (color:%d)-- ", p, val);
            if (val > 63) fprintf(filepntr,"-------------PLAYER %d INACTIVE----------- ", p);
         }

         if (game_moves[x][1] == 6)
            fprintf(filepntr,"-------------LEVEL DONE!------------------ ");

         if (game_moves[x][1] == 5)
         {
            fprintf(filepntr,"[%d][%2d] ", game_moves[x][2], game_moves[x][3]);
            char *tmp = cmtos(game_moves[x][3]);
            fprintf(filepntr,"%s", tmp);
         }
      }
      fprintf(filepntr,"\n");
   }
   fclose(filepntr);
}

void save_gm_gm(char *sfname)
{
   char fname[80];
   sprintf(fname, "savegame/%s.gm", sfname);
   filepntr = fopen(fname,"w");

   fprintf(filepntr,"%d\n", game_move_entry_pos);  // num_entries

   fprintf(filepntr,"%d\n", deathmatch_pbullets);
   fprintf(filepntr,"%d\n", deathmatch_pbullets_damage );
   fprintf(filepntr,"%d\n", suicide_pbullets);

   for (int x=0; x<game_move_entry_pos; x++)
      for (int y=0; y<4; y++)
         fprintf(filepntr,"%d\n", game_moves[x][y]);
   fclose(filepntr);
}



void save_gm()
{
   char fname[1024];
   sprintf(fname, "savegame/");
   //printf("fname:%s\n", fname);
   // convert to 'ALLEGRO_FS_ENTRY' (also makes fully qualified path)
   ALLEGRO_FS_ENTRY *FS_fname = al_create_fs_entry(fname);
   sprintf(fname, "%s\\", al_get_fs_entry_name(FS_fname));
   //printf("FS_fname:%s\n", fname);
   ALLEGRO_FILECHOOSER *afc = al_create_native_file_dialog(fname, "Save Game Filename", "*.gm", ALLEGRO_FILECHOOSER_SAVE);

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



void blind_save_game_moves(int d)
{
   int do_save = 0;
   if ((d == 1) && (auto_save_game_on_level_done)) do_save = 1;
   if ((d == 2) && (auto_save_game_on_exit)) do_save = 1;
   if ((d == 3) && (auto_save_game_on_exit)) do_save = 1;
   if (do_save)
   {
      char filename1[80];
      char filename2[80];
      struct tm *timenow;
      time_t now = time(NULL);
      timenow = localtime(&now);
      if (d == 1) strftime(filename1, sizeof(filename1), "level_done_%Y%m%d-%H%M%S", timenow);
      if (d == 2) strftime(filename1, sizeof(filename1), "game_exit_%Y%m%d-%H%M%S", timenow);
      if (d == 3) strftime(filename1, sizeof(filename1), "bad_exit_%Y%m%d-%H%M%S", timenow);
      sprintf(filename2, "%s-lev%d", filename1, play_level);
      save_gm_gm(filename2); // first save as a playable *.gm
      save_gm_txt(filename2); // then as a human readable text file
   }
}

int load_gm(char *sfname )
{
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

      ALLEGRO_FILECHOOSER *afc = al_create_native_file_dialog(fname, "Run Game Filename", "*.gm", 0);

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
      //erase old gm
      for (int x=0; x<10000; x++)
         for (int y=0; y<4; y++)
            game_moves[x][y] = 0;
      game_move_entry_pos = 0;

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
         game_move_entry_pos = atoi(buff);

         loop = 0;
         ch = fgetc(filepntr);
         while((ch != '\n') && (ch != EOF))
         {
            buff[loop] = ch;
            loop++;
            ch = fgetc(filepntr);
         }
         buff[loop] = 0;
         deathmatch_pbullets= atoi(buff);

         loop = 0;
         ch = fgetc(filepntr);
         while((ch != '\n') && (ch != EOF))
         {
            buff[loop] = ch;
            loop++;
            ch = fgetc(filepntr);
         }
         buff[loop] = 0;
         deathmatch_pbullets_damage= atoi(buff);

         loop = 0;
         ch = fgetc(filepntr);
         while((ch != '\n') && (ch != EOF))
         {
            buff[loop] = ch;
            loop++;
            ch = fgetc(filepntr);
         }
         buff[loop] = 0;
        suicide_pbullets = atoi(buff);


         // then get all the entries
         for (int x=0; x<game_move_entry_pos; x++)
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
               game_moves[x][y] = atoi(buff);
            }
         fclose(filepntr);

         // set play level
         for (int x=0; x<game_move_entry_pos; x++)
            if (game_moves[x][0] == 0) // passcount 0
               if (game_moves[x][1] == 0) // type 0
               {
                  play_level = game_moves[x][2];
                  break;
               }
         // init all
         for (int p=0; p<NUM_PLAYERS; p++) init_player(p, 1);

         // set all but 0 to inactive
         for (int p=1; p<NUM_PLAYERS; p++) players[p].active = 0;
         return 1;

      }
   }
   return 0;
}







