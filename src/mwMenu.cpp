// mwMenu.cpp

#include "pm.h"
#include "mwMenu.h"
#include "mwWindowManager.h"
#include "mwPlayer.h"
#include "mwLogo.h"
#include "mwDemoMode.h"
#include "mwDisplay.h"
#include "mwFont.h"
#include "mwColor.h"
#include "mwInput.h"
#include "mwEventQueue.h"
#include "mwLevel.h"
#include "mwScreen.h"
#include "mwMain.h"

#include "mwGameMoves.h"
#include "mwLoop.h"


mwMenu mMenu;

void mwMenu::init_zmenu(int menu_num)
{
   if (menu_num == 1) // classic mode main menu
   {
      sprintf(menu_string[0], "Start Level (%d)", mLevel.start_level);
      strcpy (menu_string[1], "Start New Game");
      strcpy (menu_string[2], "Resume Current Game");
      strcpy (menu_string[3], "Host Network Game");
      strcpy (menu_string[4], "Join Network Game");
      strcpy (menu_string[5], "Settings");
      strcpy (menu_string[6], "Level Editor");
      strcpy (menu_string[7], "Demo Mode");
      strcpy (menu_string[8], "Help Screens");
      strcpy (menu_string[9], "end");
   }
   if (menu_num == 2)  // story mode main menu
   {
      strcpy (menu_string[0], "Single Player Game");
      strcpy (menu_string[1], "Host Network Game");
      strcpy (menu_string[2], "Join Network Game");
      strcpy (menu_string[3], "Settings");
      strcpy (menu_string[4], "Demo Mode");
      strcpy (menu_string[5], "Help Screens");
      strcpy (menu_string[6], "end");
   }
}

void mwMenu::init_pmenu(int type)
{
   if (type == 2)
   {
      strcpy (menu_string[0], "Level Editor Pop-Up Menu");
      strcpy (menu_string[1], "------------------------");

      strcpy (menu_string[5], "------------------------");
      strcpy (menu_string[6], "Edit Selection");
      strcpy (menu_string[7], "Group Edit");
      strcpy (menu_string[8], "Tile Helper");
      strcpy (menu_string[9], "Show Status Window");
      strcpy (menu_string[10], "Show Selection Window");
      strcpy (menu_string[11], "------");
      strcpy (menu_string[12],"New Level");
      strcpy (menu_string[13],"Load Level");
      strcpy (menu_string[14],"Save Level");
      strcpy (menu_string[15],"Save and Quit");
      strcpy (menu_string[16],"Help Screens");
      strcpy (menu_string[17],"Quit Level Editor");
      strcpy (menu_string[18],"end");
   }
}


void mwMenu::get_zmenu_width_and_height(int menu_num, int& width, int &height)
{
   init_zmenu(menu_num);

   width = 0;
   height = 0;

   // iterate the menu items
   int c = 0;
   while (strcmp(menu_string[c],"end") != 0)
   {
      int sl = strlen(menu_string[c]) * 8;
      if (sl > width) width = sl;
      height +=10;
      c++;
   }
}


int mwMenu::zmenu(int menu_num, int menu_pos)
/*
   used only for the main game menu
   blocks until a menu item is selected
   navigate with arrow keys, ENTER and ESC
   can also navigate with game controls

   now also for more menus


*/

