// z_settings.cpp
#include "pm.h"
#include "z_sound.h"
#include "z_log.h"

void set_all_logging(int v)
{
   LOG_NET=v;
   LOG_NET_join=v;
   LOG_NET_player_array=v;
   LOG_NET_bandwidth=v;
   LOG_NET_cdat=v;
   LOG_NET_stdf=v;
   LOG_NET_stdf_all_packets=v;
   LOG_NET_stdf_when_to_apply=v;
   LOG_NET_client_ping=v;
   LOG_NET_client_timer_adj=v;
   LOG_NET_server_rx_stak=v;
   LOG_TMR_move_tot=v;
   LOG_TMR_move_all=v;
   LOG_TMR_move_enem=v;
   LOG_TMR_bmsg_add=v;
   LOG_TMR_bmsg_draw=v;
   LOG_TMR_draw_tot=v;
   LOG_TMR_draw_all=v;
   LOG_TMR_sdif=v;
   LOG_TMR_cdif=v;
   LOG_TMR_rwnd=v;
}


int redraw_all_controls(int x, int y, int bts, int tc, int show_buttons, int num)
{
   int xa = x;
   int xb = xa + 32;  // 'new' button end
   int tx = xb + 10;  // text pos padded after 'new' button
   int xc = xa + 240; // end of entire area, used for clearing and framing

   int ya = y;
   float yo = (bts-8)/2; // text y offset so it is centered with the button

   al_draw_filled_rectangle(x-1, y-1, xc, ya + bts*7, palette_color[13+224]); // erase background
   al_draw_rectangle       (x-1, y-1, xc, ya + bts*7, palette_color[15], 0);  // frame

   if (num == 0) al_draw_textf(font, palette_color[10], tx, ya+yo, 0, "Up ---- set new control");
   else          al_draw_textf(font, palette_color[tc], tx, ya+yo, 0, "Up ---- %s", key_names[players1[0].up_key]);
   if ((show_buttons) && (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,1,0,0, "new")))    players1[0].up_key = my_readkey(x, y, tc, bts, 0);
   ya+=bts;

   if (num == 1) al_draw_textf(font, palette_color[10], tx, ya+yo, 0, "Down -- set new control");
   else          al_draw_textf(font, palette_color[tc], tx, ya+yo, 0, "Down -- %s", key_names[players1[0].down_key]);
   if ((show_buttons) && (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,1,0,0, "new")))    players1[0].down_key = my_readkey(x, y, tc, bts, 1);
   ya+=bts;

   if (num == 2) al_draw_textf(font, palette_color[10], tx, ya+yo, 0, "Left -- set new control");
   else          al_draw_textf(font, palette_color[tc], tx, ya+yo, 0, "Left -- %s", key_names[players1[0].left_key]);
   if ((show_buttons) && (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,1,0,0, "new")))    players1[0].left_key = my_readkey(x, y, tc, bts, 2);
   ya+=bts;

   if (num == 3) al_draw_textf(font, palette_color[10], tx, ya+yo, 0, "Right - set new control");
   else          al_draw_textf(font, palette_color[tc], tx, ya+yo, 0, "Right - %s", key_names[players1[0].right_key]);
   if ((show_buttons) && (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,1,0,0, "new")))    players1[0].right_key = my_readkey(x, y, tc, bts, 3);
   ya+=bts;

   if (num == 4) al_draw_textf(font, palette_color[10], tx, ya+yo, 0, "Jump -- set new control");
   else          al_draw_textf(font, palette_color[tc], tx, ya+yo, 0, "Jump -- %s", key_names[players1[0].jump_key]);
   if ((show_buttons) && (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,1,0,0, "new")))    players1[0].jump_key = my_readkey(x, y, tc, bts, 4);
   ya+=bts;

   if (num == 5) al_draw_textf(font, palette_color[10], tx, ya+yo, 0, "Fire -- set new control");
   else          al_draw_textf(font, palette_color[tc], tx, ya+yo, 0, "Fire -- %s", key_names[players1[0].fire_key]);
   if ((show_buttons) && (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,1,0,0, "new")))    players1[0].fire_key = my_readkey(x, y, tc, bts, 5);
   ya+=bts;

   if (num == 6) al_draw_textf(font, palette_color[10], tx, ya+yo, 0, "Menu -- set new control");
   else          al_draw_textf(font, palette_color[tc], tx, ya+yo, 0, "Menu -- %s", key_names[players1[0].menu_key]);
   if ((show_buttons) && (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,1,0,0, "new")))    players1[0].menu_key = my_readkey(x, y, tc, bts, 6);
   ya+=bts;

   return ya;
}

