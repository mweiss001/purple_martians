#ifndef PM_MWEDITSELECTION
#define PM_MWEDITSELECTION
#include "mwEnemy.h"
#include "mwItem.h"
#include "mwLevel.h"
#include "mwLift.h"
#include "mwWindow.h"

#define SEL_SIZE 384800


struct liftIDTranslation
{
   int oldLiftNumber;
   int newLiftNumber;
};


struct itemIDTranslation
{
   int oldItemNumber;
   int newItemNumber;
};


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

   void clear_ft_variables();

   std::vector<liftIDTranslation> liftIDTranslations{};
   std::vector<itemIDTranslation> itemIDTranslations{};


   void add_sel_to_level(int lev, const char* sel_file, int x, int y);

   void copy_sel_from_level(int slev, int sx, int sy, int sw, int sh, int dlev, int dx, int dy, int clear);


   int load_selection(const char* filename);
   int load_selection_prompt();

   void fill_ft_variables_from_selection(int save_to_disk);
   void copy_ft_variables_to_level(int qx1, int qy1);

   void sel_to_ft(char * b);
   void ft_to_sel(char * b);


   void clear_area(int x1, int y1, int x2, int y2);


   void set_block_with_flag_filters(int x, int y, int tn);

   void draw_fsel();


   void draw(mwWindow &w);
   void draw_buttons(int x1, int x2, int &ya, int draw_only);
   void show_details(int x1, int x2, int &ya, int color);


   void draw_level_editor_background_overlays(int mouse_on_window);
   void process_mouse_on_background();

};

extern mwEditSelection mEditSelection;

#endif //PM_MWEDITSELECTION_H
