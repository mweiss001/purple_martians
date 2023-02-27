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


int mwSettings::redraw_all_controls(int x, int y, int bts, int tc, int show_buttons, int num)
{
   int xa = x;
   int xb = xa + 32;  // 'new' button end
   int tx = xb + 10;  // text pos padded after 'new' button
   int xc = xa + 240; // end of entire area, used for clearing and framing

   int ya = y;
   float yo = (bts-8)/2; // text y offset so it is centered with the button

   al_draw_filled_rectangle(x-1, y-1, xc, ya + bts*7, mColor.pc[13+224]); // erase background
   al_draw_rectangle       (x-1, y-1, xc, ya + bts*7, mColor.pc[15], 0);  // frame

   if (num == 0) al_draw_textf(mFont.pr8, mColor.pc[10], tx, ya+yo, 0, "Up ---- set new control");
   else          al_draw_textf(mFont.pr8, mColor.pc[tc], tx, ya+yo, 0, "Up ---- %s", mInput.key_names[mPlayer.loc[0].up_key]);
   if ((show_buttons) && (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,1,0,0, "new")))    mPlayer.loc[0].up_key = mInput.my_readkey(x, y, tc, bts, 0);
   ya+=bts;

   if (num == 1) al_draw_textf(mFont.pr8, mColor.pc[10], tx, ya+yo, 0, "Down -- set new control");
   else          al_draw_textf(mFont.pr8, mColor.pc[tc], tx, ya+yo, 0, "Down -- %s", mInput.key_names[mPlayer.loc[0].down_key]);
   if ((show_buttons) && (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,1,0,0, "new")))    mPlayer.loc[0].down_key = mInput.my_readkey(x, y, tc, bts, 1);
   ya+=bts;

   if (num == 2) al_draw_textf(mFont.pr8, mColor.pc[10], tx, ya+yo, 0, "Left -- set new control");
   else          al_draw_textf(mFont.pr8, mColor.pc[tc], tx, ya+yo, 0, "Left -- %s", mInput.key_names[mPlayer.loc[0].left_key]);
   if ((show_buttons) && (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,1,0,0, "new")))    mPlayer.loc[0].left_key = mInput.my_readkey(x, y, tc, bts, 2);
   ya+=bts;

   if (num == 3) al_draw_textf(mFont.pr8, mColor.pc[10], tx, ya+yo, 0, "Right - set new control");
   else          al_draw_textf(mFont.pr8, mColor.pc[tc], tx, ya+yo, 0, "Right - %s", mInput.key_names[mPlayer.loc[0].right_key]);
   if ((show_buttons) && (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,1,0,0, "new")))    mPlayer.loc[0].right_key = mInput.my_readkey(x, y, tc, bts, 3);
   ya+=bts;

   if (num == 4) al_draw_textf(mFont.pr8, mColor.pc[10], tx, ya+yo, 0, "Jump -- set new control");
   else          al_draw_textf(mFont.pr8, mColor.pc[tc], tx, ya+yo, 0, "Jump -- %s", mInput.key_names[mPlayer.loc[0].jump_key]);
   if ((show_buttons) && (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,1,0,0, "new")))    mPlayer.loc[0].jump_key = mInput.my_readkey(x, y, tc, bts, 4);
   ya+=bts;

   if (num == 5) al_draw_textf(mFont.pr8, mColor.pc[10], tx, ya+yo, 0, "Fire -- set new control");
   else          al_draw_textf(mFont.pr8, mColor.pc[tc], tx, ya+yo, 0, "Fire -- %s", mInput.key_names[mPlayer.loc[0].fire_key]);
   if ((show_buttons) && (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,1,0,0, "new")))    mPlayer.loc[0].fire_key = mInput.my_readkey(x, y, tc, bts, 5);
   ya+=bts;

   if (num == 6) al_draw_textf(mFont.pr8, mColor.pc[10], tx, ya+yo, 0, "Menu -- set new control");
   else          al_draw_textf(mFont.pr8, mColor.pc[tc], tx, ya+yo, 0, "Menu -- %s", mInput.key_names[mPlayer.loc[0].menu_key]);
   if ((show_buttons) && (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,1,0,0, "new")))    mPlayer.loc[0].menu_key = mInput.my_readkey(x, y, tc, bts, 6);
   ya+=bts;

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
   if (set_page != -1)  settings_current_page = set_page;
   int page = settings_current_page;

   struct settings_tab st[20] = {0};

   sprintf(st[0].title,  "Basics");
   sprintf(st[1].title,  "Controls");
   sprintf(st[2].title,  "Netgame");
   sprintf(st[3].title,  "Double");
   sprintf(st[4].title,  "Demo");
   sprintf(st[5].title,  "Advanced");
   sprintf(st[6].title,  "Info");
   sprintf(st[7].title,  "Viewport");
   sprintf(st[8].title,  "Profiling");
   sprintf(st[9].title,  "Logging");
   sprintf(st[10].title, "Overlay");
   sprintf(st[11].title, "Test 6");
   sprintf(st[12].title, "Test 7");
   sprintf(st[13].title, "Test 8");
   sprintf(st[14].title, "Test 9");
   int num_pages = 11;

   char title[80] = {0};
   sprintf(title, "Settings");

   int fc = 10; // frame color
   int tc = 15; // text color
   int frame_width = 4; // border width

   int quit = 0;
   while (!quit)
   {
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
      st[0].x1 = cfp_x1+1; // start at page x1
      st[0].y2 = 0;        // we don't know where are going to start yet, so do them at offset 0
      for (int i=0; i<num_pages; i++)
      {
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
         st[i+1].x1 = st[i].x2 + 1; // x1 of next tab is x2 of this tab plus pad
         st[i+1].y2 = st[i].y2;     // y2 of next tab is the same y2 as this tab
      }
      int tabs_height = 8 - st[num_pages-1].y1; // find the height of the last tab

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

      // draw and process tabs
      int mouse_on_tab = -1;
      for (int i=0; i<num_pages; i++)
      {
         st[i].y1 += cfp_y1; // adjust y values to top of page
         st[i].y2 += cfp_y1;
         if ((mInput.mouse_x > st[i].x1) && (mInput.mouse_x < st[i].x2) && (mInput.mouse_y > st[i].y1) && (mInput.mouse_y < st[i].y2)) mouse_on_tab = i;
         draw_tab(st, i, fc+128, tc+128); // draw the tab
      }

      draw_tab(st, page, fc, tc); // draw the current tab on top
      if (mouse_on_tab != -1)
      {
         draw_tab(st, mouse_on_tab, fc, tc); // draw the tab the mouse is on
         if (mInput.mouse_b[1][0])
         {
            while (mInput.mouse_b[1][0]) mEventQueue.proc();
            settings_current_page = page = mouse_on_tab;
         }
      }

      // frame page
      for (int a=0; a<frame_width; a++)
         al_draw_rounded_rectangle(cf_x1+a, cfp_y1+a, cf_x2-a, cf_y2-a, 4, 4, mColor.pc[fc+a*48], 1.5);








// ---------------------------------------------------------------
//  0 - basics
// ---------------------------------------------------------------
      if (page == 0)
      {
         int line_spacing = 10;
         int tc = 15;  // text color
         int fc = 13;  // frame color
         int xa = cfp_x1 + 10;
         int xb = cfp_x2 - 10;
         int bts = 16; // button spacing
         int ya = cfp_y1;

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);


         // ---------------------------------------
         // player color
         // ---------------------------------------

         ya+=7;
         al_draw_text(mFont.pr8, mColor.pc[tc], xa,     ya, 0, "Current player color:");
         cfp_draw_player(mPlayer.syn[0].color,     xa+180, ya-7);

         ya+=13;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

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

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);



         // ---------------------------------------
         // sound
         // ---------------------------------------
         int x1a = cfp_x1 + 10;
         int x1b = x1a + 60;
         int old_sound_on = mSound.sound_on;
         mWidget.togglec(x1a, ya, x1b, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,0,0, mSound.sound_on, "Sound", tc, tc);
         if ((old_sound_on == 0) && (mSound.sound_on == 1)) mSound.load_sound();

         int dim = 0;
         if (!mSound.sound_on) dim = 128;

         int old_se_scaler = mSound.se_scaler;
         x1a = x1b + 12;
         x1b = x1a + 140;
         mWidget.slideri(x1a, ya, x1b, bts,  0,0,0,0,  0,fc+dim,tc+dim,0,  0,0,0,0, mSound.se_scaler, 9, 0, 1, "Sound Effects:");

         if (old_se_scaler != mSound.se_scaler) mSound.set_se_scaler();


         //if ((old_se_scaler != se_scaler) && (sound_on)) al_set_mixer_gain(se_mixer, (float)se_scaler / 9);

         int old_st_scaler = mSound.st_scaler;
         x1a = x1b + 12;
         x1b = x1a + 140;
         mWidget.slideri(x1a, ya, x1b, bts,  0,0,0,0,  0,fc+dim,tc+dim,0,  0,0,1,0, mSound.st_scaler, 9, 0, 1, "Sound Track:");
         if (old_st_scaler != mSound.st_scaler) mSound.set_st_scaler();
