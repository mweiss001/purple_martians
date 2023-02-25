// mwTriggerEvent.h

class mwTriggerEvent
{
   public:

   int event[1000];

   mwTriggerEvent(); // default constructor
   void initialize(void);


   void show_event_line(int x, int &y, int ev, int type, int v1, int v2);
   void show_all_events(void);
   int add_item_link_translation(int sel_item_num, int sel_item_var, int sel_item_ev, int clt[][4], int clt_last);
   int check_clt_for_event(int ev, int clt[][4], int clt_last);
   int is_pm_event_used(int ev);
   int get_unused_pm_event_extended(int clt[][4], int clt_last);
   int get_unused_pm_event(void);
   void find_and_show_event_links(int type, int i, int num2);




};
extern mwTriggerEvent mTriggerEvent;



