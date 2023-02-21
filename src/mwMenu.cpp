// mwMenu.cpp

#include "pm.h"
#include "mwMenu.h"
#include "mwWindowManager.h"
#include "z_player.h"
#include "mwLogo.h"
#include "mwDemoMode.h"
#include "mwDisplay.h"
#include "mwFont.h"
#include "mwColor.h"
#include "mwInput.h"
#include "mwEventQueue.h"
#include "mwLevel.h"
#include "z_screen.h"

mwMenu mMenu;


mwMenu::mwMenu()
{
   initialize();
}

void mwMenu::initialize(void)
{

}



void mwMenu::init_zmenu(void)
{
   strcpy (menu_string[0], ""); // main menu
   strcpy (menu_string[1], "");
   sprintf(menu_string[2], "Start Level (%d)", mLevel.start_level);
   strcpy (menu_string[3], "Start New Game");
   strcpy (menu_string[4], "Resume Current Game");
   strcpy (menu_string[5], "Host Network Game");
   strcpy (menu_string[6], "Join Network Game");
   strcpy (menu_string[7], "Settings");
   strcpy (menu_string[8], "Level Editor");
   strcpy (menu_string[9], "Demo Mode");
   strcpy (menu_string[10], "Help Screens");
   strcpy (menu_string[11], "end");
}

