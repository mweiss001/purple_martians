// zlog.cpp
#include "pm.h"


void log_bandwidth_stats(int p)
{
   sprintf(msg,"total tx bytes............[%d]", players1[p].tx_total_bytes);
   add_log_entry_position_text(22, 0, 76, 10, msg, (char*)"|", (char*)" ");

   sprintf(msg,"max tx bytes per frame....[%d]", players1[p].tx_max_bytes_per_frame);
   add_log_entry_position_text(22, 0, 76, 10, msg, (char*)"|", (char*)" ");

   sprintf(msg,"avg tx bytes per frame....[%d]", players1[p].tx_total_bytes / passcount);
   add_log_entry_position_text(22, 0, 76, 10, msg, (char*)"|", (char*)" ");

   sprintf(msg,"max rx bytes per second...[%d]", players1[p].tx_max_bytes_per_tally);
   add_log_entry_position_text(22, 0, 76, 10, msg, (char*)"|", (char*)" ");

   sprintf(msg,"avg tx bytes per sec......[%d]", (players1[p].tx_total_bytes *40)/ passcount);
   add_log_entry_position_text(22, 0, 76, 10, msg, (char*)"|", (char*)" ");

   sprintf(msg,"total tx packets..........[%d]", players1[p].tx_total_packets);
   add_log_entry_position_text(22, 0, 76, 10, msg, (char*)"|", (char*)" ");

   sprintf(msg,"max tx packets per frame..[%d]", players1[p].tx_max_packets_per_frame);
   add_log_entry_position_text(22, 0, 76, 10, msg, (char*)"|", (char*)" ");

   sprintf(msg,"max tx packets per second.[%d]", players1[p].tx_max_packets_per_tally);
   add_log_entry_position_text(22, 0, 76, 10, msg, (char*)"|", (char*)" ");


   sprintf(msg,"total rx bytes............[%d]", players1[p].rx_total_bytes);
   add_log_entry_position_text(22, 0, 76, 10, msg, (char*)"|", (char*)" ");

   sprintf(msg,"max rx bytes per frame....[%d]", players1[p].rx_max_bytes_per_frame);
   add_log_entry_position_text(22, 0, 76, 10, msg, (char*)"|", (char*)" ");

   sprintf(msg,"avg rx bytes per frame....[%d]", players1[p].rx_total_bytes / passcount);
   add_log_entry_position_text(22, 0, 76, 10, msg, (char*)"|", (char*)" ");

   sprintf(msg,"max rx bytes per second...[%d]", players1[p].rx_max_bytes_per_tally);
   add_log_entry_position_text(22, 0, 76, 10, msg, (char*)"|", (char*)" ");

   sprintf(msg,"avg rx bytes per sec......[%d]", (players1[p].rx_total_bytes *40)/ passcount);
   add_log_entry_position_text(22, 0, 76, 10, msg, (char*)"|", (char*)" ");

   sprintf(msg,"total rx packets..........[%d]", players1[p].rx_total_packets);
   add_log_entry_position_text(22, 0, 76, 10, msg, (char*)"|", (char*)" ");

   sprintf(msg,"max rx packets per frame..[%d]", players1[p].rx_max_packets_per_frame);
   add_log_entry_position_text(22, 0, 76, 10, msg, (char*)"|", (char*)" ");

   sprintf(msg,"max rx packets per second.[%d]", players1[p].rx_max_packets_per_tally);
   add_log_entry_position_text(22, 0, 76, 10, msg, (char*)"|", (char*)" ");
}

void log_reason_for_client_quit(int p)
{
   char tmsg[80];
   sprintf(tmsg,"unknown");
   int r = players1[p].quit_reason;
   if (r == 64) sprintf(tmsg,"player quit game with ESC");
   if (r == 65) sprintf(tmsg,"player quit with F12");
   if (r == 70) sprintf(tmsg,"server drop (server sync > 100)");
   if (r == 71) sprintf(tmsg,"server drop (no sdak for 100 frames)");
   if (r == 74) sprintf(tmsg,"client never became active");
   if (r == 75) sprintf(tmsg,"client lost server connection");
   if (r == 80) sprintf(tmsg,"level done");
   if (r == 90) sprintf(tmsg,"local client quit");
   if (r == 91) sprintf(tmsg,"local server quit");
   if (r == 92) sprintf(tmsg,"remote server quit");
   sprintf(msg,"reason for quit...........[%s]", tmsg);
   add_log_entry_position_text(22, 0, 76, 10, msg, (char*)"|", (char*)" ");
}


