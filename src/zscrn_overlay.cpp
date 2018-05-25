// zsrn_overlay.cpp

#include "pm.h"

void draw_screen_overlay(void)
{
   // these all draw on screen buffer
   if (speed_testing) draw_speed_test_data();
   draw_top_display();
   draw_bottom_msg();
   show_player_join_quit();
}

void show_player_join_quit(void)
{
   if (show_player_join_quit_timer)
   {
      int t =  show_player_join_quit_timer--;
      int jq = show_player_join_quit_jq;
      int p = show_player_join_quit_player;
      int color = players[p].color;

      if (jq == 0) sprintf(msg, "Player %d left the game!", p);
      if (jq == 1) sprintf(msg, "Player %d joined the game!", p);
      if (jq == 3) sprintf(msg, "Player %d DIED!", p);
      if (jq == 4) sprintf(msg, "Player %d SYNC CHECK FAIL!", p);

      if (jq == 5) sprintf(msg, "Player %d LOST SERVER CONNECTION", p);

      float stretch = ( (float)SCREEN_W / (strlen(msg)*8)) - 1; // (SCREEN_W / text length*8) -1
      float ratio = (float)t / 60;

      int y_pos = SCREEN_H/2;
      int y_pos_move = SCREEN_H/2;

      if (ratio > .6)
      {
           float ra1 = 1 - ratio;                 // starts at 0 and increases to .3
           float ra2 = ra1 * 2.5;                // starts at 0 and increases to .999
           stretch =  ra2 * stretch;
           y_pos = y_pos - y_pos_move + (int)(ra2 * y_pos_move);
      }
      if (ratio < .4)
      {
           float ra1 = .4 - ratio;                // starts at 0 and increases to .3
           float ra2 = ra1 * 2.5;                // starts at 0 and increases to .999
           stretch -=  ra2 * stretch;
           y_pos += (int)(ra2 * y_pos_move);
      }
      if (stretch < .1) stretch = .1;
      rtextout_centre(NULL, msg, SCREEN_W/2, y_pos, color, stretch, 0, .5);
   }
}

void draw_fps_display(int show_type)
{
   int p = active_local_player;
   int y = 2;
   int fs = players1[p].frames_skipped;
   int fsls = frames_skipped_last_second;

   // are we not in a netgame
   if ((players[p].control_method == 0) || (players[p].control_method == 1))
   {
      if (frame_speed != 40) show_type = 1;
      //if (fs) show_type = 2;
      if (fsls) show_type = 2;
   }
   else
   {
      if (fs) show_type = 4;
      if (fsls) show_type = 5;
   }
   if (show_type == 8) sprintf(msg, "%dfps", actual_fps);
   if (show_type == 1) sprintf(msg, "FPS set:%d act:%d", frame_speed, actual_fps);
   if (show_type == 2) sprintf(msg, "frame skip:[%d]  FPS set:%d act:%d", fsls, frame_speed, actual_fps);
   if (show_type == 3) sprintf(msg, "skip:%d total:%d FPS set:%d act:%d", fsls, fs, frame_speed, actual_fps);
   if (show_type == 4) sprintf(msg, "total frames skipped:%d", fs);
   if (show_type == 5) sprintf(msg, "frame skip:%d  total:%d", fsls, fs);
   if (show_type) al_draw_text(font, palette_color[15], SCREEN_W - (strlen(msg)+2) * 8, y, 0, msg);
}

void draw_speed_test_data(void)
{
   int x = BORDER_WIDTH + 10;
   int y = BORDER_WIDTH + 10;
   al_draw_filled_rectangle(x, y, x+100, y+8, palette_color[0]);
   al_draw_textf(font, palette_color[15], x, y,  0, "%d fps", actual_fps);
}

