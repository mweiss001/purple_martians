// mwLevel.h

#ifndef PM_MWLEVEL_H
#define PM_MWLEVEL_H
#include "mwEnemy.h"
#include "mwItem.h"
#include "mwLift.h"


struct level_data
{
   char level_name[24];

   int area;
   int status;

   int min_respawns;
   int max_coins_collected;
   int tot_coins;
   int min_enemies_left;
   int min_enemies_left_par;
   int max_enemies_killed;

   int times_played;
   int times_beat;
   int times_quit;

   int time_par;
   int time_best;
   int time_best_all_coins;
   int time_worst;
   int time_average;
   int time_total;
};


struct overworld_area
{
   char name[24];
   int color;
   int visible;

   int x;
   int y;
   int w;
   int h;
};




class mwLevel
{
   public:

   int l[100][100];

   mwItem mItem;
   mwEnemy mEnemy;
   mwLift mLift;

   int draw_level3(ALLEGRO_BITMAP *b, int level, int x, int y, int size);

   int start_level;
   int play_level;

   int valid_level_loaded;
   int prev_level_loaded;
   int last_level_loaded;
   int resume_allowed;

   int number_of_starts;


   void change_block(int x, int y, int block);
   int is_block_empty(int x, int y, int test_block, int test_item, int test_enemy);

   void set_start_level();
   void set_start_level(int s);

   int get_next_level(int lev, int max_lev, int wrap);
   int get_prev_level(int lev, int max_lev, int wrap);


   int load_level_file_dialog();
   int load_level_visual_select();
   int load_level_by_number(int level_to_load, int load_only, int fail_silently);
   int load_level(const char* filename, int load_only, int fail_silently);



   void save_level(int level_to_save);
   int save_level_prompt();


   // levelDataStats
   // -----------------------------

   ALLEGRO_BITMAP * level_stats_bitmap = NULL;
   int level_stats_bmp_msg_type = 0;
   int level_stats_bmp_w;
   int level_stats_bmp_h;
   int level_stats_show_level_number = 0;


   int vline[20] = { 0 };
   int vli = 0;
   int tally[2][16] = { 0 };

   int row_type = 0;


   void show_level_stats(int x1, int y1, int msg_type);
   void draw_level_stats(int &w, int &h, int draw, int msg_type);

   void show_level_stats_area(int area, int x1, int x2, int &y, int draw, int bco, int &gy, int &ty, int &max_x);
   void show_level_stats_main_area(     int x1, int x2, int &y, int draw, int bco, int &gy, int &ty, int &max_x);
   void show_level_stats_grand_total(   int x1, int x2, int &y, int draw, int bco, int &gy, int &ty, int &max_x);

   void show_level_stats_outline_with_thicker_lines(int x1, int x2, int  gy, int   y, int c);
   void show_level_stats_title_and_header(          int x1, int x2, int& gy, int& ty, int& max_x, int& y, int draw, int col, const char* title);
   void show_level_stats_totals(                    int x1, int x2, int  gy, int  ty, int& max_x, int& y, int draw, int col);
   void show_level_stats_row(                int i, int x1, int x2, int draw, int &max_x, int &y);


   int get_total_purple_coins(int level);










   void zero_level_data();
   int show_level_data(int x_pos, int y_pos, int type);
   void level_check();



   void overworld_area_array_add(int area, const char* name, int color, int visible, int x, int y, int w, int h);

   void create_door_helper(int x1, int y1, int x2, int y2, int enter_type, int tile);
   void create_msg_helper(int area);

   void create_msg_helper_trigger(int tx, int ty, int tw, int th, int tcol, int mx, int my, int mw, int mh, int mcol, int mflags, const char *msg);
   void create_msg_helper(int sx, int sy, int mx, int my, int mw, int mh, int mcol, int mfs, const char *msg);


   void fill_overworld_area_array();

   void fill_area_array(int area, int xasc, int &aai);
   int area_array[100][2];
   struct level_data data[100];
   struct overworld_area overworld_area_array[20];




   const char* level_status_text[4]  = { "Locked", "Ready", "Complete", "Perfect" };
   int         level_status_color[4] = {       10,      13,         12,        8  };

   void unlock_all_level_in_area(int area);


   void reset_level_data();
   void unlock_all_levels_and_areas();


   void set_overworld_barriers();

   void data_helper(int i, int time_par, const char *name);

   void set_level_names_and_par_time();




   void clear_data();
   void load_data();
   void save_data();

   void dump_level_data();

   void add_play_data_record(int lev, int type);
   void calc_level_stats(int lev);
   void check_achievements();

   int level_exists(int level);

   void pml_to_var(char * b);
   void var_to_pml(char * b);




   int make_backup_on_save_level = 0;

   int skc_trigger_demo;
   int skc_trigger_demo_cheat;

};
extern mwLevel mLevel;

#endif //PM_MWLEVEL_H