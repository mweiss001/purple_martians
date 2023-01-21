// zlog.cpp
#include "pm.h"
#include "z_log.h"
#include "z_mwGraph.h"









// ------------------------------------------------
// ----------------- logging ----------------------
// ------------------------------------------------

char log_msg[NUM_LOG_CHAR]; // for logging
int log_msg_pos = 0;
char log_lines[NUM_LOG_LINES][100]; // for log file viewer
int log_lines_int[NUM_LOG_LINES][3]; // for log file viewer


int LOG_NET = 0;
int LOG_NET_join = 0;
int LOG_NET_player_array = 0;
int LOG_NET_bandwidth = 0;
int LOG_NET_cdat = 0;
int LOG_NET_stdf = 0;
int LOG_NET_stdf_all_packets = 0;
int LOG_NET_stdf_when_to_apply = 0;
int LOG_NET_client_ping = 0;
int LOG_NET_client_timer_adj = 0;
int LOG_NET_server_rx_stak = 0;

int LOG_TMR_cpu = 0;
int LOG_TMR_move_tot = 0;
int LOG_TMR_move_all = 0;
int LOG_TMR_move_enem = 0;

int LOG_TMR_bmsg_add = 0;
int LOG_TMR_bmsg_draw = 0;
int LOG_TMR_scrn_overlay = 0;


int LOG_TMR_draw_tot = 0;
int LOG_TMR_draw_all = 0;
int LOG_TMR_sdif = 0;
int LOG_TMR_cdif = 0;
int LOG_TMR_rwnd = 0;


int autosave_log_on_program_exit = 0;
int autosave_log_on_game_exit = 0;
int autosave_log_on_level_done = 0;
int autosave_game_on_game_exit = 0;
int autosave_game_on_level_done = 0;





























void log_bandwidth_stats(int p)
{
   sprintf(msg,"total tx bytes............[%d]", players1[p].tx_total_bytes);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   sprintf(msg,"max tx bytes per frame....[%d]", players1[p].tx_max_bytes_per_frame);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   sprintf(msg,"avg tx bytes per frame....[%d]", players1[p].tx_total_bytes / frame_num);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   sprintf(msg,"max rx bytes per second...[%d]", players1[p].tx_max_bytes_per_tally);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   sprintf(msg,"avg tx bytes per sec......[%d]", (players1[p].tx_total_bytes *40)/ frame_num);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   sprintf(msg,"total tx packets..........[%d]", players1[p].tx_total_packets);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   sprintf(msg,"max tx packets per frame..[%d]", players1[p].tx_max_packets_per_frame);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   sprintf(msg,"max tx packets per second.[%d]", players1[p].tx_max_packets_per_tally);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");


   sprintf(msg,"total rx bytes............[%d]", players1[p].rx_total_bytes);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   sprintf(msg,"max rx bytes per frame....[%d]", players1[p].rx_max_bytes_per_frame);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   sprintf(msg,"avg rx bytes per frame....[%d]", players1[p].rx_total_bytes / frame_num);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   sprintf(msg,"max rx bytes per second...[%d]", players1[p].rx_max_bytes_per_tally);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   sprintf(msg,"avg rx bytes per sec......[%d]", (players1[p].rx_total_bytes *40)/ frame_num);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   sprintf(msg,"total rx packets..........[%d]", players1[p].rx_total_packets);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   sprintf(msg,"max rx packets per frame..[%d]", players1[p].rx_max_packets_per_frame);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   sprintf(msg,"max rx packets per second.[%d]", players1[p].rx_max_packets_per_tally);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");
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
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");
}


void add_log_TMR(double dt, const char *tag, int p)
{
   sprintf(msg, "tmst %s:[%0.4f]\n", tag, dt*1000);
   add_log_entry2(44, p, msg);
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
   add_log_entry_header(10, 0,  "Player Array", 0);
   add_log_entry_position_text(10, 0, 76, 10, "[p][wh][a][co][m]", "|", " ");
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
      add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");
   }
   add_log_entry_centered_text(10, 0, 76, "", "+", "-");
}

void log_player_array2(void)
{
   sprintf(msg, "[p][a][m][sy]");
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
   add_log_entry_header(10, p, msg, 0);

   sprintf(msg,"total game frames.........[%d]", frame_num);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");
   sprintf(msg,"frame when client joined..[%d]", players1[p].join_frame);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   if (players1[p].quit_frame == 0) players1[p].quit_frame = frame_num;
   sprintf(msg,"frame when client quit....[%d]", players1[p].quit_frame);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   log_reason_for_player_quit(p);

   sprintf(msg,"frames client was active..[%d]", players1[p].quit_frame - players1[p].join_frame);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   sprintf(msg,"frames skipped............[%d]", players1[p].frames_skipped);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   sprintf(msg,"cdat packets total........[%d]", players1[p].client_cdat_packets_tx);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   sprintf(msg,"cdat packets late.........[%d]", players1[p].late_cdats);
   add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

   log_bandwidth_stats(p);
   add_log_entry_centered_text(10, p, 76, "", "+", "-");
}