void draw_top_display(void)
{
   int p = active_local_player;
   int tdx = BORDER_WIDTH;
   int tdy = 0;

   if (demo_mode_on) draw_demo_mode_overlay();

   if (SCREEN_W < 600) // special case for narrow screens
   {
      sprintf(msg,"Lv:%d Tm:%d En:%d" , play_level, frame_num/40, num_enemy);
      al_draw_text(font, palette_color[14], tdx, tdy+2,  0, msg);
      tdx += strlen(msg)*8 + 8;
   }
   else
   {
      sprintf(msg,"Level:%d | Time:%d | Enemies:%d" , play_level, frame_num/40, num_enemy);
      al_draw_text(font, palette_color[14], tdx, tdy+2,  0, msg);
      tdx += strlen(msg)*8 + 16;
   }
   // draw health bar
   draw_percent_bar(tdx+44, tdy, 88, 10, al_fixtoi(players[p].LIFE));
   al_draw_textf(font, palette_color[14], tdx+44, tdy+2, ALLEGRO_ALIGN_CENTRE,"Health:%-2d", al_fixtoi(players[p].LIFE));
   tdx += 88;

   // draw free men
   for (int a=0; a<players[p].LIVES; a++)
      al_draw_scaled_rotated_bitmap(player_tile[players[p].bitmap_index][1], 10, 10, tdx+8+(a*10), tdy+6, .5, .5, 0, 0);

   if (show_scale_factor > 0)
   {
      al_draw_textf(font, palette_color[15],SCREEN_W*2/3, 2, ALLEGRO_ALIGN_CENTER, "Scale:%-3.2f", scale_factor);
   }


   int cy = BORDER_WIDTH-6;
   int cx = SCREEN_W-320;


   // common debug overlay for all modes
   //if ((show_debug_overlay) || (speed_testing))
   if (show_debug_overlay)
   {
      if (ima_server) {cx = BORDER_WIDTH; cy+=80;}
      int color = 15;
      int fps_color = 14;
      int fs = players1[p].frames_skipped;
      int fsls = frames_skipped_last_second;

      al_draw_filled_rectangle(cx-4, cy+6, cx+230, cy+54, palette_color[0]);
      al_draw_textf(font, palette_color[color], cx, cy+=8, 0, "SCREEN %d x %d", SCREEN_W, SCREEN_H);
      al_draw_textf(font, palette_color[color], cx, cy+=8, 0, "full:%d display_transform_double:%d ", fullscreen, display_transform_double);
      al_draw_textf(font, palette_color[color], cx, cy+=8, 0, "disp_curr %d x %d ", disp_w_curr, disp_h_curr);
      al_draw_textf(font, palette_color[color], cx, cy+=8, 0, "display  %d x %d ", al_get_display_width(display), al_get_display_height(display));

      al_draw_textf(font, palette_color[color], cx, cy+=8, 0, "scale_factor:%f", scale_factor);
      cy+=4;
      al_draw_textf(font, palette_color[fps_color], cx, cy+=8, 0, "FPS set:%d act:%d", frame_speed, actual_fps);
      al_draw_textf(font, palette_color[fps_color], cx, cy+=8, 0, "frames skipped last second:%d", fsls);
      al_draw_textf(font, palette_color[fps_color], cx, cy+=8, 0, "total frames skipped:%d",fs);
      cy+=4;
//      al_draw_textf(font, palette_color[fps_color], cx, cy+=8, 0, "px:%d     py:%d", al_fixtoi(players[p].PX), al_fixtoi(players[p].PY));
//      al_draw_textf(font, palette_color[fps_color], cx, cy+=8, 0, "pxinc:%1.2f  pyinc:%1.2f", al_fixtof(players[p].xinc), al_fixtof(players[p].yinc));

   }

   #ifdef NETPLAY
   if (ima_server)
   {
      int bdx = BORDER_WIDTH;
      int bdy = SCREEN_H - 10;
      int ts = 0;  // text spacing

      sprintf(msg, "Netgame Server (%s) ", players1[p].hostname );
      al_draw_text(font, palette_color[14], bdx, bdy, 0, msg);
      ts += strlen(msg)*8;

       // calculate num of clients
       int num_clients = 0;
       for (int p=0; p<NUM_PLAYERS; p++)
          if ((players[p].active) && (players[p].control_method == 2)) num_clients++;

      //sprintf(msg, "[clients:%d] [moves:%d]", num_clients, game_move_entry_pos);
      sprintf(msg, " clients:%d ", num_clients);
      al_draw_text(font, palette_color[14], bdx + ts, bdy, 0, msg);
      ts += strlen(msg)*8;

      // total bandwidth
      int tb = players1[0].tx_bytes_per_tally + players1[0].rx_bytes_per_tally;
      if (tb < 1000) sprintf(msg, "(%dB/s)", tb);
      else
      {
         float ftb = (float)tb/1000;
         sprintf(msg, "(%3.1fkB/s)", ftb);
      }
      al_draw_text(font, palette_color[14], bdx + ts, bdy, 0, msg);
      ts += strlen(msg)*8;

      if (show_debug_overlay)
      {
         int x = BORDER_WIDTH;
         int y = BORDER_WIDTH-8;

         al_draw_filled_rectangle(x, y+8, x+880, y+80, palette_color[0]);

         sprintf(msg, "[p][wh][a][co][m][sync][lasd][late][start][destn][np][difs][tkbp][rkbs][dfcr][frsk][name]");
         al_draw_text(font, palette_color[13], x, y + 8, 0, msg);

         for (int p=0; p<NUM_PLAYERS; p++)
         {
            y += 8;

            if (players[p].control_method != 2) players1[p].last_sdak_rx = frame_num; // only do sync for active clients

            sprintf(msg, "[%d][%2d][%d][%2d][%d][%4d][%4d][%4d][%5d][%5d][%2d][%4d][%4.1f][%4.1f][%4d][%4d]",
                                              p,
                                              players1[p].who,
                                              players[p].active,
                                              players[p].color,
                                              players[p].control_method,
                                              players1[p].server_sync,
                                              frame_num - players1[p].last_sdak_rx,
                                              players1[p].chdf_late,
                                              client_chdf_id[p][0],
                                              client_chdf_id[p][1],
                                              players1[p].num_dif_packets,
                                              players1[p].cmp_dif_size,
                                              (float)players1[p].tx_bytes_per_tally/1000,
                                              (float)players1[p].rx_bytes_per_tally/1000,
                                              players1[p].dif_corr,
                                              players1[p].frames_skipped);

            int color = 15;
            if (players[p].active == 0) color = 63;
            if (players[p].active == 1) color = 15;

            al_draw_text(font, palette_color[color], x, y + 8, 0, msg);

            int sl = strlen(msg);

            if (p == 0)
            {
               sprintf(msg, "[%s] <-- server (me!)", players1[p].hostname);
               al_draw_text(font, palette_color[players[p].color], x+(sl)*8, y + 8, 0, msg);
            }
            if ((players[p].active) && (players[p].control_method == 2))
            {
               sprintf(msg, "[%s] <-- active client", players1[p].hostname);
               al_draw_text(font, palette_color[players[p].color], x+(sl)*8, y + 8, 0, msg);
            }
            if ((!players[p].active) && (players[p].control_method == 2))
            {
               sprintf(msg, "[%s] <-- syncing client", players1[p].hostname);
               al_draw_text(font, palette_color[players[p].color], x+(sl)*8, y + 8, 0, msg);
            }
            if (players[p].control_method == 9)
            {
               sprintf(msg, "[%s] <-- used client", players1[p].hostname);
               al_draw_text(font, palette_color[players[p].color], x+(sl)*8, y + 8, 0, msg);
            }
            if ( (players[p].active) || (players[p].control_method == 2) || (players[p].control_method == 9))
            {
               sprintf(msg, "[%2d]", players[p].color);
               al_draw_text(font, palette_color[players[p].color], x+(10*8), y + 8, 0, msg);
            }
         }

         int pty = cy; // continue on from default overlay
         for(int p=0; p<NUM_PLAYERS; p++)
            if (players[p].active)
            {
               al_draw_filled_rectangle(cx, pty+8, cx+600, pty+16, palette_color[0]);
               sprintf(msg, "p:%d bandwidth (B/s) TX cur:[%5d] max:[%5d] RX cur:[%5d] max:[%5d]", p, players1[p].tx_bytes_per_tally, players1[p].tx_max_bytes_per_tally, players1[p].rx_bytes_per_tally, players1[p].rx_max_bytes_per_tally);
               al_draw_text(font, palette_color[15], cx, pty+=8, 0, msg);
            }

         pty += 8;
         for(int p=0; p<NUM_PLAYERS; p++)
            if (players[p].active)
            {
               al_draw_filled_rectangle(cx, pty+8, cx+600, pty+16, palette_color[0]);
               sprintf(msg, "p:%d packets   (p/s) TX cur:[%5d] max:[%5d] RX cur:[%5d] max:[%5d]", p, players1[p].tx_packets_per_tally, players1[p].tx_max_packets_per_tally, players1[p].rx_packets_per_tally, players1[p].rx_max_packets_per_tally);
               al_draw_text(font, palette_color[15], cx, pty+=8, 0, msg);
            }

      }

   }
   if (ima_client)
   {
      int color = players[p].color;

      if (!players[p].active)
      {
         sprintf(msg, "Please wait for server syncronization");
         rtextout_centre(NULL, msg, SCREEN_W/2, SCREEN_H/2-32, color, 2, 0, 1);

         sprintf(msg, "[%d]", players1[p].server_sync);
         rtextout_centre(NULL, msg, SCREEN_W/2, SCREEN_H/2, color, 4, 0, 1);
      }


      int bdx = BORDER_WIDTH;
      int bdy = SCREEN_H - 10;
      int ts = 0;  // text spacing

      sprintf(msg, "Netgame Client (%s) ", local_hostname );
      al_draw_text(font, palette_color[14], bdx, bdy, 0, msg);
      ts += strlen(msg)*8;

      sprintf(msg, "sync:%d ", players1[p].server_sync);
      al_draw_text(font, palette_color[14], bdx + ts, bdy, 0, msg);
      ts += strlen(msg)*8;

      if (show_debug_overlay) // show a bunch of data for troubleshooting
      {
         int tx = BORDER_WIDTH;
         int ty = 80;

         if (players1[p].chdf_rx > 0)
         {
            sprintf(msg, "chdf rx'd:%d on_time:%d late:%d [%d]",
                          players1[p].chdf_rx, players1[p].chdf_on_time,
                          players1[p].chdf_late, (players1[p].chdf_on_time * 100) / players1[p].chdf_rx);
            al_draw_filled_rectangle(tx, ty+8, tx+strlen(msg)*8, ty+16, palette_color[0]);
            al_draw_text(font, palette_color[15], tx, ty+=8, 0, msg);
         }

         sprintf(msg, "dif corrections:%d ", players1[p].dif_corr);
         al_draw_filled_rectangle(tx, ty+8, tx+strlen(msg)*8, ty+16, palette_color[0]);
         al_draw_text(font, palette_color[15], tx, ty+=8, 0, msg);

         sprintf(msg, "late cdats dropped by server:%d", players1[p].serr_c_sync_err);
         al_draw_filled_rectangle(tx, ty+8, tx+strlen(msg)*8, ty+16, palette_color[0]);
         al_draw_text(font, palette_color[15], tx, ty+=8, 0, msg);

         sprintf(msg, "game moves lead from server:%d min:%d err:%d", players1[p].c_sync, players1[p].c_sync_min, players1[p].c_sync_err);
         al_draw_filled_rectangle(tx, ty+8, tx+strlen(msg)*8, ty+16, palette_color[0]);
         al_draw_text(font, palette_color[15], tx, ty+=8, 0, msg);


         sprintf(msg, "server sync:%d ", players1[p].server_sync);
         al_draw_filled_rectangle(tx, ty+8, tx+strlen(msg)*8, ty+16, palette_color[0]);
         al_draw_text(font, palette_color[15], tx, ty+=8, 0, msg);

         sprintf(msg, "moves:%d", game_move_entry_pos);
         al_draw_filled_rectangle(tx, ty+8, tx+strlen(msg)*8, ty+16, palette_color[0]);
         al_draw_text(font, palette_color[15], tx, ty+=8, 0, msg);

         sprintf(msg, "moves skipped:%d", players1[p].moves_skipped);
         al_draw_filled_rectangle(tx, ty+8, tx+strlen(msg)*8, ty+16, palette_color[0]);
         al_draw_text(font, palette_color[15], tx, ty+=8, 0, msg);

         sprintf(msg, "moves skipped per second :%d", players1[p].moves_skipped_last_tally);
         al_draw_filled_rectangle(tx, ty+8, tx+strlen(msg)*8, ty+16, palette_color[0]);
         al_draw_text(font, palette_color[15], tx, ty+=8, 0, msg);

         sprintf(msg, "sdat skipped:%d", players1[p].sdat_skipped);
         al_draw_filled_rectangle(tx, ty+8, tx+strlen(msg)*8, ty+16, palette_color[0]);
         al_draw_text(font, palette_color[15], tx, ty+=8, 0, msg);

         sprintf(msg, "frames_skipped:%d", players1[p].frames_skipped);
         al_draw_filled_rectangle(tx, ty+8, tx+strlen(msg)*8, ty+16, palette_color[0]);
         al_draw_text(font, palette_color[15], tx, ty+=8, 0, msg);


         ty +=8;

         sprintf(msg, "bandwidth (bytes per second)");
         al_draw_filled_rectangle(tx, ty+8, tx+strlen(msg)*8, ty+16, palette_color[0]);
         al_draw_text(font, palette_color[15], tx, ty+=8, 0, msg);

         sprintf(msg, "TX currrent:[%d] max:[%d]", players1[p].tx_bytes_per_tally, players1[p].tx_max_bytes_per_tally);
         al_draw_filled_rectangle(tx, ty+8, tx+strlen(msg)*8, ty+16, palette_color[0]);
         al_draw_text(font, palette_color[15], tx, ty+=8, 0, msg);

         sprintf(msg, "RX currrent:[%d] max:[%d]", players1[p].rx_bytes_per_tally, players1[p].rx_max_bytes_per_tally);
         al_draw_filled_rectangle(tx, ty+8, tx+strlen(msg)*8, ty+16, palette_color[0]);
         al_draw_text(font, palette_color[15], tx, ty+=8, 0, msg);

         ty +=8;

         sprintf(msg, "packets per second");
         al_draw_filled_rectangle(tx, ty+8, tx+strlen(msg)*8, ty+16, palette_color[0]);
         al_draw_text(font, palette_color[15], tx, ty+=8, 0, msg);

         sprintf(msg, "TX currrent:[%d] max:[%d]", players1[p].tx_packets_per_tally, players1[p].tx_max_packets_per_tally);
         al_draw_filled_rectangle(tx, ty+8, tx+strlen(msg)*8, ty+16, palette_color[0]);
         al_draw_text(font, palette_color[15], tx, ty+=8, 0, msg);

         sprintf(msg, "RX currrent:[%d] max:[%d]", players1[p].rx_packets_per_tally, players1[p].rx_max_packets_per_tally);
         al_draw_filled_rectangle(tx, ty+8, tx+strlen(msg)*8, ty+16, palette_color[0]);
         al_draw_text(font, palette_color[15], tx, ty+=8, 0, msg);


         int x = BORDER_WIDTH;
         int y = BORDER_WIDTH;

         // print player array for client

         sprintf(msg, "[p][a][co][m]");
         al_draw_filled_rectangle(x, y, tx+strlen(msg)*8, y+8, palette_color[0]);
         al_draw_text(font, palette_color[13], x, y, 0, msg);
         for (int p=0; p<NUM_PLAYERS; p++)
         {
            y += 8;
            char ms[80];
            sprintf(ms, " ");

            if ((players[p].active) && (players[p].control_method == 2))
               sprintf(ms, " <-- active client");

            if ((!players[p].active) && (players[p].control_method == 2))
               sprintf(ms, " <-- syncing client");

            if (players[p].control_method == 9) sprintf(ms, " <-- used client");

            if (p == active_local_player) sprintf(ms, " <-- active local player (me!)");
            if (p == 0) sprintf(ms, " <-- server");

            sprintf(msg, "[%d][%d][%2d][%d]%s",
                                     p,
                                     players[p].active,
                                     players[p].color,
                                     players[p].control_method,
                                     ms );

            int color = 15;
            if (players[p].active == 0) color = 63;
            if (players[p].active == 1) color = 15;
            if (players1[p].c_sync_err)  color = 10;
            al_draw_filled_rectangle(x, y, tx+strlen(msg)*8, y+8, palette_color[0]);
            al_draw_text(font, palette_color[color], x, y, 0, msg);

            // colorize only active, syncing or used
            if ( (players[p].active) || (players[p].control_method == 2) || (players[p].control_method == 9))
            {
               sprintf(msg, "[%2d]", players[p].color);
               al_draw_text(font, palette_color[players[p].color], x+(6*8), y, 0, msg);
            }

            if (p == active_local_player)
            {
               sprintf(msg, " <-- active local player (me!)");
               al_draw_text(font, palette_color[players[p].color], x+(13*8), y, 0, msg);
            }

         }
      } // end of debug overlay
   } // end of if (imaclient)
#endif

   if (players[0].control_method == 1) // file play
   {
      // find last gm with frame_num !=0
      int last_pc = 0;
      for (int g = game_move_entry_pos; g>0; g--)
         if (game_moves[g][0] != 0)
         {
            last_pc = game_moves[g][0];
            break; // exit loop immed
         }

      int bdx = BORDER_WIDTH;
      int bdy = SCREEN_H - 10;
      int ts = 0;  // text spacing

      cy+=4;
      sprintf(msg, "Running Saved Game ");
      al_draw_text(font, palette_color[14], bdx, bdy, 0, msg);
      ts += strlen(msg)*8;
      if (show_debug_overlay)
      {
         al_draw_filled_rectangle(cx, cy+8, cx+strlen(msg)*8, cy+16, palette_color[0]);
         al_draw_text(font, palette_color[15], cx, cy+=8, 0, msg);
      }

      sprintf(msg, "[%d%%] ", frame_num*100/last_pc);
      al_draw_text(font, palette_color[14], bdx + ts, bdy, 0, msg);
      ts += strlen(msg)*8;
      if (show_debug_overlay)
      {
         sprintf(msg, "Time:[%d%%] ", frame_num*100/last_pc);
         al_draw_filled_rectangle(cx, cy+8, cx+strlen(msg)*8, cy+16, palette_color[0]);
         al_draw_text(font, palette_color[15], cx, cy+=8, 0, msg);
      }

      int m = (game_move_current_pos * 100) / game_move_entry_pos;
      sprintf(msg, "Moves:[%d%%] ", m);
      if (show_debug_overlay)
      {
         al_draw_filled_rectangle(cx, cy+8, cx+strlen(msg)*8, cy+16, palette_color[0]);
         al_draw_text(font, palette_color[15], cx, cy+=8, 0, msg);
      }
      for (int ap=0; ap<NUM_PLAYERS; ap++)
      {
         if (players[ap].active)
         {
            //sprintf(msg, "%d", ap);
            //al_draw_text(font, palette_color[players[ap].color], bdx + ts, bdy, 0, msg);
            //ts += strlen(msg)*8;
            if (show_debug_overlay)
            {
               if (ap == active_local_player) sprintf(msg, "Player:%d <- active", ap);
               else sprintf(msg, "Player:%d", ap);
               al_draw_filled_rectangle(cx, cy+8, cx+strlen(msg)*8, cy+16, palette_color[0]);
               al_draw_text(font, palette_color[players[ap].color], cx, cy+=8, 0, msg);
            }
         }
      }
   }
}