void log_player_array()
{
   add_log_entry_header(22, 0, (char*) "Player Array", 0);
   add_log_entry_position_text(22, 0, 76, 10, (char*)"[p][wh][a][co][m]", (char*)"|", (char*)" ");
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


      add_log_entry_position_text(22, p, 76, 10, msg, (char*)"|", (char*)" ");
   }
   add_log_entry_centered_text(22, 0, 76, (char*)"", (char*)"+", (char*)"-");
}




void log_ending_stats()
{
   int p = active_local_player;
   sprintf(msg,"Client %d (%s) ending stats", p, players1[p].hostname);
   add_log_entry_header(22, 0, msg, 0);

   add_log_entry_sdat_rx_and_game_move_entered(22, p);
   add_log_entry_centered_text(22, p, 76, (char*)"", (char*)"+", (char*)"-");
   sprintf(msg,"total game frames.........[%d]", passcount);
   add_log_entry_position_text(22, p, 76, 10, msg, (char*)"|",(char*) " ");
   sprintf(msg,"frame when client joined..[%d]", players1[p].join_frame);
   add_log_entry_position_text(22, p, 76, 10, msg, (char*)"|", (char*)" ");

   if (players1[p].quit_frame == 0) players1[p].quit_frame = passcount;
   sprintf(msg,"frame when client quit....[%d]", players1[p].quit_frame);
   add_log_entry_position_text(22, p, 76, 10, msg, (char*)"|", (char*)" ");

   log_reason_for_client_quit(p);

   sprintf(msg,"frames client was active..[%d]", players1[p].quit_frame - players1[p].join_frame);
   add_log_entry_position_text(22, p, 76, 10, msg, (char*)"|", (char*)" ");

   sprintf(msg,"frames skipped............[%d]", players1[p].frames_skipped);
   add_log_entry_position_text(22, 0, 76, 10, msg, (char*)"|", (char*)" ");

   sprintf(msg,"cdat packets tx'd.........[%d]", players1[p].cdat_packets_tx);
   add_log_entry_position_text(22, p, 76, 10, msg, (char*)"|", (char*)" ");

   sprintf(msg,"cdat late to server.......[%d]", players1[p].serr_c_sync_err);
   add_log_entry_position_text(22, p, 76, 10, msg, (char*)"|", (char*)" ");
   sprintf(msg,"minimum c_sync............[%d]", players1[p].c_sync_min);
   add_log_entry_position_text(22, p, 76, 10, msg, (char*)"|", (char*)" ");
   sprintf(msg,"c_sync errors.............[%d]", players1[p].c_sync_err);
   add_log_entry_position_text(22, p, 76, 10, msg, (char*)"|", (char*)" ");

   sprintf(msg,"chdf received.............[%d]", players1[p].chdf_rx );
   add_log_entry_position_text(22, p, 76, 10, msg, (char*)"|", (char*)" ");

   sprintf(msg,"chdf received on time.....[%d]", players1[p].chdf_on_time );
   add_log_entry_position_text(22, p, 76, 10, msg, (char*)"|", (char*)" ");

   sprintf(msg,"chdf received late........[%d]", players1[p].chdf_late );
   add_log_entry_position_text(22, p, 76, 10, msg, (char*)"|", (char*)" ");

   if (players1[p].chdf_rx > 0)
   {
      sprintf(msg,"percent of late chdf......[%d]", (players1[p].chdf_late*100) / players1[p].chdf_rx);
      add_log_entry_position_text(22, p, 76, 10, msg, (char*)"|", (char*)" ");
   }

   sprintf(msg,"chdf corrections..........[%d]", players1[p].dif_corr);
   add_log_entry_position_text(22, p, 76, 10, msg, (char*)"|", (char*)" ");

   log_bandwidth_stats(p);

   add_log_entry_centered_text(22, p, 76, (char*)"", (char*)"+", (char*)"-");


   // lets get more date here temporarily
   log_ending_stats_server();

}




