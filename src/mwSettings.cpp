// mwSettings.cpp

#include "pm.h"
#include "mwSettings.h"
#include "mwSound.h"
#include "mwLog.h"
#include "mwPlayers.h"
#include "mwNetgame.h"
#include "mwLogo.h"
#include "mwBottomMessage.h"
#include "mwDemoMode.h"
#include "mwDisplay.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwWidgets.h"
#include "mwColor.h"
#include "mwInput.h"
#include "mwEventQueue.h"
#include "mwMenu.h"
#include "mwLoop.h"
#include "mwEnemy.h"
#include "mwLevel.h"
#include "mwConfig.h"
#include "mwMiscFnx.h"
#include "mwScreen.h"
#include "mwShots.h"
#include "mwGameMoves.h"
#include "mwCodeStats.h"
#include "mwHelp.h"
#include "mwVisualLevel.h"


mwSettings mSettings;

void mwSettings::set_all_logging(int v)
{
   mLog.LOG_NET=v;
   mLog.LOG_NET_join=v;
   mLog.LOG_NET_player_array=v;
   mLog.LOG_NET_bandwidth=v;
   mLog.LOG_NET_cdat=v;
   mLog.LOG_NET_stdf=v;
   mLog.LOG_NET_stdf_all_packets=v;
   mLog.LOG_NET_dif_applied=v;
   mLog.LOG_NET_dif_not_applied=v;
   mLog.LOG_NET_client_ping=v;
   mLog.LOG_NET_client_timer_adj=v;
   mLog.LOG_NET_server_rx_stak=v;
   mLog.LOG_TMR_cpu=v;
   mLog.LOG_TMR_move_tot=v;
   mLog.LOG_TMR_move_all=v;
   mLog.LOG_TMR_move_enem=v;
   mLog.LOG_TMR_bmsg_add=v;
   mLog.LOG_TMR_bmsg_draw=v;
   mLog.LOG_TMR_scrn_overlay=v;
   mLog.LOG_TMR_draw_tot=v;
   mLog.LOG_TMR_draw_all=v;
   mLog.LOG_TMR_sdif=v;
   mLog.LOG_TMR_cdif=v;
   mLog.LOG_TMR_rwnd=v;
   mLog.LOG_TMR_client_timer_adj=v;
   mLog.LOG_TMR_client_ping=v;
}










void mwSettings::redraw_one_fcontrol(int x, int y, int &ya, int bts, int tc, int show_buttons, int num, int num_lines, int i, int &key, const char* nam)
{
   int xa = x+1;
   int xb = xa + 35;  // 'new' button end
   int tx = xb + 10;  // text pos padded after 'new' button
   float yo = (bts-8)/2; // text y offset so it is centered with the button

   if (num == i) al_draw_textf(mFont.pr8, mColor.pc[10], tx, ya+yo, 0, "%s set new control", nam);
   else          al_draw_textf(mFont.pr8, mColor.pc[tc], tx, ya+yo, 0, "%s %s", nam, mInput.key_names[key]);

   ya+=1; bts-=2; // make button a little smaller
   if ((show_buttons) && (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,13,15, 0,  1,1,0,0, "new"))) mInput.my_readkey3(x, y, tc, bts+2, i, num_lines);

   ya-=1; bts+=2; // fix values

   al_draw_line(x-1, ya-1, xa+320, ya-1, mColor.White, 0);
   ya+=bts;
}


int mwSettings::redraw_all_fcontrols(int x, int y, int bts, int tc, int show_buttons, int num, int num_lines)
{
   int w = 320;
   int ya = y;
   int yb = ya + bts*num_lines-1;

   al_draw_filled_rectangle(x-1, y-1, x+w+1, yb, mColor.pc[13+224]); // erase background
   al_draw_rectangle       (x-1, y-1, x+w+1, yb, mColor.pc[15], 0);  // frame


   float x9 = 38;
   al_draw_line            (x+x9, y-1, x+x9, yb, mColor.pc[15], 0);  // frame
   x9 = 178;
   al_draw_line            (x+x9, y-1, x+x9, yb, mColor.pc[15], 0);  // frame


   for (int h=0; h<num_lines; h++)
   {
      if (h ==  0) redraw_one_fcontrol(x, y, ya, bts, tc, show_buttons, num, num_lines,  0, mInput.function_key_fullscreen,      "Fullscreen      ");
      if (h ==  1) redraw_one_fcontrol(x, y, ya, bts, tc, show_buttons, num, num_lines,  1, mInput.function_key_text_double,     "Text Double     ");
      if (h ==  2) redraw_one_fcontrol(x, y, ya, bts, tc, show_buttons, num, num_lines,  2, mInput.function_key_debug_overlay,   "Debug Overlay   ");
      if (h ==  3) redraw_one_fcontrol(x, y, ya, bts, tc, show_buttons, num, num_lines,  3, mInput.function_key_zoom_out,        "Zoom Out        ");
      if (h ==  4) redraw_one_fcontrol(x, y, ya, bts, tc, show_buttons, num, num_lines,  4, mInput.function_key_zoom_in,         "Zoom In         ");
      if (h ==  5) redraw_one_fcontrol(x, y, ya, bts, tc, show_buttons, num, num_lines,  5, mInput.function_key_speed_dec,       "Speed Decrease  ");
      if (h ==  6) redraw_one_fcontrol(x, y, ya, bts, tc, show_buttons, num, num_lines,  6, mInput.function_key_speed_inc,       "Speed Increase  ");
      if (h ==  7) redraw_one_fcontrol(x, y, ya, bts, tc, show_buttons, num, num_lines,  7, mInput.function_key_printscreen,     "Save Screenshot ");
      if (h ==  8) redraw_one_fcontrol(x, y, ya, bts, tc, show_buttons, num, num_lines,  8, mInput.function_key_fakekey,         "Fake Key Mode   ");
      if (h ==  9) redraw_one_fcontrol(x, y, ya, bts, tc, show_buttons, num, num_lines,  9, mInput.function_key_force_save,      "Force Save      ");
      if (h == 10) redraw_one_fcontrol(x, y, ya, bts, tc, show_buttons, num, num_lines, 10, mInput.function_key_init_background, "Init Redraw     ");
   }
   return ya;
}























void mwSettings::redraw_one_control(int x, int y, int &ya, int bts, int tc, int show_buttons, int num, int i, int &key, const char* nam)
{
   int xa = x+1;
   int xb = xa + 35;  // 'new' button end
   int tx = xb + 10;  // text pos padded after 'new' button
   float yo = (bts-8)/2; // text y offset so it is centered with the button

   if (num == i) al_draw_textf(mFont.pr8, mColor.pc[10], tx, ya+yo, 0, "%s set new control", nam);
   else          al_draw_textf(mFont.pr8, mColor.pc[tc], tx, ya+yo, 0, "%s %s", nam, mInput.key_names[key]);

   ya+=1; bts-=2; // make button a little smaller
   if ((show_buttons) && (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,13,15, 0,  1,1,0,0, "new"))) mInput.my_readkey2(x, y, tc, bts+2, i);

   ya-=1; bts+=2; // fix values

   al_draw_line(x-1, ya-1, xa+240, ya-1, mColor.White, 0);
   ya+=bts;
}




int mwSettings::redraw_all_controls(int x, int y, int bts, int tc, int show_buttons, int num)
{
   int ya = y;

   al_draw_filled_rectangle(x-1, y-1, x+241, ya + bts*7-1, mColor.pc[13+224]); // erase background
   al_draw_rectangle       (x-1, y-1, x+241, ya + bts*7-1, mColor.pc[15], 0);  // frame


   float x9 = 38;
   al_draw_line            (x+x9, y-1, x+x9, ya + bts*7-1, mColor.pc[15], 0);  // frame
   x9 = 89;
   al_draw_line            (x+x9, y-1, x+x9, ya + bts*7-1, mColor.pc[15], 0);  // frame


   redraw_one_control(x, y, ya, bts, tc, show_buttons, num, 0, mPlayer.loc[0].up_key,    "Up   ");
   redraw_one_control(x, y, ya, bts, tc, show_buttons, num, 1, mPlayer.loc[0].down_key,  "Down ");
   redraw_one_control(x, y, ya, bts, tc, show_buttons, num, 2, mPlayer.loc[0].left_key,  "Left ");
   redraw_one_control(x, y, ya, bts, tc, show_buttons, num, 3, mPlayer.loc[0].right_key, "Right");
   redraw_one_control(x, y, ya, bts, tc, show_buttons, num, 4, mPlayer.loc[0].jump_key,  "Jump ");
   redraw_one_control(x, y, ya, bts, tc, show_buttons, num, 5, mPlayer.loc[0].fire_key,  "Fire ");
   redraw_one_control(x, y, ya, bts, tc, show_buttons, num, 6, mPlayer.loc[0].menu_key,  "Menu ");

   return ya;
}

void mwSettings::cfp_draw_player(int pco, int x, int y)
{
   al_draw_bitmap(mBitmap.player_tile[pco][1], x, y, 0 );
   al_draw_text(mFont.pr8, mColor.pc[pco], x+22, y+7, 0, mColor.color_name[pco]);
}

int mwSettings::cfp_draw_line(int xa, int xb, int ya, int line_spacing, int col)
{
   ya+=line_spacing;
   al_draw_line(xa, ya, xb, ya, mColor.pc[col], 1);
   ya+=line_spacing;
   return ya;
}

