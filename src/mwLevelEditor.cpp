#include "pm.h"
#include "mwLevelEditor.h"

#include "mwBitmap.h"
#include "mwBitmapTools.h"
#include "mwColor.h"
#include "mwDisplay.h"
#include "mwEditorMain.h"
#include "mwEditSelection.h"
#include "mwEnemy.h"
#include "mwEventQueue.h"
#include "mwFont.h"
#include "mwGroupEdit.h"
#include "mwInput.h"
#include "mwItem.h"
#include "mwLevel.h"
#include "mwLoop.h"
#include "mwMiscFnx.h"
#include "mwObjectViewer.h"
#include "mwScreen.h"
#include "mwSelectionWindow.h"
#include "mwTileEditor.h"
#include "mwTileHelper.h"
#include "mwTileSets.h"

mwLevelEditor mLevelEditor;

void mwLevelEditor::init(int level)
{
   selection.setXYWH(0,0,0,0);
   if (level) mLevel.load_level(level, 0, 0); // load passed level
   else mLevel.load_level_prompt();           // prompt for level
   mEnemy.sort_enemy();
   mItem.sort_item(1);
   mInput.initialize();
   init_windows();
   set_mode(1);
}

void mwLevelEditor::init_windows()
{
   mWM.mW[1].index = 1;
   mWM.mW[1].layer = 0;
   mWM.mW[1].active = 1;
   mWM.mW[1].set_pos(100, 100);
   mWM.mW[1].set_size(320, 53);
   mWM.mW[1].set_title("Status Window");
   mWM.mW[1].drawFunction = [this]() { mEditorMain.draw_status_window(mWM.mW[1].rect, mWM.mW[1].disable_input, mWM.mW[1].have_focus); };
   mWM.mW[1].detectMouse  = [this]() { return mEditorMain.status_window_mouse_detect(mWM.mW[1].rect); };
   mWM.mW[1].redrawCallback = []() { mLevelEditor.redraw_callback(); };

   mWM.mW[2].index = 2;
   mWM.mW[2].layer = 1;
   mWM.mW[2].active = 1;
   mWM.mW[2].set_pos(100, 300);
   mWM.mW[2].set_size(322, 100);
   mWM.mW[2].set_title("Selection Window");
   mWM.mW[2].drawFunction = [this]() { mSelectionWindow.draw(mWM.mW[2].rect, mWM.mW[2].disable_input, mWM.mW[2].have_focus); };
   mWM.mW[2].redrawCallback = []() { mLevelEditor.redraw_callback(); };

   mWM.mW[3].index = 3;
   mWM.mW[3].layer = 2;
   mWM.mW[3].active = 0;
   mWM.mW[3].set_pos(500, 100);
   mWM.mW[3].set_size(82, 100);
   mWM.mW[3].set_title("Filters");
   mWM.mW[3].drawFunction = [this]() { mEditorMain.draw_filter_window(mWM.mW[3].rect, mWM.mW[3].disable_input, mWM.mW[3].have_focus); };
   mWM.mW[3].redrawCallback = []() { mLevelEditor.redraw_callback(); };

   mWM.mW[4].index = 4;
   mWM.mW[4].layer = 3;
   mWM.mW[4].active = 0;
   mWM.mW[4].set_pos(700, 100);
   mWM.mW[4].set_size(160, 250);
   mWM.mW[4].set_title("Edit Selection");
   mWM.mW[4].drawFunction = [this]() { mEditSelection.draw(mWM.mW[4].rect, mWM.mW[4].disable_input, mWM.mW[4].have_focus); };
   mWM.mW[4].redrawCallback = []() { mLevelEditor.redraw_callback(); };

   mWM.mW[5].index = 5;
   mWM.mW[5].layer = 4;
   mWM.mW[5].active = 0;
   mWM.mW[5].set_pos(400, 400);
   mWM.mW[5].set_size(160, 269);
   mWM.mW[5].set_title("Group Edit List");
   mWM.mW[5].drawFunction = [this]() { mGroupEdit.draw_list(mWM.mW[5].rect, mWM.mW[5].disable_input, mWM.mW[5].have_focus); };
   mWM.mW[5].redrawCallback = []() { mLevelEditor.redraw_callback(); };

   mWM.mW[6].index = 6;
   mWM.mW[6].layer = 5;
   mWM.mW[6].active = 0;
   mWM.mW[6].set_pos(600, 60);
   mWM.mW[6].set_size(100, 100);
   mWM.mW[6].set_title("Group Edit Controls");
   mWM.mW[6].drawFunction = [this]() { mGroupEdit.draw_controls(mWM.mW[6].rect, mWM.mW[6].disable_input, mWM.mW[6].have_focus); };
   mWM.mW[6].redrawCallback = []() { mLevelEditor.redraw_callback(); };

   mWM.mW[7].index = 7;
   mWM.mW[7].layer = 6;
   mWM.mW[7].active = 0;
   mWM.mW[7].set_pos(200, 60);
   mWM.mW[7].set_size(300, 300);
   mWM.mW[7].set_title("Object Viewer");
   mWM.mW[7].drawFunction = [this]() { mObjectViewer.draw(mWM.mW[7].rect, mWM.mW[7].disable_input, mWM.mW[7].have_focus); };
   mWM.mW[7].redrawCallback = []() { mLevelEditor.redraw_callback(); };

   mWM.mW[8].index = 8;
   mWM.mW[8].layer = 7;
   mWM.mW[8].active = 1;
   mWM.mW[8].set_pos(0, 0);
   mWM.mW[8].set_size(mDisplay.SCREEN_W, BORDER_WIDTH);
   mWM.mW[8].set_title("Level Editor Top Menu");
   mWM.mW[8].moveable = 0;
   mWM.mW[8].drawFunction = [this]() { mEditorMain.process_menu_bar(mWM.mW[8].rect, mWM.mW[8].disable_input, mWM.mW[8].have_focus); };
   mWM.mW[8].redrawCallback = []() { mLevelEditor.redraw_callback(); };

   mWM.mW[9].index = 9;
   mWM.mW[9].layer = 8;
   mWM.mW[9].active = 0;
   mWM.mW[9].set_pos(100, 300);
   mWM.mW[9].set_size(320, 328);
   mWM.mW[9].set_title("Tile Helper");
   mWM.mW[9].drawFunction = [this]() { mTileHelper.draw(mWM.mW[9].rect, mWM.mW[9].disable_input, mWM.mW[9].have_focus); };
   mWM.mW[9].redrawCallback = []() { mLevelEditor.redraw_callback(); };
}

