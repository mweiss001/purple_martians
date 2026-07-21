// mwDemoRecord.cpp
#include "pm.h"
#include "mwDemoRecord.h"
#include "mwDemoMode.h"
#include "mwColor.h"
#include "mwFont.h"
#include "mwGameMoves.h"
#include "mwGmInfo.h"
#include "mwInput.h"
#include "mwMenu.h"
#include "mwWidget.h"

void mwDemoRecord::proc_file_details_menu(void)
{
   if (mInput.mouse_b[2][0])
   {
      sprintf(mMenu.menu_string[0],"File Details Menu");
      sprintf(mMenu.menu_string[1],"-----------------");
      sprintf(mMenu.menu_string[2],"Find Unused Fire Sections");
      sprintf(mMenu.menu_string[3],"Remove Unused Fire Sections");
      sprintf(mMenu.menu_string[4],"Remove Duplicate Game Move");
      sprintf(mMenu.menu_string[5],"Delete Past Level Done");
      sprintf(mMenu.menu_string[6],"Sort");
      sprintf(mMenu.menu_string[7],"GDT");
      sprintf(mMenu.menu_string[8],"end");
      int mp = mMenu.pmenu(5, 13, -12, 1);
      if (mp == 2)
      {
         find_fire_held_sections(-1);
         refresh();
      }
      if (mp == 3)
      {
         remove_unused_fire_held_sections(0);
         refresh();
      }
      if (mp == 4)
      {
         mGameMoves.remove_doubled_moves();
         refresh();
      }
      if (mp == 5) // delete past level done
      {
         for (int x=0; x<mGameMoves.entry_pos; x++)
            if (mGameMoves.arr[x][0] > mGmInfo.levelDoneFrame) mGameMoves.clear_single(x);
         mGameMoves.gm_sort();
         refresh();
      }
      if (mp == 6) mGameMoves.gm_sort();
      if (mp == 7) gdt();
   }
}


int mwDemoRecord::draw_file_details(int x1, int y1)
{
   int w = 208;
   //w +=  mLoop.pct_x;
   int x2 = x1 + w;

   int h = 109;
   //h +=  mLoop.pct_y;
   int y2 = y1 + h;

   int col = 10;

   if ((mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2)) proc_file_details_menu();

   al_draw_filled_rectangle(x1, y1, x2, y2, mColor.pc[0]);
   al_draw_rectangle(x1, y1, x2, y2, mColor.pc[col], 1);

   char msg[256];
   sprintf(msg, "Current File:%s", al_get_path_filename(al_create_path(current_loaded_demo_file)));
   int x2tl = x1 + 6 + strlen(msg) * 8;
   al_draw_filled_rectangle(x1, y1, x2tl, y1+12, mColor.pc[0]);
   al_draw_rectangle(x1, y1, x2tl, y1+12, mColor.pc[col], 1);
   al_draw_text(mFont.pr8, mColor.pc[15], x1+3, y1+2, 0, msg);


   int xa = x1+3;
   int xa2 = x2-3;

   int ya = y1+14;
   int bts = 16;

   int c = 9;

   if (mWidget.buttont(xa,      ya, xa+48,  bts,  0,0,0,0,  0,c,15,0,  1,0,0,0, "Load")) load_demo_record();
   if (mWidget.buttont(xa+50,   ya, xa+116, bts,  0,0,0,0,  0,c,15,0,  1,0,0,0, "Reload")) reload();
   if (mWidget.buttont(xa+118,  ya, xa2,    bts,  0,0,0,0,  0,c,15,0,  1,0,1,0, "Refresh")) refresh();

   char m1[64], m2[64];
   al_draw_textf(mFont.pr8, mColor.pc[15], xa, ya, 0, "Range:%s to %s", gettf(0, m1), gettf(mDemoMode.last_frame, m2));
   ya+=10;

   al_draw_textf(mFont.pr8, mColor.pc[15], xa, ya, 0, "Number of Moves:%d", mGameMoves.entry_pos);
   ya+=10;

   if (mGmInfo.levelDoneFrame != -1) al_draw_textf(mFont.pr8, mColor.pc[11], xa, ya, 0, "Level Done:%s", gettf(mGmInfo.levelDoneFrame, m2));
   else                              al_draw_textf(mFont.pr8, mColor.pc[11], xa, ya, 0, "Level Done:no");
   ya+=10;


   c = 4;
   if (mWidget.buttont(xa, ya, xa2, bts,  0,0,0,0,  0,c,15, 0,  1,0,1,0, "Save (overwrite current)"))
   {
      mGameMoves.save_gm(current_loaded_demo_file, 0);
      mGameMoves.load_gm(current_loaded_demo_file);
      refresh();
   }
   if (mWidget.buttont(xa, ya, xa2, bts,  0,0,0,0,  0,c,15, 0,  1,0,1,0, "Save (auto new name)"))
   {
      mGameMoves.save_gm_make_fn("new copy", 0);
      mGameMoves.load_gm(current_loaded_demo_file);
      refresh();
   }
   if (mWidget.buttont(xa, ya, xa2, bts,  0,0,0,0,  0,c,15, 0,  1,0,1,0, "Save (prompt for name)"))
   {
      mGameMoves.save_gm_file_select();
      mGameMoves.load_gm(current_loaded_demo_file);
      refresh();
   }

   return y2;
}
