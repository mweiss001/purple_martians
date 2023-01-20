// z_screen_overlay.cpp

#include "pm.h"
#include "z_qGraph.h"
#include "z_sound.h"
#include "z_log.h"

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
   if (players[0].level_done_mode == 5)
   {
      if (!players[p].level_done_ack)
      {
         c = flash_color;
         int pay = 16;
         al_draw_textf(font, palette_color[c], tx+158, y+pay, 0, "press");
         al_draw_textf(font, palette_color[c], tx+158, y+pay+8, 0, " any");
         int tl = players[0].level_done_timer/40;
         if (tl > 9) al_draw_textf(font, palette_color[c], tx+154, y+pay+18, 0, "  %2d", tl);
         else        al_draw_textf(font, palette_color[c], tx+158, y+pay+18, 0, "  %d", tl);
      }
      else al_draw_textf(font, palette_color[15], tx+158, y+20, 0, "ready");
   }


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
   if (LOG_TMR_scrn_overlay) t0 = al_get_time();
   al_set_target_backbuffer(display);
   if ((players[0].level_done_mode) && (players[0].level_done_mode < 8)) show_level_done();
   if (speed_testing) draw_speed_test_data();
   draw_top_display();
   draw_bmsg();


   double tt = 0;
   if (LOG_TMR_scrn_overlay) tt = al_get_time();
   show_player_join_quit();
   if (LOG_TMR_scrn_overlay) add_log_TMR(al_get_time() - tt, "scov_jq", 0);



   if (LOG_TMR_scrn_overlay) add_log_TMR(al_get_time() - t0, "scrn_overlay_all", 0);
}