void cfp_draw_player(int pco, int x, int y)
{
   al_draw_bitmap(player_tile[pco][1], x, y, 0 );
   al_draw_text(font, palette_color[pco], x+22, y+7, 0, color_name[pco]);
}

int cfp_draw_line(int xa, int xb, int ya, int line_spacing, int col)
{
   ya+=line_spacing;
   al_draw_line(xa, ya, xb, ya, palette_color[col], 1);
   ya+=line_spacing;
   return ya;
}

struct settings_tab
{
   char title[80];
   float x1;
   float y1;
   float x2;
   float y2;
};

void draw_tab(struct settings_tab st[], int p, int col, int text_color)
{
   int s = 2; // slant
   float v[4][2] = {0};
   v[0][0] = st[p].x1+s;  v[0][1] = st[p].y1; // ul
   v[1][0] = st[p].x1;    v[1][1] = st[p].y2; // ll
   v[2][0] = st[p].x2;    v[2][1] = st[p].y2; // lr
   v[3][0] = st[p].x2-s;  v[3][1] = st[p].y1; // ur
   al_draw_filled_polygon(*v, 4, palette_color[col+192]);
   al_draw_polygon(*v, 4, ALLEGRO_LINE_JOIN_ROUND, palette_color[col], 1.0, 0);
   al_draw_text(font, palette_color[text_color], st[p].x1+4, st[p].y1+3, 0, st[p].title);
}

