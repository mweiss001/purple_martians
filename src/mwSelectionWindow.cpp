#include "mwSelectionWindow.h"

#include "pm.h"
#include "mwBitmap.h"
#include "mwTileSets.h"


mwSelectionWindow mSelectionWindow;

void mwSelectionWindow::init()
{
   fill_block_array();
   load_pde();
}

void mwSelectionWindow::block_set_set(int t, int x, int y)
{
   block_array[y][x] = t | mBitmap.sa[t][0];        // add to list with default flags+
   block_array[y][x] &= ~PM_BTILE_SHOW_SELECT_WIN;  // clear select win flag
}

void mwSelectionWindow::block_set_add(int t, int &x, int &y)
{
   block_set_set(t, x, y);
   if (++x > 15) { x = 0; y++; }
}

void mwSelectionWindow::set_block_set_rainbow(int t, int x, int y)
{
   block_set_set(t+0,  x++, y); // tl corner
   block_set_set(t+1,  x++, y); // tr corner
   block_set_set(t+4,  x++, y);
   block_set_set(t+5,  x++, y);

   x -= 4;
   y++;

   block_set_set(t+2, x++, y); // ll corner
   block_set_set(t+3, x++, y); // lr corner
   block_set_set(t+6, x++, y);
   block_set_set(t+7, x++, y);

}


void mwSelectionWindow::set_block_extended(int t, int y)
{
   int x = 0;

   for (const auto& s : mTileSets.tileSets)
      if (s.startIndex == t && s.extendedMode)
      {
         block_set_set(s.FrameCornerTL,       x+0, y+0);
         block_set_set(s.FrameEdgeBTee,       x+1, y+0);
         block_set_set(s.FrameCornerTR,       x+2, y+0);
         block_set_set(s.VLineT,              x+3, y+0);

         block_set_set(s.FrameEdgeLTee,       x+0, y+1);
         block_set_set(s.FrameCross,          x+1, y+1);
         block_set_set(s.FrameEdgeRTee,       x+2, y+1);
         block_set_set(s.VLineM,              x+3, y+1);

         block_set_set(s.FrameCornerBL,       x+0, y+2);
         block_set_set(s.FrameEdgeTTee,       x+1, y+2);
         block_set_set(s.FrameCornerBR,       x+2, y+2);
         block_set_set(s.VLineB,              x+3, y+2);

         block_set_set(s.HLineL,              x+0, y+3);
         block_set_set(s.HLineM,              x+1, y+3);
         block_set_set(s.HLineR,              x+2, y+3);
         block_set_set(s.Single,         x+3, y+3);

         block_set_set(s.OuterCornerTL,       x+4, y+0);
         block_set_set(s.OuterCornerTR,       x+5, y+0);
         block_set_set(s.OuterCornerBL,       x+4, y+1);
         block_set_set(s.OuterCornerBR,       x+5, y+1);

         block_set_set(s.InnerCornerTL,       x+4, y+2);
         block_set_set(s.InnerCornerTR,       x+5, y+2);
         block_set_set(s.InnerCornerBL,       x+4, y+3);
         block_set_set(s.InnerCornerBR,       x+5, y+3);

         block_set_set(s.OuterEdgeL,          x+6, y+0);
         block_set_set(s.OuterEdgeT,          x+7, y+0);
         block_set_set(s.OuterEdgeB,          x+6, y+1);
         block_set_set(s.OuterEdgeR,          x+7, y+1);

         block_set_set(s.OuterEdgeLTee,       x+6, y+2);
         block_set_set(s.OuterEdgeTTee,       x+7, y+2);
         block_set_set(s.OuterEdgeBTee,       x+6, y+3);
         block_set_set(s.OuterEdgeRTee,       x+7, y+3);

         block_set_set(s.OuterCornerTLTeeT,   x+8, y+0);
         block_set_set(s.OuterCornerTRTeeR,   x+9, y+0);
         block_set_set(s.OuterCornerBLTeeL,   x+8, y+1);
         block_set_set(s.OuterCornerBRTeeB,   x+9, y+1);

         block_set_set(s.OuterCornerTLTeeL,   x+8, y+2);
         block_set_set(s.OuterCornerTRTeeT,   x+9, y+2);
         block_set_set(s.OuterCornerBLTeeB,   x+8, y+3);
         block_set_set(s.OuterCornerBRTeeR,   x+9, y+3);

         block_set_set(s.OuterCornerTLTeeTL,  x+10, y+0);
         block_set_set(s.OuterCornerTRTeeTR,  x+11, y+0);
         block_set_set(s.OuterCornerBLTeeBL,  x+10, y+1);
         block_set_set(s.OuterCornerBRTeeBR,  x+11, y+1);

         block_set_set(s.OuterCornerTRDiag,   x+10, y+2);
         block_set_set(s.OuterCornerTLDiag,   x+10, y+3);

         block_set_set(s.SolidFill,           x+11, y+2);


         block_set_set(s.HLineMKeyR,          x+12, y+0);
         block_set_set(s.HLineMKeyG,          x+12, y+1);
         block_set_set(s.HLineMKeyB,          x+12, y+2);
         block_set_set(s.HLineMKeyP,          x+12, y+3);

         block_set_set(s.VLineMKeyR,          x+13, y+0);
         block_set_set(s.VLineMKeyG,          x+13, y+1);
         block_set_set(s.VLineMKeyB,          x+13, y+2);
         block_set_set(s.VLineMKeyP,          x+13, y+3);




      }

}

