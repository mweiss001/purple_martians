// zlog.cpp

#include "pm.h"


void log_bandwidth_stats(int p)
{
   sprintf(msg,"total tx bytes............[%d]", players1[p].tx_total_bytes);
   add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

   sprintf(msg,"max tx bytes per frame....[%d]", players1[p].tx_max_bytes_per_frame);
   add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

   sprintf(msg,"avg tx bytes per frame....[%d]", players1[p].tx_total_bytes / frame_num);
   add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

   sprintf(msg,"max rx bytes per second...[%d]", players1[p].tx_max_bytes_per_tally);
   add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

   sprintf(msg,"avg tx bytes per sec......[%d]", (players1[p].tx_total_bytes *40)/ frame_num);
   add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

   sprintf(msg,"total tx packets..........[%d]", players1[p].tx_total_packets);
   add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

   sprintf(msg,"max tx packets per frame..[%d]", players1[p].tx_max_packets_per_frame);
   add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

   sprintf(msg,"max tx packets per second.[%d]", players1[p].tx_max_packets_per_tally);
   add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");


   sprintf(msg,"total rx bytes............[%d]", players1[p].rx_total_bytes);
   add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

   sprintf(msg,"max rx bytes per frame....[%d]", players1[p].rx_max_bytes_per_frame);
   add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

   sprintf(msg,"avg rx bytes per frame....[%d]", players1[p].rx_total_bytes / frame_num);
   add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

   sprintf(msg,"max rx bytes per second...[%d]", players1[p].rx_max_bytes_per_tally);
   add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

   sprintf(msg,"avg rx bytes per sec......[%d]", (players1[p].rx_total_bytes *40)/ frame_num);
   add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

   sprintf(msg,"total rx packets..........[%d]", players1[p].rx_total_packets);
   add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

   sprintf(msg,"max rx packets per frame..[%d]", players1[p].rx_max_packets_per_frame);
   add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

   sprintf(msg,"max rx packets per second.[%d]", players1[p].rx_max_packets_per_tally);
   add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");
}

void log_reason_for_player_quit(int p)
{
   char tmsg[80];
   sprintf(tmsg,"unknown");
   int r = players1[p].quit_reason;
   if (r == 64) sprintf(tmsg,"player pressed ESC or menu key");
   if (r == 70) sprintf(tmsg,"server drop (server sync > 100)");
   if (r == 71) sprintf(tmsg,"server drop (no sdak for 100 frames)");
   if (r == 74) sprintf(tmsg,"client never became active");
   if (r == 75) sprintf(tmsg,"client lost server connection");
   if (r == 80) sprintf(tmsg,"level done");
   if (r == 90) sprintf(tmsg,"local client quit");
   if (r == 91) sprintf(tmsg,"local server quit");
   if (r == 92) sprintf(tmsg,"remote server quit");
   if (r == 93) sprintf(tmsg,"remote client quit");

   sprintf(msg,"reason for quit...........[%s]", tmsg);
   add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");
}





void log_time_date_stamp(void)
{
   char tmsg[80];
   struct tm *timenow;
   time_t now = time(NULL);
   timenow = localtime(&now);
   strftime(tmsg, sizeof(tmsg), "%Y-%m-%d  %H:%M:%S", timenow);
   sprintf(msg, "Date and time: %s",tmsg);
   printf("%s\n", msg);
   add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");
}


void log_versions(void)
{
   add_log_entry_centered_text(10, 0, 76, "", "+", "-");
   sprintf(msg, "Purple Martians Version %s", pm_version_string);
   add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");
   add_log_entry_position_text(10, 0, 76, 10, al_version_string, "|", " ");
   log_time_date_stamp();
   add_log_entry_centered_text(10, 0, 76, "", "+", "-");
}

void log_player_array(void)
{
   add_log_entry_header(22, 0,  "Player Array", 0);
   add_log_entry_position_text(22, 0, 76, 10, "[p][wh][a][co][m]", "|", " ");
   for (int p=0; p<NUM_PLAYERS; p++)
   {
      char ms[80];
      sprintf(ms, " ");

      if ((players[p].active) && (players[p].control_method == 2))
         sprintf(ms, " <-- active client");

      if ((!players[p].active) && (players[p].control_method == 2))
         sprintf(ms, " <-- syncing client");

      if (players[p].control_method == 9) sprintf(ms, " <-- used client");

      if (p == active_local_player) sprintf(ms, " <-- active local player (me!)");
      if (p == 0) sprintf(ms, " <-- server");

      sprintf(msg, "[%d][%2d][%d][%2d][%d] - %s %s",
                                              p,
                                              players1[p].who,
                                              players[p].active,
                                              players[p].color,
                                              players[p].control_method,
                                              players1[p].hostname,
                                              ms );
      add_log_entry_position_text(22, 0, 76, 10, msg, "|", " ");
   }
   add_log_entry_centered_text(22, 0, 76, "", "+", "-");
}

void log_player_array2(void)
{
   sprintf(msg, "[p][a][m][sy] - level_done_mode:%d", players[0].level_done_mode);
   add_log_entry_position_text(26, 0, 76, 10, msg, "|", " ");
   for (int p=0; p<NUM_PLAYERS; p++)
   {
      int sy = players1[p].server_sync;
      if (p == 0) sy = 0;
      sprintf(msg, "[%d][%d][%d][%2d]",   p, players[p].active, players[p].control_method, sy );
      add_log_entry_position_text(26, 0, 76, 10, msg, "|", " ");
   }
}


void log_ending_stats(int p)
{
   sprintf(msg,"Client %d (%s) ending stats", p, players1[p].hostname);
   add_log_entry_header(22, p, msg, 0);

   sprintf(msg,"total game frames.........[%d]", frame_num);
   add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");
   sprintf(msg,"frame when client joined..[%d]", players1[p].join_frame);
   add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

   if (players1[p].quit_frame == 0) players1[p].quit_frame = frame_num;
   sprintf(msg,"frame when client quit....[%d]", players1[p].quit_frame);
   add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

   log_reason_for_player_quit(p);

   sprintf(msg,"frames client was active..[%d]", players1[p].quit_frame - players1[p].join_frame);
   add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

   sprintf(msg,"frames skipped............[%d]", players1[p].frames_skipped);
   add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

   sprintf(msg,"cdat packets total........[%d]", players1[p].client_cdat_packets_tx);
   add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

   sprintf(msg,"cdat packets late.........[%d]", players1[p].late_cdats);
   add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

   log_bandwidth_stats(p);
   add_log_entry_centered_text(22, p, 76, "", "+", "-");
}

void log_ending_stats_server()
{
   sprintf(msg,"Server (%s) ending stats", local_hostname);
   add_log_entry_header(22, 0, msg, 0);

   add_log_entry_centered_text(22, 0, 76, "", "+", "-");

   sprintf(msg,"level.....................[%d]", play_level);
   add_log_entry_position_text(22, 0, 76, 10, msg, "|", " ");

   sprintf(msg,"total frames..............[%d]", frame_num);
   add_log_entry_position_text(22, 0, 76, 10, msg, "|", " ");

   sprintf(msg,"total moves...............[%d]", game_move_entry_pos);
   add_log_entry_position_text(22, 0, 76, 10, msg, "|", " ");

   sprintf(msg,"total time (seconds)......[%d]", frame_num/40);
   add_log_entry_position_text(22, 0, 76, 10, msg, "|", " ");

   sprintf(msg,"total time (minutes)......[%d]", frame_num/40/60);
   add_log_entry_position_text(22, 0, 76, 10, msg, "|", " ");

   sprintf(msg,"server frames skipped.. ..[%d]", players1[0].frames_skipped);
   add_log_entry_position_text(22, 0, 76, 10, msg, "|", " ");

   log_bandwidth_stats(0);

   add_log_entry_centered_text(22, 0, 76, "", "+", "-");

   for (int p=1; p<NUM_PLAYERS; p++)
   {
      if ((players[p].control_method == 2) || (players[p].control_method == 8))
      {
         sprintf(msg,"Player:%d (%s)", p, players1[p].hostname);
         add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

         sprintf(msg,"frame when client joined..[%d]", players1[p].join_frame);
         add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

         if (players1[p].quit_frame == 0) players1[p].quit_frame = frame_num;
         sprintf(msg,"frame when client quit....[%d]", players1[p].quit_frame);
         add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

         log_reason_for_player_quit(p);

         sprintf(msg,"frames client was active..[%d]", players1[p].quit_frame - players1[p].join_frame);
         add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

         sprintf(msg,"frames skipped............[%d]", players1[p].frames_skipped);
         add_log_entry_position_text(22, 0, 76, 10, msg, "|", " ");

         sprintf(msg,"cdat packets total........[%d]", players1[p].client_cdat_packets_tx);
         add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

         sprintf(msg,"cdat packets late.........[%d]", players1[p].late_cdats);
         add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

         log_bandwidth_stats(p);

         add_log_entry_centered_text(22, p, 76, "", "+", "-");
      }
   }
   log_player_array();
}

void erase_log(void)
{
   log_msg[0] = 0;
   log_msg_pos = 0;
}

void save_log_file(void)
{
   if (L_LOGGING_NETPLAY)
   {
      al_make_directory("logs"); // create if not already created
      FILE *filepntr;

      char filename[256];
      struct tm *timenow;
      time_t now = time(NULL);
      timenow = localtime(&now);
      strftime(filename, sizeof(filename), "logs/%Y%m%d-%H%M%S", timenow);

      char lh[16];
      strncpy(lh, local_hostname, 16); // to remove compiler error in case local_hostname is too long

      char ph[80];
      sprintf(ph, "-[%d][%s].txt", play_level, lh );

      strcat(filename, ph);

      if (strlen(log_msg) > 0)
      {
         filepntr = fopen(filename,"w");
         fprintf(filepntr, "%s", log_msg);
         fclose(filepntr);
         printf("%s saved \n", filename);
      }
   }
   erase_log();
}