void settings_pages(int set_page)
{
   if (set_page != -1)  settings_current_page = set_page;
   int page = settings_current_page;

   struct settings_tab st[20] = {0};

   sprintf(st[0].title,  "Basics");
   sprintf(st[1].title,  "Controls");
   sprintf(st[2].title,  "Netgame");
   sprintf(st[3].title,  "Logging");
   sprintf(st[4].title,  "Demo");
   sprintf(st[5].title,  "Advanced");
   sprintf(st[6].title,  "Info");
   sprintf(st[7].title,  "Viewport");
   sprintf(st[8].title,  "Profiling");
   sprintf(st[9].title,  "Test 4");
   sprintf(st[10].title, "Test 5");
   sprintf(st[11].title, "Test 6");
   sprintf(st[12].title, "Test 7");
   sprintf(st[13].title, "Test 8");
   sprintf(st[14].title, "Test 9");
   int num_pages = 9;

   char title[80] = {0};
   sprintf(title, "Settings");

   int fc = 10; // frame color
   int tc = 15; // text color
   int frame_width = 4; // border width

   int quit = 0;
   while (!quit)
   {
      al_show_mouse_cursor(display);
      // entire area, including title, tabs, and page
      int cf_w = 400;
      int cf_x1 = (SCREEN_W - cf_w)/2;
      int cf_x2 = cf_x1 + cf_w;

      int cf_h = 400;
      int cf_y1 = menu_map_y - 61; // line up exactly with the menu item "Settings"
      int cf_y2 = cf_y1 + cf_h;

      if (cf_y2 > SCREEN_H)     // if bottom is past bottom of screen
      {
         cf_y2 = SCREEN_H;      // bottom is bottom of screen
         cf_y1 = cf_y2 - cf_h;

         if (cf_y1 < 0)         // if top is past top of screen
         {
            cf_y1 = 0;          // top is top of screen
            cf_y2 = cf_y1 + cf_h;
         }
      }

      fc = players[0].color; // frame color

      al_set_target_backbuffer(display);
      al_flip_display();
      al_clear_to_color(al_map_rgb(0, 0, 0));
      frame_and_title(1);
      mdw_an();
      for (int c=0; c<7; c++)       // show first 7 menu items
      {
         int b = 15;
         if ((!resume_allowed) && (c==4)) b+=80; // dimmer if can't resume
         al_draw_text(font, palette_color[b], SCREEN_W/2, 14+(c*10)+1, ALLEGRO_ALIGN_CENTRE, global_string[7][c]);
      }

      proc_controllers();

      al_draw_filled_rectangle(cf_x1, cf_y1, cf_x2, cf_y2, palette_color[fc+224]); // erase everything

      // figure out the title size
      int bx, by, bw, bh;
      al_get_text_dimensions(font, title, &bx, &by, &bw, &bh);
      float title_w = bw + 12;
      float title_x1 = cf_x1 + (cf_w - title_w) / 2;
      float title_x2 = title_x1 + title_w;
      float title_xc = title_x1 + title_w / 2;
      float title_h = bh + 8;
      float title_y1 = cf_y1 + frame_width/2-1;
      float title_y2 = title_y1 + title_h;
      float title_ty = 0.5 + title_y1 + (title_h - bh) / 2;

      // set page x size, this has to go before the tab size
      int cfp_x1 = cf_x1 + frame_width;
      int cfp_x2 = cf_x2 - frame_width;
      int cfp_txc = cfp_x1 + (cfp_x2 - cfp_x1) / 2;


      // iterate through all the tab, determine width and position
      st[0].x1 = cfp_x1+1; // start at page x1
      st[0].y2 = 0;        // we don't know where are going to start yet, so do them at offset 0
      for (int i=0; i<num_pages; i++)
      {
         int bx, by, bw, bh; // set size of tab based on text size
         al_get_text_dimensions(font, st[i].title, &bx, &by, &bw, &bh);
         bh = 8 + 5; // force 8 because with descender, text height is sometimes different and things don't line up
         bw += 8;

         if ((st[i].x1 + bw) > cfp_x2) // time for a new line
         {
            st[i].y2 -= bh;      // raise y2 one line
            st[i].x1 = cfp_x1+1; // reset x1
         }

         st[i].y1 = st[i].y2 - bh; // y1 is above (less than) y2
         st[i].x2 = st[i].x1 + bw; // x2 is to the right (greater than) x1

         // advance to next
         st[i+1].x1 = st[i].x2 + 1; // x1 of next tab is x2 of this tab plus pad
         st[i+1].y2 = st[i].y2;     // y2 of next tab is the same y2 as this tab
      }
      int tabs_height = 8 - st[num_pages-1].y1; // find the height of the last tab

      // now we can set the top of the page, where the tabs will start
      int cfp_y1 = cf_y1 + tabs_height + title_h;
      //int cfp_y2 = cf_y2 - frame_width;

      // frame everything
      for (int a=0; a<frame_width; a++)
         al_draw_rounded_rectangle(cf_x1+a, cf_y1+a, cf_x2-a, cf_y2-a, 4, 4, palette_color[fc+a*48], 1.5);

      // frame from top of page to to top of window, this is here to draw under the tabs
      for (int a=0; a<frame_width; a++)
         al_draw_rounded_rectangle(cf_x1+a, cf_y1+a, cf_x2-a, cfp_y1-a+3, 4, 4, palette_color[fc+a*48], 1.5);

      // frame and draw title, this needs to go on top of previous frame
      for (int a=0; a<frame_width; a++)
         al_draw_rounded_rectangle(title_x1+a, title_y1+a, title_x2-a, title_y2-a, 4, 4, palette_color[fc+a*48], 1.5);
      al_draw_text(font, palette_color[tc], title_xc, title_ty, ALLEGRO_ALIGN_CENTER, title);

      // draw and process tabs
      int mouse_on_tab = -1;
      for (int i=0; i<num_pages; i++)
      {
         st[i].y1 += cfp_y1; // adjust y values to top of page
         st[i].y2 += cfp_y1;
         if ((mouse_x > st[i].x1) && (mouse_x < st[i].x2) && (mouse_y > st[i].y1) && (mouse_y < st[i].y2)) mouse_on_tab = i;
         draw_tab(st, i, fc+128, tc+128); // draw the tab
      }

      draw_tab(st, page, fc, tc); // draw the current tab on top
      if (mouse_on_tab != -1)
      {
         draw_tab(st, mouse_on_tab, fc, tc); // draw the tab the mouse is on
         if (mouse_b[1][0])
         {
            while (mouse_b[1][0]) proc_controllers();
            settings_current_page = page = mouse_on_tab;
         }
      }

      // frame page
      for (int a=0; a<frame_width; a++)
         al_draw_rounded_rectangle(cf_x1+a, cfp_y1+a, cf_x2-a, cf_y2-a, 4, 4, palette_color[fc+a*48], 1.5);








// ---------------------------------------------------------------
//  0 - basics
// ---------------------------------------------------------------
      if (page == 0)
      {
         int line_spacing = 14;
         int tc = 15;  // text color
         int fc = 13;  // frame color
         int xa = cfp_x1 + 10;
         int xb = cfp_x2 - 10;
         int bts = 16; // button spacing
         int ya = cfp_y1;

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         // ---------------------------------------
         // splash screen
         // ---------------------------------------
         int x1a = cfp_x1 + 10;
         int x1b = x1a + 250;
         mdw_togglec(x1a, ya, x1b, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,0,0, show_splash_screen, "Show splash screen on startup", tc, tc);
         x1a = x1b + 30;
         x1b = x1a + 74;
         if (mdw_buttont(x1a, ya, x1b, bts,  0,0,0,0,  0,fc,tc, 0,  1,0,1,0, "Show now"))
         {
             splash_screen();
             while (key[ALLEGRO_KEY_ESCAPE][0]) proc_controllers();
         }
         ya -=2;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         // ---------------------------------------
         // sound
         // ---------------------------------------
         x1a = cfp_x1 + 10;
         x1b = x1a + 60;
         int old_sound_on = sound_on;
         mdw_togglec(x1a, ya, x1b, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,0,0, sound_on, "Sound", tc, tc);
         if ((old_sound_on == 0) && (sound_on == 1)) load_sound();

         int dim = 0;
         if (!sound_on) dim = 128;

         int old_se_scaler = se_scaler;
         x1a = x1b + 12;
         x1b = x1a + 140;
         mdw_slideri(x1a, ya, x1b, bts,  0,0,0,0,  0,fc+dim,tc+dim,0,  0,0,0,0, se_scaler, 9, 0, 1, "Sound Effects:");

         if (old_se_scaler != se_scaler) set_se_scaler();


         //if ((old_se_scaler != se_scaler) && (sound_on)) al_set_mixer_gain(se_mixer, (float)se_scaler / 9);

         int old_st_scaler = st_scaler;
         x1a = x1b + 12;
         x1b = x1a + 140;
         mdw_slideri(x1a, ya, x1b, bts,  0,0,0,0,  0,fc+dim,tc+dim,0,  0,0,1,0, st_scaler, 9, 0, 1, "Sound Track:");
         if (old_st_scaler != st_scaler) set_st_scaler();
//         if ((old_st_scaler != st_scaler) && (sound_on)) al_set_mixer_gain(st_mixer, (float)st_scaler / 9);

         ya -=2;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         // ---------------------------------------
         // player color
         // ---------------------------------------

         ya+=7;
         al_draw_text(font, palette_color[tc], xa,     ya, 0, "Current player color:");
         cfp_draw_player(players[0].color,     xa+180, ya-7);

         ya+=13;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         int px1 = 0;
         int py1 = 0;
         int spacing = 76;
         for (int i=1; i<16; i++)
         {
            cfp_draw_player(i, xa+px1, ya+py1);
            if ( (mouse_x > (xa + px1)) && (mouse_x < (xa + px1 + spacing)) && (mouse_y > (ya + py1)) && (mouse_y < (ya + py1 +22)) && (mouse_b[1][0]))
            {
               while (mouse_b[1][0]) proc_controllers();
               players[0].color = i;
            }
            px1 += spacing;
            if (px1 > (spacing * 4))
            {
               px1 = 0;
               py1 += 22;
            }
         }
         ya+=64;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);
      }
// ---------------------------------------------------------------
//  1 - controls
// ---------------------------------------------------------------
      if (page == 1)
      {
         int tc = 15;  // text color
         int bts = 14; // button spacing

         int kw = 240; // width of the key display
         int kx = cfp_x1 + ((cfp_x2-cfp_x1) - kw) / 2; // x pos of key display so its is centered on page
         int ky = cfp_y1 + 16;
         int ya = redraw_all_controls(kx, ky, bts, tc, 1, -1);

         int bw = 180; // button width
         int xa = cfp_x1 + ((cfp_x2-cfp_x1) - bw) / 2;
         int xb = xa + bw;

         ya+=10;
         if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Get all new controls"))  get_all_keys(kx, ky, tc, 14);
         ya += 4;
         if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Set all to joystick 1")) set_controls_to_custom_sets(1);
         if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Set all to joystick 2")) set_controls_to_custom_sets(2);
         if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Set all to arrow keys")) set_controls_to_custom_sets(3);
         if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Set all to IJKL"))       set_controls_to_custom_sets(4);
         ya += 4;
         if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,14,15, 0,  1,0,1,0, "Test controls"))         test_keys(cfp_x1 + (cfp_x2-cfp_x1) / 2, ya-bts*7);
      }
