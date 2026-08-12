// mwDemoRecord.cpp

#include "pm.h"
#include "mwDemoRecord.h"
#include "mwDemoMode.h"
#include "mwLoop.h"
#include "mwLevel.h"
#include "mwPlayer.h"
#include "mwGameMoves.h"
#include "mwInput.h"
#include "mwMiscFnx.h"
#include "mwEventQueue.h"
#include "mwDisplay.h"
#include "mwScreen.h"
#include "mwFont.h"
#include "mwColor.h"
#include "mwWidget.h"
#include "mwDrawSequence.h"
#include "mwBitmap.h"
#include "mwQuickGraph2.h"
#include "mwMenu.h"
#include "mwConfig.h"
#include "mwGmInfo.h"
#include "mwItem.h"

mwDemoRecord mDemoRecord;

void mwDemoRecord::init()
{

   mWM.mW[1].init(1, 0, 10, 10, 256, 276, 5, "Demo Record Main Controls", 1, 1, 14, 1);
   mWM.mW[1].redrawCallback = []() { mDemoRecord.redraw_callback(); };
   mWM.mW[1].drawFunction = [this]() { mDemoRecord.draw_mainW(mWM.mW[1]); };

   mWM.mW[2].init(2, 1, 800, 300, 329, 200, 15, "Moves", 1, 0, 14, 1);
   mWM.mW[2].set_resizeable(329, 329, 200, 1600);
   mWM.mW[2].redrawCallback = []() { mDemoRecord.redraw_callback(); };
   mWM.mW[2].drawFunction = [this]() { mDemoRecord.draw_GMList(mWM.mW[2]); };

   mWM.mW[3].init(3, 2, 500, 100, 320, 58, 8, "Transport Controls", 1, 1, 12, 1);
   mWM.mW[3].redrawCallback = []() { mDemoRecord.redraw_callback(); };
   mWM.mW[3].drawFunction = [this]() { mDemoRecord.draw_transport_controls(mWM.mW[3]); };
   mWM.mW[3].enable_X_button = 0;

   mWM.mW[4].init(4, 3, 500, 500, 300, 100, 8, "Timeline", 1, 0, 12, 1);
   mWM.mW[4].set_resizeable(200, 2000, 100, 800);
   mWM.mW[4].redrawCallback = []() { mDemoRecord.redraw_callback(); };
   mWM.mW[4].drawFunction = [this]() { mDemoRecord.draw_timeline(mWM.mW[4]); };

   mWM.mW[5].init(5, 4, 200, 400, 208, 136, 11, "Current Section", 1, 1, 14, 1);
   mWM.mW[5].redrawCallback = []() { mDemoRecord.redraw_callback(); };
   mWM.mW[5].drawFunction = [this]() { mDemoRecord.draw_current_section(mWM.mW[5]); };

   mWM.mW[6].init(6, 5, 100, 100, 208, 136, 9, "File Details", 1, 1, 14, 1);
   mWM.mW[6].redrawCallback = []() { mDemoRecord.redraw_callback(); };
   mWM.mW[6].drawFunction = [this]() { mDemoRecord.draw_file_details(mWM.mW[6]); };

   mWM.mW[7].init(7, 6, 100, 800, 164, 87, 6, "Range Tools", 1, 1, 14, 1);
   mWM.mW[7].redrawCallback = []() { mDemoRecord.redraw_callback(); };
   mWM.mW[7].drawFunction = [this]() { mDemoRecord.draw_range_tools(mWM.mW[7]); };

   set_timeline_colors(11, 14, 10, 8, 14, -4, 1);



}


void mwDemoRecord::set_timeline_colors(int level_done, int mouse_pos, int player_deaths, int purple_coins, int enemies_hit, int time_labels, int player_icons)
{
   timeline_level_done_color = level_done;
   timeline_mouse_pos_color = mouse_pos;
   timeline_player_deaths_color = player_deaths;
   timeline_purple_coins_color = purple_coins;
   timeline_enemy_hit_color = enemies_hit;
   timeline_display_time_labels = time_labels;
   timeline_show_player_icons = player_icons;
}