void log_ending_stats_server()
{
   sprintf(msg,"Server (%s) ending stats", local_hostname);
   add_log_entry_header(10, 0, msg, 0);

   add_log_entry_centered_text(10, 0, 76, "", "+", "-");

   sprintf(msg,"level.....................[%d]", play_level);
   add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");

   sprintf(msg,"total frames..............[%d]", frame_num);
   add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");

   sprintf(msg,"total moves...............[%d]", game_move_entry_pos);
   add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");

   sprintf(msg,"total time (seconds)......[%d]", frame_num/40);
   add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");

   sprintf(msg,"total time (minutes)......[%d]", frame_num/40/60);
   add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");

   sprintf(msg,"server frames skipped.. ..[%d]", players1[0].frames_skipped);
   add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");

   log_bandwidth_stats(0);

   add_log_entry_centered_text(10, 0, 76, "", "+", "-");

   for (int p=1; p<NUM_PLAYERS; p++)
   {
      if ((players[p].control_method == 2) || (players[p].control_method == 8))
      {
         sprintf(msg,"Player:%d (%s)", p, players1[p].hostname);
         add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

         sprintf(msg,"frame when client joined..[%d]", players1[p].join_frame);
         add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

         if (players1[p].quit_frame == 0) players1[p].quit_frame = frame_num;
         sprintf(msg,"frame when client quit....[%d]", players1[p].quit_frame);
         add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

         log_reason_for_player_quit(p);

         sprintf(msg,"frames client was active..[%d]", players1[p].quit_frame - players1[p].join_frame);
         add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

         sprintf(msg,"frames skipped............[%d]", players1[p].frames_skipped);
         add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");

         sprintf(msg,"cdat packets total........[%d]", players1[p].client_cdat_packets_tx);
         add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

         sprintf(msg,"cdat packets late.........[%d]", players1[p].late_cdats);
         add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");

         log_bandwidth_stats(p);

         add_log_entry_centered_text(10, p, 76, "", "+", "-");
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

int lp[8][2];

void get_tag_text(char *str, char *res, int show)
{
   if (show) printf("\nget tag initial %s\n", str);

   // get first tag - server_sync
   char * pch1 = strchr(str, '[');
   char * pch2 = strchr(str, ']');
   int p1 = pch1-str;
   int p2 = pch2-str;
   int plen = p2-p1;

   if (show) printf("p1:%d  p2:%d  plen:%d\n", p1, p2, plen);

   if (plen < 10) // ???
   {
       for(int j=0; j<plen; j++)
          res[j] = str[j+p1+1];
       res[plen-1] = 0;
       if (show) printf("tag text [%s]\n", res);
       chop_first_x_char(str, p2+1);
   }
   if (show) printf("get tag final %s\n", str);
}

int load_log_lines_array_from_static_file(const char* f)
{
   int num_lines=0, done=0;

   al_set_target_backbuffer(display);
   al_clear_to_color(al_map_rgb(0,0,0));
   al_draw_textf(font, palette_color[15], SCREEN_W/2, SCREEN_H/2, ALLEGRO_ALIGN_CENTER, "Loading Log File:%s", f);
   al_flip_display();

   FILE *filepntr=fopen(f,"r");
   while(!done)
   {
      int ch = fgetc(filepntr);
      int loop = 0;
      char buff[200];
      while((ch != '\n') && (ch != EOF))
      {
         if (ch != 13) buff[loop++] = ch;
         ch = fgetc(filepntr);
      }
      buff[loop] = 0; // terminate the string
      if (loop > 99) printf("log line%d exceeded 99 char - %s\n", num_lines, buff);
      strncpy(log_lines[num_lines++], buff, 99); // copy only first 99 char
      if (num_lines >= NUM_LOG_LINES)
      {
         printf("log file exceeded %d lines\n", num_lines);
         done = 1;
      }
      if (ch == EOF) done = 1;
   }
   fclose(filepntr);
   num_lines--;
   // printf("num_lines:%d\n", num_lines);
   for (int i=0; i<num_lines; i++)  // find and add tags to log_lines_int array
   {
      char res[80];
      get_tag_text(log_lines[i], res, 0); // get first tag - type
      log_lines_int[i][0] = atoi(res);
      get_tag_text(log_lines[i], res, 0); // get second tag - player
      log_lines_int[i][1] = atoi(res);
      get_tag_text(log_lines[i], res, 0); // get third tag - frame_num
      log_lines_int[i][2] = atoi(res);
   }
   return num_lines;
}

int log_file_viewer(int type)
{
   al_show_mouse_cursor(display);
   char fname[1024];
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

   int num_lines = load_log_lines_array_from_static_file(fname);

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

   tags[36][0] = 1; tags[36][1] = 13; tags[36][3] = 84; sprintf(ctags[36], "tmaj"); // timer adjust (T) [C]
   tags[37][0] = 1; tags[37][1] = 13; tags[37][3] = 78; sprintf(ctags[37], "cpng"); // client ping  (N) [C]

   tags[40][0] = 1; tags[40][1] = 15; tags[40][3] = 69; sprintf(ctags[40], "gmar"); // game move array (E) [S]
   tags[41][0] = 1; tags[41][1] = 15; tags[41][3] = 77; sprintf(ctags[41], "move"); // move objects    (M) [S]


   tags[44][0] = 1; tags[44][1] = 15; tags[44][3] = 79; sprintf(ctags[44], "tmst"); // timestamp       (O) [CS]

   tags[99][0] = 1; tags[99][1] = 10; // bad tag

   for (int i=0; i<num_lines; i++) tags[log_lines_int[i][0]][2]++; // count tags


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
         players[i].color = 8+i;

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
            tags[i][4] = ly; ly+=8;
            // set the ypos
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
      if (mdw_buttont(xa, ly, xb, 16, 0,0,0,0, 0,13,15,0, 1,0,1,0, "Bandwidth Graph"))    log_bandwidth_graph(num_lines, 1);
      ly+=4;
      if (mdw_buttont(xa, ly, xb, 16, 0,0,0,0, 0,13,15,0, 1,0,1,0, "Ping Graph"))         log_ping_graph(num_lines);
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
         if (CTRL()) first_line -= 1000;
         else if (SHFT()) first_line -= 100;
         else first_line-=10;
      }
      if (key[ALLEGRO_KEY_PGDN][3])
      {
         if (CTRL()) first_line += 1000;
         else if (SHFT()) first_line += 100;
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





void run_ping_graph(void)
{
   int sc = 13;
   mG[0].set_series(0, "dsync", 10, 0);
   mG[0].set_series(1, "offset", 11, 0);
   mG[0].set_series(2, "chase", 14, 0);
   mG[0].calc_data_range();
   mG[0].autorange_axis(1, 1);
   mG[0].set_title("Timer Adjust", 2, sc, sc);          // text, text_color, frame_color
   mG[0].set_x_axis_legend("Time", "frames", 0, 15, 0);  // text, font, text_color, frame_color
   mG[0].set_y_axis_legend("various", "ms or fps", 0, sc, 0);   // text, font, text_color, frame_color
   mG[0].set_x_axis_labels(1, 1, 2, 15);                // type, font, tick_size, color
   mG[0].set_y_axis_labels(3, 1, 2, sc);                // type, font, tick_size, color
   mG[0].linked_group_id = 17;

   sc = 7;
   mG[1].set_series(0, "ping", 10, 0);
   mG[1].set_series(1, "avg", 11, 0);
   mG[1].calc_data_range();
   mG[1].autorange_axis(1, 1);
   mG[1].set_title("Client to Server Round Trip Ping Times", 2, sc, sc);                 // text, text_color, frame_color
   mG[1].set_x_axis_legend("Time", "frames", 0, 15, 0);      // text, font, text_color, frame_color
   mG[1].set_y_axis_legend("Ping", "ms", 0, sc, 0);         // text, font, text_color, frame_color
   mG[1].set_x_axis_labels(1, 1, 2, 15);                    // type, font, tick_size, color
   mG[1].set_y_axis_labels(3, 1, 2, sc);                    // type, font, tick_size, color
   mG[1].x_axis_slave = 100;
   mG[1].x_axis_legend_draw_on = 0;
   mG[1].x_axis_grid_label_draw_on = 0;
   mG[1].linked_group_id = 17;

   int split_pos = SCREEN_H/2; // initial
   int sb = 1; // split_bar_size
   int sg = 2; // space between graphs
   int quit = 0;
   while (!quit)
   {
      al_set_target_backbuffer(display);
      al_clear_to_color(al_map_rgb(0, 0, 0));

      int sx1 = mG[0].plot_x1;
      int sx2 = SCREEN_W;
      int sy1 = split_pos - sb -1;
      int sy2 = split_pos + sb;

      al_draw_filled_rectangle(sx1, sy1, sx2, sy2, palette_color[15+192]);
      if ((mouse_y >= sy1) && (mouse_y <= sy2) && (mouse_x > sx1) && (mouse_x < sx2))
      {
         al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_N);
         al_draw_filled_rectangle(sx1, sy1, sx2, sy2, palette_color[15+64]);
         while (mouse_b[1][0])
         {
            split_pos = mouse_y;
            if (split_pos < SCREEN_H*1/4) split_pos = SCREEN_H*1/4;
            if (split_pos > SCREEN_H*3/4) split_pos = SCREEN_H*3/4;
            sy1 = split_pos - sb -1;
            sy2 = split_pos + sb;

            mG[0].set_graph_pos(0, split_pos+sg, SCREEN_W, SCREEN_H);
            mG[1].set_graph_pos(0, 0,            SCREEN_W, split_pos-sg);
            mG[0].draw_graph(1);
            mG[1].draw_graph(1);

            al_draw_filled_rectangle(sx1, sy1, sx2, sy2, palette_color[10]);

            al_flip_display();
            proc_controllers();
            al_clear_to_color(al_map_rgb(0, 0, 0));
         }
      }

//      mG[0].set_graph_pos(0,0, SCREEN_W, SCREEN_H);

      mG[0].set_graph_pos(0, split_pos+sg, SCREEN_W, SCREEN_H);
      mG[1].set_graph_pos(0, 0,            SCREEN_W, split_pos-sg);
      mG[0].proc_graph();
      mG[1].proc_graph();

      al_flip_display();
      proc_controllers();
      if (key[ALLEGRO_KEY_ESCAPE][3]) quit = 1;
   }
}


void log_ping_graph(int num_lines)
{
   mG[0].initialize();
   mG[0].set_series(0, "dsync", 10, 0);
   mG[0].set_series(1, "offset", 11, 0);
   mG[0].set_series(2, "chase", 14, 0);

   mG[1].initialize();
   mG[1].set_series(0, "ping", 10, 0);
   mG[1].set_series(1, "avg", 11, 0);

   // iterate all log lines and build array of data points
   for (int i=0; i<num_lines; i++)
   {
      int type = log_lines_int[i][0];
      int fn = log_lines_int[i][2];
      char tll[200]; // temp log line
      char res[80];

      if (type == 36) // tmaj
      {
         sprintf(tll, "%s", log_lines[i]);

         get_tag_text(tll, res, 0);
         double dsync = atof(res);

         get_tag_text(tll, res, 0);
         double offset = atof(res);

         get_tag_text(tll, res, 0);
         double fps_chase = atof(res);

         printf("d:%f o:%f c:%f\n", dsync, offset, fps_chase);

         if (dsync < 100) // ignore stupidly high ones at start
         {
            mG[0].add_data_point(0, (double) fn, dsync);
            mG[0].add_data_point(1, (double) fn, offset);
            mG[0].add_data_point(2, (double) fn, fps_chase);
         }
      }
      if (type == 37) // cpng
      {
         sprintf(tll, "%s", log_lines[i]);

         get_tag_text(tll, res, 0);
         double ping = atof(res);

         get_tag_text(tll, res, 0);
         double avg = atof(res);

         printf("ping:%f avg:%f\n", ping, avg);
         mG[1].add_data_point(0, (double) fn, ping);
         mG[1].add_data_point(1, (double) fn, avg);
      }
   }
   if ((mG[0].calc_data_range()) && (mG[1].calc_data_range())) run_ping_graph();
}


void graph_test(void)
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
      get_tag_text(tll, res, 0);
      int type = atoi(res);


      // get 2nd tag -
      get_tag_text(tll, res, 0);
      int p = atoi(res);


      // get 3rd tag - frame
      get_tag_text(tll, res, 0);
      int fn = atoi(res);


      if (type == 23) // tx rate
      {
         get_tag_text(tll, res, 0);
         int tx = atoi(res);
         mG[0].add_data_point(p, (double) fn, (double) tx);
      }

      if (type == 30) // stak
      {
         get_tag_text(tll, res, 0);
         //int sync = atoi(res);

         get_tag_text(tll, res, 0);
         float dsync = atof(res);

         mG[1].add_data_point(p, (double) fn, dsync);
      }
   }


   int sc = 10;
   mG[0].set_series_legend_type(1);
   mG[0].calc_data_range();
   mG[0].autorange_axis(1, 1);
   mG[0].set_title("Transmit Rate", 2, sc, sc);             // text, text_color, frame_color
   mG[0].set_x_axis_legend("Time", "frames", 0, 15, 0);      // text, font, text_color, frame_color
   mG[0].set_y_axis_legend("Transmit", "kBps", 0, sc, 15);    // text, font, text_color, frame_color
   mG[0].set_x_axis_labels(1, 1, 2, 15);                    // type, font, tick_size, color
   mG[0].set_y_axis_labels(1, 1, 2, sc);                    // type, font, tick_size, color


   mG[0].y_axis_grid_draw_on = 1;
   mG[0].y_axis_grid_label_draw_on = 1;
   mG[0].y_axis_scrollbar_draw_on = 1;
   mG[0].y_axis_legend_draw_on = 1;




   sc = 11;
   mG[1].set_series_legend_type(1);
   mG[1].calc_data_range();
   mG[1].autorange_axis(1, 1);
   mG[1].set_title("Client dsync", 2, sc, sc);                 // text, text_color, frame_color
   mG[1].set_x_axis_legend("Time", "frames", 0, 15, 0);      // text, font, text_color, frame_color
   mG[1].set_y_axis_legend("dsync", "ms", 0, sc, 0);         // text, font, text_color, frame_color
   mG[1].set_x_axis_labels(1, 1, 2, 15);                    // type, font, tick_size, color
   mG[1].set_y_axis_labels(3, 1, 2, sc);                    // type, font, tick_size, color

   mG[1].x_axis_slave = 100;
   mG[1].x_axis_legend_draw_on = 0;
   mG[1].x_axis_grid_label_draw_on = 0;



   int quit = 0;
   int split_pos = SCREEN_H/2;
   int sb = 1;  // split_bar_size
   int sg = 4; // space between graphs

   while (!quit)
   {
      al_set_target_backbuffer(display);
      al_clear_to_color(al_map_rgb(0, 0, 0));

      al_draw_filled_rectangle(0, split_pos-sb, SCREEN_W, split_pos+sb, palette_color[15]);
      if ((mouse_y > split_pos-sb) && (mouse_y < split_pos+sb))
      {
         al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_N);
         al_draw_filled_rectangle(0, split_pos-sb, SCREEN_W, split_pos+sb, palette_color[14]);
         while (mouse_b[1][0])
         {
            split_pos = mouse_y;
            if (split_pos < SCREEN_H*1/4) split_pos = SCREEN_H*1/4;
            if (split_pos > SCREEN_H*3/4) split_pos = SCREEN_H*3/4;

            mG[1].set_graph_pos(0, 0,            SCREEN_W, split_pos-sg);
            mG[0].set_graph_pos(0, split_pos+sg, SCREEN_W, SCREEN_H);
            mG[0].draw_graph(1);
            mG[1].draw_graph(1);

            al_draw_filled_rectangle(0, split_pos-sb, SCREEN_W, split_pos+sb, palette_color[10]);
            al_flip_display();
            proc_controllers();
            al_clear_to_color(al_map_rgb(0, 0, 0));
         }
      }


//      mG[0].set_graph_pos(0,0, SCREEN_W, SCREEN_H);
      mG[1].set_graph_pos(0, 0,            SCREEN_W, split_pos-sg);
      mG[0].set_graph_pos(0, split_pos+sg, SCREEN_W, SCREEN_H);


      mG[0].proc_graph();
      mG[1].proc_graph();

      al_flip_display();
      proc_controllers();
      if (key[ALLEGRO_KEY_ESCAPE][3]) quit = 1;
   }
}