// ---------------------------------------------------------------
//  2 - netgame
// ---------------------------------------------------------------
      if (page == 2)
      {
         int line_spacing = 10 + pct_y;
         int tc = 13;
         int fc = 15;
         int xa = cfp_x1 + 10;
         int xb = cfp_x2 - 10;
         int ya = cfp_y1;
         int bts = 16;

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, fc);


         sprintf(msg, "Server IP Address:%s", m_serveraddress);
         if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,-1,fc, 0,  1,0,1,0, msg))  edit_server_name(1, cfp_txc, ya);
         ya+=4;
         al_draw_text(font, palette_color[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "Clients need the server name");
         al_draw_text(font, palette_color[tc], cfp_txc, ya+8, ALLEGRO_ALIGN_CENTER, "or IP address set here.");

         ya+=18;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, fc);

         mdw_toggle( xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, TCP, "Packet type:UDP", "Packet type:TCP", fc, fc, -1, -1);
         al_draw_text(font, palette_color[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "The type of packet used to communicate.");

         ya+=10;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, fc);

         mdw_togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, deathmatch_pbullets, "Deathmatch player bullets", fc, fc);
         al_draw_text(font, palette_color[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "Do player's bullets affect other players?");

         ya+=10;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, fc);

         mdw_togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, suicide_pbullets, "Suicide player bullets", fc, fc);
         al_draw_text(font, palette_color[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "Do player's bullets affect themselves?");

         ya+=10;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, fc);
         ya+=4;

         mdw_slideri(xa, ya, xb, bts,  0,0,0,0,  0,12,fc,fc,  0,0,1,0, deathmatch_pbullets_damage, 100, -10, 1, "Player bullet damage:");
         ya+=4;
         al_draw_text(font, palette_color[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "The amount of damage player's bullets");
         al_draw_text(font, palette_color[tc], cfp_txc, ya+8, ALLEGRO_ALIGN_CENTER, "do to other players and themselves.");

         ya+=18;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, fc);

      }