void mwSelectionWindow::set_block_set(int t, int x, int y)
{

   block_set_set(t+8,  x++, y); // tl corner
   block_set_set(t+9,  x++, y); // tr corner
   block_set_set(t+0,  x++, y); // middle cross
   block_set_set(t+2,  x++, y); // hline l
   block_set_set(t+3,  x++, y); // hline m
   block_set_set(t+4,  x++, y); // hline r
   block_set_set(t+12, x++, y); // tee tl
   block_set_set(t+14, x++, y); // tee tr

   x -= 8;
   y++;

   block_set_set(t+10, x++, y); // ll corner
   block_set_set(t+11, x++, y); // lr corner
   block_set_set(t+1,  x++, y); // standalone single
   block_set_set(t+5,  x++, y); // vline t
   block_set_set(t+6,  x++, y); // vline m
   block_set_set(t+7,  x++, y); // vline b
   block_set_set(t+15, x++, y); // tee ll
   block_set_set(t+13, x++, y); // tee lr

}


void mwSelectionWindow::fill_block_array(void)
{
   // erase all
   block_array_num_lines = 0;
   for (int y=0; y<100; y++)
      for (int x=0; x<16; x++)
         block_array[y][x] = 0;

   // running position
   int x = 0;
   int y = 0;

   // add first 256
   for (int c=0; c<256; c++)
      if (mBitmap.sa[c][0] & PM_BTILE_SHOW_SELECT_WIN) block_set_add(c, x, y);
   x = 0;
   y++;

   set_block_extended(256, y);
   y+=4;

   set_block_set(832, x, y);   // wires
   y+=2;

   set_block_set(864, x, y);   // yellow thatch
   set_block_set(896, x+8, y); // brain
   y+=2;

   set_block_set(928, x, y);   // grey bricks
   set_block_set(960, x+8, y); // brown bricks
   y+=2;

   set_block_set(992, x, y); // blue
   set_block_set_rainbow(672, x+8, y);
   set_block_set_rainbow(704, x+12, y);
   y+=2;



   block_array_cur_lines = block_array_num_lines = y; // number of block array lines
}






void mwSelectionWindow::pushCreator(std::string text, int type, int tile, int creator_id, std::string desc)
{
   struct pde p;
   p.type = type;

   p.fa = { 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00 };
   p.ia = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   p.ia[0] = tile;
   p.ia[1] = creator_id;

   p.name = text;
   std::string dash(text.length(), '-');
   p.desc.insert(0, dash + "\n" + text + "\n" + dash);
   if (desc.length() > 0) p.desc +=  "\n\n"  + desc + "\n";
   pdes.push_back(p);
}


void mwSelectionWindow::pushRegular(struct pde p, std::string text, int type, std::string desc)
{
   p.type = type;
   p.name = text;
   std::string dash(text.length(), '-');
   p.desc.insert(0, dash + "\n" + text + "\n" + dash);
   if (desc.length() > 0) p.desc +=  "\n\n"  + desc + "\n";
   pdes.push_back(p);
}


void mwSelectionWindow::pushBlankSpacer()
{
   struct pde p;
   p.type = -1;
   pdes.push_back(p);
}