void mwDemoRecord::set_window_positions(int set)
{
   if (set == 1)
   {
      // all off
      //for (int i=2; i<9; i++) mDemoRecord.mWM.mW[i].active = 0;
      mWM.mW[2].active = 1; // Game Moves List
      mWM.mW[3].active = 1; // Transport Controls
      mWM.mW[4].active = 1; // Timeline
      mWM.mW[5].active = 1; // Current Section
      mWM.mW[6].active = 1; // File Details
      mWM.mW[7].active = 0; // Range Tools

      mWM.mW[1].set_pos(mWM.mW[6].rect.x2+2, 20);    // Main

      mWM.mW[6].set_pos(20, 20);                     // File Details
      mWM.mW[5].set_pos(20, mWM.mW[6].rect.y2 + 10); // Current Section
      mWM.mW[3].set_pos(20, mWM.mW[5].rect.y2 + 10); // Transport Controls
      mWM.mW[7].set_pos(20, mWM.mW[3].rect.y2 + 10); // Range Tools
      mWM.mW[8].set_pos(20, mWM.mW[7].rect.y2 + 10); // Record Settings
      mWM.mW[4].set_pos(20, mDisplay.SCREEN_H - mWM.mW[4].rect.h-20); // Timeline
      mWM.mW[2].set_pos(mDisplay.SCREEN_W - mWM.mW[2].rect.w-20, 20); // Game Moves List
   }

   if (set == 2)
   {
      // all off
      //for (int i=2; i<9; i++) mDemoRecord.mWM.mW[i].active = 0;
      mWM.mW[2].active = 1; // Game Moves List
      mWM.mW[3].active = 1; // Transport Controls
      mWM.mW[4].active = 1; // Timeline
      mWM.mW[5].active = 1; // Current Section
      mWM.mW[6].active = 1; // File Details
      mWM.mW[7].active = 0; // Range Tools

      mWM.mW[6].set_pos(20, 20);                     // File Details
      mWM.mW[5].set_pos(20, mWM.mW[6].rect.y2 + 10); // Current Section
      mWM.mW[1].set_pos(20, mWM.mW[5].rect.y2 + 10);    // Main

      //      mWM.mW[2].rect.setHeight(mDisplay.SCREEN_H - 40);
      mWM.mW[2].set_pos(20, mWM.mW[1].rect.y2 + 10);

      // Timeline
      mWM.mW[4].set_size(mDisplay.SCREEN_W/2, 120);
      mWM.mW[4].set_pos(20, mDisplay.SCREEN_H - mWM.mW[4].rect.h-20);

      // Transport Controls
      mWM.mW[3].set_pos(20, mWM.mW[4].rect.y1 - mWM.mW[3].rect.h - 40);

   }



/*
   // Game Moves List far right full height
   mWM.mW[2].rect.setHeight(mDisplay.SCREEN_H - 40);
   mWM.mW[2].set_pos(mDisplay.SCREEN_W - mWM.mW[2].rect.w-20, 20);

*/









}




void mwDemoRecord::draw_mainW(mwWindow w)
{
   int xa = w.rect.x1+4;
   int xb = w.rect.x2-4;
   int ya = w.rect.y1+14;
   int d = w.disable_input;
   int bts = 12;


   for (int i=2; i<8; i++)
      mWidget.mCheckBox(0, xa, xb,  1, ya + (i-2)*bts, bts, 0, mDemoRecord.mWM.mW[i].active, mDemoRecord.mWM.mW[i].title, 15, 15, d);
   ya+=6*bts;

   ya+=2; al_draw_line(w.rect.x1, ya, w.rect.x2, ya, mColor.pc[w.color], 1); ya+=4;

   std::vector<struct listItem> listItems =
   {
      {  0,  "Set Mode"  },
      {  1,  "Default" },
      {  2,  "All" }
   };

   int res = 0;
   if (mWidget.mDropDown(1, xa, 100,   1, ya, bts, 0, 0, 0, 0, 15, 10, listItems, res, d)) set_window_positions(res);
   ya += bts;

   ya+=4; al_draw_line(w.rect.x1, ya, w.rect.x2, ya, mColor.pc[w.color], 1); ya+=6;

   mWidget.togglec(xa, ya, xa+40, bts,  0,0,0,0,  0,0,0,0, 1,0,1,d, time_format,          "Time/Frame format", 15, 15);
   mWidget.togglec(xa, ya, xa+40, bts,  0,0,0,0,  0,0,0,0, 1,0,1,d, show_cpu_graph,       "Show cpu graph", 15, 15);
   mWidget.togglec(xa, ya, xa+40, bts,  0,0,0,0,  0,0,0,0, 1,0,1,d, show_player_grid,     "Show player grid", 15, 15);
   mWidget.togglec(xa, ya, xa+40, bts,  0,0,0,0,  0,0,0,0, 1,0,1,d, show_windows_in_play, "Show windows when playing", 15, 15);
   mWidget.togglec(xa, ya, xa+40, bts,  0,0,0,0,  0,0,0,0, 1,0,1,d, show_windows_in_rec,  "Show windows when recording", 15, 15);


   ya+=4; al_draw_line(w.rect.x1, ya, w.rect.x2, ya, mColor.pc[w.color], 1); ya+=6;

   bts = 16;

   mWidget.buttonp(xa, ya, xa+200, bts,  701,0,0,0,  0,w.color+128,15,0, 1,0,1,d, background_x_justify);
   mWidget.buttonp(xa, ya, xa+200, bts,  702,0,0,0,  0,w.color+128,15,0, 1,0,1,d, background_y_justify);



//   mWidget.buttonp(xa, ya, xb, bts, 51,n,0,0,  0,11,15,0, 1,0,1,d, mItem.item[n][12]); // exit link show






}