void log_ending_stats_server()
{
   sprintf(msg,"Server (%s) ending stats", local_hostname);
   add_log_entry_header(22, 0, msg, 0);

   add_log_entry_centered_text(22, 0, 76, (char*)"", (char*)"+", (char*)"-");

   sprintf(msg,"level.....................[%d]", play_level);
   add_log_entry_position_text(22, 0, 76, 10, msg, (char*)"|", (char*)" ");

   sprintf(msg,"total frames..............[%d]", passcount);
   add_log_entry_position_text(22, 0, 76, 10, msg, (char*)"|", (char*)" ");

   sprintf(msg,"total moves...............[%d]", game_move_entry_pos);
   add_log_entry_position_text(22, 0, 76, 10, msg, (char*)"|", (char*)" ");

   sprintf(msg,"total time (seconds)......[%d]", passcount/40);
   add_log_entry_position_text(22, 0, 76, 10, msg, (char*)"|", (char*)" ");

   sprintf(msg,"total time (minutes)......[%d]", passcount/40/60);
   add_log_entry_position_text(22, 0, 76, 10, msg, (char*)"|", (char*)" ");

   sprintf(msg,"server frames skipped.. ..[%d]", players1[0].frames_skipped);
   add_log_entry_position_text(22, 0, 76, 10, msg, (char*)"|", (char*)" ");

   log_bandwidth_stats(0);

   add_log_entry_centered_text(22, 0, 76, (char*)"", (char*)"+", (char*)"-");

   for (int p=1; p<NUM_PLAYERS; p++)
   {
      if ((players[p].control_method == 2) || (players[p].control_method == 9))
      {
         sprintf(msg,"Player:%d (%s)", p, players1[p].hostname);
         add_log_entry_position_text(22, p, 76, 10, msg, (char*)"|", (char*)" ");

         sprintf(msg,"frame when client joined..[%d]", players1[p].join_frame);
         add_log_entry_position_text(22, p, 76, 10, msg, (char*)"|", (char*)" ");

         if (players1[p].quit_frame == 0) players1[p].quit_frame = passcount;
         sprintf(msg,"frame when client quit....[%d]", players1[p].quit_frame);
         add_log_entry_position_text(22, p, 76, 10, msg, (char*)"|", (char*)" ");

         log_reason_for_client_quit(p);

         sprintf(msg,"frames client was active..[%d]", players1[p].quit_frame - players1[p].join_frame);
         add_log_entry_position_text(22, p, 76, 10, msg, (char*)"|", (char*)" ");

         sprintf(msg,"cdat late to server.......[%d]", players1[p].c_sync_err);
         add_log_entry_position_text(22, p, 76, 10, msg, (char*)"|", (char*)" ");

         sprintf(msg,"chdf late.................[%d]", players1[p].chdf_late);
         add_log_entry_position_text(22, p, 76, 10, msg, (char*)"|", (char*)" ");

         sprintf(msg,"chdf corrections..........[%d]", players1[p].dif_corr);
         add_log_entry_position_text(22, p, 76, 10, msg, (char*)"|", (char*)" ");

         sprintf(msg,"frames skipped............[%d]", players1[p].frames_skipped);
         add_log_entry_position_text(22, 0, 76, 10, msg, (char*)"|", (char*)" ");

         log_bandwidth_stats(p);

         add_log_entry_centered_text(22, p, 76,(char*) "", (char*)"+", (char*)"-");


      }
   }

   log_player_array();

}

void save_log_file(void)
{
   if (L_LOGGING)
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
      log_msg[0] = 0;
      log_msg_pos = 0;
   }
}


