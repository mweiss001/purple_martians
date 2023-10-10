// mwLogGraph.cpp

#include "pm.h"
#include "mwLog.h"
#include "mwGraph.h"
#include "mwDisplay.h"
#include "mwColor.h"
#include "mwInput.h"
#include "mwEventQueue.h"
#include "mwMiscFnx.h"
#include "mwFileIterator.h"




void mwLog::run_ping_graph(int num_lines)
{
   if (load_ping_graph(num_lines))
   {

      int sc = 13;
      mGraph[0].set_series(0, "dsync", 10, 0);
      mGraph[0].set_series(1, "offset", 11, 0);
      mGraph[0].set_series(2, "chase", 14, 0);
      mGraph[0].calc_data_range();
      mGraph[0].autorange_axis(1, 1);
      mGraph[0].set_title("Timer Adjust", 2, sc, sc);          // text, text_color, frame_color
      mGraph[0].set_x_axis_legend("Time", "frames", 0, 15, 0);  // text, font, text_color, frame_color
      mGraph[0].set_y_axis_legend("various", "ms or fps", 0, sc, 0);   // text, font, text_color, frame_color
      mGraph[0].set_x_axis_labels(1, 1, 2, 15);                // type, font, tick_size, color
      mGraph[0].set_y_axis_labels(3, 1, 2, sc);                // type, font, tick_size, color
      mGraph[0].linked_group_id = 17;

      sc = 7;
      mGraph[1].set_series(0, "ping", 10, 0);
      mGraph[1].set_series(1, "avg", 11, 0);
      mGraph[1].calc_data_range();
      mGraph[1].autorange_axis(1, 1);
      mGraph[1].set_title("Client to Server Round Trip Ping Times", 2, sc, sc);                 // text, text_color, frame_color
      mGraph[1].set_x_axis_legend("Time", "frames", 0, 15, 0);      // text, font, text_color, frame_color
      mGraph[1].set_y_axis_legend("Ping", "ms", 0, sc, 0);         // text, font, text_color, frame_color
      mGraph[1].set_x_axis_labels(1, 1, 2, 15);                    // type, font, tick_size, color
      mGraph[1].set_y_axis_labels(3, 1, 2, sc);                    // type, font, tick_size, color
      mGraph[1].x_axis_slave = 100;
      mGraph[1].x_axis_legend_draw_on = 0;
      mGraph[1].x_axis_grid_label_draw_on = 0;
      mGraph[1].linked_group_id = 17;

      int split_pos = mDisplay.SCREEN_H/2; // initial
      int sb = 1; // split_bar_size
      int sg = 2; // space between graphs
      int quit = 0;
      while (!quit)
      {
         al_set_target_backbuffer(mDisplay.display);
         al_clear_to_color(al_map_rgb(0, 0, 0));

         int sx1 = mGraph[0].plot_x1;
         int sx2 = mDisplay.SCREEN_W;
         int sy1 = split_pos - sb -1;
         int sy2 = split_pos + sb;

         al_draw_filled_rectangle(sx1, sy1, sx2, sy2, mColor.pc[15+192]);
         if ((mInput.mouse_y >= sy1) && (mInput.mouse_y <= sy2) && (mInput.mouse_x > sx1) && (mInput.mouse_x < sx2))
         {
            al_set_system_mouse_cursor(mDisplay.display, ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_N);
            al_draw_filled_rectangle(sx1, sy1, sx2, sy2, mColor.pc[15+64]);
            while (mInput.mouse_b[1][0])
            {
               split_pos = mInput.mouse_y;
               if (split_pos < mDisplay.SCREEN_H*1/4) split_pos = mDisplay.SCREEN_H*1/4;
               if (split_pos > mDisplay.SCREEN_H*3/4) split_pos = mDisplay.SCREEN_H*3/4;
               sy1 = split_pos - sb -1;
               sy2 = split_pos + sb;

               mGraph[0].set_graph_pos(0, split_pos+sg, mDisplay.SCREEN_W, mDisplay.SCREEN_H);
               mGraph[1].set_graph_pos(0, 0,            mDisplay.SCREEN_W, split_pos-sg);
               mGraph[0].draw_graph(1);
               mGraph[1].draw_graph(1);

               al_draw_filled_rectangle(sx1, sy1, sx2, sy2, mColor.pc[10]);

               al_flip_display();
               mEventQueue.proc(1);
               al_clear_to_color(al_map_rgb(0, 0, 0));
            }
         }
         mGraph[0].set_graph_pos(0, split_pos+sg, mDisplay.SCREEN_W, mDisplay.SCREEN_H);
         mGraph[1].set_graph_pos(0, 0,            mDisplay.SCREEN_W, split_pos-sg);
         mGraph[0].proc_graph();
         mGraph[1].proc_graph();

         al_flip_display();
         mEventQueue.proc(1);
         if (mInput.key[ALLEGRO_KEY_ESCAPE][3]) quit = 1;
      }
   }
}