void set_player_joint_quit_display(int p, int type, int time)
{
   show_player_join_quit_timer = time;
   show_player_join_quit_jq = type;
   show_player_join_quit_player = p;
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
      if (jq == 2) sprintf(msg, "Player %d found the exit!", p);
      if (jq == 3) sprintf(msg, "Player %d DIED!", p);

      float stretch = ( (float)SCREEN_W / (strlen(msg)*8)) - 1; // (SCREEN_W / text length*8) -1
      float ratio = (float)t / 60;

      int y_pos = SCREEN_H/2;
      int y_pos_move = SCREEN_H/2;

      if (ratio > .6)
      {
           float ra1 = 1 - ratio;                // starts at 0 and increases to .3
           float ra2 = ra1 * 2.5;                // starts at 0 and increases to .999
           stretch =  ra2 * stretch;
           y_pos = y_pos - y_pos_move + (int)(ra2 * y_pos_move);
      }
      if (ratio < .4)
      {
           float ra1 = .4 - ratio;               // starts at 0 and increases to .3
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
   int fsls = players1[p].frames_skipped_last_sec;

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

void sdg_show_column(int col, int &x, int y)
{
   int color = 15;
   int color1 = 15;
   int color2 = 63 + 64;

   if (col == 1) // player number
   {
      al_draw_text(font, palette_color[color], x, y+=8, 0, "[p]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (players[p].active == 1) color = color1;
         if (players[p].active == 0) color = color2;
         al_draw_textf(font, palette_color[color], x, y+=8, 0, "[%d]", p);
      }
      x+=3*8;
   }


   if (col == 2) // active
   {
      al_draw_text(font, palette_color[color], x, y+=8, 0, "[a]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (players[p].active == 1) color = color1;
         if (players[p].active == 0) color = color2;
         al_draw_textf(font, palette_color[color], x, y+=8, 0, "[%d]", players[p].active);
      }
      x+=3*8;
   }


   if (col == 3) // color
   {
      al_draw_text(font, palette_color[color], x, y+=8, 0, "[co]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (players[p].active == 1) color = players[p].color;
         if (players[p].active == 0) color = color2;

         al_draw_textf(font, palette_color[color], x, y+=8, 0, "[%2d]", players[p].color);
      }
      x+=4*8;
   }


   if (col == 4) // control method
   {
      al_draw_text(font, palette_color[color], x, y+=8, 0, "[m]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (players[p].active == 1) color = color1;
         if (players[p].active == 0) color = color2;
         al_draw_textf(font, palette_color[color], x, y+=8, 0, "[%d]", players[p].control_method);
      }
      x+=3*8;
   }



   if (col == 5) // who
   {
      al_draw_text(font, palette_color[color], x, y+=8, 0, "[wh]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (players[p].active == 1) color = color1;
         if (players[p].active == 0) color = color2;
         al_draw_textf(font, palette_color[color], x, y+=8, 0, "[%d]", players1[p].who);
      }
      x+=4*8;
   }


   if (col == 6) // server sync
   {
      al_draw_text(font, palette_color[color], x, y+=8, 0, "[sync]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (players[p].active == 1) color = color1;
         if (players[p].active == 0) color = color2;
         al_draw_textf(font, palette_color[color], x, y+=8, 0, "[%4d]", players1[p].server_sync);
      }
      x+=6*8;
   }

   if (col == 7) // client chase fps
   {
      al_draw_text(font, palette_color[color], x, y+=8, 0, "[chas]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (players[p].active == 1) color = color1;
         if (players[p].active == 0) color = color2;
         al_draw_textf(font, palette_color[color], x, y+=8, 0, "[%4.1f]", players1[p].client_chase_fps);
      }
      x+=6*8;
   }


   if (col == 8) // server_game_move_sync
   {
      al_draw_text(font, palette_color[color], x, y+=8, 0, "[sgms]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (players[p].active == 1) color = color1;
         if (players[p].active == 0) color = color2;
         al_draw_textf(font, palette_color[color], x, y+=8, 0, "[%4d]", players1[p].server_game_move_sync);
      }
      x+=6*8;
   }

   if (col == 9) // client_base_resets
   {
      al_draw_text(font, palette_color[color], x, y+=8, 0, "[base]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (players[p].active == 1) color = color1;
         if (players[p].active == 0) color = color2;
         al_draw_textf(font, palette_color[color], x, y+=8, 0, "[%4d]", players1[p].client_base_resets);
      }
      x+=6*8;
   }



   if (col == 10) // dif src
   {
      al_draw_text(font, palette_color[color], x, y+=8, 0, "[start]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (players[p].active == 1) color = color1;
         if (players[p].active == 0) color = color2;
         al_draw_textf(font, palette_color[color], x, y+=8, 0, "[%5d]", srv_client_state_frame_num[p][0]);
      }
      x+=7*8;
   }
   if (col == 11) // dif dst
   {
      al_draw_text(font, palette_color[color], x, y+=8, 0, "[destn]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (players[p].active == 1) color = color1;
         if (players[p].active == 0) color = color2;
         al_draw_textf(font, palette_color[color], x, y+=8, 0, "[%5d]", srv_client_state_frame_num[p][1]);
      }
      x+=7*8;
   }
   if (col == 12) // number of packets
   {
      al_draw_text(font, palette_color[color], x, y+=8, 0, "[np]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (players[p].active == 1) color = color1;
         if (players[p].active == 0) color = color2;
         al_draw_textf(font, palette_color[color], x, y+=8, 0, "[%2d]", players1[p].num_dif_packets);
      }
      x+=4*8;
   }
   if (col == 13) // dif size
   {
      al_draw_text(font, palette_color[color], x, y+=8, 0, "[difs]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (players[p].active == 1) color = color1;
         if (players[p].active == 0) color = color2;
         al_draw_textf(font, palette_color[color], x, y+=8, 0, "[%4d]", players1[p].cmp_dif_size);
      }
      x+=6*8;
   }
   if (col == 14) // tx kB/sec
   {
      al_draw_text(font, palette_color[color], x, y+=8, 0, "[tkbs]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (players[p].active == 1) color = color1;
         if (players[p].active == 0) color = color2;
         al_draw_textf(font, palette_color[color], x, y+=8, 0, "[%4.1f]", (float)players1[p].tx_bytes_per_tally/1000);
      }
      x+=6*8;
   }

   if (col == 15) // rx kB/sec
   {
      al_draw_text(font, palette_color[color], x, y+=8, 0, "[rkbs]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (players[p].active == 1) color = color1;
         if (players[p].active == 0) color = color2;
         al_draw_textf(font, palette_color[color], x, y+=8, 0, "[%4.1f]", (float)players1[p].rx_bytes_per_tally/1000);
      }
      x+=6*8;
   }

   if (col == 16) // frames skipped
   {
      al_draw_text(font, palette_color[color], x, y+=8, 0, "[frsk]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (players[p].active == 1) color = color1;
         if (players[p].active == 0) color = color2;
         al_draw_textf(font, palette_color[color], x, y+=8, 0, "[%4d]", players1[p].frames_skipped);
      }
      x+=6*8;
   }

   if (col == 18) // server name and description
   {
      y+=8;
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         y+=8;

         if (p == 0)
         {
            sprintf(msg, "[%s] <-- server (me!)", players1[p].hostname);
            al_draw_text(font, palette_color[players[p].color], x, y, 0, msg);
         }
         if ((players[p].active) && (players[p].control_method == 2))
         {
            sprintf(msg, "[%s] <-- active client", players1[p].hostname);
            al_draw_text(font, palette_color[players[p].color], x, y, 0, msg);
         }
         if ((!players[p].active) && (players[p].control_method == 2))
         {
            sprintf(msg, "[%s] <-- syncing client", players1[p].hostname);
            al_draw_text(font, palette_color[players[p].color], x, y, 0, msg);
         }
         if (players[p].control_method == 9)
         {
            sprintf(msg, "[%s] <-- used client", players1[p].hostname);
            al_draw_text(font, palette_color[players[p].color], x, y, 0, msg);
         }
      }
   }


   if (col == 19) // client name and description
   {
      y+=8;
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         y+=8;
         sprintf(msg, " "); // blank
         if (p == 0) sprintf(msg, "[%s] <-- server", players1[p].hostname);
         else
         {
            if ((players[p].active) && (players[p].control_method == 2)) sprintf(msg, "[%s] <-- active client", players1[p].hostname);
            if ((!players[p].active) && (players[p].control_method == 2)) sprintf(msg, "[%s] <-- syncing client", players1[p].hostname);
            if (players[p].control_method == 9) sprintf(msg, "[%s] <-- used client", players1[p].hostname);
            if (p == active_local_player) sprintf(msg, "[%s] <-- local client (me)", players1[p].hostname);
         }
         al_draw_text(font, palette_color[players[p].color], x, y, 0, msg);
      }
   }




   if (col == 20) // sc sync
   {
      al_draw_text(font, palette_color[color], x, y+=8, 0, "[sc]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (players[p].active == 1) color = color1;
         if (players[p].active == 0) color = color2;
         al_draw_textf(font, palette_color[color], x, y+=8, 0, "[%2d]", players1[p].sc_sync);
      }
      x+=4*8;
   }

   if (col == 21) // cs sync
   {
      al_draw_text(font, palette_color[color], x, y+=8, 0, "[cs]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (players[p].active == 1) color = color1;
         if (players[p].active == 0) color = color2;
         al_draw_textf(font, palette_color[color], x, y+=8, 0, "[%2d]", players1[p].cs_sync);
      }
      x+=4*8;
   }
   if (col == 22) // rt sync
   {
      al_draw_text(font, palette_color[color], x, y+=8, 0, "[rt]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (players[p].active == 1) color = color1;
         if (players[p].active == 0) color = color2;
         al_draw_textf(font, palette_color[color], x, y+=8, 0, "[%2d]", players1[p].rt_sync);
      }
      x+=4*8;
   }


   if (col == 23) // late cdats
   {
      al_draw_text(font, palette_color[color], x, y+=8, 0, "[lcdt]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (players[p].active == 1) color = color1;
         if (players[p].active == 0) color = color2;
         al_draw_textf(font, palette_color[color], x, y+=8, 0, "[%4d]", players1[p].late_cdats);
      }
      x+=6*8;
   }


   if (col == 24) // late cdats last second
   {
      al_draw_text(font, palette_color[color], x, y+=8, 0, "[lcls]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (players[p].active == 1) color = color1;
         if (players[p].active == 0) color = color2;
         al_draw_textf(font, palette_color[color], x, y+=8, 0, "[%4d]", players1[p].late_cdats_last_sec);
      }
      x+=6*8;
   }





   if (col == 25) // client dsync
   {
      al_draw_text(font, palette_color[color], x, y+=8, 0, "[cdsy]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (players[p].active == 1) color = color1;
         if (players[p].active == 0) color = color2;
         al_draw_textf(font, palette_color[color], x, y+=8, 0, "[%4.0f]", players1[p].dsync*1000);
      }
      x+=6*8;
   }


   if (col == 26) // client game move sync avg last sec
   {
      al_draw_text(font, palette_color[color], x, y+=8, 0, "[gmav]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (players[p].active == 1) color = color1;
         if (players[p].active == 0) color = color2;
         al_draw_textf(font, palette_color[color], x, y+=8, 0, "[%4.0f]", players1[p].game_move_dsync_avg_last_sec*1000);
      }
      x+=6*8;
   }




   if (col == 27) // ping
   {
      al_draw_text(font, palette_color[color], x, y+=8, 0, "[ping]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (players[p].active == 1) color = color1;
         if (players[p].active == 0) color = color2;
         if (p == 0) al_draw_textf(font, palette_color[color], x, y+=8, 0, "[     ]");
         else        al_draw_textf(font, palette_color[color], x, y+=8, 0, "[%4.1f]", players1[p].ping*1000);
      }
      x+=6*8;
   }








}