void run_bandwidth_graph(int both)
{
   int c1 = 10;
   mG[0].set_series_legend_type(1);
   mG[0].calc_data_range();
   mG[0].autorange_axis(1, 1);
   mG[0].set_title("Transmit Rate", 2, c1, c1);             // text, text_color, frame_color
   mG[0].set_x_axis_legend("Time", "frames", 0, 15, 0);     // text, font, text_color, frame_color
   mG[0].set_y_axis_legend("Transmit", "kBps", 0, c1, 0);   // text, font, text_color, frame_color
   mG[0].set_x_axis_labels(1, 1, 2, 15);                    // type, font, tick_size, color
   mG[0].set_y_axis_labels(1, 1, 2, c1);                    // type, font, tick_size, color
   mG[0].linked_group_id = 17;

   if (both)
   {
      int c2 = 13;
      mG[1].set_series_legend_type(1);
      mG[1].calc_data_range();
      mG[1].autorange_axis(1, 1);
      mG[1].set_title("Receive Rate", 2, c2, c2);              // text, text_color, frame_color
      mG[1].set_x_axis_legend("Time", "frames", 0, 15, 0);     // text, font, text_color, frame_color
      mG[1].set_y_axis_legend("Receive", "kBps", 0, c2, 0);    // text, font, text_color, frame_color
      mG[1].set_x_axis_labels(1, 1, 2, 15);                    // type, font, tick_size, color
      mG[1].set_y_axis_labels(2, 1, 2, c2);                    // type, font, tick_size, color
      mG[1].x_axis_slave = 100;
      mG[1].series_legend_slave = 100;
      mG[1].x_axis_legend_draw_on = 0;
      mG[1].linked_group_id = 17;
      mG[1].x_axis_grid_draw_on = 1;
      mG[1].x_axis_grid_label_draw_on = 0;
   }

   int split_pos = SCREEN_H/2; // initial
   int sb = 1; // split_bar_size
   int sg = 2; // space between graphs
   int quit = 0;
   while (!quit)
   {
      al_set_target_backbuffer(display);
      al_clear_to_color(al_map_rgb(0, 0, 0));
      if (both)
      {
         int sx1 = mG[0].plot_x1;
         int sx2 = SCREEN_W;
         int sy1 = split_pos - sb -1;
         int sy2 = split_pos + sb;

         al_draw_filled_rectangle(sx1, sy1, sx2, sy2, palette_color[15+192]);
         if ((mouse_y >= sy1) && (mouse_y <= sy2) && (mouse_x > sx1) && (mouse_x < sx2))
         {
            al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_N);
            al_draw_filled_rectangle(sx1, sy1, sx2, sy2, palette_color[15+64]);

            while (mouse_b[1][0])
            {
               split_pos = mouse_y;
               if (split_pos < SCREEN_H*1/4) split_pos = SCREEN_H*1/4;
               if (split_pos > SCREEN_H*3/4) split_pos = SCREEN_H*3/4;
               sy1 = split_pos - sb -1;
               sy2 = split_pos + sb;

               mG[0].set_graph_pos(0, split_pos+sg, SCREEN_W, SCREEN_H);
               mG[1].set_graph_pos(0, 0,            SCREEN_W, split_pos-sg);
               mG[0].draw_graph(1);
               mG[1].draw_graph(1);
               al_draw_filled_rectangle(sx1, sy1, sx2, sy2, palette_color[10]);
               al_flip_display();
               proc_controllers();
               al_clear_to_color(al_map_rgb(0, 0, 0));
            }
         }
      }
      if (both)
      {
         mG[0].set_graph_pos(0, split_pos+sg, SCREEN_W, SCREEN_H);
         mG[1].set_graph_pos(0, 0,            SCREEN_W, split_pos-sg);
         mG[0].proc_graph();
         mG[1].proc_graph();
      }
      else
      {
         mG[0].set_graph_pos(0, 0, SCREEN_W, SCREEN_H);
         mG[0].proc_graph();
      }
      al_flip_display();
      proc_controllers();
      if (key[ALLEGRO_KEY_ESCAPE][3]) quit = 1;
   }
}