void mwSettings::cfp_4tog(int xa, int xb, int &ya, int bts, int tc, int fc, int line_spacing, int index, const char * name)
{
   ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);
   ya -=7;
   al_draw_text(mFont.pr8, mColor.pc[tc], xa, ya, 0, name);
   for (int i=0; i<4; i++)
      mWidget.togglec(xb-86+(i*24), ya, xb-72+(i*24), bts,  0,0,0,0,  0, 0, 0, 0,  1,0,0,0, overlay_grid[index][i],  "", tc, fc);
}



void mwSettings::draw_tab(struct settings_tab st[], int p, int col, int text_color)
{
   int s = 2; // slant
   float v[4][2] = {0};
   v[0][0] = st[p].x1+s;  v[0][1] = st[p].y1; // ul
   v[1][0] = st[p].x1;    v[1][1] = st[p].y2; // ll
   v[2][0] = st[p].x2;    v[2][1] = st[p].y2; // lr
   v[3][0] = st[p].x2-s;  v[3][1] = st[p].y1; // ur
   int bc = col+192; if (bc > 256) bc = 0; //  tab background color
   al_draw_filled_polygon(*v, 4, mColor.pc[bc]);
   al_draw_polygon(*v, 4, ALLEGRO_LINE_JOIN_ROUND, mColor.pc[col], 1.0, 0);
   al_draw_text(mFont.pr8, mColor.pc[text_color], st[p].x1+4, st[p].y1+3, 0, st[p].title);
}

void mwSettings::settings_pages(int set_page)
{
   char msg[1024];
   if (set_page != -1)  current_page = set_page;
   int page = current_page;


   struct settings_tab st[20] = {0};

   sprintf(st[0].title,  "Main");
   sprintf(st[1].title,  "Controls");
   sprintf(st[2].title,  "Controls 2");
   sprintf(st[3].title,  "Netgame");
   sprintf(st[4].title,  "Demo");
   sprintf(st[5].title,  "Message");

   sprintf(st[8].title,  "Viewport");
   sprintf(st[9].title,  "Overlay");
   sprintf(st[10].title, "Double");
   sprintf(st[11].title, "Speed");

   sprintf(st[15].title,  "Info");
   sprintf(st[16].title,  "Profiling");
   sprintf(st[17].title,  "Logging");
   sprintf(st[18].title,  "misc");


   int num_pages = 20;


   const char *title = "Settings";



   int fc = 10; // frame color
   int tc = 15; // text color
   int frame_width = 4; // border width

   int quit = 0;
   while (!quit)
   {

      for (int i=0; i<20; i++) st[i].show = 0; // all off

      for (int i=0; i<6; i++) st[i].show = 1; // always on

      if (show_advanced)
      {
         for (int i=8; i<12; i++) st[i].show = 1;
         if (show_debug)
            for (int i=15; i<19; i++) st[i].show = 1;
      }

      // if current page is not shown, change to page 0
      if (!st[page].show) current_page = page = 0;

      al_show_mouse_cursor(mDisplay.display);
      // entire area, including title, tabs, and page
      int cf_w = 400;
      int cf_x1 = (mDisplay.SCREEN_W - cf_w)/2;
      int cf_x2 = cf_x1 + cf_w;

      int cf_h = 400;
      int cf_y1 = mLogo.menu_map_y - 61; // line up exactly with the menu item "Settings"
      int cf_y2 = cf_y1 + cf_h;

      if (cf_y2 > mDisplay.SCREEN_H)     // if bottom is past bottom of screen
      {
         cf_y2 = mDisplay.SCREEN_H;      // bottom is bottom of screen
         cf_y1 = cf_y2 - cf_h;

         if (cf_y1 < 0)         // if top is past top of screen
         {
            cf_y1 = 0;          // top is top of screen
            cf_y2 = cf_y1 + cf_h;
         }
      }

      fc = mPlayer.syn[0].color; // frame color

      al_set_target_backbuffer(mDisplay.display);
      al_flip_display();
      al_clear_to_color(al_map_rgb(0, 0, 0));
      mScreen.frame_and_title(1);
      mLogo.mdw_an(mLogo.mdw_map_logo_x, mLogo.mdw_map_logo_y, mLogo.mdw_map_logo_scale);
      for (int c=0; c<7; c++)       // show first 7 menu items
      {
         int b = 15;
         if ((!mLevel.resume_allowed) && (c==4)) b+=80; // dimmer if can't resume
         al_draw_text(mFont.pr8, mColor.pc[b], mDisplay.SCREEN_W/2, 14+(c*10)+1, ALLEGRO_ALIGN_CENTRE, mMenu.menu_string[c]);
      }

      while (!mEventQueue.menu_update) mEventQueue.proc();
      mEventQueue.menu_update = 0;

      al_draw_filled_rectangle(cf_x1, cf_y1, cf_x2, cf_y2, mColor.pc[fc+224]); // erase everything

      // figure out the title size
      int bx, by, bw, bh;
      al_get_text_dimensions(mFont.pr8, title, &bx, &by, &bw, &bh);
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
      int next_x1 = cfp_x1+1; // start at page x1;
      int next_y2 = 0;        // we don't know where are going to start yet, so do them at offset 0
      for (int i=0; i<num_pages; i++)
      {
         if (st[i].show)
         {
            st[i].x1 = next_x1;

            st[i].y2 = next_y2;
            int bx, by, bw, bh; // set size of tab based on text size
            al_get_text_dimensions(mFont.pr8, st[i].title, &bx, &by, &bw, &bh);
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
            next_x1 = st[i].x2 + 1; // x1 of next tab is x2 of this tab plus pad
            next_y2 = st[i].y2;     // y2 of next tab is the same y2 as this tab

         }
      }

      int tabs_height = 8 - (next_y2-bh); // find the height of the last tab

      // now we can set the top of the page, where the tabs will start
      int cfp_y1 = cf_y1 + tabs_height + title_h;
      //int cfp_y2 = cf_y2 - frame_width;

      // frame everything
      for (int a=0; a<frame_width; a++)
         al_draw_rounded_rectangle(cf_x1+a, cf_y1+a, cf_x2-a, cf_y2-a, 4, 4, mColor.pc[fc+a*48], 1.5);

      // frame from top of page to to top of window, this is here to draw under the tabs
      for (int a=0; a<frame_width; a++)
         al_draw_rounded_rectangle(cf_x1+a, cf_y1+a, cf_x2-a, cfp_y1-a+3, 4, 4, mColor.pc[fc+a*48], 1.5);

      // frame and draw title, this needs to go on top of previous frame
      for (int a=0; a<frame_width; a++)
         al_draw_rounded_rectangle(title_x1+a, title_y1+a, title_x2-a, title_y2-a, 4, 4, mColor.pc[fc+a*48], 1.5);
      al_draw_text(mFont.pr8, mColor.pc[tc], title_xc, title_ty, ALLEGRO_ALIGN_CENTER, title);

      int xa = title_xc+46;
      int ya = title_ty;
      mWidget.togglec(xa, ya, xa+80, 12,  0,0,0,0,  0, 0, 0, 0,  1,0,0,0, show_advanced, "Advanced", tc, tc);

      if (show_advanced)
      {
         xa += 88;
         mWidget.togglec(xa, ya, xa+56, 12,  0,0,0,0,  0, 0, 0, 0,  1,0,0,0, show_debug, "Debug", tc, tc);
      }

      // draw and process tabs
      int mouse_on_tab = -1;
      for (int i=0; i<num_pages; i++)
      {
         if (st[i].show)
         {
            st[i].y1 += cfp_y1; // adjust y values to top of page
            st[i].y2 += cfp_y1;
            if ((mInput.mouse_x > st[i].x1) && (mInput.mouse_x < st[i].x2) && (mInput.mouse_y > st[i].y1) && (mInput.mouse_y < st[i].y2)) mouse_on_tab = i;
            draw_tab(st, i, fc+128, tc+128); // draw the tab
         }
      }

      draw_tab(st, page, fc, tc); // draw the current tab on top
      if (mouse_on_tab != -1)
      {
         draw_tab(st, mouse_on_tab, fc, tc); // draw the tab the mouse is on
         if (mInput.mouse_b[1][0])
         {
            while (mInput.mouse_b[1][0]) mEventQueue.proc();
            current_page = page = mouse_on_tab;
         }
      }

      // frame page
      for (int a=0; a<frame_width; a++)
         al_draw_rounded_rectangle(cf_x1+a, cfp_y1+a, cf_x2-a, cf_y2-a, 4, 4, mColor.pc[fc+a*48], 1.5);








// ---------------------------------------------------------------
//  0 - Main
// ---------------------------------------------------------------
      if (page == 0)
      {
         int line_spacing = 11;
         //line_spacing += mLoop.pct_y;
         if (show_advanced) line_spacing = 8;
         int tc = 15;  // text color
         int fc = 13;  // frame color
         int xa = cfp_x1 + 10;
         int xb = cfp_x2 - 10;
         int bts = 20; // button spacing
         //bts += mLoop.pct_y;
         int ya = cfp_y1;



         // ---------------------------------------
         // player color
         // ---------------------------------------
         ya+= line_spacing;


         fc = mPlayer.syn[0].color;
         int frame_y1 = ya;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, fc);


         ya+=7;
         al_draw_text(mFont.pr8, mColor.pc[tc], xa,     ya, 0, "Current player color:");
         cfp_draw_player(mPlayer.syn[0].color,     xa+180, ya-7);

         ya+=13;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, fc);

         int px1 = 0;
         int py1 = 0;
         int spacing = 76;
         for (int i=1; i<16; i++)
         {
            cfp_draw_player(i, xa+px1, ya+py1);
            if ( (mInput.mouse_x > (xa + px1)) && (mInput.mouse_x < (xa + px1 + spacing)) && (mInput.mouse_y > (ya + py1)) && (mInput.mouse_y < (ya + py1 +22)) && (mInput.mouse_b[1][0]))
            {
               while (mInput.mouse_b[1][0]) mEventQueue.proc();
               mPlayer.syn[0].color = i;
            }
            px1 += spacing;
            if (px1 > (spacing * 4))
            {
               px1 = 0;
               py1 += 22;
            }
         }
         ya+=64;

         al_draw_line(cfp_x1+4, frame_y1+line_spacing, cfp_x1+4, ya+line_spacing, mColor.pc[fc], 1 ); // draw the sides of the frame first
         al_draw_line(cfp_x2-4, frame_y1+line_spacing, cfp_x2-4, ya+line_spacing, mColor.pc[fc], 1 );
         ya = cfp_draw_line(cfp_x1+4, cfp_x2-4, ya, line_spacing, fc);




         // section with: sound, splash screen, message, demo
         fc = 13;
         frame_y1 = ya;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, fc);


         // ---------------------------------------
         // sound
         // ---------------------------------------
         int x1a = cfp_x1 + 10;
         int x1b = x1a + 60;
         int old_sound_on = mSound.sound_on;
         mWidget.togglec(x1a, ya, x1b, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,0,0, mSound.sound_on, "Sound", tc, tc);
         if ((old_sound_on == 0) && (mSound.sound_on == 1)) mSound.load_sound();

         int sc = 13;
         int dim = 0;
         if (!mSound.sound_on) dim = 128;

         int old_se_scaler = mSound.se_scaler;
         x1a = x1b + 12;
         x1b = x1a + 140;
         mWidget.slideri(x1a, ya, x1b, bts,  0,0,0,0,  0,sc+dim,15+dim+32,0,  0,0,0,0, mSound.se_scaler, 9, 0, 1, "Sound Effects:");

         if (old_se_scaler != mSound.se_scaler) mSound.set_se_scaler();

         int old_st_scaler = mSound.st_scaler;
         x1a = x1b + 12;
         x1b = x1a + 140;
         mWidget.slideri(x1a, ya, x1b, bts,  0,0,0,0,  0,sc+dim,15+dim+32,0,  0,0,1,0, mSound.st_scaler, 9, 0, 1, "Sound Track:");
         if (old_st_scaler != mSound.st_scaler) mSound.set_st_scaler();

         ya -=2;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, fc);






         // ---------------------------------------
         // splash screen
         // ---------------------------------------
         x1a = cfp_x1 + 10;
         x1b = x1a + 250;
         mWidget.togglec(x1a, ya, x1b, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,0,0, mLogo.show_splash_screen, "Show splash screen on startup", tc, tc);
         x1a = x1b + 24;
         x1b = x1a + 80;
         if (mWidget.buttont(x1a, ya, x1b, bts,  0,0,0,0,  0,8,tc, 0,  1,0,1,0, "Show now"))
         {
             mLogo.splash_screen();
             while (mInput.key[ALLEGRO_KEY_ESCAPE][0]) mEventQueue.proc();
         }
         ya -=2;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, fc);

         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, mBottomMessage.bottom_msg_on, "Show bottom message display", tc, 15);

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mDemoMode.demo_mode_config_enable, "Autoplay random demo at program start", tc, 15);
         ya -=2;

         al_draw_line(cfp_x1+4, frame_y1+line_spacing, cfp_x1+4, ya+line_spacing, mColor.pc[fc], 1 ); // draw the sides of the frame first
         al_draw_line(cfp_x2-4, frame_y1+line_spacing, cfp_x2-4, ya+line_spacing, mColor.pc[fc], 1 );
         ya = cfp_draw_line(cfp_x1+4, cfp_x2-4, ya, line_spacing, fc);


         if (show_advanced)
         {
            ya+= line_spacing;


            x1a = cfp_x1 + 60;
            x1b = cfp_x2 - 60;
            if (mWidget.buttont(x1a, ya, x1b, bts,  0,0,0,0,  0,10,14, 0,  1,0,1,0, "Reset All Settings to Defaults!"))
            {
               char sys_cmd[500];
               sprintf(sys_cmd, "del pm.cfg");                         printf("%s\n",sys_cmd);   system(sys_cmd);
               sprintf(sys_cmd, "del bitmaps\\mW.pm ");                printf("%s\n",sys_cmd);   system(sys_cmd);
               mConfig.load();

            }
         }

