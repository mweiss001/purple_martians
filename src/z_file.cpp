// zfile.cpp
#include "pm.h"

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
      load_level(num+1000,0);
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
      save_level(num+1000);
      return 1;
   }
   else return 0; // user pressed cancel
}

int save_tiles(void)
{
   ALLEGRO_BITMAP* temp = al_create_bitmap(640, 640);
   al_set_target_bitmap(temp);
   for (int y = 0; y < 32; y++)
      for (int x = 0; x < 32; x++)
         al_draw_bitmap(tile[y*32 + x], (x*20), (y*20), 0);

   al_save_bitmap("bitmaps/tiles.bmp", temp);
   al_destroy_bitmap(temp);

   char sprit_filename[20] = "bitmaps/sprit001.pm";

   for (int c=0; c<NUM_ANS; c++)    // set all to initial
      if (zz[4][c] != 0)
      {
         zz[0][c]=zz[5][c];
         zz[1][c]=0;
         zz[2][c]=0;
      }
   filepntr = fopen( sprit_filename,"wb");
   for (int c=0; c<NUM_ANS; c++) // put animation sequences
      for (int y=0; y<20; y++)
      {
         int ho = (zz[y][c] / 256);
         int lo = zz[y][c] - (ho*256);
         fputc(ho, filepntr);
         fputc(lo, filepntr);
      }
   for (int c=0; c<NUM_SPRITES; c++)  // shape attributes sa[512][2]
      for (int y=0; y<2; y++)
         fputc(sa[c][y], filepntr);

   fclose(filepntr);
   return 0;
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
         for (int b=0; b<19; b++)
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


   // get animation sequences and shape attributes
   char sprit_filename[20] = "bitmaps/sprit001.pm";
//   if (exists(sprit_filename) == 0) // does file exist?
//   if (0)
//   {
//      sprintf(msg, "Can't find sprit %s ", sprit_filename);
//      //textout_ex(screen, font, msg, SCREEN_W/2, ((SCREEN_H*3)/4)+10, palette_color[1], 0);
//      //m_err(msg);
//      load_error = 1;
//   }

   if (!load_error)  // open file
      if ((filepntr=fopen(sprit_filename,"rb")) == 0)
      {
         //printf("Error opening %s ", sprit_filename);
         sprintf(msg, "Error opening %s ", sprit_filename);
         m_err(msg);
         load_error = 1;
      }
   if (!load_error)  // file open !
   {
      // get animation seq
      for (int c=0; c<NUM_ANS; c++)
         for (int y=0; y<20; y++)
         {
            int ho = fgetc(filepntr);
            int lo = fgetc(filepntr);
            zz[y][c] = ho*256 + lo;
         }
      // get shape attributes
      for (int c=0; c<NUM_SPRITES; c++)
         for (int y=0; y<2; y++)
            sa[c][y] = fgetc(filepntr);
   } // end of if not sprit load error
   fclose(filepntr);
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
      if (item[c][0] == 10)
      {
         free (pmsg[c]);
         pmsg[c] = NULL;
      }
      for (int x=0; x<16; x++) item[c][x] = 0;
      for (int x=0; x<4; x++) itemf[c][x] = al_itofix(0);
   }
   sort_item();

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