//         if ((old_st_scaler != st_scaler) && (sound_on)) al_set_mixer_gain(st_mixer, (float)st_scaler / 9);

         ya -=2;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);












         // ---------------------------------------
         // splash screen
         // ---------------------------------------
         x1a = cfp_x1 + 10;
         x1b = x1a + 250;
         mWidget.togglec(x1a, ya, x1b, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,0,0, mLogo.show_splash_screen, "Show splash screen on startup", tc, tc);
         x1a = x1b + 30;
         x1b = x1a + 74;
         if (mWidget.buttont(x1a, ya, x1b, bts,  0,0,0,0,  0,fc,tc, 0,  1,0,1,0, "Show now"))
         {
             mLogo.splash_screen();
             while (mInput.key[ALLEGRO_KEY_ESCAPE][0]) mEventQueue.proc();
         }
         ya -=2;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, mBottomMessage.bottom_msg_on, "Show bottom message display", tc, 15);

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
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Get all new controls"))  mInput.get_all_keys(kx, ky, tc, 14);
         ya += 4;
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Set all to joystick 1")) mInput.set_controls_to_custom_sets(1);
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Set all to joystick 2")) mInput.set_controls_to_custom_sets(2);
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Set all to arrow keys")) mInput.set_controls_to_custom_sets(3);
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Set all to IJKL"))       mInput.set_controls_to_custom_sets(4);
         ya += 4;
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,14,15, 0,  1,0,1,0, "Test controls"))         mInput.test_keys(cfp_x1 + (cfp_x2-cfp_x1) / 2, ya-bts*7);
      }