//void add_log_entry_sdat_rx_and_game_move_entered(int type, int player)
//{
//   int st = players1[player].client_sdat_packets_rx;      // sdat total
//   int ss = players1[player].client_sdat_packets_skipped; // sdat skipped
//   float sdpc = 0;                                        // % skipped
//   if (st != 0) sdpc = (float)ss * 100 / (float)st;       // prevent divide by zero
//   sprintf(msg,"sdat packets rx'd.........[%3d]  skipped:[%3d][%4.1f%%]", st, ss, sdpc);
//   add_log_entry_position_text(type, player, 76, 10, msg, "|", " ");
//
//   int mt = players1[player].moves_entered + players1[player].moves_skipped; // moves total
//   int ms = players1[player].moves_skipped;                                  // moves skipped
//   float mspc = 0;                                                           // % skipped
//   if (mt != 0) mspc = (float)ms * 100 / (float)mt;                          // prevent divide by zero
//   sprintf(msg,"moves entered:............[%3d]  skipped:[%3d][%4.1f%%]", mt, ms, mspc);
//   add_log_entry_position_text(type, player, 76, 10, msg, "|", " ");
//}

void add_log_entry2(int type, int player, const char *txt)
{
   char tmsg[200];
   sprintf(tmsg, "[%2d][%d][%d]%s", type, player, frame_num, txt);
   // strcat(log_msg, tmsg);

   if ((log_msg_pos + strlen(tmsg)) >= NUM_LOG_CHAR)
   {
      printf("log array full, > %d char\n", NUM_LOG_CHAR);
   }
   else
   {
      memcpy(log_msg + log_msg_pos, tmsg, strlen(tmsg));
      log_msg_pos += strlen(tmsg);
      log_msg[log_msg_pos+1] = 0; // NULL terminate
      //sprintf(log_msg, "%s", txt);
      //printf("%s", tmsg);
   }
}

void add_log_entry_position_text(int type, int player, int width, int pos, const char *txt, const char *border, const char *fill)
{
   int l = strlen(txt);
   int j1 = pos-2;
   int j2 = width - l - pos;
   char p[200];

   if (pos > 1)
   {
      strcpy(p, border);
      for (int i=0; i<j1; i++) strcat(p, fill);
      strcat(p, txt);
      for (int i=0; i<j2; i++) strcat(p, fill);
      strcat(p, border);
      strcat(p, "\n");
   }

   if (pos == 1)
   {
      j2--;
      strcpy(p, border);
      for (int i=0; i<j1; i++) strcat(p, fill);
      strcat(p, txt);
      for (int i=0; i<j2; i++) strcat(p, fill);
      strcat(p, border);
      strcat(p, "\n");
   }

   if (pos == 0)
   {
      strcpy(p, "");
      j2--;
      for (int i=0; i<j1; i++) strcat(p, fill);
      strcat(p, txt);
      for (int i=0; i<j2; i++) strcat(p, fill);
      strcat(p, border);
      strcat(p, "\n");
   }
   add_log_entry2(type, player, p);


}

void add_log_entry_centered_text(int type, int player, int width, const char *txt, const char *border, const char *fill)
{
   int l = strlen(txt);
   int j1 = (width-l)/2 - 1;
   int j2 = j1 + 1;
   if (l % 2 == 0) j2 = j1;

   char p[200];

   strcpy(p, border);
   for (int i=0; i<j1; i++) strcat(p, fill);
   strcat(p, txt);
   for (int i=0; i<j2; i++) strcat(p, fill);
   strcat(p, border);

   strcat(p, "\n");
   add_log_entry2(type, player, p);
}

void add_log_entry_header(int type, int player, const char *txt, int blank_lines)
{
   char htext[80]; // make a copy so that doesn't get overwritten
   sprintf(htext, "%s", txt);
   add_log_entry_centered_text(type, player, 76, "", "+", "-");

   for (int i=0; i<blank_lines; i++)
      add_log_entry_centered_text(type, player, 76, "", "|", " ");

   add_log_entry_centered_text(type, player, 76, htext, "|", " ");

   for (int i=0; i<blank_lines; i++)
      add_log_entry_centered_text(type, player, 76, "", "|", " ");

   add_log_entry_centered_text(type, player, 76, "", "+", "-");
}



// for log file viewer code to find most recent log file
ALLEGRO_FS_ENTRY *filenames[1000];
int num_filenames;

int fill_filename_array(ALLEGRO_FS_ENTRY *fs, void * extra)
{
   if (num_filenames > 999) return 0; // only get 1000 max
   filenames[num_filenames] = al_create_fs_entry(al_get_fs_entry_name(fs));
   num_filenames++;
   return ALLEGRO_FOR_EACH_FS_ENTRY_OK;
}

void log_client_server_sync_graph(int num_lines);
void log_bandwidth_graph(int num_lines);
int lp[8][2];



void get_tag_text(char *str, char *res)
{
//    printf("\nget tag initial %s\n", str);

   // get first tag - server_sync
   char * pch1 = strchr(str, '[');
   char * pch2 = strchr(str, ']');
   int p1 = pch1-str;
   int p2 = pch2-str;
   int plen = p2-p1;

 //  printf("p1:%d  p2:%d  plen:%d\n", p1, p2, plen);

   if (plen < 8) // ???
   {
       for(int j=0; j<plen; j++)
          res[j] = str[j+p1+1];
       res[plen-1] = 0;
   //    printf("tag text [%s]\n", res);
       chop_first_x_char(str, p2+1);
   }
//   printf("get tag final %s\n", str);
}





