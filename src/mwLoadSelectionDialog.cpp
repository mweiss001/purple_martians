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

mwLoadSelectionDialog mLoadSelectionDialog;

void mwLoadSelectionDialog::run()
{

   mWM.mW[1].init(1, 0, 20, 20, 600, 400, 15, "File Select", 1, 0, 14, 1);
   mWM.mW[1].set_resizeable(300, 800, 100, 1600);
   mWM.mW[1].redrawCallback = []() { mLoadSelectionDialog.redraw_callback(); };
   mWM.mW[1].drawFunction = [this]() { mLoadSelectionDialog.draw_file_select(mWM.mW[1]); };


   mWM.mW[2].init(2, 1, 20, 600, 200, 200, 15, "Selection", 0, 0, 14, 0);
   mWM.mW[2].redrawCallback = []() { mLoadSelectionDialog.redraw_callback(); };
   mWM.mW[2].drawFunction = [this]() { mLoadSelectionDialog.draw_selection(mWM.mW[2]); };

   // set display window adjacent to list window
   mWM.mW[2].rect.setX1Y1(mWM.mW[1].rect.x2, mWM.mW[1].rect.y1);



   mListBox.init();
   int num_items = mFileIterator.iterate("sel\\");
   for (int i=0; i<num_items; i++)
      mListBox.list.push_back(al_get_fs_entry_name(mFileIterator.filenames[i]));


   int quit = 0;
   while (!quit)
   {
      mWM.cycle_windows(0);

      al_flip_display();
      mEventQueue.proc(1);
      if (mInput.key[ALLEGRO_KEY_ESCAPE][0])
      {
         while (mInput.key[ALLEGRO_KEY_ESCAPE][0]) mEventQueue.proc(1);
         quit = 1;
      }

      if (!mWM.mW[1].active) quit = 1;


   }
}



void mwLoadSelectionDialog::redraw_callback()
{
   mLevelEditor.redraw_callback();
   mWM.cycle_windows(1);
   al_flip_display();
   mEventQueue.proc(1);
}

void mwLoadSelectionDialog::draw_selection(mwWindow w)
{
   // set display window adjacent to list window
   mWM.mW[2].rect.setX1Y1(mWM.mW[1].rect.x2, mWM.mW[1].rect.y1);


   // erase and frame
   w.rect.draw_filled_rectangle(mColor.pc[0]);
   w.rect.draw_rectangle(mColor.pc[w.color], 1);

   int xa = w.rect.x1+1;
   int ya = w.rect.y1+2;

   int ts = 10;

   char filename[500];
   sprintf(filename, "%s", mListBox.list[mListBox.selected_item].c_str());

   al_draw_textf(mFont.pr8, mColor.pc[15], xa+2, ya, 0, "%s", filename); ya+=ts;
   al_draw_textf(mFont.pr8, mColor.pc[15], xa+2, ya, 0, "Width:%d",    mEditSelection.ft_level_header[8]); ya+=ts;
   al_draw_textf(mFont.pr8, mColor.pc[15], xa+2, ya, 0, "Height:%d",   mEditSelection.ft_level_header[9]); ya+=ts;
   al_draw_textf(mFont.pr8, mColor.pc[15], xa+2, ya, 0, "Items: %d",   mEditSelection.ft_level_header[3]); ya+=ts;
   al_draw_textf(mFont.pr8, mColor.pc[15], xa+2, ya, 0, "Enemies: %d", mEditSelection.ft_level_header[4]); ya+=ts;
   al_draw_textf(mFont.pr8, mColor.pc[15], xa+2, ya, 0, "Lifts: %d",   mEditSelection.ft_level_header[5]); ya+=ts;

   al_draw_line(w.rect.x1, ya, w.rect.x2, ya, mColor.pc[w.color], 1); ya+=1;


   if (mListBox.selection_changed)
   {
      mListBox.selection_changed = false;

      // load new selection
      mEditSelection.load_selection(filename);

      mLevelEditor.redraw_callback();


      // change window size to match selection
      int sw = mEditSelection.ft_level_header[8] * 20;
      int sh = mEditSelection.ft_level_header[9] * 20;
      mWM.mW[2].rect.setWH(sw+2, sh + (ya - w.rect.y1) + 1);
   }

   if (mEditSelection.ft_bmp)
   {
      al_set_target_backbuffer(mDisplay.display);
      al_draw_bitmap(mEditSelection.ft_bmp, xa, ya, 0);
   }

   al_flip_display();
}





void mwLoadSelectionDialog::draw_file_select(mwWindow w)
{
   int x1 = mWM.mW[1].rect.x1+2;
   int x2 = mWM.mW[1].rect.x2-2;
   int y1 = mWM.mW[1].rect.y1+14;
   int y2 = mWM.mW[1].rect.y2-4;

   sprintf(mWM.mW[1].title, "File Select: %s", mListBox.list[mListBox.selected_item].c_str());

   mListBox.rect.setX1Y1X2Y2(x1, y1, x2, y2);
   mListBox.draw();
}