void mwLevelEditor::set_mode(int new_mode)
{
   mode = new_mode;
   if (!mode) mode = 1;
   if (mode == 1) // edit menu
   {
      mWM.mW[1].active = 1; // status
      mWM.mW[2].active = 1; // select
      mWM.mW[3].active = 0; // filter
      mWM.mW[4].active = 0; // selection edit
      mWM.mW[5].active = 0; // ge list
      mWM.mW[6].active = 0; // ge controls
      mWM.mW[7].active = 0; // viewer
      mWM.mW[9].active = 0; // tile helper
   }

   if (mode == 2) // selection edit
   {
      mWM.mW[1].active = 0; // status
      mWM.mW[2].active = 0; // select
      mWM.mW[3].active = 1; // filter
      mEditorMain.filter_mode = 3;
      mWM.mW[4].active = 1; // selection edit
      mWM.mW[5].active = 0; // ge list
      mWM.mW[6].active = 0; // ge controls
      mWM.mW[7].active = 0; // viewer
      mWM.mW[9].active = 0; // tile helper
   }
   if (mode == 3) // group edit
   {
      mWM.mW[1].active = 0; // status
      mWM.mW[2].active = 0; // select
      mWM.mW[3].active = 1; // filter
      mEditorMain.filter_mode = 1;
      mWM.mW[4].active = 0; // selection edit
      mWM.mW[5].active = 1; // ge list
      mWM.mW[6].active = 1; // ge controls
      mWM.mW[7].active = 0; // viewer
      mWM.mW[9].active = 0; // tile helper
   }
   if (mode == 4) // object viewer
   {
      mWM.mW[1].active = 0; // status
      mWM.mW[2].active = 0; // select
      mWM.mW[3].active = 1; // filter
      mEditorMain.filter_mode = 2;
      mWM.mW[4].active = 0; // selection edit
      mWM.mW[5].active = 0; // ge list
      mWM.mW[6].active = 0; // ge controls
      mWM.mW[7].active = 1; // viewer
      mWM.mW[9].active = 0; // tile helper
   }
   if (mode == 9) // tile helper
   {
      mWM.mW[1].active = 0; // status
      mWM.mW[2].active = 0; // select
      mWM.mW[3].active = 0; // filter
      mWM.mW[4].active = 0; // selection edit
      mWM.mW[5].active = 0; // ge list
      mWM.mW[6].active = 0; // ge controls
      mWM.mW[7].active = 0; // viewer
      mWM.mW[9].active = 1; // tile helper
   }
}