int log_file_viewer(int type)
{
   al_show_mouse_cursor(display);


   char fname[1024];
   FILE *filepntr;
   char buff[200];
   int num_lines=0;
   int ch=0;

   sprintf(fname, "logs/");
   //printf("fname:%s\n", fname);
   // convert to 'ALLEGRO_FS_ENTRY' (to makes fully qualified path)
   ALLEGRO_FS_ENTRY *FS_fname = al_create_fs_entry(fname);

   // convert back to string
   sprintf(fname, "%s\\", al_get_fs_entry_name(FS_fname));
   //printf("FS_fname:%s\n", fname);

   if (type == 2) // most recent file
   {
      num_filenames = 0;

      // iterate levels in log folder and put in filename array
      al_for_each_fs_entry(FS_fname, fill_filename_array, NULL);

      if (num_filenames == 0)
      {
         printf("No log files found.\n");
         type = 1; // prompt for select file
      }

      else
      {
         //for (int i=0; i< num_filenames; i++)
         //printf("%s\n", al_get_fs_entry_name(filenames[i]));

         time_t t = 0;
         int latest = 0;
         for (int i=0; i< num_filenames; i++)
            if (al_get_fs_entry_ctime(filenames[i]) > t)
            {
               t = al_get_fs_entry_ctime(filenames[i]);
               latest = i;
            }

         //printf("most recent file is: %s\n", al_get_fs_entry_name(filenames[latest]));

         sprintf(fname, "%s", al_get_fs_entry_name(filenames[latest]));
      }
   }


   if (type == 1) // select file
   {
      int user_cancelled = 0;
      ALLEGRO_FILECHOOSER *afc = al_create_native_file_dialog(fname, "Select Log File to View", "*.txt", 0);
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
      if (user_cancelled) return 0;
   }


   // get just the name part of the path
   ALLEGRO_PATH * path = al_create_path(fname);
   const char *tmp = al_get_path_filename(path);
   char fnam[100];
   sprintf(fnam, "%s", tmp);
   al_destroy_path(path);


   al_set_target_backbuffer(display);
   al_clear_to_color(al_map_rgb(0,0,0));
   al_draw_textf(font, palette_color[15], SCREEN_W/2, SCREEN_H/2+6, ALLEGRO_ALIGN_CENTER, "Loading Log File:%s       ", fnam);
   al_flip_display();


   filepntr=fopen(fname,"r");
   while(ch != EOF)
   {
      int loop = 0;
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

      if (loop > 99) printf("log line%d exceeded 100 char - %s\n", num_lines, buff);

      strncpy (log_lines[num_lines], buff, 99);

      num_lines++;

      if (num_lines >= NUM_LOG_LINES)
      {
         ch = EOF;
         printf("log file exceeded %d lines\n", num_lines);
      }
      // printf("num_lines:%d\n", num_lines);
   }
   fclose(filepntr);
   num_lines--;


   al_set_target_backbuffer(display);
   al_clear_to_color(al_map_rgb(0,0,0));
   al_draw_textf(font, palette_color[15], SCREEN_W/2, SCREEN_H/2+6, ALLEGRO_ALIGN_CENTER, "Loading Log File:%s - Done", fnam);
   al_flip_display();

   // printf("log file 2\n");


   char ctags[100][20];
   int tags[100][5];
   for (int i=0; i<100; i++)
   {
      tags[i][0] = 1;  // show/hide (all on by default)
      tags[i][1] = 15; // color (white by default)
      tags[i][2] = 0;  // number of type
      tags[i][3] = 0;  // key toggle
      tags[i][4] = 0;  // pos in list
   }

   // always on
   tags[10][0] = 1; tags[10][1] = 15; // reg netplay
   tags[11][0] = 1; tags[11][1] = 15; // join
   tags[20][0] = 1; tags[20][1] = 15; // game init stuff
   tags[22][0] = 1; tags[22][1] = 15; // ending stats


   tags[23][0] = 0; tags[23][1] = 15; tags[23][3] = 66; sprintf(ctags[23], "byts"); // bandwidth    (B) [CS]
   tags[24][0] = 0; tags[24][1] = 15; tags[24][3] = 65; sprintf(ctags[24], "pcks"); // packets      (A) [CS]

   tags[26][0] = 1; tags[26][1] = 15; tags[26][3] = 82; sprintf(ctags[26], "parr"); // player array (R) [CS]

   tags[27][0] = 1; tags[27][1] = 13; tags[27][3] = 88; sprintf(ctags[27], "stdf"); // stdf         (X) [CS]
   tags[28][0] = 0; tags[28][1] = 1;  tags[28][3] = 80; sprintf(ctags[28], "stdp"); // stdf piece   (P) [CS]
   tags[29][0] = 0; tags[29][1] = 7;  tags[29][3] = 87; sprintf(ctags[29], "stdw"); // stdf when    (W) [C]
   tags[30][0] = 0; tags[30][1] = 6;  tags[30][3] = 75; sprintf(ctags[30], "stak"); // stak         (K) [S]

   tags[31][0] = 0; tags[31][1] = 15; tags[31][3] = 68; sprintf(ctags[31], "dif1"); // show diff1   (D) [C]
   tags[32][0] = 0; tags[32][1] = 15; tags[32][3] = 70; sprintf(ctags[32], "dif2"); // show diff2   (F) [C]
   tags[35][0] = 1; tags[35][1] = 3;  tags[35][3] = 67; sprintf(ctags[35], "cdat"); // cdat         (C) [CS]


   tags[40][0] = 1; tags[40][1] = 15; tags[40][3] = 69; sprintf(ctags[40], "gmar"); // game move array (E) [S]
   tags[41][0] = 1; tags[41][1] = 15; tags[41][3] = 77; sprintf(ctags[41], "move"); // move objects    (M) [S]




   tags[99][0] = 1; tags[99][1] = 10; // bad tag

   // printf("log file 3\n");


   // find and process tags
   for (int i=0; i<num_lines; i++)
   {
      int bad_tags = 0;

      char res[80];

      // get first tag - type
      get_tag_text(log_lines[i], res);
      log_lines_int[i][0] = atoi(res);

      // get second tag - player
      get_tag_text(log_lines[i], res);
      log_lines_int[i][1] = atoi(res);

      // get third tag - frame_num
      get_tag_text(log_lines[i], res);
      log_lines_int[i][2] = atoi(res);

      if (bad_tags)
      {
         log_lines_int[i][0] = 99;
         tags[99][2]++; // inc number of this tag
      }

      //printf("Line:%d\n", i);

      if (num_lines > 100)
      {
         if ((i % (num_lines/100)) == 0)
         {
            // printf("i:%d nl:%d\n", i, num_lines);
            al_set_target_backbuffer(display);
            al_clear_to_color(al_map_rgb(0,0,0));
            draw_percent_bar(SCREEN_W/2, SCREEN_H/2, SCREEN_W-200, 20, (i*100)/num_lines);
            al_draw_text(font, palette_color[15], SCREEN_W/2, SCREEN_H/2+6, ALLEGRO_ALIGN_CENTER, "Parsing tags");
            al_flip_display();
         }
      }
      // printf("log file 4\n");
   }

   // printf("log file 5\n");

   // get start and end frame_nums
   int start_pc = log_lines_int[0][2];
   int end_pc = 0;
   for (int i=0; i<num_lines; i++)
      if (log_lines_int[i][2] > end_pc) end_pc = log_lines_int[i][2];

   int first_line = 0; // the top frame_num line on the screen
   int quit = 0;

   // find players in this file
   for (int i=0; i<8; i++)
   {
      lp[i][0] = 0; // show/hide
      lp[i][1] = 0; // num
   }
   for (int i=0; i<num_lines; i++)
   {
      int p = log_lines_int[i][1];
      if ((p >=0) && (p < 8))
      {
         lp[p][0] = 1;
         lp[p][1]++;
      }
   }

   // set players colors
   for (int i=0; i<8; i++)
      if (lp[i][1]) // we have moves for this player
      {
          players[i].color = 8+i;
      }


   while (!quit)
   {
      al_set_target_backbuffer(display);
      al_flip_display();
      al_clear_to_color(al_map_rgb(0,0,0));

      int ty1 = 0;
      int color = 15;


      // draw all the lines
      int i = first_line;
      int last_line = first_line;

      int max_line_length = 0;


      int done = 0;
      while (!done)
      {
         int type = log_lines_int[i][0];
         int p    = log_lines_int[i][1];
         int pc   = log_lines_int[i][2];

         if (type == 99) // bad tags on this line
         {
            sprintf(msg, "i[%d] t[%d] p[%d] pc[%d] (bad tags)- %s", i, type, p, pc, log_lines[i]);
            al_draw_text(font, palette_color[color], 0, ty1+=8, 0, msg);
         }
         else
         {
            sprintf(msg, "[%3d][%4d][%d]%s", i, pc, p, log_lines[i]);
            //sprintf(msg, "i[%d] t[%d] p[%d] pc[%d] - %s", i, type, p, pc, log_lines[i]);
            //sprintf(msg, "[%4d][%d]%s", pc, type, log_lines[i]);
            //sprintf(msg, "[%4d][%d]%s", pc, p, log_lines[i]);
            //sprintf(msg, "[%4d]%s", pc, log_lines[i]);

            color = tags[type][1];
            if ((tags[type][0]) && (lp[p][0])) // tag and player filter
            {
                al_draw_text(font, palette_color[color], 0, ty1+=8, 0, msg);
                last_line = i;
                if ((int)strlen(msg) > max_line_length) max_line_length = strlen(msg);
            }
         }
         if (++i >= num_lines) done = 1; // no more lines
         if (ty1 > SCREEN_H - 20) done = 1; // no more screen
      }


      // use max_line length to get xpos of right side panel info
      int xpos = max_line_length * 8;
      if (xpos < 720) xpos = 720;

      // vertical scroll bar
      int sbc1 = 15; // frame color
      int sbc3 = 11; // position indicator color


      // frame
      int sbx1 = xpos;
      int sbx2 = sbx1+8;
      int sby1 = 8;
      int sby2 = SCREEN_H - 10;
      int sbh = sby2-sby1;
      al_draw_rectangle(sbx1, sby1, sbx2, sby2+2, palette_color[sbc1], 1);

      // position indicator

      // percentage of first line in num lines
      float fline_pc = (float) first_line / (float)num_lines;

      // percentage of last line in num lines
      float lline_pc = (float) last_line / (float)num_lines;

      // scale to screen height
      float sbby1 = fline_pc * sbh;
      float sbby2 = lline_pc * sbh;

      // enforce minimum size
      if ((sbby2 - sbby1) < 2) sbby2 = sbby1 + 2;

      //al_draw_rectangle(sbx1+1, sby1+sbby1+1, sbx2-1, sby1+sbby2, palette_color[sbc3], 1);
      al_draw_filled_rectangle(sbx1+1, sby1+sbby1+1, sbx2-1, sby1+sbby2, palette_color[sbc3]);


      al_draw_textf(f3, palette_color[sbc3], sbx2+4, sby1+sbby1-8, 0, "%d", first_line);
      al_draw_textf(f3, palette_color[sbc3], sbx2+4, sby1+sbby2-1, 0, "%d", last_line);




      if ((mouse_x > sbx1) && (mouse_x < sbx2))
      {
         //al_draw_rectangle(sbx1, sby1, sbx2, sby2, palette_color[sbc2], 1); // highlight scroll bar


         float my = mouse_y - sby1; // mouse offset from start of scroll bar
         float mp = my / (float)sbh; // percent of mouse to scroll bar height
         int sbmy = num_lines * (float) mp; // log line that mouse_y corresponds to


         // frame number of that log line

         int fn   = log_lines_int[sbmy][2];

         al_draw_textf(f3, palette_color[15], sbx2+4, mouse_y-8, 0, "%d - frame:%d %ds %dm", sbmy, fn, fn/40, fn/2400);





         if (mouse_b[1][0])
         {
            while (mouse_b[1][0]) proc_controllers();
            first_line = sbmy; // set new log line pos
         }
      }






      // how many lines are visible?
      int vis_lines = 0;
      for (int i=0; i<num_lines; i++)
      {
         int type = log_lines_int[i][0];
         int p    = log_lines_int[i][1];
         if ((tags[type][0]) && (lp[p][0])) vis_lines++; // tag and player filter
      }


      xpos+=32;

      int ly = 4;
      sprintf(msg, "Current Log");
      al_draw_text(font, palette_color[15], xpos, ly+=8, 0, msg);

      sprintf(msg, "%s", fnam);
      al_draw_text(font, palette_color[15], xpos, ly+=8, 0, msg);

      sprintf(msg, "Starting frame...[%d]", start_pc);
      al_draw_text(font, palette_color[15],xpos, ly+=8, 0, msg);

      sprintf(msg, "Ending frame.....[%d]", end_pc);
      al_draw_text(font, palette_color[15],xpos, ly+=8, 0, msg);

      sprintf(msg, "Total lines......[%d]", num_lines);
      al_draw_text(font, palette_color[15],xpos, ly+=8, 0, msg);

      sprintf(msg, "Visible lines....[%d]", vis_lines);
      al_draw_text(font, palette_color[15],xpos, ly+=8, 0, msg);




      // show tag labels
      ly+=8;
      for (int i=23; i<50; i++)
      {
         if (tags[i][2]) // this tag type is present in log
         {
            int col = tags[i][1];
            char tmsg[5];
            if (tags[i][0]) sprintf(tmsg,"on ");
            else
            {
               sprintf(tmsg,"off");
               col = 127; //grey
            }
            tags[i][4] = ly; ly+=8; // set the ypos
            sprintf(msg, "%c %s %s num:[%d]", tags[i][3], ctags[i], tmsg, tags[i][2]);
            al_draw_text(font, palette_color[col], xpos, ly, 0, msg);
         }
      }

      // show players
      ly+=8;
      for (int i=0; i<8; i++)
      if (lp[i][1])
      {
         int col = 15;
         char tmsg[5];
         if (lp[i][0]) sprintf(tmsg,"on ");
         else
         {
            sprintf(tmsg,"off");
            col = 127; //grey
         }
         al_draw_textf(font, palette_color[col], xpos, ly+=8, 0, "%d plyr:%d %s num:[%d]", i, i, tmsg, lp[i][1]);
      }



      // current display
      ly+=8;
      sprintf(msg, "Current Display");
      al_draw_text(font, palette_color[11],xpos, ly+=8, 0, msg);

      sprintf(msg, "Lines......[%d] to [%d]", first_line, last_line);
      al_draw_text(font, palette_color[11],xpos, ly+=8, 0, msg);


      int first_frame = log_lines_int[first_line][2];
      int last_frame = log_lines_int[last_line][2];

      sprintf(msg, "Frames.....[%d] to [%d]", first_frame, last_frame);
      al_draw_text(font, palette_color[11],xpos, ly+=8, 0, msg);

      sprintf(msg, "Seconds....[%d] to [%d]", first_frame/40, last_frame/40);
      al_draw_text(font, palette_color[11],xpos, ly+=8, 0, msg);



      ly+=20;

      int xa = xpos;
      int xb = xpos + 180;

      if (mdw_buttont(xa, ly, xb, 16, 0,0,0,0, 0,13,15,0, 1,0,1,0, "Client Sync Graph"))  log_client_server_sync_graph(num_lines);
      ly+=4;
      if (mdw_buttont(xa, ly, xb, 16, 0,0,0,0, 0,13,15,0, 1,0,1,0, "Bandwidth Graph"))    log_bandwidth_graph(num_lines);
      ly+=4;
      if (mdw_buttont(xa, ly, xb, 16, 0,0,0,0, 0,14,15,0, 1,0,1,0, "Help"))               help("Log File Viewer");

      al_flip_display();

      proc_controllers();

      int k = key_pressed_ASCII;
      if ((k > 36) && (k < 45)) k+=11; // convert number pad number to regular numbers
      if ((k > 47) && (k < 56))        // numbers 0-7 toggle players
      {
         int p = k-48;
         lp[p][0] = !lp[p][0];
      }
      if ((k > 96) && (k < 123)) k-=32; // convert lower case to upper
      if ((k > 64) && (k < 91))         // letters toggle tags
      {
         for (int i=23; i<50; i++)
            if (tags[i][3] == k) tags[i][0] = !tags[i][0]; // toggle tag on/off
      }

      if (key[ALLEGRO_KEY_UP  ][3]) first_line--;
      if (key[ALLEGRO_KEY_DOWN][3]) first_line++;

      if (key[ALLEGRO_KEY_PGUP][3])
      {
         if ((key[ALLEGRO_KEY_LCTRL][0]) || (key[ALLEGRO_KEY_RCTRL][0])) first_line -= 1000;
         else if ((key[ALLEGRO_KEY_LSHIFT][0]) || (key[ALLEGRO_KEY_RSHIFT][0])) first_line -= 100;
         else first_line-=10;
      }
      if (key[ALLEGRO_KEY_PGDN][3])
      {
         if ((key[ALLEGRO_KEY_LCTRL][0]) || (key[ALLEGRO_KEY_RCTRL][0])) first_line += 1000;
         else if ((key[ALLEGRO_KEY_LSHIFT][0]) || (key[ALLEGRO_KEY_RSHIFT][0])) first_line += 100;
         else first_line+=10;
      }

      if (key[ALLEGRO_KEY_HOME][3]) first_line = 0;
      if (key[ALLEGRO_KEY_END ][3]) first_line = num_lines-1;

      if (first_line < 0) first_line = 0;
      if (first_line > num_lines-1) first_line = num_lines-1;

      if (key[ALLEGRO_KEY_ESCAPE][3]) quit = 1;

   } // end of log file viewer
   al_hide_mouse_cursor(display);
   return 0;
}


