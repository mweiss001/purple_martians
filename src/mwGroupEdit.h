#ifndef PM_MWGROUPEDIT_H
#define PM_MWGROUPEDIT_H


#define NUM_OBJ 600
#include "mwRect.h"


// structure for predefined control things that can be group edited
struct ge_datum
{
   char name[40];
   char snam[8];
   int vartyp;     // 2=item, 3=mEnemy.Ei, 4=mEnemy.Ef, 5=initial direction, 6=initial position, 8 = sec xc
   int varnum;     // index of the variable
   int valid;      // does it apply to items in list
   int collapsed;  // how it is drawn
   int adj_mode;
   float adj_min;
   float adj_max;
   float min_allowed;
   float max_allowed;
};


class mwGroupEdit
{

   public:

   mwGroupEdit() { init(); }
   void init();

   struct ge_datum ge_data[100];

   // mapping table of controls to objects and types
   int cot[100][4][20] = {0}; // control, object_type, type,  mapping table

   int obj_list[NUM_OBJ][3];

   int show_sel_frame;

   void set_valid_controls();

   void collapse_all_controls();
   void expand_all_controls();
   void clear_obj_list();

   void add_to_obj_list(int t, int i);
   void remove_obj_list_item(int o);
   void remove_obj_list_filtered_items();
   void swap_obj_list_items(int i1, int i2);

   void object_initial_position_random(int typ, int num);

   int draw_list_items(int x1, int y1, int ni);
   void show_obj_list(int x, int y, int *ew, int *eh, int draw_only);

   void set_var(int gvt, int gvn, int num, float val);
   float get_var(int gvt, int gvn, int num);

   int show_controls(int x, int y, int *ew, int *eh, int hidden, int draw_only);
   void add_selection_to_list(int set_filters);
   void process_mouse_on_background();

   void draw_list(mwRect<int> &rect, int draw_only, int have_focus);
   void draw_controls(mwRect<int> &rect, int draw_only, int have_focus);

   void draw_level_editor_background_overlays(int mouse_on_window);



};

extern mwGroupEdit mGroupEdit;

#endif