// ---------------------------------------------------------------
//  3 - logging
// ---------------------------------------------------------------
      if (page == 3)
      {
         int line_spacing = 10;
         int xa = cfp_x1 + 10;
         int xb = cfp_x2 - 10;
         int ya = cfp_y1 + 10;
         int bts = 10;
         int tc = 13;
         int fc = 15;
         mdw_togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, LOG_NET,                    "LOG_NET", tc, fc);
         mdw_togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, LOG_NET_join,               "LOG_NET_join", tc, fc);
         mdw_togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, LOG_NET_player_array,       "LOG_NET_player_array", tc, fc);
         mdw_togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, LOG_NET_bandwidth,          "LOG_NET_bandwidth", tc, fc);
         mdw_togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, LOG_NET_cdat,               "LOG_NET_cdat", tc, fc);
         mdw_togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, LOG_NET_stdf,               "LOG_NET_stdf", tc, fc);
         mdw_togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, LOG_NET_stdf_all_packets,   "LOG_NET_stdf_all_packets", tc, fc);
         mdw_togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, LOG_NET_stdf_when_to_apply, "LOG_NET_stdf_when_to_apply", tc, fc);
         mdw_togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, LOG_NET_client_ping,        "LOG_NET_client_ping", tc, fc);
         mdw_togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, LOG_NET_client_timer_adj,   "LOG_NET_client_timer_adj", tc, fc);
         mdw_togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, LOG_NET_server_rx_stak,     "LOG_NET_server_rx_stak", tc, fc);

         ya+=10;
         bts = 14;
         xb = xa+60;
         if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,1,0,0, "All On")) set_all_logging(1);
         xa = xa+80;
         xb = xa+60;
         if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,1,1,0, "All Off")) set_all_logging(0);


         xa = cfp_x1 + 10;
         xb = cfp_x2 - 10;

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         bts = 10;
         mdw_togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, autosave_log_on_level_done,    "Autosave log on level done", tc, fc);
         mdw_togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, autosave_log_on_game_exit,     "Autosave log on game exit", tc, fc);
         mdw_togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, autosave_log_on_program_exit,  "Autosave log on program exit", tc, fc);

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         bts = 16;
         xb = xa+180;
         if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,13,15, 0,  1,0,0,0, "Run Log File Viewer")) log_file_viewer(1);
         xa = xa+200;
         xb = cfp_x2 - 10;
         if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,10,15, 0,  1,0,1,0, "Run Profile Graph")) load_profile_graph(1);
      }