void autoscale_log_bandwidth_graph(int mode, int num_data, int data[][4], int graph_w, int graph_h, float &y_scale, float &x_scale, int &g_stf, int &g_rng, int end_fn)
{
   if (mode == 1) // reset x pos and scale
   {
      g_stf = 0;
      g_rng = end_fn;
      x_scale = (float)graph_w / (float)g_rng;
   }

   // find the max y value currently displayed
   int max = 0;
   for (int i=0; i<num_data; i++)
   {
      if (lp[data[i][1]][0]) // only if this player's data is not hidden
      {
         if ((data[i][0] >= g_stf) && (data[i][0] <= (g_stf + g_rng) )) // only for the time range currently displayed
         {
            if (data[i][2] > max) max = data[i][2];
            if (data[i][3] > max) max = data[i][3];
         }
      }
   }
   if (max == 0) y_scale = 1;
   else y_scale = (float)(graph_h-40) / (float)max; // set y scale based on graph height and largest value
}



void redraw_log_bandwidth_graph(int num_data, int data[][4], int graph_w, int graph_h, float y_scale, float x_scale, int &g_stf, int &g_rng, int end_fn, int xbl, int ybl)
{
   al_set_target_backbuffer(display);
   al_clear_to_color(al_map_rgb(0,0,0));

   if (g_stf < 0) g_stf = 0;

   // label the x axis and draw gridlines
   g_rng = (graph_w / x_scale);    // x axis range in frames
   int maxf = g_stf + g_rng;       // max x value

   // now choose units
   if (g_rng < 400) // frames
   {
      al_draw_text(font, palette_color[13], xbl+graph_w/2, ybl+12, ALLEGRO_ALIGN_CENTER, "time (frames)");
      int div = 1;
      // get max label text size in pixels
      sprintf(msg, "%d ", maxf / div);
      int tm = strlen(msg) * 8;
      int spi = (int) ((float)tm / x_scale); // scale to graph size
      int sp2 = ((spi/div) * div) + div; // snap to div size
      // draw legend and gridline every sp2 frames
      for (float i=g_stf; (int)((i-g_stf)*x_scale) < graph_w; i+= sp2)
      {
         int hx = xbl + (int)((i-g_stf)*x_scale);
         al_draw_textf(font, palette_color[13], hx, ybl+4, ALLEGRO_ALIGN_CENTER, "%d", (int)(i));
         al_draw_line(hx, 0, hx, ybl, palette_color[13+96], 1);
      }
   }

   if ((g_rng >= 400) && (g_rng < 20000)) // seconds
   {
      al_draw_text(font, palette_color[13], xbl+graph_w/2, ybl+12, ALLEGRO_ALIGN_CENTER, "time (seconds)");
      int div = 40;
      // get max label text size in pixels
      sprintf(msg, "%d ", maxf / div);
      int tm = strlen(msg) * 8;
      int spi = (int) ((float)tm / x_scale); // scale to graph size
      int sp2 = ((spi/div) * div) + div; // snap to div size

       // only allowed values are 40, 80, 200, 400, 800
      if ((sp2 > 80) && (sp2 < 200)) sp2 = 200;
      if ((sp2 > 200) && (sp2 < 400)) sp2 = 400;
      if ((sp2 > 400) && (sp2 < 800)) sp2 = 800;
      if ((sp2 > 800) && (sp2 < 1600)) sp2 = 1600;

      // draw legend every sp2 frames
      for (float i=g_stf; (int)((i-g_stf)*x_scale) < graph_w; i+= sp2)
      {
         int hx = xbl + (int)((i-g_stf)*x_scale);
         al_draw_textf(font, palette_color[13], hx, ybl+4, ALLEGRO_ALIGN_CENTER, "%d", (int)(i/div));
         //al_draw_line(hx, 0, hx, ybl+4, palette_color[13+32], 1);
      }
      // draw scale lines every second
      for (float i=g_stf; (int)((i-g_stf)*x_scale) < graph_w; i+= 40)
      {
         int hx = xbl + (int)((i-g_stf)*x_scale);
         al_draw_line(hx, 0, hx, ybl+2, palette_color[13+96], 1);
      }
      // draw scale line every 10 seconds
      for (float i=g_stf; (int)((i-g_stf)*x_scale) < graph_w; i+= 400)
      {
         int hx = xbl + (int)((i-g_stf)*x_scale);
         al_draw_line(hx, 0, hx, ybl+2, palette_color[13+32], 2);
      }
   }

   if (g_rng >= 20000) // minutes
   {
      al_draw_text(font, palette_color[13], xbl+graph_w/2, ybl+12, ALLEGRO_ALIGN_CENTER, "time (minutes)");
      int div = 2400;
      // get max label text size in pixels
      sprintf(msg, "%d ", maxf / div);
      int tm = strlen(msg) * 8;
      int spi = (int) ((float)tm / x_scale); // scale to graph size
      int sp2 = ((spi/div) * div) + div; // snap to div size
      // draw legend and scale line every sp2 frames
      for (float i=g_stf; (int)((i-g_stf)*x_scale) < graph_w; i+= sp2)
      {
         int hx = xbl + (int)((i-g_stf)*x_scale);
         al_draw_textf(font, palette_color[13], hx, ybl+4, ALLEGRO_ALIGN_CENTER, "%d", (int)(i/div));
         al_draw_line(hx, 0, hx, ybl, palette_color[13+32], 2);
      }
      // draw x scale lines every 10 seconds
      for (float i=g_stf; (int)((i-g_stf)*x_scale) < graph_w; i+= 400)
      {
         int hx = xbl + (int)((i-g_stf)*x_scale);
         al_draw_line(hx, 0, hx, ybl, palette_color[13+32], 1);
      }
   }

   // get label spacing
   int s = (int) (10 / y_scale);
   int sp2 = ((s/100) * 100) + 100; // snap to div size
   if ((sp2 > 200) && (sp2 < 500)) sp2 = 500;
   if ((sp2 > 500) && (sp2 < 1000)) sp2 = 1000;
   if ((sp2 > 1000) && (sp2 < 2000)) sp2 = 2000;
   if ((sp2 > 2000) && (sp2 < 5000)) sp2 = 5000;

   for (float i=0; i*y_scale < graph_h; i+= sp2)
   {
      int y_pos = ybl - (int)(i*y_scale);
      al_draw_line(xbl, y_pos, xbl+graph_w, y_pos, palette_color[11+96], 1);

      if (i<1000)         sprintf(msg, "%5.0fB/s", i);
      else if (i<10000)   sprintf(msg, "%3.2fkB/s", i/1000);
      else if (i<100000)  sprintf(msg, "%3.1fkB/s", i/1000);
      else if (i<1000000) sprintf(msg, "%4.0fkB/s", i/1000);
      al_draw_text(font, palette_color[11], 10, y_pos-4, 0, msg);
   }

//   // draw major grid lines every 1kB/s
//   for (float i=0; i*y_scale < graph_h; i+= 1000)
//   {
//      int y_pos = ybl - (int)(i*y_scale);
//      al_draw_line(xbl, y_pos, xbl+graph_w, y_pos, palette_color[11+96], 2);
//   }

   // draw the data one player at a time
   int old_ix, old_ity, old_iry;
   al_set_clipping_rectangle(xbl*display_transform_double, 0, graph_w*display_transform_double, graph_h*display_transform_double);
   for (int p=0; p<NUM_PLAYERS; p++)
   {
      if (lp[p][0])
      {
         int first_time = 1;
         for (int i=0; i<num_data; i++)
         {
            int pc = data[i][0];
            int dp = data[i][1];
            int tx = data[i][2];
            int ty1 = data[i][3];
            int col = players[p].color;
            if (dp == p)
            {
               int ix = xbl + (int) ( (float)(pc-g_stf) * x_scale);
               int ity = ybl - (int) ( (float)tx * y_scale);
               int iry = ybl - (int) ( (float)ty1 * y_scale);
               if (first_time) // set previous point to this point
               {
                  first_time = 0;
                  old_ix = ix;
                  old_ity = ity;
                  old_iry = iry;
               }
               al_draw_line(old_ix, old_iry, ix, iry, palette_color[col], 4);
               al_draw_line(old_ix, old_iry, ix, iry, palette_color[0], 1);
               al_draw_filled_circle(ix, iry, 3, palette_color[col]);


               al_draw_line(old_ix, old_ity, ix, ity, palette_color[col], 4);
               al_draw_filled_circle(ix, ity, 3, palette_color[col]);

               old_ix = ix;
               old_ity = ity;
               old_iry = iry;
            }
         }
      }
   }
   al_reset_clipping_rectangle();




   // draw controls
   int ypos = 6;
   int xpos = SCREEN_W - 234;

   al_draw_filled_rectangle(xpos-1, ypos+6, xpos+232, ypos+89, palette_color[15+196]);
   al_draw_rectangle(xpos-1, ypos+6, xpos+232, ypos+89, palette_color[15], 1);

  // framed title bar
   int xc = xpos + 116;
   al_draw_filled_rectangle(xc-34, ypos-5, xc+34, ypos+6, palette_color[15+196]);
   al_draw_rectangle(xc-34, ypos-5, xc+34, ypos+6, palette_color[15], 1);
   al_draw_text(font, palette_color[15], xc, ypos-3, ALLEGRO_ALIGN_CENTER, "Controls");

   al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "[NUMBERS 0-7] toggle player");


