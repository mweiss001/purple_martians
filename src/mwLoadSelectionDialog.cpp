// mwLoadSelectionDialog.cpp

#include "pm.h"
#include "mwLoadSelectionDialog.h"

#include "mwColor.h"
#include "mwDisplay.h"
#include "mwEditSelection.h"
#include "mwEventQueue.h"
#include "mwFileIterator.h"
#include "mwFont.h"
#include "mwInput.h"
#include "mwLevelEditor.h"
#include "mwWidget.h"

mwLoadSelectionDialog mLoadSelectionDialog;

int mwLoadSelectionDialog::run()
{


   int color = 11;


   mWM.mW[1].init(1, 0, 20, 20, 600, 400, color, "File Select", 1, 0, 14, 1);
   mWM.mW[1].set_resizeable(300, 800, 100, 1600);
   mWM.mW[1].redrawCallback = []() { mLoadSelectionDialog.redraw_callback(); };
   mWM.mW[1].drawFunction = [this]() { mLoadSelectionDialog.draw_file_select(mWM.mW[1]); };


   mWM.mW[2].init(2, 1, 20, 600, 200, 200, color, "Selection", 0, 0, 14, 0);
   mWM.mW[2].redrawCallback = []() { mLoadSelectionDialog.redraw_callback(); };
   mWM.mW[2].drawFunction = [this]() { mLoadSelectionDialog.draw_selection(mWM.mW[2]); };

   // set display window adjacent to list window
   mWM.mW[2].rect.setX1Y1(mWM.mW[1].rect.x2, mWM.mW[1].rect.y1);


   mListBox.init();
   mListBox.color = color;

   int num_items = mFileIterator.iterate("sel\\");
   for (int i=0; i<num_items; i++)
      mListBox.list.push_back(al_get_fs_entry_name(mFileIterator.filenames[i]));


   quit = 0;
   retval = 0;
   while (!quit)
   {
      mWM.cycle_windows(0);

      al_flip_display();
      mEventQueue.proc(1);
      if (mInput.key[ALLEGRO_KEY_ESCAPE][0])
      {
         while (mInput.key[ALLEGRO_KEY_ESCAPE][0]) mEventQueue.proc(1);
         quit = 1;
         retval = 0;
      }

      if (mInput.key[ALLEGRO_KEY_ENTER][0])
      {
         while (mInput.key[ALLEGRO_KEY_ENTER][0]) mEventQueue.proc(1);
         quit = 1;
         retval = 1;
      }


      if (!mWM.mW[1].active)
      {
         quit = 1;
         retval = 0;
      }
   }


   return retval;



}


void mwLoadSelectionDialog::redraw_callback()
{
   mEventQueue.proc(1);
   if (mEventQueue.program_update)
   {
      mEventQueue.program_update = 0;
      mLevelEditor.redraw_callback();
   }
   mWM.cycle_windows(1);
   al_flip_display();
}


void mwLoadSelectionDialog::draw_selection(mwWindow &w)
{
   // nothing has been selected yet
   if (mListBox.selected_item == -1)
   {
      mWM.mW[2].rect.setWH(0, 0);
      return;
   }

   // get filename
   char filename[500];
   sprintf(filename, "%s", mListBox.list[mListBox.selected_item].c_str());

   // load new selection and clear flag
   if (mListBox.selection_changed)
   {
      mListBox.selection_changed = false;
      mEditSelection.load_selection(filename);
      mLevelEditor.redraw_callback();
   }


   // selection size
   int sw = mEditSelection.ft_level_header[8] * 20;
   int sh = mEditSelection.ft_level_header[9] * 20;

   // change window size to match selection
   int ww = sw + 2 + 32;
   int wh = sh + 64 + 32;

   // enforce minimum size
   if (ww < 200) ww = 200;

   mWM.mW[2].rect.setWH(ww, wh);

   // set display window adjacent to list window
   mWM.mW[2].rect.setX1Y1(mWM.mW[1].rect.x2, mWM.mW[1].rect.y1);


   // erase and frame
   w.rect.draw_filled_rectangle(mColor.pc[0]);
   w.rect.draw_rectangle(mColor.pc[w.color], 1);

   int xa = w.rect.x1+1;
   int ya = w.rect.y1+2;

   int ts = 10;


   al_draw_textf(mFont.pr8, mColor.pc[15], xa+2, ya, 0, "%s", filename); ya+=ts;
   al_draw_textf(mFont.pr8, mColor.pc[15], xa+2, ya, 0, "Width:%d",    mEditSelection.ft_level_header[8]); ya+=ts;
   al_draw_textf(mFont.pr8, mColor.pc[15], xa+2, ya, 0, "Height:%d",   mEditSelection.ft_level_header[9]); ya+=ts;
   al_draw_textf(mFont.pr8, mColor.pc[15], xa+2, ya, 0, "Items: %d",   mEditSelection.ft_level_header[3]); ya+=ts;
   al_draw_textf(mFont.pr8, mColor.pc[15], xa+2, ya, 0, "Enemies: %d", mEditSelection.ft_level_header[4]); ya+=ts;
   al_draw_textf(mFont.pr8, mColor.pc[15], xa+2, ya, 0, "Lifts: %d",   mEditSelection.ft_level_header[5]); ya+=ts;

   al_draw_line(w.rect.x1, ya, w.rect.x2, ya, mColor.pc[w.color], 1); ya+=1;


   if (mEditSelection.ft_bmp)
   {
      int x = xa + 16;
      int y = ya + 16;

      for (int a=0; a<8; a++)
      {
         al_draw_rectangle(x-a, y-a, x+sw+a, y+sh+a, mColor.pc[w.color+(8-a)*32], 1);
      }


      al_draw_bitmap(mEditSelection.ft_bmp, x, y, 0);

   }

   al_flip_display();
}




void mwLoadSelectionDialog::draw_file_select(mwWindow w)
{
   int x1 = mWM.mW[1].rect.x1+0;
   int x2 = mWM.mW[1].rect.x2-0;
   int y1 = mWM.mW[1].rect.y1+12;
   int y2 = mWM.mW[1].rect.y2-20;
   mListBox.rect.setX1Y1X2Y2(x1, y1, x2, y2);
   mListBox.draw();


   int c = mWM.mW[1].color;
   int d = mWM.mW[1].disable_input;
   int ya = mWM.mW[1].rect.y2-16;



   int xa1 = x1 + mWM.mW[1].rect.w * 1 / 3;
   int xa2 = x1 + mWM.mW[1].rect.w * 2 / 3;

   if (mWidget.mButton(5,xa1,0,  1,ya,12,  0,  1,3,1,  c+192,c,15, 15,0, "Select", d))
   {
      quit = 1;
      retval = 1;
   }

   if (mWidget.mButton(5,xa2,0,  1,ya,12,  0,  1,3,1,  c+192,c,15, 15,0, "Cancel", d))
   {
      quit = 1;
      retval = 0;
   }









}