// ---------------------------------------------------------------
//  4 - demo
// ---------------------------------------------------------------
      if (page == 4) // demo
      {
         int line_spacing = 14;
         int tc = 15;  // text color
         int fc = 13;  // frame color
         int xa = cfp_x1 + 10;
         int xb = cfp_x2 - 10;
         int bts = 20; // button spacing
         int ya = cfp_y1;

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         if (mdw_buttont(xa+60, ya, xb-60, bts,  0,0,0,0,  0,14,tc, 0,  1,0,1,0, "Save current game in progress")) save_gm();
         ya -=2;

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         mdw_togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, autosave_game_on_level_done, "Autosave on level done", tc, 14);
         mdw_togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, autosave_game_on_game_exit,  "Autosave on game exit", tc, 14);
         ya -=2;

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         if (mdw_buttont(xa+60, ya, xb-60, bts,  0,0,0,0,  0,fc,tc, 0,  1,0,1,0, "Choose file and run saved game"))
         if (load_gm("-"))
         {
            new_program_state = 14;
            old_program_state = 3;
            return;
         }
         ya +=10;
         if (mdw_buttont(xa+90, ya, xb-90, bts,  0,0,0,0,  0,fc,tc, 0,  1,0,1,0, "Play random demo game"))
         {
            new_program_state = 2;
            older_program_state = 3;
            return;
         }
         ya -=2;

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         mdw_togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, demo_mode_config_enable, "Autoplay random demo at program start", tc, fc);
         ya -=2;

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         float old_demo_mode_overlay_opacity = demo_mode_overlay_opacity;
         mdw_sliderd(xa, ya, xb, bts,  0,0,0,0,  0,12,fc,fc,  0,0,1,0, demo_mode_overlay_opacity, 0.8, 0, .01, "Demo mode overlay opacity:");
         if (old_demo_mode_overlay_opacity != demo_mode_overlay_opacity) save_config();

      }
// ---------------------------------------------------------------
//  5  - advanced
// ---------------------------------------------------------------
      if (page == 5)
      {
         int line_spacing = 14;
         int tc = 15;  // text color
         int fc = 13;  // frame color
         int xa = cfp_x1 + 10;
         int xb = cfp_x2 - 10;
         int bts = 20; // button spacing
         int ya = cfp_y1;

         int e = 11;    // normal
         int d = 10+96; // dim

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         ya -=12;

         int old_frame_speed = frame_speed;

         if (speed_control_lock)
         {
            mdw_togglec(xa, ya, xb, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, speed_control_lock, "Speed Control Lock", tc, 10);
            mdw_slideri(xa, ya, xb, bts,  0,0,0,0,  0,d,d,d, 0,0,1,1, frame_speed, 200, 4, 1, "Frame Speed:");
         }
         else
         {
            mdw_togglec(xa, ya, xb, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, speed_control_lock, "Speed Control Lock", tc, 11);
            mdw_slideri(xa, ya, xb, bts,  0,0,0,0,  0,e,e,e, 0,0,1,0, frame_speed, 200, 4, 1, "Frame Speed:");
         }

         if (old_frame_speed != frame_speed) set_speed();
         ya -=6;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);


         mdw_slideri(xa, ya, xb, bts,  0,0,0,0,  0,e,e,e, 0,0,1,0, display_transform_double_max, 10, 2, 1, "Display Transform Double Max:");


         static int newval = display_transform_double;

         mdw_slideri(xa, ya, xb, bts,  0,0,0,0,  0,e,e,e, 0,0,1,0, newval, display_transform_double_max, 1, 1, "New Value:");

         if (mdw_buttont(xa+60, ya, xb-60, bts,  0,0,0,0,  0,fc,tc, 0,  1,0,1,0, "Set New Value Now")) set_saved_display_transform(newval);

         if (mdw_buttont(xa+60, ya, xb-60, bts,  0,0,0,0,  0,fc,tc, 0,  1,0,1,0, "Set to Auto Mode")) set_saved_display_transform(0);



         if (saved_display_transform_double == 0) sprintf(msg, "Current Mode: %d [Auto]", display_transform_double);
         else                                     sprintf(msg, "Current Mode: %d [Static]", display_transform_double);

         mdw_buttont(xa+60, ya, xb-60, bts,  0,0,0,0,  0,fc,tc, 0,  1,0,1,0, msg);


         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);


         if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Spline adjust")) spline_adjust();
         if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Spline test")) spline_test();












      }