void add_screen_msg(char *txt, int x, int y, int delay, int ssn, int z1, int z2, int z3, int z4 )
{
   // check to see if identical to any active entry
   int already_exists = 0;
    for (int a=0; a<100; a++)
       if (screen_msgs[a].active)
          if (screen_msgs[a].original_x == x)
             if (screen_msgs[a].original_y == y)
                if (strcmp(screen_msgs[a].text, txt) == 0)
                   if (ssn != 4 ) already_exists = 1;

   if (!already_exists)
   {
      for (int a=0; a<100; a++)
         if (!screen_msgs[a].active) // find first empty
         {
             // default settings
             screen_msgs[a].delay = 40;
             screen_msgs[a].active = 1;
             screen_msgs[a].color = 15;
             screen_msgs[a].color_inc = 0;
             screen_msgs[a].color_inc_holdoff = 0;
             screen_msgs[a].current_holdoff = 0;
             screen_msgs[a].delay = delay;
             screen_msgs[a].ssn = ssn;
             screen_msgs[a].original_x = x;
             screen_msgs[a].original_y = y;
             screen_msgs[a].x = x;
             screen_msgs[a].y = y;
             screen_msgs[a].xinc = 0;
             screen_msgs[a].yinc = 0;
             sprintf(screen_msgs[a].text, "%s", txt);
             screen_msgs[a].size = 1.0;
             screen_msgs[a].size_inc = 0.0;
             screen_msgs[a].rot = 0;
             screen_msgs[a].rot_inc = 0;

             // specific overrides

             if (ssn == 1) // Door
             {
                screen_msgs[a].delay = 1;
                screen_msgs[a].color = 0;
                screen_msgs[a].rot = 0;
                screen_msgs[a].rot_inc = 0;
                screen_msgs[a].size = 1.0;
                screen_msgs[a].size_inc = -.000;
                screen_msgs[a].x = x;
                screen_msgs[a].y = y-12; // raise above door

                // multistep
                screen_msgs[a].current_step = 1;

                // initial step set here
                screen_msgs[a].size = 0;
                screen_msgs[a].size_inc = 0;
             }


             if (ssn == 2) // Health +
             {
                screen_msgs[a].delay = 1;
                screen_msgs[a].color = 11; // green

                // multistep
                screen_msgs[a].current_step = 1;

                // initial step set here
                screen_msgs[a].size = 0;
                screen_msgs[a].size_inc = 0;

             }

             if (ssn == 3) // Health -
             {
                screen_msgs[a].delay = 1;
                screen_msgs[a].color = 10; // red

                // multistep
                screen_msgs[a].current_step = 1;

                // initial step set here
                screen_msgs[a].size = 0;
                screen_msgs[a].size_inc = 0;

             }


             if (ssn == 4) // player shoots, bang!
             {
                screen_msgs[a].delay = 8;

                int p = z1; // player num
                int b = z2; // bullet num

                screen_msgs[a].x = pbullet[b][2];
                screen_msgs[a].y = pbullet[b][3];

                screen_msgs[a].xinc = pbullet[b][4]/3;
                screen_msgs[a].yinc = pbullet[b][5]/3;

                // do one inc to see if lines up better
                screen_msgs[a].x += pbullet[b][4];
                screen_msgs[a].y += pbullet[b][5];

                screen_msgs[a].color = players[p].color;

                screen_msgs[a].size = .2;
                screen_msgs[a].size_inc = .1;
             }


             if (ssn == 5) // key
             {
                screen_msgs[a].delay = 20;
                // get key tile
                if  (z2 == 1039) screen_msgs[a].color = 10; //red
                if  (z2 == 1040) screen_msgs[a].color = 11; //green
                if  (z2 == 1041) screen_msgs[a].color = 13; //lt blue
                if  (z2 == 1042) screen_msgs[a].color = 8;  //purple
             }

             if (ssn == 6) // boing
             {
                screen_msgs[a].delay = 20;
                screen_msgs[a].color = 14; //yellow

                screen_msgs[a].rot = 0;
                screen_msgs[a].rot_inc = 0;

                screen_msgs[a].size = .4;
                screen_msgs[a].size_inc = .015;

                screen_msgs[a].x = x;
                screen_msgs[a].y = y;
                screen_msgs[a].yinc = -4;

             }


             if (ssn == 7) // switch
             {
                screen_msgs[a].delay = 20;
                screen_msgs[a].color = 15; // white

                screen_msgs[a].rot = 0;
                screen_msgs[a].rot_inc = 0;

                screen_msgs[a].size = .8;
                screen_msgs[a].size_inc = .005;

                screen_msgs[a].x = x;
                screen_msgs[a].y = y;
                screen_msgs[a].yinc = -1;

             }

             if ((ssn == 34) || (ssn == 35))  // squished or stuck
             {
                screen_msgs[a].delay = 8;
                screen_msgs[a].color = 10; // red

                screen_msgs[a].rot = 0;
                screen_msgs[a].rot_inc = 0;

                screen_msgs[a].size = .8;
                screen_msgs[a].size_inc = .035;

                screen_msgs[a].x = x;
                screen_msgs[a].y = y-10;
                screen_msgs[a].yinc = -3;

             }

              a =100; // quit loop
         }
   }
}