//   al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "[RIGHT]       zoom in x axis");
//   al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "[LEFT]        zoom out x axis");
//   al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "[CTRL][RIGHT] scroll right");
//   al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "[CTRL][LEFT]  scroll left");
//   al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "[SHFT][RIGHT] scroll faster");
//   al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "[SHFT][LEFT]  scroll faster");


   al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "[HOME]        reset x axis");
   al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "[UP]          zoom + y axis");
   al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "[DOWN]        zoom - y axis");
   al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "[?]           help");
   al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "[ESC]         quit");
   al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "----- mouse controls -----");
   al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "wheel         zoom x axis");
   al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "drag          move x axis");
   al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "[SHIFT] drag  zoom x region");



   // draw legend
   xpos = SCREEN_W - 226;
   ypos+=28;

   // framed title bar
   xc = xpos + 110;
   al_draw_filled_rectangle(xc-30, ypos-13, xc+30, ypos-2, palette_color[15+196]);
   al_draw_rectangle(xc-30, ypos-13, xc+30, ypos-2, palette_color[15], 1);
   al_draw_text(font, palette_color[15], xc, ypos-11, ALLEGRO_ALIGN_CENTER, "Legend");

   // iterate players that have bandwidth data
   for (int i=0; i<8; i++)
      if (lp[i][1])
      {
         // clear and frame row
         al_draw_filled_rectangle(xpos-1, ypos-1, xpos+220, ypos+8, palette_color[15+196]);
         al_draw_rectangle(xpos-2, ypos-2, xpos+220, ypos+9, palette_color[15], 1);

         int col = players[i].color;
         char tmsg[5];
         if (lp[i][0]) sprintf(tmsg,"on ");
         else
         {
            sprintf(tmsg,"off");
            col = 127; //grey
         }
         al_draw_textf(font, palette_color[col], xpos, ypos, 0, "player:%d %s", i, tmsg);

         if (lp[i][0])
         {
            int yp = ypos +4;
            int rt = xpos +106;
            int rs = rt+25;
            int re = rs+20;
            al_draw_text(font, palette_color[col], rt, ypos, 0, "rx:");
            al_draw_line(rs, yp, re, yp, palette_color[col], 4);
            al_draw_line(rs, yp, re, yp, palette_color[0], 1);
            al_draw_filled_circle(rs, yp, 3, palette_color[col]);
            al_draw_filled_circle(re, yp, 3, palette_color[col]);

            rt = re+20;
            rs = rt+25;
            re = rs+20;
            al_draw_text(font, palette_color[col], rt, ypos, 0, "tx:");
            al_draw_line(rs, yp, re, yp, palette_color[col], 4);
            al_draw_filled_circle(rs, yp, 3, palette_color[col]);
            al_draw_filled_circle(re, yp, 3, palette_color[col]);
         }
         ypos+=11;
      }
}






void log_bandwidth_graph(int asgdj)
{




















   FILE *filepntr;
   char buff[200];
   int num_lines=0;
   int ch=0;

   char fnam[100];
   sprintf(fnam, "logs/a.txt");


   al_set_target_backbuffer(display);
   al_clear_to_color(al_map_rgb(0,0,0));
   al_draw_textf(font, palette_color[15], SCREEN_W/2, SCREEN_H/2+6, ALLEGRO_ALIGN_CENTER, "Loading Log File:%s       ", fnam);
   al_flip_display();


   filepntr=fopen(fnam,"r");
   while(ch != EOF)
   {
      int loop = 0;
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

      if (loop > 99) printf("log line%d exceeded 100 char - %s\n", num_lines, buff);

      strncpy (log_lines[num_lines], buff, 99);

      num_lines++;

      if (num_lines >= NUM_LOG_LINES)
      {
         ch = EOF;
         printf("log file exceeded %d lines\n", num_lines);
      }
      // printf("num_lines:%d\n", num_lines);
   }
   fclose(filepntr);
   num_lines--;


   printf("num_lines:%d\n", num_lines);



   // iterate all log lines and build array of data points
   for (int i=0; i<num_lines; i++)
//   for (int i=0; i<1000; i++)
   {
      char tll[200]; // temp log line
      sprintf(tll, "%s", log_lines[i]);



      char res[80];

      // get first tag - type
      get_tag_text(tll, res);
      int type = atoi(res);


      // get 2nd tag -
      get_tag_text(tll, res);
      int p = atoi(res);


      // get 3rd tag - frame
      get_tag_text(tll, res);
      int fn = atoi(res);


      if ((type == 23) && (p == 0))
      {
         //printf("tll: %s", tll);

         // get 4th - tx
         get_tag_text(tll, res);
         int tx = atoi(res);

          mG[0].add_data_point((double) fn, (double) tx);
    //     printf("added data %d %d %d \n", mG[0].data_points, fn, tx);

      }

   }





   al_set_target_backbuffer(display);

   mG[0].set_screen_pos(200, 200, 800, 800);
   mG[0].calc_data_range();
   mG[0].autorange_axis(1, 1);
   mG[0].y_axis_zoom_lock = 0;


   sprintf(mG[0].title, "Testing Data");
   mG[0].show_title = 1;
   mG[0].title_color = 10;

   sprintf(mG[0].x_axis_legend, "Frame");
   mG[0].show_x_axis_legend = 1;
   mG[0].x_axis_legend_color = 10;


   int quit = 0;
   al_show_mouse_cursor(display);
   while (!quit)
   {
      al_clear_to_color(al_map_rgb(0, 0, 0));
      mG[0].draw();
      mG[0].process_input();

      al_flip_display();
      proc_controllers();
      if (key[ALLEGRO_KEY_ESCAPE][3]) quit = 1;
   }











}

void oldlog_bandwidth_graph(int num_lines)
{
   // draws a bandwidth graph
   // uses this log entry:
   // [23][0][1360]bandwidth (B/s) TX cur:[ 1878] max:[ 5060] RX cur:[  480] max:[  514]

   // log file has already been loaded into:
   // int log_lines_int[1000000][3];
   // char log_lines[1000000][100];
   // int num_lines
   // player array has already been populated
   // int lp[8][2];


   // max amount of data points
   int max_data = 100000;

   int num_data = 0;

   // array of data points
   int data[max_data][4];
   // 0  frame_num
   // 1  player
   // 2  tx b
   // 3  rx b


   char res[80];

   int end_fn = 0; // calculate our own end_fn

   // clear data points
   for (int i=0; i<max_data; i++)
      for (int j=0; j<4; j++)
         data[i][j] = 0;

   // iterate all log lines and build array of data points
   for (int i=0; i<num_lines; i++)
   {
      if (log_lines_int[i][0] == 23)
      {
         int p = log_lines_int[i][1];
         int fn = log_lines_int[i][2];

         char tll[200]; // temp log line
         sprintf(tll, "%s", log_lines[i]);

         // get first tag - tx_cur
         get_tag_text(tll, res);
         int tx = atoi(res);

         // get second tag and discard - tx_max
         get_tag_text(tll, res);

         // get third tag  - rx_cur
         get_tag_text(tll, res);
         int rx = atoi(res);

         // enter one array data point
         data[num_data][0] = fn;
         data[num_data][1] = p;
         data[num_data][2] = tx;
         data[num_data][3] = rx;

         end_fn = fn;

         num_data++;

         //printf("fn:%d  p:%d tx:%d  rx:%d\n", fn, p, tx, rx);

         if (num_data > max_data)
         {
            printf("number of graph points > %d\n", max_data);
            i=num_lines; // break out of loop
         }
      }
   }
   // all the data is in the array
   num_data--;

   int gquit = 0;
   int redraw = 1;

   // set the graph width, height and baseline

   int xbl = 70;            // x baseline
   int ybl = SCREEN_H - 20; // y baseline

   int graph_w = SCREEN_W - 70;
   int graph_h = SCREEN_H - 20;

   int g_stf = 0; // graph start fn
   int g_rng = 0; // graph frame range

   float y_scale = 1;
   float x_scale = 1;

   autoscale_log_bandwidth_graph(1, num_data, data, graph_w, graph_h, y_scale, x_scale, g_stf, g_rng, end_fn);

   while (!gquit)
   {
      if (redraw)
      {
         redraw_log_bandwidth_graph(num_data, data, graph_w, graph_h, y_scale, x_scale, g_stf, g_rng, end_fn, xbl, ybl);
         al_flip_display();
         redraw = 0;
      }

      if (mouse_b[1][0])
      {
         if ((key[ALLEGRO_KEY_LSHIFT][0]) || (key[ALLEGRO_KEY_RSHIFT][0])) // shift drag to set new zoom area
         {
            int mx1 = mouse_x;
            int my1 = mouse_y;
            int mx2 = 0;
            int my2 = 0;
            while (mouse_b[1][0])
            {
               proc_controllers();
               mx2 = mouse_x;
               my2 = mouse_y;
               redraw_log_bandwidth_graph(num_data, data, graph_w, graph_h, y_scale, x_scale, g_stf, g_rng, end_fn, xbl, ybl);
               al_draw_rectangle(mx1, my1, mx2, my2, palette_color[15], 1);
               al_flip_display();
            }
            // convert mouse x position to graph x pos
            int gx1 = g_stf + (mx1-xbl) / x_scale;
            int gx2 = g_stf + (mx2-xbl) / x_scale;

            if (gx1 < 0) gx1 = 0;
            if (gx2 < 0) gx2 = 0;

            if (gx1 > gx2) swap_int(&gx1, &gx2);

            if (gx1 != gx2)
            {
               g_stf = gx1;     // start frame
               g_rng = gx2-gx1; // frame range

               // calc x_scale so that range fits screen width
               x_scale = (float)graph_w / (float)g_rng;
            }
            autoscale_log_bandwidth_graph(0, num_data, data, graph_w, graph_h, y_scale, x_scale, g_stf, g_rng, end_fn);
            redraw = 1;
         }
         else // normal mouse drag to slide graph
         {
            int mx = mouse_x;
            int gx = 0;
            while (mouse_b[1][0])
            {
               proc_controllers();
               gx = (mx - mouse_x) / x_scale; // mouse x offset converted to x axis scale

               int temp_g_stf = g_stf+gx;
               if (temp_g_stf < 0) temp_g_stf = 0;


               redraw_log_bandwidth_graph(num_data, data, graph_w, graph_h, y_scale, x_scale, temp_g_stf, g_rng, end_fn, xbl, ybl);
               al_flip_display();
            }
            g_stf += gx; // make the offset permanent
         }
      }

      if (mouse_dz)
      {
         if (mouse_dz > 0)
         {
            mouse_dz = 0;
            int orx = g_stf + (mouse_x-xbl) / x_scale; // get x axis postion under mouse
            x_scale *= 1.1; // change scale
            g_stf = orx - (mouse_x-xbl) / x_scale; // change x axis start frame
            if (g_stf < 0) g_stf = 0;
            redraw = 1;
         }
         if (mouse_dz < 0)
         {
            mouse_dz = 0;
            int orx = g_stf + (mouse_x-xbl) / x_scale; // get x axis postion under mouse
            x_scale *= 0.9;      // change scale
            g_stf = orx - (mouse_x-xbl) / x_scale; // change x axis start frame
            if (g_stf < 0) g_stf = 0;
            redraw = 1;
         }
      }



      proc_controllers();
      int k = key_pressed_ASCII;
      if ((k > 36) && (k < 45)) k+=11; // convert number pad number to regular numbers
      if ((k > 47) && (k < 56)) // numbers 0-7 toggle players
      {
         int p = k-48;
         //printf("p:%d\n", p);
         lp[p][0] = !lp[p][0];
         autoscale_log_bandwidth_graph(0, num_data, data, graph_w, graph_h, y_scale, x_scale, g_stf, g_rng, end_fn);
         redraw = 1;
      }

      int scroll_amt = g_rng / 10; // scroll 10% of screen at once
      if (scroll_amt < 1) scroll_amt = 1;

      if (key[ALLEGRO_KEY_HOME][3])
      {
         autoscale_log_bandwidth_graph(1, num_data, data, graph_w, graph_h, y_scale, x_scale, g_stf, g_rng, end_fn);
         redraw = 1;
      }

      if (key[ALLEGRO_KEY_SLASH][3])
      {
         help("Bandwidth Graph");
         redraw = 1;
      }
      if (key[ALLEGRO_KEY_ESCAPE][3]) gquit = 1;
   }
}











