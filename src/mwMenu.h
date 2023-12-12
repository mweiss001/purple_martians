// mwMenu.h

class mwMenu
{
   public:

   char menu_string[25][80];

   void init_zmenu(int menu_num);

   void get_zmenu_width_and_height(int menu_num, int& width, int &height);

   int zmenu(int menu_num, int menu_pos);

   int tmenu(int menu_pos, int x1, int y1);

   void init_pmenu(int type);
   int pmenu(int type, int bg_color, int y_offset, int default_selection);


};

extern mwMenu mMenu;

