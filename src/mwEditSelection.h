#ifndef PM_MWEDITSELECTION
#define PM_MWEDITSELECTION
#include "mwEnemy.h"
#include "mwItem.h"
#include "mwLevel.h"
#include "mwLift.h"

#define SEL_SIZE 384800


class mwEditSelection
{
   public:
   mwEditSelection();


   void init();
   int ft_level_header[20];
   mwLevel ftLevel;
   mwEnemy ftEnemy;
   mwItem ftItem;
   mwLift ftLift;
   ALLEGRO_BITMAP *ft_bmp = NULL;  //  file temp paste bmp

   int copy_mode;
   int brf_mode;
   int sw;
   int sh;

   void clear_ft_variables();

   int load_selection();
   void fill_ft_variables_from_selection(int save_to_disk);
   void sel_to_ft(char * b);
   void ft_to_sel(char * b);

   void do_floodfill(int x, int y, int flood_block);
   void do_copy(int qx1, int qy1);
   void do_clear();
   void set_block_with_flag_filters(int x, int y, int tn);

   void draw_fsel();

   int draw_buttons(int x3, int x4, int yfb, int draw_only);


   void draw(mwRect<int> &rect, int d, int have_focus);

   void draw_level_editor_background_overlays(int mouse_on_window);


   void show_pointer_text(int x1, int x2, int y, int mouse_on_window);
   void process_mouse_on_background();

};

extern mwEditSelection mEditSelection;

#endif //PM_MWEDITSELECTION_H
