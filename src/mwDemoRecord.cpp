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
   mWM.mW[1].index = 1;
   mWM.mW[1].layer = 0;
   mWM.mW[1].active = 1;
   mWM.mW[1].resizable = 1;
   mWM.mW[1].min_width = 100;
   mWM.mW[1].max_width = 200;
   mWM.mW[1].min_height = 200;
   mWM.mW[1].max_height = 400;
   mWM.mW[1].set_pos(1000, 100);
   mWM.mW[1].set_size(320, 53);
   mWM.mW[1].set_title("Status Window");
   mWM.mW[1].redrawCallback = []() { mDemoRecord.redraw_callback(); };

   mWM.mW[2].index = 2;
   mWM.mW[2].layer = 1;
   mWM.mW[2].active = 1;
   mWM.mW[2].resizable = 1;
   mWM.mW[2].min_width = 329;
   mWM.mW[2].max_width = 329;
   mWM.mW[2].min_height = 200;
   mWM.mW[2].max_height = 800;
   mWM.mW[2].set_pos(800, 300);
   mWM.mW[2].set_size(329, 200);
   mWM.mW[2].set_title("Game Moves List");
   mWM.mW[2].redrawCallback = []() { mDemoRecord.redraw_callback(); };
   mWM.mW[2].drawFunction = [this]() { mDemoRecord.draw_GMList(mWM.mW[2].rect, mWM.mW[2].disable_input, mWM.mW[2].have_focus); };

   mWM.mW[3].index = 3;
   mWM.mW[3].layer = 2;
   mWM.mW[3].active = 1;
   mWM.mW[3].set_pos(500, 100);
   mWM.mW[3].set_size(192, 82);
   mWM.mW[3].set_title("Transport Controls");
   mWM.mW[3].redrawCallback = []() { mDemoRecord.redraw_callback(); };
   mWM.mW[3].drawFunction = [this]() { mDemoRecord.draw_transport_controls(mWM.mW[3].rect, mWM.mW[3].disable_input, mWM.mW[3].have_focus); };


   mWM.mW[4].index = 4;
   mWM.mW[4].layer = 3;
   mWM.mW[4].active = 1;
   mWM.mW[4].resizable = 1;
   mWM.mW[4].min_width = 200;
   mWM.mW[4].max_width = 1000;
   mWM.mW[4].min_height = 100;
   mWM.mW[4].max_height = 800;
   mWM.mW[4].set_pos(500, 500);
   mWM.mW[4].set_size(300, 100);
   mWM.mW[4].set_title("Timeline Controls");
   mWM.mW[4].redrawCallback = []() { mDemoRecord.redraw_callback(); };
   mWM.mW[4].drawFunction = [this]() { mDemoRecord.draw_timeline(mWM.mW[4].rect, mWM.mW[4].disable_input, mWM.mW[4].have_focus); };

}


void mwDemoRecord::draw_timeline(mwRect<int> rect, int disable_input, int have_focus)
{
   int c = 13;
   rect.clear_frame_title(mColor.pc[0], mColor.pc[c], mColor.pc[15], 12, ALLEGRO_ALIGN_CENTER, mFont.pr8, "Timeline");

   // size adjust rect
   mwRect<int> size_adj_rect = rect;
   size_adj_rect.setWH_adj_X1Y1(10, 10);

   int triangle_color = c;
   if (size_adj_rect.contains(mInput.mouse_x, mInput.mouse_y)) triangle_color = 14;
   al_draw_filled_triangle(rect.x2-10, rect.y2, rect.x2, rect.y2, rect.x2, rect.y2-10, mColor.pc[triangle_color]);

   // timeline rect
   mwRect<int> timeline_rect = mwRect<int>::fromX1Y1X2Y2(rect.x1, rect.y1+14, rect.x2-10, rect.y2 );
   int num_tracks = mGmInfo.gmPlayerInfo.size();
   int ls = (timeline_rect.h / num_tracks); // line size
   int bts = ls - 2;
   mScreen.draw_demo_controls_overlay_timeline_tracks(timeline_rect.x1, timeline_rect.x2, timeline_rect.y1, bts, 2, 0, 0, timeline_rect.w, timeline_rect.h, 1, disable_input);
}