int mwLog::load_ping_graph(int num_lines)
{
   mGraph[0].initialize();
   mGraph[0].set_series(0, "dsync", 10, 0);
   mGraph[0].set_series(1, "offset", 11, 0);
   mGraph[0].set_series(2, "chase", 14, 0);

   mGraph[1].initialize();
   mGraph[1].set_series(0, "ping", 10, 0);
   mGraph[1].set_series(1, "avg", 11, 0);

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

         mMiscFnx.get_tag_text(tll, res, 0);
         double sync = atof(res);

         mMiscFnx.get_tag_text(tll, res, 0);
         double offset = atof(res);

         mMiscFnx.get_tag_text(tll, res, 0);
         double fps_chase = atof(res);

         //printf("d:%f o:%f c:%f\n", dsync, offset, fps_chase);

         if (sync < 100) // ignore stupidly high ones at start
         {
            mGraph[0].add_data_point(0, (double) fn, sync);
            mGraph[0].add_data_point(1, (double) fn, offset);
            mGraph[0].add_data_point(2, (double) fn, fps_chase);
         }
      }
      if (type == 37) // cpng
      {
         sprintf(tll, "%s", log_lines[i]);

         mMiscFnx.get_tag_text(tll, res, 0);
         double ping = atof(res);

         mMiscFnx.get_tag_text(tll, res, 0);
         double avg = atof(res);

         //printf("ping:%f avg:%f\n", ping, avg);
         mGraph[1].add_data_point(0, (double) fn, ping);
         mGraph[1].add_data_point(1, (double) fn, avg);
      }
   }
   if ((mGraph[0].calc_data_range()) && (mGraph[1].calc_data_range())) return 1;
   return 0;
}