void log_bandwidth_graph(int num_lines, int both)
{
   // draws a bandwidth graph
   // uses this log entry:
   // [23][0][1360]bandwidth (B/s) TX cur:[ 1878] max:[ 5060] RX cur:[  480] max:[  514]
   // log file has already been loaded into:
   // int log_lines_int[1000000][3];
   // char log_lines[1000000][100];

   mG[0].initialize();
   mG[1].initialize();

   // iterate all log lines and build array of data points
   for (int i=0; i<num_lines; i++)
      if (log_lines_int[i][0] == 23)
      {
         int p = log_lines_int[i][1];
         int fn = log_lines_int[i][2];

         char tll[200]; // temp log line
         char res[80];
         sprintf(tll, "%s", log_lines[i]);

         // get first tag - tx_cur
         get_tag_text(tll, res, 0);
         int tx = atoi(res);

         // get second tag and discard - tx_max
         get_tag_text(tll, res, 0);

         // get third tag  - rx_cur
         get_tag_text(tll, res, 0);
         int rx = atoi(res);

         // enter one array data point
         mG[0].add_data_point(p, (double) fn, (double) tx);
         mG[1].add_data_point(p, (double) fn, (double) rx);

         //printf("fn:%d  p:%d tx:%d  rx:%d\n", fn, p, tx, rx);
    }
   mG[0].set_series_legend_type(1);
   mG[1].set_series_legend_type(1);

   if ((mG[0].calc_data_range()) && (mG[1].calc_data_range())) run_bandwidth_graph(both);
}


