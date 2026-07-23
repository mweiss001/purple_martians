// mwDemoRecord.h
#include "mwWindowManager.h"
class mwDemoRecord
{
   public:
   mwWindowManager mWM = mwWindowManager(9);

   void draw_mainW(mwWindow w);
   void draw_GMList(mwWindow w);
   void draw_transport_controls(mwWindow w);
   void draw_timeline(mwWindow w);
   void draw_current_section(mwWindow w);
   void draw_file_details(mwWindow w);
   void draw_range_tools(mwWindow w);
   void draw_record_settings(mwWindow w);


   void draw_transport_controls_seek(int fx1, int fy1, int fy2, int d);

   void draw_transport_controls_seek2(mwRect<int> rect, int d);



   void draw_transport_controls_speed(int fx1, int fy1, int bts, int ls, int d);

   void sh(bool inc_dec, float per);



   void set_window_positions(int set);

   int record_mode_active{};

   int lnk_arr[10000] = {0};
   int lnk_entry_pos = 0;

   char current_loaded_demo_file[256];

   int fhs[10000][4] = {0};
   int fhs_indx = 0;

   int play = 0;
   int record = 0;
   int record_punch_in_armed = 0;

   int current_section = 0;

   int tm_frame = 1;

   int show_cpu_graph = 0;
   int show_player_grid = 0;
   int show_windows_in_play = 1;
   int show_windows_in_rec = 0;

   int gm_list_all = 0;
   int gm_list_mono = 0;

   int rt_start_frame = 0;
   int rt_end_frame = 0;
   int rt_all_players = 0;


   void gdt();
   char * gettf(int frame, char* ft);


private:
   void seek_to_frame(int frame, int draw);
public:

   void init();

   void redraw_callback();

   void demo_record();
   int load_demo_record();
   void refresh();
   void reload();

   void start_record();
   void stop_transport();

   void set_active_section(int i);

   void range_tools_set_fire_moves(int f, int clear_set);
   void range_tools_do(int action);
   void proc_range_tools_menu();

   void proc_gm_list_menu(int gi, int x, int y);
   void edit_gm(int gi, int x, int y);
   void copy_ptp(int sp, int dp);
   void proc_edit_gm_type_menu(int & t);
   void change_player_num_menu(int & p);

   void find_fire_held_sections(int p);
   void mark_player_carry(int p);
   void mark_player_shot_used(int p, int f, int type);
   void remove_unused_fire_held_sections(int test);

   void proc_file_details_menu();

   void load_lnk_arr();
   void proc_cpu_time(double frame_start_timestamp);
   void proc_section_details_menu();

   void proc_transport_menu();


   void save_mWM();
   void load_mWM();



};
extern mwDemoRecord mDemoRecord;