//         fc = 10;
//         frame_y1 = ya;
//         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, fc);
//
//         x1a = cfp_x1 + 10;
//         x1b = x1a + 250;
//         mWidget.togglec(x1a, ya, x1b, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, show_advanced, "Show Advanced Settings", tc, tc);
//         mWidget.togglec(x1a, ya, x1b, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, show_debug,    "Show Debug Settings",    tc, tc);
//
//         ya -=2;
//         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, fc);
//
//         x1a = cfp_x1 + 60;
//         x1b = cfp_x2 - 60;
//         if (mWidget.buttont(x1a, ya, x1b, bts,  0,0,0,0,  0,10,14, 0,  1,0,1,0, "Reset All Settings to Defaults!"))
//         {
//            char sys_cmd[500];
//            sprintf(sys_cmd, "del pm.cfg");                         printf("%s\n",sys_cmd);   system(sys_cmd);
//            sprintf(sys_cmd, "del bitmaps\\mW.pm ");                printf("%s\n",sys_cmd);   system(sys_cmd);
//            mConfig.load();
//
//         }
//
//         ya -= 2;
//         al_draw_line(cfp_x1+4, frame_y1+line_spacing, cfp_x1+4, ya+line_spacing, mColor.pc[fc], 1 ); // draw the sides of the frame first
//         al_draw_line(cfp_x2-4, frame_y1+line_spacing, cfp_x2-4, ya+line_spacing, mColor.pc[fc], 1 );
//         ya = cfp_draw_line(cfp_x1+4, cfp_x2-4, ya, line_spacing, fc);
      }
// ---------------------------------------------------------------
//  1 - controls
// ---------------------------------------------------------------
      if (page == 1)
      {
         int tc = 15;  // text color
         int bts = 18; // button spacing
         //bts += mLoop.pct_y;
         int line_spacing = 12;
         //line_spacing +=  mLoop.pct_y;
         int xa = cfp_x1 + 10;
         int xb = cfp_x2 - 10;
         int ya = cfp_y1 + 10;

         al_draw_text(mFont.pr8, mColor.pc[15], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "Game Controls");
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, 15);

         int kw = 240; // width of the key display
         int kx = cfp_x1 + ((cfp_x2-cfp_x1) - kw) / 2; // x pos of key display so its is centered on page
         ya = redraw_all_controls(kx, ya, bts, tc, 1, -1);

         int bw = 280; // button width
         xa = cfp_x1 + ((cfp_x2-cfp_x1) - bw) / 2;
         xb = xa + bw;

         int ty1 = ya;

         ya+=12;
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Prompt to set all new controls"))  mInput.get_all_keys(kx, ya, tc, bts);

         int gc = 11;

         bw = 200; // button width
         xa = cfp_x1 + ((cfp_x2-cfp_x1) - bw) / 2;
         xb = xa + bw;

         ya += 12;
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,gc,15, 0,  1,0,1,0, "Set all to joystick 1")) mInput.set_controls_to_custom_sets(1);
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,gc,15, 0,  1,0,1,0, "Set all to joystick 2")) mInput.set_controls_to_custom_sets(2);
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,gc,15, 0,  1,0,1,0, "Set all to arrow keys")) mInput.set_controls_to_custom_sets(3);
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,gc,15, 0,  1,0,1,0, "Set all to IJKL"))       mInput.set_controls_to_custom_sets(4);
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,gc,15, 0,  1,0,1,0, "Set all to WASD"))       mInput.set_controls_to_custom_sets(5);

         ya += 12;
         xa = cfp_x1 + 40;
         xb = cfp_x2 - 40;
         int xw4 = (xb - xa)/2;

         xb = xa+xw4-10;
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,14,15, 0,  1,0,0,0, "Test controls"))
         {
            al_draw_filled_rectangle(cf_x1+10, ty1, cf_x2-10, cf_y2-10, mColor.pc[fc+224]); // erase everything
            mInput.test_keys(cfp_x1 + (cfp_x2-cfp_x1) / 2, ya-bts*7);
         }

         xa += xw4; xb = xa+xw4-10;
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,10,15, 0,  1,0,1,0, "Controls Help")) mHelp.help("Game Controls");

      }


// ---------------------------------------------------------------
//  2 - Controls 2 (Function Keys)
// ---------------------------------------------------------------
      if (page == 2)
      {
         int line_spacing = 12;
         //line_spacing +=  mLoop.pct_y;
         int xa = cfp_x1 + 10;
         int xb = cfp_x2 - 10;
         int ya = cfp_y1 + 10;
         int bts = 20;
         //bts += mLoop.pct_y;
         int tc = 13;

         int last_control = 8;

         if (show_advanced) last_control = 11;

         al_draw_text(mFont.pr8, mColor.pc[15], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "Function Keys");
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, 15);

         ya = redraw_all_fcontrols(xa+24, ya, bts, tc, 1, -1, last_control);
         ya += line_spacing;
         if (mWidget.buttont(xa+90, ya, xb-90, bts,  0,0,0,0,  0,11,15, 0,  1,0,1,0, "Reset all to defaults"))
         {
            mInput.function_key_fullscreen      = ALLEGRO_KEY_F12;
            mInput.function_key_text_double     = ALLEGRO_KEY_F11;
            mInput.function_key_debug_overlay   = ALLEGRO_KEY_F10;
            mInput.function_key_zoom_out        = ALLEGRO_KEY_F5;
            mInput.function_key_zoom_in         = ALLEGRO_KEY_F6;
            mInput.function_key_speed_dec       = ALLEGRO_KEY_F7;
            mInput.function_key_speed_inc       = ALLEGRO_KEY_F8;
            mInput.function_key_printscreen     = ALLEGRO_KEY_PRINTSCREEN;
            mInput.function_key_fakekey         = 0;
            mInput.function_key_force_save      = 0;
            mInput.function_key_init_background = 0;
         }
      }


