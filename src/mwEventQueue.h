// mwEventQueue.h

class mwEventQueue
{
   public:
   ALLEGRO_EVENT_QUEUE *event_queue;

   ALLEGRO_TIMER * fps_timer;  // used to control the speed of the game
   ALLEGRO_TIMER * mnu_timer;  // used to control the speed of the menu
   ALLEGRO_TIMER * sec_timer;  // used to do things on a 1 Hz schedule
   ALLEGRO_TIMER * mou_timer;  // used to control hiding the mouse cursor

   void create_timers(void);

   int program_update;
   int program_update_1s;
   int menu_update;

   void proc(int pch);
   void proc_menu(void);

   void adjust_fps_timer(int new_fps);
   void reset_fps_timer(void);

   private:
   void proc_events(ALLEGRO_EVENT ev);

};
extern mwEventQueue mEventQueue;