void mwLog::run_bandwidth_graph(int num_lines, int both)
{
   if (load_bandwidth_graph(num_lines, both))
   {
      int c1 = 10;
      mGraph[0].set_series_legend_type(1);
      mGraph[0].calc_data_range();
      mGraph[0].autorange_axis(1, 1);
      mGraph[0].set_title("Transmit Rate", 2, c1, c1);             // text, text_color, frame_color
      mGraph[0].set_x_axis_legend("Time", "frames", 0, 15, 0);     // text, font, text_color, frame_color
      mGraph[0].set_y_axis_legend("Transmit", "kBps", 0, c1, 0);   // text, font, text_color, frame_color
      mGraph[0].set_x_axis_labels(1, 1, 2, 15);                    // type, font, tick_size, color
      mGraph[0].set_y_axis_labels(1, 1, 2, c1);                    // type, font, tick_size, color
      mGraph[0].linked_group_id = 17;


      if (both)
      {
         int c2 = 13;
         mGraph[1].set_series_legend_type(1);
         mGraph[1].calc_data_range();
         mGraph[1].autorange_axis(1, 1);
         mGraph[1].set_title("Receive Rate", 2, c2, c2);              // text, text_color, frame_color
         mGraph[1].set_x_axis_legend("Time", "frames", 0, 15, 0);     // text, font, text_color, frame_color
         mGraph[1].set_y_axis_legend("Receive", "kBps", 0, c2, 0);    // text, font, text_color, frame_color
         mGraph[1].set_x_axis_labels(1, 1, 2, 15);                    // type, font, tick_size, color
         mGraph[1].set_y_axis_labels(2, 1, 2, c2);                    // type, font, tick_size, color
         mGraph[1].x_axis_slave = 100;
         mGraph[1].series_legend_slave = 100;
         mGraph[1].x_axis_legend_draw_on = 0;
         mGraph[1].linked_group_id = 17;
         mGraph[1].x_axis_grid_draw_on = 1;
         mGraph[1].x_axis_grid_label_draw_on = 0;
      }

      int split_pos = mDisplay.SCREEN_H/2; // initial
      int sb = 1; // split_bar_size
      int sg = 2; // space between graphs
      int quit = 0;
      while (!quit)
      {
         al_set_target_backbuffer(mDisplay.display);
         al_clear_to_color(al_map_rgb(0, 0, 0));
         if (both)
         {
            int sx1 = mGraph[0].plot_x1;
            int sx2 = mDisplay.SCREEN_W;
            int sy1 = split_pos - sb -1;
            int sy2 = split_pos + sb;

            al_draw_filled_rectangle(sx1, sy1, sx2, sy2, mColor.pc[15+192]);
            if ((mInput.mouse_y >= sy1) && (mInput.mouse_y <= sy2) && (mInput.mouse_x > sx1) && (mInput.mouse_x < sx2))
            {
               al_set_system_mouse_cursor(mDisplay.display, ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_N);
               al_draw_filled_rectangle(sx1, sy1, sx2, sy2, mColor.pc[15+64]);

               while (mInput.mouse_b[1][0])
               {
                  split_pos = mInput.mouse_y;
                  if (split_pos < mDisplay.SCREEN_H*1/4) split_pos = mDisplay.SCREEN_H*1/4;
                  if (split_pos > mDisplay.SCREEN_H*3/4) split_pos = mDisplay.SCREEN_H*3/4;
                  sy1 = split_pos - sb -1;
                  sy2 = split_pos + sb;

                  mGraph[0].set_graph_pos(0, split_pos+sg, mDisplay.SCREEN_W, mDisplay.SCREEN_H);
                  mGraph[1].set_graph_pos(0, 0,            mDisplay.SCREEN_W, split_pos-sg);
                  mGraph[0].draw_graph(1);
                  mGraph[1].draw_graph(1);
                  al_draw_filled_rectangle(sx1, sy1, sx2, sy2, mColor.pc[10]);
                  al_flip_display();
                  mEventQueue.proc(1);
                  al_clear_to_color(al_map_rgb(0, 0, 0));
               }
            }
         }
         if (both)
         {
            mGraph[0].set_graph_pos(0, split_pos+sg, mDisplay.SCREEN_W, mDisplay.SCREEN_H);
            mGraph[1].set_graph_pos(0, 0,            mDisplay.SCREEN_W, split_pos-sg);
            mGraph[0].proc_graph();
            mGraph[1].proc_graph();
         }
         else
         {
            mGraph[0].set_graph_pos(0, 0, mDisplay.SCREEN_W, mDisplay.SCREEN_H);
            mGraph[0].proc_graph();
         }
         al_flip_display();
         mEventQueue.proc(1);
         if (mInput.key[ALLEGRO_KEY_ESCAPE][3]) quit = 1;
      }
   }
}

