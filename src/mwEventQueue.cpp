// mwEventQueue.cpp

#include "pm.h"
#include "mwEventQueue.h"
#include "mwDisplay.h"
#include "mwPlayer.h"
#include "mwInput.h"
#include "mwLoop.h"
#include "mwMain.h"

mwEventQueue mEventQueue;

void mwEventQueue::create_timers(void)
{
   // create timers
   fps_timer = al_create_timer(1 / (float) mLoop.frame_speed);
   mnu_timer = al_create_timer(.008); // 125 fps
   sec_timer = al_create_timer(1);    // 1s
   mou_timer = al_create_timer(1);    // 1s

   // register timer event source
   al_register_event_source(event_queue, al_get_timer_event_source(fps_timer));
   al_register_event_source(event_queue, al_get_timer_event_source(mnu_timer));
   al_register_event_source(event_queue, al_get_timer_event_source(sec_timer));
   al_register_event_source(event_queue, al_get_timer_event_source(mou_timer));

   // start timers
   al_start_timer(fps_timer);
   al_start_timer(mnu_timer);
   al_start_timer(sec_timer);
   al_start_timer(mou_timer);
}

void mwEventQueue::proc_events(ALLEGRO_EVENT ev)
{
   if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE) mDisplay.proc_display_change();
   if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) mMain.fast_exit();
   if (ev.type == ALLEGRO_EVENT_TIMER)
   {
      if (ev.timer.source == fps_timer) program_update = 1;
      if (ev.timer.source == sec_timer) program_update_1s = 1;
      if (ev.timer.source == mnu_timer) menu_update = 1;
      if (ev.timer.source == mou_timer)
      {
         if (mLoop.level_editor_running) al_set_timer_count(mou_timer, 0);
         if (al_get_timer_count(mou_timer) > 2) al_hide_mouse_cursor(mDisplay.display);
      }
   }
   mInput.proc_input_events(ev);  // send all other events to input handler
}

void mwEventQueue::proc(int pch)
{
   mInput.key[ALLEGRO_KEY_PRINTSCREEN][0] = 0; // hack to make PRINTSCREEN key work properly
   mInput.key_pressed_ASCII = 0;

   while (!al_is_event_queue_empty(event_queue))
   {
      ALLEGRO_EVENT ev;
      al_get_next_event(event_queue, &ev);
      proc_events(ev);
   }
   if (pch) mInput.proc_keys_held();
//   printf("f:%d mt:%d\n", mLoop.frame_num, al_get_timer_count(mEventQueue.mou_timer));
}

void mwEventQueue::proc_menu(void)
{
   proc(1);
   // this is done so that the game controls can be used to navigate menus and visual level select
   mPlayer.clear_controls(mPlayer.active_local_player);
   mPlayer.set_controls_from_player_key_check(mPlayer.active_local_player);
}



void mwEventQueue::adjust_fps_timer(int new_fps)
{
   mLoop.frame_speed = new_fps;
   al_set_timer_speed(mEventQueue.fps_timer, (1 / (double) mLoop.frame_speed));
}


void mwEventQueue::reset_fps_timer(void)
{
   mLoop.frame_speed = 40;
   al_set_timer_speed(mEventQueue.fps_timer, (1 / (double) mLoop.frame_speed));
}


















