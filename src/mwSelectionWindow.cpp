#include "mwSelectionWindow.h"

#include "pm.h"
#include "mwBitmap.h"
#include "mwTileSets.h"
#include "mwWindowManager.h"

mwSelectionWindow mSelectionWindow;

void mwSelectionWindow::init()
{
   fill_block_array();
   load_pde();
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

   // these blocks are always os

   // empty block and blocks with grass
   for (int c=0; c<7; c++) block_set_add(c, x, y);

   // empty blocks with arrows
   for (int c=11; c<16; c++) block_set_add(c, x, y);

   x=12;
   // empty switch blocks
   for (int c=72; c<76; c++) block_set_add(c, x, y);

   // new row

   // empty blocks to mark bomb and rocket cloner trigger
   for (int c=42; c<44; c++) block_set_add(c, x, y);
   for (int c=17; c<26; c++) block_set_add(c, x, y);


   x=12;
   // solid switch blocks
   for (int c=76; c<80; c++) block_set_add(c, x, y);


   // new row

   // other solid blocks
   for (int c=129; c<135; c++) block_set_add(c, x, y);

   // bombable
   x=7;
   for (int c=64; c<68; c++) block_set_add(c, x, y);

   x++;

   // keyed blocks
   for (int c=80; c<84; c++) block_set_add(c, x, y);


   // new row

   // semi-solid screen
   for (int c=33; c<41; c++) block_set_add(c, x, y);

   x++;
   // ropes and ladders
   for (int c=50; c<57; c++) block_set_add(c, x, y);

   // new row

   // solid color blocks
   for (int c=97; c<112; c++) block_set_add(c, x, y);
   y++; x=0; // force new row


   if (isTileSetGroupVisible("Platforms"  ))
   {


   }
/*

   set_block_extended(192,  y); y+=4;
   set_block_extended(256,  y); y+=4;
   set_block_set_24(640, x, y); y+=2;
*/


   set_block_extended(192, y);  y+=4;


   if (isTileSetGroupVisible("Purple Pipes")) { set_block_extended(256, y);  y+=4; }
   if (isTileSetGroupVisible("Red Pipes"   )) { set_block_extended(320, y);  y+=4; }
   if (isTileSetGroupVisible("Green Pipes" )) { set_block_extended(384, y);  y+=4; }
   if (isTileSetGroupVisible("Blue Pipes"  )) { set_block_extended(448, y);  y+=4; }

   if (isTileSetGroupVisible("Wires"  ))
   {
      int t = 832;

      set_block_set_16_2x8(t, x, y);   // wires

      block_set_set(t+17, x+8, y);   // B H
      block_set_set(t+18, x+8, y+1);  // B V

      block_set_set(t+19, x+9,  y);    // I H
      block_set_set(t+20, x+9,  y+1);   // I V
      block_set_set(t+16, x+10, y);    // V H

      y+=2;
   }

   if (isTileSetGroupVisible("Template"     ))  { set_block_set_24(736, x, y); y+=2;  }
   if (isTileSetGroupVisible("Industrial 1" ))  { set_block_set_24(768, x, y); y+=2;  }
   if (isTileSetGroupVisible("Industrial 2" ))  { set_block_set_24(800, x, y); y+=2;  }



//   if (isTileSetGroupVisible("Purple Bricks" )) { set_block_set_24(640, x, y); y+=2;  }

   if (isTileSetGroupVisible("Yellow Thatch" )) set_block_set_16_4x4(864,  x, y);
   if (isTileSetGroupVisible("Brain" ))         set_block_set_16_4x4(896,  x, y);
   if (isTileSetGroupVisible("Grey Bricks" ))   set_block_set_16_4x4(928,  x, y);
   if (isTileSetGroupVisible("Brown Bricks" ))  set_block_set_16_4x4(960,  x, y);
   if (isTileSetGroupVisible("Slate Bricks" ))  set_block_set_16_4x4(880,  x, y);
   if (isTileSetGroupVisible("Red Dirt" ))      set_block_set_16_4x4(944,  x, y);
   if (isTileSetGroupVisible("Grey Rock" ))     set_block_set_16_4x4(976,  x, y);
   if (isTileSetGroupVisible("Grey Blocks" ))   set_block_set_16_4x4(1008, x, y);
                                                   set_block_set_16_4x4(720,  x, y); // wrought iron grate
   if (isTileSetGroupVisible("Rainbow" ))       set_block_set_rainbow(704, x, y);

   if (isTileSetGroupVisible("Wires" ))         set_block_set_16_4x4(832, x, y);



   set_block_set_16_4x4(992,  x, y); // fat pipes
   set_block_set_1x4(588, x, y); // rough brown column
   set_block_set_1x4(592, x, y); // gold pipe column


   // after the end of the 4x4's do a new line
   x=0; y+=4;

   // 2x3 columns
   set_block_set_2x3(512, x, y);
   set_block_set_2x3(518, x, y);
   set_block_set_2x3(524, x, y);
   set_block_set_2x3(530, x, y);
   set_block_set_2x3(536, x, y);

   // 1x3 columns
   set_block_set_1x3(544, x, y); // gold rough column
   set_block_set_1x3(547, x, y); // grey pipe
   set_block_set_1x3(550, x, y); // orange
   set_block_set_1x3(553, x, y); // blue
   set_block_set_1x3(556, x, y); // grey round with holes
   set_block_set_1x3(559, x, y); // gold orange pipe

   set_block_set_1x3(611, x, y); // grey hole girder
   set_block_set_1x3(617, x, y); // red cross girder
   set_block_set_1x3(623, x, y); // grey cross girder
   set_block_set_1x3(629, x, y); // grey rivet girder

   set_block_set_1x3(632, x, y); // grey/red dogbone
   set_block_set_1x3(635, x, y); // grey hex pipe

   // after the end of the x3's do a new line
   x=0; y+=3;

   // 3x1 platforms
   set_block_set_3x1(562, x, y); // gold pipe
   set_block_set_3x1(565, x, y); // gold beam
   set_block_set_3x1(568, x, y); // gool wood grain
   set_block_set_3x1(571, x, y); // green platform

   set_block_set_3x1(576, x, y); // grey segments
   set_block_set_3x1(579, x, y); // gold curved
   set_block_set_3x1(582, x, y); // second grey floater
   set_block_set_3x1(585, x, y); // brown bricks

   set_block_set_3x1(608, x, y); // grey hole girder
   set_block_set_3x1(614, x, y); // red cross girder
   set_block_set_3x1(620, x, y); // grey cross girder
   set_block_set_3x1(626, x, y); // grey rivet girder


   // new row to end
   y++;

   block_array_num_lines = y;
}