int mwLog::load_bandwidth_graph(int num_lines, int both)
{
   // draws a bandwidth graph
   // uses this log entry:
   // [23][0][1360]bandwidth (B/s) TX cur:[ 1878] max:[ 5060] RX cur:[  480] max:[  514]
   // log file has already been loaded into:
   // int log_lines_int[1000000][3];
   // char log_lines[1000000][100];

   mGraph[0].initialize();
   mGraph[1].initialize();

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
         mMiscFnx.get_tag_text(tll, res, 0);
         int tx = atoi(res);

         // get second tag  - rx_cur
         mMiscFnx.get_tag_text(tll, res, 0);
         int rx = atoi(res);


//         // get third tag - tx_pak
//         mMiscFnx.get_tag_text(tll, res, 0);
//         int txp = atoi(res);
//
//         // get fourth tag - rx_pak
//         mMiscFnx.get_tag_text(tll, res, 0);
//         int rxp = atoi(res);



         // enter one array data point
         mGraph[0].add_data_point(p, (double) fn, (double) tx);
         mGraph[1].add_data_point(p, (double) fn, (double) rx);

         // packets
//         mGraph[0].add_data_point(p, (double) fn, (double) txp);
//         mGraph[1].add_data_point(p, (double) fn, (double) rxp);

         //printf("fn:%d  p:%d tx:%d  rx:%d\n", fn, p, tx, rx);
    }
   mGraph[0].set_series_legend_type(1);
   mGraph[1].set_series_legend_type(1);

   if ((mGraph[0].calc_data_range()) && (mGraph[1].calc_data_range())) return 1;
   return 0;
}


