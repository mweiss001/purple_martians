
#include "mwRect.h"

class mwListBox
{

public:

   void init();
   void draw();

   int color = 9;

   std::vector<std::string> list;

   mwRect<int> rect = mwRect<int>::fromX1Y1WH(0,0,0,0);
   mwRect<int> list_rect = mwRect<int>::fromX1Y1WH(0,0,0,0);

   int selected_item = 0;
   bool selection_changed;


private:


   int num_items;
   int num_items_shown;

   int list_start_index;
   int list_last_index;

   int hover_item = -1;

   int line_height = 10;
   int line_padding = 1;


   void draw_list();
   void set_list_start_index(int index);
   void set_selected_item(int index);
   void frame_line(int line, int color);
};





