// mwEventQueue.h

class mwEventQueue
{
   public:

   mwEventQueue(); // default constructor
   void initialize(void);

   void create_timers(void);

   int program_update;
   int program_update_1s;
   int menu_update;

   void set_speed(void);

   void proc_events(ALLEGRO_EVENT ev);
   void proc_event_queue(void);
   void proc_event_queue_menu(void);
   ALLEGRO_EVENT_QUEUE *event_queue;


   ALLEGRO_TIMER * fps_timer;  // used to control the speed of the game
   ALLEGRO_TIMER * mnu_timer;  // used to control the speed of the menu
   ALLEGRO_TIMER * sec_timer;  // used to count the actual frames per second
   ALLEGRO_TIMER * png_timer;  // used to control the speed client pings
   ALLEGRO_TIMER * mou_timer;  // used to control hiding the mouse cursor


};
extern mwEventQueue mwEQ;