void mwLog::run_client_server_sync_graph(int num_lines)
{
   if (load_client_server_sync_graph(num_lines))
   {

      int c0 = 11;
      mGraph[0].set_series_legend_type(1);
      mGraph[0].series_legend_draw_on = 0;
      mGraph[0].calc_data_range();
      mGraph[0].autorange_axis(1, 1);
      mGraph[0].set_title("Player Active", 2, c0, c0);             // text, text_color, frame_color
      mGraph[0].set_x_axis_legend("Time", "frames", 0, 15, 0);     // text, font, text_color, frame_color
      mGraph[0].set_y_axis_legend("Active", "", 0, c0, 0);         // text, font, text_color, frame_color
      mGraph[0].set_x_axis_labels(1, 1, 2, 15);                    // type, font, tick_size, color
      mGraph[0].set_y_axis_labels(0, 1, 2, c0);                    // type, font, tick_size, color
      mGraph[0].x_axis_legend_draw_on = 0;
      mGraph[0].x_axis_slave = 101;
      mGraph[0].series_legend_slave = 101;
      mGraph[0].x_axis_grid_label_draw_on = 0;
      mGraph[0].y_axis_type = 5;
      mGraph[0].y_axis_rng_min = 1.2;
      mGraph[0].y_axis_min = -0.1;
      mGraph[0].y_axis_max = 1.1;
      mGraph[0].y_axis_rng = 1.2;
      mGraph[0].y_axis_zoom_lock = 1;
      mGraph[0].y_axis_grid_draw_on = 0;
      mGraph[0].y_axis_scrollbar_draw_on = 0;
      mGraph[0].y_axis_legend_draw_on = 1;
      mGraph[0].linked_group_id = 17;


      int c2 = 10;
      mGraph[2].set_series_legend_type(1);
      mGraph[2].calc_data_range();
      mGraph[2].autorange_axis(1, 1);
      mGraph[2].set_title("FPS Chase", 2, c2, c2);                 // text, text_color, frame_color
      mGraph[2].set_x_axis_legend("Time", "frames", 0, 15, 0);     // text, font, text_color, frame_color
      mGraph[2].set_y_axis_legend("Chase", "fps", 0, c2, 0);       // text, font, text_color, frame_color
      mGraph[2].set_x_axis_labels(1, 1, 2, 15);                    // type, font, tick_size, color
      mGraph[2].set_y_axis_labels(4, 1, 2, c2);                    // type, font, tick_size, color
      mGraph[2].x_axis_slave = 101;
      mGraph[2].series_legend_slave = 101;
      mGraph[2].x_axis_grid_label_draw_on = 0;
      mGraph[2].x_axis_legend_draw_on = 0;
      mGraph[2].linked_group_id = 17;

      int c1 = 13;
      mGraph[1].set_series_legend_type(1);
      mGraph[1].calc_data_range();
      mGraph[1].autorange_axis(1, 1);
      mGraph[1].set_title("D Sync", 2, c1, c1);                 // text, text_color, frame_color
      mGraph[1].set_x_axis_legend("Time", "frames", 0, 15, 0);  // text, font, text_color, frame_color
      mGraph[1].set_y_axis_legend("D Sync", "ms", 0, c1, 0);    // text, font, text_color, frame_color
      mGraph[1].set_x_axis_labels(1, 1, 2, 15);                 // type, font, tick_size, color
      mGraph[1].set_y_axis_labels(3, 1, 2, c1);                 // type, font, tick_size, color
      mGraph[1].x_axis_legend_draw_on = 1;
      mGraph[1].linked_group_id = 17;


      int otsh = (mDisplay.SCREEN_H-30) / 10; // one tenth screen height
      int split_pos0 = otsh;   // initial
      int split_pos1 = otsh*5; // initial

      int sb = 1; // split_bar_size
      int sg = 2; // space between graphs
      int quit = 0;
      while (!quit)
      {
         al_set_target_backbuffer(mDisplay.display);
         al_clear_to_color(al_map_rgb(0, 0, 0));

         // 2 draggable resize bars!!
         int sx1 = mGraph[0].plot_x1;
         int sx2 = mDisplay.SCREEN_W;
         int s0y1 = split_pos0 - sb -1;
         int s0y2 = split_pos0 + sb;
         int s1y1 = split_pos1 - sb -1;
         int s1y2 = split_pos1 + sb;
         al_draw_filled_rectangle(sx1, s0y1, sx2, s0y2, mColor.pc[15+192]);
         al_draw_filled_rectangle(sx1, s1y1, sx2, s1y2, mColor.pc[15+192]);
         if ((mInput.mouse_y >= s0y1) && (mInput.mouse_y <= s0y2) && (mInput.mouse_x > sx1) && (mInput.mouse_x < sx2))
         {
            al_set_system_mouse_cursor(mDisplay.display, ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_N);
            al_draw_filled_rectangle(sx1, s0y1, sx2, s0y2, mColor.pc[15+64]);
            while (mInput.mouse_b[1][0])
            {
               split_pos0 = mInput.mouse_y;
               if (split_pos0 < otsh) split_pos0 = otsh;
               if (split_pos0 > (9 * otsh)) split_pos0 = (9 * otsh);
               if (split_pos0 > (split_pos1 - otsh)) split_pos0 = split_pos1 - otsh;

               s0y1 = split_pos0 - sb -1;
               s0y2 = split_pos0 + sb;

               mGraph[0].set_graph_pos(0, 0,             mDisplay.SCREEN_W, split_pos0-sg);
               mGraph[2].set_graph_pos(0, split_pos0+sg, mDisplay.SCREEN_W, split_pos1-sg);
               mGraph[1].set_graph_pos(0, split_pos1+sg, mDisplay.SCREEN_W, mDisplay.SCREEN_H);

               mGraph[0].draw_graph(1);
               mGraph[1].draw_graph(1);
               mGraph[2].draw_graph(1);

               al_draw_filled_rectangle(sx1, s0y1, sx2, s0y2, mColor.pc[10]);
               al_flip_display();
               mEventQueue.proc(1);
               al_clear_to_color(al_map_rgb(0, 0, 0));
            }
         }




         if ((mInput.mouse_y >= s1y1) && (mInput.mouse_y <= s1y2) && (mInput.mouse_x > sx1) && (mInput.mouse_x < sx2))
         {
            al_set_system_mouse_cursor(mDisplay.display, ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_N);
            al_draw_filled_rectangle(sx1, s1y1, sx2, s1y2, mColor.pc[15+64]);
            while (mInput.mouse_b[1][0])
            {
               split_pos1 = mInput.mouse_y;
               if (split_pos1 < otsh) split_pos1 = otsh;
               if (split_pos1 < (split_pos0 + otsh)) split_pos1 = split_pos0 + otsh;
               if (split_pos1 > (9 * otsh)) split_pos1 = 9 * otsh;

               s1y1 = split_pos1 - sb -1;
               s1y2 = split_pos1 + sb;

               mGraph[0].set_graph_pos(0, 0,             mDisplay.SCREEN_W, split_pos0-sg);
               mGraph[2].set_graph_pos(0, split_pos0+sg, mDisplay.SCREEN_W, split_pos1-sg);
               mGraph[1].set_graph_pos(0, split_pos1+sg, mDisplay.SCREEN_W, mDisplay.SCREEN_H);

               mGraph[0].draw_graph(1);
               mGraph[1].draw_graph(1);
               mGraph[2].draw_graph(1);

               al_draw_filled_rectangle(sx1, s1y1, sx2, s1y2, mColor.pc[10]);
               al_flip_display();
               mEventQueue.proc(1);
               al_clear_to_color(al_map_rgb(0, 0, 0));
            }
         }


         mGraph[0].set_graph_pos(0, 0,             mDisplay.SCREEN_W, split_pos0-sg);
         mGraph[2].set_graph_pos(0, split_pos0+sg, mDisplay.SCREEN_W, split_pos1-sg);
         mGraph[1].set_graph_pos(0, split_pos1+sg, mDisplay.SCREEN_W, mDisplay.SCREEN_H);

         mGraph[0].proc_graph();
         mGraph[1].proc_graph();
         mGraph[2].proc_graph();

         al_flip_display();
         mEventQueue.proc(1);
         if (mInput.key[ALLEGRO_KEY_ESCAPE][3]) quit = 1;
      }
   }
}