int test_toggle = 1;







void redraw_log_client_server_sync_graph(int num_data, int data[][5], int graph_w, float x_scale, int &g_stf, int &g_rng, int end_fn, int xbl, int yxl)
{
   al_set_target_backbuffer(display);
   al_clear_to_color(al_map_rgb(0,0,0));



   if (g_stf < 0) g_stf = 0;

   // label the x axis and draw gridlines
   g_rng = (graph_w / x_scale);    // x axis range in frames
   int maxf = g_stf + g_rng;       // max x value

   // now choose units
   if (g_rng < 400) // frames
   {
      al_draw_text(font, palette_color[13], xbl+graph_w/2, yxl+12, ALLEGRO_ALIGN_CENTER, "time (frames)");
      int div = 1;
      // get max label text size in pixels
      sprintf(msg, "%d ", maxf / div);
      int tm = strlen(msg) * 8;
      int spi = (int) ((float)tm / x_scale); // scale to graph size
      int sp2 = ((spi/div) * div) + div; // snap to div size
      // draw legend and gridline every sp2 frames
      for (float i=g_stf; (int)((i-g_stf)*x_scale) < graph_w; i+= sp2)
      {
         int hx = xbl + (int)((i-g_stf)*x_scale);
         al_draw_textf(font, palette_color[13], hx, yxl+4, ALLEGRO_ALIGN_CENTER, "%d", (int)(i));
         al_draw_line(hx, 0, hx, yxl, palette_color[13+96], 1);
      }
   }

   if ((g_rng >= 400) && (g_rng < 20000)) // seconds
   {
      al_draw_text(font, palette_color[13], xbl+graph_w/2, yxl+12, ALLEGRO_ALIGN_CENTER, "time (seconds)");
      int div = 40;
      // get max label text size in pixels
      sprintf(msg, "%d ", maxf / div);
      int tm = strlen(msg) * 8;
      int spi = (int) ((float)tm / x_scale); // scale to graph size
      int sp2 = ((spi/div) * div) + div; // snap to div size

       // only allowed values are 40, 80, 200, 400, 800
      if ((sp2 > 80) && (sp2 < 200)) sp2 = 200;
      if ((sp2 > 200) && (sp2 < 400)) sp2 = 400;
      if ((sp2 > 400) && (sp2 < 800)) sp2 = 800;
      if ((sp2 > 800) && (sp2 < 1600)) sp2 = 1600;

      // draw legend every sp2 frames
      for (float i=g_stf; (int)((i-g_stf)*x_scale) < graph_w; i+= sp2)
      {
         int hx = xbl + (int)((i-g_stf)*x_scale);
         al_draw_textf(font, palette_color[13], hx, yxl+4, ALLEGRO_ALIGN_CENTER, "%d", (int)(i/div));
         //al_draw_line(hx, 0, hx, ybl+4, palette_color[13+32], 1);
      }
      // draw scale lines every second
      for (float i=g_stf; (int)((i-g_stf)*x_scale) < graph_w; i+= 40)
      {
         int hx = xbl + (int)((i-g_stf)*x_scale);
         al_draw_line(hx, 0, hx, yxl+2, palette_color[13+96], 1);
      }
      // draw scale line every 10 seconds
      for (float i=g_stf; (int)((i-g_stf)*x_scale) < graph_w; i+= 400)
      {
         int hx = xbl + (int)((i-g_stf)*x_scale);
         al_draw_line(hx, 0, hx, yxl+2, palette_color[13+32], 2);
      }
   }

   if (g_rng >= 20000) // minutes
   {
      al_draw_text(font, palette_color[13], xbl+graph_w/2, yxl+12, ALLEGRO_ALIGN_CENTER, "time (minutes)");
      int div = 2400;
      // get max label text size in pixels
      sprintf(msg, "%d ", maxf / div);
      int tm = strlen(msg) * 8;
      int spi = (int) ((float)tm / x_scale); // scale to graph size
      int sp2 = ((spi/div) * div) + div; // snap to div size
      // draw legend and scale line every sp2 frames
      for (float i=g_stf; (int)((i-g_stf)*x_scale) < graph_w; i+= sp2)
      {
         int hx = xbl + (int)((i-g_stf)*x_scale);
         al_draw_textf(font, palette_color[13], hx, yxl+4, ALLEGRO_ALIGN_CENTER, "%d", (int)(i/div));
         al_draw_line(hx, 0, hx, yxl, palette_color[13+32], 2);
      }
      // draw x scale lines every 10 seconds
      for (float i=g_stf; (int)((i-g_stf)*x_scale) < graph_w; i+= 400)
      {
         int hx = xbl + (int)((i-g_stf)*x_scale);
         al_draw_line(hx, 0, hx, yxl, palette_color[13+32], 1);
      }
   }


   // label the y axis and draw gridlines


   // Graph 1 - Server Sync Drift
   // y axis location, scale, labels and gridlines
   // --------------------------------------------
   int g1_y_screen = SCREEN_H - 90;     // g1 y lowest screen pos
   int g1_h_screen = SCREEN_H/2 - 40;   // g1 y screen height

   // static y scale
   int g1_y_lowest_val  = -80;
   int g1_y_highest_val = +40;

   if (test_toggle) // auto scale based on visble
   {

      g_rng = (graph_w / x_scale);    // x axis range in frames
      int maxf = g_stf + g_rng;       // max x value


      printf("stf:%d enf:%d\n", g_stf, maxf);


      // find min and ma
      int g1_min = 0;
      int g1_max = 0;
      for (int i=0; i<num_data; i++)
      {
         if (data[i][2] != -999)
         {
            if (lp[data[i][1]][0]) // if player active
            {
               if ((data[i][0] >= g_stf) && (data[i][0] <= maxf))  // is frame range on screen?
               {
                  int fc = data[i][2];
                  if (fc > g1_max) g1_max = fc;
                  if (fc < g1_min) g1_min = fc;
               }

            }
         }
      }
      // printf("g1_min:%d g1_max:%d\n", g1_min, g1_max);
      g1_y_lowest_val  = g1_min;
      g1_y_highest_val = g1_max;


   }








   int g1_y_range = g1_y_highest_val - g1_y_lowest_val;
   // set y scale based on screen height and range
   float g1_y_scale = (float)g1_h_screen / (float)(g1_y_range);



   if (g1_y_range > 20)
   {
      // g1 numbered labels
      for (float i=g1_y_lowest_val; i<=g1_y_highest_val; i+=10)
      {
         int y_pos = g1_y_screen - (i-g1_y_lowest_val) * g1_y_scale;
         al_draw_line(xbl, y_pos, xbl+graph_w, y_pos, palette_color[15+96], 1);
         al_draw_textf(font, palette_color[15], 0, y_pos-4, 0, "%5.0f ms", i);
//         al_draw_textf(font, palette_color[15], 8, y_pos+4, 0, "ms");
      }
      // minor grid lines
      for (float i=g1_y_lowest_val; i<=g1_y_highest_val; i+=2)
      {
         int y_pos = g1_y_screen - (i-g1_y_lowest_val) * g1_y_scale;
         al_draw_line(xbl, y_pos, xbl+graph_w, y_pos, palette_color[15+96], 0);
      }
   }
   else
   {
      // g1 numbered labels
      for (float i=g1_y_lowest_val; i<=g1_y_highest_val; i+=2)
      {
         int y_pos = g1_y_screen - (i-g1_y_lowest_val) * g1_y_scale;
         al_draw_line(xbl, y_pos, xbl+graph_w, y_pos, palette_color[15+96], 1);
         al_draw_textf(font, palette_color[15], 0, y_pos-4, 0, "%5.0f ms", i);
//         al_draw_textf(font, palette_color[15], 8, y_pos+4, 0, "ms");
      }
      // minor grid lines
      for (float i=g1_y_lowest_val; i<=g1_y_highest_val; i+=1)
      {
         int y_pos = g1_y_screen - (i-g1_y_lowest_val) * g1_y_scale;
         al_draw_line(xbl, y_pos, xbl+graph_w, y_pos, palette_color[15+96], 0);
      }
   }





   // Graph 2 - Client Chase FPS
   // y axis location, scale, labels and gridlines
   // --------------------------------------------
   int g2_y_screen = SCREEN_H/2 - 80;   // g2 y lowest screen pos
   int g2_h_screen = SCREEN_H/2 - 100;  // g2 y screen height

   // static y scale
   int g2_y_lowest_val  = 20;
   int g2_y_highest_val = 60;

   // auto scale
   int g2_min = 0; // find min and max
   int g2_max = 0;
   for (int i=0; i<num_data; i++)
   {
      if (data[i][3] != -999)
      {
         if (lp[data[i][1]][0]) // if player active
         {
            int fc = data[i][3];
            if (fc > g2_max) g2_max = fc;
            if (fc < g2_min) g2_min = fc;
         }
      }
   }
   // printf("g2_min:%d g2_max:%d\n", g2_min, g2_max);

   g2_y_lowest_val  = 30;
//   g2_y_lowest_val  = g2_min;
   g2_y_highest_val = g2_max;
   int g2_y_range = g2_y_highest_val - g2_y_lowest_val;

   // set y scale based on screen height and range
   float g2_y_scale = (float)g2_h_screen / (float)(g2_y_range);

   // g2 numbered labels
   for (float i=g2_y_lowest_val; i<=g2_y_highest_val; i+=10)
   {
      int y_pos = g2_y_screen - (i-g2_y_lowest_val) * g2_y_scale;
      al_draw_line(xbl, y_pos, xbl+graph_w, y_pos, palette_color[15+96], 1);
      al_draw_textf(font, palette_color[15], 0, y_pos-4, 0, "%4.0f fps", i);
//            al_draw_textf(font, palette_color[15], 8, y_pos+4, 0, "fps");
   }
   // minor grid lines
   for (float i=g2_y_lowest_val; i<=g2_y_highest_val; i+=2)
   {
      int y_pos = g2_y_screen - (i-g2_y_lowest_val) * g2_y_scale;
      al_draw_line(xbl, y_pos, xbl+graph_w, y_pos, palette_color[15+96], 0);
   }

   // Graph 3 - Client Active
   // y axis location, scale, labels and gridlines
   // --------------------------------------------
   int g3_y_screen = SCREEN_H - 30;   // g3 y lowest screen pos
   int g3_h_screen = 30;              // g3 y screen height

   // min and max y data values
   int g3_y_lowest_val  = 0;
   int g3_y_highest_val = 1;
   int g3_y_range = g3_y_highest_val - g3_y_lowest_val;

   // set y scale based on screen height and range
   float g3_y_scale = (float)g3_h_screen / (float)(g3_y_range);

   // g3 labels
   float i=g3_y_lowest_val;
   int y_pos = g3_y_screen - (i-g3_y_lowest_val) * g3_y_scale;
   al_draw_textf(font, palette_color[15], 0, y_pos-4, 0, "Inactive");
   al_draw_line(xbl, y_pos, xbl+graph_w, y_pos, palette_color[15+96], 0);

   i=g3_y_highest_val;
   y_pos = g3_y_screen - (i-g3_y_lowest_val) * g3_y_scale;
   al_draw_textf(font, palette_color[15], 0, y_pos-4, 0, "Active");
   al_draw_line(xbl, y_pos, xbl+graph_w, y_pos, palette_color[15+96], 0);




   // plot g1 data
   int old_ix, old_ity;
   int fx = xbl;
   int fy = g1_y_screen - g1_h_screen;
   int fw = graph_w;
   int fh = g1_h_screen;
   //al_draw_rectangle(fx, fy, fx+fw, fy+fh, palette_color[10], 3);
   al_set_clipping_rectangle(fx * display_transform_double, fy * display_transform_double, fw * display_transform_double, fh * display_transform_double);
   for (int p=0; p<NUM_PLAYERS; p++)
   {
      if (lp[p][0])
      {
         int first_time = 1;
         for (int i=0; i<num_data; i++)
         {
            if (data[i][2] != -999)
            {
               int pc = data[i][0];
               int dp = data[i][1];
               int sy = data[i][2]-g1_y_lowest_val;
               int col = players[p].color;


               if (dp == p)
               {
                  int ix = xbl + (int) ( (float)(pc-g_stf) * x_scale);
                  int ity =  g1_y_screen - (int) ( (float)sy * g1_y_scale);
                  if (first_time) // set previous point to this point
                  {
                     first_time = 0;
                     old_ix = ix;
                     old_ity = ity;
                  }
                  al_draw_line(old_ix, old_ity, ix, ity, palette_color[col], 1);
                  old_ix = ix;
                  old_ity = ity;
               }
            }
         }
      }
   }
   al_reset_clipping_rectangle();


   // plot g2 data
   fx = xbl;
   fy = g2_y_screen - g2_h_screen;
   fw = graph_w;
   fh = g2_h_screen    ; //  bottom line
   //al_draw_rectangle(fx, fy, fx+fw, fy+fh, palette_color[10], 3);
   al_set_clipping_rectangle(fx * display_transform_double, fy * display_transform_double, fw * display_transform_double, fh * display_transform_double);

   for (int p=0; p<NUM_PLAYERS; p++)
   {
      if (lp[p][0])
      {
         int first_time = 1;
         for (int i=0; i<num_data; i++)
         {
            if (data[i][3] != -999)
            {
               int pc = data[i][0];
               int dp = data[i][1];
               int fc = data[i][3]-g2_y_lowest_val;
               int col = players[p].color;
               if (dp == p)
               {
                  int ix = xbl + (int) ( (float)(pc-g_stf) * x_scale);
                  int ity =  g2_y_screen - (int) ( (float)fc * g2_y_scale);
                  if (first_time) // set previous point to this point
                  {
                     first_time = 0;
                     old_ix = ix;
                     old_ity = ity;
                  }
                  al_draw_line(old_ix, old_ity, ix, ity, palette_color[col], 2);
                  old_ix = ix;
                  old_ity = ity;
               }
            }
         }
      }
   }
   al_reset_clipping_rectangle();

   // plot g3 data
   fx = xbl;
   fy = g3_y_screen - g3_h_screen;
   fw = graph_w;
   fh = g3_h_screen+8    ; //  bottom line
   //al_draw_rectangle(fx, fy, fx+fw, fy+fh, palette_color[10], 3);
   al_set_clipping_rectangle(fx * display_transform_double, fy * display_transform_double, fw * display_transform_double, fh * display_transform_double);

   for (int p=0; p<NUM_PLAYERS; p++)
   {
      if (lp[p][0])
      {
         int first_time = 1;
         for (int i=0; i<num_data; i++)
         {
            int pc = data[i][0];
            int dp = data[i][1];
            int pa = data[i][4]-g3_y_lowest_val;
            int col = players[p].color;
            if (dp == p)
            {
               int ix = xbl + (int) ( (float)(pc-g_stf) * x_scale);
               int ity =  g3_y_screen - (int) ( (float)pa * g3_y_scale)+p;
               if (first_time) // set previous point to this point
               {
                  first_time = 0;
                  old_ix = ix;
                  old_ity = ity;
               }
               al_draw_line(old_ix, old_ity, ix, ity, palette_color[col], 1);
               old_ix = ix;
               old_ity = ity;
            }
         }
      }
   }
   al_reset_clipping_rectangle();


   // erase some things to make it look better
   // erase gridlines above g2
   al_draw_filled_rectangle(xbl-2, 0, SCREEN_W, 20, palette_color[0]);

   // erase gridlines between g2 and g1
   fy = g1_y_screen - g1_h_screen; // top of g1
   fw = g2_y_screen; // bottom of g2
   al_draw_filled_rectangle(xbl-2, fy, SCREEN_W, fw, palette_color[0]);

   // erase gridlines between g3 and g1
   fy = g3_y_screen - g3_h_screen; // top of g3
   fw = g1_y_screen;               // bottom of g1
   al_draw_filled_rectangle(xbl-2, fy, SCREEN_W, fw, palette_color[0]);


   // title g1
   int xc = SCREEN_W/2;
   int x1 = xc-75;
   int x2 = xc+75;
   int y1 = g1_y_screen-g1_h_screen-16;
   int y2 = y1 + 13;
   al_draw_filled_rectangle(x1, y1, x2, y2, palette_color[12+160]);
   al_draw_rounded_rectangle(x1, y1, x2, y2, 2, 2, palette_color[12], 2);
   al_draw_text(font, palette_color[15], xc, y1+3, ALLEGRO_ALIGN_CENTER, "Client-Server Sync");
   // g1 frame
   fx = xbl;
   fy = g1_y_screen - g1_h_screen;
   fw = graph_w-2;
   fh = g1_h_screen;
   al_draw_rectangle(fx, fy, fx+fw, fy+fh, palette_color[12], 2);

   // title g2
   x1 = xc-67;
   x2 = xc+67;
   y1 = g2_y_screen-g2_h_screen-16;
   y2 = y1 + 13;
   al_draw_filled_rectangle(x1, y1, x2, y2, palette_color[11+160]);
   al_draw_rounded_rectangle(x1, y1, x2, y2, 2, 2, palette_color[11], 2);
   al_draw_text(font, palette_color[15], xc, y1+3, ALLEGRO_ALIGN_CENTER, "Client Chase FPS");
   // g2 frame
   fx = xbl;
   fy = g2_y_screen - g2_h_screen;
   fw = graph_w-2;
   fh = g2_h_screen;
   al_draw_rectangle(fx, fy, fx+fw, fy+fh, palette_color[11], 2);

   // title g3
   x1 = xc-67;
   x2 = xc+67;
   y1 = g3_y_screen-g3_h_screen-16;
   y2 = y1 + 13;
   al_draw_filled_rectangle(x1, y1, x2, y2, palette_color[13+160]);
   al_draw_rounded_rectangle(x1, y1, x2, y2, 2, 2, palette_color[13], 2);
   al_draw_text(font, palette_color[15], xc, y1+3, ALLEGRO_ALIGN_CENTER, "Client Active");
   // g2 frame
   fx = xbl;
   fy = g3_y_screen - g3_h_screen;
   fw = graph_w-2;
   fh = g3_h_screen+9;
   al_draw_rectangle(fx, fy, fx+fw, fy+fh, palette_color[13], 2);

   // draw controls
   int ypos = 6;
   int xpos = SCREEN_W - 234;

   al_draw_filled_rectangle(xpos-1, ypos+6, xpos+232, ypos+73, palette_color[15+196]);
   al_draw_rectangle(xpos-1, ypos+6, xpos+232, ypos+73, palette_color[15], 1);

   // framed title bar
   xc = xpos + 116;
   al_draw_filled_rectangle(xc-34, ypos-5, xc+34, ypos+6, palette_color[15+196]);
   al_draw_rectangle(xc-34, ypos-5, xc+34, ypos+6, palette_color[15], 1);
   al_draw_text(font, palette_color[15], xc, ypos-3, ALLEGRO_ALIGN_CENTER, "Controls");

   al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "[NUMBERS 1-7] toggle client");

   al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "[HOME]        reset x axis");
   al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "[?]           help");
   al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "[ESC]         quit");
   al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "----- mouse controls -----");
   al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "wheel         zoom x axis");
   al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "drag          move x axis");
   al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "[SHIFT] drag  zoom x region");