void mwSelectionWindow::load_pde()
{
   pdes.clear();
   struct pde p;

   pushCreator("Start Block Creator", 1, 201, 383, "Where players start the level\n\nor re-spawn after death.");
   pushCreator("Exit Creator",        1, 202, 398, "Where players exit the level.\n\nMultiple exits can exist.");

   pushCreator("One Way Door Set Creator", 1, 210, 972, "Set of one way linked doors.");
   pushCreator("Two Way Door Set Creator", 1, 210, 972, "Set of two way linked doors.");
   pushCreator("Single Door Item Creator", 1, 210, 972, "Door with no exit link.");

   pushCreator("VinePod Creator", 1, 204, 374, "Step 1 - Set initial location.\n\nStep 2 - Set extended position.\n\nStep 3 - Draw trigger box.");
   pushCreator("Lift Creator",    1, 208, 505, "Step 1 - Set initial location.\n\nStep 2 - Add additional steps.\n\nStep 3 - Edit with lift viewer.");
   pushCreator("Message Creator", 1, 206, 256, "Step 1 - Type a new message\n\nStep 2 - Set the item location\n\nStep 3 - Set the message location");
   pushCreator("Cloner Creator",  1, 207, 551, "Step 1 - Set cloner location.\n\nStep 2 - Draw source area.\n\nStep 3 - Position destination area.");

   pushCreator("Trigger Creator",      1, 213, 991, "");
   pushCreator("Block Manip Creator",  1, 214, 989, "");
   pushCreator("Block Damage Creator", 1, 215, 988, "");
   pushCreator("Timer Creator",        1, 217, 987, "");

   p.ia = { 106, 418, 16, 0, 0, 0, 0, 60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   pushRegular(p, "Orb", 1,  "");

   p.ia = { 107, 1024, 1, 1, 0, 0, 0, 0, 50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   pushRegular(p, "Mine", 1,  "");

   p.ia = { 115, 1052, 1, -1, 0, 0, 3, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   pushRegular(p, "Sproingy", 1,  "");




   p.ia = { 102, 1023, 1, 1, 0, 0, 1, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   pushRegular(p, "Health Bonus 10", 1,  "1/10 of Full Health");

   p.ia = { 102, 1027, 1, 1, 0, 0, 1, 50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   pushRegular(p, "Health Bonus 50", 1,  "1/2 of Full Health");

   p.ia = { 102, 197, 0, 1, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   pushRegular(p, "Purple Coin", 1,  "");




   p.ia = { 108, 464, 1, -1, 0, 0, 0, 20, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   pushRegular(p, "Bomb", 1,  "Regular bomb.");

   p.ia = { 108, 537, 1, -1, 0, 0, 0, 60, 0, 120, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   pushRegular(p, "Bomb", 1,  "Bomb with remote detonator.");

   p.ia = { 108, 535, 1, -1, 0, 0, 0, 80, 0, 160, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   pushRegular(p, "Bomb", 1,  "Sticky bomb with remote detonator.");

   p.ia = { 108, 534, 1, -1, 0, 0, 0, 40, 0, 80, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   pushRegular(p, "Bomb", 1,  "Sticky bomb.");


   p.ia = { 111, 246, 2, 1, 0, 0, 1, 80, 8, 50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   pushRegular(p, "Rocket", 1,  "");


   p.ia = { 104, 1039, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   pushRegular(p, "Red Key", 1,  "");

   p.ia = { 104, 1040, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   pushRegular(p, "Green Key", 1,  "");

   p.ia = { 104, 1041, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   pushRegular(p, "Blue Key", 1,  "");

   p.ia = { 104, 1042, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   pushRegular(p, "Purple Key", 1,  "");



   p.ia = { 114, 98, 0, 0, 0, 0, 0, 0, 2000, 2000, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   pushRegular(p, "Red Switch", 1,  "");

   p.ia = { 114, 97, 0, 0, 0, 0, 0, 0, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   pushRegular(p, "Green Switch", 1,  "");

   p.ia = { 114, 99, 0, 0, 0, 0, 0, 0, 2000, 2000, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   pushRegular(p, "Blue Switch", 1,  "");

   p.ia = { 114, 100, 0, 0, 0, 0, 0, 0, 2000, 2000, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   pushRegular(p, "Purple Switch", 1,  "");




   p.ia = { 1, 336, 0, 29, 1, 29, 31, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 10, 0, 0 };
   p.fa = { 0.00, 0.00, 0.00, 3.00, 1.62, 3.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00 };
   pushRegular(p, "FlickenBy", 1,  "Normal Speed.\n\nPod seek ans!");

   p.ia = { 1, 336, 0, 29, 1, 29, 31, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 10, 0, 0 };
   p.fa = { 0.00, 0.00, 0.00, 4.00, 1.62, 4.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00 };
   pushRegular(p, "FlickenBy 2", 1,  "Faster Speed.\n\nPod seek ans!");


   p.ia = { 1, 511, 0, 14, 1, 14, 31, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 10, 0, 0 };
   p.fa = { 0.00, 0.00, 0.00, 3.00, 1.62, 3.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00 };
   pushRegular(p, "FlutterBy", 1,  "Normal Speed.\n\nPod seek ans!");

   p.ia = { 1, 511, 0, 14, 1, 14, 31, 0, 4, 10, 0, 5, 30, 0, 0, 0, 100, 30, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 10, 0, 0 };
   p.fa = { 0.00, 0.00, 0.00, 4.00, 1.62, 4.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00 };
   pushRegular(p, "FlutterBy 2", 1,  "Faster Speed.\n\nPod seek ans!");


   p.ia = { 2, 507, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 100, 0, 1000, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 10, 0, 0 };
   p.fa = { 0.00, 0.00, 0.00, 3.00, 2.60, 3.00, 0.00, 2.33, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00 };
   pushRegular(p, "Cannon 1", 1,  "Standard");

   p.ia = { 2, 507, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 150, 0, 1000, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 10, 0, 0 };
   p.fa = { 0.00, 0.00, 0.00, 0.00, 2.55, 0.00, 0.00, 4.02, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00 };
   pushRegular(p, "Cannon 2", 1,  "Stationary");

   p.ia = { 2, 507, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 80, 0, 1000, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 10, 0, 0 };
   p.fa = { 0.00, 0.00, 0.00, 5.00, 2.47, 5.00, 0.00, 3.72, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00 };
   pushRegular(p, "Cannon 3", 1,  "Faster");


   p.ia = { 3, 500, 1, 2, 2, 0, 0, 30, 1, 0, 0, 5, 30, 0, 0, 30, 0, 100, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 10, 0, 0 };
   p.fa = { 0.00, 0.00, 2.00, 2.27, 2.52, 0.00, 2.00, 6.27, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00 };
   pushRegular(p, "ArchWagon 1", 1,  "Standard");

   p.ia = { 3, 500, 1, 2, 2, 0, 0, 30, 1, 0, 0, 5, 30, 0, 0, 18, 0, 160, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 10, 0, 0 };
   p.fa = { 0.00, 0.00, 3.00, 3.57, 2.52, 0.00, 3.00, 10.21, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00 };
   pushRegular(p, "ArchWagon 2", 1,  "Faster");


   p.ia = { 4, 864, 1, 56, 2, 0, 0, 30, 1, 0, 0, 5, 40, 35586216, 0, 0, 100, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0 };
   p.fa = { 0.00, 0.00, 2.16, 2.87, 2.52, 0.00, 2.16, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00 };
   pushRegular(p, "Block Walker", 1,  "");

   p.ia = { 5, 705, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 10, 0, 0 };
   p.fa = { 0.00, 0.00, 2.00, 2.00, 3.00, 0.00, 4.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00 };
   pushRegular(p, "JumpWorm", 1,  "");

   p.ia = { 6, 155, 1, 60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60, 0, 160, 40, 100, 60, 30, 0, 0, 0, 10, 0, 0, 0, 10, 0, 0 };
   p.fa = { 0.00, 0.00, 0.00, 2.00, 3.00, 2.63, 0.20, 4.00, 0.76, 0.00, 3.00, 0.00, 1.00, 0.00, 0.00, 0.00 };
   pushRegular(p, "Flapper", 1,  "Standard");


   p.ia = { 8, 384, 1, 18, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 40, 0, 80, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 10, 0, 0 };
   p.fa = { 0.00, 0.00, 1.37, 1.31, 2.52, 0.00, 0.00, 2.69, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00 };
   pushRegular(p, "TrakBot 1", 1,  "Standard");

   p.ia = { 8, 384, 1, 18, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 20, 0, 80, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 10, 0, 0 };
   p.fa = { 0.00, 0.00, 2.08, 2.11, 2.52, 0.00, 0.00, 2.20, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00 };
   pushRegular(p, "TrakBot 2", 1,  "Faster");

   pushBlankSpacer();
   pushBlankSpacer();


   p.ia = { 19, 401, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   p.fa = { 0.00, 0.00, 0.00, 2.00, 0.00, 0.00, 2.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00 };
   pushRegular(p, "Crew (NPC)", 1,  "Children to rescue");

   pushCreator("Hider Creator",        1, 219, 476, "");

   p.ia = { 118, 940, 0, 0, 0, 0, 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   pushRegular(p, "Gate", 1,  "");


}



