// mwWindowManager.cpp
#include "pm.h"
#include "mwWindowManager.h"
#include "mwWindow.h"
#include "mwDisplay.h"
#include "mwFont.h"
#include "mwLift.h"
#include "mwColor.h"
#include "mwInput.h"
#include "mwEventQueue.h"
#include "mwBitmap.h"
#include "mwBitmapTools.h"
#include "mwEditorMain.h"
#include "mwEditSelection.h"
#include "mwLoop.h"
#include "mwItem.h"
#include "mwEnemy.h"
#include "mwGroupEdit.h"
#include "mwLevel.h"
#include "mwMiscFnx.h"
#include "mwObjectViewer.h"
#include "mwScreen.h"
#include "mwSelectionWindow.h"
#include "mwTileEditor.h"
#include "mwTileHelper.h"
#include "mwTileSets.h"


mwWindowManager mWM;


void mwWindowManager::initialize(int edit_level)
{
   selection_rect.setXYWH(0,0,0,0);

   if (edit_level) mLevel.load_level(edit_level, 0, 0); // load passed level
   else mLevel.load_level_prompt();                     // prompt for level

   mEnemy.sort_enemy();
   mItem.sort_item(1);
   mInput.initialize();

   initialize_windows();

}

void mwWindowManager::initialize_windows()
{

   mW[1].index = 1;
   mW[1].layer = 0;
   mW[1].active = 1;
   mW[1].set_pos(100, 100);
   mW[1].set_size(320, 53);
   mW[1].set_title("Status Window");


   mW[2].index = 2;
   mW[2].layer = 1;
   mW[2].active = 1;
   mW[2].set_pos(100, 300);
   mW[2].set_size(322, 100);
   mW[2].set_title("Selection Window");

   mW[3].index = 3;
   mW[3].layer = 2;
   mW[3].active = 0;
   mW[3].set_pos(500, 100);
   mW[3].set_size(82, 100);
   mW[3].set_title("Filters");

   mW[4].index = 4;
   mW[4].layer = 3;
   mW[4].active = 0;
   mW[4].set_pos(700, 100);
   mW[4].set_size(160, 250);
   mW[4].set_title("selection edit");

   mW[5].index = 5;
   mW[5].layer = 4;
   mW[5].active = 0;
   mW[5].set_pos(400, 400);
   mW[5].set_size(160, 269);
   mW[5].set_title("ge list");

   mW[6].index = 6;
   mW[6].layer = 5;
   mW[6].active = 0;
   mW[6].set_pos(600, 60);
   mW[6].set_size(100, 100);
   mW[6].set_title("ge controls");

   mW[7].index = 7;
   mW[7].layer = 6;
   mW[7].active = 0;
   mW[7].set_pos(200, 60);
   mW[7].set_size(300, 300);
   mW[7].set_title("viewer");

   mW[8].index = 8;
   mW[8].layer = 7;
   mW[8].active = 1;
   mW[8].set_pos(0, 0);
   mW[8].set_size(mDisplay.SCREEN_W, BORDER_WIDTH);
   mW[8].set_title("top menu");
   mW[8].moveable = 0;

   mW[9].index = 9;
   mW[9].layer = 8;
   mW[9].active = 0;
   mW[9].set_pos(100, 300);
   mW[9].set_size(320, 328);
   mW[9].set_title("Tile Helper");

   set_level_editor_mode(1);

}