// popup menu to set player number
// used by:
// transport controls to set punch in player
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


char * mwDemoRecord::gettf(int frame, char* ft)
{
   if (tm_frame) sprintf(ft, "%d", frame);
   else sprintf(ft, "%s", mMiscFnx.chrms(frame, ft));
   return ft;
}



int mwDemoRecord::load_demo_record()
{
//   if (!mGameMoves.load_gm_file_select()) return 0;
//   if (!mGameMoves.load_demo_level(5)) return 0;
   if (!mGameMoves.load_gm("C:/pm/savegame/demo/TAS/lev016-8P-115.gm", 1)) return 0;

   current_frame_num = 1;
   refresh();
   set_active_section(0);
   return 1;
}

void mwDemoRecord::refresh()
{
//   fill_player_sections();
   mGmInfo.fill();
   load_lnk_arr();    // load games moves into link array
   //get_more_player_section_info();
   //find_ack_moves(0);
   seek_to_frame(current_frame_num, 1);
}

void mwDemoRecord::reload()
{
   mGameMoves.load_gm(current_loaded_demo_file);
   refresh();
}




int mwDemoRecord::draw_record_settings(int x1, int y1)
{
   int col = 10;

   int w = 148;
   //w +=  mLoop.pct_x;
   int x2 = x1 + w;

   int h = 88;
   //h +=  mLoop.pct_y;
   int y2 = y1 + h;

   al_draw_filled_rectangle(x1, y1, x2, y2, mColor.pc[0]);
   al_draw_rectangle(x1, y1, x2, y2, mColor.pc[col], 1);
   al_draw_line(x1, y1+12, x2, y1+12, mColor.pc[col], 1);
   al_draw_textf(mFont.pr8, mColor.pc[15], (x1+w/2), y1+2, ALLEGRO_ALIGN_CENTER, "Record Settings");
   y1+=14;

   int xa = x1+4;
   int xb = x2-4;

   int oldrpn = record_player_number;
   mWidget.slideri(xa, y1, xb, 16,  0,0,0,0,  0,12,15,15,  0,0,1,0, record_player_number, 7, 0, 1, "Player Number:");
   if (oldrpn != record_player_number)
   {
      mPlayer.active_local_player = record_player_number;
      record_player_color = mPlayer.syn[record_player_number].color;
   }

   int cl = mWidget.colsel(xa, y1, xb, 16,  10,0,0,0,  0,12,15,15,  0,0,1,0);
   if (cl != -1) record_player_color = cl;

   al_draw_bitmap(mBitmap.player_tile[record_player_color][1], xa, y1, 0 );
   al_draw_text(mFont.pr8, mColor.pc[record_player_color], xa+22, y1+7, 0, mColor.color_name[record_player_color]);

   y1+=24;

   al_draw_line(x1, y1, x2, y1, mColor.pc[col], 1);

   y1+=2;

   mWidget.toggle(x1+4, y1, x2-4, 16,   0,0,0,0,  0,0,0,0, 1,0,1,0, record_punch_in_armed, "Arm", "Armed", 15, 15, 9, 10);

   return y1;
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

   if (sh_cpu_graph)
   {
      mQuickGraph2[9].set_pos(mDisplay.SCREEN_W-mQuickGraph2[9].width-28, mDisplay.SCREEN_H-mQuickGraph2[9].height-20);
      al_set_target_backbuffer(mDisplay.display);
      mQuickGraph2[9].draw_graph();
   }

}

