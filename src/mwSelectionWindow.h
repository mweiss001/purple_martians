#ifndef PM_MWSELECTIONWINDOW_H
#define PM_MWSELECTIONWINDOW_H
#include <string>
#include <vector>
#include <array>

#include "mwRect.h"


struct pde
{
   int type;
   std::string name;
   std::string desc;
   std::array<int, 32> ia;
   std::array<float, 16> fa;
};


struct tileSetGroup
{
   std::string name;
   int display_tile;
   bool visible;
};





class mwSelectionWindow
{

public:

   mwSelectionWindow() { init(); }


   void init();
   tileSetGroup tileSetGroups[32];


   std::vector<pde> pdes;

   int block_array[100][16];

   bool block_on;
   bool special_on;

   int block_array_num_lines;  // number of lines

   int special_array_num_lines = 4;  // number of lines
   int special_array_cur_lines = 3;  // number of lines currently shown

   void fill_block_array(void);

   void draw(mwRect<int> &rect, int draw_only, int have_focus);



private:

   int lasty; // last y position inserted


   void load_pde(void);

   void pushCreator(std::string text, int type, int tile, int creator_id, std::string desc = "");
   void pushRegular(struct pde p, std::string text, int type, std::string desc);
   void pushBlankSpacer();

   void block_set_set(int t, int x, int y);
   void block_set_add(int t, int &x, int &y);
   void set_block_set_rainbow(int t, int &x, int &y);

   void set_block_set_24(int t, int &x, int &y);
   void set_block_set_16_4x4(int t, int &x, int &y);
   void set_block_set_16_2x8(int t, int x, int y);


   void set_block_set_2x3(int t, int &x, int &y);
   void set_block_set_3x1(int t, int &x, int &y);
   void set_block_set_1x3(int t, int &x, int &y);
   void set_block_set_1x4(int t, int &x, int &y);

   void set_block_set_3x3(int t, int &x, int &y);

   void set_block_extended(int t, int &x, int &y);

   void set_block_set_main_blocks(int &x, int &y);




   bool isTileSetGroupVisible(std::string name);








};


extern mwSelectionWindow mSelectionWindow;


#endif //PM_MWSELECTIONWINDOW_H