// popup menu to set player number
// used by:
// edit gm list to change player number in game move
void mwDemoRecord::change_player_num_menu(int & p)
{
   if (mInput.mouse_b[2][0])
   {
      mInput.mouse_y+=8;
      al_set_mouse_xy(mDisplay.display, mInput.mouse_x * mDisplay.display_transform_double, mInput.mouse_y * mDisplay.display_transform_double);
      sprintf(mMenu.menu_string[0],"P0");
      sprintf(mMenu.menu_string[1],"P1");
      sprintf(mMenu.menu_string[2],"P2");
      sprintf(mMenu.menu_string[3],"P3");
      sprintf(mMenu.menu_string[4],"P4");
      sprintf(mMenu.menu_string[5],"P5");
      sprintf(mMenu.menu_string[6],"P6");
      sprintf(mMenu.menu_string[7],"P7");
      sprintf(mMenu.menu_string[8],"end");
      int mp = mMenu.pmenu(5, 13, -4, 99);
      if ((mp >= 0) && (mp < 8)) p = mp;
      mInput.mouse_y-=8;
      al_set_mouse_xy(mDisplay.display, mInput.mouse_x * mDisplay.display_transform_double, mInput.mouse_y * mDisplay.display_transform_double);
   }
}


// time formatted string
// pass it frame number and a string to write on
// it will return the string formatted with either:
// if (time_format) - integer frame number
// or               - mm:ss
char * mwDemoRecord::gettf(int frame, char* ft)
{
   if (time_format) sprintf(ft, "%d", frame);
   else             sprintf(ft, "%s", mMiscFnx.chrms(frame, ft));
   return ft;
}

int mwDemoRecord::load_demo_record(bool get_new)
{
   // check if  current_loaded_demo_file exists and try to load it

   if (get_new)
   {
      if (!mGameMoves.load_gm_file_select()) return 0;
   }
   else // try to load current_loaded_demo_file
   {
      if (!mGameMoves.load_gm(last_loaded_demo_file, 1))
         if (!mGameMoves.load_gm_file_select()) return 0;
   }

   mLoop.frame_num = 0;

   timeline_last_frame = mDemoMode.last_frame;

   // printf("timeline_last_frame:%d\n", timeline_last_frame);

   refresh();
   set_active_section(0);
   return 1;
}

void mwDemoRecord::refresh()
{
   int old_frame_num = mLoop.frame_num;
   mGmInfo.fill();
   load_lnk_arr();    // load games moves into link array
   mDemoMode.seek_to_frame(old_frame_num, 1);
}

void mwDemoRecord::reload()
{
   int old_frame_num = mLoop.frame_num;
   mGameMoves.load_gm(last_loaded_demo_file);
   mLoop.frame_num = old_frame_num;
   refresh();
}