void add_log_entry_sdat_rx_and_game_move_entered(int type, int player)
{
   add_log_entry_centered_text(type, player, 76, (char*)"", (char*)"+", (char*)"-");

   int st = players1[player].sdat_total;
   int ss = players1[player].sdat_skipped;
   float sdpc=0;                 // % skipped
   if (st != 0) // prevent divide by zero
   {
      sdpc = (float)ss * 100 / (float)st;
   }
   sprintf(msg,"sdat packets rx'd:[%3d]  skipped:[%3d][%4.1f%%%%]", st, ss, sdpc);
   add_log_entry_position_text(type, player, 76, 10, msg, (char*)"|", (char*)" ");


   int mt = players1[player].moves_entered + players1[player].moves_skipped; // moves total
   int ms = players1[player].moves_skipped;                             // moves skipped
   float mspc=0;                 // % skipped
   if (mt != 0) // prevent divide by zero
   {
      mspc = (float)ms * 100 / (float)mt;
   }
   sprintf(msg,"moves entered:    [%3d]  skipped:[%3d][%4.1f%%%%]", mt, ms, mspc);
   add_log_entry_position_text(type, player, 76, 10, msg, (char*)"|", (char*)" ");

   add_log_entry_centered_text(type, player, 76, (char*)"", (char*)"+", (char*)"-");

}

// not used anymore in favor of add_log_entry2
//void add_log_entry(char *txt)
//{
//   strcat(log_msg, txt);
// //  printf(log_msg);
//}

void add_log_entry2(int type, int player, char *txt)
{
   char tmsg[200];
   sprintf(tmsg, "[%2d][%d][%d]%s", type, player, passcount, txt);
   // strcat(log_msg, tmsg);
   memcpy(log_msg + log_msg_pos, tmsg, strlen(tmsg));
   log_msg_pos += strlen(tmsg);
   log_msg[log_msg_pos+1] = 0; // NULL terminate

   //sprintf(log_msg, "%s", txt);
   //printf("%s", tmsg);
}


void add_log_entry_position_text(int type, int player, int width, int pos, char *txt, char *border, char *fill)
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

void add_log_entry_centered_text(int type, int player, int width, char *txt, char *border, char *fill)
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

void add_log_entry_header(int type, int player, char *txt, int blank_lines)
{

   char htext[80]; // make a copy so that doesn't get overwritten
   sprintf(htext, "%s", txt);

   add_log_entry_centered_text(type, player, 76, (char*)"", (char*)"+", (char*)"-");

   for (int i=0; i<blank_lines; i++)
      add_log_entry_centered_text(type, player, 76, (char*)"", (char*)"|", (char*)" ");

   add_log_entry_centered_text(type, player, 76, htext, (char*)"|", (char*)" ");

   for (int i=0; i<blank_lines; i++)
      add_log_entry_centered_text(type, player, 76, (char*)"", (char*)"|", (char*)" ");

   add_log_entry_centered_text(type, player, 76, (char*)"", (char*)"+", (char*)"-");

}