// ---------------------------------------------------------------
//  3 - netgame
// ---------------------------------------------------------------
      if (page == 3)
      {
         int line_spacing = 10;
         if (show_advanced) line_spacing = 5;
//         line_spacing +=  mLoop.pct_y;
         int tc = 13;
         int fc = 15;
         int xa = cfp_x1 + 10;
         int xb = cfp_x2 - 10;
         int ya = cfp_y1 + 4;
         int bts = 16;



         fc = 8;
         ya += line_spacing;
         int frame_y1 = ya;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, fc);

         al_draw_text(mFont.pr8, mColor.pc[15], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "Server Name or IP Address");
         ya+=12;

         sprintf(msg, "%s", mNetgame.m_serveraddress);

         if (mWidget.buttontcb(cfp_txc - (strlen(msg)*4), ya, 0, 13,  0,0,0,0,  0,15,15,10,  1,0,1,0, msg))  mMiscFnx.edit_server_name(cfp_txc, ya-28);

         ya+=8;
         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "Clients require the Server's Name or IP");

         ya += 8;
         al_draw_line(cfp_x1+4, frame_y1+line_spacing, cfp_x1+4, ya+line_spacing, mColor.pc[fc], 1 ); // draw the sides of the frame first
         al_draw_line(cfp_x2-4, frame_y1+line_spacing, cfp_x2-4, ya+line_spacing, mColor.pc[fc], 1 );
         ya = cfp_draw_line(cfp_x1+4, cfp_x2-4, ya, line_spacing, fc);



         fc = 12;
         frame_y1 = ya;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, fc);

         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mShot.deathmatch_shots, "Deathmatch player shots", 15, 15);
         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "Do player's shots affect other players?");

         ya+=10;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, fc);

         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mShot.suicide_shots, "Suicide player shots", 15, 15);
         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "Do player's shots affect themselves?");

         ya+=10;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, fc);
         ya+=4;

         mWidget.slideri(xa, ya, xb, bts,  0,0,0,0,  0,12,15,15,  0,0,1,0, mShot.deathmatch_shot_damage, 100, -10, 1, "Player shot damage:");
         ya+=4;
         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "The amount of damage player's shots do to");
         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya+10, ALLEGRO_ALIGN_CENTER, "other players and themselves.");


         ya += 20;
         al_draw_line(cfp_x1+4, frame_y1+line_spacing, cfp_x1+4, ya+line_spacing, mColor.pc[fc], 1 ); // draw the sides of the frame first
         al_draw_line(cfp_x2-4, frame_y1+line_spacing, cfp_x2-4, ya+line_spacing, mColor.pc[fc], 1 );
         ya = cfp_draw_line(cfp_x1+4, cfp_x2-4, ya, line_spacing, fc);

         ya+=18;
         if (mWidget.buttont(xa+120, ya, xb-120, bts,  0,0,0,0,  0,10,15, 0,  1,0,1,0, "Netgame Help")) mHelp.help("Netgame");


         if (show_advanced)
         {
            ya+=20;

            fc = 14;
            frame_y1 = ya;
            ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, fc);

            al_draw_text(mFont.pr8, mColor.pc[14], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "Advanced Settings");
            ya+=8;
            ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, fc);

            ya-=2;
            mWidget.toggle( xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mNetgame.TCP, "Packet type:UDP", "Packet type:TCP", 15, 15, -1, -1);
            ya-=4;

            ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, fc);

            ya-=2;
            mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, mPlayer.loc[0].server_state_freq_mode, "State Frequency Auto Adjust", 15, 15);
            ya-=4;

            al_draw_line(cfp_x1+4, frame_y1+line_spacing, cfp_x1+4, ya+line_spacing, mColor.pc[fc], 1 ); // draw the sides of the frame first
            al_draw_line(cfp_x2-4, frame_y1+line_spacing, cfp_x2-4, ya+line_spacing, mColor.pc[fc], 1 );
            ya = cfp_draw_line(cfp_x1+4, cfp_x2-4, ya, line_spacing, fc);
         }
      }


// ---------------------------------------------------------------
//  4 - demo
// ---------------------------------------------------------------
      if (page == 4) // demo
      {
         int line_spacing = 12;

         //line_spacing +=  mLoop.pct_y;

         int tc = 15;  // text color
         int fc = 13;  // frame color
         int xa = cfp_x1 + 10;
         int xb = cfp_x2 - 10;
         int bts = 20; // button spacing
         int ya = cfp_y1;

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);


         if (mWidget.buttont(xa+90, ya, xb-90, bts,  0,0,0,0,  0,fc,tc, 0,  1,0,1,0, "Play random demo game"))
         {
            mLoop.new_program_state = 2;
            mLoop.older_program_state = 3;
            al_hide_mouse_cursor(mDisplay.display);
            mConfig.save();
            return;
         }
         ya +=10;
         if (mWidget.buttont(xa+60, ya, xb-60, bts,  0,0,0,0,  0,fc,tc, 0,  1,0,1,0, "Choose file and run saved game"))
         if (mGameMoves.load_gm("-"))
         {
            mLoop.new_program_state = 14;
            mLoop.old_program_state = 3;
            al_hide_mouse_cursor(mDisplay.display);
            mConfig.save();
            return;
         }

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);


         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mDemoMode.demo_mode_config_enable, "Autoplay random demo at program start", tc, fc);

         ya -=4;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         if (mWidget.buttont(xa+60, ya, xb-60, bts,  0,0,0,0,  0,14,tc, 0,  1,0,1,0, "Save current game in progress")) mGameMoves.save_gm();

         ya +=6;
         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mLog.autosave_game_on_level_done, "Autosave on level done", tc, 14);
         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mLog.autosave_game_on_game_exit,  "Autosave on game exit", tc, 14);

         ya -=4;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         //ya -=4;
         al_draw_text(mFont.pr8, mColor.pc[15], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "'DEMO MODE' overlay opacity");

         ya +=16;
         float old_demo_mode_overlay_opacity = mDemoMode.demo_mode_overlay_opacity;
         if ((mInput.mouse_x > xa) && (mInput.mouse_x < xb) && (mInput.mouse_y > ya) && (mInput.mouse_y < ya + bts)) mScreen.draw_large_text_overlay(3, 15);
         mWidget.sliderfnb(xa, ya, xb, bts,  2,0,0,0,  0,12,15,15,  0,0,1,0, mDemoMode.demo_mode_overlay_opacity, 0.4, 0, .01, "");
         if (old_demo_mode_overlay_opacity != mDemoMode.demo_mode_overlay_opacity) mConfig.save();

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);
      }




