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

void log_reason_for_client_quit(int p)
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
                                              ms

                                               );


      add_log_entry_position_text(22, 0, 76, 10, msg, "|", " ");
   }
   add_log_entry_centered_text(22, 0, 76, "", "+", "-");
}




void log_ending_stats()
{
   int p = active_local_player;
   sprintf(msg,"Client %d (%s) ending stats", p, players1[p].hostname);
   add_log_entry_header(22, p, msg, 0);

   sprintf(msg,"total game frames.........[%d]", frame_num);
   add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");
   sprintf(msg,"frame when client joined..[%d]", players1[p].join_frame);
   add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

   if (players1[p].quit_frame == 0) players1[p].quit_frame = frame_num;
   sprintf(msg,"frame when client quit....[%d]", players1[p].quit_frame);
   add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

   log_reason_for_client_quit(p);

   sprintf(msg,"frames client was active..[%d]", players1[p].quit_frame - players1[p].join_frame);
   add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

   sprintf(msg,"frames skipped............[%d]", players1[p].frames_skipped);
   add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

   sprintf(msg,"cdat packets tx'd.........[%d]", players1[p].client_cdat_packets_tx);
   add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

   sprintf(msg,"cdat late to server.......[%d]", players1[p].serr_c_sync_err);
   add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");
   sprintf(msg,"minimum c_sync............[%d]", players1[p].client_game_move_sync_min);
   add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");
   sprintf(msg,"c_sync errors.............[%d]", players1[p].client_game_move_sync_err);
   add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

   sprintf(msg,"stdf received.............[%d]", players1[p].stdf_rx );
   add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

   sprintf(msg,"stdf received on time.....[%d]", players1[p].stdf_on_time );
   add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

   sprintf(msg,"stdf received late........[%d]", players1[p].stdf_late );
   add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

   if (players1[p].stdf_rx > 0)
   {
      sprintf(msg,"percent of late stdf......[%5.2f]", (float)(players1[p].stdf_late*100) / (float)players1[p].stdf_rx);
      add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");
   }
   sprintf(msg,"stdf corrections..........[%d]", players1[p].dif_corr);
   add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

   log_bandwidth_stats(p);

   add_log_entry_sdat_rx_and_game_move_entered(22, p);

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
      if ((players[p].control_method == 2) || (players[p].control_method == 9))
      {
         sprintf(msg,"Player:%d (%s)", p, players1[p].hostname);
         add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

         sprintf(msg,"frame when client joined..[%d]", players1[p].join_frame);
         add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

         if (players1[p].quit_frame == 0) players1[p].quit_frame = frame_num;
         sprintf(msg,"frame when client quit....[%d]", players1[p].quit_frame);
         add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

         log_reason_for_client_quit(p);

         sprintf(msg,"frames client was active..[%d]", players1[p].quit_frame - players1[p].join_frame);
         add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

         sprintf(msg,"cdat late to server.......[%d]", players1[p].client_game_move_sync_err);
         add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

         sprintf(msg,"stdf late.................[%d]", players1[p].stdf_late);
         add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

         sprintf(msg,"stdf corrections..........[%d]", players1[p].dif_corr);
         add_log_entry_position_text(22, p, 76, 10, msg, "|", " ");

         sprintf(msg,"frames skipped............[%d]", players1[p].frames_skipped);
         add_log_entry_position_text(22, 0, 76, 10, msg, "|", " ");

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
      char f1[256];
      char filename[256];
      struct tm *timenow;
      time_t now = time(NULL);
      timenow = localtime(&now);

      //strftime(filename, sizeof(filename), "logs/%Y%m%d-%H%M%S.txt", timenow);
      strftime(f1, sizeof(f1), "logs/%Y%m%d-%H%M%S", timenow);
      sprintf(filename, "%s-[%d][%s].txt", f1, play_level, local_hostname );

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





void add_log_entry_sdat_rx_and_game_move_entered(int type, int player)
{
   int st = players1[player].client_sdat_packets_rx;      // sdat total
   int ss = players1[player].client_sdat_packets_skipped; // sdat skipped
   float sdpc = 0;                                        // % skipped
   if (st != 0) sdpc = (float)ss * 100 / (float)st;       // prevent divide by zero
   sprintf(msg,"sdat packets rx'd.........[%3d]  skipped:[%3d][%4.1f%%]", st, ss, sdpc);
   add_log_entry_position_text(type, player, 76, 10, msg, "|", " ");

   int mt = players1[player].moves_entered + players1[player].moves_skipped; // moves total
   int ms = players1[player].moves_skipped;                                  // moves skipped
   float mspc = 0;                                                           // % skipped
   if (mt != 0) mspc = (float)ms * 100 / (float)mt;                          // prevent divide by zero
   sprintf(msg,"moves entered:............[%3d]  skipped:[%3d][%4.1f%%]", mt, ms, mspc);
   add_log_entry_position_text(type, player, 76, 10, msg, "|", " ");
}

// not used anymore in favor of add_log_entry2
//void add_log_entry(char *txt)
//{
//   strcat(log_msg, txt);
// //  printf(log_msg);
//}

void add_log_entry2(int type, int player, const char *txt)
{
   char tmsg[200];
   sprintf(tmsg, "[%2d][%d][%d]%s", type, player, frame_num, txt);
   // strcat(log_msg, tmsg);
   memcpy(log_msg + log_msg_pos, tmsg, strlen(tmsg));
   log_msg_pos += strlen(tmsg);
   log_msg[log_msg_pos+1] = 0; // NULL terminate

   //sprintf(log_msg, "%s", txt);
   //printf("%s", tmsg);
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



int fill_filename_array(ALLEGRO_FS_ENTRY *fs, void * extra)
{
   if (num_filenames > 999) return 0; // only get 1000 max
   filenames[num_filenames] = al_create_fs_entry(al_get_fs_entry_name(fs));
   num_filenames++;
   return ALLEGRO_FOR_EACH_FS_ENTRY_OK;
}








void log_bandwidth_graph(int num_lines, int end_pc);
int lp[8][2];




int log_file_viewer(int type)
{
   int line_mode = 1;
   int line_pos = 0;

   char fname[1024];
   FILE *filepntr;
   char buff[200];
   char buff2[80];
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
      strcpy (log_lines[num_lines], buff);
      num_lines++;
      //printf("num_lines:%d\n", num_lines);

   }
   fclose(filepntr);
   num_lines--;

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

   tags[27][0] = 1; tags[27][1] = 13; tags[27][3] = 88; sprintf(ctags[27], "stdf"); // stdf         (X) [CS]
   tags[28][0] = 0; tags[28][1] = 1;  tags[28][3] = 80; sprintf(ctags[28], "stdp"); // stdf piece   (P) [CS]
   tags[29][0] = 0; tags[29][1] = 7;  tags[29][3] = 87; sprintf(ctags[29], "stdw"); // stdf when    (W) [C]
   tags[31][0] = 0; tags[31][1] = 15; tags[31][3] = 68; sprintf(ctags[31], "dif1"); // show diff1   (D) [C]
   tags[32][0] = 0; tags[32][1] = 15; tags[32][3] = 70; sprintf(ctags[32], "dif2"); // show diff2   (F) [C]
   tags[35][0] = 1; tags[35][1] = 3;  tags[35][3] = 67; sprintf(ctags[35], "cdat"); // cdat         (C) [CS]
   tags[37][0] = 1; tags[37][1] = 9;  tags[37][3] = 83; sprintf(ctags[37], "sdat"); // sdat         (S) [CS]
   tags[38][0] = 0; tags[38][1] = 6;  tags[38][3] = 71; sprintf(ctags[38], "move"); // game move    (G) [C]
   tags[39][0] = 0; tags[39][1] = 6;  tags[39][3] = 75; sprintf(ctags[39], "sdak"); // sdak         (K) [S]

   tags[99][0] = 1; tags[99][1] = 10; // bad tag


   // find and process tags
   for (int i=0; i<num_lines; i++)
   {
      int bad_tags = 0;
      // get first tag - (type) in the format "[xx]"
      char * pch1 = strchr(log_lines[i], '[');
      char * pch2 = strchr(log_lines[i], ']');
      int p1 = pch1-log_lines[i];
      int p2 = pch2-log_lines[i];
      if ((p1 == 0) && (p2 == 3))
      {
         buff2[0] = log_lines[i][1];
         buff2[1] = log_lines[i][2];
         buff2[2] = 0;
         log_lines_int[i][0] = atoi(buff2); // type
         chop_first_x_char(log_lines[i], 4);
         tags[ log_lines_int[i][0] ]  [2]  ++; // inc number of this tag

      }
      else bad_tags = 1;
      if (!bad_tags)
      {
         // get second tag - (player) in the format "[x]"
         char * pch1 = strchr(log_lines[i], '[');
         char * pch2 = strchr(log_lines[i], ']');
         int p1 = pch1-log_lines[i];
         int p2 = pch2-log_lines[i];
         if ((p1 == 0) && (p2 == 2))
         {
            buff2[0] = log_lines[i][1];
            buff2[1] = 0;
            log_lines_int[i][1] = atoi(buff2); // player
            chop_first_x_char(log_lines[i], 3);
         }
         else bad_tags = 1;
      }
      if (!bad_tags)
      {
         // get third tag - (frame_num) in the format "[xxx..]"
         char * pch1 = strchr(log_lines[i], '[');
         char * pch2 = strchr(log_lines[i], ']');
         int p1 = pch1-log_lines[i];
         int p2 = pch2-log_lines[i];
         if ((p1 == 0) && (p2 < 8))
         {
            for(int j=0; j<p2; j++)
               buff2[j] = log_lines[i][j+1];
            buff2[p2] = 0;
            log_lines_int[i][2] = atoi(buff2); // frame_num
            chop_first_x_char(log_lines[i], p2+1);
         }
         else bad_tags = 1;
      }
      if (bad_tags)
      {
         log_lines_int[i][0] = 99;
         tags[99][2]++; // inc number of this tag
      }
   }


   // get start and end frame_nums
   int start_pc = log_lines_int[0][2];
   int end_pc = 0;
   for (int i=0; i<num_lines; i++)
      if (log_lines_int[i][2] > end_pc) end_pc = log_lines_int[i][2];

   int pos = 0; // the top frame_num line on the screen
   int quit = 0;
   //int redraw = 1;


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


   int original_display_transform_double = display_transform_double;
   display_transform_double = 1;
   set_display_transform();


   while (!quit)
   {
      if (line_mode)
      {
         if (line_pos < 0) line_pos = 0;
         if (line_pos > num_lines) line_pos = num_lines;
         pos = log_lines_int[line_pos][2];
      }
      else // pascount mode
      {
         if (pos < start_pc) pos = start_pc;
         if (pos > end_pc) pos = end_pc;
      }

      al_set_target_backbuffer(display);
      al_flip_display();
      al_clear_to_color(al_map_rgb(0,0,0));

      int ty = 0;
      int color = 15;
      int first_line = 0;

      if (line_mode) first_line = line_pos;
      else // pascount mode
      {
         // find index of first line that is equal to or greater than current frame_num
         for (int i=0; i<num_lines; i++)
            if (log_lines_int[i][2] >= pos)
            {
               first_line = i;
               break;
            }
         line_pos = first_line;
      }

      int xpos = 760;


      int ly = 20;
      sprintf(msg, "Log file.........[%s]", fnam);
      al_draw_text(font, palette_color[15], xpos, ly+=8, 0, msg);

      sprintf(msg, "Number of lines..[%d]", num_lines);
      al_draw_text(font, palette_color[15],xpos, ly+=8, 0, msg);

      sprintf(msg, "Starting frame...[%d]", start_pc);
      al_draw_text(font, palette_color[15],xpos, ly+=8, 0, msg);

      sprintf(msg, "Ending frame.....[%d]", end_pc);
      al_draw_text(font, palette_color[15],xpos, ly+=8, 0, msg);

      ly+=4;
      sprintf(msg, "Current line.....[%d]", first_line);
      al_draw_text(font, palette_color[15],xpos, ly+=8, 0, msg);

      sprintf(msg, "Current frame....[%d]", pos);
      al_draw_text(font, palette_color[15],xpos, ly+=8, 0, msg);

      ly+=8;

      if (line_mode) sprintf(msg, "Line Mode");
      else sprintf(msg, "frame_num Mode");
      al_draw_text(font, palette_color[15],xpos, ly+=8, 0, msg);


      // show tag labels
      int lpos = 100;
      for (int i=23; i<40; i++)
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
            tags[i][4] = lpos; lpos+=8; // set the ypos
            sprintf(msg, "%c %s %s num:[%d]", tags[i][3], ctags[i], tmsg, tags[i][2]);
            al_draw_text(font, palette_color[col], xpos, lpos, 0, msg);
         }
      }

      // show players
      lpos+=8;
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
         al_draw_textf(font, palette_color[col], xpos, lpos+=8, 0, "%d plyr:%d %s num:[%d]", i, i, tmsg, lp[i][1]);
      }

      // draw all the lines
      int i = first_line;
      int done = 0;
      while (!done)
      {
         int type = log_lines_int[i][0];
         int p    = log_lines_int[i][1];
         int pc   = log_lines_int[i][2];

         if (type == 99) // bad tags on this line
         {
            sprintf(msg, "i[%d] t[%d] p[%d] pc[%d] (bad tags)- %s", i, type, p, pc, log_lines[i]);
            al_draw_text(font, palette_color[color], 0, ty+=8, 0, msg);
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
               al_draw_text(font, palette_color[color], 0, ty+=8, 0, msg);
         }
         if (++i >= num_lines) done = 1; // no more lines
         if (ty > SCREEN_H - 20) done = 1; // no more screen
      }
      al_flip_display();


      int k = proc_controllers();
      if ((k > 26) && (k < 35)) // numbers 0-7 toggle players
      {
         int p = k-27;
         //printf("p:%d\n", p);
         lp[p][0] = !lp[p][0];
      }


      if ((k > 0) && (k < 27)) // letters toggle tags
      {
         //redraw = 1;
         k += 64; // convert to ascii
         //printf("%c\n", k);
         for (int i=23; i<40; i++)
            if (tags[i][3] == k) tags[i][0] = !tags[i][0]; // toggle tag on/off
      }

      if (key[ALLEGRO_KEY_L])
      {
         while (key[ALLEGRO_KEY_L]) proc_controllers();
         line_mode =! line_mode;
      }

      if (key[ALLEGRO_KEY_UP])
      {
         while (key[ALLEGRO_KEY_UP]) proc_controllers();
         if (line_mode) line_pos--;
         else pos--;
      }
      if (key[ALLEGRO_KEY_DOWN])
      {
         while (key[ALLEGRO_KEY_DOWN]) proc_controllers();
         if (line_mode) line_pos++;
         else pos++;
      }
      if (key[ALLEGRO_KEY_PGUP])
      {
         while (key[ALLEGRO_KEY_PGUP]) proc_controllers();
         if (line_mode)
         {
            if ((key[ALLEGRO_KEY_LCTRL]) || (key[ALLEGRO_KEY_RCTRL])) line_pos -= 10000;
            else if ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT])) line_pos -= 1000;
            else line_pos-=100;
         }
         else
         {
            if ((key[ALLEGRO_KEY_LCTRL]) || (key[ALLEGRO_KEY_RCTRL])) pos -= 10000;
            else if ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT])) pos -= 1000;
            else pos-=100;
         }
      }
      if (key[ALLEGRO_KEY_PGDN])
      {
         while (key[ALLEGRO_KEY_PGDN]) proc_controllers();
         if (line_mode)
         {
            if ((key[ALLEGRO_KEY_LCTRL]) || (key[ALLEGRO_KEY_RCTRL])) line_pos += 10000;
            else if ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT])) line_pos += 1000;
            else line_pos+=100;
         }
         else
         {
            if ((key[ALLEGRO_KEY_LCTRL]) || (key[ALLEGRO_KEY_RCTRL])) pos += 10000;
            else if ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT])) pos += 1000;
            else pos+=100;
         }
      }
      if (key[ALLEGRO_KEY_HOME])
      {
         while (key[ALLEGRO_KEY_HOME]) proc_controllers();
         if (line_mode) line_pos = 0;
         else pos = 0;
      }
      if (key[ALLEGRO_KEY_END])
      {
         while (key[ALLEGRO_KEY_END]) proc_controllers();
         if (line_mode) line_pos = num_lines;
         else pos = end_pc;
      }

      if (key[ALLEGRO_KEY_DELETE])
      {
         while (key[ALLEGRO_KEY_DELETE]) proc_controllers();

         log_bandwidth_graph(num_lines, end_pc);
      }
      if (key[ALLEGRO_KEY_ESCAPE])
      {
         while (key[ALLEGRO_KEY_ESCAPE]) proc_controllers();
         quit = 1;
      }
   } // end of log file viewer

   display_transform_double = original_display_transform_double;
   set_display_transform();
   return 0;
}