int mwLevelEditor::loop(int level)
{
   mLoop.level_editor_running = 1;
   al_show_mouse_cursor(mDisplay.display);
   mEventQueue.reset_fps_timer();
   init(level);
   if (mLoop.autosave_level_editor_state) load_mW();

   while (active)
   {
      redraw_background();
      mWM.cycle_windows(0);
      process_mouse();
      process_keypress();
   }

   if (mLoop.autosave_level_editor_state) save_mW();
   mLoop.level_editor_running = 0;
   mLevel.resume_allowed = 0;
   al_hide_mouse_cursor(mDisplay.display);
   return mLevel.last_level_loaded;
}


void mwLevelEditor::process_scrolledge(void)
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
void mwLevelEditor::show_selection_rect(mwRect<int> rect, int color, const char * text)
{
   rect.ensure_x1y1_less_than_x2y2();
   rect.multiply(20);
   rect.resize(21, 21);
   rect.enforceLimits(1, 1, 1998, 1998);
   rect.draw_rectangle(mColor.pc[14], 1);
   al_draw_text(mFont.pr8, mColor.pc[color], rect.x1+2, rect.y1-11,  0, text);
}

// this function draws a framed text box at full scale on level buffer, beside the selection
// if offscreen it will reposition to be seen
// used by em tileset drawRect
void mwLevelEditor::show_selection_text(mwRect<int> rect)
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
// mouse b1 is pressed when called, blocks until released
// if preview is set, also call show_selection_text and precess keypresses for alt draw modes
bool mwLevelEditor::get_new_selection(const char* text, bool preview)
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
      // get_mouse_position_on_background();
      temp_bx2 = gx;
      temp_by2 = gy;

      // set the real values and correct
      selection.setX1Y1X2Y2(temp_bx1, temp_by1, temp_bx2, temp_by2);
      selection.ensure_x1y1_less_than_x2y2();

      redraw_background(0);
      show_selection_rect(selection, 14, text);

      if (preview)
      {
         show_selection_text(selection);

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


void mwLevelEditor::process_mouse()
{
   //get_mouse_position_on_background();

   if (!mWM.mouse_on_window)
   {
      if (mode == 1) mEditorMain.process_mouse_on_background();
      if (mode == 2) mEditSelection.process_mouse_on_background();
      if (mode == 3) mGroupEdit.process_mouse_on_background();
      if (mode == 4) mObjectViewer.ov_process_mouse_on_background();
      if (mode == 9) mTileHelper.process_mouse_on_background();
   }
}


void mwLevelEditor::get_mouse_position_on_background()
{
   pos_valid = true;
   if (mWM.mouse_on_window) pos_valid = false;

   // gx, gy in 0-99 scale
   // the mouse position past the border width is how far we are into the scaled level background
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

   if (mx4<0 || mx4>99 || my4<0 || my4>99) pos_valid = false;

   // put in final variables with limit check
   gx = mMiscFnx.enforce_limit(mx4, 0, 99);
   gy = mMiscFnx.enforce_limit(my4, 0, 99);



   // hx, hy in 0-1999 scale
   // the mouse position past the border width is how far we are into the scaled level background
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

   if (mx4<0 || mx4>1999 || my4<0 || my4>1999) pos_valid = false;

   // put in final variables with limit check
   hx = mMiscFnx.enforce_limit(mx4, 0, 1999);
   hy = mMiscFnx.enforce_limit(my4, 0, 1999);
}




void mwLevelEditor::process_keypress()
{
   if (mode == 4) mObjectViewer.ov_process_keypress();

   if (mInput.key[ALLEGRO_KEY_H][0]) set_mode(9);
   if (mInput.key[ALLEGRO_KEY_Q][3]) mBitmapTools.copy_tiles();
   if (mInput.key[ALLEGRO_KEY_F][3]) mBitmapTools.edit_tile_flags();
   if (mInput.key[ALLEGRO_KEY_E][3])
   {
      mwTileEditor te;
      te.edit_tile(0);
   }


   while (mInput.key[ALLEGRO_KEY_ESCAPE][0])
   {
      active = 0;
      mEventQueue.proc(1);
   }
   if (mEditorMain.level_editor_quit_confirmation_dialog && active == 0)
   {
      int ret = mMiscFnx.exit_level_editor_dialog();
      if (ret == 0) mLevel.save_level(mLevel.last_level_loaded); // save and exit
      if (ret == 2) active = 1; // cancel
   }
}


// when drawing from somewhere else, mode 0 is best,
// calling with this wrapper restores the original mode after
void mwLevelEditor::redraw_background(int temp_mode)
{
   int old_mode = mode;
   mode = temp_mode;
   redraw_background();
   mode = old_mode;
}

int mwLevelEditor::redraw_background()
{
   int drawn = 0;

   get_mouse_position_on_background();

   mEventQueue.proc(1);
   if (mEventQueue.program_update)
   {
      mEventQueue.program_update = 0;
      drawn = 1;

      al_flip_display();

      mColor.process_flash_color();
      mBitmap.update_animation();

      process_scrolledge();
      mDisplay.proc_scale_factor_change();

      mScreen.get_new_background(0);
      mLift.draw_lifts();
      mItem.draw_items();
      mEnemy.draw_enemies();

      if (mode == 1) mEditorMain.draw_level_editor_background_overlays(mWM.mouse_on_window);    // Editor Main
      if (mode == 2) mEditSelection.draw_level_editor_background_overlays(mWM.mouse_on_window); // Edit Selection
      if (mode == 3) mGroupEdit.draw_level_editor_background_overlays(mWM.mouse_on_window);     // Group Edit
      if (mode == 4) mObjectViewer.draw_level_editor_background_overlays(mWM.mouse_on_window);  // Object Viewer
      if (mode == 9) mTileHelper.draw_level_editor_background_overlays(mWM.mouse_on_window);    // Tile Helper
      if (mode) mScreen.draw_scaled_level_region_to_display();
   }
   return drawn;
}


// used by window class to draw while moving or resizing
// also used by:
// groupEdit drag reorder list
// level editor main menu, when blocking
void mwLevelEditor::redraw_callback()
{
   redraw_background(0);
   mScreen.draw_scaled_level_region_to_display();
   mWM.cycle_windows(1); // draw only
}

void mwLevelEditor::save_mW()
{
   // convert window vector to POD window array
   mwWindow w[20];
   for (int i = 0; i < (int)mWM.mW.size(); i++) w[i] = mWM.mW[i];

   FILE *fp = fopen("data/levelEditorWindowGeometry.pm", "wb");
   if (fp)
   {
      fwrite(&w, sizeof(w), 1, fp);
      fclose(fp);
   }
   else printf("error saving levelEditorWindowGeometry.pm\n");
}



void mwLevelEditor::load_mW()
{
   // load static window array
   mwWindow w[20];
   FILE *fp = fopen("data/levelEditorWindowGeometry.pm", "rb");
   if (fp)
   {
      fread(&w, sizeof(w), 1, fp);
      fclose(fp);

      // copy only these values back to window vector
      for (int i = 0; i < (int)mWM.mW.size(); i++)
      {
         mWM.mW[i].rect = w[i].rect;
         mWM.mW[i].active = w[i].active;
         mWM.mW[i].layer = w[i].layer;
      }
   }
   else printf("error loading levelEditorWindowGeometry.pm -- using defaults\n");

   // delete after loading so that if it crashes after loading, the next time it runs will be a clean slate
   al_remove_filename("data/levelEditorWindowGeometry.pm");
}

