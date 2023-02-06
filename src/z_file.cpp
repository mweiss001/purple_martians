// z_file.cpp
#include "pm.h"
#include "z_log.h"
#include "e_mWindow.h"
#include "z_player.h"
#include "n_netgame.h"


FILE *filepntr;

void make_filename(int x)
{
   char temp[255];
   strcpy( temp, "levels/level");

   if (x < 10) sprintf(msg,"00%-1d", x);
   if ((x  > 9) && (x  < 100)) sprintf(msg,"0%-2d", x);
   if (x > 99) sprintf(msg,"%-3d", x);

   strcat( temp, msg);
   strcat( temp, ".pml");
   strcpy(level_filename, temp);

}
int load_level_prompt()
{
   sprintf(level_filename,"levels\\");
   if (mw_file_select("Load Selection", level_filename, ".pml", 0))
   {
      int len = strlen(level_filename);
      char g[10];
      g[0] = level_filename[len-7];
      g[1] = level_filename[len-6];
      g[2] = level_filename[len-5];
      g[3] = 0;
      int num = atoi(g);
      load_level(num,0);
   }
   return 0;
}

int save_level_prompt()
{
   char title[80];
   int len = strlen(level_filename);
   char g[10];
   g[0] = level_filename[len-7];
   g[1] = level_filename[len-6];
   g[2] = level_filename[len-5];
   g[3] = 0;
   int num = atoi(g);
   sprintf(level_filename,"levels\\level%03d.pml", num);
   sprintf(title,"Save Level %d ",num);
   if (mw_file_select(title, level_filename, ".pml", 1))
   {

      len = strlen(level_filename);
      g[0] = level_filename[len-7];
      g[1] = level_filename[len-6];
      g[2] = level_filename[len-5];
      g[3] = 0;
      num = atoi(g);
      save_level(num);
      last_level_loaded = num;
      return 1;
   }
   else return 0; // user pressed cancel
}



void save_sprit(void)
{
   //printf("saving sprit001.pm\n");

   for (int c=0; c<NUM_ANS; c++) // set all animation initial
      if (zz[4][c])
      {
         zz[0][c]=zz[5][c];
         zz[1][c]=0;
         zz[2][c]=0;
      }

   // ensure sa[][0] does not have any bits set other than the ones we want
   for (int c=0; c<NUM_SPRITES; c++)
   {
      sa[c][0] &= PM_BTILE_ALL_FLAGS;
      sa[c][1] = 0; // not used
   }

   FILE *fp = fopen("bitmaps/sprit001.pm", "wb");
   fwrite(zz, sizeof(zz), 1, fp);
   fwrite(sa, sizeof(sa), 1, fp);
   fclose(fp);
}

void load_sprit(void)
{
   FILE *fp = fopen("bitmaps/sprit001.pm", "rb");
   fread(zz, sizeof(zz), 1, fp);
   fread(sa, sizeof(sa), 1, fp);
   fclose(fp);
}


void save_mW(void)
{
   FILE *fp = fopen("bitmaps/mW.pm", "wb");
   if (fp)
   {
      fwrite(mW, sizeof(mW), 1, fp);
      fclose(fp);
   }
   else printf("error saving mW.pm\n");
}


void load_mW(void)
{
   FILE *fp = fopen("bitmaps/mW.pm", "rb");
   if (fp)
   {
      fread(mW, sizeof(mW), 1, fp);
      fclose(fp);
   }
   else
   {
      printf("error loading mW.pm -- recreating\n");
      save_mW();
   }

}