void run_client_server_sync_graph(void)
{

   int c0 = 11;
   mG[0].set_series_legend_type(1);
   mG[0].series_legend_draw_on = 0;
   mG[0].calc_data_range();
   mG[0].autorange_axis(1, 1);
   mG[0].set_title("Player Active", 2, c0, c0);             // text, text_color, frame_color
   mG[0].set_x_axis_legend("Time", "frames", 0, 15, 0);     // text, font, text_color, frame_color
   mG[0].set_y_axis_legend("Active", "", 0, c0, 0);         // text, font, text_color, frame_color
   mG[0].set_x_axis_labels(1, 1, 2, 15);                    // type, font, tick_size, color
   mG[0].set_y_axis_labels(0, 1, 2, c0);                    // type, font, tick_size, color
   mG[0].x_axis_legend_draw_on = 0;
   mG[0].x_axis_slave = 101;
   mG[0].series_legend_slave = 101;
   mG[0].x_axis_grid_label_draw_on = 0;
   mG[0].y_axis_type = 5;
   mG[0].y_axis_rng_min = 1.2;
   mG[0].y_axis_min = -0.1;
   mG[0].y_axis_max = 1.1;
   mG[0].y_axis_rng = 1.2;
   mG[0].y_axis_zoom_lock = 1;
   mG[0].y_axis_grid_draw_on = 0;
   mG[0].y_axis_scrollbar_draw_on = 0;
   mG[0].y_axis_legend_draw_on = 1;
   mG[0].linked_group_id = 17;


   int c2 = 10;
   mG[2].set_series_legend_type(1);
   mG[2].calc_data_range();
   mG[2].autorange_axis(1, 1);
   mG[2].set_title("FPS Chase", 2, c2, c2);                 // text, text_color, frame_color
   mG[2].set_x_axis_legend("Time", "frames", 0, 15, 0);     // text, font, text_color, frame_color
   mG[2].set_y_axis_legend("Chase", "fps", 0, c2, 0);       // text, font, text_color, frame_color
   mG[2].set_x_axis_labels(1, 1, 2, 15);                    // type, font, tick_size, color
   mG[2].set_y_axis_labels(4, 1, 2, c2);                    // type, font, tick_size, color
   mG[2].x_axis_slave = 101;
   mG[2].series_legend_slave = 101;
   mG[2].x_axis_grid_label_draw_on = 0;
   mG[2].x_axis_legend_draw_on = 0;
   mG[2].linked_group_id = 17;

   int c1 = 13;
   mG[1].set_series_legend_type(1);
   mG[1].calc_data_range();
   mG[1].autorange_axis(1, 1);
   mG[1].set_title("D Sync", 2, c1, c1);                 // text, text_color, frame_color
   mG[1].set_x_axis_legend("Time", "frames", 0, 15, 0);  // text, font, text_color, frame_color
   mG[1].set_y_axis_legend("D Sync", "ms", 0, c1, 0);    // text, font, text_color, frame_color
   mG[1].set_x_axis_labels(1, 1, 2, 15);                 // type, font, tick_size, color
   mG[1].set_y_axis_labels(3, 1, 2, c1);                 // type, font, tick_size, color
   mG[1].x_axis_legend_draw_on = 1;
   mG[1].linked_group_id = 17;


   int otsh = (SCREEN_H-30) / 10; // one tenth screen height
   int split_pos0 = otsh;   // initial
   int split_pos1 = otsh*5; // initial

   int sb = 1; // split_bar_size
   int sg = 2; // space between graphs
   int quit = 0;
   while (!quit)
   {
      al_set_target_backbuffer(display);
      al_clear_to_color(al_map_rgb(0, 0, 0));


      // 2 draggable resize bars!!
      int sx1 = mG[0].plot_x1;
      int sx2 = SCREEN_W;
      int s0y1 = split_pos0 - sb -1;
      int s0y2 = split_pos0 + sb;
      int s1y1 = split_pos1 - sb -1;
      int s1y2 = split_pos1 + sb;
      al_draw_filled_rectangle(sx1, s0y1, sx2, s0y2, palette_color[15+192]);
      al_draw_filled_rectangle(sx1, s1y1, sx2, s1y2, palette_color[15+192]);
      if ((mouse_y >= s0y1) && (mouse_y <= s0y2) && (mouse_x > sx1) && (mouse_x < sx2))
      {
         al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_N);
         al_draw_filled_rectangle(sx1, s0y1, sx2, s0y2, palette_color[15+64]);
         while (mouse_b[1][0])
         {
            split_pos0 = mouse_y;
            if (split_pos0 < otsh) split_pos0 = otsh;
            if (split_pos0 > (9 * otsh)) split_pos0 = (9 * otsh);
            if (split_pos0 > (split_pos1 - otsh)) split_pos0 = split_pos1 - otsh;

            s0y1 = split_pos0 - sb -1;
            s0y2 = split_pos0 + sb;

            mG[0].set_graph_pos(0, 0,             SCREEN_W, split_pos0-sg);
            mG[2].set_graph_pos(0, split_pos0+sg, SCREEN_W, split_pos1-sg);
            mG[1].set_graph_pos(0, split_pos1+sg, SCREEN_W, SCREEN_H);

            mG[0].draw_graph(1);
            mG[1].draw_graph(1);
            mG[2].draw_graph(1);

            al_draw_filled_rectangle(sx1, s0y1, sx2, s0y2, palette_color[10]);
            al_flip_display();
            proc_controllers();
            al_clear_to_color(al_map_rgb(0, 0, 0));
         }
      }

      if ((mouse_y >= s1y1) && (mouse_y <= s1y2) && (mouse_x > sx1) && (mouse_x < sx2))
      {
         al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_N);
         al_draw_filled_rectangle(sx1, s1y1, sx2, s1y2, palette_color[15+64]);
         while (mouse_b[1][0])
         {
            split_pos1 = mouse_y;
            if (split_pos1 < otsh) split_pos1 = otsh;
            if (split_pos1 < (split_pos0 + otsh)) split_pos1 = split_pos0 + otsh;
            if (split_pos1 > (9 * otsh)) split_pos1 = 9 * otsh;

            s1y1 = split_pos1 - sb -1;
            s1y2 = split_pos1 + sb;

            mG[0].set_graph_pos(0, 0,             SCREEN_W, split_pos0-sg);
            mG[2].set_graph_pos(0, split_pos0+sg, SCREEN_W, split_pos1-sg);
            mG[1].set_graph_pos(0, split_pos1+sg, SCREEN_W, SCREEN_H);

            mG[0].draw_graph(1);
            mG[1].draw_graph(1);
            mG[2].draw_graph(1);

            al_draw_filled_rectangle(sx1, s1y1, sx2, s1y2, palette_color[10]);
            al_flip_display();
            proc_controllers();
            al_clear_to_color(al_map_rgb(0, 0, 0));
         }
      }

      mG[0].set_graph_pos(0, 0,             SCREEN_W, split_pos0-sg);
      mG[2].set_graph_pos(0, split_pos0+sg, SCREEN_W, split_pos1-sg);
      mG[1].set_graph_pos(0, split_pos1+sg, SCREEN_W, SCREEN_H);

      mG[0].proc_graph();
      mG[1].proc_graph();
      mG[2].proc_graph();

      al_flip_display();
      proc_controllers();
      if (key[ALLEGRO_KEY_ESCAPE][3]) quit = 1;
   }
}

void log_client_server_sync_graph(int num_lines)
{
   // server only
   // draws a graph of client server sync

   // log file has already been loaded into:
   // int log_lines_int[1000000][3];
   // char log_lines[1000000][100];
   // with num_lines and end_pc set

   mG[0].initialize();
   mG[1].initialize();
   mG[2].initialize();
   mG[0].set_series_legend_type(1);
   mG[1].set_series_legend_type(1);
   mG[2].set_series_legend_type(1);

   int pa[8] = {0}; // keep track of when players come active
   for (int p=0; p<8; p++)
      mG[0].add_data_point(p, 0, pa[p]); // set initial inactive on frame 0 for all players

   // iterate all log lines and build array of data points
   for (int i=0; i<num_lines; i++)
   {
      char tll[200]; // temp log line
      char res[80];
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
            mG[0].add_data_point(p, (double) fn-1, 0); // inactive
            mG[0].add_data_point(p, (double) fn, 1); // active
            pa[p] = 1;
         }
      }
      if (log_lines_int[i][0] == 30) // stak line
      {
         int p = log_lines_int[i][1];
         int fn = log_lines_int[i][2];

         get_tag_text(tll, res, 0); // server_sync
         //int sy = atoi(res); // not used any more

         get_tag_text(tll, res, 0); // dsync
         double dsy = atof(res);

         get_tag_text(tll, res, 0); // fps_chase
         double fc = atof(res);

         mG[1].add_data_point(p, (double) fn, dsy); // dsync
         mG[2].add_data_point(p, (double) fn, fc);  // fps chase
      }
   }
   int last_fn = log_lines_int[num_lines-1][2];
   for (int p=0; p<8; p++)
      mG[0].add_data_point(p, (double) last_fn, pa[p]); // active

   if ((mG[0].calc_data_range()) && (mG[1].calc_data_range())&& (mG[2].calc_data_range())) run_client_server_sync_graph();
}
