void sdg_show(int x, int y) // server debug grid
{


   al_draw_filled_rectangle(x, y+8, x+880, y+80, palette_color[0]);


   sdg_show_column(1, x, y); // player number
   sdg_show_column(2, x, y); // active
   sdg_show_column(3, x, y); // color
   sdg_show_column(4, x, y); // control method
//   sdg_show_column(5, x, y); // who
   sdg_show_column(6, x, y); // server sync
   sdg_show_column(7, x, y); // client chase fps
   sdg_show_column(8, x, y); // server_game_move_sync


   sdg_show_column(20, x, y); // sc sync
   sdg_show_column(21, x, y); // cs sync
   sdg_show_column(22, x, y); // rt sync

   sdg_show_column(9, x, y); // client base resets

   sdg_show_column(23, x, y); // late cdats
   sdg_show_column(24, x, y); // late cdats


//   sdg_show_column(10, x, y); // dif src
//   sdg_show_column(11, x, y); // dif dst
   sdg_show_column(12, x, y); // number of packets
   sdg_show_column(13, x, y); // dif size
   sdg_show_column(14, x, y); // tx kB/sec
   sdg_show_column(15, x, y); // rx kB/sec
   sdg_show_column(16, x, y); // frame skipped


   sdg_show_column(27, x, y); // ping

   sdg_show_column(25, x, y); //
   sdg_show_column(26, x, y); //

   sdg_show_column(18, x, y); // name and description (server version)





}