void mwWindowManager::set_level_editor_mode(int mode)
{

   level_editor_mode = mode;
   if (!level_editor_mode)
   {
      level_editor_mode = 1;
   }

   if (level_editor_mode == 1) // edit menu
   {
      mW[1].active = 1; // status
      mW[2].active = 1; // select
      mW[3].active = 0; // filter
      mW[4].active = 0; // selection edit
      mW[5].active = 0; // ge list
      mW[6].active = 0; // ge controls
      mW[7].active = 0; // viewer
      mW[9].active = 0; // tile helper
   }

   if (level_editor_mode == 2) // selection edit
   {
      mW[1].active = 0; // status
      mW[2].active = 0; // select
      mW[3].active = 1; // filter
      mEditorMain.filter_mode = 3;
      mW[4].active = 1; // selection edit
      mW[5].active = 0; // ge list
      mW[6].active = 0; // ge controls
      mW[7].active = 0; // viewer
      mW[9].active = 0; // tile helper
   }
   if (level_editor_mode == 3) // group edit
   {
      mW[1].active = 0; // status
      mW[2].active = 0; // select
      mW[3].active = 1; // filter
      mEditorMain.filter_mode = 1;
      mW[4].active = 0; // selection edit
      mW[5].active = 1; // ge list
      mW[6].active = 1; // ge controls
      mW[7].active = 0; // viewer
      mW[9].active = 0; // tile helper
   }
   if (level_editor_mode == 4) // object viewer
   {
      mW[1].active = 0; // status
      mW[2].active = 0; // select
      mW[3].active = 1; // filter
      mEditorMain.filter_mode = 2;
      mW[4].active = 0; // selection edit
      mW[5].active = 0; // ge list
      mW[6].active = 0; // ge controls
      mW[7].active = 1; // viewer
      mW[9].active = 0; // tile helper
   }
   if (level_editor_mode == 9) // tile helper
   {
      mW[1].active = 0; // status
      mW[2].active = 0; // select
      mW[3].active = 0; // filter
      mW[4].active = 0; // selection edit
      mW[5].active = 0; // ge list
      mW[6].active = 0; // ge controls
      mW[7].active = 0; // viewer
      mW[9].active = 1; // tile helper
   }
}



int mwWindowManager::loop(int edit_level)
{
   mLoop.level_editor_running = 1;
   al_show_mouse_cursor(mDisplay.display);

   mEventQueue.reset_fps_timer();

   initialize(edit_level);
   if (mLoop.autosave_level_editor_state) load_mW();
   active = 1;

   while (active)
   {
      redraw_level_editor_background();
      if (!cycle_windows(0)) process_mouse();
      process_keypress();
   }

   if (mLoop.autosave_level_editor_state) save_mW();
   mLoop.level_editor_running = 0;
   mLevel.resume_allowed = 0;
   al_hide_mouse_cursor(mDisplay.display);
   return mLevel.last_level_loaded;
}

void mwWindowManager::get_block_position_on_map(void)
{
   // x, y in 0-99 scale
   // the mouse position past the border width is how far we are into the scaled map
   float mx1 = mInput.mouse_x-BORDER_WIDTH;
   float my1 = mInput.mouse_y-BORDER_WIDTH;

   // divide that by bs to get how many blocks we are into the map
   float mx2 = mx1 / (mDisplay.scale_factor_current * 20);
   float my2 = my1 / (mDisplay.scale_factor_current * 20);
   // get block position of WX
   float mx3 = (float)mScreen.level_display_region_x / 20;
   float my3 = (float)mScreen.level_display_region_y / 20;

   // add
   float mx4 = mx3 + mx2;
   float my4 = my3 + my2;

   // put in final variables with limit check
   gx = mMiscFnx.enforce_limit(mx4, 0, 99);
   gy = mMiscFnx.enforce_limit(my4, 0, 99);


   // hx, hy in 0-1999 scale
   // the mouse position past the border width is how far we are into the scaled map
   mx1 = mInput.mouse_x-BORDER_WIDTH;
   my1 = mInput.mouse_y-BORDER_WIDTH;

   // scale
   mx2 = mx1 / mDisplay.scale_factor_current;
   my2 = my1 / mDisplay.scale_factor_current;

   // get position of WX
   mx3 = (float)mScreen.level_display_region_x;
   my3 = (float)mScreen.level_display_region_y;

   // add
   mx4 = mx3 + mx2;
   my4 = my3 + my2;

   // put in final variables with limit check
   hx = mMiscFnx.enforce_limit(mx4, 0, 1999);
   hy = mMiscFnx.enforce_limit(my4, 0, 1999);
}