//   al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "[RIGHT]       zoom in x axis");
//   al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "[LEFT]        zoom out x axis");
//   al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "[CTRL][RIGHT] scroll right");
//   al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "[CTRL][LEFT]  scroll left");
//   al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "[SHFT][RIGHT] scroll faster");
//   al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "[SHFT][LEFT]  scroll faster");
//   al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "[ESC]         quit");


   // draw legend
   xpos = SCREEN_W - 226;
   ypos+=28;

   // framed title bar
   xc = xpos + 110;
   al_draw_filled_rectangle(xc-30, ypos-13, xc+30, ypos-2, palette_color[15+196]);
   al_draw_rectangle(xc-30, ypos-13, xc+30, ypos-2, palette_color[15], 1);
   al_draw_text(font, palette_color[15], xc, ypos-11, ALLEGRO_ALIGN_CENTER, "Legend");

   xpos +=64;

   // iterate players that have data
   for (int i=1; i<8; i++)
      if (lp[i][1])
      {
         // clear and frame row
         al_draw_filled_rectangle(xpos-1, ypos-1, xpos+98, ypos+8, palette_color[15+196]);
         al_draw_rectangle(       xpos-2, ypos-2, xpos+98, ypos+9, palette_color[15], 1);

         int col = players[i].color;
         char tmsg[5];
         if (lp[i][0]) sprintf(tmsg,"on ");
         else
         {
            sprintf(tmsg,"off");
            col = 127; //grey
         }
         al_draw_textf(font, palette_color[col], xpos+1, ypos, 0, "player:%d %s", i, tmsg);
         ypos+=11;
      }
}




