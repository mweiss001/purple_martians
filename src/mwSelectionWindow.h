#ifndef PM_MWSELECTIONWINDOW_H
#define PM_MWSELECTIONWINDOW_H
#include <string>
#include <vector>
#include <array>


struct pde
{
   int type;
   std::string name;
   std::string desc;
   std::array<int, 32> ia;
   std::array<float, 16> fa;
};


class mwSelectionWindow
{


public:

   void init();

   std::vector<pde> pdes;

   int block_array[100][16];

   bool block_on;
   bool special_on;

   int block_array_num_lines;  // number of lines
   int block_array_cur_lines;  // number of lines currently shown

   int special_array_num_lines = 4;  // number of lines
   int special_array_cur_lines = 3;  // number of lines currently shown

private:

   void fill_block_array(void);
   void load_pde(void);

   void pushCreator(std::string text, int type, int tile, int creator_id, std::string desc = "");
   void pushRegular(struct pde p, std::string text, int type, std::string desc);
   void pushBlankSpacer();

   void block_set_set(int t, int x, int y);
   void block_set_add(int t, int &x, int &y);
   void set_block_set_rainbow(int t, int x, int y);
   void set_block_set(int t, int x, int y);

   void set_block_extended(int t, int y);


};


extern mwSelectionWindow mSelectionWindow;


#endif //PM_MWSELECTIONWINDOW_H