int load_tiles(void)
{
   int load_error = 0;

   // get main tiles
   tilemap = al_load_bitmap("bitmaps/tiles.bmp");
   if (!tilemap)
   {
      m_err("Can't load tiles from bitmaps/tiles.bmp");
      load_error = 1;
   }
   else
   {
      //printf("load good\n");
      al_convert_mask_to_alpha(tilemap, al_map_rgb(0, 0, 0)) ;
      al_set_target_bitmap(M_tilemap);
      al_draw_bitmap(tilemap, 0, 0, 0);
      for (int y=0; y<32; y++)
         for (int x=0; x<32; x++)
            tile[y*32 + x] = al_create_sub_bitmap(tilemap, x*20, y*20, 20, 20);
   }



   // get block tiles
   btilemap = al_load_bitmap("bitmaps/block_tiles.bmp");
   if (!btilemap)
   {
      m_err("Can't load tiles from bitmaps/block_tiles.bmp");
      load_error = 1;
   }
   else
   {
      //printf("load good\n");
      al_convert_mask_to_alpha(btilemap, al_map_rgb(0, 0, 0)) ;
      al_set_target_bitmap(M_btilemap);
      al_draw_bitmap(btilemap, 0, 0, 0);
      for (int y=0; y<32; y++)
         for (int x=0; x<32; x++)
            btile[y*32 + x] = al_create_sub_bitmap(btilemap, x*20, y*20, 20, 20);
   }


   //fill_player_tile();


   // get player tiles
   ptilemap = al_load_bitmap("bitmaps/player_tiles.bmp");
   if (!ptilemap)
   {
      m_err("Can't load tiles from bitmaps/player_tiles.bmp");
      load_error = 1;
   }
   else
   {
      //printf("load good\n");
      al_convert_mask_to_alpha(ptilemap, al_map_rgb(0, 0, 0)) ;
      al_set_target_bitmap(M_ptilemap);
      al_draw_bitmap(ptilemap, 0, 0, 0);
      for (int a=0; a<16; a++)
         for (int b=0; b<24; b++)
            player_tile[a][b] = al_create_sub_bitmap(ptilemap, b*20, a*20, 20, 20);

   }

   // get door tiles
   dtilemap = al_load_bitmap("bitmaps/door_tiles.bmp");
   if (!dtilemap)
   {
      m_err("Can't load tiles from bitmaps/door_tiles.bmp");
      load_error = 1;
   }
   else
   {
      //printf("load good\n");
      al_convert_mask_to_alpha(dtilemap, al_map_rgb(0, 0, 0)) ;
      al_set_target_bitmap(M_dtilemap);
      al_draw_bitmap(dtilemap, 0, 0, 0);
      for (int a=0; a<16; a++)
         for (int b=0; b<8; b++)
         {
            door_tile[0][a][b] = al_create_sub_bitmap(dtilemap, b*20,     a*20, 20, 20);
            door_tile[1][a][b] = al_create_sub_bitmap(dtilemap, b*20, 320+a*20, 20, 20);
         }
   }

   load_sprit(); // get animation sequences and shape attributes

   if (load_error) return 0;
   else return 1;
}


void zero_level_data(void)
{
   for (int c=0; c<100; c++)    // blocks
      for (int x=0; x<100; x++)
           l[c][x] = 0;

   for (int c=0; c<500; c++)  // items
   {
      for (int x=0; x<500; x++) pmsgtext[c][x] = 0;
      for (int x=0; x<16; x++) item[c][x] = 0;
      for (int x=0; x<4; x++) itemf[c][x] = al_itofix(0);
   }
   sort_item(1);

   for (int c=0; c<100; c++)
   {
      for (int x=0; x<16; x++)  Efi[c][x] = al_itofix(0); // enemy al_fixed
      for (int x=0; x<32; x++)  Ei[c][x] = 0;          // enemy ints
   }
   sort_enemy();

   for (int c=0; c<40; c++) // lifts
   {
      clear_lift(c);
      for (int x=0; x<40; x++)
         clear_lift_step(c,x);
   }
   num_lifts = 0;
}

void level_check(void)
{
   // set number of purple coins
   number_of_purple_coins = 0;
   for (int c=0; c < 500; c++)
      if ((item[c][0] == 2) && (item[c][6] == 3)) number_of_purple_coins++;

/*
   int error = 0;

   // count number of starts
   number_of_starts = 0;
   int s[8] = {0};


   for (int i=0; i<500; i++)
      if (item[i][0] == 5)
      {
         int x = item[i][7]; // start index
         if (x < 0) m_err("Level has a start index less than 0");
         if (x > 7) m_err("Level has a start index more than 7");
         if ((x >=0) && (x<8))
         {
            number_of_starts++;
            s[x]++; // save index of this start
         }
      }

   if (number_of_starts == 0)
   {
      m_err("Level has no start block");
      error = 1;
   }

   if (number_of_starts > 8 )
   {
      m_err("Level has more than 8 start blocks");
      error = 1;
   }
   for (int i=0; i<8; i++)
      if (s[i] > 1)
      {
         sprintf(msg, "Level has duplicate start blocks with index %d", i);
         m_err(msg);
         error = 1;
      }

   if (number_of_starts > 1)
      for (int i=1; i<number_of_starts; i++)
         if (s[i] == 0)
         {
            sprintf(msg, "Level has %d starts but no start with index %d", number_of_starts, i);
            m_err(msg);
            error = 1;
         }
   if (error)
   {
      printf("number_of_starts:%d\n", number_of_starts);
      for (int i=0; i<8; i++)
         printf("%d %d\n", i, s[i]);
   }

   */

}