void log_bandwidth_graph(int num_lines, int end_pc)
{
   // draws a bandwidth graph
   // uses this log entry:
   // [23][0][1360]bandwidth (B/s) TX cur:[ 1878] max:[ 5060] RX cur:[  480] max:[  514]

   // log file has already been loaded into:
   // int log_lines_int[1000000][3];
   // char log_lines[1000000][100];
   // with num_lines and end_pc set

   // player array has already been populated
   // int lp[8][2];



   // build array of data points
   char buff2[80];

   // array of graph data points
   int data[10000][4];
   // 0 = frame_num
   // 1 = player
   // 2  tx b
   // 3  rx b

   int num_data = 0;

   //clear data points
   for (int i=0; i<10000; i++)
      for (int j=0; j<4; j++)
         data[i][j] = 0;

   // iterate all log lines and build array of data point
   for (int i=0; i<num_lines; i++)
   {
      if (log_lines_int[i][0] == 23)
      {
         int p = log_lines_int[i][1];
         int fn = log_lines_int[i][2];
         int tx = 0, rx = 0;


         char tll[200]; // temp log line
         sprintf(tll, "%s", log_lines[i]);


         // get first tag - tx_cur
         char * pch1 = strchr(tll, '[');
         char * pch2 = strchr(tll, ']');
         int p1 = pch1-tll;
         int p2 = pch2-tll;
         if (p2 - p1 < 8)
         {
             for(int j=0; j<p2; j++)
                buff2[j] = tll[j+p1+1];
             buff2[p2] = 0;
             tx = atoi(buff2);
             chop_first_x_char(tll, p2+1);
         }



         // get second tag and discard - tx_max
         pch1 = strchr(tll, '[');
         pch2 = strchr(tll, ']');
         p1 = pch1-tll;
         p2 = pch2-tll;
         if (p2 - p1 < 8)
         {
            for(int j=0; j<p2; j++)
               buff2[j] = tll[j+p1+1];
            buff2[p2] = 0;
            //tx_max = atoi(buff2); // ignore
            chop_first_x_char(tll, p2+1);
         }


         // get third tag  - rx_cur
         pch1 = strchr(tll, '[');
         pch2 = strchr(tll, ']');
         p1 = pch1-tll;
         p2 = pch2-tll;
         if (p2 - p1 < 8)
         {
            for(int j=0; j<p2; j++)
               buff2[j] = tll[j+p1+1];
            buff2[p2] = 0;
            rx = atoi(buff2);
            chop_first_x_char(tll, p2+1);
         }

         // enter one array data point
         data[num_data][0] = fn;
         data[num_data][1] = p;
         data[num_data][2] = tx;
         data[num_data][3] = rx;
         num_data++;

         //printf("pc:%d  p:%d tx:%d  rx:%d\n", pc, p, tx, rx);
      }
   }
   // all the data is in the array

   int gquit = 0;
   int redraw = 1;

   // set the graph width, height and baseline

   int xbl = 70;            // x baseline
   int ybl = SCREEN_H - 20; // y baseline

   int graph_w = SCREEN_W - 70;
   int graph_h = SCREEN_H - 20;

   int gs_pc = 0; // graph start pc

   int rangef; // frame range of current display, used to set scroll amount

   float y_scale = 1;
   float p2g = 1;
   int old_ix, old_ity, old_iry;

   while (!gquit)
   {
      if (redraw)
      {
         al_set_target_backbuffer(display);
         al_clear_to_color(al_map_rgb(0,0,0));

         if (redraw == 1) // auto set scale
         {
            // find the max value
            int max = 0;
            for (int i=0; i<num_data; i++)
            {
               int p = data[i][1];
               if (lp[p][0]) // only if this player is not hidden
               {
                  if (data[i][2] > max) max = data[i][2];
                  if (data[i][3] > max) max = data[i][3];
               }
            }
            // set y scale based on graph height and largest value
            y_scale = (float)(graph_h-40) / (float)max;

            // set x scale based on graph width and end frame
            p2g = (float)graph_w / (float)end_pc;  // frame_num to graph scaler
         }






         // label the x axis and draw gridlines
         // first of all, what is the range of data in frames?
         // we will use this to determine the units (frames, seconds, minutes)
         int maxf = 0;
         for (float i=gs_pc; (int)((i-gs_pc)*p2g) < graph_w; i++) if (i > maxf) maxf = i;
         rangef = maxf-gs_pc;

         //al_draw_textf(font, palette_color[13], 100, ybl-20, 0, "p2g:%f, mn:%d, mx:%d range:%d", p2g, gs_pc, maxf, rangef);

         // now choose units
         if (rangef < 400) // frames
         {
            al_draw_text(font, palette_color[13], xbl+graph_w/2, ybl+12, ALLEGRO_ALIGN_CENTER, "time (frames)");
            int div = 1;
            // get max label text size in pixels
            sprintf(msg, "%d ", maxf / div);
            int tm = strlen(msg) * 8;
            int spi = (int) ((float)tm / p2g); // scale to graph size
            int sp2 = ((spi/div) * div) + div; // snap to div size
            // draw legend and gridline every sp2 frames
            for (float i=gs_pc; (int)((i-gs_pc)*p2g) < graph_w; i+= sp2)
            {
               int hx = xbl + (int)((i-gs_pc)*p2g);
               al_draw_textf(font, palette_color[13], hx, ybl+4, ALLEGRO_ALIGN_CENTER, "%d", (int)(i));
               al_draw_line(hx, 0, hx, ybl, palette_color[13+96], 1);
            }
         }

         if ((rangef >= 400) && (rangef < 20000)) // seconds
         {
            al_draw_text(font, palette_color[13], xbl+graph_w/2, ybl+12, ALLEGRO_ALIGN_CENTER, "time (seconds)");
            int div = 40;
            // get max label text size in pixels
            sprintf(msg, "%d ", maxf / div);
            int tm = strlen(msg) * 8;
            int spi = (int) ((float)tm / p2g); // scale to graph size
            int sp2 = ((spi/div) * div) + div; // snap to div size

             // only allowed values are 40, 80, 200, 400, 800
            if ((sp2 > 80) && (sp2 < 200)) sp2 = 200;
            if ((sp2 > 200) && (sp2 < 400)) sp2 = 400;
            if ((sp2 > 400) && (sp2 < 800)) sp2 = 800;
            if ((sp2 > 800) && (sp2 < 1600)) sp2 = 1600;

            // draw legend every sp2 frames
            for (float i=gs_pc; (int)((i-gs_pc)*p2g) < graph_w; i+= sp2)
            {
               int hx = xbl + (int)((i-gs_pc)*p2g);
               al_draw_textf(font, palette_color[13], hx, ybl+4, ALLEGRO_ALIGN_CENTER, "%d", (int)(i/div));
               //al_draw_line(hx, 0, hx, ybl+4, palette_color[13+32], 1);
            }
            // draw scale lines every second
            for (float i=gs_pc; (int)((i-gs_pc)*p2g) < graph_w; i+= 40)
            {
               int hx = xbl + (int)((i-gs_pc)*p2g);
               al_draw_line(hx, 0, hx, ybl+2, palette_color[13+96], 1);
            }
            // draw scale line every 10 seconds
            for (float i=gs_pc; (int)((i-gs_pc)*p2g) < graph_w; i+= 400)
            {
               int hx = xbl + (int)((i-gs_pc)*p2g);
               al_draw_line(hx, 0, hx, ybl+2, palette_color[13+32], 2);
            }
         }

         if (rangef >= 20000) // minutes
         {
            al_draw_text(font, palette_color[13], xbl+graph_w/2, ybl+12, ALLEGRO_ALIGN_CENTER, "time (minutes)");
            int div = 2400;
            // get max label text size in pixels
            sprintf(msg, "%d ", maxf / div);
            int tm = strlen(msg) * 8;
            int spi = (int) ((float)tm / p2g); // scale to graph size
            int sp2 = ((spi/div) * div) + div; // snap to div size
            // draw legend and scale line every sp2 frames
            for (float i=gs_pc; (int)((i-gs_pc)*p2g) < graph_w; i+= sp2)
            {
               int hx = xbl + (int)((i-gs_pc)*p2g);
               al_draw_textf(font, palette_color[13], hx, ybl+4, ALLEGRO_ALIGN_CENTER, "%d", (int)(i/div));
               al_draw_line(hx, 0, hx, ybl, palette_color[13+32], 2);
            }
            // draw x scale lines every 10 seconds
            for (float i=gs_pc; (int)((i-gs_pc)*p2g) < graph_w; i+= 400)
            {
               int hx = xbl + (int)((i-gs_pc)*p2g);
               al_draw_line(hx, 0, hx, ybl, palette_color[13+32], 1);
            }
         }




         // label the y axis and draw gridlines
         // first of all, what is the max B/s
         int maxb = 0;
         for (float i=0; i*y_scale < graph_h; i+= 1000)
            if (i > maxb) maxb = i;

         // get label spacing
         int s = (int) (10 / y_scale);
         int sp2 = ((s/100) * 100) + 100; // snap to div size
         if ((sp2 > 200) && (sp2 < 500)) sp2 = 500;
         if ((sp2 > 500) && (sp2 < 1000)) sp2 = 1000;
         if ((sp2 > 1000) && (sp2 < 2000)) sp2 = 2000;
         if ((sp2 > 2000) && (sp2 < 5000)) sp2 = 5000;

         //al_draw_textf(font, palette_color[13], 100, ybl-20, 0, "y_scale:%f, s:%d sp2:%d", y_scale, s, sp2 );


         for (float i=0; i*y_scale < graph_h; i+= sp2)
         {
            int y_pos = ybl - (int)(i*y_scale);
            al_draw_line(xbl, y_pos, xbl+graph_w, y_pos, palette_color[11+96], 1);

            if (i<1000)        sprintf(msg, "%5.0fB/s", i);
            else if (i<10000)  sprintf(msg, "%3.2fkB/s", i/1000);
            else if (i<100000) sprintf(msg, "%3.1fkB/s", i/1000);
            else if (i>999999) sprintf(msg, "%4.0fkB/s", i/1000);
            al_draw_text(font, palette_color[11], 0, y_pos-4, 0, msg);
         }

         // draw major grid lines every 1kB/s
         for (float i=0; i*y_scale < graph_h; i+= 1000)
         {
            int y_pos = ybl - (int)(i*y_scale);
            al_draw_line(xbl, y_pos, xbl+graph_w, y_pos, palette_color[11+96], 2);
         }


         // draw the data one player at a time
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
                  int ty = data[i][3];
                  int col = players[p].color;
                  if (dp == p)
                  {
                     int ix = xbl + (int) ( (float)(pc-gs_pc) * p2g);
                     int ity = ybl - (int) ( (float)tx * y_scale);
                     int iry = ybl - (int) ( (float)ty * y_scale);
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
         al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "[UP]          zoom in y axis");
         al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "[DOWN]        zoom out y axis");
         al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "[RIGHT]       zoom in x axis");
         al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "[LEFT]        zoom out x axis");
         al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "[CTRL][RIGHT] scroll right");
         al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "[CTRL][LEFT]  scroll left");
         al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "[SHFT][RIGHT] scroll faster");
         al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "[SHFT][LEFT]  scroll faster");
         al_draw_text(font, palette_color[15], xpos, ypos+=8, 0, "[ESC]         quit");


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
         al_flip_display();
         redraw = 0;
      } // end of redraw

      int k = proc_controllers();
      if ((k > 26) && (k < 35)) // numbers 0-7 toggle players
      {
         redraw = 1;
         int p = k-27;
         //printf("p:%d\n", p);
         lp[p][0] = !lp[p][0];
      }

      int scroll_amt = rangef / 10; // scroll 10% of screen at once
      if (scroll_amt < 1) scroll_amt = 1;

      if (key[ALLEGRO_KEY_RIGHT])
      {
         while (key[ALLEGRO_KEY_RIGHT]) proc_controllers();
         if ((key[ALLEGRO_KEY_LCTRL]) || (key[ALLEGRO_KEY_RCTRL]))
         {
            gs_pc += scroll_amt;
            if (gs_pc > end_pc) gs_pc = end_pc;
         }
         else if ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT]))
         {
            gs_pc += scroll_amt * 10;
            if (gs_pc > end_pc) gs_pc = end_pc;
         }
         else p2g *= 1.1;
         redraw = 2;
      }
      if (key[ALLEGRO_KEY_LEFT])
      {
         while (key[ALLEGRO_KEY_LEFT]) proc_controllers();
         if ((key[ALLEGRO_KEY_LCTRL]) || (key[ALLEGRO_KEY_RCTRL]))
         {
            gs_pc -= scroll_amt;
            if (gs_pc < 0) gs_pc = 0;
         }
         else if ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT]))
         {
            gs_pc -= scroll_amt * 10;
            if (gs_pc < 0) gs_pc = 0;
         }
         else p2g *= .9;
         redraw = 2;
      }
      if (key[ALLEGRO_KEY_UP])
      {
         while (key[ALLEGRO_KEY_UP]) proc_controllers();
         y_scale *= 1.1;
         redraw = 2;
      }
      if (key[ALLEGRO_KEY_DOWN])
      {
         while (key[ALLEGRO_KEY_DOWN]) proc_controllers();
         y_scale *= .9;
         redraw = 2;
      }

      if (key[ALLEGRO_KEY_ESCAPE])
      {
         while (key[ALLEGRO_KEY_ESCAPE]) proc_controllers();
         gquit = 1;
      }
   }
}