int mwDemoRecord::draw_overlay(void)
{
   if ((sh_overlay_in_play) && (play)) return 0;
   if ((sh_overlay_in_rec) && (record)) return 0;

   // 1st column
   int xa = 14;
   int ya = 14;
   ya = draw_file_details(xa, ya) + 1;
   if (sh_section_details) ya = draw_section_details(xa, ya) + 1;
   if (sh_rec_controls) ya = draw_record_settings(xa, ya) + 28;
   if (sh_range_tools) ya = draw_range_tools(xa, ya) + 28;

   // 2nd column - checkbox settings
   int xa2 = xa + 226;
   int ya2 = 32;
   int bts = 10;

   al_draw_filled_rectangle(xa2, ya2, xa2+200, ya2+100, mColor.pc[0]);

   mWidget.togglec(xa2, ya2, xa2+40, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, tm_frame,           "Time/Frame format", 15, 15);

//   ya2+=2;

   mWidget.togglec(xa2, ya2, xa2+40, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, sh_gamemoves_list,  "Show game moves list", 15, 15);

   mWidget.togglec(xa2, ya2, xa2+40, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, sh_timeline,        "Show timeline", 15, 15);

   mWidget.togglec(xa2, ya2, xa2+40, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, sh_section_details, "Show section details", 15, 15);
   mWidget.togglec(xa2, ya2, xa2+40, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, sh_rec_controls,    "Show record controls", 15, 15);

   mWidget.togglec(xa2, ya2, xa2+40, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, sh_cpu_graph,       "Show cpu graph", 15, 15);
   mWidget.togglec(xa2, ya2, xa2+40, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, sh_player_grid,     "Show player grid", 15, 15);

   mWidget.togglec(xa2, ya2, xa2+40, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, sh_range_tools,     "Show range tools", 15, 15);

   mWidget.togglec(xa2, ya2, xa2+40, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, sh_overlay_in_play, "Hide overlay when playing", 15, 15);
   mWidget.togglec(xa2, ya2, xa2+40, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, sh_overlay_in_rec,  "Hide overlay when recording", 15, 15);

   // debug player grid
   if (sh_player_grid) mScreen.drg_show(mDisplay.SCREEN_W-130, mDisplay.SCREEN_W-200);

   // // game moves list
   // if (sh_gamemoves_list) ;
   //
   // // timeline
   // if (sh_timeline) ;

   return 0;

}




void mwDemoRecord::demo_record(void)
{
   init();
   double frame_start_timestamp;
   mQuickGraph2[9].initialize(200,    36,    0,   50, "CPU",      9, 12, 13, 1);
   mLoop.frame_num = 0;

   record_mode_active = 1;
   record_player_number = 0;
   record_player_color = 8;
   record = 0;
   play = 0;

   current_section = 0;

   int quit = 0;

   if (!load_demo_record()) quit = 1;

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

         if (play)
         {
            mLoop.frame_num++;
            current_frame_num = mLoop.frame_num;
            mBitmap.update_animation();
            mPlayer.proc_player_input();
            mGameMoves.proc();
            mLoop.move_frame();
            mDrawSequence.ds_draw(0, 0);

            if ((!record) && (record_punch_in_armed))
            {
               mPlayer.set_comp_move_from_player_key_check(0);
               if (mPlayer.loc[0].comp_move) start_record();
            }
         }
         else
         {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            mScreen.draw_scaled_level_region_to_display();
            mScreen.draw_screen_overlay();
         }
         proc_cpu_time(frame_start_timestamp);
      }

      al_set_target_backbuffer(mDisplay.display);
      quit = draw_overlay();

      mWM.cycle_windows(0);

      al_flip_display();

      if (mInput.key[ALLEGRO_KEY_ESCAPE][0])
      {
         while (mInput.key[ALLEGRO_KEY_ESCAPE][0]) mEventQueue.proc(1);
         if (play) stop_transport();
         else quit = 1;
      }
   }

   mPlayer.active_local_player = 0;
   mConfig.load_config(); // to restore colors and other settings

   mLoop.state[0] = PM_PROGRAM_STATE_MENU;
   mLoop.quit_action = 1; // menu

   record_mode_active = 0;
}

void mwDemoRecord::redraw_callback()
{
   mEventQueue.proc(1);
   al_clear_to_color(al_map_rgb(0, 0, 0));
   mScreen.draw_scaled_level_region_to_display();
   mScreen.draw_screen_overlay();
   mWM.cycle_windows(1);
   al_flip_display();
}