// ---------------------------------------------------------------
//  5 - bottom message settings
// ---------------------------------------------------------------
      if (page == 5)
      {
         int line_spacing = 8;
         if (show_advanced) line_spacing = 7;
         //line_spacing +=  mLoop.pct_y;
         int xa = cfp_x1 + 10;
         int xb = cfp_x2 - 10;
         int ya = cfp_y1 + 10;
         int bts = 16;
         int tc = 13;
         int fc = 15;

         bts = 14;

         int sbs = 8; // space between sections

         // make a test list of types of bmsg
         if (!mBottomMessage.test_mode_list_created) mBottomMessage.create_test_mode_list();

         // redraw bottom frame to remove version that we will draw over
         int color = mPlayer.syn[mPlayer.active_local_player].color;
         for (int x = 0; x < BORDER_WIDTH; x++)
            al_draw_line(BORDER_WIDTH+10, (mDisplay.SCREEN_H-1-x)+0.5f, mDisplay.SCREEN_W-(BORDER_WIDTH+10), (mDisplay.SCREEN_H-1-x)+0.5f,  mColor.pc[color + (x * 16)], 1);


         mBottomMessage.bottom_msg_timer = 10;
         mBottomMessage.draw(1);

         int reload = 0;

         ya += line_spacing - 6;
         al_draw_text(mFont.pr8, mColor.pc[fc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "Bottom Message Settings");

         int bmsf_y1 = ya+8;
         ya = cfp_draw_line(cfp_x1+4, cfp_x2-4, ya+8, line_spacing, tc);

         ya -= 4;
         xb = cfp_x2 - 10;
         xa = xb - 180;

         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,8+96,15, 0,  1,0,0,0, "Reset All To Defaults"))
         {
            for (int i=0; i<100; i++)
               mBottomMessage.filter_event[i] = 1;

            mBottomMessage.num_lines = 16;
            mBottomMessage.io = 1.0;
            mBottomMessage.fo = 0.1;
            mBottomMessage.ihs = 0.5;
            mBottomMessage.ivs = 0.5;
            mBottomMessage.fhs = 0.1;
            mBottomMessage.fvs = 0.1;
            mBottomMessage.disp_player = 2;
            mBottomMessage.disp_enemy = 3;
            mBottomMessage.disp_item = 3;
            mBottomMessage.disp_health = 3;
            mBottomMessage.disp_player_text_long = 1;
            reload = 1;
         }

         xa = cfp_x1 + 10;
         xb = xa + 200;
         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, mBottomMessage.bottom_msg_on, "Show bottom messages", tc, 15);

         ya -= 6;
         al_draw_line(cfp_x1+4, bmsf_y1+line_spacing, cfp_x1+4, ya+line_spacing, mColor.pc[tc], 1 ); // draw the sides of the frame first
         al_draw_line(cfp_x2-4, bmsf_y1+line_spacing, cfp_x2-4, ya+line_spacing, mColor.pc[tc], 1 );
         ya = cfp_draw_line(cfp_x1+4, cfp_x2-4, ya, line_spacing, tc);

         // -------------------------
         // line config section
         // -------------------------




         // give some space and do our own line
         int lc = 10;
         int lcw = lc+96;
         ya-=line_spacing*2;
         ya+=sbs;

         int lcf_y1 = ya; // save this for later when we draw frame
         ya = cfp_draw_line(cfp_x1+4, cfp_x2-4, ya, line_spacing, lc);


         al_draw_text(mFont.pr8, mColor.pc[lc], cfp_txc, ya-2, ALLEGRO_ALIGN_CENTER, "Line Configuration");


         ya -=4;
         xb = cfp_x2-10;
         xa = xb - 80;

         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,lcw,15, 0,  1,0,0,0, "Defaults"))
         {
            mBottomMessage.num_lines = 16;
            mBottomMessage.io = 1.0;
            mBottomMessage.fo = 0.1;
            mBottomMessage.ihs = 0.5;
            mBottomMessage.ivs = 0.5;
            mBottomMessage.fhs = 0.1;
            mBottomMessage.fvs = 0.1;
            reload = 1;
         }


         ya = cfp_draw_line(cfp_x1+4, cfp_x2-4, ya+8, line_spacing, lc);


         ya-=4;

         xa = cfp_x1 + 10;
         xb = cfp_x2 - 10;
         float nl = mBottomMessage.num_lines;
         mWidget.sliderfnb(xa, ya, xb, bts,  0,0,0,0,  0,lcw,15,15, 0,0,1,0, nl, BMSG_MAX_LINES, 1, 1, "Number of lines:");
         mBottomMessage.num_lines = (int) nl;

         //mWidget.slideri(xa, ya, xb, bts,  0,0,0,0,  0,12,15,15, 0,0,1,0, mBottomMessage.num_lines, 20, 1, 1, "Number of lines:");



         ya = cfp_draw_line(cfp_x1+4, cfp_x2-4, ya-6, line_spacing, lc);
         ya -=4;


         xa = cfp_x1 + 10;
         xb = cfp_x2 - 10;
         int xw4 = (xb - xa)/2;

         xb = xa+xw4-10;
         mWidget.sliderfnb(xa, ya, xb, bts,  0,0,0,0,  0,lcw,15,15, 0,0,1,0, mBottomMessage.io, 1, 0, .05, "Initial Opacity:");
         mWidget.sliderfnb(xa, ya, xb, bts,  0,0,0,0,  0,lcw,15,15, 0,0,1,0, mBottomMessage.fo, 1, 0, .05, "Final Opacity:");

         ya-=bts*2;

         xa += xw4; xb = xa+xw4-10;
         mWidget.sliderfnb(xa, ya, xb, bts,  0,0,0,0,  0,lcw,15,15, 0,0,1,0, mBottomMessage.ivs, 1, 0, .05, "Initial Scale:");
         mWidget.sliderfnb(xa, ya, xb, bts,  0,0,0,0,  0,lcw,15,15, 0,0,1,0, mBottomMessage.fvs, 1, 0, .05, "Final Scale:");

         // force horizontal and vertical scales to be the same
         mBottomMessage.ihs = mBottomMessage.ivs;
         mBottomMessage.fhs = mBottomMessage.fvs;

         ya -= 6;
         al_draw_line(cfp_x1+4, lcf_y1+line_spacing, cfp_x1+4, ya+line_spacing, mColor.pc[lc], 1 ); // draw the sides of the frame first
         al_draw_line(cfp_x2-4, lcf_y1+line_spacing, cfp_x2-4, ya+line_spacing, mColor.pc[lc], 1 );
         ya = cfp_draw_line(cfp_x1+4, cfp_x2-4, ya, line_spacing, lc);


         // ------------------------------
         // text / icon config section
         // ------------------------------

         // give some space and do our own line
         int tic = 11;
         int ticw = tic+96;

         ya-=line_spacing*2;
         ya+=sbs;

         int ticf_y1 = ya; // save this for later when we draw frame
         ya = cfp_draw_line(cfp_x1+4, cfp_x2-4, ya, line_spacing, tic);

         al_draw_text(mFont.pr8, mColor.pc[tic], cfp_txc, ya-2, ALLEGRO_ALIGN_CENTER, "Text / Icon Configuration");

         ya -=4;

         xa = cfp_x1+10;
         xb = xa + 40;
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,ticw,15, 0,  1,0,0,0, "Min"))
         {
            mBottomMessage.disp_player = 0;
            mBottomMessage.disp_enemy = 0;
            mBottomMessage.disp_item = 0;
            mBottomMessage.disp_health = 3;
            mBottomMessage.disp_player_text_long = 0;
            reload = 1;
         }
         xb = cfp_x2-10;
         xa = xb - 40;
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,ticw,15, 0,  1,0,0,0, "Max"))
         {
            mBottomMessage.disp_player = 2;
            mBottomMessage.disp_enemy = 3;
            mBottomMessage.disp_item = 3;
            mBottomMessage.disp_health = 0;
            mBottomMessage.disp_player_text_long = 1;
            reload = 1;
         }


         ya = cfp_draw_line(cfp_x1+4, cfp_x2-4, ya+8, line_spacing, tic);


         ya-=4;
         int yto = (bts-11) / 2;
         xa = cfp_x1 + 20;
         xb = xa + 230;
         ALLEGRO_BITMAP *etmp = al_create_bitmap(200, 20);
         int xd = 0;

         // player display line
         if (mWidget.buttonp(xa, ya, xb, bts,  10,0,0,0,  0,ticw,15,15, 0,0,0,0, mBottomMessage.disp_player)) reload = 1;
         al_set_target_bitmap(etmp);
         al_clear_to_color(al_map_rgba(0, 0, 0, 0));
         xd = 0;
         mBottomMessage.draw_player(0, xd);
         al_set_target_backbuffer(mDisplay.display);
         al_draw_scaled_bitmap(etmp, 0, 0, 200, 20, xb+4, ya+yto, 100, 10, 0);
         ya+=bts;


         if (mBottomMessage.disp_player) // any mode that has text
         {
            if (mWidget.buttonp(xa, ya, xb, bts,  14,0,0,0,  0,ticw,15,15, 0,0,1,0, mBottomMessage.disp_player_text_long)) reload = 1;
         }

         // enemy display line
         if (mWidget.buttonp(xa, ya, xb, bts,  11,0,0,0,  0,ticw,15,15, 0,0,0,0, mBottomMessage.disp_enemy)) reload = 1;
         al_set_target_bitmap(etmp);
         al_clear_to_color(al_map_rgba(0, 0, 0, 0));
         xd = 0;
         mBottomMessage.draw_enemy(6, xd);
         al_set_target_backbuffer(mDisplay.display);
         al_draw_scaled_bitmap(etmp, 0, 0, 200, 20, xb+4, ya+yto, 100, 10, 0);
         ya+=bts;

         // item display line
         if (mWidget.buttonp(xa, ya, xb, bts,  12,0,0,0,  0,ticw,15,15, 0,0,0,0, mBottomMessage.disp_item)) reload = 1;
         al_set_target_bitmap(etmp);
         al_clear_to_color(al_map_rgba(0, 0, 0, 0));
         xd = 0;
         mBottomMessage.draw_item(mBitmap.tile[99], "switch", 15, xd, 0, 0);
         al_set_target_backbuffer(mDisplay.display);
         al_draw_scaled_bitmap(etmp, 0, 0, 200, 20, xb+4, ya+yto, 100, 10, 0);
         ya+=bts;



         // health display line
         if (mWidget.buttonp(xa, ya, xb, bts,  13,0,0,0,  0,ticw,15,15, 0,0,0,0, mBottomMessage.disp_health)) reload = 1;
         al_set_target_bitmap(etmp);
         al_clear_to_color(al_map_rgba(0, 0, 0, 0));
         xd = 0;
         mBottomMessage.draw_health(0, -23, xd, 0, 0);
         al_set_target_backbuffer(mDisplay.display);
         al_draw_scaled_bitmap(etmp, 0, 0, 200, 20, xb+4, ya+yto, 100, 10, 0);
         ya+=bts;

         al_destroy_bitmap(etmp);


         ya -= 6;
         al_draw_line(cfp_x1+4, ticf_y1+line_spacing, cfp_x1+4, ya+line_spacing, mColor.pc[tic], 1 ); // draw the sides of the frame first
         al_draw_line(cfp_x2-4, ticf_y1+line_spacing, cfp_x2-4, ya+line_spacing, mColor.pc[tic], 1 );
         ya = cfp_draw_line(cfp_x1+4, cfp_x2-4, ya, line_spacing, tic);


         // --------------------------------------
         // event selection section
         // --------------------------------------


         // give some space and do our own line
         int ec = 14;
         int ecw = ec+96;
         ya-=line_spacing*2;
         ya+=sbs;

         int esf_y1 = ya; // save this for later when we draw frame
         ya = cfp_draw_line(cfp_x1+4, cfp_x2-4, ya, line_spacing, ec);


         al_draw_text(mFont.pr8, mColor.pc[ec], cfp_txc, ya-2, ALLEGRO_ALIGN_CENTER, "Event Selection");

         ya -=4;

         xa = cfp_x1+10;
         xb = xa + 68;
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,ecw,15, 0,  1,0,0,0, "All Off"))
         {
            for (int i=0; i<100; i++)
               mBottomMessage.filter_event[i] = 0;
            reload = 1;
         }


         xb = cfp_x2-10;
         xa = xb - 60;
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,ecw,15, 0,  1,0,0,0, "All On"))
         {
            for (int i=0; i<100; i++)
               mBottomMessage.filter_event[i] = 1;
            reload = 1;
         }

         ya = cfp_draw_line(cfp_x1+4, cfp_x2-4, ya+8, line_spacing, ec);

         ya-=4;
         bts = 16;
         xa = cfp_x1 + 10;
         xw4 = (xb - xa)/5;

         xb = xa+xw4-10;
         if (mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0,0,0,0, 1,0,0,0, mBottomMessage.filter_event[20], "Key",    tc, 15)) reload = 1;
         xa += xw4; xb = xa+xw4-10;
         if (mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0,0,0,0, 1,0,0,0, mBottomMessage.filter_event[22], "Door",   tc, 15)) reload = 1;
         xa += xw4; xb = xa+xw4-10;
         if (mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0,0,0,0, 1,0,0,0, mBottomMessage.filter_event[23], "Exit",   tc, 15)) reload = 1;
         xa += xw4; xb = xa+xw4-10;
         if (mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0,0,0,0, 1,0,0,0, mBottomMessage.filter_event[25], "Bomb",   tc, 15)) reload = 1;
         xa += xw4; xb = xa+xw4-10;
         if (mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0,0,0,0, 1,0,0,0, mBottomMessage.filter_event[27], "Coin",   tc, 15)) reload = 1;
         xa += xw4; xb = xa+xw4-10;

         ya += line_spacing+8;
         xa = cfp_x1 + 10;
         xb = cfp_x2 - 10;
         xw4 = (xb - xa)/5;

         xb = xa+xw4-10;
         if (mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0,0,0,0, 1,0,0,0, mBottomMessage.filter_event[26], "Rocket", tc, 15)) reload = 1;
         xa += xw4; xb = xa+xw4-10;
         if (mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0,0,0,0, 1,0,0,0, mBottomMessage.filter_event[21], "Switch", tc, 15)) reload = 1;
         xa += xw4; xb = xa+xw4-10;
         if (mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0,0,0,0, 1,0,0,0, mBottomMessage.filter_event[12], "Damage", tc, 15)) reload = 1;
         xa += xw4; xb = xa+xw4-10;
         if (mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0,0,0,0, 1,0,0,0, mBottomMessage.filter_event[28], "Health Bonus",  tc, 15)) reload = 1;

         ya = cfp_draw_line(cfp_x1+4, cfp_x2-4, ya+10, line_spacing, ec);

         ya -= 4;
         xa = cfp_x1 + 10;
         xb = cfp_x2 - 10;
         xw4 = (xb - xa)/3;
         xb = xa+xw4-10;
         if (mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0,0,0,0, 1,0,0,0, mBottomMessage.filter_event[8], "Player Died", tc, 15)) reload = 1;
         xa += xw4; xb = xa+xw4-10;
         if (mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0,0,0,0, 1,0,0,0, mBottomMessage.filter_event[7], "Player Quit", tc, 15)) reload = 1;
         xa += xw4; xb = xa+xw4-10;
         if (mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0,0,0,0, 1,0,0,0, mBottomMessage.filter_event[6], "Player Joined", tc, 15)) reload = 1;

         ya = cfp_draw_line(cfp_x1+4, cfp_x2-4, ya+10, line_spacing, ec);


         ya -= 4;
         xa = cfp_x1 + 10;
         xb = cfp_x2 - 10;
         xw4 = (xb - xa)/5;
         xb = xa+xw4-10;
         if (mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0,0,0,0, 1,0,0,0, mBottomMessage.filter_event[41], "Player Hurt by Enemy", tc, 15)) reload = 1;

         xa += xw4*3; xb = xa+xw4-10;
         if (mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0,0,0,0, 1,0,0,0, mBottomMessage.filter_event[42], "Enemy Killed", tc, 15)) reload = 1;


         ya += line_spacing+8;
         xa = cfp_x1 + 10;
         xb = cfp_x2 - 10;
         xw4 = (xb - xa)/2;
         xb = xa+xw4-10;
         if (mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0,0,0,0, 1,0,0,0, mBottomMessage.filter_event[40], "Player Hurt by Player", tc, 15)) reload = 1;


         ya += 10;
         al_draw_line(cfp_x1+4, esf_y1+line_spacing, cfp_x1+4, ya+line_spacing, mColor.pc[ec], 1 ); // draw the sides of the frame first
         al_draw_line(cfp_x2-4, esf_y1+line_spacing, cfp_x2-4, ya+line_spacing, mColor.pc[ec], 1 );
         ya = cfp_draw_line(cfp_x1+4, cfp_x2-4, ya, line_spacing, ec);



         if (reload)  mBottomMessage.test_mode_list_created = 0; // force reload

      }