void mwMenu::init_pmenu(int type)
{
   if (type == 2)
   {
      strcpy (menu_string[0], "Level Editor Pop-Up Menu");
      strcpy (menu_string[1], "------------------------");
//      strcpy (menu_string[2], "Copy ---");
//      strcpy (menu_string[3], "View ---");
//      strcpy (menu_string[4], "Delete ---");
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
//   if (type == 6)
//   {
//      sprintf(menu_string[0],"Lift:0 Step:0");
//      sprintf(menu_string[1],"---------------");
//      sprintf(menu_string[2],"Cancel");
//      sprintf(menu_string[3],"Move Step 0");
//      sprintf(menu_string[4],"Delete Step 0");
//      sprintf(menu_string[5],"Insert Steps");
//      sprintf(menu_string[6],"end");
//   }
}





int mwMenu::zmenu(int menu_pos, int y)  // this menu function does not pass through, it waits for a selection and then exits
/*
   used only for the main game menu
   blocks until a menu item is selected
   naviagate with arrow keys, ENTER and ESC
   can also navigate with game controls
*/
{

   init_zmenu();

   y+=4;

   mwDM.demo_mode_countdown_val = mwDM.demo_mode_countdown_reset;

   int highlight = menu_pos;
   int selection = 999;
   int last_list_item;

   int up_held = 0;
   int down_held = 0;
   int left_held = 0;
   int right_held = 0;

   mI.initialize();

   while (selection == 999)
   {




      al_set_target_backbuffer(display);
      al_flip_display();
      al_clear_to_color(al_map_rgb(0, 0, 0));


      while (!mwEQ.menu_update) mwEQ.proc_event_queue_menu();
      mwEQ.menu_update = 0;


      frame_and_title(1);
      mwL.mdw_an(mwL.mdw_map_logo_x, mwL.mdw_map_logo_y, mwL.mdw_map_logo_scale);



      int mx = mwD.SCREEN_W/2;

      draw_level();

      if (mLevel.resume_allowed) mwDM.demo_mode_enabled = 0;
      if (mwDM.demo_mode_enabled)
      {
         if (--mwDM.demo_mode_countdown_val < 0)
         {
            mwDM.demo_mode_countdown_val = mwDM.demo_mode_countdown_reset;
            mwDM.demo_mode_enabled = 0;
            return 9;
         }
         sprintf(menu_string[9], "Demo Mode (%d)", mwDM.demo_mode_countdown_val / 100);
      }
      else sprintf(menu_string[9], "Demo Mode");

      // draw the menu items
      int c = 0;
      while (strcmp(menu_string[c],"end") != 0)
      {
         int b = 15; // b = players[active_local_player].color;
         if ((!mLevel.resume_allowed) && (c==4)) b+=80; // dimmer if can't resume

         if (c == highlight)
         {
            int sl = strlen(menu_string[c]) * 4;
            al_draw_rectangle(mx-sl-2+0.5f, y+(c*10)-1+0.5f, mx+sl+0.5f, y+(c*10)+9+0.5f, mC.pc[b+80], 1);
         }
         al_draw_text(mF.pr8, mC.pc[b], mx, y+(c*10)+1, ALLEGRO_ALIGN_CENTRE, menu_string[c]);
         c++;
      }
      last_list_item = c-1;


      if (((mI.key[ALLEGRO_KEY_RIGHT][0]) || (players[0].right)) && (right_held == 0))
      {
         right_held = 1;
         selection = highlight + 100;
      }
      if ( (!(mI.key[ALLEGRO_KEY_RIGHT][0])) &&  (!(players[0].right)) )  right_held = 0;
      if (((mI.key[ALLEGRO_KEY_LEFT][0]) || (players[0].left)) && (left_held == 0))
      {
         left_held = 1;
         selection = highlight + 200;
      }
      if ( (!(mI.key[ALLEGRO_KEY_LEFT][0])) &&  (!(players[0].left)) )  left_held = 0;
      if (((mI.key[ALLEGRO_KEY_DOWN][0]) || (players[0].down))  && (down_held == 0))
      {
         if (++highlight > last_list_item) highlight = last_list_item;
         down_held = 1;
         mwDM.demo_mode_countdown_val = mwDM.demo_mode_countdown_reset;
         mwDM.demo_mode_enabled = 0;
      }
      if ( (!(mI.key[ALLEGRO_KEY_DOWN][0])) && (!(players[0].down))) down_held = 0;
      if (((mI.key[ALLEGRO_KEY_UP][0]) || (players[0].up)) && (up_held == 0))
      {
         if (--highlight < 2) highlight = 2;
         up_held = 1;
         mwDM.demo_mode_countdown_val = mwDM.demo_mode_countdown_reset;
         mwDM.demo_mode_enabled = 0;
      }
      if ( (!(mI.key[ALLEGRO_KEY_UP][0])) && (!(players[0].up))) up_held = 0;


      // shortcut keys
      if (mI.key[ALLEGRO_KEY_L][0])                      return 8; // level editor
      if (mI.key[ALLEGRO_KEY_O][0] && mI.SHFT() && mI.CTRL() ) return 7; // settings

      if (mI.key[ALLEGRO_KEY_PGDN][0])   highlight = last_list_item;
      if (mI.key[ALLEGRO_KEY_PGUP][0])   highlight = 2;
      if (mI.key[ALLEGRO_KEY_ENTER][0])  selection = highlight;
      if (mI.key[ALLEGRO_KEY_ESCAPE][0]) selection = 1;
   }
   return selection;
}















int mwMenu::tmenu(int menu_pos, int x1, int y1)
/*
   used in the level editor as the main menu


*/

{
   int pc = players[active_local_player].color;

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
      mwWM.redraw_level_editor_background();
      mwWM.cycle_windows(1);

      // draw menu title
      int mt = strlen(menu_string[0])*8;
      al_draw_filled_rectangle(x1-2, y1-2, x1+mt+2, y1+yh, mC.pc[pc+128]);
      al_draw_rectangle(       x1-2, y1-2, x1+mt+2, y1+yh, mC.pc[15], 1);
      al_draw_text(mF.pr8, mC.pc[15], x1, y1+1, 0, menu_string[0]);

      // erase menu background
      al_draw_filled_rectangle(x1-2, y1+12, x2, y2, mC.pc[pc+128+48]);
      al_draw_rectangle(x1-2, y1+12, x2, y2, mC.pc[15], 1);

      // draw the menu items
      for (c=1; c<last_list_item; c++)
      {
         int b = 15+48; // default text color
         int h = 15;    // highlight text color
         al_draw_text(mF.pr8, mC.pc[b], x1, y1+(c*12)+2, 0, menu_string[c]);
         if (c == highlight)
         {
            al_draw_filled_rectangle(            x1-2, y1+(c*12), x2, y1+(c*12)+11, mC.pc[h+128]);
            al_draw_text(mF.pr8, mC.pc[h], x1,   y1+(c*12)+2, 0, menu_string[c]);
            al_draw_rectangle(                   x1-2, y1+(c*12), x2, y1+(c*12)+11, mC.pc[h], 1);
         }
      }

      if ( (mI.mouse_x > x1) && (mI.mouse_x < x2) && (mI.mouse_y > y1 ) && (mI.mouse_y < y2) )
      {
         highlight = (mI.mouse_y - y1)/yh;
      }
      else highlight = 0;

      if (highlight < 1) highlight = 0;
      if (highlight > last_list_item-1) highlight = 0;


      if (mI.mouse_b[1][0])
      {
         while (mI.mouse_b[1][0]) mwEQ.proc_event_queue();
         selection = highlight;
      }

      if (mI.mouse_b[2][0])
      {
         while (mI.mouse_b[2][0]) mwEQ.proc_event_queue();
         selection = 0;
      }

      if (mI.key[ALLEGRO_KEY_ESCAPE][0])
      {
         while (mI.key[ALLEGRO_KEY_ESCAPE][0]) mwEQ.proc_event_queue();
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

   int kx = mI.mouse_x;
   if (kx < 100) kx = 100;
   if (kx > mwD.SCREEN_W-100) kx = mwD.SCREEN_W-100;

   int up = 0;
   int ky = mI.mouse_y-20;
   if (type == 2) if (ky > mwD.SCREEN_H - 160) up=1; // main editor menu
   if (type == 6) if (ky > mwD.SCREEN_H -  60) up=1; // lift step menu
   if (type == 5) if (ky > mwD.SCREEN_H -  80) up=1; // generic menu

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
            al_draw_filled_rectangle(kx-w, ky-2, kx+w, ky+c*8+1, mC.pc[bg_color+192]); // blank background
            al_draw_rectangle       (kx-w, ky-2, kx+w, ky+c*8+1, mC.pc[bg_color], 1);     // frame entire menu
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

            if (bg_color == 0) al_draw_filled_rectangle(kx-w, ky+(c*8), kx+w, ky+(c*8)+8, mC.pc[0]);
            al_draw_text(mF.pr8, mC.pc[b], kx, ky+(c*8),  ALLEGRO_ALIGN_CENTER, menu_string[c]);
            c++;
         }
         last_list_item = c-1;
         al_flip_display();
         mwEQ.proc_event_queue();
         highlight = 2;
         if ( (mI.mouse_x > (kx - 100)) && (mI.mouse_x < (kx+100)) )
            if ( (mI.mouse_y > ky ) && (mI.mouse_y < ky + ((last_list_item+1)*8)) )
               highlight = (mI.mouse_y-ky) / 8;
         if (!(mI.mouse_b[2][0])) selection = highlight; // mouse b2 released

      } while (selection == 999);
   }
   if (up)  // reverse version
   {
      if (type == 5) ky += 18; // generic menu
      else ky = mI.mouse_y+12; // to put mouse on default button


      if (ky > mwD.SCREEN_H) ky = mwD.SCREEN_H;
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
            al_draw_filled_rectangle(kx-w, ky+9, kx+w, ky-c*8+6, mC.pc[bg_color+192]); // blank background
            al_draw_rectangle       (kx-w, ky+9, kx+w, ky-c*8+6, mC.pc[bg_color], 1);     // frame entire menu
         }

         c = 0;
         while (strcmp(menu_string[c],"end") != 0)
         {
            b = 9+96;
            if (c == 0) b = 9;
            if (c == highlight) b=9;
            int w = strlen(menu_string[c])*4;
            if (bg_color == 0) al_draw_filled_rectangle(kx-w, ky-(c*8), kx+w, ky-(c*8)+8, mC.pc[0]);
            al_draw_text(mF.pr8, mC.pc[b], kx, ky-(c*8),  ALLEGRO_ALIGN_CENTER, menu_string[c]);
            c++;
         }
         last_list_item = c-1;

         al_flip_display();
         mwEQ.proc_event_queue();

         //show_mouse(screen);
         highlight = 2;
         if ( (mI.mouse_x > (kx - 100)) && (mI.mouse_x < (kx+100)) )
            if ( (mI.mouse_y < ky ) && (mI.mouse_y > ky - ((last_list_item+1)*8) ) )
               highlight = (ky-mI.mouse_y+8) / 8;
         if (!(mI.mouse_b[2][0])) selection = highlight; // mouse b2 released

      } while (selection == 999);
   }
   //show_mouse(NULL);
   return selection;
}