int load_level(int level_to_load, int display)
{
   int level_header[20];

   display = 1; // force display on or off
   int error_logging = 1;
   valid_level_loaded = 0;
   resume_allowed = 0;
   int level_load_error = 0;
   int loop, ch, c, x, y;
   char buff[2000];

   //printf("load level\n");

   zero_level_data();

   while (level_to_load > 1000) level_to_load -= 1000;

   make_filename(level_to_load);   // update filename
   level_num = level_to_load;
   level_load_error = 0;

   if (!al_filename_exists(level_filename))
   {
      sprintf(msg, "Can't Find Level %s ", level_filename);
      //m_err(msg);
      level_load_error = 1;
   }

   if (!level_load_error) // file exists
   {
      if (error_logging) printf("file exists\n");
      if ((filepntr=fopen(level_filename,"r")) == 0)
      {
         sprintf(msg, "Error opening %s ", level_filename);
         m_err(msg);
         level_load_error = 1;
      }
   }

  if (!level_load_error)  // file open !
  {
     if (error_logging) printf("reading level header\n");
     for (c=0; c<20; c++) // level header
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
        level_header[c] = atoi(buff);
        if (ch == EOF)
        {
           sprintf(msg, "Error reading level header in %s ", level_filename);
           m_err(msg);
           level_load_error = 1;
        }
     }
     num_lifts = level_header[5];

     if (error_logging) printf("%d items\n", level_header[3]);
     if (error_logging) printf("%d enemies\n", level_header[4]);
     if (error_logging) printf("%d lifts \n", level_header[5]);
     if (error_logging) printf("reading blocks\n");
     for (c=0; c<100; c++)  // l[100][100]
        for (y=0; y<100; y++)
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
           l[c][y] = atoi(buff);
           if (ch == EOF)
           {
              sprintf(msg, "Error reading blocks in %s ", level_filename);
              m_err(msg);
              level_load_error = 1;
           }
        }
     if (!level_load_error)
     {
        if (error_logging) printf("reading items\n");
        for (c = 0; c < level_header[3]; c++)  // read item
        {
           for (x = 0; x < 16; x++)
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
              item[c][x] = atoi(buff);

              if (ch == EOF)
              {
                 sprintf(msg, "Error reading items in %s ", level_filename);
                 m_err(msg);
                 level_load_error = 1;
              }
           }
           if (item[c][0] == 10) // get pmsg
           {
              loop = 0;
              ch = fgetc(filepntr);
              while((ch != '\n') && (ch != EOF))
              {
                 if (ch != 13) // ignore and don't add if 13 ( needed for linux)
                 {
                    buff[loop] = ch;
                    loop++;
                 }
                 ch = fgetc(filepntr);
              }
              buff[loop] = 0;
              pmsg[c] = (char*) malloc (strlen(buff)+1);
              strcpy(pmsg[c], buff);
           }
        }
        for (int x=0; x<500; x++)
           if (item[x][0]) // only if active set x y
           {
              itemf[x][0] = al_itofix(item[x][4]);
              itemf[x][1] = al_itofix(item[x][5]);
           }
     }
     if (!level_load_error)
     {
        if (error_logging) printf("reading enemy fixeds \n");
        for (c=0; c<level_header[4]; c++) // read enemy fixeds
           for (x=0; x<16; x++)
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
              Efi[c][x] = atoi(buff);  // enemy al_fixed

              if (ch == EOF)
              {
                 sprintf(msg, "Error reading Ef in %s ", level_filename);
                 m_err(msg);
                 level_load_error = 1;
              }
           }
        if (error_logging) printf("reading enemy ints\n");
        for (c=0; c < level_header[4]; c++)  // enemy ints
           for (x=0; x<32; x++)
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
              Ei[c][x] = atoi(buff);
              if (ch == EOF)
              {
                 sprintf(msg, "Error reading Ei in %s ", level_filename);
                 m_err(msg);
                 level_load_error = 1;
              }
           }
     }
     if (!level_load_error)
     {
        if (error_logging) printf("reading %d lifts\n", num_lifts);
        for (c=0; c<num_lifts; c++) // read lifts
        {
           if (error_logging) printf("--------------\nreading lifts %d\n", c);
           int tr[5];
           char tmsg[80];
           for (x=0; x<5; x++) // lift data
           {

              loop = 0;
              ch = fgetc(filepntr);
              while((ch != '\n') && (ch != EOF))
              {
                 if (ch != 13)
                 {
                    buff[loop] = ch;
                    loop++;
                 }
                 ch = fgetc(filepntr);
              }
              buff[loop] = 0;
              if (x == 0) strcpy(tmsg, buff); // get lift name
              else tr[x] = atoi(buff); // get int
           }
           if (error_logging) printf("name(%s) w:%d h:%d c:%d num_step[%d]\n", tmsg, tr[1],tr[2],tr[3],tr[4]);
           construct_lift(c, tmsg, tr[1],tr[2],tr[3],tr[4]);

           for (x=0; x<lifts[c] . num_steps; x++) // step data
           {
              if (error_logging) printf("   reading lift step %d  ", x);

              int tr[4];
              for (y=0; y<4; y++) // read 4
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
                 tr[y] = atoi(buff);
              }
              if (error_logging) printf("d1:%4d d2:%4d d3:%4d d4:%4d\n", tr[0],tr[1],tr[2],tr[3]);
              construct_lift_step(c, x, tr[0], tr[1], tr[2], tr[3]);

           }
           set_lift(c, 0);
        }
     }
     fclose(filepntr);
   } // end of file open


   if (level_load_error)
   {
      if (error_logging) printf("level loading error occurred\n");
      num_lifts = 0;
      return 0;
   }
   else
   {
      if (error_logging) printf("level loading complete with no errors\n");
      valid_level_loaded = 1;
      init_level_background(); // draw blocks and lift lines on level_background
      if (error_logging) printf("blocks drawn\n");
      reset_animation_sequence_frame_nums(0);
      if (error_logging) printf("ans seq reset\n");
      for (int p=0; p<NUM_PLAYERS; p++) set_player_start_pos(p);
      if (error_logging) printf("got start\n");
      return 1;
   }
}