void mwSelectionWindow::block_set_set(int t, int x, int y)
{
   block_array[y][x] = t | mBitmap.tileFlags[t];    // add to list with default flags+
}

void mwSelectionWindow::block_set_add(int t, int &x, int &y)
{
   block_set_set(t, x, y);
   if (++x > 15) { x = 0; y++; }
}


void mwSelectionWindow::set_block_set_3x1(int t, int &x, int &y)
{
   // check at the start
   if (x>13) { x=0; y++; }

   block_set_set(t+0, x+0, y);
   block_set_set(t+1, x+1, y);
   block_set_set(t+2, x+2, y);

   x+=3;
}

void mwSelectionWindow::set_block_set_2x3(int t, int &x, int &y)
{
   // check at the start
   if (x>14) { x=0; y+=3; }

   block_set_set(t+0, x+0, y+0);
   block_set_set(t+1, x+1, y+0);
   block_set_set(t+2, x+0, y+1);
   block_set_set(t+3, x+1, y+1);
   block_set_set(t+4, x+0, y+2);
   block_set_set(t+5, x+1, y+2);
   x+=2;
}

void mwSelectionWindow::set_block_set_1x3(int t, int &x, int &y)
{
   // check at the start
   if (x>15) { x=0; y+=3; }

   block_set_set(t+0, x+0, y+0);
   block_set_set(t+1, x+0, y+1);
   block_set_set(t+2, x+0, y+2);
   x+=1;
}

void mwSelectionWindow::set_block_set_1x4(int t, int &x, int &y)
{
   // check at the start
   if (x>15) { x=0; y+=4; }

   block_set_set(t+0, x+0, y+0);
   block_set_set(t+1, x+0, y+1);
   block_set_set(t+2, x+0, y+2);
   block_set_set(t+3, x+0, y+3);

   x+=1;
}