int mwLog::load_client_server_sync_graph(int num_lines)
{
   // server only
   // draws a graph of client server sync

   // log file has already been loaded into:
   // int log_lines_int[1000000][3];
   // char log_lines[1000000][100];
   // with num_lines and end_pc set

   mGraph[0].initialize();
   mGraph[1].initialize();
   mGraph[2].initialize();
   mGraph[0].set_series_legend_type(1);
   mGraph[1].set_series_legend_type(1);
   mGraph[2].set_series_legend_type(1);

   int pa[8] = {0}; // keep track of when players come active
   for (int p=0; p<8; p++)
      mGraph[0].add_data_point(p, 0, pa[p]); // set initial inactive on frame 0 for all players

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
            // printf("p:%d inactive:%d active:%d", p, fn-1, fn);
            mGraph[0].add_data_point(p, (double) fn-1, 0); // inactive
            mGraph[0].add_data_point(p, (double) fn, 1); // active
            pa[p] = 1;
         }
      }


      if (log_lines_int[i][0] == 33) // stak line
      {
         int p = log_lines_int[i][1];
         int fn = log_lines_int[i][2];

         mMiscFnx.get_tag_text(tll, res, 0); // dsync
         double dsy = atof(res);

         mMiscFnx.get_tag_text(tll, res, 0); // fps_chase
         double fc = atof(res);

         mGraph[1].add_data_point(p, (double) fn, dsy); // dsync
         mGraph[2].add_data_point(p, (double) fn, fc);  // fps chase
      }
   }

   int last_fn = log_lines_int[num_lines-1][2];
   for (int p=0; p<8; p++)
      mGraph[0].add_data_point(p, (double) last_fn, pa[p]); // active

   if ((mGraph[0].calc_data_range()) && (mGraph[1].calc_data_range()) && (mGraph[2].calc_data_range())) return 1;
   return 0;
}


void mwLog::run_profile_graph(int choose)
{
   if (load_profile_graph(choose))
   {

      int c1 = 10;
      mGraph[0].calc_data_range();
      mGraph[0].autorange_axis(1, 1);
      mGraph[0].set_title("Profile Time", 2, c1, c1);             // text, text_color, frame_color
      mGraph[0].set_x_axis_legend("Time", "frames", 0, 15, 0);    // text, font, text_color, frame_color
      mGraph[0].set_y_axis_legend("Time", "ms",     0, c1, 0);    // text, font, text_color, frame_color
      mGraph[0].set_x_axis_labels(1, 0, 2, 15);                   // type, font, tick_size, color
      mGraph[0].set_y_axis_labels(3, 1, 2, c1);                   // type, font, tick_size, color

      int quit = 0;
      while (!quit)
      {
         al_set_target_backbuffer(mDisplay.display);
         al_clear_to_color(al_map_rgb(0, 0, 0));

         mGraph[0].set_graph_pos(0, 0, mDisplay.SCREEN_W, mDisplay.SCREEN_H);
         mGraph[0].proc_graph();

         al_flip_display();
         mEventQueue.proc(1);
         if (mInput.key[ALLEGRO_KEY_ESCAPE][3]) quit = 1;
      }
   }
}