// ---------------------------------------------------------------
//  8 - viewport
// ---------------------------------------------------------------
      if (page == 8)
      {
         int line_spacing = 5;
         int tc = 15;  // text color
         int fc = 13;  // frame color
         int xa = cfp_x1 + 10;
         int xb = cfp_x2 - 10;
         int bts = 18; // button spacing
         int ya = cfp_y1;

         ya+=4+line_spacing;
         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "The viewport is the visible area of the level.");
         ya+=16;

         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "The player is usually near the center");
         ya+=12;
         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "and the level scrolls to follow the player.");

         ya +=8;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "The hysteresis rectangle is an area in the");
         ya+=12;
         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "center where the player can move without");
         ya+=12;
         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "causing the level to scroll");

         ya +=8;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         ya -= 2;

         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mDisplay.viewport_show_hyst, "Show Hysteresis Rectangle", tc, fc);

         xa = cfp_x1 + 10;
         xb = cfp_x2 - 10;
         int xw4 = (xb - xa)/2;
         xb = xa+xw4-10;
         mWidget.sliderf(xa, ya, xb, bts,  0,0,0,0,  0,12,fc,15,  0,0,0,0, mDisplay.viewport_x_div, 1, 0.01, .01, "X scale:");

         xa += xw4; xb = xa+xw4-10;
         mWidget.sliderf(xa, ya, xb, bts,  0,0,0,0,  0,12,fc,15,  0,0,1,0, mDisplay.viewport_y_div, 1, 0.01, .01, "Y scale:");

         xa = cfp_x1 + 10;
         xb = cfp_x2 - 10;

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "Show it to help with adjustment, and leave");
         ya+=12;
         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "it on while playing to see how it works.");

         ya +=8;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         mWidget.buttonp(xa+80, ya, xb-80, bts,  20,0,0,0,  0, 8, fc, 0,  1,0,1,0, mDisplay.viewport_mode);

         ya+=4;

         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "Instant mode moves the viewport instantly.");
         ya+=20;
         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "Gradual mode moves the viewport gradually.");
         ya+=12;
         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "It also moves the way you are facing.");
         ya+=12;
         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "Including up and down.");
         ya+=20;
         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "Static mode disables the hystersis rectangle.");
         ya+=12;
         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "The player is always in the center of the");
         ya+=12;
         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "screen. (Except near the level boundaries).");
         if (mDisplay.viewport_show_hyst) mScreen.draw_hyst_rect();

         ya +=8;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         if (mWidget.buttont(xa+90, ya, xb-90, bts,  0,0,0,0,  0,11,15, 0,  1,0,1,0, "Reset all to defaults"))
         {
            mDisplay.viewport_mode = 1;
            mDisplay.viewport_show_hyst = 0;
            mDisplay.viewport_x_div = 0.33;
            mDisplay.viewport_y_div = 0.33;
         }

      }


// ---------------------------------------------------------------
//  9 - overlay
// ---------------------------------------------------------------
      if (page == 9)
      {
         int line_spacing = 14;
         int xa = cfp_x1 + 10;
         int xb = cfp_x2 - 10;
         int ya = cfp_y1 + 10;
         int bts = 10;
         int tc = 13;
         int fc = 15;



         al_draw_text(mFont.pr8, mColor.pc[fc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "Debug Overlay Modes");
         ya +=4;
         int y3 = ya + line_spacing;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);
         ya -=7;

         al_draw_text(mFont.pr8, mColor.pc[tc], xb-130,     ya, 0, "Mode  0  1  2  3");
         cfp_4tog(xa, xb, ya, bts, tc, fc, line_spacing, 0, "CPU graph");
         cfp_4tog(xa, xb, ya, bts, tc, fc, line_spacing, 1, "display info");
         cfp_4tog(xa, xb, ya, bts, tc, fc, line_spacing, 2, "drawing profile timers");
         cfp_4tog(xa, xb, ya, bts, tc, fc, line_spacing, 3, "debug grid (client, server)");
         cfp_4tog(xa, xb, ya, bts, tc, fc, line_spacing, 4, "sync graph (client only)");
         cfp_4tog(xa, xb, ya, bts, tc, fc, line_spacing, 5, "sync adjust (client only");
         cfp_4tog(xa, xb, ya, bts, tc, fc, line_spacing, 6, "state freq adj (server only)");
         cfp_4tog(xa, xb, ya, bts, tc, fc, line_spacing, 7, "bandwidth stats (client, server)");
         cfp_4tog(xa, xb, ya, bts, tc, fc, line_spacing, 8, "miscellaneous");

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);


         // show which modes are inactive
         int x1 = xb-86;
         int x2 = x1 + number_of_debug_overlay_modes * 24 - 8;
         int x3 = xb+1;
         int y4 = ya - line_spacing;


         mMiscFnx.rectangle_with_diagonal_lines(x2+1, y3, x3, y4, 4, 10, 10+64, 1);
         al_draw_rectangle(x1, y3, x2, y4, mColor.pc[11], 1);


         bts = 20;
         mWidget.slideri(xb-140, ya, xb, bts,  0,0,0,0,  0,11,15,15, 0,0,0,0, number_of_debug_overlay_modes, 4, 2, 1, "Active modes:");
         if (mWidget.buttont(xa+20, ya, xa+200, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Reset to Defaults"))
         {
            for (int i=0; i<10; i++)
               for (int j=0; j<4; j++)
                  overlay_grid[i][j] = 0;
            overlay_grid[0][1] = 1;
            number_of_debug_overlay_modes = 2;
         }
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);
      }

