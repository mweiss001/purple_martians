// mwEventQueue.cpp

#include "pm.h"
#include "mwEventQueue.h"
#include "mwDisplay.h"
#include "z_player.h"
#include "mwInput.h"
#include "mwProgramState.h"
#include "z_control.h"
#include "z_main.h"



mwEventQueue mwEQ;

mwEventQueue::mwEventQueue()
{
   initialize();
}

void mwEventQueue::initialize(void)
{

}


void mwEventQueue::set_speed(void)
{
   al_set_timer_speed(mwEQ.fps_timer, 1/(float)mwPS.frame_speed);
}




void mwEventQueue::create_timers(void)
{
   // --- timers ---------------------
   // create timers
   fps_timer = al_create_timer(1 / (float) mwPS.frame_speed);
   sec_timer = al_create_timer(1);    // 1s
   mnu_timer = al_create_timer(.008); // 125 fps
   png_timer = al_create_timer(.5);   // 2 fps
   mou_timer = al_create_timer(5);    // 5s

   // register timer event source
   al_register_event_source(event_queue, al_get_timer_event_source(mnu_timer));
   al_register_event_source(event_queue, al_get_timer_event_source(fps_timer));
   al_register_event_source(event_queue, al_get_timer_event_source(sec_timer));
   al_register_event_source(event_queue, al_get_timer_event_source(png_timer));
   al_register_event_source(event_queue, al_get_timer_event_source(mou_timer));

   // start timers
   al_start_timer(fps_timer);
   al_start_timer(sec_timer);
   al_start_timer(mnu_timer);
   al_start_timer(png_timer);
   al_start_timer(mou_timer);


}











void mwEventQueue::proc_events(ALLEGRO_EVENT ev)
{
   if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE) mwD.proc_display_change();
   if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) fast_exit(0);
   if (ev.type == ALLEGRO_EVENT_TIMER)
   {
      if (ev.timer.source == fps_timer) program_update = 1;
      if (ev.timer.source == sec_timer) program_update_1s = 1;
      if (ev.timer.source == png_timer) players1[active_local_player].client_ping_flag = 1;
      if (ev.timer.source == mou_timer) al_hide_mouse_cursor(display);
      if (ev.timer.source == mnu_timer) menu_update = 1;
   }
   mI.proc_input_events(ev);  // send all other events to input handler

}

void mwEventQueue::proc_event_queue(void)
{
   mI.key[ALLEGRO_KEY_PRINTSCREEN][0] = 0; // hack to make PRINTSCREEN key work properly
   mI.key_pressed_ASCII = 0;

   while (!al_is_event_queue_empty(event_queue))
   {
      ALLEGRO_EVENT ev;
      al_get_next_event(event_queue, &ev);
      proc_events(ev);
   }
   mI.proc_keys_held();
   mI.function_key_check();
}

void mwEventQueue::proc_event_queue_menu(void)
{
   proc_event_queue();

   // this is done so that the game controls can be used to navigate menus and visual level select
   clear_controls(active_local_player);
   set_controls_from_player_key_check(active_local_player);
}