void run_profile_graph(void)
{
   int c1 = 10;
   mG[0].calc_data_range();
   mG[0].autorange_axis(1, 1);
   mG[0].set_title("Profile Time", 2, c1, c1);             // text, text_color, frame_color
   mG[0].set_x_axis_legend("Time", "frames", 0, 15, 0);    // text, font, text_color, frame_color
   mG[0].set_y_axis_legend("Time", "ms",     0, c1, 0);    // text, font, text_color, frame_color
   mG[0].set_x_axis_labels(1, 0, 2, 15);                   // type, font, tick_size, color
   mG[0].set_y_axis_labels(3, 1, 2, c1);                   // type, font, tick_size, color

   int quit = 0;
   while (!quit)
   {
      al_set_target_backbuffer(display);
      al_clear_to_color(al_map_rgb(0, 0, 0));

      mG[0].set_graph_pos(0, 0, SCREEN_W, SCREEN_H);
      mG[0].proc_graph();

      al_flip_display();
      proc_controllers();
      if (key[ALLEGRO_KEY_ESCAPE][3]) quit = 1;
   }
}














int get_tag_text2(char *str, char *res, char *res1, int show)
{
   if (show) printf("Initial string:%s\n", str);

   // get first tag - from space to :
   char * pch1 = strchr(str, ' ');
   char * pch2 = strchr(str, ':');
   if ((pch1 == NULL) || (pch2 == NULL))
   {
      printf("Did not find 1st tag\n");
      return 0;
   }
   else
   {
      int p1 = pch1-str;
      int p2 = pch2-str;
      int plen = p2-p1;

      //if (show) printf("p1:%d  p2:%d  plen:%d\n", p1, p2, plen);

      if (plen < 20) // ???
      {
          for(int j=0; j<plen; j++)
             res[j] = str[j+p1+1];
          res[plen-1] = 0;
          if (show) printf("First tag:'%s'\n", res);
          chop_first_x_char(str, p2+1);
      }
      if (show) printf("Final string after chop:'%s'\n", str);
   }

   // get 2nd tag - from [ to ]
   pch1 = strchr(str, '[');
   pch2 = strchr(str, ']');
   if ((pch1 == NULL) || (pch2 == NULL))
   {
      printf("Did not find 2nd tag\n");
      return 0;
   }
   else
   {
      int p1 = pch1-str;
      int p2 = pch2-str;
      int plen = p2-p1;

      // if (show) printf("p1:%d  p2:%d  plen:%d\n", p1, p2, plen);
      if (plen < 20) // ???
      {
          for(int j=0; j<plen; j++)
             res1[j] = str[j+p1+1];
          res1[plen-1] = 0;
          if (show) printf("2nd tag:'%s'\n", res1);
          chop_first_x_char(str, p2+1);
      }
      if (show) printf("Final string after chop:'%s'\n", str);
   }
   return 1;
}




























void load_profile_graph(int choose)
{
   mG[0].initialize();
   mG[0].set_series(0, "", 1, 0);
   char fname[256] = {0};
   int done = 0;
   int debug_print = 0;

   if (choose)
   {
      ALLEGRO_FILECHOOSER *afc = al_create_native_file_dialog(fname, "Select Log File to View", "*.txt", 0);
      if (al_show_native_file_dialog(display, afc))
      {
         if (al_get_native_file_dialog_count(afc) == 1) sprintf(fname, "%s", al_get_native_file_dialog_path(afc, 0));
      }
      else { al_destroy_native_file_dialog(afc); return; } // user cancelled
      al_destroy_native_file_dialog(afc);
   }
   else // most recent file
   {
      sprintf(fname, "logs/");
      //printf("fname:%s\n", fname);
      // convert to 'ALLEGRO_FS_ENTRY' (to makes fully qualified path)
      ALLEGRO_FS_ENTRY *FS_fname = al_create_fs_entry(fname);

      // convert back to string
      sprintf(fname, "%s\\", al_get_fs_entry_name(FS_fname));
      //printf("FS_fname:%s\n", fname);

      num_filenames = 0;
      // iterate levels in log folder and put in filename array
      al_for_each_fs_entry(FS_fname, fill_filename_array, NULL);
      if (num_filenames == 0) printf("No log files found.\n");
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


   FILE *filepntr=fopen(fname,"r");
   while(!done)
   {
      int ch = fgetc(filepntr);
      int loop = 0;
      char buff[512] = {0};
      while((ch != '\n') && (ch != EOF) && (loop < 500))
      {
         if (ch != 13) buff[loop++] = ch;
         ch = fgetc(filepntr);
      }
      buff[loop] = 0; // terminate the string
      if (loop >= 500) printf("log line exceeded 500 char - %s\n", buff);


      if (debug_print) printf("%s\n", buff);

      if (ch == EOF) done = 1;
      else
      {
         char res[80];
         char res1[80];

         get_tag_text(buff, res, 0); // get first tag - type
         int type = atoi(res);
         get_tag_text(buff, res, 0); // get second tag - player
         //int p = atoi(res);
         get_tag_text(buff, res, 0); // get third tag - frame_num
         double fn = atof(res);

         if (type == 44) // tmst
         {
            while (strlen(buff) > 4) // keep getting more tags
            {
               get_tag_text2(buff, res, res1, 0);
               double val = atof(res1);
               int match = -1;
               for (int s=0; s<20; s++) // search for a series with this name
                  if (strcmp(mG[0].series[s].name, res) == 0)  match = s;
               if (match > -1)
               {
                   mG[0].add_data_point(match, fn, val);
                   if (debug_print) printf("added data point to series:%s  fn:%0.0f, val:%0.4f\n", res, fn, val);
               }
               else  // no match found
               {
                  for (int s=19; s>=0; s--) // find last empty searching backwards
                     if (strlen(mG[0].series[s].name) == 0)  match = s;

                  if (match > -1)
                  {
                     mG[0].set_series(match, res, 1, 0);
                     mG[0].autoset_new_series_color(match);
                     mG[0].add_data_point(match, fn, val);
                     if (debug_print) printf("added initial data point to new series[%d]:%s  fn:%0.0f, val:%0.4f\n", match, res, fn, val);
                  }
                  else if (debug_print) printf("no empty series found\n");
               }
               // at this point, if we don't have a match or an empty just do nothing
            } // end of log line
         } // end of type 44
      }
   }
   fclose(filepntr);
   if (mG[0].calc_data_range())  run_profile_graph();
}






































































void load_profile_graph2(void)
{
   char fname[256];

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
      if (user_cancelled) return;
   }

   // get just the name part of the path
   ALLEGRO_PATH * path = al_create_path(fname);
   const char *tmp = al_get_path_filename(path);
   char fnam[100];
   sprintf(fnam, "%s", tmp);
   al_destroy_path(path);

   int num_lines = load_log_lines_array_from_static_file(fname);


   mG[0].initialize();
   mG[0].set_series(0, "", 1, 0);

   // iterate all log lines and build array of data points
   for (int i=0; i<num_lines; i++)
   {
      int type = log_lines_int[i][0];
      int fn = log_lines_int[i][2];
      char tll[200]; // temp log line
      char res[80];
      char res1[80];

      if (type == 44) // tmst
      {
         sprintf(tll, "%s", log_lines[i]);

         get_tag_text2(tll, res, res1, 0);
         double v = atof(res1);

         // printf("%s %f\n", res, v);


         int match = -1;

         // search for a series with this name
         for (int s=0; s<20; s++)
            if (strcmp(mG[0].series[s].name, res) == 0)  match = s;

         if (match > -1)
         {
             mG[0].add_data_point(match, (double) fn, v);
         }
         else  // no match found
         {
            // find first empty
            for (int s=0; s<20; s++)
               if (strlen(mG[0].series[s].name) == 0)  match = s;

            if (match > -1)
            {
               mG[0].set_series(match, res, 1+match, 0);
               mG[0].add_data_point(match, (double) fn, v);
            }
         }
         // at this point, if we don't have a match or an empty just do nothing
      }
   }
   if ((mG[0].calc_data_range()) && (mG[0].calc_data_range())) run_profile_graph();
}














































