void mwDemoRecord::proc_cpu_time(double frame_start_timestamp)
{
   // --------------------------------------------
   // measure time to process loop
   // --------------------------------------------
   double pt = al_get_time() - frame_start_timestamp;

   // convert to 'cpu', a percent of the timer frame period
   double frame_period = 1 / (double) mLoop.frame_speed;
   float cpu = (pt / frame_period) * 100;

   // store in local cpu variables
   mRollingAverage[0].add_data(cpu);
   mQuickGraph2[9].add_data(0, mRollingAverage[0].max, 0);
   mQuickGraph2[9].new_entry_pos();

   if (show_cpu_graph)
   {
      mQuickGraph2[9].set_pos(mDisplay.SCREEN_W-mQuickGraph2[9].width-28, mDisplay.SCREEN_H-mQuickGraph2[9].height-20);
      al_set_target_backbuffer(mDisplay.display);
      mQuickGraph2[9].draw_graph();
   }

}


void mwDemoRecord::demo_record()
{
   init();
   if (!load_mWM()) set_window_positions(2);



   double frame_start_timestamp;
   mQuickGraph2[9].initialize(200,    36,    0,   50, "CPU",      9, 12, 13, 1);
   mLoop.frame_num = 0;

   record = 0;
   play = 0;
   current_section = 0;

   int quit = 0;

   if (!load_demo_record(0)) quit = 1;


   while (!quit)
   {
     mEventQueue.proc(1);

      if (mEventQueue.program_update)
      {
         mEventQueue.program_update = 0;
         frame_start_timestamp = al_get_time();

         mDisplay.proc_scale_factor_change();
         if (mLoop.frame_speed < 40) mDisplay.proc_scale_factor_change();
         if (mLoop.frame_speed < 30) mDisplay.proc_scale_factor_change();
         if (mLoop.frame_speed < 20) mDisplay.proc_scale_factor_change();


         if (mLoop.frame_num > timeline_last_frame) timeline_last_frame = mLoop.frame_num;


         // punch in
         if ((!record) && (record_punch_in_armed))
         {
            mPlayer.set_comp_move_from_player_key_check(0);
            if (mPlayer.loc[0].comp_move) start_record();
         }

         if (play)
         {
            mLoop.frame_num++;

            mBitmap.update_animation();
            mPlayer.proc_player_input();
            mGameMoves.proc();
            mLoop.move_frame();
            mDrawSequence.ds_draw(0, 0);
         }
         else
         {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            mScreen.draw_scaled_level_region_to_display();
            mScreen.draw_screen_overlay();
            if (show_player_grid) mScreen.drg_show(mDisplay.SCREEN_W-130, mDisplay.SCREEN_H-200);
         }
         if (show_cpu_graph) proc_cpu_time(frame_start_timestamp);
      }


      al_set_target_backbuffer(mDisplay.display);

      // set transport pos and size to dock above timeline
      mWM.mW[3].rect.setXYWH(mWM.mW[4].rect.x1, mWM.mW[4].rect.y1 - mWM.mW[3].rect.h, mWM.mW[4].rect.w, mWM.mW[3].rect.h);


      int show_windows = 1;
      if (play && !show_windows_in_play) show_windows = 0;
      if (record && !show_windows_in_rec) show_windows = 0;
      if (show_windows) mWM.cycle_windows(0);


      // toggle for main window in top left corner
      int ya = 1;
      mWidget.togglec(-1, ya, 9, 10,  0,0,0,0,  0,0,0,0, 1,0,0,0, mWM.mW[1].active, "", 15, 15);





      al_flip_display();

      if (mInput.key[ALLEGRO_KEY_ESCAPE][0])
      {
         while (mInput.key[ALLEGRO_KEY_ESCAPE][0]) mEventQueue.proc(1);
         if (play) stop_transport();
         else quit = 1;
      }
   }

   save_mWM();


   mPlayer.active_local_player = 0;
   mConfig.load_config(); // to restore colors and other settings

   mLoop.state[0] = PM_PROGRAM_STATE_MENU;
   mLoop.quit_action = 1; // menu

}