int fill_filename_array(ALLEGRO_FS_ENTRY *fs, void * extra)
{
   if (num_filenames > 999) return 0; // only get 1000 max
   filenames[num_filenames] = al_create_fs_entry(al_get_fs_entry_name(fs));
   num_filenames++;
   return ALLEGRO_FOR_EACH_FS_ENTRY_OK;
}



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


   if (type == 2) // most recent file
   {
      num_filenames = 0;

      // iterate levels in log folder and put in filename array
      al_for_each_fs_entry(FS_fname, fill_filename_array, NULL);

      if (num_filenames == 0)
      {
         printf("No log files found.\n");
      }

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

   tags[25][0] = 0; tags[25][1] = 5;  tags[25][3] = 84; sprintf(ctags[25], "time"); // timer adjust (T) [C]
   tags[27][0] = 1; tags[27][1] = 13; tags[27][3] = 88; sprintf(ctags[27], "cdf1"); // chdf         (X) [CS]
   tags[28][0] = 0; tags[28][1] = 1;  tags[28][3] = 80; sprintf(ctags[28], "cdfp"); // chdf piece   (P) [CS]
   tags[29][0] = 0; tags[29][1] = 7;  tags[29][3] = 87; sprintf(ctags[29], "cdfw"); // chdf when    (W) [C]
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
         // get third tag - (passcount) in the format "[xxx..]"
         char * pch1 = strchr(log_lines[i], '[');
         char * pch2 = strchr(log_lines[i], ']');
         int p1 = pch1-log_lines[i];
         int p2 = pch2-log_lines[i];
         if ((p1 == 0) && (p2 < 8))
         {
            for(int j=0; j<p2; j++)
               buff2[j] = log_lines[i][j+1];
            buff2[p2] = 0;
            log_lines_int[i][2] = atoi(buff2); // passcount
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


   // get start and end passcounts
   int start_pc = log_lines_int[0][2];
   int end_pc = 0;
   for (int i=0; i<num_lines; i++)
      if (log_lines_int[i][2] > end_pc) end_pc = log_lines_int[i][2];

   int pos = 0; // the top passcount line on the screen
   int quit = 0;
   //int redraw = 1;


   // find players in this file
   int lp[8][2];
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

      les = 1;
      set_ortho();
      al_set_target_backbuffer(display);
      al_flip_display();
      al_clear_to_color(al_map_rgb(0,0,0));

      int ty = 0;
      int color = 15;
      int first_line = 0;


      if (line_mode) first_line = line_pos;
      else // pascount mode
      {
         // find index of first line that is equal to or greater than current passcount
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
      else sprintf(msg, "Passcount Mode");
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
         // draw a graph
         // find the range 0 - end_pc

         // build array of data points
         int data[10000][4];
         int num_data = 0;
         for (int i=0; i<1000; i++)
            for (int j=0; j<4; j++)
               data[i][j] = 0;

         // 0 = passcount
         // 1 = player
         // 2  tx b
         // 3  rx b

         for (int i=0; i<num_lines; i++)
         {
            int tx = 99;
            int rx = 99;

            int type = log_lines_int[i][0];
            int p = log_lines_int[i][1];
            int pc = log_lines_int[i][2];
            if (type == 23)
            {
               char tll[200]; // temp log line
               sprintf(tll, "%s", log_lines[i]);

               // get first tag - (type) in the format "[xx]"
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

               // get second tag and discard
               pch1 = strchr(tll, '[');
               pch2 = strchr(tll, ']');

               p1 = pch1-tll;
               p2 = pch2-tll;

               if (p2 - p1 < 8)
               {
                  for(int j=0; j<p2; j++)
                     buff2[j] = tll[j+p1+1];
                  buff2[p2] = 0;
                  //tx = atoi(buff2); // ignore
                  chop_first_x_char(tll, p2+1);
               }

               // get the third tag
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

               data[num_data][0] = pc;
               data[num_data][1] = p;
               data[num_data][2] = tx;
               data[num_data][3] = rx;
               num_data++;

               //printf("pc:%d  tx:%d  rx:%d\n", pc, tx, rx);
            }
         }
         // all the data is in the array

         int gquit = 0;
         int redraw = 1;

         // set the graph width, height and baseline
         int graph_w = SCREEN_W;
         int graph_h = SCREEN_H - 20;

         int gs_pc = 0; // graph start pc


         int bl = graph_h - 20; // baseline
         float y_scale = 1;
         float p2g = 1;            int old_ix, old_ity, old_iry;

         while (!gquit)
         {
            if (redraw)
            {
               al_set_target_backbuffer(display);
               al_clear_to_color(al_map_rgb(0,0,0));

               if (redraw != 2) // don't auto set scale
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
                  p2g = (float)graph_w / (float)end_pc;  // passcount to graph scaler
               }

               // draw x scale lines every second
               for (float i=40; (int)((i-gs_pc)*p2g) < graph_w; i+= 40)
               {
                  int hx = (int)((i-gs_pc)*p2g);
                  al_draw_line(hx, 0, hx, bl, palette_color[13+96], 1);
               }
               // draw x scale lines every 10 seconds
               for (float i=400; (int)((i-gs_pc)*p2g) < graph_w; i+= 400)
               {
                  int hx = (int)((i-gs_pc)*p2g);

                  al_draw_line(hx, 0, hx, bl, palette_color[13+32], 1);
                  sprintf(msg, "%ds", (int)(i/40));
                  int so = 4 * strlen(msg);
                  al_draw_text(font, palette_color[13], hx-so, bl+4, 0, msg);

                  sprintf(msg, "%d", (int)(i));
                  so = 4 * strlen(msg);
                  al_draw_text(font, palette_color[13], hx-so, bl+12, 0, msg);

                  sprintf(msg, "frames");
                  so = 4 * strlen(msg);
                  al_draw_text(font, palette_color[13], hx-so, bl+20, 0, msg);
               }

               // draw y scale lines every 1000 B/s
               al_draw_text(font, palette_color[11], 0, bl-4, 0, msg);
               al_draw_line(44, bl, graph_w, bl, palette_color[11+48], 1);
               for (float i=1000; i*y_scale < graph_h; i+= 1000)
               {
                  int y_pos = bl - (int)(i*y_scale);
                  sprintf(msg, "%d B/s", (int)i);
                  al_draw_text(font, palette_color[11], 0, y_pos-4, 0, msg);
                  al_draw_line(76, y_pos, graph_w, y_pos, palette_color[11+96], 1);
               }


               // draw the data one player at a time
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
                        if ((dp == p) &&  (pc >= gs_pc))
                        {
                           int ix = (int) ( (float)(pc-gs_pc) * p2g);
                           int ity = bl - (int) ( (float)tx * y_scale);
                           int iry = bl - (int) ( (float)ty * y_scale);
                           if (first_time) // set previous point to this point
                           {
                              first_time = 0;
                              old_ix = ix;
                              old_ity = ity;
                              old_iry = iry;
                           }
                           al_draw_line(old_ix, old_iry, ix, iry, palette_color[col-96], 1);
                           al_draw_line(old_ix, old_ity, ix, ity, palette_color[col], 1);
                           old_ix = ix;
                           old_ity = ity;
                           old_iry = iry;
                        }
                     }
                  }
               }


               int lpos = 20;
               for (int i=0; i<8; i++)
                  if (lp[i][1])
                  {
                     int col = players[i].color;
                     char tmsg[5];
                     if (lp[i][0]) sprintf(tmsg,"on ");
                     else
                     {
                        sprintf(tmsg,"off");
                        col = 127; //grey
                     }
                     al_draw_textf(font, palette_color[col], SCREEN_W - 102, lpos+=8, 0, "player:%d %s", i, tmsg);
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

            if (key[ALLEGRO_KEY_RIGHT])
            {
               while (key[ALLEGRO_KEY_RIGHT]) proc_controllers();
               if ((key[ALLEGRO_KEY_LCTRL]) || (key[ALLEGRO_KEY_RCTRL]))
               {
                  gs_pc += 100;
                  if (gs_pc > end_pc) gs_pc = end_pc;
               }
               else if ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT]))
               {
                  gs_pc += 1000;
                  if (gs_pc > end_pc) gs_pc = end_pc;
               }
               else p2g *= 1.1;//                     redraw = 1;

               redraw = 2;
            }
            if (key[ALLEGRO_KEY_LEFT])
            {
               while (key[ALLEGRO_KEY_LEFT]) proc_controllers();
               if ((key[ALLEGRO_KEY_LCTRL]) || (key[ALLEGRO_KEY_RCTRL]))
               {
                  gs_pc -= 100;
                  if (gs_pc < 0) gs_pc = 0;
               }
               else if ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT]))
               {
                  gs_pc -= 1000;
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
         } // end of bandwidth graph
      }
      if (key[ALLEGRO_KEY_ESCAPE])
      {
         while (key[ALLEGRO_KEY_ESCAPE]) proc_controllers();
         quit = 1;
      }
   } // end of log file viewer
   return 0;
}