// ---------------------------------------------------------------
//  10 - double
// ---------------------------------------------------------------
      if (page == 10)
      {
         int line_spacing = 10;
         //line_spacing +=  mLoop.pct_y;


         int tc = 15;  // text color
         int xa = cfp_x1 + 10;
         int xb = cfp_x2 - 10;
         int bts = 20; // button spacing
         int ya = cfp_y1;
         int e = 9;    // normal

         ya+=5+line_spacing;
         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "Display Transform Double is a method to");
         ya+=12;
         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "enlarge text. On high resolution displays");
         ya+=12;
         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "the text may become too small to read.");
         ya+=9;

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         ya+=2;


         if (mDisplay.saved_display_transform_double == 0) sprintf(msg, "Current Mode: %d [Auto]", mDisplay.display_transform_double);
         else                                       sprintf(msg, "Current Mode: %d [Static]", mDisplay.display_transform_double);
         mWidget.buttont(xa+60, ya, xb-60, bts,  0,0,0,0,  0,6,tc, 0,  1,0,1,0, msg);



         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);


         ya+=2;
         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "In auto mode, a reasonable value is set");
         ya+=12;
         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "automatically, depending on the current");
         ya+=12;
         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "screen resolution or window size.");
         ya+=12 + line_spacing;

         if (mWidget.buttont(xa+60, ya, xb-60, bts,  0,0,0,0,  0,fc,tc, 0,  1,0,1,0, "Set to Auto Mode")) mDisplay.set_saved_display_transform(0);

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         ya+=2;

         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "In static mode you can set a permanent");
         ya+=12;
         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "value as well as the maximum values when");
         ya+=12;
         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "cycling through with the function key.");
         ya+=12;
         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "(See Controls 2)");
         ya+=12;

         mWidget.slideri(xa+20, ya, xb-20, bts,  0,0,0,0,  0,7,15,15, 0,0,1,0, mDisplay.display_transform_double_max, 10, 2, 1, "Maximum Value:");

         ya +=4;

         xb = xa+180;

         static int newval = mDisplay.display_transform_double;
         mWidget.slideri(xa+20, ya, xb, bts,  0,0,0,0,  0,e,15,15, 0,0,0,0, newval, mDisplay.display_transform_double_max, 1, 1, "New Value:");

         xa = xa+200;
         xb = cfp_x2 - 30;
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,e,15, 0,  1,0,1,0, "Set New Value Now")) mDisplay.set_saved_display_transform(newval);

         xa = cfp_x1 + 10;
         xb = cfp_x2 - 10;

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);


         ya +=2;


         if (mWidget.buttont(xa+90, ya, xb-90, bts,  0,0,0,0,  0,11,15, 0,  1,0,1,0, "Reset all to defaults"))
         {
            mDisplay.display_transform_double_max = 3;
            mDisplay.set_saved_display_transform(0);
            newval = mDisplay.display_transform_double;
         }



      }




// ---------------------------------------------------------------
//  11 - speed
// ---------------------------------------------------------------
      if (page == 11)
      {
         int line_spacing = 8;
         //line_spacing +=  mLoop.pct_y;

         int tc = 15;  // text color
         int xa = cfp_x1 + 10;
         int xb = cfp_x2 - 10;
         int bts = 20; // button spacing
         int ya = cfp_y1+10;

         int uc = 10;    // unlocked color
         int lc = 11+96; // locked color

         int cc = uc; // current color
         if (mLoop.speed_control_lock) cc = lc;


         al_draw_text(mFont.pr8, mColor.pc[15], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "Game Speed Control"); ya+=4;

         ya = cfp_draw_line(cfp_x1+4, cfp_x2-4, ya, line_spacing, tc);

         //mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, mLoop.speed_control_lock, "Speed Control Lock", 15, 15);

         mWidget.toggle(cfp_x1+100, ya, cfp_x2-100, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, mLoop.speed_control_lock, "Speed Control Unlocked", "Speed Control Locked", 15, 15, uc, lc);
         ya-=2;


         ya = cfp_draw_line(cfp_x1+4, cfp_x2-4, ya, line_spacing, tc);


         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "When unlocked, the speed of the game");
         ya+=12;
         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "can be changed from the normal 40fps.");
         ya+=8;

         ya = cfp_draw_line(cfp_x1+4, cfp_x2-4, ya, line_spacing, tc);

         int adj = 0;
         if (mLoop.speed_control_lock)
         {
            adj = 1;
         }


         int old_frame_speed = mLoop.frame_speed;
         mWidget.slideri(xa, ya, xb, bts,  0,0,0,0,  0,cc,15,15, 0,0,1,adj, mLoop.frame_speed, 200, 4, 1, "Frame Speed:");
         if (old_frame_speed != mLoop.frame_speed) mEventQueue.set_speed();
         ya -=2;



         ya = cfp_draw_line(cfp_x1+4, cfp_x2-4, ya, line_spacing, tc);



         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "You can set the speed on this page");         ya+=12;
         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "or use the function keys (see Controls 2)");  ya+=12;
         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "when the game is running.");                  ya+=8;

         ya = cfp_draw_line(cfp_x1+4, cfp_x2-4, ya, line_spacing, tc);


         const char * dk = mInput.key_names[mInput.function_key_speed_dec];
         const char * ik = mInput.key_names[mInput.function_key_speed_inc];

         al_draw_textf(mFont.pr8, mColor.pc[tc], xa, ya, 0, "Speed Decrease Key: %s", dk); ya+=12;
         al_draw_textf(mFont.pr8, mColor.pc[tc], xa, ya, 0, "Speed Increase Key: %s", ik); ya+=8;

         ya = cfp_draw_line(cfp_x1+4, cfp_x2-4, ya, line_spacing, tc);

         al_draw_textf(mFont.pr8, mColor.pc[tc], xa, ya, 0, "              %s -1fps",    dk); ya+=8;
         al_draw_textf(mFont.pr8, mColor.pc[tc], xa, ya, 0, "       [SHFT] %s -10fps",   dk); ya+=8;
         al_draw_textf(mFont.pr8, mColor.pc[tc], xa, ya, 0, "[CTRL]        %s -100fps",  dk); ya+=8;
         al_draw_textf(mFont.pr8, mColor.pc[tc], xa, ya, 0, "[CTRL] [SHFT] %s -1000fps", dk); ya+=12;

         al_draw_textf(mFont.pr8, mColor.pc[tc], xa, ya, 0, "              %s +1fps",    ik); ya+=8;
         al_draw_textf(mFont.pr8, mColor.pc[tc], xa, ya, 0, "       [SHFT] %s +10fps",   ik); ya+=8;
         al_draw_textf(mFont.pr8, mColor.pc[tc], xa, ya, 0, "[CTRL]        %s +100fps",  ik); ya+=8;
         al_draw_textf(mFont.pr8, mColor.pc[tc], xa, ya, 0, "[CTRL] [SHFT] %s +1000fps", ik); ya+=8;

         ya = cfp_draw_line(cfp_x1+4, cfp_x2-4, ya, line_spacing, tc);

         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "Pressing both keys at the same"); ya+=12;
         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "time resets the speed to 40fps."); ya+=8;

         ya = cfp_draw_line(cfp_x1+4, cfp_x2-4, ya, line_spacing, tc);


      }


// ---------------------------------------------------------------
//  15 - info
// ---------------------------------------------------------------
      if (page == 15)
      {
         int line_spacing = 8;
         int tc = 15;  // text color
         //int fc = 13;  // frame color
         int xa = cfp_x1 + 10;
         int xb = cfp_x2 - 10;
         int bts = 20; // button spacing
         int ya = cfp_y1;

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);
         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "Sends info to the console");
         ya +=8;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Show vars")) mDisplay.show_var_sizes();
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "hostname"))                          printf("Local hostname:%s\n", mLoop.local_hostname);
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Show bitmap flags 'tilemap'"))       mDisplay.show_bitmap_flags(al_get_bitmap_flags(mBitmap.tilemap));
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Show bitmap flags 'M_tilemap'"))     mDisplay.show_bitmap_flags(al_get_bitmap_flags(mBitmap.M_tilemap));
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Show pixel format 'tilemap'"))       mDisplay.show_pixel_format(al_get_bitmap_format(mBitmap.tilemap));
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Show pixel format 'tilemap'"))       mDisplay.show_pixel_format(al_get_bitmap_format(mBitmap.M_tilemap));
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Show pixel format 'display'"))       mDisplay.show_pixel_format(al_get_display_format(mDisplay.display));
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Show display flags"))                mDisplay.show_display_flags(al_get_display_flags(mDisplay.display));
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Show display options"))              mDisplay.show_display_options();
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Show display orienation"))           mDisplay.show_display_orienation();
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Show refesh rate"))                  printf("refresh rate:%d\n", al_get_display_refresh_rate(mDisplay.display));
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Show fullscreen modes"))             mDisplay.show_fullscreen_modes();
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Show display adapters"))             mDisplay.show_display_adapters();
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Show code statistics"))              mwCodeStats::run();
      }