void draw_screen_msg(void)
{
    for (int a=0; a<100; a++)
       if (screen_msgs[a].active)
       {
          int ssn = screen_msgs[a].ssn;

          int x = screen_msgs[a].x;
          int y = screen_msgs[a].y;
          int c = screen_msgs[a].color;
          int rot = screen_msgs[a].rot;
          float s = screen_msgs[a].size;

          rtextout_centre(level_buffer, screen_msgs[a].text, x + 10, y + 10, c, s, rot, 1 );

           // for all mode that aren't multimode
           if ((ssn == 0) || (ssn == 0) || (ssn == 4) || (ssn == 5) || (ssn == 6) || (ssn == 7) || (ssn == 34)|| (ssn == 35))
           {
             // decrement delay till 0 then set inactive
             --screen_msgs[a].delay;
             if (screen_msgs[a].delay == 0) screen_msgs[a].active = 0;
           }

           if (screen_msgs[a].ssn == 2) // health +
           {
             --screen_msgs[a].delay;

              if (screen_msgs[a].current_step == 1) // immediate next
              {
                 if (screen_msgs[a].delay == 0) // next out
                 {
                    screen_msgs[a].delay = 5;
                    screen_msgs[a].color = 11;
                    screen_msgs[a].yinc = -5;
                    screen_msgs[a].size = .5;
                    screen_msgs[a].size_inc = .11;
                    screen_msgs[a].current_step = 2;
                 }
              }
              if (screen_msgs[a].current_step == 2)
              {
                 if (screen_msgs[a].delay == 0) // next wait
                 {
                    screen_msgs[a].delay = 30;
                    screen_msgs[a].color = 11;

                    screen_msgs[a].yinc = 0;
                    screen_msgs[a].size_inc = 0;
                    screen_msgs[a].current_step = 3;
                 }
              }

              if (screen_msgs[a].current_step == 3)
              {
                 if (screen_msgs[a].delay == 0) // next in
                 {
                    screen_msgs[a].delay = 6;
                    screen_msgs[a].color = 11;
                    screen_msgs[a].yinc = 4;
                    screen_msgs[a].size_inc = -.14;
                    screen_msgs[a].current_step = 8;
                 }
              }
              if (screen_msgs[a].current_step == 8)
              {
                 if (screen_msgs[a].delay == 0)
                 {
                    screen_msgs[a].active = 0;  // done
                 }
              }

          }

          if (screen_msgs[a].ssn == 3) // health -
          {
             --screen_msgs[a].delay;

              if (screen_msgs[a].current_step == 1) // immediate next
              {
                 if (screen_msgs[a].delay == 0) // next out
                 {
                    screen_msgs[a].delay = 5;
                    screen_msgs[a].color = 10;
                    screen_msgs[a].yinc = -8;
                    screen_msgs[a].size = .5;
                    screen_msgs[a].size_inc = .11;
                    screen_msgs[a].rot_inc = 0;
                    screen_msgs[a].current_step = 2;
                 }
              }
              if (screen_msgs[a].current_step == 2)
              {
                 if (screen_msgs[a].delay == 0) // next wait
                 {
                    screen_msgs[a].delay = 30;
                    screen_msgs[a].yinc = 0;
                    screen_msgs[a].size_inc = 0;
                    screen_msgs[a].current_step = 3;
                 }
              }

              if (screen_msgs[a].current_step == 3)
              {


                 if (screen_msgs[a].delay == 0) // next in
                 {
                    screen_msgs[a].delay = 6;
                    screen_msgs[a].yinc = 4;
                    screen_msgs[a].size_inc = -.14;
                    screen_msgs[a].current_step = 8;
                 }
              }
              if (screen_msgs[a].current_step == 8)
              {
                 if (screen_msgs[a].delay == 0)
                 {
                    screen_msgs[a].active = 0;  // done
                 }
              }

          }

           if (screen_msgs[a].ssn == 1) // door (fade in and out)
           {
             --screen_msgs[a].delay;

              if (screen_msgs[a].current_step == 1) // immediate next
              {
                 if (screen_msgs[a].delay == 0) // fade in
                 {
                    screen_msgs[a].delay = 6;

                    screen_msgs[a].color = 12 + (6 * 32);

                    screen_msgs[a].color_inc = - 32;

//                  screen_msgs[a].color_inc_holdoff = screen_msgs[a].delay / 12;
                    screen_msgs[a].color_inc_holdoff = 1;

                    screen_msgs[a].current_holdoff = screen_msgs[a].color_inc_holdoff;

                    screen_msgs[a].size = 1.0;
                    screen_msgs[a].size_inc = 0;
                    screen_msgs[a].current_step = 2;
                 }
              }
              if (screen_msgs[a].current_step == 2)
              {
                 if (screen_msgs[a].delay == 0) // next wait
                 {
                    screen_msgs[a].delay = 30;
                    screen_msgs[a].color = 12;
                    screen_msgs[a].color_inc = 0;
                    screen_msgs[a].current_step = 3;
                 }
              }

              if (screen_msgs[a].current_step == 3)
              {
                 if (screen_msgs[a].delay == 0) // fade out
                 {
                    screen_msgs[a].delay = 15;
                    screen_msgs[a].color = 12;
                    screen_msgs[a].color_inc = 16;
                    screen_msgs[a].color_inc_holdoff = 1;

                    screen_msgs[a].current_holdoff = screen_msgs[a].color_inc_holdoff;


                    screen_msgs[a].current_step = 4;
                 }
              }

              if (screen_msgs[a].current_step == 4)
              {
                 if (screen_msgs[a].delay == 0)
                 {
                    screen_msgs[a].active = 0;  // done
                 }
              }
          }

          // increment the size
          screen_msgs[a].size = screen_msgs[a].size + screen_msgs[a].size_inc;

          // increment the rot
          screen_msgs[a].rot = screen_msgs[a].rot + screen_msgs[a].rot_inc;

          // increment x and y
          screen_msgs[a].x = screen_msgs[a].x + (int) screen_msgs[a].xinc;
          screen_msgs[a].y = screen_msgs[a].y + (int) screen_msgs[a].yinc;

          // increment color
          if (screen_msgs[a].color_inc) // do nothing if no color_inc
          {
             if (--screen_msgs[a].current_holdoff <= 0)
             {
                screen_msgs[a].color += screen_msgs[a].color_inc;

                if (screen_msgs[a].color < 0) screen_msgs[a].color += 256;
                if (screen_msgs[a].color < 255) screen_msgs[a].color -= 256;

                screen_msgs[a].current_holdoff = screen_msgs[a].color_inc_holdoff;
             }
          }

       }
}