int load_level(int level_to_load, int load_only)
{
   //printf("loading: %s\n", level_filename);

   for (int i=0; i<20; i++) level_header[i] = 0;

   int level_load_error = 0;

   last_level_loaded = level_to_load;
   valid_level_loaded = 0;
   resume_allowed = 0;

   zero_level_data();

   make_filename(level_to_load);   // update filename
   FILE *fp = fopen(level_filename,"rb");
   if (!fp)
   {
      //sprintf(msg, "Error opening %s ", level_filename);
      //m_err(msg);
      level_load_error = 1;
   }
   if (!level_load_error)  // file open !
   {
      // read the compressed data
      char cmp[PML_SIZE];
      fread(cmp, sizeof(cmp), 1, fp);
      fclose(fp);

      // decompress cmp to pml
      char pml[PML_SIZE];
      uLongf destLen = sizeof(pml);
      uncompress((Bytef*)pml, (uLongf*)&destLen, (Bytef*)cmp, sizeof(cmp));

      pml_to_var(pml);
   } // end of file open

   if (level_load_error)
   {
      sprintf(msg, "Level loading error occurred for: %s ", level_filename);
      printf("%s\n", msg);
      m_err(msg);
      num_lifts = 0;
      return 0;
   }
   else
   {
      num_lifts = level_header[5];
      if (!load_only)
      {
         valid_level_loaded = 1;
         lift_setup();
         for (int x=0; x<500; x++)
            if (item[x][0]) // only if active set x y
            {
               itemf[x][0] = al_itofix(item[x][4]);
               itemf[x][1] = al_itofix(item[x][5]);
            }
         level_check();
         init_level_background(0); // draw blocks on level_background
         reset_animation_sequences();
         //set_player_start_pos(0, 0);
      }
      return 1;
   }
}

int save_level(int level_to_save)
{
   level_check();

   for (int i=0; i<20; i++) level_header[i] = 0;

   level_header[0] = 5; // .pml level version
   level_header[3] = sort_item(1); // num_of_items
   sort_enemy();
   level_header[4] = num_enemy;  // num_of_enemies
   lift_setup();
   level_header[5] = num_lifts;  // num of lifts

   make_filename(level_to_save);   // update filename

   // put variables in pml
   char pml[PML_SIZE];
   var_to_pml(pml);

   // compress pml to cmp
   char cmp[PML_SIZE];
   uLongf destLen= sizeof(cmp);
   compress2((Bytef*)cmp, (uLongf*)&destLen, (Bytef*)pml, sizeof(pml), -1);
   int cmp_size = destLen;

   // write cmp to file
   FILE *fp = fopen(level_filename,"wb");
   fwrite(cmp, cmp_size, 1, fp);
   fclose(fp);
   return 0;
}


