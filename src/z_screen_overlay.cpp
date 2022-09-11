// z_screen_overlay.cpp

#include "pm.h"



int dif_from_now_to_nl()
{
   int min_nl = 9999999;
   for (int x=game_move_entry_pos; x>0; x--)  // look back
      if (game_moves[x][1] == 7)
      {
         if (game_moves[x][0] < min_nl) min_nl = game_moves[x][0];
      }
   if (min_nl < 9999999) return min_nl - frame_num;
   return 0;
}



void show_player_stat_box(int tx, int y, int p)
{

   al_draw_filled_rectangle(tx, y, tx+200, y+50, palette_color[0]);
   al_draw_rectangle(       tx, y, tx+200, y+50, palette_color[15], 1);
   al_draw_rectangle(       tx, y, tx+200, y+11, palette_color[15], 1);


   y+=2;

   int c = 15;

   if ((ima_server) || (ima_client))
      al_draw_textf(font, palette_color[c], tx+2, y, 0, "Player:%d [%s]", p, players1[p].hostname);
   else al_draw_textf(font, palette_color[c], tx+2, y, 0, "Player:%d", p);





   if ((level_done_mode == 5) && (!has_player_acknowledged(p)))
   {
      c = flash_color;
      int pay = 16;
      al_draw_textf(font, palette_color[c], tx+158, y+pay, 0, "press");
      al_draw_textf(font, palette_color[c], tx+158, y+pay+8, 0, " any");
      int tl = (dif_from_now_to_nl()+30)/40;
      if (tl > 9) al_draw_textf(font, palette_color[c], tx+154, y+pay+18, 0, "  %2d", tl);
      else        al_draw_textf(font, palette_color[c], tx+158, y+pay+18, 0, "  %d", tl);
   }
   if ((level_done_mode == 5) && (has_player_acknowledged(p))) al_draw_textf(font, palette_color[15], tx+158, y+20, 0, "ready");


   y += 12;

   al_draw_bitmap(player_tile[players[p].color][1], tx+2, y, 0); // draw shape


   c = 15;
   al_draw_textf(font, palette_color[15], tx+24, y, 0, "Deaths:%d", players[p].stat_respawns); y+=8;
   al_draw_textf(font, palette_color[15], tx+24, y, 0, "Shots:%d", players[p].stat_bullets_fired); y+=8;
   al_draw_textf(font, palette_color[15], tx+24, y, 0, "Enemy Hits:%d", players[p].stat_enemy_hits); y+=8;
   al_draw_textf(font, palette_color[15], tx+24, y, 0, "Purple Coins:%d/%d", players[p].stat_purple_coins, number_of_purple_coins); y+=8;

}



void show_level_done(void)
{
   draw_large_text_overlay(2, 0);
   process_flash_color();

   int x = SCREEN_W/2;
   int y = SCREEN_H/2;

   int np = 0; // number of active players
   for (int p=0; p<NUM_PLAYERS; p++) if (players[p].active) np++;

   int tx=0, ty=0, pc=0;
   if (np == 1)
   {
      tx = x-100;
      ty = y-25;
      for (int p=0; p<NUM_PLAYERS; p++)
         if (players[p].active)
         {
            show_player_stat_box(tx, ty, p);
            ty += 56;
         }
   }
   if (np == 2)
   {
      int tx = x-200;
      int ty = y-25;
      for (int p=0; p<NUM_PLAYERS; p++)
         if (players[p].active)
         {
            show_player_stat_box(tx, ty, p);
            tx = x+5;
         }
   }
   if (np == 3)
   {
      for (int p=0; p<NUM_PLAYERS; p++)
         if (players[p].active)
         {
            if (pc == 0) { tx = x-100; ty = y-80; }
            if (pc == 1) { tx = x-100; ty = y-25; }
            if (pc == 2) { tx = x-100; ty = y+30; }
            show_player_stat_box(tx, ty, p);
            pc++;
         }
   }
   if (np == 4)
   {
      for (int p=0; p<NUM_PLAYERS; p++)
         if (players[p].active)
         {
            if (pc == 0) { tx = x+-200; ty = y-50; }
            if (pc == 1) { tx = x+5;    ty = y-50; }
            if (pc == 2) { tx = x+-200; ty = y+5; }
            if (pc == 3) { tx = x+5;    ty = y+5; }
            show_player_stat_box(tx, ty, p);
            pc++;
         }
   }
   if ((np == 5) || (np == 6))
   {
      for (int p=0; p<NUM_PLAYERS; p++)
         if (players[p].active)
         {
            if (pc == 0) { tx = x-200; ty = y-80; }
            if (pc == 1) { tx = x+5;   ty = y-80; }
            if (pc == 2) { tx = x-200; ty = y-25; }
            if (pc == 3) { tx = x+5;   ty = y-25; }
            if (pc == 4) { tx = x-200; ty = y+30; }
            if (pc == 5) { tx = x+5;   ty = y+30; }
            show_player_stat_box(tx, ty, p);
            pc++;
         }
   }
   if ((np == 7) || (np == 8))
   {
      for (int p=0; p<NUM_PLAYERS; p++)
         if (players[p].active)
         {
            if (pc == 0) { tx = x-200; ty = y-105; }
            if (pc == 1) { tx = x+5;   ty = y-105; }
            if (pc == 2) { tx = x-200; ty = y-50; }
            if (pc == 3) { tx = x+5;   ty = y-50; }
            if (pc == 4) { tx = x-200; ty = y+5; }
            if (pc == 5) { tx = x+5;   ty = y+5; }
            if (pc == 6) { tx = x-200; ty = y+60; }
            if (pc == 7) { tx = x+5;   ty = y+60; }
            show_player_stat_box(tx, ty, p);
            pc++;
         }
   }
}










