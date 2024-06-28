// mwLogViewer.cpp

#include "pm.h"
#include "mwLog.h"
#include "mwPlayer.h"
#include "mwDisplay.h"
#include "mwFont.h"
#include "mwWidget.h"
#include "mwColor.h"
#include "mwInput.h"
#include "mwEventQueue.h"
#include "mwHelp.h"
#include "mwMiscFnx.h"
#include "mwFileIterator.h"

int mwLog::load_log_lines_array_from_static_file(const char* f)
{
   int num_lines=0, done=0;

   al_set_target_backbuffer(mDisplay.display);
   al_clear_to_color(al_map_rgb(0,0,0));
   al_draw_textf(mFont.pr8, mColor.pc[15], mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2, ALLEGRO_ALIGN_CENTER, "Loading Log File:%s", f);
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

      if (loop > 0) strncpy(log_lines[num_lines++], buff, 99); // copy only first 99 char (and only copy if length > 0)

      if (num_lines >= NUM_LOG_LINES)
      {
         printf("log file exceeded %d lines\n", num_lines);
         done = 1;
      }
      if (ch == EOF) done = 1;
   }
   fclose(filepntr);

   //printf("num_lines:%d\n", num_lines);

   for (int i=0; i<num_lines; i++)  // find and add tags to log_lines_int array
   {
      //printf("line:%d %s \n", i, log_lines[i]);
      char res[256] = {0};
      mMiscFnx.get_tag_text(log_lines[i], res, 0); // get first tag - type
      log_lines_int[i][0] = atoi(res);
      mMiscFnx.get_tag_text(log_lines[i], res, 0); // get second tag - player
      log_lines_int[i][1] = atoi(res);
      mMiscFnx.get_tag_text(log_lines[i], res, 0); // get third tag - frame_num
      log_lines_int[i][2] = atoi(res);
   }
   return num_lines;
}

