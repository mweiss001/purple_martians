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


   void find_event_txrs_for_event(int ev, int &evan, int eva[][2]);
   void find_event_rxrs_for_event(int ev, int &evan, int eva[][2]);


   void find_and_show_event_links(int obj_type, int obj_num, int obj_ext);
   int is_mouse_on_trigger_item(int &obt_type, int &obj_num, int &obj_ext);
   int get_trigger_item(int obj_type, int obj_num, int obj_ext, int& ti_obj_type, int& ti_obj_num, int& ti_obj_ext);
   void set_event_num_in_sender(int obj_type, int obj_num, int obj_ext, int tog, int ev);
   void find_event_sender_for_obj(int obj_type, int obj_num, int obj_ext, int obj_ext2);

};
extern mwTriggerEvent mTriggerEvent;