void new_bmsg(char *nb)
{
    slide_bmsg();
    sprintf(b_msg[0], "%s", nb);
    bottom_msg = 120;
    erase_last_bmsg();
}

void draw_bottom_msg()
{
   int type = 2;
   if (type == 1) // old style may cause low fps, due to scaled drawing and target bitmap changes
   {
      if (--bottom_msg > 0)
      {
         int a;
         int nb = 8;    // NUM_BOTTOM_MSG_LINES

         float chs = 1.3; // current h size
         float hss = .15;  // h size step

         float cvs = 1.3; // current v size
         float vss = .15;  // v size step

         int cc = 15;  // current color
         int ci = 32;  // color inc use  only 16, 32, 48, 64 etc

         float ypos = SCREEN_H - BORDER_WIDTH - (chs*8);
         for (a=0; a< nb; a++)
         {
            mtextout_centre(b_msg[a], SCREEN_W/2, (int)ypos, cvs, chs, cc);
            chs = chs - hss;
            cvs = cvs - vss;
            cc = cc + ci;
            float xh = (chs*8);
            ypos -= xh;
         }
      }
      else bottom_msg = 0;
   }
   if (type == 2) // new style - faster but not as nice looking
   {
      if (--bottom_msg > 0)
      {
         int a;
         int nb = 3;    // NUM_BOTTOM_MSG_LINES

         int cc = 15;  // current color
         int ci = 32;  // color inc use  only 16, 32, 48, 64 etc

         float ypos = SCREEN_H - 10;
         for (a=0; a< nb; a++)
         {
            al_draw_text(font, palette_color[cc], SCREEN_W/2, ypos, ALLEGRO_ALIGN_CENTER, b_msg[a]);
            cc = cc + ci;
            if (a == 0) cc = cc + ci*4;
            ypos -= 8;
         }
      }
      else bottom_msg = 0;
   }
}