int mwLog::load_profile_graph(int choose)
{
   mGraph[0].initialize();
   mGraph[0].set_series(0, "", 1, 0);

   char fname[256] = {0};
   int done = 0;
   int debug_print = 0;

   if (choose)
   {
      ALLEGRO_FILECHOOSER *afc = al_create_native_file_dialog(fname, "Select Log File to View", "*.txt", 0);
      if (al_show_native_file_dialog(mDisplay.display, afc))
      {
         if (al_get_native_file_dialog_count(afc) == 1) sprintf(fname, "%s", al_get_native_file_dialog_path(afc, 0));
      }
      else { al_destroy_native_file_dialog(afc); return 0; } // user cancelled
      al_destroy_native_file_dialog(afc);
   }
   else // most recent file
   {
      sprintf(fname, "%s", mFileIterator.get_most_recent_fname("logs/", fname));
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

         mMiscFnx.get_tag_text(buff, res, 0); // get first tag - type
         int type = atoi(res);
         mMiscFnx.get_tag_text(buff, res, 0); // get second tag - player
         //int p = atoi(res);
         mMiscFnx.get_tag_text(buff, res, 0); // get third tag - mLoop.frame_num
         double fn = atof(res);

         if (type == 44) // tmst
         {
            while (strlen(buff) > 4) // keep getting more tags
            {
               mMiscFnx.get_tag_text2(buff, res, res1, 0);
               double val = atof(res1);
               int match = -1;
               for (int s=0; s<20; s++) // search for a series with this name
                  if (strcmp(mGraph[0].series[s].name, res) == 0)  match = s;
               if (match > -1)
               {
                   mGraph[0].add_data_point(match, fn, val);
                   if (debug_print) printf("added data point to series:%s  fn:%0.0f, val:%0.4f\n", res, fn, val);
               }
               else  // no match found
               {
                  for (int s=19; s>=0; s--) // find last empty searching backwards
                     if (strlen(mGraph[0].series[s].name) == 0)  match = s;

                  if (match > -1)
                  {
                     mGraph[0].set_series(match, res, 1, 0);
                     mGraph[0].autoset_new_series_color(match);
                     mGraph[0].add_data_point(match, fn, val);
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
//   if (mGraph[0].calc_data_range())  run_profile_graph();
   if (mGraph[0].calc_data_range()) return 1;
   else return 0;
}




































void mwLog::run_timestamp_graph(void)
{

   int c1 = 10;
   mGraph[0].calc_data_range();
   mGraph[0].autorange_axis(1, 1);
   mGraph[0].set_title("Draw Time", 2, c1, c1);              // text, text_color, frame_color
   mGraph[0].set_x_axis_legend("Time", "frames", 0, 15, 0);  // text, font, text_color, frame_color
   mGraph[0].set_y_axis_legend("Time", "ms", 0, c1, 0);      // text, font, text_color, frame_color
   mGraph[0].set_x_axis_labels(1, 1, 2, 15);                 // type, font, tick_size, color
   mGraph[0].set_y_axis_labels(3, 1, 2, c1);                 // type, font, tick_size, color
   mGraph[0].linked_group_id = 17;


//   int c1 = 10;
//   mGraph[0].set_series(0, "draw time", c1, 0);
//   mGraph[0].calc_data_range();
//   mGraph[0].autorange_axis(1, 1);
//   mGraph[0].set_title("Draw Time", 2, c1, c1);              // text, text_color, frame_color
//   mGraph[0].set_x_axis_legend("Time", "frames", 0, 15, 0);  // text, font, text_color, frame_color
//   mGraph[0].set_y_axis_legend("Time", "ms", 0, c1, 0);      // text, font, text_color, frame_color
//   mGraph[0].set_x_axis_labels(1, 1, 2, 15);                 // type, font, tick_size, color
//   mGraph[0].set_y_axis_labels(3, 1, 2, c1);                 // type, font, tick_size, color
//   mGraph[0].linked_group_id = 17;
//
//   int c2 = 13;
//   mGraph[1].set_series(0, "move time", c2, 0);
//   mGraph[1].calc_data_range();
//   mGraph[1].autorange_axis(1, 1);
//   mGraph[1].set_title("Move Time", 2, c2, c2);              // text, text_color, frame_color
//   mGraph[1].set_x_axis_legend("Time", "frames", 0, 15, 0);  // text, font, text_color, frame_color
//   mGraph[1].set_y_axis_legend("Time", "ms", 0, c2, 0);      // text, font, text_color, frame_color
//   mGraph[1].set_x_axis_labels(1, 1, 2, 15);                 // type, font, tick_size, color
//   mGraph[1].set_y_axis_labels(3, 1, 2, c2);                 // type, font, tick_size, color
//   mGraph[1].x_axis_slave = 100;
//   mGraph[1].series_legend_slave = 100;
//   mGraph[1].x_axis_legend_draw_on = 0;
//   mGraph[1].linked_group_id = 17;
//   mGraph[1].x_axis_grid_draw_on = 1;
//   mGraph[1].x_axis_grid_label_draw_on = 0;

   int quit = 0;
   while (!quit)
   {
      al_set_target_backbuffer(mDisplay.display);
      al_clear_to_color(al_map_rgb(0, 0, 0));

      mGraph[0].set_graph_pos(0, 0, mDisplay.SCREEN_W, mDisplay.SCREEN_H);
      mGraph[0].proc_graph();


//      mGraph[0].set_graph_pos(0, mDisplay.SCREEN_H/2, mDisplay.SCREEN_W, mDisplay.SCREEN_H);
//      mGraph[1].set_graph_pos(0, 0,          mDisplay.SCREEN_W, mDisplay.SCREEN_H/2);
//      mGraph[0].proc_graph();
//      mGraph[1].proc_graph();


      al_flip_display();
      mEventQueue.proc(1);
      if (mInput.key[ALLEGRO_KEY_ESCAPE][3]) quit = 1;
   }
}


void mwLog::log_timestamp_graph(int num_lines)
{
   mGraph[0].initialize();
   mGraph[0].set_series(0, "stdf time", 10, 0);
   mGraph[0].set_series(1, "base0", 11, 0);
   mGraph[0].set_series(2, "cmp_size", 12, 0);
   mGraph[0].set_series(3, "packets", 13, 0);


//   mGraph[0].initialize();
//   mGraph[0].set_series(0, "draw time", 10, 0);
//
//   mGraph[1].initialize();
//   mGraph[1].set_series(0, "move time", 10, 0);

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

         mMiscFnx.get_tag_text(tll, res, 0);
         double stdf = atof(res);

         mMiscFnx.get_tag_text(tll, res, 0);
         double base = atof(res);

         mMiscFnx.get_tag_text(tll, res, 0);
         double cmp_sz = atof(res) / 1000;

         mMiscFnx.get_tag_text(tll, res, 0);
         double np = atof(res);

        //printf("td:%f tm:%f tt:%f\n", td, tm, tt);

         mGraph[0].add_data_point(0, (double) fn, stdf);
         mGraph[0].add_data_point(1, (double) fn, base);
         mGraph[0].add_data_point(2, (double) fn, cmp_sz);
         mGraph[0].add_data_point(3, (double) fn, np);



//         mGraph[0].add_data_point(2, (double) fn, tt);
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
//         mGraph[0].add_data_point(0, (double) fn, td);
//         mGraph[1].add_data_point(0, (double) fn, tm);
////         mGraph[0].add_data_point(2, (double) fn, tt);
      }
   }
   if ((mGraph[0].calc_data_range()) && (mGraph[0].calc_data_range())) run_timestamp_graph();
}