void cdg_show(int x, int y) // client debug grid
{

   al_draw_filled_rectangle(x, y+8, x+400, y+80, palette_color[0]);

   // al_draw_rectangle(x, y+8, x+400, y+80, palette_color[15], 1);

   sdg_show_column(1, x, y); // player number
   sdg_show_column(2, x, y); // active
   sdg_show_column(3, x, y); // color
   sdg_show_column(4, x, y); // control method

   sdg_show_column(19, x, y); // name and description (client version)

}


void draw_top_frame(int p)
{
   int tdx = BORDER_WIDTH;
   int tdy = 0;

   double tt = 0;

   // ----------------------------------
   // draw info on top frame
   // ----------------------------------
   if (LOG_TMR_scrn_overlay) tt = al_get_time();

   if (SCREEN_W < 600) sprintf(msg,"Lv:%d Tm:%d En:%d ",                play_level, frame_num/40, num_enemy); // special case for narrow screens
   else                sprintf(msg,"Level:%d | Time:%d | Enemies:%d  ", play_level, frame_num/40, num_enemy);
   al_draw_text(font, palette_color[14], tdx, tdy+2,  0, msg);
   tdx += strlen(msg)*8;

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
   if (LOG_TMR_scrn_overlay) add_log_TMR(al_get_time() - tt, "scov_top_frm", 0);


}