{

   init_zmenu(menu_num);

   int y = mScreen.menu_y;
   int mx = mScreen.menu_x;

   mDemoMode.countdown_val = mDemoMode.countdown_reset;

   int demo_mode_menu_item_num = 0; // off by default
   if (menu_num == 1) demo_mode_menu_item_num = 7;
   if (menu_num == 2) demo_mode_menu_item_num = 4;


   int highlight = menu_pos;

   //printf("highlight:%d\n", highlight);


   int selection = 999;
   int last_list_item;

   int up_held = 0;
   int down_held = 0;
   int left_held = 0;
   int right_held = 0;

   mInput.initialize();

   while (selection == 999)
   {



      al_set_target_backbuffer(mDisplay.display);
      al_flip_display();
      al_clear_to_color(al_map_rgb(0, 0, 0));

      while (!mEventQueue.menu_update) mEventQueue.proc_menu();
      mEventQueue.menu_update = 0;


      mx = mScreen.menu_x; // re-load every frame in case screen size changes

      mScreen.draw_line_of_players();


      mLogo.mdw_an(mScreen.menu_logo_x, mScreen.menu_logo_y, mScreen.menu_logo_scale);

      mScreen.draw_level_map_under_menu();

      if (mMain.classic_mode) mScreen.draw_level_info();

      mScreen.frame_and_title();



      if (demo_mode_menu_item_num)
      {
         if (mLevel.resume_allowed) mDemoMode.autoplay_enabled = 0;
         if (mDemoMode.autoplay_enabled)
         {
            if (--mDemoMode.countdown_val < 0)
            {
               mDemoMode.countdown_val = mDemoMode.countdown_reset;
               mDemoMode.autoplay_enabled = 0;
               return demo_mode_menu_item_num;
            }
            sprintf(menu_string[demo_mode_menu_item_num], "Demo Mode (%d)", mDemoMode.countdown_val / 100);
         }
         else sprintf(menu_string[demo_mode_menu_item_num], "Demo Mode");
      }


//      float hx1 = mx - mScreen.menu_w/2;
//      float hx2 = mx + mScreen.menu_w/2;
//      float hy1 = y;
//      float hy2 = y + mScreen.menu_h;
//      al_draw_rectangle(hx1-1, hy1, hx2+1, hy2, mColor.pc[10], 0);





      // draw the menu items
      int c = 0;
      while (strcmp(menu_string[c],"end") != 0)
      {
         int sl = strlen(menu_string[c]) * 4;
         int b = 15; // b = mPlayer.syn[mPlayer.active_local_player].color;

         if ((menu_num == 1) && (!mLevel.resume_allowed) && (c==2)) b+=80; // dimmer if can't resume

         // is mouse on menu item
         float mix1 = mx-sl-2;
         float miy1 = y+(c*10)-1;
         float mix2 = mx+sl;
         float miy2 = y+(c*10)+9;

         if ((mInput.mouse_x > mix1) && (mInput.mouse_x < mix2) && (mInput.mouse_y > miy1) && (mInput.mouse_y < miy2) && (!al_get_timer_count(mEventQueue.mou_timer)))
         {
            highlight = c;
            if (mInput.mouse_b[1][0])
            {
               while (mInput.mouse_b[1][0]) mEventQueue.proc();
               selection = highlight;
            }
         }




         if (c == highlight) al_draw_rectangle(mix1+0.5f, miy1+0.5f, mix2+0.5f, miy2+0.5f, mColor.pc[b+80], 1);
         al_draw_text(mFont.pr8, mColor.pc[b], mx, y+(c*10)+1, ALLEGRO_ALIGN_CENTRE, menu_string[c]);
         c++;
      }
      last_list_item = c-1;

//      if (highlight > last_list_item) highlight = last_list_item;

      if (((mInput.key[ALLEGRO_KEY_RIGHT][0]) || (mPlayer.syn[0].right)) && (right_held == 0))
      {
         right_held = 1;
         selection = highlight + 100;
      }
      if ( (!(mInput.key[ALLEGRO_KEY_RIGHT][0])) &&  (!(mPlayer.syn[0].right)) )  right_held = 0;
      if (((mInput.key[ALLEGRO_KEY_LEFT][0]) || (mPlayer.syn[0].left)) && (left_held == 0))
      {
         left_held = 1;
         selection = highlight + 200;
      }
      if ( (!(mInput.key[ALLEGRO_KEY_LEFT][0])) &&  (!(mPlayer.syn[0].left)) )  left_held = 0;


      if (((mInput.key[ALLEGRO_KEY_DOWN][0]) || (mPlayer.syn[0].down))  && (down_held == 0))
      {
         if (++highlight > last_list_item) highlight = last_list_item;

         if ((menu_num == 1) && (!mLevel.resume_allowed) && (highlight == 2)) highlight = 3; // skip



         down_held = 1;
         mDemoMode.countdown_val = mDemoMode.countdown_reset;
         mDemoMode.autoplay_enabled = 0;
      }
      if ( (!(mInput.key[ALLEGRO_KEY_DOWN][0])) && (!(mPlayer.syn[0].down))) down_held = 0;

      if (((mInput.key[ALLEGRO_KEY_UP][0]) || (mPlayer.syn[0].up)) && (up_held == 0))
      {
         if (--highlight < 0) highlight = 0;

         if ((menu_num == 1) && (!mLevel.resume_allowed) && (highlight == 2)) highlight = 1; // skip

         up_held = 1;
         mDemoMode.countdown_val = mDemoMode.countdown_reset;
         mDemoMode.autoplay_enabled = 0;
      }
      if ( (!(mInput.key[ALLEGRO_KEY_UP][0])) && (!(mPlayer.syn[0].up))) up_held = 0;

      if (menu_num == 1)
      {
         // shortcut keys
         if (mInput.key[ALLEGRO_KEY_L][0])                                    return 6; // level editor
         if (mInput.key[ALLEGRO_KEY_O][0] && mInput.SHFT() && mInput.CTRL() ) return 5; // settings
      }
      if (menu_num == 2)
      {
         // shortcut keys
         if (mInput.key[ALLEGRO_KEY_O][0] && mInput.SHFT() && mInput.CTRL() ) return 3; // settings
      }
      if (mInput.key[ALLEGRO_KEY_PGDN][0])   highlight = last_list_item;
      if (mInput.key[ALLEGRO_KEY_PGUP][0])   highlight = 0;
      if (mInput.key[ALLEGRO_KEY_ENTER][0])  selection = highlight;
      if (mInput.key[ALLEGRO_KEY_ESCAPE][0]) selection = -1;


      if (mLevel.skc_trigger_demo) selection = 666;




   }
   return selection;
}