// ---------------------------------------------------------------
//  2 - netgame
// ---------------------------------------------------------------
      if (page == 2)
      {
         int line_spacing = 10;
         int tc = 13;
         int fc = 15;
         int xa = cfp_x1 + 10;
         int xb = cfp_x2 - 10;
         int ya = cfp_y1;
         int bts = 16;

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, fc);


         sprintf(msg, "Server IP Address:%s", mNetgame.m_serveraddress);
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,-1,fc, 0,  1,0,1,0, msg))  mMiscFnx.edit_server_name(1, cfp_txc, ya);
         ya+=4;
         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "Clients need the server name");
         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya+8, ALLEGRO_ALIGN_CENTER, "or IP address set here.");

         ya+=18;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, fc);

         mWidget.toggle( xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mNetgame.TCP, "Packet type:UDP", "Packet type:TCP", fc, fc, -1, -1);
         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "The type of packet used to communicate.");

         ya+=10;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, fc);

         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mShot.deathmatch_shots, "Deathmatch player shots", fc, fc);
         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "Do player's shots affect other players?");

         ya+=10;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, fc);

         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mShot.suicide_shots, "Suicide player shots", fc, fc);
         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "Do player's shots affect themselves?");

         ya+=10;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, fc);
         ya+=4;

         mWidget.slideri(xa, ya, xb, bts,  0,0,0,0,  0,12,fc,fc,  0,0,1,0, mShot.deathmatch_shot_damage, 100, -10, 1, "Player shot damage:");
         ya+=4;
         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "The amount of damage player's shots");
         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya+8, ALLEGRO_ALIGN_CENTER, "do to other players and themselves.");

         ya+=18;
         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, fc);

      }