void mwWindowManager::process_scrolledge(void)
{
   int bw = BORDER_WIDTH;
   int swb = mDisplay.SCREEN_W-bw;
   int shb = mDisplay.SCREEN_H-bw;

   if (mInput.mouse_y > shb) mScreen.level_display_region_y+=(mInput.mouse_y - shb)*2; // scroll down
   if (mInput.mouse_x > swb) mScreen.level_display_region_x+=(mInput.mouse_x - swb)*2; // scroll right
   if (mInput.mouse_x < bw)  mScreen.level_display_region_x-=(bw - mInput.mouse_x)*2;  // scroll left
   if (mInput.mouse_y < 4)   mScreen.level_display_region_y-=(4  - mInput.mouse_y)*7;  // scroll up (is different because of menu)

   // find the size of the source screen from actual screen size and scaler
   int SW = (int)( (float)(mDisplay.SCREEN_W - bw *2) / mDisplay.scale_factor_current);
   int SH = (int)( (float)(mDisplay.SCREEN_H - bw *2) / mDisplay.scale_factor_current);
   if (SW > 2000) SW = 2000;
   if (SH > 2000) SH = 2000;
   mScreen.level_display_region_w = SW;
   mScreen.level_display_region_h = SH;

   // correct for edges
   if (mScreen.level_display_region_x < 0)         mScreen.level_display_region_x = 0;
   if (mScreen.level_display_region_y < 0)         mScreen.level_display_region_y = 0;
   if (mScreen.level_display_region_x > (2000-SW)) mScreen.level_display_region_x = 2000-SW;
   if (mScreen.level_display_region_y > (2000-SH)) mScreen.level_display_region_y = 2000-SH;

}



// this function draws a rectangle frame at full scale on level buffer
// used by se, ge and em
void mwWindowManager::show_level_buffer_block_rect(mwRect<int> rect, int color, const char * text)
{
   rect.ensure_x1y1_less_than_x2y2();
   rect.multiply(20);
   rect.resize(21, 21);
   rect.enforceLimits(1, 1, 1998, 1998);
   rect.draw_rectangle(mColor.pc[14], 1);
   al_draw_text(mFont.pr8, mColor.pc[color], rect.x1+2, rect.y1-11,  0, text);
}

// this function draws a frame text box at full scale on level buffer, beside the selection
// if offscreen it will reposition to be seen
// used by em tileset drawRect
void mwWindowManager::show_level_buffer_block_rect_text(mwRect<int> rect)
{
   rect.ensure_x1y1_less_than_x2y2();
   rect.multiply(20);
   rect.resize(20, 20);

   // translate level block positions (0-99) to x and y positions (0-1999)
   int qx1 = rect.x1;
   int qy1 = rect.y1;
   // int qx2 = rect.x2;
   int qy2 = rect.y2;

   int tw = 280;
   int th = 46;
   if (mTileSets.altTextLine2.size()) th+=20;

   // default is to draw the box underneath, left justified
   int tx1 = qx1;
   int tx2 = tx1 + tw;

   int ty1 = qy2 + 10;
   int ty2 = ty1 + th;

   // if below bottom of level buffer 2000, switch to draw above
   if (ty2 > 1999)
   {
      ty1 = qy1 - th - 10;
      ty2 = ty1 + th;
   }


   // if to the right of level buffer 2000, switch to draw on the left
   if (tx2 > 1999)
   {
      tx1 = qx1 - tw - 10;
      tx2 = tx1 + tw;
   }

   // we could still be off the edge of the screen

   // I would somehow like the position to be on the corner that the mouse is moving


   al_draw_filled_rectangle(tx1, ty1, tx1+tw, ty2, mColor.Black);
   al_draw_rectangle(tx1, ty1, tx1+tw, ty2, mColor.White, 1);
   ty1+=4;

   al_draw_textf(mFont.pr8, mColor.pc[15], tx1+4, ty1,  0, "Tileset Type: %s ", mTileSets.type_name[mTileSets.ts.tileSetType]);
   ty1+=10;

   al_draw_textf(mFont.pr8, mColor.pc[15], tx1+4, ty1,  0, "TileSet Name: %s", mTileSets.ts.name.c_str());
   ty1+=10;

   al_draw_textf(mFont.pr8, mColor.pc[15], tx1+4, ty1,  0, "Mode: %s", mTileSets.altTextLine1.c_str());
   ty1+=10;

   al_draw_textf(mFont.pr8, mColor.pc[13], tx1+4, ty1,  0, "press [1] and [2] to change mode");
   ty1+=10;

   al_draw_textf(mFont.pr8, mColor.pc[15], tx1+4, ty1,  0, "%s", mTileSets.altTextLine2.c_str());
   ty1+=10;

   al_draw_textf(mFont.pr8, mColor.pc[13], tx1+4, ty1,  0, "%s", mTileSets.altTextLine3.c_str());
   ty1+=10;

}