int mwMenu::tmenu(int menu_pos, int x1, int y1)
/*
   used in the level editor as the main menu

*/

{
   int pc = mPlayer.syn[mPlayer.active_local_player].color;

   int highlight = menu_pos;
   int selection = 999;
   int c=0, last_list_item = 0;
   int max_strlen = 0;

   // find max string length
   while (strcmp(menu_string[c],"end") != 0)
   {
      int len = strlen(menu_string[c]);
      if (len > max_strlen) max_strlen = len;
      c++;
   }
   last_list_item = c;


   int yh = 12; // y height

   int x2 = x1+max_strlen*8+2;
   int y2 = y1+last_list_item*yh-1;

   while (selection == 999)
   {
      mWM.redraw_level_editor_background();
      mWM.cycle_windows(1);

      // draw menu title
      int mt = strlen(menu_string[0])*8;
      al_draw_filled_rectangle(x1-2, y1-2, x1+mt+2, y1+yh, mColor.pc[pc+128]);
      al_draw_rectangle(       x1-2, y1-2, x1+mt+2, y1+yh, mColor.pc[15], 1);
      al_draw_text(mFont.pr8, mColor.pc[15], x1, y1+1, 0, menu_string[0]);

      // erase menu background
      al_draw_filled_rectangle(x1-2, y1+12, x2, y2, mColor.pc[pc+128+48]);
      al_draw_rectangle(x1-2, y1+12, x2, y2, mColor.pc[15], 1);

      // draw the menu items
      for (c=1; c<last_list_item; c++)
      {
         int b = 15+48; // default text color
         int h = 15;    // highlight text color
         al_draw_text(mFont.pr8, mColor.pc[b], x1, y1+(c*12)+2, 0, menu_string[c]);
         if (c == highlight)
         {
            al_draw_filled_rectangle(            x1-2, y1+(c*12), x2, y1+(c*12)+11, mColor.pc[h+128]);
            al_draw_text(mFont.pr8, mColor.pc[h], x1,   y1+(c*12)+2, 0, menu_string[c]);
            al_draw_rectangle(                   x1-2, y1+(c*12), x2, y1+(c*12)+11, mColor.pc[h], 1);
         }
      }

      if ( (mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1 ) && (mInput.mouse_y < y2) )
      {
         highlight = (mInput.mouse_y - y1)/yh;
      }
      else highlight = 0;

      if (highlight < 1) highlight = 0;
      if (highlight > last_list_item-1) highlight = 0;


      if (mInput.mouse_b[1][0])
      {
         while (mInput.mouse_b[1][0]) mEventQueue.proc();
         selection = highlight;
      }

      if (mInput.mouse_b[2][0])
      {
         while (mInput.mouse_b[2][0]) mEventQueue.proc();
         selection = 0;
      }

      if (mInput.key[ALLEGRO_KEY_ESCAPE][0])
      {
         while (mInput.key[ALLEGRO_KEY_ESCAPE][0]) mEventQueue.proc();
         selection = 0; // default position for back
      }

   }
   return selection;
}