int mwLog::log_file_viewer(int type)
{
   char msg[1024];
   al_show_mouse_cursor(mDisplay.display);
   char fname[1024];
   sprintf(fname, "logs/");

//   //printf("fname:%s\n", fname);
//   // convert to 'ALLEGRO_FS_ENTRY' (to make a fully qualified path)
//   ALLEGRO_FS_ENTRY *FS_fname = al_create_fs_entry(fname);
//
//   // convert back to string
//   sprintf(fname, "%s\\", al_get_fs_entry_name(FS_fname));
//   //printf("FS_fname:%s\n", fname);

   if (type == 2) // most recent file
   {
      sprintf(fname, "%s", mFileIterator.get_most_recent_fname("logs/", fname));
   }
   if (type == 1) // select file
   {
      int user_cancelled = 0;
      ALLEGRO_FILECHOOSER *afc = al_create_native_file_dialog(fname, "Select Log File to View", "*.txt", 0);
      if (al_show_native_file_dialog(mDisplay.display, afc))
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


   int i=0;

   // always on
   i = LOG_NET;                 tags[i][0] = 1; tags[i][1] = 15; // network
   i = LOG_NET_ending_stats;    tags[i][0] = 1; tags[i][1] = 15; // ending stats

   i = LOG_NET_bandwidth;       tags[i][0] = 0; tags[i][1] = 15; tags[i][3] = 66; sprintf(ctags[i], "band"); // bandwidth    (B) [CS]
   i = LOG_NET_stdf;            tags[i][0] = 1; tags[i][1] = 13; tags[i][3] = 88; sprintf(ctags[i], "stdf"); // stdf         (X) [CS]
   i = LOG_NET_stdf_packets;    tags[i][0] = 0; tags[i][1] = 1;  tags[i][3] = 80; sprintf(ctags[i], "stdp"); // stdf piece   (P) [CS]
   i = LOG_NET_stak;            tags[i][0] = 0; tags[i][1] = 6;  tags[i][3] = 75; sprintf(ctags[i], "stak"); // stak         (K) [S]
   i = LOG_NET_dif_apply;       tags[i][0] = 0; tags[i][1] = 7;  tags[i][3] = 68; sprintf(ctags[i], "difa"); // dif applied  (D) [C]
   i = LOG_NET_cdat;            tags[i][0] = 1; tags[i][1] = 3;  tags[i][3] = 67; sprintf(ctags[i], "cdat"); // cdat         (C) [CS]
   i = LOG_NET_timer_adjust;    tags[i][0] = 1; tags[i][1] = 6;  tags[i][3] = 84; sprintf(ctags[i], "tmaj"); // timer adjust (T) [C]
   i = LOG_NET_client_ping;     tags[i][0] = 1; tags[i][1] = 14; tags[i][3] = 78; sprintf(ctags[i], "cpng"); // client ping  (N) [C]

   i = LOG_TMR;                 tags[i][0] = 1; tags[i][1] = 15; tags[i][3] = 79; sprintf(ctags[i], "tmst"); // timestamp    (O) [CS]

   tags[99][0] = 1; tags[99][1] = 10; // bad tag

   for (int i=0; i<num_lines; i++) tags[log_lines_int[i][0]][2]++; // count tags


   // get start and end frame_nums
   int start_pc = log_lines_int[0][2];
   int end_pc = 0;
   for (int i=0; i<num_lines; i++)
      if (log_lines_int[i][2] > end_pc) end_pc = log_lines_int[i][2];

   int first_line = 0; // the top mLoop.frame_num line on the screen
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
         mPlayer.syn[i].color = 8+i;

   while (!quit)
   {
      al_set_target_backbuffer(mDisplay.display);
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
            al_draw_text(mFont.pr8, mColor.pc[color], 0, ty1+=8, 0, msg);
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
                al_draw_text(mFont.pr8, mColor.pc[color], 0, ty1+=8, 0, msg);
                last_line = i;
                if ((int)strlen(msg) > max_line_length) max_line_length = strlen(msg);
            }
         }
         if (++i >= num_lines) done = 1; // no more lines
         if (ty1 > mDisplay.SCREEN_H - 20) done = 1; // no more screen
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
      int sby2 = mDisplay.SCREEN_H - 10;
      int sbh = sby2-sby1;
      al_draw_rectangle(sbx1, sby1, sbx2, sby2+2, mColor.pc[sbc1], 1);

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

      //al_draw_rectangle(sbx1+1, sby1+sbby1+1, sbx2-1, sby1+sbby2, mColor.pc[sbc3], 1);
      al_draw_filled_rectangle(sbx1+1, sby1+sbby1+1, sbx2-1, sby1+sbby2, mColor.pc[sbc3]);

      al_draw_textf(mFont.pixl, mColor.pc[sbc3], sbx2+4, sby1+sbby1-8, 0, "%d", first_line);
      al_draw_textf(mFont.pixl, mColor.pc[sbc3], sbx2+4, sby1+sbby2-1, 0, "%d", last_line);

      if ((mInput.mouse_x > sbx1) && (mInput.mouse_x < sbx2))
      {
         //al_draw_rectangle(sbx1, sby1, sbx2, sby2, mColor.pc[sbc2], 1); // highlight scroll bar

         float my = mInput.mouse_y - sby1; // mouse offset from start of scroll bar
         float mp = my / (float)sbh; // percent of mouse to scroll bar height
         int sbmy = num_lines * (float) mp; // log line that mInput.mouse_y corresponds to

         // frame number of that log line
         int fn   = log_lines_int[sbmy][2];
         al_draw_textf(mFont.pixl, mColor.pc[15], sbx2+4, mInput.mouse_y-8, 0, "%d - frame:%d %ds %dm", sbmy, fn, fn/40, fn/2400);

         if (mInput.mouse_b[1][0])
         {
            while (mInput.mouse_b[1][0]) mEventQueue.proc(1);
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
      al_draw_text(mFont.pr8, mColor.pc[15], xpos, ly+=8, 0, msg);

      sprintf(msg, "%s", fnam);
      al_draw_text(mFont.pr8, mColor.pc[15], xpos, ly+=8, 0, msg);

      sprintf(msg, "Starting frame...[%d]", start_pc);
      al_draw_text(mFont.pr8, mColor.pc[15],xpos, ly+=8, 0, msg);

      sprintf(msg, "Ending frame.....[%d]", end_pc);
      al_draw_text(mFont.pr8, mColor.pc[15],xpos, ly+=8, 0, msg);

      sprintf(msg, "Total lines......[%d]", num_lines);
      al_draw_text(mFont.pr8, mColor.pc[15],xpos, ly+=8, 0, msg);

      sprintf(msg, "Visible lines....[%d]", vis_lines);
      al_draw_text(mFont.pr8, mColor.pc[15],xpos, ly+=8, 0, msg);

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
            al_draw_text(mFont.pr8, mColor.pc[col], xpos, ly, 0, msg);
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
         al_draw_textf(mFont.pr8, mColor.pc[col], xpos, ly+=8, 0, "%d plyr:%d %s num:[%d]", i, i, tmsg, lp[i][1]);
      }

      // current display
      ly+=8;
      sprintf(msg, "Current Display");
      al_draw_text(mFont.pr8, mColor.pc[11],xpos, ly+=8, 0, msg);

      sprintf(msg, "Lines......[%d] to [%d]", first_line, last_line);
      al_draw_text(mFont.pr8, mColor.pc[11],xpos, ly+=8, 0, msg);

      int first_frame = log_lines_int[first_line][2];
      int last_frame = log_lines_int[last_line][2];

      sprintf(msg, "Frames.....[%d] to [%d]", first_frame, last_frame);
      al_draw_text(mFont.pr8, mColor.pc[11],xpos, ly+=8, 0, msg);

      sprintf(msg, "Seconds....[%d] to [%d]", first_frame/40, last_frame/40);
      al_draw_text(mFont.pr8, mColor.pc[11],xpos, ly+=8, 0, msg);

      ly+=20;

      int xa = xpos;
      int xb = xpos + 180;

      if (mWidget.buttont(xa, ly, xb, 16, 0,0,0,0, 0,13,15,0, 1,0,1,0, "Client Sync Graph"))  run_client_server_sync_graph(num_lines);
      ly+=4;
      if (mWidget.buttont(xa, ly, xb, 16, 0,0,0,0, 0,13,15,0, 1,0,1,0, "Bandwidth Graph"))    run_bandwidth_graph(num_lines, 1);
      ly+=4;
      if (mWidget.buttont(xa, ly, xb, 16, 0,0,0,0, 0,13,15,0, 1,0,1,0, "Ping Graph"))         run_ping_graph(num_lines);
      ly+=4;
      if (mWidget.buttont(xa, ly, xb, 16, 0,0,0,0, 0,14,15,0, 1,0,1,0, "Help"))               mHelp.help("Log File Viewer");

      al_flip_display();

      mEventQueue.proc(1);

      int k = mInput.key_pressed_ASCII;
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

      if (mInput.key[ALLEGRO_KEY_UP  ][3]) first_line--;
      if (mInput.key[ALLEGRO_KEY_DOWN][3]) first_line++;

      if (mInput.key[ALLEGRO_KEY_PGUP][3])
      {
         if (mInput.CTRL()) first_line -= 1000;
         else if (mInput.SHFT()) first_line -= 100;
         else first_line-=10;
      }
      if (mInput.key[ALLEGRO_KEY_PGDN][3])
      {
         if (mInput.CTRL()) first_line += 1000;
         else if (mInput.SHFT()) first_line += 100;
         else first_line+=10;
      }

      if (mInput.key[ALLEGRO_KEY_HOME][3]) first_line = 0;
      if (mInput.key[ALLEGRO_KEY_END ][3]) first_line = num_lines-1;

      if (first_line < 0) first_line = 0;
      if (first_line > num_lines-1) first_line = num_lines-1;

      if (mInput.key[ALLEGRO_KEY_ESCAPE][3]) quit = 1;

   } // end of log file viewer
   al_hide_mouse_cursor(mDisplay.display);
   return 0;
}