// used by se, ge and em
// when called, mouse b1 is pressed, blocks until released
bool mwWindowManager::get_new_box(const char* text, bool preview)
{
   // use temp values internally for drag
   int temp_bx1 = gx;
   int temp_by1 = gy;

   // set both corners to initial position
   int temp_bx2 = temp_bx1;
   int temp_by2 = temp_by1;

   while (mInput.mouse_b[1][0])
   {
      // update position
      temp_bx2 = gx;
      temp_by2 = gy;

      // set the real values and correct
      selection_rect.setX1Y1X2Y2(temp_bx1, temp_by1, temp_bx2, temp_by2);
      selection_rect.ensure_x1y1_less_than_x2y2();

      redraw_level_editor_background(0);
      show_level_buffer_block_rect(selection_rect, 14, text);

      if (preview)
      {
         show_level_buffer_block_rect_text(selection_rect);

         mTileSets.altDrawRectModeForceSingle = 0;
         if (mInput.key[ALLEGRO_KEY_S][0]) mTileSets.altDrawRectModeForceSingle = 1;

         if (mInput.key[ALLEGRO_KEY_1][3]) mTileSets.altDrawRectMode++;
         if (mInput.key[ALLEGRO_KEY_2][3]) mTileSets.altDrawRectMode--;

         if (mInput.key[ALLEGRO_KEY_UP   ][3]) mTileSets.altDrawRectModePatternHeight--;
         if (mInput.key[ALLEGRO_KEY_DOWN ][3]) mTileSets.altDrawRectModePatternHeight++;
         if (mInput.key[ALLEGRO_KEY_RIGHT][3]) mTileSets.altDrawRectModePatternWidth++;
         if (mInput.key[ALLEGRO_KEY_LEFT ][3]) mTileSets.altDrawRectModePatternWidth--;

         if (mInput.key[ALLEGRO_KEY_PGUP ][3]) mTileSets.altDrawRectModePatternFill++;
         if (mInput.key[ALLEGRO_KEY_PGDN ][3]) mTileSets.altDrawRectModePatternFill--;

         mTileSets.drawRect(1);
      }
      mScreen.draw_scaled_level_region_to_display();
   }

   if (mInput.key[ALLEGRO_KEY_ESCAPE][0])
   {
      while (mInput.key[ALLEGRO_KEY_ESCAPE][0]) mEventQueue.proc(1);
      return false;
   }
   return true;
}