void draw_common_debug_overlay(int p, int &cx, int &cy)
{

   double tt = 0;
   if (LOG_TMR_scrn_overlay) tt = al_get_time();

   // -----------------------------------
   // common debug overlay for all modes
   // -----------------------------------

   if (ima_server) {cx = BORDER_WIDTH; cy+=80;}
   int color = 15;
   int fps_color = 14;

   int fs = players1[p].frames_skipped;
   int fsls = players1[p].frames_skipped_last_sec;

   int ta = players1[p].timer_adjust;
   int tals = players1[p].timer_adjust_last_sec;

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

   al_draw_textf(font, palette_color[fps_color], cx, cy+=8, 0, "timer adjust last second:%d", tals);
   al_draw_textf(font, palette_color[fps_color], cx, cy+=8, 0, "total timer adjust:%d",ta);


   if (ima_server) al_draw_textf(font, palette_color[fps_color], cx, cy+=8, 0, "total game moves:%d", game_move_entry_pos);

   if (LOG_TMR_scrn_overlay) add_log_TMR(al_get_time() - tt, "scov_dbgcom", 0);



   if (LOG_TMR_scrn_overlay) tt = al_get_time();

   sprintf(qG[0].series[0].name, "CPU");
   qG[0].series[0].color = 13;
   qG[0].series[0].active = 1;

   qG[0].width = 200;
   qG[0].height = 36;
   qG[0].draw_graph(SCREEN_W-228, SCREEN_H-56);

   cy+=4;

   if (LOG_TMR_scrn_overlay) add_log_TMR(al_get_time() - tt, "scov_CPU", 0);


   //al_draw_textf(font, palette_color[fps_color], cx, cy+=8, 0, "px:%d     py:%d", al_fixtoi(players[p].PX), al_fixtoi(players[p].PY));
   //al_draw_textf(font, palette_color[fps_color], cx, cy+=8, 0, "pxinc:%1.2f  pyinc:%1.2f", al_fixtof(players[p].xinc), al_fixtof(players[p].yinc));

}