void run_timestamp_graph(void)
{

   int c1 = 10;
   mG[0].calc_data_range();
   mG[0].autorange_axis(1, 1);
   mG[0].set_title("Draw Time", 2, c1, c1);              // text, text_color, frame_color
   mG[0].set_x_axis_legend("Time", "frames", 0, 15, 0);  // text, font, text_color, frame_color
   mG[0].set_y_axis_legend("Time", "ms", 0, c1, 0);      // text, font, text_color, frame_color
   mG[0].set_x_axis_labels(1, 1, 2, 15);                 // type, font, tick_size, color
   mG[0].set_y_axis_labels(3, 1, 2, c1);                 // type, font, tick_size, color
   mG[0].linked_group_id = 17;











//   int c1 = 10;
//   mG[0].set_series(0, "draw time", c1, 0);
//   mG[0].calc_data_range();
//   mG[0].autorange_axis(1, 1);
//   mG[0].set_title("Draw Time", 2, c1, c1);              // text, text_color, frame_color
//   mG[0].set_x_axis_legend("Time", "frames", 0, 15, 0);  // text, font, text_color, frame_color
//   mG[0].set_y_axis_legend("Time", "ms", 0, c1, 0);      // text, font, text_color, frame_color
//   mG[0].set_x_axis_labels(1, 1, 2, 15);                 // type, font, tick_size, color
//   mG[0].set_y_axis_labels(3, 1, 2, c1);                 // type, font, tick_size, color
//   mG[0].linked_group_id = 17;
//
//   int c2 = 13;
//   mG[1].set_series(0, "move time", c2, 0);
//   mG[1].calc_data_range();
//   mG[1].autorange_axis(1, 1);
//   mG[1].set_title("Move Time", 2, c2, c2);              // text, text_color, frame_color
//   mG[1].set_x_axis_legend("Time", "frames", 0, 15, 0);  // text, font, text_color, frame_color
//   mG[1].set_y_axis_legend("Time", "ms", 0, c2, 0);      // text, font, text_color, frame_color
//   mG[1].set_x_axis_labels(1, 1, 2, 15);                 // type, font, tick_size, color
//   mG[1].set_y_axis_labels(3, 1, 2, c2);                 // type, font, tick_size, color
//   mG[1].x_axis_slave = 100;
//   mG[1].series_legend_slave = 100;
//   mG[1].x_axis_legend_draw_on = 0;
//   mG[1].linked_group_id = 17;
//   mG[1].x_axis_grid_draw_on = 1;
//   mG[1].x_axis_grid_label_draw_on = 0;

   int quit = 0;
   while (!quit)
   {
      al_set_target_backbuffer(display);
      al_clear_to_color(al_map_rgb(0, 0, 0));

      mG[0].set_graph_pos(0, 0, SCREEN_W, SCREEN_H);
      mG[0].proc_graph();


//      mG[0].set_graph_pos(0, SCREEN_H/2, SCREEN_W, SCREEN_H);
//      mG[1].set_graph_pos(0, 0,          SCREEN_W, SCREEN_H/2);
//      mG[0].proc_graph();
//      mG[1].proc_graph();


      al_flip_display();
      proc_controllers();
      if (key[ALLEGRO_KEY_ESCAPE][3]) quit = 1;
   }
}



void log_timestamp_graph(int num_lines)
{

   mG[0].initialize();
   mG[0].set_series(0, "stdf time", 10, 0);
   mG[0].set_series(1, "base0", 11, 0);
   mG[0].set_series(2, "cmp_size", 12, 0);
   mG[0].set_series(3, "packets", 13, 0);




//   mG[0].initialize();
//   mG[0].set_series(0, "draw time", 10, 0);
//
//   mG[1].initialize();
//   mG[1].set_series(0, "move time", 10, 0);

   // iterate all log lines and build array of data points
   for (int i=0; i<num_lines; i++)
   {
      int type = log_lines_int[i][0];
      int fn = log_lines_int[i][2];
      char tll[200]; // temp log line
      char res[80];

      if (type == 45) // tmst
      {
         sprintf(tll, "%s", log_lines[i]);

         get_tag_text(tll, res, 0);
         double stdf = atof(res);

         get_tag_text(tll, res, 0);
         double base = atof(res);

         get_tag_text(tll, res, 0);
         double cmp_sz = atof(res) / 1000;

         get_tag_text(tll, res, 0);
         double np = atof(res);

        //printf("td:%f tm:%f tt:%f\n", td, tm, tt);

         mG[0].add_data_point(0, (double) fn, stdf);
         mG[0].add_data_point(1, (double) fn, base);
         mG[0].add_data_point(2, (double) fn, cmp_sz);
         mG[0].add_data_point(3, (double) fn, np);



//         mG[0].add_data_point(2, (double) fn, tt);
//         get_tag_text(tll, res, 0);
//         double td = atof(res);
//
//         get_tag_text(tll, res, 0);
//         double tm = atof(res);
//
//         get_tag_text(tll, res, 0);
//         //double tt = atof(res);
//
//        //printf("td:%f tm:%f tt:%f\n", td, tm, tt);
//
//         mG[0].add_data_point(0, (double) fn, td);
//         mG[1].add_data_point(0, (double) fn, tm);
////         mG[0].add_data_point(2, (double) fn, tt);
      }
   }
   if ((mG[0].calc_data_range()) && (mG[0].calc_data_range())) run_timestamp_graph();
}


struct code_stat
{
   char fname[256];
   char name[256];
   int lines;
   int chars;
   int blank_chars;
   int blanks;
   int code;
   int comment;
   int comment_only;
   int code_only;
   int code_and_comment;
   int max_line_length;
};