void log_client_server_sync_graph(int num_lines)
{
   // server only
   // draws a graph of client server sync
   // uses this log entry:

   // log file has already been loaded into:
   // int log_lines_int[1000000][3];
   // char log_lines[1000000][100];
   // with num_lines and end_pc set

   // player array has already been populated
   // int lp[8][2];

   int max_data = 100000;

   // array of graph data points
   int data[max_data][5];
   // 0  frame_num
   // 1  player
   // 2  dsync
   // 3  fps_chase
   // 4  player active

   int num_data = 0;
   int end_fn = 0;

   //clear data points
   for (int i=0; i<max_data; i++)
      for (int j=0; j<5; j++)
         data[i][j] = 0;

   int pa[8] = {0}; // keep track of when players come active

   // iterate all log lines and build array of data point
   for (int i=0; i<num_lines; i++)
   {
      char tll[200]; // temp log line
      sprintf(tll, "%s", log_lines[i]);


      // find where players became active
      if (log_lines_int[i][0] == 10)
      {
         int p = log_lines_int[i][1];
         int fn = log_lines_int[i][2];
         const char act[10] = "ACTIVE";
         const char ict[10] = "INACTIVE";
         char * ar = strstr(tll, act);
         char * ir = strstr(tll, ict);
         if ((ar) && (!ir))
         {
            //printf("Frame:%d Player:%d --- Active\n", fn, p);
            pa[p] = 1;
            data[num_data][0] = fn;
            data[num_data][1] = p;
            data[num_data][2] = -999;
            data[num_data][3] = -999;
            data[num_data][4] = 1; // player active
            num_data++;
            end_fn = fn;
         }
      }

      if (log_lines_int[i][0] == 30) // stak line
      {
         int p = log_lines_int[i][1];
         int fn = log_lines_int[i][2];

         char res[80];

         get_tag_text(tll, res); // server_sync
         // int sy = atoi(res); not used any more

         get_tag_text(tll, res); // dsync
         double dsy = atof(res);

         get_tag_text(tll, res); // fps_chase
         double fc = atof(res);

         // enter one array data point
         data[num_data][0] = fn;
         data[num_data][1] = p;
         data[num_data][2] = (int) dsy;
         data[num_data][3] = (int) fc;
         data[num_data][4] = pa[p]; // player active

         num_data++;
         end_fn = fn;
         // printf("fn:%d  p:%d sync:%d dsync:%f fps_chase:%f\n", fn, p, sy, dsy, fc);

      }

      if (num_data>max_data)
      {
         printf("number of graph points > %d\n", max_data);
         i=num_lines; // break out of loop
      }

   }

   // all the data is in the array
   num_data--;

   int gquit = 0;
   int redraw = 1;

   // set the graph width and x baseline
   int xbl = 70;                 // x baseline
   int graph_w = SCREEN_W - 70;

   // x axis legend y pos
   int yxl = SCREEN_H - 20;

   int g_stf = 0; // graph start pc
   int g_rng = 0; // frame range of current display, used to set scroll amount

   float x_scale = (float)graph_w / (float)2400;  // start with showing 60 seconds

   while (!gquit)
   {


      int ya = 200;
      int old_test_toggle = test_toggle;
      mdw_toggle(100, ya, 160, 16, 0,0,0,0,   0, 0, 0, 0,  1,0,1,0, test_toggle, "Fixed", "Auto", 15, 15, 7, 7);
      if (old_test_toggle == test_toggle) al_flip_display();
      else redraw = 1;


      if (redraw)
      {
         redraw = 0;
         redraw_log_client_server_sync_graph(num_data, data, graph_w, x_scale, g_stf, g_rng, end_fn, xbl, yxl);
         al_flip_display();
      }


      if (mouse_b[1][0])
      {
         if ((key[ALLEGRO_KEY_LSHIFT][0]) || (key[ALLEGRO_KEY_RSHIFT][0])) // shift drag to set new zoom area
         {
            int mx1 = mouse_x;
            int my1 = mouse_y;
            int mx2 = 0;
            int my2 = 0;
            while (mouse_b[1][0])
            {
               proc_controllers();
               mx2 = mouse_x;
               my2 = mouse_y;
               redraw_log_client_server_sync_graph(num_data, data, graph_w, x_scale, g_stf, g_rng, end_fn, xbl, yxl);
               al_draw_rectangle(mx1, my1, mx2, my2, palette_color[15], 1);
               al_flip_display();
            }
            // convert mouse x position to graph x pos
            int gx1 = g_stf + (mx1-xbl) / x_scale;
            int gx2 = g_stf + (mx2-xbl) / x_scale;

            if (gx1 < 0) gx1 = 0;
            if (gx2 < 0) gx2 = 0;

            if (gx1 > gx2) swap_int(&gx1, &gx2);

            if (gx1 != gx2)
            {
               g_stf = gx1;     // start frame
               g_rng = gx2-gx1; // frame range

               // calc x_scale so that range fits screen width
               x_scale = (float)graph_w / (float)g_rng;
            }
            redraw = 1;
         }
         else // normal mouse drag to slide graph
         {
            int mx = mouse_x;
            int gx = 0;
            while (mouse_b[1][0])
            {
               proc_controllers();
               gx = (mx - mouse_x) / x_scale; // mouse x offset converted to x axis scale
               int temp_g_stf = g_stf+gx;
               if (temp_g_stf < 0) temp_g_stf = 0;
               redraw_log_client_server_sync_graph(num_data, data, graph_w, x_scale, temp_g_stf, g_rng, end_fn, xbl, yxl);
               al_flip_display();
            }
            g_stf += gx; // make the offset permanent
         }
      }

      if (mouse_dz)
      {
         if (mouse_dz > 0)
         {
            mouse_dz = 0;
            int orx = g_stf + (mouse_x-xbl) / x_scale; // get x axis postion under mouse
            x_scale *= 1.1; // change scale
            g_stf = orx - (mouse_x-xbl) / x_scale; // change x axis start frame
            if (g_stf < 0) g_stf = 0;
            redraw = 1;
         }
         if (mouse_dz < 0)
         {
            mouse_dz = 0;
            int orx = g_stf + (mouse_x-xbl) / x_scale; // get x axis postion under mouse
            x_scale *= 0.9;      // change scale
            g_stf = orx - (mouse_x-xbl) / x_scale; // change x axis start frame
            if (g_stf < 0) g_stf = 0;
            redraw = 1;
         }
      }


      proc_controllers();
      int k = key_pressed_ASCII;
      if ((k > 36) && (k < 45)) k+=11; // convert number pad number to regular numbers
      if ((k > 47) && (k < 56)) // numbers 0-7 toggle players
      {
         int p = k-48;
         redraw = 1;
         lp[p][0] = !lp[p][0];
      }

      int scroll_amt = g_rng / 10; // scroll 10% of screen at once
      if (scroll_amt < 1) scroll_amt = 1;

      if (key[ALLEGRO_KEY_SLASH][3])
      {
         help("Client Sync Graph");
         redraw = 1;
      }
      if (key[ALLEGRO_KEY_HOME][3])
      {
         g_stf = 0; // graph start pc
         x_scale = (float)graph_w / (float)2400;  // start with showing 60 seconds
         redraw = 1;
      }
      if (key[ALLEGRO_KEY_ESCAPE][3]) gquit = 1;
   }
}