// used for wires and 24 block tilesets starting point
// sets up 16 tiles on 2 lines 2x8
void mwSelectionWindow::set_block_set_16_2x8(int t, int x, int y)
{
   block_set_set(t+8,  x++, y); // tl corner
   block_set_set(t+9,  x++, y); // tr corner
   block_set_set(t+0,  x++, y); // middle cross
   block_set_set(t+2,  x++, y); // hline l
   block_set_set(t+3,  x++, y); // hline m
   block_set_set(t+4,  x++, y); // hline r
   block_set_set(t+12, x++, y); // tee tl
   block_set_set(t+14, x++, y); // tee tr

   x-=8; y++;

   block_set_set(t+10, x++, y); // ll corner
   block_set_set(t+11, x++, y); // lr corner
   block_set_set(t+1,  x++, y); // standalone single
   block_set_set(t+5,  x++, y); // vline t
   block_set_set(t+6,  x++, y); // vline m
   block_set_set(t+7,  x++, y); // vline b
   block_set_set(t+15, x++, y); // tee ll
   block_set_set(t+13, x++, y); // tee lr
}




// sets up 16 tiles on 4 lines 4x4
// used for most 16 block tileset
void mwSelectionWindow::set_block_set_16_4x4(int t, int &x, int &y)
{
   for (const auto& s : mTileSets.tileSets)
      if (s.startIndex == t)
      {
         block_set_set(s.OuterCornerTL,       x+0, y+0);
         block_set_set(s.OuterCornerTR,       x+2, y+0);
         block_set_set(s.OuterCornerBL,       x+0, y+2);
         block_set_set(s.OuterCornerBR,       x+2, y+2);

         block_set_set(s.OuterEdgeT,          x+1, y+0);
         block_set_set(s.OuterEdgeB,          x+1, y+2);
         block_set_set(s.OuterEdgeL,          x+0, y+1);
         block_set_set(s.OuterEdgeR,          x+2, y+1);

         block_set_set(s.SolidFill,           x+1, y+1);

         block_set_set(s.VLineT,              x+3, y+0);
         block_set_set(s.VLineM,              x+3, y+1);
         block_set_set(s.VLineB,              x+3, y+2);

         block_set_set(s.HLineL,              x+0, y+3);
         block_set_set(s.HLineM,              x+1, y+3);
         block_set_set(s.HLineR,              x+2, y+3);

         block_set_set(s.Single,              x+3, y+3);
      }

   if (x < 12) x += 4;
   else { x=0; y+=4; }
}

void mwSelectionWindow::set_block_set_24(int t, int x, int y)
{
   set_block_set_16_2x8(t, x, y); // template

   block_set_set(t+16, x+8, y);
   block_set_set(t+17, x+9, y);
   block_set_set(t+18, x+8, y+1);
   block_set_set(t+19, x+9, y+1);

   block_set_set(t+20, x+10, y);
   block_set_set(t+21, x+11, y);
   block_set_set(t+22, x+10, y+1);
   block_set_set(t+23, x+11, y+1);
}


void mwSelectionWindow::set_block_set_rainbow(int t, int x, int y)
{
   t+=8;
   block_set_set(t+0,  x+0, y+0);
   block_set_set(t+1,  x+1, y+0);
   block_set_set(t+4,  x+2, y+0);
   block_set_set(t+5,  x+3, y+0);

   block_set_set(t+2,  x+0, y+1);
   block_set_set(t+3,  x+1, y+1);
   block_set_set(t+6,  x+2, y+1);
   block_set_set(t+7,  x+3, y+1);

   t-=8;
   block_set_set(t+0,  x+0, y+2);
   block_set_set(t+1,  x+1, y+2);
   block_set_set(t+4,  x+2, y+2);
   block_set_set(t+5,  x+3, y+2);

   block_set_set(t+2,  x+0, y+3);
   block_set_set(t+3,  x+1, y+3);
   block_set_set(t+6,  x+2, y+3);
   block_set_set(t+7,  x+3, y+3);

   if (x < 12) x += 4;
   else { x=0; y+=4; }
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
         block_set_set(s.Single,              x+3, y+3);

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
         block_set_set(s.SingleReverse,       x+11, y+3);

         block_set_set(s.SemiSolid,           x+12, y+3);

      }
}


bool mwSelectionWindow::isTileSetGroupVisible(std::string name)
{
   for (int i=0; i<32; i++)
      if (mWM.tileSetGroups[i].name == name && mWM.tileSetGroups[i].visible) return true;
   return false;
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
   pushCreator("Two Way Door Set Creator", 1, 211, 972, "Set of two way linked doors.");
   pushCreator("Single Door Item Creator", 1, 209, 972, "Door with no exit link.");

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