int mw_file_select(const char * title, char * fn, const char * ext, int save)
{
   // no matter what I do I cannot start the native file chooser with the file I pass to it selected
   // i have tried everything I can think of
   // the filename and path is absolute, but still shows up blank

   int mode = 0; // default
   if (save) mode = ALLEGRO_FILECHOOSER_SAVE;

   char wext[100];
   sprintf(wext, "*%s", ext);


   //printf("fn:%s\n", fn);
   // convert to 'ALLEGRO_FS_ENTRY' (also makes fully qualified path)
   ALLEGRO_FS_ENTRY *FS_fname = al_create_fs_entry(fn);
   sprintf(fn, "%s", al_get_fs_entry_name(FS_fname));
   //printf("FS_fn:'%s'  title:'%s' ext:'%s'  mode:%d\n", fn, title, wext, mode);
   ALLEGRO_FILECHOOSER *afc = al_create_native_file_dialog(fn, title, wext, mode);
   if (afc==NULL) printf("failed to create native filechooser dialog\n");

   if (al_show_native_file_dialog(display, afc))
   {
      if (al_get_native_file_dialog_count(afc) == 1)
      {
         const char * r = al_get_native_file_dialog_path(afc, 0);
         sprintf(fn, "%s", r);

         //enforce extension
         if (save)
         {
            printf("save file name and path is:%s\n", fn);
            ALLEGRO_PATH *tp = al_create_path(fn);
            const char * pfn = al_get_path_filename(tp);
            if (pfn == NULL)
            {
               printf("filename is NULL??\n");
               al_destroy_native_file_dialog(afc);
               return 0;
            }
            printf("save file name is:%s\n", pfn);


            const char * pfe = al_get_path_extension(tp);
            if (pfe == NULL)
            {
               printf("extension is NULL\n");
               al_set_path_extension(tp, ext);
            }

            if (strcmp(ext, pfe) != 0)
            {
               printf("extensions do not match [%s] [%s] \n", pfe, ext);
               al_set_path_extension(tp, ext);
            }

            printf("extension is:%s\n", pfe);
            sprintf(fn, "%s", al_path_cstr(tp, ALLEGRO_NATIVE_PATH_SEP));
            // printf("final path and filename is:%s\n", fn);

         }

         al_destroy_native_file_dialog(afc);
         return 1;
      }
   }
   //else printf("file select cancelled\n" );
   al_destroy_native_file_dialog(afc);
   return 0;
}





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

   printf("%s - saved\n", fname);


   //replace_extension(fname, fname, "txt", sizeof(fname) );

   filepntr = fopen(fname,"w");

   fprintf(filepntr,"number of entries %d\n", game_move_entry_pos);
   fprintf(filepntr,"deathmatch_pbullets %d\n", deathmatch_pbullets);
   fprintf(filepntr,"deathmatch_pbullets_damage %d\n", deathmatch_pbullets_damage);
   fprintf(filepntr,"suicide_pbullets %d\n", suicide_pbullets);

   fprintf(filepntr,"[ gm][frame][t][p][cm]\n");

   for (int x=0; x<game_move_entry_pos; x++)
   {
      int f = game_moves[x][0]; // frame
      int t = game_moves[x][1]; // type
      int p = game_moves[x][2]; // player
      int v = game_moves[x][3]; // value

      fprintf(filepntr,"[%3d][%5d][%d][%d][%2d]", x, f, t, p, v);

      if (t == 0) fprintf(filepntr,"-------------START (level:%d)------------- ", v);
      if (t == 1) fprintf(filepntr,"-------------PLAYER %d ACTIVE (color:%d)-- ", p, v);
      if (t == 2) fprintf(filepntr,"-------------PLAYER %d INACTIVE------------", p);
      if (t == 3) fprintf(filepntr,"-------------CLIENT %d JOIN!-------------- ", p);
      if (t == 4) fprintf(filepntr,"-------------CLIENT %d QUIT!-------------- ", p);
      if (t == 5) fprintf(filepntr,"%s", cmtos(game_moves[x][3]));
      if (t == 8) fprintf(filepntr,"-------------PLAYER %d ACKNOWLEDGE---------", p);


      fprintf(filepntr,"\n");
   }
   fclose(filepntr);
}

void save_gm_gm(char *sfname)
{
   FILE *filepntr;
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



void blind_save_game_moves(int d)
{
   int do_save = 0;
   if ((d == 1) && (autosave_game_on_level_done))    do_save = 1;
   if ((d == 2) && (autosave_game_on_game_exit))     do_save = 1;
   if ((d == 3) && (autosave_game_on_game_exit))     do_save = 1;
   if (do_save)
   {
      char lev[80];
      sprintf(lev, "-lev%d", play_level);
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

int load_gm(const char *sfname )
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

      clear_game_moves();

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
         play_level = game_moves[0][3]; // set play level
         demo_mode_last_frame = game_moves[game_move_entry_pos-1][0];
         //printf("dmlf:%d\n", demo_mode_last_frame );
         return 1;
      }
   }
   return 0;
}