void mwDemoRecord::redraw_callback()
{
   mEventQueue.proc(1);
   al_clear_to_color(al_map_rgb(0, 0, 0));
   mScreen.draw_scaled_level_region_to_display();
   mScreen.draw_screen_overlay();
   // set transport position and size to dock above timeline
   if (mWM.mW[4].moving) mWM.mW[3].rect.setXYWH(mWM.mW[4].rect.x1, mWM.mW[4].rect.y1 - mWM.mW[3].rect.h, mWM.mW[4].rect.w, mWM.mW[3].rect.h);
   // set timeline position to dock below timeline
   if (mWM.mW[3].moving) mWM.mW[4].rect.setX1Y1(mWM.mW[3].rect.x1, mWM.mW[3].rect.y2);

   mWM.cycle_windows(1);
   al_flip_display();
}



void mwDemoRecord::save_mWM()
{
   // convert window vector to POD window array
   mwWindow w[10];

   // copy only these values
   for (int i=0; i<(int)mWM.mW.size(); i++)
   {
      w[i].rect   = mWM.mW[i].rect;
      w[i].active = mWM.mW[i].active;
      w[i].layer  = mWM.mW[i].layer;
   }

   FILE *fp = fopen("data/demoRecordWindowGeometry.pm", "wb");
   if (fp)
   {
      fwrite(&w,                             sizeof(w),                             1, fp);
      fwrite(&timeline_level_done_color,     sizeof(timeline_level_done_color),     1, fp);
      fwrite(&timeline_mouse_pos_color,      sizeof(timeline_mouse_pos_color),      1, fp);
      fwrite(&timeline_player_deaths_color,  sizeof(timeline_player_deaths_color),  1, fp);
      fwrite(&timeline_purple_coins_color,   sizeof(timeline_purple_coins_color),   1, fp);
      fwrite(&timeline_enemy_hit_color,      sizeof(timeline_enemy_hit_color),      1, fp);
      fwrite(&timeline_display_time_labels,  sizeof(timeline_display_time_labels),  1, fp);
      fwrite(&timeline_show_player_icons,    sizeof(timeline_show_player_icons),    1, fp);
      fwrite(&time_format,                   sizeof(time_format),                   1, fp);
      fwrite(&gm_list_simple,                sizeof(gm_list_simple),                1, fp);

      fwrite(&background_x_justify,          sizeof(background_x_justify),          1, fp);
      fwrite(&background_y_justify,          sizeof(background_y_justify),          1, fp);


      fwrite(&last_loaded_demo_file,         sizeof(last_loaded_demo_file),         1, fp);


      fclose(fp);
   }
   else printf("error saving demoRecordWindowGeometry.pm\n");
}


bool mwDemoRecord::load_mWM()
{
   // load static window array
   mwWindow w[10];
   FILE *fp = fopen("data/demoRecordWindowGeometry.pm", "rb");
   if (fp)
   {
      fread(&w,                             sizeof(w),                             1, fp);
      fread(&timeline_level_done_color,     sizeof(timeline_level_done_color),     1, fp);
      fread(&timeline_mouse_pos_color,      sizeof(timeline_mouse_pos_color),      1, fp);
      fread(&timeline_player_deaths_color,  sizeof(timeline_player_deaths_color),  1, fp);
      fread(&timeline_purple_coins_color,   sizeof(timeline_purple_coins_color),   1, fp);
      fread(&timeline_enemy_hit_color,      sizeof(timeline_enemy_hit_color),      1, fp);
      fread(&timeline_display_time_labels,  sizeof(timeline_display_time_labels),  1, fp);
      fread(&timeline_show_player_icons,    sizeof(timeline_show_player_icons),    1, fp);
      fread(&time_format,                   sizeof(time_format),                   1, fp);
      fread(&gm_list_simple,                sizeof(gm_list_simple),                1, fp);
      fread(&background_x_justify,          sizeof(background_x_justify),          1, fp);
      fread(&background_y_justify,          sizeof(background_y_justify),          1, fp);


      fread(&last_loaded_demo_file,         sizeof(last_loaded_demo_file),         1, fp);


      fclose(fp);

      // copy only these values back to window vector
      for (int i=0; i<(int)mWM.mW.size(); i++)
      {
         mWM.mW[i].rect   = w[i].rect;
         mWM.mW[i].active = w[i].active;
         mWM.mW[i].layer  = w[i].layer;
      }
      // delete after loading so that if it crashes after loading, the next time it runs will be a clean slate
      al_remove_filename("data/demoRecordWindowGeometry.pm");
      return true;
   }
   printf("error loading demoRecordWindowGeometry.pm -- using defaults\n");
   return false;

}




