void mwWindowManager::process_mouse()
{
   if (level_editor_mode == 1) mEditorMain.process_mouse();
   if (level_editor_mode == 2) mEditSelection.process_mouse();
   if (level_editor_mode == 3) mGroupEdit.process_mouse();
   if (level_editor_mode == 4) mObjectViewer.ov_process_mouse();
   if (level_editor_mode == 9) mTileHelper.process_mouse();
}

void mwWindowManager::process_keypress()
{
   if (level_editor_mode == 4) mObjectViewer.ov_process_keypress();

   while (mInput.key[ALLEGRO_KEY_ESCAPE][0])
   {
      active = 0;
      mEventQueue.proc(1);
   }

   if (mInput.key[ALLEGRO_KEY_Q][0])
   {
      while (mInput.key[ALLEGRO_KEY_Q][0]) mEventQueue.proc(1);
      mBitmapTools.copy_tiles();
   }


   if (mInput.key[ALLEGRO_KEY_F][0])
   {
      while (mInput.key[ALLEGRO_KEY_F][0]) mEventQueue.proc(1);
      mBitmapTools.edit_tile_flags();
   }

   if (mInput.key[ALLEGRO_KEY_E][0])
   {
      while (mInput.key[ALLEGRO_KEY_E][0]) mEventQueue.proc(1);
      {
         mwTileEditor te;
         te.edit_tile(0);
      }
   }

   if (mInput.key[ALLEGRO_KEY_H][0])
   {
      while (mInput.key[ALLEGRO_KEY_H][0]) mEventQueue.proc(1);
      {
         mWM.set_level_editor_mode(9);
      }
   }


   if (mEditorMain.level_editor_quit_confirmation_dialog && active == 0)
   {
      int ret = mMiscFnx.exit_level_editor_dialog();
      if (ret == 0) mLevel.save_level(mLevel.last_level_loaded); // save and exit
      if (ret == 2) active = 1; // cancel
   }
}

void mwWindowManager::redraw_level_editor_background(int mode)
{
   int old_lem = level_editor_mode;
   level_editor_mode = mode;
   redraw_level_editor_background();
   level_editor_mode = old_lem;
}

int mwWindowManager::redraw_level_editor_background()
{
   int drawn = 0;

   mEventQueue.proc(1);
   if (mEventQueue.program_update)
   {
      mEventQueue.program_update = 0;
      drawn = 1;

      al_flip_display();

      int mouse_on_window = is_mouse_on_any_window();
      if ((!mouse_on_window) || (level_editor_mode == 0) || (level_editor_mode == 4)) get_block_position_on_map();

      mColor.process_flash_color();
      mBitmap.update_animation();

      process_scrolledge();
      mDisplay.proc_scale_factor_change();

      mScreen.get_new_background(0);
      mLift.draw_lifts();
      mItem.draw_items();
      mEnemy.draw_enemies();

      if (level_editor_mode == 1) mEditorMain.draw_level_editor_background_overlays(mouse_on_window);    // Editor Main
      if (level_editor_mode == 2) mEditSelection.draw_level_editor_background_overlays(mouse_on_window); // Edit Selection
      if (level_editor_mode == 3) mGroupEdit.draw_level_editor_background_overlays(mouse_on_window);     // Group Edit
      if (level_editor_mode == 4) mObjectViewer.draw_level_editor_background_overlays(mouse_on_window);  // Object Viewer
      if (level_editor_mode == 9) mTileHelper.draw_level_editor_background_overlays(mouse_on_window);    // Tile Helper
      if (level_editor_mode) mScreen.draw_scaled_level_region_to_display();
   }
   return drawn;
}