// ---------------------------------------------------------------
//  6 - info
// ---------------------------------------------------------------
      if (page == 6)
      {
         int line_spacing = 8;
         int tc = 15;  // text color
         //int fc = 13;  // frame color
         int xa = cfp_x1 + 10;
         int xb = cfp_x2 - 10;
         int bts = 20; // button spacing
         int ya = cfp_y1;

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);
         al_draw_text(font, palette_color[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "Sends info to the console");
         ya +=8;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Show vars")) show_var_sizes();
         if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "hostname")) get_hostname(1);
         if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Show bitmap flags 'tilemap'"))   show_bitmap_flags(al_get_bitmap_flags(tilemap));
         if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Show bitmap flags 'M_tilemap'")) show_bitmap_flags(al_get_bitmap_flags(M_tilemap));
         if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Show pixel format 'tilemap'")) show_pixel_format(al_get_bitmap_format(tilemap));
         if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Show pixel format 'M_tilemap'")) show_pixel_format(al_get_bitmap_format(M_tilemap));
         if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Show pixel format 'display'")) show_pixel_format(al_get_display_format(display));
         if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Show display flags")) show_display_flags(al_get_display_flags(display));
         if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Show display options")) show_display_options();
         if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Show display orienation")) show_display_orienation();
         if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Show refesh rate")) printf("refresh rate:%d\n", al_get_display_refresh_rate(display));
         if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Show fullscreen modes")) show_fullscreen_modes();
         if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Show display adapters")) show_display_adapters();
         if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Show code statistics")) show_code_stats();

//         ya -=2;
//         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);
      }