void level_check(void)
{
   // count number of starts
   int ns = 0;
   int s[8] = {0};

   for (int i=0; i<500; i++)
      if (item[i][0] == 5)
      {
         ns++;
         s[item[i][7]] = i; // save index of this start
      }


   if (ns == 0) m_err("Level has no start block");
   if (ns > 8 ) m_err("Level has more than 8 start blocks");


}








int save_level(int level_to_save)
{

   level_check();





   int level_header[20];
   int c, x, y;

   while (level_to_save > 1000) level_to_save -= 1000;
   make_filename(level_to_save);   // update filename

   filepntr = fopen(level_filename,"w");

   level_num = level_to_save;

   level_header[3] = sort_item(); // num_of_items
   sort_enemy();
   level_header[4] = num_enemy; // num_of_enemies
   level_header[5] = num_lifts;

   for (x=0; x<20; x++)
      fprintf(filepntr,"%d\n",level_header[x]);

   for (c=0; c<100; c++)  // level
      for (x=0; x<100; x++)
         fprintf(filepntr,"%d\n",l[c][x]);

   for (c=0; c < level_header[3]; c++) // item
   {
      for (x=0; x<16; x++)
         fprintf(filepntr,"%d\n",item[c][x]);

      if (item[c][0] == 10) // pmsg
      {
         y = 0;
         while (pmsg[c][y] != 0)
         {
            if (pmsg[c][y] == 13) fprintf(filepntr,"%c", 126);
            else fprintf(filepntr,"%c", pmsg[c][y]);
            y++ ;
         }
         fprintf(filepntr,"\n");
      }
   }
   for (c=0; c < level_header[4]; c++)  // enemy float
      for (x=0; x<16; x++)
         fprintf(filepntr,"%d\n", Efi[c][x]); // enemy fixed

   for (c=0; c < level_header[4]; c++) // enemy int
      for (x=0; x<32; x++)
         fprintf(filepntr,"%d\n",Ei[c][x]);

   for (c=0; c < level_header[5]; c++)   // lifts
   {
      fprintf(filepntr,"%s\n",lifts[c].lift_name);
      fprintf(filepntr,"%d\n",lifts[c].width);
      fprintf(filepntr,"%d\n",lifts[c].height);
      fprintf(filepntr,"%d\n",lifts[c].color);
      fprintf(filepntr,"%d\n",lifts[c].num_steps);

      for (x=0; x<lifts[c].num_steps; x++)  // steps
      {
         fprintf(filepntr,"%d\n",lift_steps[c][x].x);
         fprintf(filepntr,"%d\n",lift_steps[c][x].y);
         fprintf(filepntr,"%d\n",lift_steps[c][x].val);
         fprintf(filepntr,"%d\n",lift_steps[c][x].type);
      }
   }
   fclose(filepntr);
   return 0;
}




int mw_file_select(const char * title, char * fn, const char * ext, int save)
{
   int mode = 0; // default
   if (save) mode = ALLEGRO_FILECHOOSER_SAVE;

   char wext[100];
   sprintf(wext, "*%s", ext);

   //printf("fn:%s\n", fn);
   // convert to 'ALLEGRO_FS_ENTRY' (also makes fully qualified path)
   ALLEGRO_FS_ENTRY *FS_fname = al_create_fs_entry(fn);
   sprintf(fn, "%s", al_get_fs_entry_name(FS_fname));
   //printf("FS_fn:%s\n", fn);
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
            //printf("save file name and path is:%s\n", fn);
            ALLEGRO_PATH *tp = al_create_path(fn);
            const char * pfn = al_get_path_filename(tp);
            if (pfn == NULL)
            {
               printf("filename is NULL??\n");
               al_destroy_native_file_dialog(afc);
               return 0;
            }
            //printf("save file name is:%s\n", pfn);


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

            //printf("extension is:%s\n", pfe);
            sprintf(fn, "%s", al_path_cstr(tp, ALLEGRO_NATIVE_PATH_SEP));
            //printf("final path and filename is:%s\n", fn);

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
         play_level = game_moves[0][2]; // set play level
         return 1;
      }
   }
   return 0;
}