int mwMenu::pmenu(int type, int bg_color)
{


   init_pmenu(type);

   int highlight = 2;
   int selection = 999;
   int last_list_item;
   int c, b;

   int kx = mInput.mouse_x;
   if (kx < 100) kx = 100;
   if (kx > mDisplay.SCREEN_W-100) kx = mDisplay.SCREEN_W-100;

   int up = 0;
   int ky = mInput.mouse_y-20;
   if (type == 2) if (ky > mDisplay.SCREEN_H - 160) up=1; // main editor menu
   if (type == 6) if (ky > mDisplay.SCREEN_H -  60) up=1; // lift step menu
   if (type == 5) if (ky > mDisplay.SCREEN_H -  80) up=1; // generic menu

   if (!up) // normal version
   {
      do   // until selection is made
      {
         al_rest(0.02);
         int max_strlen = 0;
         int w = 0;
         if (bg_color != 0)
         {
            c = 0;
            // run through the menu to get height and width
            while (strcmp(menu_string[c],"end") != 0)
            {
               int sl = strlen(menu_string[c]);
               if (sl > max_strlen) max_strlen = sl;
               c++;
            }
            w = max_strlen*4 + 2;
            al_draw_filled_rectangle(kx-w, ky-2, kx+w, ky+c*8+1, mColor.pc[bg_color+192]); // blank background
            al_draw_rectangle       (kx-w, ky-2, kx+w, ky+c*8+1, mColor.pc[bg_color], 1);  // frame entire menu
         }
         c = 0;
         while (strcmp(menu_string[c],"end") != 0)
         {
            b = 9 + 96;
            if (c == 0) b = 9;
            if (c == highlight) b=9;

            int sl = strlen(menu_string[c]);
            if (sl > max_strlen) max_strlen = sl;
            w = sl*4;

            if (bg_color == 0) al_draw_filled_rectangle(kx-w, ky+(c*8), kx+w, ky+(c*8)+8, mColor.pc[0]);
            al_draw_text(mFont.pr8, mColor.pc[b], kx, ky+(c*8),  ALLEGRO_ALIGN_CENTER, menu_string[c]);
            c++;
         }
         last_list_item = c-1;
         al_flip_display();
         mEventQueue.proc();
         highlight = 2;
         if ( (mInput.mouse_x > (kx - 100)) && (mInput.mouse_x < (kx+100)) )
            if ( (mInput.mouse_y > ky ) && (mInput.mouse_y < ky + ((last_list_item+1)*8)) )
               highlight = (mInput.mouse_y-ky) / 8;
         if (!(mInput.mouse_b[2][0])) selection = highlight; // mouse b2 released

      } while (selection == 999);
   }
   if (up)  // reverse version
   {
      if (type == 5) ky += 18; // generic menu
      else ky = mInput.mouse_y+12; // to put mouse on default button


      if (ky > mDisplay.SCREEN_H) ky = mDisplay.SCREEN_H;
      do   // until selection is made
      {
         al_rest(0.02);
         int max_strlen = 0;
         int w = 0;
         if (bg_color != 0)
         {
            c = 0;
            // run through the menu to get height and width
            while (strcmp(menu_string[c],"end") != 0)
            {
               int sl = strlen(menu_string[c]);
               if (sl > max_strlen) max_strlen = sl;
               c++;
            }
            w = max_strlen*4 + 2;
            al_draw_filled_rectangle(kx-w, ky+9, kx+w, ky-c*8+6, mColor.pc[bg_color+192]); // blank background
            al_draw_rectangle       (kx-w, ky+9, kx+w, ky-c*8+6, mColor.pc[bg_color], 1);     // frame entire menu
         }

         c = 0;
         while (strcmp(menu_string[c],"end") != 0)
         {
            b = 9+96;
            if (c == 0) b = 9;
            if (c == highlight) b=9;
            int w = strlen(menu_string[c])*4;
            if (bg_color == 0) al_draw_filled_rectangle(kx-w, ky-(c*8), kx+w, ky-(c*8)+8, mColor.pc[0]);
            al_draw_text(mFont.pr8, mColor.pc[b], kx, ky-(c*8),  ALLEGRO_ALIGN_CENTER, menu_string[c]);
            c++;
         }
         last_list_item = c-1;

         al_flip_display();
         mEventQueue.proc();

         //show_mouse(screen);
         highlight = 2;
         if ( (mInput.mouse_x > (kx - 100)) && (mInput.mouse_x < (kx+100)) )
            if ( (mInput.mouse_y < ky ) && (mInput.mouse_y > ky - ((last_list_item+1)*8) ) )
               highlight = (ky-mInput.mouse_y+8) / 8;
         if (!(mInput.mouse_b[2][0])) selection = highlight; // mouse b2 released

      } while (selection == 999);
   }
   return selection;
}