// ---------------------------------------------------------------
//  7 - viewport
// ---------------------------------------------------------------
      if (page == 7)
      {
         int line_spacing = 8;
         int tc = 15;  // text color
         int fc = 13;  // frame color
         int xa = cfp_x1 + 10;
         int xb = cfp_x2 - 10;
         int bts = 20; // button spacing
         int ya = cfp_y1;

         ya+=4+line_spacing;
         al_draw_text(font, palette_color[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "The viewport is the visible area of the level.");
         ya+=16;

         al_draw_text(font, palette_color[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "The player is usually near the center");
         ya+=12;
         al_draw_text(font, palette_color[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "and the level scrolls to follow the player.");

         ya +=8;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         al_draw_text(font, palette_color[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "The hysteresis rectangle is an area in the");
         ya+=12;
         al_draw_text(font, palette_color[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "center where the player can move without");
         ya+=12;
         al_draw_text(font, palette_color[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "causing the level to scroll");

         ya +=8;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         ya +=1;
         mdw_sliderd(xa, ya, xb, bts,  0,0,0,0,  0,12,fc,fc,  0,0,1,0, viewport_x_div, 1, 0.01, .01, "Hysteresis scale x:");
         mdw_sliderd(xa, ya, xb, bts,  0,0,0,0,  0,12,fc,fc,  0,0,1,0, viewport_y_div, 1, 0.01, .01, "Hysteresis scale y:");

         mdw_togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, viewport_show_hyst, "Show Hysteresis Rectangle", tc, fc);

         ya-=6;

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         al_draw_text(font, palette_color[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "Show it to help with adjustment, and leave");
         ya+=12;
         al_draw_text(font, palette_color[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "it on while playing to see how it works.");

         ya +=8;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         mdw_buttonp(xa+80, ya, xb-80, bts,  20,0,0,0,  0, 8, fc, 0,  1,0,1,0, viewport_mode);

         ya+=4;

         al_draw_text(font, palette_color[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "Instant mode moves the viewport instantly.");
         ya+=20;
         al_draw_text(font, palette_color[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "Gradual mode moves the viewport gradually.");
         ya+=12;
         al_draw_text(font, palette_color[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "It also moves the way you are facing.");
         ya+=12;
         al_draw_text(font, palette_color[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "Including up and down.");
         ya+=20;
         al_draw_text(font, palette_color[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "Static mode disables the hystersis rectangle.");
         ya+=12;
         al_draw_text(font, palette_color[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "The player is always in the center of the");
         ya+=12;
         al_draw_text(font, palette_color[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "screen. (Except near the level boundaries).");
         if (viewport_show_hyst) draw_hyst_rect();
      }

// ---------------------------------------------------------------
//  8 - profiling
// ---------------------------------------------------------------
      if (page == 8)
      {
         int line_spacing = 10;
         int xa = cfp_x1 + 10;
         int xb = cfp_x2 - 10;
         int ya = cfp_y1 + 10;
         int bts = 10;
         int tc = 13;
         int fc = 15;

         mdw_togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, LOG_TMR_move_tot,      "LOG_TMR_move_tot", tc, fc);
         mdw_togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, LOG_TMR_move_all,      "LOG_TMR_move_all", tc, fc);
         mdw_togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, LOG_TMR_move_enem,     "LOG_TMR_move_enem", tc, fc);
         mdw_togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, LOG_TMR_draw_tot,      "LOG_TMR_draw_tot", tc, fc);
         mdw_togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, LOG_TMR_draw_all,      "LOG_TMR_draw_all", tc, fc);
         mdw_togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, LOG_TMR_sdif,          "LOG_TMR_sdif", tc, fc);
         mdw_togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, LOG_TMR_cdif,          "LOG_TMR_cdif", tc, fc);
         mdw_togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, LOG_TMR_rwnd,          "LOG_TMR_rwnd", tc, fc);
         mdw_togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, LOG_TMR_bmsg_add,      "LOG_TMR_bmsg_add", tc, fc);
         mdw_togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, LOG_TMR_bmsg_draw,     "LOG_TMR_bmsg_draw", tc, fc);

         ya+=10;
         bts = 14;
         xb = xa+60;
         if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,1,0,0, "All On")) set_all_logging(1);
         xa = xa+80;
         xb = xa+60;
         if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,1,1,0, "All Off")) set_all_logging(0);


         xa = cfp_x1 + 10;
         xb = cfp_x2 - 10;

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         bts = 10;
         mdw_togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, autosave_log_on_level_done,    "Autosave log on level done", tc, fc);
         mdw_togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, autosave_log_on_game_exit,     "Autosave log on game exit", tc, fc);
         mdw_togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, autosave_log_on_program_exit,  "Autosave log on program exit", tc, fc);

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         bts = 16;
         if (mdw_buttont(xa+40, ya, xb-40, bts,  0,0,0,0,  0,10,15, 0,  1,0,1,0, "Open Most Recent Profile Graph")) load_profile_graph(0);

         ya += 8;

         if (mdw_buttont(xa+40, ya, xb-40, bts,  0,0,0,0,  0,13,15, 0,  1,0,1,0, "Select and Open Profile Graph")) load_profile_graph(1);

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         if (mdw_buttont(xa+80, ya, xb-80, bts,  0,0,0,0,  0,11,15, 0,  1,0,1,0, "Start Single Player Game"))
         {
            new_program_state = 10;
            old_program_state = 3;
            return;
         }

         ya += 8;
         xb = xa+180;
         if (mdw_buttont(xa+20, ya, xb, bts,  0,0,0,0,  0,9,15, 0,  1,0,0,0, "Host Network Game"))
         {
            new_program_state = 20;
            old_program_state = 3;
            return;
         }
         xa = xa+200;
         xb = cfp_x2 - 30;
         if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,8,15, 0,  1,0,1,0, "Join Network Game"))
         {
            new_program_state = 24;
            old_program_state = 3;
            return;
         }
      }


      if (key[ALLEGRO_KEY_ESCAPE][0])
      {
         while (key[ALLEGRO_KEY_ESCAPE][0]) proc_controllers();
         quit = 1;
      }
   }
   al_hide_mouse_cursor(display);
   save_config();
   new_program_state = 1;
   old_program_state = 1;
}

