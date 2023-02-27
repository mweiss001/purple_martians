// mwMenu.h

class mwMenu
{
   public:

   char menu_string[25][80];

   void init_zmenu(void);
   int zmenu(int menu_pos, int y);

   int tmenu(int menu_pos, int x1, int y1);

   void init_pmenu(int type);
   int pmenu(int type, int bg_color);

};

extern mwMenu mMenu;