// ---------------------------------------------------------------
//  3 - double
// ---------------------------------------------------------------
      if (page == 3)
      {
         int line_spacing = 11;
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
         al_draw_text(mFont.pr8, mColor.pc[tc], cfp_txc, ya, ALLEGRO_ALIGN_CENTER, "cycling through with CTRL-SHIFT-F12.");
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

         static int test_opacity = 0;

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         if (mWidget.buttont(xa+60, ya, xb-60, bts,  0,0,0,0,  0,14,tc, 0,  1,0,1,0, "Save current game in progress")) mGameMoves.save_gm();
         ya -=2;

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mLog.autosave_game_on_level_done, "Autosave on level done", tc, 14);
         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mLog.autosave_game_on_game_exit,  "Autosave on game exit", tc, 14);
         ya -=2;

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         if (mWidget.buttont(xa+60, ya, xb-60, bts,  0,0,0,0,  0,fc,tc, 0,  1,0,1,0, "Choose file and run saved game"))
         if (mGameMoves.load_gm("-"))
         {
            mLoop.new_program_state = 14;
            mLoop.old_program_state = 3;
            al_hide_mouse_cursor(mDisplay.display);
            mConfig.save();
            return;
         }
         ya +=10;
         if (mWidget.buttont(xa+90, ya, xb-90, bts,  0,0,0,0,  0,fc,tc, 0,  1,0,1,0, "Play random demo game"))
         {
            mLoop.new_program_state = 2;
            mLoop.older_program_state = 3;
            al_hide_mouse_cursor(mDisplay.display);
            mConfig.save();
            return;
         }
         ya -=2;

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mDemoMode.demo_mode_config_enable, "Autoplay random demo at program start", tc, fc);
         ya -=2;

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);


         float old_demo_mode_overlay_opacity = mDemoMode.demo_mode_overlay_opacity;
         mWidget.sliderfnb(xa, ya, xb, bts,  2,0,0,0,  0,12,fc,fc,  0,0,1,0, mDemoMode.demo_mode_overlay_opacity, 0.4, 0, .01, "Demo mode overlay opacity:");
         if (old_demo_mode_overlay_opacity != mDemoMode.demo_mode_overlay_opacity) mConfig.save();

         if (test_opacity) mScreen.draw_large_text_overlay(3, 15);

         mWidget.togglec(xa, ya, xa+20, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, test_opacity, "Test demo mode overlay opacity", tc, fc);
         ya +=8;



      }
// ---------------------------------------------------------------
//  5  - advanced
// ---------------------------------------------------------------
      if (page == 5)
      {
         int line_spacing = 14;
         int tc = 15;  // text color
         int xa = cfp_x1 + 10;
         int xb = cfp_x2 - 10;
         int bts = 20; // button spacing
         int ya = cfp_y1;

         int e = 11;    // normal
         int d = 10+96; // dim

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);

         ya -=12;

         int old_frame_speed = mLoop.frame_speed;

         if (mLoop.speed_control_lock)
         {
            mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, mLoop.speed_control_lock, "Speed Control Lock", tc, 10);
            mWidget.slideri(xa, ya, xb, bts,  0,0,0,0,  0,d,d,d, 0,0,1,1, mLoop.frame_speed, 200, 4, 1, "Frame Speed:");
         }
         else
         {
            mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, mLoop.speed_control_lock, "Speed Control Lock", tc, 11);
            mWidget.slideri(xa, ya, xb, bts,  0,0,0,0,  0,e,e,e, 0,0,1,0, mLoop.frame_speed, 200, 4, 1, "Frame Speed:");
         }

         if (old_frame_speed != mLoop.frame_speed) mEventQueue.set_speed();
         ya -=6;

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);


         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Spline adjust")) mLogo.spline_adjust();
         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,0, "Spline test")) mLogo.spline_test();

         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);


         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, mPlayer.loc[0].server_state_freq_mode, "State Frequency Auto Adjust", tc, 15);









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

//         ya -=2;
//         ya = cfp_draw_line(xa-6, xb+6, ya, line_spacing, tc);
      }

// ---------------------------------------------------------------
//  7 - viewport
// ---------------------------------------------------------------
      if (page == 7)
      {
         int line_spacing = 5;
         int tc = 15;  // text color
         int fc = 13;  // frame color
         int xa = cfp_x1 + 10;
         int xb = cfp_x2 - 10;
         int bts = 20; // button spacing
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

         ya +=1;
         mWidget.sliderf(xa, ya, xb, bts,  0,0,0,0,  0,12,fc,fc,  0,0,1,0, mDisplay.viewport_x_div, 1, 0.01, .01, "Hysteresis scale x:");
         mWidget.sliderf(xa, ya, xb, bts,  0,0,0,0,  0,12,fc,fc,  0,0,1,0, mDisplay.viewport_y_div, 1, 0.01, .01, "Hysteresis scale y:");

         mWidget.togglec(xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, mDisplay.viewport_show_hyst, "Show Hysteresis Rectangle", tc, fc);

         ya-=6;

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
//  8 - profiling
// ---------------------------------------------------------------
      if (page == 8)
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
//  9 - logging
// ---------------------------------------------------------------
      if (page == 9)
      {
         int line_spacing = 10;
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
//  10 - overlay
// ---------------------------------------------------------------
      if (page == 10)
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








      if (mInput.key[ALLEGRO_KEY_ESCAPE][0])
      {
         while (mInput.key[ALLEGRO_KEY_ESCAPE][0]) mEventQueue.proc();
         quit = 1;
      }
   }
   al_hide_mouse_cursor(mDisplay.display);
   mConfig.save();
   mLoop.new_program_state = 1;
   mLoop.old_program_state = 1;
}