void mw_code_stat(struct code_stat &cs)
{
   int done = 0;
   int debug_print = 0;

   cs.lines = 0;
   cs.chars = 0;
   cs.blank_chars = 0;
   cs.blanks = 0;
   cs.code = 0;
   cs.comment = 0;
   cs.comment_only = 0;
   cs.code_only = 0;
   cs.code_and_comment = 0;
   cs.max_line_length = 0;


   FILE *filepntr=fopen(cs.fname,"r");
   while(!done)
   {
      int ch = fgetc(filepntr);
      int loop = 0;
      char buff[512] = {0};
      while((ch != '\n') && (ch != EOF) && (loop < 500))
      {
         if (ch != 13) buff[loop++] = ch;
         ch = fgetc(filepntr);
      }
      buff[loop] = 0; // terminate the string
      if (loop >= 500) printf("log line exceeded 500 char - %s\n", buff);


      if (debug_print) printf("%d - %s\n", cs.lines, buff);

      if (ch == EOF) done = 1;
      else
      {
         cs.lines++;                                          // add to line count
         int len = strlen(buff);                           // get line length
         cs.chars += len;                                     // add to total num of char
         if (len > cs.max_line_length) cs.max_line_length = len; // get max line length
         //printf("\n%d - len:%d\n%s\n", lines, len, buff);
         if (len < 1) cs.blanks++;                            // zero length line is blank
         else                                              // process non zero length line
         {
            int blank_c = 1; // set line blank by default, if any non blank space is found clear it
            int comment_c = 0;
            int code_c = 0;

            // go through the line one char at a time
            for (int i=0; i<len; i++)
            {
               int c = buff[i]; // current char
               //printf("%c[%d] ", buff[i], buff[i]);

               // look for lines that have only (9-tab or 32-space)
               // this never fires, I think the IDE (code::blocks) does not save spaces or tabs on a line if that is the only thing on the line
               if ((c == 9) || (c == 32)) cs.blank_chars++; // add to blank char count
               else blank_c = 0;  // entire line has at least one non blank

               if (i > 0) // look for patterns that include previous char
               {
                  int pc = buff[i-1];  // previous char
                  if ((c == 47) && (pc == 47)) // current and previous are both /
                  {
                     comment_c = 1;

                     // check if not first non blank thing on line
                     if ((i-1) > 0)
                     {
                        for (int j=0; j<(i-1); j++)
                        {
                           int jc = buff[j];
                           if ((jc != 9) && (jc != 32)) code_c = 1;
                        }
                     }
                  }
               }
            }
            if ((!blank_c) && (!comment_c)) code_c = 1; // code only (line is not blank and does not have a comment)
            if (blank_c) cs.blanks++;
            if (code_c) cs.code++;
            if (comment_c) cs.comment++;
            if ((code_c) && (comment_c)) cs.code_and_comment++;
            if ((code_c) && (!comment_c)) cs.code_only++;
            if ((!code_c) && (comment_c)) cs.comment_only++;
         }
      }
   }
   fclose(filepntr);
//   printf("\n");
//   printf("%6d - Total lines\n", lines);
//   printf("%6d - Blank lines\n", blanks);
//
//   printf("%6d - Total non-blank lines\n", lines-blanks);
//   printf("%6d - Code\n", code);
//   printf("%6d - Comments\n", comment);
//   printf("\n");
//
//   printf("%6d - Blank lines\n", blanks);
//   printf("%6d - Code\n", code);
//   printf("%6d - Comments\n", comment);
//
//   printf("%6d - Code Only\n", code_only);
//   printf("%6d - Comment Only\n", comment_only);
//   printf("%6d - Code and Comment\n", code_and_comment);
//
//   printf("--------------------\n");
//   printf("%6d - Total lines\n", lines);
//
//   printf("\nmax line length: %d\n", max_line_length);
//   printf("average length of non blank lines: %d\n", chars / (lines-blanks));
//   printf("%6d - Total chars\n", chars);
//   printf("%6d - Blank chars\n", blank_chars);
//   printf("test Code:[%d] should equal Code Only:[%d] plus Code and Comments:[%d]   [%d]\n", code, code_only, code_and_comment, code_only+code_and_comment);
//   printf("test Comment:[%d] should equal Comment Only:[%d] plus Code and Comments:[%d]   [%d]\n", comment, comment_only, code_and_comment, comment_only+code_and_comment);
//   printf("test Total:[%d] should equal Comment Only:[%d] plus Code only:[%d] plus Code and Comments:[%d]  plus blank:[%d]   [%d]\n",
//           lines, comment_only, code_only, code_and_comment, blanks, comment_only + code_only + code_and_comment + blanks);
}








void show_code_stats(void)
{
   struct code_stat cs[100] = {0};

   char fname[1024];
   sprintf(fname, "src/");
   ALLEGRO_FS_ENTRY *FS_fname = al_create_fs_entry(fname);

   // convert back to string
   sprintf(fname, "%s\\", al_get_fs_entry_name(FS_fname));

   num_filenames = 0;

   // iterate files in folder and put in filename array
   al_for_each_fs_entry(FS_fname, fill_filename_array, NULL);

   if (num_filenames == 0) printf("No files found.\n");
   else
   {
      for (int i=0; i<num_filenames; i++)
      {
         // printf("%s\n", al_get_fs_entry_name(filenames[i]));
         sprintf(cs[i].fname, al_get_fs_entry_name(filenames[i]));
         mw_code_stat(cs[i]);

         // get just the name part of the path
         ALLEGRO_PATH * path = al_create_path(cs[i].fname);
         sprintf(cs[i].name, "%s", al_get_path_filename(path));
         al_destroy_path(path);
      }
   }
   // do the totals
   for (int i=0; i<num_filenames; i++)
   {
      cs[99].lines +=            cs[i].lines;
      cs[99].chars +=            cs[i].chars;
      cs[99].blank_chars +=      cs[i].blank_chars;
      cs[99].blanks +=           cs[i].blanks;
      cs[99].code +=             cs[i].code;
      cs[99].comment +=          cs[i].comment;
      cs[99].comment_only +=     cs[i].comment_only;
      cs[99].code_only +=        cs[i].code_only;
      cs[99].code_and_comment += cs[i].code_and_comment;
      cs[99].max_line_length +=  cs[i].max_line_length;
   }
//   sprintf(cs[99].name, "Total");
   sprintf(cs[99].name, "%d files", num_filenames);

   time_t now = time(NULL);
   struct tm *timenow = localtime(&now);
   strftime(msg, sizeof(msg), "%Y-%m-%d  %H:%M:%S", timenow);
   printf("\n\nPurple Martians Source Code Line Counts [%s]\n\n", msg);

   printf("[%6s][%6s][%6s][%6s][%6s][%6s][%6s]\n", "total", "code", "cd_onl", "commnt", "cm_onl", "cd+cm", "blank");
   printf("--------------------------------------------------------\n");

   if (0) // list sorted alphabetically (or how file iterator chose them)
   {
      for (int i=0; i<num_filenames; i++)
         printf("[%6d][%6d][%6d][%6d][%6d][%6d][%6d] - %s\n", cs[i].lines, cs[i].code, cs[i].code_only, cs[i].comment, cs[i].comment_only, cs[i].code_and_comment, cs[i].blanks, cs[i].name);
   }
   else  // list sorted by code size
   {
      int cur_s = 1;  // current max size
      int i = 0; // current max size index
      while (cur_s)
      {
         cur_s = 0;
         for (int j=0; j<num_filenames; j++)
            if (cs[j].code > cur_s)
            {
               cur_s = cs[j].code;
               i = j;
            }
         if (cur_s > 0)
         {
            printf("[%6d][%6d][%6d][%6d][%6d][%6d][%6d] - %s\n", cs[i].lines, cs[i].code, cs[i].code_only, cs[i].comment, cs[i].comment_only, cs[i].code_and_comment, cs[i].blanks, cs[i].name);
            cs[i].code = 0; // set to zero so we won't print again
         }
      }
   }

   int i = 99;
   printf("-------------------------------------------------------- - totals\n");
   printf("[%6d][%6d][%6d][%6d][%6d][%6d][%6d] - %s\n", cs[i].lines, cs[i].code, cs[i].code_only, cs[i].comment, cs[i].comment_only, cs[i].code_and_comment, cs[i].blanks, cs[i].name);


}