void game_event(int ev, int x, int y, int z1, int z2, int z3, int z4)
{
   int bottom_msg_on = 1;
   int screen_messages_on = 1;
   if (sound_on)
   {
      switch (ev)
      {
         /*  sample numbers
         0 - player shoots
         1 - d'OH
         2 - bonus
         3 - hiss
         4 - la dee dah  door, key, exit
         5 - explosion
         6 - grunt 1 shot
         7 - grunt 2 hit
         8 - enemy killed  */
        case  1: // player shoots
           al_play_sample(snd[0], 0.71, 0, .8, ALLEGRO_PLAYMODE_ONCE, NULL);
        break;
        case  2: case  4: case  5: // la dee dah (key, exit, door)
           if (sample_delay[4]+30 < frame_num)
           {
              sample_delay[4] = frame_num;
              al_play_sample(snd[4], 0.78, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
           }
        break;
        case  6: case  9: // health bonus and free man
           if (sample_delay[2]+30 < frame_num)
           {
              sample_delay[2] = frame_num;
              al_play_sample(snd[2], 0.78, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
           }
        break;
        case 11: // player got shot
           al_play_sample(snd[6], 0.5, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        break;
        case 8: case 10: case 12: case 34: case 35: // player got hit (bomb, mine, enemy collision, squished, stuck)
           if (sample_delay[7]+14 < frame_num)
           {
              sample_delay[7] = frame_num;
              al_play_sample(snd[7], 0.5, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
           }
        break;
        case 13: // enemy killed
             al_play_sample(snd[8], 0.5, 0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
        break;
        case 21: // d'Oh (player died)
             al_play_sample(snd[1], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        break;
        case 22: // explosion
             al_play_sample(snd[5], .78, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        break;
      }
   }

   if (bottom_msg_on)
   {
      switch (ev)
      {
         //case  1:/* player bullet fired */ break;
         case  2: new_bmsg((char*)"You got a key!"); break;
         case  3:
            if (z1 == 1) sprintf(msg, "1 enemy left to kill!!");
            else sprintf(msg, "%d enemies left to kill!", z1);
            new_bmsg(msg);
         break;
         //case  4: /* level_done  */ break;
         case  5: new_bmsg((char*)"Door!"); break;
         case  6: sprintf(msg, "Health + %d", z1 ); new_bmsg(msg); break;
         case  7: sprintf(msg, "Health - %d", z1 ); new_bmsg(msg); break;
         case  8: sprintf(msg, "Bomb Damage %d", z1 ); new_bmsg(msg); break;
         case  9: new_bmsg((char*)"Wahoo! You got a Free Man!"); break;
         case 10: new_bmsg((char*)"You hit a Mine!"); break;
         case 11: new_bmsg((char*)"You got shot!"); break;
         case 12: sprintf(msg, "You got hit by %s!", enemy_name[Ei[z1][0]]); new_bmsg(msg); break;
         case 13: sprintf(msg, "%s killed!...%d enemies left", enemy_name[Ei[z1][0]],  num_enemy-1); new_bmsg(msg); break;
         //case 14: /* cloner cloned something */ break;
         //case 15: /* jump  */ break;
         //case 16: /* arrow fired */ break;
         //case 17: /* green fired */ break;
         //case 18: /* cannonball  */ break;
         //case 19: /* twirly      */ break;
         case 21: new_bmsg((char*)"You Died!"); break;
         //case 22: /* explosion */ break;
         case 23: new_bmsg((char*)"Enemy killed by bomb!"); break;
         case 24: sprintf(msg, "#%d bomb with %d second fuse",item[x][7]/20, item[x][9]/10 );  new_bmsg(msg); break;
         case 25: new_bmsg((char*)"Rocket!"); break;
         case 26: new_bmsg((char*)"You already have perfect health!"); break;
         case 30: new_bmsg((char*)"You got a Switch"); break;
         case 31: new_bmsg((char*)"Sproingy!"); break;
         case 32: new_bmsg((char*)"Door!"); break;
         //case 33: /* Out Door */ break;
         case 34: new_bmsg((char*)"You got squished!"); break;
         case 35: new_bmsg((char*)"You got stuck!"); break;
      }
   }
   if (screen_messages_on)
      switch (ev)
      {
//         case   1: add_screen_msg("bang!", x, y, 100, 4, z1, z2, z3, z4); break;
//         case   2: add_screen_msg("key", x, y, 100, 5, z1, z2, z3, z4); break;
//        case   3: sprintf(msg, "%d enemies left to kill!", z1);
//                  add_screen_msg(msg, x, y, 100, 0, z1, z2, z3, z4); break;
//         case   3: add_screen_msg("------O------", x, y, 100, 0); break;
//         case   5: add_screen_msg("--O--", x, y, 100, 0); break;
//         case   5: add_screen_msg("Door", x, y, 100, 1, z1, z2, z3, z4); break;
         case   6:  sprintf(msg, "H+%d", z1);
                    add_screen_msg(msg, x, y, 100, 2, z1, z2, z3, z4); break;
         case   7:  sprintf(msg, "H-%d", z1);
                    add_screen_msg(msg, x, y, 100, 3, z1, z2, z3, z4); break;
//         case  8: add_screen_msg("Bomb Damage!", x, y, 100, 0, z1, z2, z3, z4); break;
//         case   10: add_screen_msg("Mine", x, y, 100, 0, z1, z2, z3, z4); break;
//         case  13: sprintf(msg, "%s died! (%d left)", enemy_name[Ei[z1][0]],  num_enemy-1);
//                   add_screen_msg(msg, x, y, 100, 0, z1, z2, z3, z4); break;
//         case  21: add_screen_msg("You Died!", x, y, 100, 0, z1, z2, z3, z4); break;
//         case  30: add_screen_msg("switch", x, y, 100, 7, z1, z2, z3, z4); break;
//         case  31: add_screen_msg("boing!", x, y, 100, 6, z1, z2, z3, z4); break;
//         case  32: add_screen_msg("In", x, y, 100, 1, z1, z2, z3, z4); break;
//         case  33: add_screen_msg("Out", x, y, 100, 1, z1, z2, z3, z4); break;
//         case  34: add_screen_msg("Ouch", x, y, 100, 34, z1, z2, z3, z4); break;
//         case  35: add_screen_msg("Ouch", x, y, 100, 35, z1, z2, z3, z4); break;
      }
}
