// ---------------------------------------------------------------
//  16 - profiling
// ---------------------------------------------------------------
      if (page == 16)
      {
         int line_spacing = 8;
         int xa = cfp_x1 + 10;
         int xb = cfp_x2 - 10;
         int ya = cfp_y1 + 10;
         int bts = 10;
         int tc = 13;
         int fc = 15;

         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mLog.LOG_TMR_cpu,              "LOG_TMR_cpu", tc, fc);
         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mLog.LOG_TMR_move_tot,         "LOG_TMR_move_tot", tc, fc);
         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mLog.LOG_TMR_move_all,         "LOG_TMR_move_all", tc, fc);
         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mLog.LOG_TMR_move_enem,        "LOG_TMR_move_enem", tc, fc);
         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mLog.LOG_TMR_draw_tot,         "LOG_TMR_draw_tot", tc, fc);
         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mLog.LOG_TMR_draw_all,         "LOG_TMR_draw_all", tc, fc);
         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mLog.LOG_TMR_sdif,             "LOG_TMR_sdif", tc, fc);
         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mLog.LOG_TMR_cdif,             "LOG_TMR_cdif", tc, fc);
         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mLog.LOG_TMR_rwnd,             "LOG_TMR_rwnd", tc, fc);
         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mLog.LOG_TMR_bmsg_add,         "LOG_TMR_bmsg_add", tc, fc);
         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mLog.LOG_TMR_bmsg_draw,        "LOG_TMR_bmsg_draw", tc, fc);
         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mLog.LOG_TMR_scrn_overlay,     "LOG_TMR_scrn_overlay", tc, fc);
         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mLog.LOG_TMR_client_timer_adj, "LOG_TMR_client_timer_adj", tc, fc);
         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mLog.LOG_TMR_client_ping,      "LOG_TMR_client_ping", tc, fc);

         ya+=10;
         bts = 14;
         xb = xa+60;
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,1,0,0, "All On")) set_all_logging(1);
         xa = xa+80;
         xb = xa+60;
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,1,1,0, "All Off")) set_all_logging(0);


         xa = cfp_x1 + 10;
         xb = cfp_x2 - 10;

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         bts = 10;
         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mLog.autosave_log_on_level_done,    "Autosave log on level done", tc, fc);
         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mLog.autosave_log_on_game_exit,     "Autosave log on game exit", tc, fc);
         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mLog.autosave_log_on_program_exit,  "Autosave log on program exit", tc, fc);

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         bts = 16;
         if (mWidget.buttont(xa+40, ya, xb-40, bts,  0,0,0,0,  0,10,15, 0,  1,0,1,0, "Open Most Recent Profile Graph")) mLog.run_profile_graph(0);

         ya += 8;

         if (mWidget.buttont(xa+40, ya, xb-40, bts,  0,0,0,0,  0,13,15, 0,  1,0,1,0, "Select and Open Profile Graph")) mLog.run_profile_graph(1);

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         if (mWidget.buttont(xa+80, ya, xb-80, bts,  0,0,0,0,  0,11,15, 0,  1,0,1,0, "Start Single Player Game"))
         {
            mLoop.new_program_state = 10;
            mLoop.old_program_state = 3;
            al_hide_mouse_cursor(mDisplay.display);
            mConfig.save();
            return;
         }

         ya += 8;
         xb = xa+180;
         if (mWidget.buttont(xa+20, ya, xb, bts,  0,0,0,0,  0,9,15, 0,  1,0,0,0, "Host Network Game"))
         {
            mLoop.new_program_state = 20;
            mLoop.old_program_state = 3;
            al_hide_mouse_cursor(mDisplay.display);
            mConfig.save();
            return;
         }
         xa = xa+200;
         xb = cfp_x2 - 30;
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,8,15, 0,  1,0,1,0, "Join Network Game"))
         {
            mLoop.new_program_state = 24;
            mLoop.old_program_state = 3;
            al_hide_mouse_cursor(mDisplay.display);
            mConfig.save();
            return;
         }
      }
// ---------------------------------------------------------------
//  17 - logging
// ---------------------------------------------------------------
      if (page == 17)
      {
         int line_spacing = 12;
         //line_spacing +=  mLoop.pct_y;
         int xa = cfp_x1 + 10;
         int xb = cfp_x2 - 10;
         int ya = cfp_y1 + 10;
         int bts = 10;
         int tc = 13;
         int fc = 15;
         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mLog.LOG_NET,                    "LOG_NET", tc, fc);
         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mLog.LOG_NET_join,               "LOG_NET_join", tc, fc);
         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mLog.LOG_NET_player_array,       "LOG_NET_player_array", tc, fc);
         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mLog.LOG_NET_bandwidth,          "LOG_NET_bandwidth", tc, fc);
         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mLog.LOG_NET_cdat,               "LOG_NET_cdat", tc, fc);
         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mLog.LOG_NET_stdf,               "LOG_NET_stdf", tc, fc);
         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mLog.LOG_NET_stdf_all_packets,   "LOG_NET_stdf_all_packets", tc, fc);
         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mLog.LOG_NET_dif_applied,        "LOG_NET_dif_applied", tc, fc);
         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mLog.LOG_NET_dif_not_applied,    "LOG_NET_dif_not_applied", tc, fc);
         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mLog.LOG_NET_client_ping,        "LOG_NET_client_ping", tc, fc);
         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mLog.LOG_NET_client_timer_adj,   "LOG_NET_client_timer_adj", tc, fc);
         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mLog.LOG_NET_server_rx_stak,     "LOG_NET_server_rx_stak", tc, fc);

         ya+=10;
         bts = 14;
         xb = xa+60;
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,1,0,0, "All On")) set_all_logging(1);
         xa = xa+80;
         xb = xa+60;
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,1,1,0, "All Off")) set_all_logging(0);


         xa = cfp_x1 + 10;
         xb = cfp_x2 - 10;

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         bts = 10;
         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mLog.autosave_log_on_level_done,    "Autosave log on level done", tc, fc);
         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mLog.autosave_log_on_game_exit,     "Autosave log on game exit", tc, fc);
         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mLog.autosave_log_on_program_exit,  "Autosave log on program exit", tc, fc);

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         bts = 16;
         if (mWidget.buttont(xa+20, ya, xb-20, bts,  0,0,0,0,  0,10,15, 0,  1,0,1,0, "Open Most Recent Log In Log File Viewer")) mLog.log_file_viewer(2);
         ya+=4;
         if (mWidget.buttont(xa+20, ya, xb-20, bts,  0,0,0,0,  0,13,15, 0,  1,0,1,0, "Select And Open Log In Log File Viewer")) mLog.log_file_viewer(1);

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         if (mWidget.buttont(xa+80, ya, xb-80, bts,  0,0,0,0,  0,11,15, 0,  1,0,1,0, "Start Single Player Game"))
         {
            mLoop.new_program_state = 10;
            mLoop.old_program_state = 3;
            al_hide_mouse_cursor(mDisplay.display);
            mConfig.save();
            return;
         }

         ya += 8;
         xb = xa+180;
         if (mWidget.buttont(xa+20, ya, xb, bts,  0,0,0,0,  0,9,15, 0,  1,0,0,0, "Host Network Game"))
         {
            mLoop.new_program_state = 20;
            mLoop.old_program_state = 3;
            al_hide_mouse_cursor(mDisplay.display);
            mConfig.save();
            return;
         }
         xa = xa+200;
         xb = cfp_x2 - 30;
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,8,15, 0,  1,0,1,0, "Join Network Game"))
         {
            mLoop.new_program_state = 24;
            mLoop.old_program_state = 3;
            al_hide_mouse_cursor(mDisplay.display);
            mConfig.save();
            return;
         }
      }

// ---------------------------------------------------------------
//  18 - debug
// ---------------------------------------------------------------
      if (page == 18)
      {
         int line_spacing = 12;
         //line_spacing +=  mLoop.pct_y;
         int xa = cfp_x1 + 10;
         int xb = cfp_x2 - 10;
         int ya = cfp_y1 + 10;
         int bts = 16;
         int tc = 13;


         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Spline adjust")) mLogo.spline_adjust();
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Spline test")) mLogo.spline_test();

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, mLoop.eco_draw, "Eco Draw", tc, 15);

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         int old_mln = mVisualLevel.max_level_num;
         mWidget.slideri(xa, ya, xb, bts,  0,0,0,0,  0,8,15,15, 0,0,1,0, mVisualLevel.max_level_num, 400, 4, 1, "Max Level Number for Visual Level Select:");
         if (old_mln != mVisualLevel.max_level_num) mVisualLevel.load_visual_level_select_done = 0;
         ya -=2;

         ya = cfp_draw_line(cfp_x1+4, cfp_x2-4, ya, line_spacing, tc);

      }







      if (mInput.key[ALLEGRO_KEY_ESCAPE][0])
      {
         while (mInput.key[ALLEGRO_KEY_ESCAPE][0]) mEventQueue.proc();
         quit = 1;
      }

      if (mInput.mouse_b[2][0])
      {
         while (mInput.mouse_b[2][0]) mEventQueue.proc();
         quit = 1;
      }




   }
   al_hide_mouse_cursor(mDisplay.display);
   mConfig.save();
   mLoop.new_program_state = 1;
   mLoop.old_program_state = 1;
}