int mwWindowManager::is_mouse_on_any_window(void)
{
   // check all windows declared sizes
   for (int a=0; a<NUM_MW; a++)
      if ((mW[a].active) && (mW[a].detect_mouse())) return 1;


   // check if on draw flags area
   if (mEditorMain.status_window_has_mouse) return 1;

   // check if mouse position gx, gy is valid
   // x, y in 0-99 scale
   // the mouse position past the border width is how far we are into the scaled map
   float mx1 = mInput.mouse_x-BORDER_WIDTH;
   float my1 = mInput.mouse_y-BORDER_WIDTH;

   // divide that by bs to get how many blocks we are into the map
   float mx2 = mx1 / (mDisplay.scale_factor_current * 20);
   float my2 = my1 / (mDisplay.scale_factor_current * 20);

   // get block position
   float mx3 = (float)mScreen.level_display_region_x / 20;
   float my3 = (float)mScreen.level_display_region_y / 20;

   // add to tl corner index
   int mx4 = (mx3 + mx2);
   int my4 = (my3 + my2);
   if (mx4 < 0 || mx4 > 99 || my4 < 0 || my4 > 99) return 1;

   return 0;
}


void mwWindowManager::set_focus(int n)
{
   mW[n].have_focus = 1;

   // detect if this window is not top window
   if (mW[n].layer !=0)
   {
      // get old layer number of window we are promoting
      int on = mW[n].layer;

      // slide everything down one layer (add 1 to all layers)
      for (int a=0; a<NUM_MW; a++)
         if (mW[a].active) mW[a].layer++;

      // set new top layer
      mW[n].layer = 0;

      // remove gaps in layers
      for (int b=on+2; b<NUM_MW+10; b++) // cycle from old promoted layer down
         for (int a=0; a<NUM_MW; a++)    // cycle all windows
            if ((mW[a].active) && (mW[a].layer == b)) mW[a].layer--; // move layer up
   }
}

int mwWindowManager::get_max_layer()
{
   int max_layer = 0;
   for (int a=0; a<NUM_MW; a++)
      if ((mW[a].active) && (mW[a].layer > max_layer)) max_layer = mW[a].layer;
   return max_layer;
}



// iterate windows by layers down from max, drawing and detecting mouse as we go
// draw_only is set only when redrawing in the middle of a mouse drag move
int mwWindowManager::cycle_windows(int draw_only)
{
   for (int a=0; a<NUM_MW; a++) mW[a].check_offscreen();

   int mouse_on_window = 0;
   for (int b = get_max_layer(); b>=0; b--)
      for (int a=0; a<NUM_MW; a++) // draw any window at that layer
         if ((mW[a].active) && (mW[a].layer == b))
         {
            mW[a].draw(draw_only);
            if ((!draw_only) && (mW[a].detect_mouse())) mouse_on_window = a;
         }
  // at this point, all windows have been drawn in order
  // and 'mouse_on_window' is set to the top window that has the mouse on it

   // allows focus to change
   // allows mouse drag move to start
   // allows mouse b2 pop up menu
   if (!draw_only)
   {
      // set all window to not have focus
      for (int a=0; a<NUM_MW; a++)
         if (mW[a].active) mW[a].have_focus = 0;

      // this one does
      if (mouse_on_window) set_focus(mouse_on_window);

      // process window that has focus
      for (int a=0; a<NUM_MW; a++)
         if ((mW[a].active) && (mW[a].have_focus)) mW[a].process_mouse();

   }
   return mouse_on_window;
}


void mwWindowManager::save_mW()
{
   FILE *fp = fopen("data/mW.pm", "wb");
   if (fp)
   {
      fwrite(&mWM, sizeof(mWM), 1, fp);
      fclose(fp);
   }
   else printf("error saving mW.pm\n");
}

void mwWindowManager::load_mW()
{
   FILE *fp = fopen("data/mW.pm", "rb");
   if (fp)
   {
      fread(&mWM, sizeof(mWM), 1, fp);
      fclose(fp);
   }
   else
   {
      printf("error loading mW.pm -- using defaults\n");
   }
   // why do I delete it after I load it?
   // so that if it crashes after loading, the next time it runs will be a clean slate
   al_remove_filename("data/mW.pm");


   mSelectionWindow.fill_block_array();
}