void draw_server_debug_overlay(int p, int &cx, int &cy)
{
   double lts = 0;

   // ------------------------------------
   // ---   server debug grid
   // ------------------------------------
   if (LOG_TMR_scrn_overlay) lts = al_get_time();
   sdg_show(BORDER_WIDTH, BORDER_WIDTH-8);
   if (LOG_TMR_scrn_overlay) add_log_TMR(al_get_time() - lts, "scov_sgrid", 0);


   // -----------------------------------------------------
   // server buttons to display and change s1 and s2
   // -----------------------------------------------------
   if (LOG_TMR_scrn_overlay) lts = al_get_time();
   int csx1 = cx;
   int csw = 80;
   int csx2 = csx1 + csw;
   int csy1 = cy + 20;
   int csh = 36;
   int csy2 = csy1 + csh;
   int color = 13;

   al_show_mouse_cursor(display);

   al_draw_filled_rectangle(csx1, csy1, csx2, csy2, palette_color[color+224]); // erase background
   al_draw_rectangle(csx1, csy1, csx2, csy2, palette_color[color], 1);         // frame

   // non blocking buttons!
   int ya = 0, btw = 16;
   int csy3 = csy1 + 3;

   ya = csy3;
   static int b1_pres = 0;
   if (mdw_buttont_nb(csx1+2, ya, csx1+2+btw, 16,  0,0,0,0,  0,color,15, 0,  1,0,1,0, "-"))
   {
      if (b1_pres == 0) players1[p].s1--;
      if (players1[p].s1 < 1) players1[p].s1 = 1;
      b1_pres = 1;
   }
   else b1_pres = 0;

   ya = csy3;
   static int b2_pres = 0;
   if (mdw_buttont_nb(csx2-btw-4, ya, csx2-2, 16,  0,0,0,0,  0,color,15, 0,  1,0,1,0, "+"))
   {
      if (b2_pres == 0) players1[p].s1++;
      b2_pres = 1;
   }
   else b2_pres = 0;
   al_draw_textf(font, palette_color[15], csx1+csw/2, csy3+2, ALLEGRO_ALIGN_CENTER, "s1:%d", players1[p].s1);

   csy3 += 17;

   ya = csy3;
   static int b3_pres = 0;
   if (mdw_buttont_nb(csx1+2, ya, csx1+2+btw, 16,  0,0,0,0,  0,color,15, 0,  1,0,1,0, "-"))
   {
      if (b3_pres == 0) players1[p].s2--;
      if (players1[p].s2 < 1) players1[p].s2 = 1;
      b3_pres = 1;
   }
   else b3_pres = 0;

   ya = csy3;
   static int b4_pres = 0;
   if (mdw_buttont_nb(csx2-btw-4, ya, csx2-2, 16,  0,0,0,0,  0,color,15, 0,  1,0,1,0, "+"))
   {
      if (b4_pres == 0) players1[p].s2++;
      b4_pres = 1;
   }
   else b4_pres = 0;
   al_draw_textf(font, palette_color[15], csx1+csw/2, csy3+2, ALLEGRO_ALIGN_CENTER, "s2:%d", players1[p].s2);
   if (LOG_TMR_scrn_overlay) add_log_TMR(al_get_time() - lts, "scov_sbutt", 0);


   if (0) // show some bandwidth stats
   {
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


void draw_client_debug_overlay(int p, int &cx, int &cy)
{

   double lts = 0;

   // ------------------------------------
   // ---   client debug grid
   // ------------------------------------
   if (LOG_TMR_scrn_overlay) lts = al_get_time();
   cdg_show(BORDER_WIDTH, BORDER_WIDTH-8); // client debug grid
   if (LOG_TMR_scrn_overlay) add_log_TMR(al_get_time() - lts, "scov_cgrid", 0);


   // ----------------------------------
   // ping and sync graph
   // ----------------------------------
   if (LOG_TMR_scrn_overlay) lts = al_get_time();

   double ds = -players1[p].dsync    * 1000; // the current value of dsync for display
   double pa = players1[p].ping_avg * 1000;

   qG[1].add_data(0, ds);
   qG[1].add_data(1, pa);

   sprintf(qG[1].series[0].name, "SYNC");
   qG[1].series[0].color = 10;
   qG[1].series[0].active = 1;

   sprintf(qG[1].series[1].name, "PING");
   qG[1].series[1].color = 14;
   qG[1].series[1].active = 1;

   qG[1].width = 200;
   qG[1].height = 40;
   qG[1].draw_graph(SCREEN_W-228, SCREEN_H-36-48-20);

   if (LOG_TMR_scrn_overlay) add_log_TMR(al_get_time() - lts, "scov_sync", 0);




   // ----------------------------------
   // client sync display and adjustment buttons
   // ----------------------------------
   if (LOG_TMR_scrn_overlay) lts = al_get_time();
   int csx1 = SCREEN_W - 160;
   int csw = 140;
   int csx2 = csx1 + csw;
   int csy1 = SCREEN_H-36-48-20-60;
   int csh = 43;
   int csy2 = csy1 + csh;
   int color = 13;
   double ts = players1[p].dsync; // the current value of dsync for display
   al_show_mouse_cursor(display);
   al_draw_filled_rectangle(csx1, csy1, csx2, csy2, palette_color[color+224]); // erase background
   al_draw_rectangle(csx1, csy1, csx2, csy2, palette_color[color], 1);    // frame
   al_draw_textf(font, palette_color[15], csx1+csw/2, csy1+2, ALLEGRO_ALIGN_CENTER, "Sync:%+3.0fms", ts*1000);
   // graph
   int csy3 = csy1 + 12;
   int csy4 = csy3 + 12;
   int csx3 = csx1 + 7;
   int csx4 = csx2 - 7;
   int csw2 = csx2 - csx1;

   al_draw_rectangle(csx3, csy3, csx4, csy4, palette_color[color], 1);
   al_draw_textf(f3, palette_color[color], csx3, csy3-11, ALLEGRO_ALIGN_CENTER, "-50");
   al_draw_textf(f3, palette_color[color], csx4, csy3-11, ALLEGRO_ALIGN_CENTER, "+25");

   // convert from -0.025 to +0.025 from 0 to csw2
   double gr = -0.050; // graph start
   double gs =  0.075; // graph range

   float dt = ((ts-gr)/gs) * csw2; // convert from range of (0.00-0.025) to (0 - csw)
   al_draw_filled_rectangle(csx3+dt-2, csy3, csx3+dt+2, csy4, palette_color[15]);

   // non blocking buttons!
   int ya = csy4 + 3;
   int btw = 16;

   static int b1_pres = 0;
   if (mdw_buttont_nb(csx1+2, ya, csx1+2+btw, 16,  0,0,0,0,  0,color,15, 0,  1,0,1,0, "-"))
   {
      if (b1_pres == 0)
      {
         if (CTRL()) players1[p].client_chase_offset -=0.01;
         else players1[p].client_chase_offset -=0.001;
      }
      b1_pres = 1;
   }
   else b1_pres = 0;

   ya = csy4 + 3;
   static int b2_pres = 0;
   if (mdw_buttont_nb(csx2-btw-4, ya, csx2-2, 16,  0,0,0,0,  0,color,15, 0,  1,0,1,0, "+"))
   {
      if (b2_pres == 0)
      {
         if (CTRL()) players1[p].client_chase_offset +=0.01;
         else players1[p].client_chase_offset +=0.001;
      }

   }
   else b2_pres = 0;
   al_draw_textf(font, palette_color[15], csx1+csw/2, csy1+30, ALLEGRO_ALIGN_CENTER, "offset:%+3.0fms", players1[p].client_chase_offset*1000);


   if (LOG_TMR_scrn_overlay) add_log_TMR(al_get_time() - lts, "scov_cbut", 0);




   int tx = BORDER_WIDTH;
   int ty1 = 80;

   al_draw_filled_rectangle(tx, ty1+8, tx+strlen(msg)*8, ty1+16, palette_color[0]);
   al_draw_text(font, palette_color[15], tx, ty1+=8, 0, msg);

   sprintf(msg, "server sync:%d ", players1[p].client_sync);
   al_draw_filled_rectangle(tx, ty1+8, tx+strlen(msg)*8, ty1+16, palette_color[0]);
   al_draw_text(font, palette_color[15], tx, ty1+=8, 0, msg);

   sprintf(msg, "local moves:%d", game_move_entry_pos);
   al_draw_filled_rectangle(tx, ty1+8, tx+strlen(msg)*8, ty1+16, palette_color[0]);
   al_draw_text(font, palette_color[15], tx, ty1+=8, 0, msg);

   sprintf(msg, "frames_skipped:%d", players1[p].frames_skipped);
   al_draw_filled_rectangle(tx, ty1+8, tx+strlen(msg)*8, ty1+16, palette_color[0]);
   al_draw_text(font, palette_color[15], tx, ty1+=8, 0, msg);

   if (0) // show a bunch of bandwidth stats
   {
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
   }
}



void draw_top_display(void)
{
   int p = active_local_player;
   int cy = BORDER_WIDTH-6;
   int cx = SCREEN_W-320;

   double tt = 0;

   // ----------------------------------
   // draw demo mode overlay
   // ----------------------------------
   if (LOG_TMR_scrn_overlay) tt = al_get_time();
   if (demo_mode_on) draw_large_text_overlay(3, 15);
   if (LOG_TMR_scrn_overlay) add_log_TMR(al_get_time() - tt, "scov_demo_ov", 0);


   draw_top_frame(p);

   if (show_debug_overlay) draw_common_debug_overlay(p, cx, cy);

   if (ima_server)
   {
      tt = al_get_time();

      // ----------------------------------
      // draw info on bottom frame
      // ----------------------------------
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

      // sprintf(msg, "[clients:%d] [moves:%d]", num_clients, game_move_entry_pos);
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

      if (show_debug_overlay) draw_server_debug_overlay(p, cx, cy);

      if (LOG_TMR_scrn_overlay) add_log_TMR(al_get_time() - tt, "scov_server", 0);
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

      // ----------------------------------
      // draw info on bottom frame
      // ----------------------------------
      int bdx = BORDER_WIDTH;
      int bdy = SCREEN_H - 10;
      int ts = 0;  // text spacing

      sprintf(msg, "Netgame Client (%s) ", local_hostname );
      al_draw_text(font, palette_color[14], bdx, bdy, 0, msg);
      ts += strlen(msg)*8;

      sprintf(msg, " ping:%3.1fms", players1[p].ping * 1000);
      al_draw_text(font, palette_color[14], bdx + ts, bdy, 0, msg);
      ts += strlen(msg)*8;

//      sprintf(msg, "sync:%d ", players1[p].client_sync);
//      al_draw_text(font, palette_color[14], bdx + ts, bdy, 0, msg);
//      ts += strlen(msg)*8;

      if (show_debug_overlay) draw_client_debug_overlay(p, cx, cy);

      if (LOG_TMR_scrn_overlay) add_log_TMR(al_get_time() - tt, "scov_client", 0);
   }



   if (players[0].control_method == 1) // file play
   {
      // ----------------------------------
      // draw info on bottom frame
      // ----------------------------------
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

      sprintf(msg, "[%d%%] ", frame_num*100/demo_mode_last_frame);
      al_draw_text(font, palette_color[14], bdx + ts, bdy, 0, msg);
      ts += strlen(msg)*8;



      if (show_debug_overlay)
      {
         sprintf(msg, "Time:[%d%%] ", frame_num*100/demo_mode_last_frame);
         al_draw_filled_rectangle(cx, cy+8, cx+strlen(msg)*8, cy+16, palette_color[0]);
         al_draw_text(font, palette_color[15], cx, cy+=8, 0, msg);
      }

      //printf("l:%d f:%d\n", play_level, frame_num);

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
         case 60: case 62: // enemy killed
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



void clear_bmsg(void)
{
   for (int c=0; c<20; c++)
   {
      al_set_target_bitmap(bmsg_bmp[c]);
      al_clear_to_color(al_map_rgba(0,0,0,0));
   }
   bmsg_index = 0;
   bottom_msg = 0;

  for (int i=0; i<10; i++)
     game_event_retrigger_holdoff[i] = 0;
}

int bmsg_show_text(const char *txt, int col, int bmsg_length)
{
   al_draw_text(font2, palette_color[col], bmsg_length, 2, ALLEGRO_ALIGN_INTEGER, txt);
   //printf("%d %s\n",frame_num, txt);
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
   int len = 0;
   if (0) // enemy name
   {
      sprintf(msg, "%s", enemy_name[e_type][0]);
      len += bmsg_show_text(msg, 15, bmsg_length);
   }
   if (1) // enemy tile
   {
      bmsg_draw_tile(enemy_tile[e_type], bmsg_length);
      len = 20;
   }
   if (0) // name and tile
   {
      sprintf(msg, "%s ", enemy_name[e_type][0]);
      len += bmsg_show_text(msg, 15, bmsg_length);
      bmsg_draw_tile(enemy_tile[e_type], bmsg_length+len);
      len +=20;
   }
   if (0) // tile and name
   {
      bmsg_draw_tile(enemy_tile[e_type], bmsg_length);
      len +=20;
      sprintf(msg, " %s", enemy_name[e_type][0]);
      len += bmsg_show_text(msg, 15, bmsg_length+len);
   }
   return len;
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
   if (LOG_TMR_bmsg_add) t0 = al_get_time();

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
   if ((ev != 0) && (ev != 1) && (ev != 4) && (ev != 15) && (ev != 22) && (ev != 31) && (ev != 31) ) // events that don't have bmsg handler
   {
      int bmsg_length = 0; // keep a running total
      int custom_drawn = 0;

      al_set_target_bitmap(bmsg_temp);
      al_clear_to_color(al_map_rgba(0, 0, 0, 0));

      bmsg_length += bmsg_draw_player(z1, bmsg_length); // all bmsg start with player

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
         al_clear_to_color(al_map_rgba(0, 0, 0, 0));
         al_draw_bitmap(bmsg_temp, (400 - (bmsg_length/2)), 0, 0);
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
   }
   if (LOG_TMR_bmsg_add) add_log_TMR(al_get_time() - t0, "bmsg_add4", 0);
}

void draw_bmsg()
{
   if (LOG_TMR_bmsg_draw) t0 = al_get_time();
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
   if (LOG_TMR_bmsg_draw) add_log_TMR(al_get_time() - t0, "bmsg_draw", 0);
}