void draw_screen_overlay(void)
{
   al_set_target_backbuffer(display);
   if (level_done_mode) show_level_done();
   if (speed_testing) draw_speed_test_data();
   draw_top_display();
   draw_bmsg();
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

   if (demo_mode_on) draw_large_text_overlay(3, 15);

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
   al_draw_textf(font, palette_color[16], tdx+44, tdy+2, ALLEGRO_ALIGN_CENTRE,"Health:%-2d", al_fixtoi(players[p].LIFE));
   tdx += 88;

   // draw purple coins
   al_draw_scaled_bitmap(tile[197], 0, 0, 19, 19, tdx+8, tdy+1, 10, 10, 0);
   // spin_shape(197, tdx+5, tdy-3, 0, 0, 19, 19, 0.6, 0.5, 60);
   al_draw_textf(font, palette_color[14], tdx+17, tdy+2, 0, ":%d/%d", players[active_local_player].stat_purple_coins, number_of_purple_coins);


   if (show_scale_factor > 0)
   {
      al_draw_textf(font, palette_color[15],SCREEN_W*2/3, 2, ALLEGRO_ALIGN_CENTER, "Scale:%-3.2f", scale_factor);
   }

   if (show_dtd > 0)
   {
      show_dtd --;
      if (saved_display_transform_double) al_draw_textf(font, palette_color[15],SCREEN_W*4/5, 2, ALLEGRO_ALIGN_CENTER, "dtd:%d [f]", display_transform_double);
      else                                al_draw_textf(font, palette_color[15],SCREEN_W*4/5, 2, ALLEGRO_ALIGN_CENTER, "dtd:%d [a]", display_transform_double);
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
//      al_draw_textf(font, palette_color[color], cx, cy+=8, 0, "full:%d display_transform_double:%d saved_dtd:%d", fullscreen, display_transform_double, saved_display_transform_double);
      al_draw_textf(font, palette_color[color], cx, cy+=8, 0, "full:%d dtd:%d saved_dtd:%d", fullscreen, display_transform_double, saved_display_transform_double);
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

            if (players[p].control_method != 2) players1[p].server_last_sdak_rx_frame_num = frame_num; // only do sync for active clients

            sprintf(msg, "[%d][%2d][%d][%2d][%d][%4d][%4d][%4d][%5d][%5d][%2d][%4d][%4.1f][%4.1f][%4d][%4d]",
                                              p,
                                              players1[p].who,
                                              players[p].active,
                                              players[p].color,
                                              players[p].control_method,
                                              players1[p].server_sync,
                                              frame_num - players1[p].server_last_sdak_rx_frame_num,
                                              players1[p].stdf_late,
                                              srv_client_state_frame_num[p][0],
                                              srv_client_state_frame_num[p][1],
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

         sprintf(msg, "[%d]", players1[p].client_sync);
         rtextout_centre(NULL, msg, SCREEN_W/2, SCREEN_H/2, color, 4, 0, 1);
      }


      int bdx = BORDER_WIDTH;
      int bdy = SCREEN_H - 10;
      int ts = 0;  // text spacing

      sprintf(msg, "Netgame Client (%s) ", local_hostname );
      al_draw_text(font, palette_color[14], bdx, bdy, 0, msg);
      ts += strlen(msg)*8;

      sprintf(msg, "sync:%d ", players1[p].client_sync);
      al_draw_text(font, palette_color[14], bdx + ts, bdy, 0, msg);
      ts += strlen(msg)*8;

      if (show_debug_overlay) // show a bunch of data for troubleshooting
      {
         int tx = BORDER_WIDTH;
         int ty1 = 80;

         if (players1[p].stdf_rx > 0)
         {
            sprintf(msg, "stdf rx'd:%d on_time:%d late:%d [%d]",
                          players1[p].stdf_rx, players1[p].stdf_on_time,
                          players1[p].stdf_late, (players1[p].stdf_on_time * 100) / players1[p].stdf_rx);
            al_draw_filled_rectangle(tx, ty1+8, tx+strlen(msg)*8, ty1+16, palette_color[0]);
            al_draw_text(font, palette_color[15], tx, ty1+=8, 0, msg);
         }

         sprintf(msg, "dif corrections:%d ", players1[p].dif_corr);
         al_draw_filled_rectangle(tx, ty1+8, tx+strlen(msg)*8, ty1+16, palette_color[0]);
         al_draw_text(font, palette_color[15], tx, ty1+=8, 0, msg);

         sprintf(msg, "late cdats dropped by server:%d", players1[p].serr_c_sync_err);
         al_draw_filled_rectangle(tx, ty1+8, tx+strlen(msg)*8, ty1+16, palette_color[0]);
         al_draw_text(font, palette_color[15], tx, ty1+=8, 0, msg);

         sprintf(msg, "game moves lead from server:%d min:%d err:%d", players1[p].client_game_move_sync, players1[p].client_game_move_sync_min, players1[p].client_game_move_sync_err);
         al_draw_filled_rectangle(tx, ty1+8, tx+strlen(msg)*8, ty1+16, palette_color[0]);
         al_draw_text(font, palette_color[15], tx, ty1+=8, 0, msg);

         sprintf(msg, "server sync:%d ", players1[p].client_sync);
         al_draw_filled_rectangle(tx, ty1+8, tx+strlen(msg)*8, ty1+16, palette_color[0]);
         al_draw_text(font, palette_color[15], tx, ty1+=8, 0, msg);

         sprintf(msg, "moves:%d", game_move_entry_pos);
         al_draw_filled_rectangle(tx, ty1+8, tx+strlen(msg)*8, ty1+16, palette_color[0]);
         al_draw_text(font, palette_color[15], tx, ty1+=8, 0, msg);

         sprintf(msg, "moves skipped:%d", players1[p].moves_skipped);
         al_draw_filled_rectangle(tx, ty1+8, tx+strlen(msg)*8, ty1+16, palette_color[0]);
         al_draw_text(font, palette_color[15], tx, ty1+=8, 0, msg);

         sprintf(msg, "moves skipped per second :%d", players1[p].moves_skipped_last_tally);
         al_draw_filled_rectangle(tx, ty1+8, tx+strlen(msg)*8, ty1+16, palette_color[0]);
         al_draw_text(font, palette_color[15], tx, ty1+=8, 0, msg);

         sprintf(msg, "sdat skipped:%d", players1[p].client_sdat_packets_skipped);
         al_draw_filled_rectangle(tx, ty1+8, tx+strlen(msg)*8, ty1+16, palette_color[0]);
         al_draw_text(font, palette_color[15], tx, ty1+=8, 0, msg);

         sprintf(msg, "frames_skipped:%d", players1[p].frames_skipped);
         al_draw_filled_rectangle(tx, ty1+8, tx+strlen(msg)*8, ty1+16, palette_color[0]);
         al_draw_text(font, palette_color[15], tx, ty1+=8, 0, msg);


         ty1 +=8;

         sprintf(msg, "bandwidth (bytes per second)");
         al_draw_filled_rectangle(tx, ty1+8, tx+strlen(msg)*8, ty1+16, palette_color[0]);
         al_draw_text(font, palette_color[15], tx, ty1+=8, 0, msg);

         sprintf(msg, "TX currrent:[%d] max:[%d]", players1[p].tx_bytes_per_tally, players1[p].tx_max_bytes_per_tally);
         al_draw_filled_rectangle(tx, ty1+8, tx+strlen(msg)*8, ty1+16, palette_color[0]);
         al_draw_text(font, palette_color[15], tx, ty1+=8, 0, msg);

         sprintf(msg, "RX currrent:[%d] max:[%d]", players1[p].rx_bytes_per_tally, players1[p].rx_max_bytes_per_tally);
         al_draw_filled_rectangle(tx, ty1+8, tx+strlen(msg)*8, ty1+16, palette_color[0]);
         al_draw_text(font, palette_color[15], tx, ty1+=8, 0, msg);

         ty1 +=8;

         sprintf(msg, "packets per second");
         al_draw_filled_rectangle(tx, ty1+8, tx+strlen(msg)*8, ty1+16, palette_color[0]);
         al_draw_text(font, palette_color[15], tx, ty1+=8, 0, msg);

         sprintf(msg, "TX currrent:[%d] max:[%d]", players1[p].tx_packets_per_tally, players1[p].tx_max_packets_per_tally);
         al_draw_filled_rectangle(tx, ty1+8, tx+strlen(msg)*8, ty1+16, palette_color[0]);
         al_draw_text(font, palette_color[15], tx, ty1+=8, 0, msg);

         sprintf(msg, "RX currrent:[%d] max:[%d]", players1[p].rx_packets_per_tally, players1[p].rx_max_packets_per_tally);
         al_draw_filled_rectangle(tx, ty1+8, tx+strlen(msg)*8, ty1+16, palette_color[0]);
         al_draw_text(font, palette_color[15], tx, ty1+=8, 0, msg);


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
            if (players1[p].server_game_move_sync_err)  color = 10;
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

   if (players[0].control_method == 1) // file play
   {
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

      sprintf(msg, "[%d%%] ", frame_num*100/demo_mode_last_pc);
      al_draw_text(font, palette_color[14], bdx + ts, bdy, 0, msg);
      ts += strlen(msg)*8;
      if (show_debug_overlay)
      {
         sprintf(msg, "Time:[%d%%] ", frame_num*100/demo_mode_last_pc);
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


void game_event(int ev, int x, int y, int z1, int z2, int z3, int z4)
{
   if (bottom_msg_on) new_bmsg(ev, x, y, z1, z2, z3, z4); // pass everything to new_bmgs and do it there
   if (sound_on)
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
      switch (ev)
      {
         // al_play_sample(ALLEGRO_SAMPLE *spl, float gain, float pan, float speed, ALLEGRO_PLAYMODE loop, ALLEGRO_SAMPLE_ID *ret_id)
         case  1: // player shoots
            //al_play_sample(snd[0], 0.71, 0, .8, ALLEGRO_PLAYMODE_ONCE, NULL);
            al_play_sample(snd[0], 0.81, 0, .7, ALLEGRO_PLAYMODE_ONCE, NULL);
         break;
         case  2: case  4: case  5: // la dee dah (key, exit, door)
            if (sample_delay[4]+30 < frame_num)
            {
               sample_delay[4] = frame_num;
               al_play_sample(snd[4], 0.78, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
         break;
         case 70: case 71: // free man and purple coin
           if (sample_delay[2]+30 < frame_num)
            {
               sample_delay[2] = frame_num;
               al_play_sample(snd[2], 0.78, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
         break;
         case 40: case 41: case 43:// player got shot
            al_play_sample(snd[6], 0.5, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
         break;
         case 44: case 50: case 52: case 54: case 56: case 57: case 59: // player got hit (bomb, mine, enemy collision, squished, stuck)
            if (sample_delay[7]+14 < frame_num)
            {
               sample_delay[7] = frame_num;
               al_play_sample(snd[7], 0.5, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
         break;
         case 60: case 62: case 64: // enemy killed
            al_play_sample(snd[8], 0.5, 0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
         break;
         case 90: // d'Oh (player died)
            al_play_sample(snd[1], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
         break;
         case 22: // explosion
            al_play_sample(snd[5], .78, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
         break;

         case 31: // sproingy
            al_play_sample(snd[9], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
         break;
      }
   }
}


void dtextout(const char *txt1, int x, int y, int col)
{
   int sw = strlen(txt1) * 8;      // string length in pixels
   int sh = 8;                     // string height in pixels

   ALLEGRO_BITMAP *temps = NULL;
   temps = al_create_bitmap(sw,sh);
   al_set_target_bitmap(temps);
   al_clear_to_color(al_map_rgb(0,0,0));

   al_draw_text(font, palette_color[col], 0, 0, 0, txt1 );

   al_set_target_bitmap(bmsg_temp);
   al_draw_scaled_bitmap(temps, 0, 0, sw, sh, x, y, sw*2, sh*2, 0);
   al_destroy_bitmap(temps);
}








void clear_bmsg(void)
{
   for (int c=0; c<20; c++)
   {
      al_set_target_bitmap(bmsg_bmp[c]);
      al_clear_to_color(al_map_rgb(0, 0, 0));
      al_convert_mask_to_alpha(bmsg_bmp[c], al_map_rgb(0, 0, 0)) ;
   }
   bmsg_index = 0;
   bottom_msg = 0;

  for (int i=0; i<10; i++)
     game_event_retrigger_holdoff[i] = 0;
}

int bmsg_show_text(const char *txt, int col, int bmsg_length)
{
   dtextout(txt, bmsg_length, 2, col);
   return (strlen(txt)*16);
}

int bmsg_draw_tile(int tn, int bmsg_length)
{
   al_draw_bitmap(tile[tn], bmsg_length, 0, 0);
   return 20;
}

int bmsg_draw_tile2(int tn, int bmsg_length, int xo, int yo)
{
   al_draw_bitmap(tile[tn], bmsg_length+xo, +yo, 0);
   return 20;
}


int bmsg_draw_player(int p, int bmsg_length)
{
   int len = 0;
   if (0) // 'Player x'
   {
      sprintf(msg, "Player %d", p);
      len += bmsg_show_text(msg, players[p].color, bmsg_length);
   }

   if (1) // player tile
   {
      al_draw_bitmap(player_tile[players[p].color][1], bmsg_length + len, 0, 0); // draw shape
      len += 20;
   }

   if (0) // 'Player x' and tile
   {
      sprintf(msg, "Player %d ", p);
      len += bmsg_show_text(msg, players[p].color, bmsg_length + len);
      al_draw_bitmap(player_tile[players[p].color][1], bmsg_length + len, 0, 0); // draw shape
      len += 20;
   }
   return len;
}

int bmsg_draw_enemy(int e_type, int bmsg_length)
{
   if (0) // enemy name
   {
      al_draw_textf(font, palette_color[15], bmsg_length*8, 0, 0, "%s", enemy_name[e_type]);
      return (strlen(enemy_name[e_type])*16);
   }

   if (1) // enemy tile
   {
      bmsg_draw_tile(enemy_tile[e_type], bmsg_length);
      return 20;
   }
}

int bmsg_show_health(int h, int bmsg_length)
{
   int col = 9; // green
   if (h < 0) col = 10; // red
   if (h == 0) col = 13; // blue
   sprintf(msg, " Health %+d", h);
   bmsg_length += bmsg_show_text(msg, col, bmsg_length);
   return (strlen(msg)*16);
}

void new_bmsg(int ev, int x, int y, int z1, int z2, int z3, int z4)
{

   // event retrigger holdoff for these events that can repeat every frame
   if (ev == 3) // exit
   {
      if (game_event_retrigger_holdoff[1] < frame_num) game_event_retrigger_holdoff[1] = frame_num + 60;
      else ev = 0;
   }

   if (ev == 50) // mine
   {
      if (game_event_retrigger_holdoff[2] < frame_num) game_event_retrigger_holdoff[2] = frame_num + 20;
      else ev = 0;
   }

   if ((ev == 57) || (ev == 59)) // raw damage that needs to be tallied
   {
      int p = z1;

      float damage = 0;
      if (ev == 59) damage = (float)item[z2][15] / 100; // damage from item 17 - damage

      //players1[p].field_damage_enemy_number = z2;

      if (players1[p].field_damage_holdoff < frame_num) // triggered and not in holdoff
      {
         players1[p].field_damage_holdoff = frame_num + 20; // set holdoff
         players1[p].field_damage_tally = damage; // init tally with current damage
         ev = 0; // don't let this event do anything
      }

      if (players1[p].field_damage_holdoff > frame_num) // triggered and in holdoff
      {
         players1[p].field_damage_tally += damage; // inc tally with current damage
         ev = 0; // don't let this event do anything
      }
   }


   if ((ev != 0) && (ev != 1) && (ev != 4) && (ev != 15) && (ev != 22) && (ev != 31) ) // events that don't have bmsg handler
   {
      int bmsg_length = 0; // keep a running total
      int custom_drawn = 0;

      bmsg_temp = al_create_bitmap(800, 20); // create a temp bitmap to build a single line
      al_set_target_bitmap(bmsg_temp);
      al_clear_to_color(al_map_rgb(0, 0, 0));

      if (ev != 64) bmsg_length += bmsg_draw_player(z1, bmsg_length); // all bmsg start with player
      // except for 64


      if (ev == 5) // player went through a door
      {
         custom_drawn = 1;
         bmsg_length += bmsg_show_text(" went through a door", 15, bmsg_length);
      }
      if (ev == 24) // player lit bomb
      {
         custom_drawn = 1;
         bmsg_length += bmsg_show_text(" lit a bomb ", 15, bmsg_length);
         bmsg_length += bmsg_draw_tile(465, bmsg_length);
         sprintf(msg, " with %d sec fuse",  z3);
         bmsg_length += bmsg_show_text(msg, 15, bmsg_length);
      }
      if (ev == 25) // player armed a bomb with a remote detonator
      {
         custom_drawn = 1;
         bmsg_length += bmsg_show_text(" armed a bomb ", 15, bmsg_length);
         bmsg_length += bmsg_draw_tile(538, bmsg_length);
         bmsg_length += bmsg_show_text(" with remote detonator ", 15, bmsg_length);
         bmsg_length += bmsg_draw_tile(539, bmsg_length);
      }
      if (ev == 26) // player lit rocket
      {
         custom_drawn = 1;
         bmsg_length += bmsg_show_text(" lit a rocket ", 15, bmsg_length);
         bmsg_length += bmsg_draw_tile(249, bmsg_length);
      }
      if (ev == 3) // player tried to exit
      {
         custom_drawn = 1;
         if (z3 == 1) sprintf(msg, " tried to exit with 1 enemy left");
         else sprintf(msg, " tried to exit with %d enemies left",  z3);
         bmsg_length += bmsg_show_text(msg, 15, bmsg_length);
      }
      if (ev == 2) // key
      {
         custom_drawn = 1;
         bmsg_length += bmsg_show_text(" got a ", 15, bmsg_length);

         int c1 =0;  // color
         int tn = 0; // tile_numer
         int k = item[z2][1] - 1039;
         char tmsg[20] = {0};

         if (k == 0) { sprintf(tmsg, "red");    c1 = 10; tn = 272; } // red
         if (k == 1) { sprintf(tmsg, "green");  c1 = 11; tn = 279; } // green
         if (k == 2) { sprintf(tmsg, "blue");   c1 = 13; tn = 288; } // blue
         if (k == 3) { sprintf(tmsg, "purple"); c1 = 8;  tn = 295; } // purple

         bmsg_length += bmsg_show_text(tmsg, c1, bmsg_length);
         bmsg_length += bmsg_show_text(" key ", 15, bmsg_length);
         bmsg_length += bmsg_draw_tile2(tn, bmsg_length, -10, 0);
      }

      if (ev == 30) // switch
      {
         custom_drawn = 1;
         bmsg_length += bmsg_show_text(" flipped a switch ", 15, bmsg_length);
         bmsg_length += bmsg_draw_tile2(item[z2][1], bmsg_length, -10, -2);
      }

//      if (ev == 30) // switch
//      {
//         custom_drawn = 1;
//         bmsg_length += bmsg_show_text("012345678901234567890123456789012345678901234567Z-", 15, bmsg_length);
//      }



      if (ev == 40) // player got shot by another player
      {
         custom_drawn = 1;
         bmsg_length += bmsg_show_text(" got shot by ", 15, bmsg_length);
         bmsg_length += bmsg_draw_player(z2, bmsg_length);
         bmsg_length += bmsg_show_health(-z4, bmsg_length);
      }
      if (ev == 41) // player shot themself
      {
         custom_drawn = 1;
         bmsg_length += bmsg_show_text(" shot themself!", 15, bmsg_length);
         bmsg_length += bmsg_show_health(-z4, bmsg_length);
      }
      if (ev == 43) // player got shot by enemy
      {
         custom_drawn = 1;
         bmsg_length += bmsg_show_text(" got shot by ", 15, bmsg_length);
         bmsg_length += bmsg_draw_enemy(z2, bmsg_length);
         bmsg_length += bmsg_show_health(-z4, bmsg_length);
      }
      if (ev == 44) // player got shot by bullet
      {
         custom_drawn = 1;
         bmsg_length += bmsg_show_text(" got hit by ", 15, bmsg_length);
         bmsg_length += bmsg_draw_enemy(Ei[z2][0], bmsg_length);
         bmsg_length += bmsg_show_health(-z4, bmsg_length);
      }
      if (ev == 50) // player hit a mine
      {
         custom_drawn = 1;
         bmsg_length += bmsg_show_text(" hit a mine ", 15, bmsg_length);
         bmsg_length += bmsg_draw_tile(456, bmsg_length);
         bmsg_length += bmsg_show_health(-z4, bmsg_length);
      }
      if (ev == 52) // player exploded another player
      {
         custom_drawn = 1;
         bmsg_length += bmsg_show_text(" exploded ", 15, bmsg_length);
         bmsg_length += bmsg_draw_player(z2, bmsg_length);
         bmsg_length += bmsg_show_health(-z4, bmsg_length);
      }
      if (ev == 53) // player exploded themself
      {
         custom_drawn = 1;
         bmsg_length += bmsg_show_text(" exploded themself ", 15, bmsg_length);
         bmsg_length += bmsg_show_health(-z4, bmsg_length);
      }
      if (ev == 54) // player got squished
      {
         custom_drawn = 1;
         bmsg_length += bmsg_show_text(" got squished ", 15, bmsg_length);
         bmsg_length += bmsg_show_health(-z4, bmsg_length);
      }
      if (ev == 56) // player got stuck
      {
         custom_drawn = 1;
         bmsg_length += bmsg_show_text(" got stuck ", 15, bmsg_length);
         bmsg_length += bmsg_show_health(-z4, bmsg_length);
      }

      if (ev == 58) // player took damage from a field
      {
         custom_drawn = 1;
         int damage = (int) players1[z1].field_damage_tally;
         int e = players1[z1].field_damage_enemy_number;
         int type = Ei[e][19];

         if (type == 1) bmsg_length += bmsg_show_text(" was hurt by spikey floor ", 15, bmsg_length);
         else           bmsg_length += bmsg_show_text(" was hurt by a damage field ", 15, bmsg_length);
         bmsg_length += bmsg_show_health(-damage, bmsg_length);
      }

      if (ev == 60) // player killed enemy with a bullet
      {
         custom_drawn = 1;
         bmsg_length += bmsg_show_text(" killed ", 15, bmsg_length);
         bmsg_length += bmsg_draw_enemy(Ei[z2][0], bmsg_length);
         bmsg_length += bmsg_show_text(" with a bullet.", 15, bmsg_length);
      }
      if (ev == 62) // player killed enemy with explosion
      {
         custom_drawn = 1;
         bmsg_length += bmsg_show_text(" killed ", 15, bmsg_length);
         bmsg_length += bmsg_draw_enemy(Ei[z2][0], bmsg_length);
         bmsg_length += bmsg_show_text(" with explosion!", 15, bmsg_length);
      }
      if (ev == 64) // enemy killed with field
      {
         custom_drawn = 1;
         bmsg_length += bmsg_draw_enemy(Ei[z2][0], bmsg_length);
         bmsg_length += bmsg_show_text(" killed with damage field!", 15, bmsg_length);
      }
      if (ev == 70) // player got a free man
      {
         custom_drawn = 1;
         bmsg_length += bmsg_show_text(" got a free man! ", 15, bmsg_length);
         bmsg_length += bmsg_draw_tile(265, bmsg_length);
      }

      if (ev == 71) // player got a purple coin
      {
         custom_drawn = 1;
         bmsg_length += bmsg_show_text(" got a purple coin! ", 15, bmsg_length);
         bmsg_length += bmsg_draw_tile(265, bmsg_length);
      }

      if (ev == 72) // player got a health bonus
      {
         custom_drawn = 1;
         bmsg_length += bmsg_show_text(" picked up a Health Bonus! ", 15, bmsg_length);
         int bt = 320;
         if (z3 == 1023) bt = 304;
         if (z3 == 1027) bt = 240;
         if (z3 == 1035) bt = 320;
         bmsg_length += bmsg_draw_tile(bt, bmsg_length);
         bmsg_length += bmsg_show_health(z4, bmsg_length);
      }
      if (ev == 80) // player joined
      {
         custom_drawn = 1;
         bmsg_length += bmsg_show_text(" joined!", 15, bmsg_length);
      }
      if (ev == 81) // player quit
      {
         custom_drawn = 1;
         bmsg_length += bmsg_show_text(" quit!", 15, bmsg_length);
      }
      if (ev == 90) // player died
      {
         custom_drawn = 1;
         bmsg_length += bmsg_show_text(" died!", 15, bmsg_length);
      }


      if (custom_drawn) // caught by one of the handlers here
      {
         bottom_msg = 100; // start the timer
         if (++bmsg_index > 19) bmsg_index = 0;

         al_set_target_bitmap(bmsg_bmp[bmsg_index]);

         al_clear_to_color(al_map_rgb(0, 0, 0));
         al_draw_bitmap(bmsg_temp, (400 - (bmsg_length/2)), 0, 0);

         al_convert_mask_to_alpha(bmsg_bmp[bmsg_index], al_map_rgb(0, 0, 0)) ;
         al_set_target_backbuffer(display);

         // convert array to array2 to re-arrange order...this does not actually copy anything except pointers!
         int tc = 0;
         // cycle from 1-index to 1-min
         for (int c=bmsg_index; c>=0; c--)
         {
            bmsg_bmp2[tc] = bmsg_bmp[c];
            tc++;
         }
         // cycle from 1-max to 1-index
         for (int c=19; c>bmsg_index; c--)
         {
            bmsg_bmp2[tc] = bmsg_bmp[c];
            tc++;
         }
      }
      else
      {
         printf(" no bmsg handler for event:%d\n",ev);
        // 1-shoot 4-exit 15-jump 22-explosion 31-sproingy
      }
      al_destroy_bitmap(bmsg_temp); // destroy the temp bitmap
   }
}

void draw_bmsg()
{
   if (bottom_msg_on)
   {

//      bottom_msg = 100; // always draw
      if (bottom_msg > 0)
      {
         bottom_msg--;

         int nb = 8;  // number of bottom message lines to display (max 20)

         int dfb = 2000 - al_fixtoi(players[active_local_player].PY); // player distance from bottom of level
         if (dfb < 300)  nb = 4;
         if (dfb < 200)  nb = 2;


         int sw = 800; // string length in pixels
         int sh = 20;  // string height in pixels
         float x = SCREEN_W/2 - 10;

         float io = 1.0; // initial opacity
         float fo = 0.1; // final opacity
         float oss = (io - fo) / (float) nb;  // opacity step
         float co = io; // current opacity

         float ivs = 0.5; // initial v size
         float fvs = 0.1; // final v size
         float vss = (ivs - fvs) / (float) nb;  // v size step
         float cvs = ivs; // current v size
         float y = SCREEN_H - ivs*20 - 1;

         float ihs = 0.5; // initial h size
         float fhs = 0.1; // final h size
         float hss = (ihs - fhs) / (float) nb;  // v size step
         float chs = ihs; // current v size

         for (int m=0; m<nb; m++)
         {
            float dw = chs * sw;
            float dh = cvs * sh;
            ALLEGRO_COLOR col = al_map_rgba_f(co, co, co, co);
            al_draw_tinted_scaled_bitmap(bmsg_bmp2[m], col, 0, 0, sw, sh, x-dw/2, y, dw, dh, 0);
            co -= oss;
            cvs -= vss;
            chs -= hss;
            y -= dh;
         }

         // draw bounding box to show what size it is
//         float tvs = (sw/2)*ivs;
//         al_draw_rectangle(x-tvs, SCREEN_H,  x+tvs, y, palette_color[15], 1);

      }
   }
}




