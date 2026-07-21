#include "pm.h"
#include "mwSelectionWindow.h"
#include "mwBitmap.h"
#include "mwBitmapTools.h"
#include "mwColor.h"
#include "mwEditorMain.h"
#include "mwEventQueue.h"
#include "mwFont.h"
#include "mwHelp.h"
#include "mwInput.h"
#include "mwItem.h"
#include "mwLevelEditor.h"
#include "mwLift.h"
#include "mwLoop.h"
#include "mwObjectViewer.h"
#include "mwTileSets.h"
#include "mwWidget.h"
#include "mwWindowManager.h"

mwSelectionWindow mSelectionWindow;





void mwSelectionWindow::init()
{
   block_on = 1;
   special_on = 1;

   fill_block_array();
   load_pde();


      // set up tile set groups

   int i;

   for (i=0; i<32; i++)
   {
      tileSetGroups[i].name = "";
      tileSetGroups[i].display_tile = 0;
      tileSetGroups[i].visible= 0;
   }

   i=0;
   tileSetGroups[i].name = "Platforms";
   tileSetGroups[i].display_tile = 609;
   tileSetGroups[i].visible = 1;

   i++;
   tileSetGroups[i].name = "Columns";
   tileSetGroups[i].display_tile = 612;
   tileSetGroups[i].visible = 1;


   i++;
   tileSetGroups[i].name = "Extended";
   tileSetGroups[i].display_tile = 1024;
   tileSetGroups[i].visible = 0;


   i++;
   tileSetGroups[i].name = "Industrial";
   tileSetGroups[i].display_tile = 776;
   tileSetGroups[i].visible = 1;

   i++;
   tileSetGroups[i].name = "Wires"; // and 24
   tileSetGroups[i].display_tile = 832;
   tileSetGroups[i].visible = 1;


   i++;
   tileSetGroups[i].name = "Screen";
   tileSetGroups[i].display_tile = 928;
   tileSetGroups[i].visible = 1;

   i++;
   tileSetGroups[i].name = "Dirt";
   tileSetGroups[i].display_tile = 920;
   tileSetGroups[i].visible = 1;

   i++;
   tileSetGroups[i].name = "Blocks";
   tileSetGroups[i].display_tile = 1009;
   tileSetGroups[i].visible = 1;

   i++;
   tileSetGroups[i].name = "Rare";
   tileSetGroups[i].display_tile = 712;
   tileSetGroups[i].visible = 0;


/*

   i++;
   tileSetGroups[i].name = "Purple Bricks";
   tileSetGroups[i].display_tile = 192;
   tileSetGroups[i].visible= 1;


   i++;
   tileSetGroups[i].name = "Purple Pipes";
   tileSetGroups[i].display_tile = 256;
   tileSetGroups[i].visible= 0;

   i++;
   tileSetGroups[i].name = "Red Pipes";
   tileSetGroups[i].display_tile = 320;
   tileSetGroups[i].visible= 0;

   i++;
   tileSetGroups[i].name = "Green Pipes";
   tileSetGroups[i].display_tile = 384;
   tileSetGroups[i].visible= 0;

   i++;
   tileSetGroups[i].name = "Blue Pipes";
   tileSetGroups[i].display_tile = 448;
   tileSetGroups[i].visible= 0;




   i++;
   tileSetGroups[i].name = "Wires";
   tileSetGroups[i].display_tile = 832;
   tileSetGroups[i].visible= 1;



   i++;
   tileSetGroups[i].name = "Template";
   tileSetGroups[i].display_tile = 737;
   tileSetGroups[i].visible= 0;

   i++;
   tileSetGroups[i].name = "Industrial 1";
   tileSetGroups[i].display_tile = 776;
   tileSetGroups[i].visible= 0;

   i++;
   tileSetGroups[i].name = "Industrial 2";
   tileSetGroups[i].display_tile = 808;
   tileSetGroups[i].visible= 0;


   i++;
   tileSetGroups[i].name = "Purple Bricks2";
   tileSetGroups[i].display_tile = 640;
   tileSetGroups[i].visible= 1;


   i++;
   tileSetGroups[i].name = "Yellow Thatch";
   tileSetGroups[i].display_tile = 864;
   tileSetGroups[i].visible= 0;

   i++;
   tileSetGroups[i].name = "Brain";
   tileSetGroups[i].display_tile = 896;
   tileSetGroups[i].visible= 0;

   i++;
   tileSetGroups[i].name = "Grey Bricks";
   tileSetGroups[i].display_tile = 928;
   tileSetGroups[i].visible= 0;

   i++;
   tileSetGroups[i].name = "Brown Bricks";
   tileSetGroups[i].display_tile = 960;
   tileSetGroups[i].visible= 0;

   i++;
   tileSetGroups[i].name = "Slate Bricks";
   tileSetGroups[i].display_tile = 880;
   tileSetGroups[i].visible= 1;

   i++;
   tileSetGroups[i].name = "Red Dirt";
   tileSetGroups[i].display_tile = 944;
   tileSetGroups[i].visible= 1;

   i++;
   tileSetGroups[i].name = "Grey Rock";
   tileSetGroups[i].display_tile = 977;
   tileSetGroups[i].visible= 1;

   i++;
   tileSetGroups[i].name = "Grey Blocks";
   tileSetGroups[i].display_tile = 1009;
   tileSetGroups[i].visible= 1;

   i++;
   tileSetGroups[i].name = "Rainbow";
   tileSetGroups[i].display_tile = 712;
   tileSetGroups[i].visible= 1;

*/




































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

   // to keep track of the last inserted position
   lasty = 0;


   set_block_set_main_blocks(x, y);


   if (isTileSetGroupVisible("Platforms"))
   {
      // 3x1 platforms
      set_block_set_3x1(564, x, y); // gold pipe
      set_block_set_3x1(567, x, y); // gold beam
      set_block_set_3x1(570, x, y); // gold wood grain
      set_block_set_3x1(573, x, y); // green platform

      set_block_set_3x1(576, x, y); // grey segments
      set_block_set_3x1(579, x, y); // gold curved
      set_block_set_3x1(582, x, y); // second grey floater
      set_block_set_3x1(585, x, y); // brown bricks

      // force new row
      x=0; y+=1;
   }
   if (isTileSetGroupVisible("Columns"))
   {
      // 1x4 columns
      set_block_set_1x4(588, x, y); // rough brown column
      set_block_set_1x4(592, x, y); // gold pipe column
      set_block_set_1x4(596, x, y); // orange
      set_block_set_1x4(600, x, y); // blue
      set_block_set_1x4(604, x, y); // grey round with holes
      set_block_set_1x4(544, x, y); // gold rough column
      set_block_set_1x4(548, x, y); // grey pipe
      set_block_set_1x4(552, x, y); // gold orange pipe
      set_block_set_1x4(556, x, y); // grey/red dogbone
      set_block_set_1x4(560, x, y); // grey hex pipe
      set_block_set_1x4(635, x, y); // white chain

      x=0; y+=4;


      // 2x3 columns
      set_block_set_2x3(512, x, y);
      set_block_set_2x3(518, x, y);
      set_block_set_2x3(524, x, y);
      set_block_set_2x3(530, x, y);
      set_block_set_2x3(536, x, y);

      // after the end of the x3's do a new line
      x=0; y+=3;


      // 3x3 hline and vline girder sets
      set_block_set_3x3(608, x, y); // grey hole girder
      set_block_set_3x3(617, x, y); // grey cross girder
      set_block_set_3x3(626, x, y); // grey rivet girder
      set_block_set_3x3(640, x, y); // red cross girder

      // after the end of the x3's do a new line
      x=0; y+=3;

   }

   if (isTileSetGroupVisible("Extended"))
   {
      set_block_extended(1216,x, y);  // Purple Bricks
      set_block_extended(1024,x, y);  // Purple Pipes
      set_block_extended(1072,x, y);  // Red Pipes
      set_block_extended(1120,x, y);  // Green Pipes
      set_block_extended(1168,x, y);  // Blue Pipes
   }

   if (isTileSetGroupVisible("Industrial"))
   {
      set_block_set_24(672, x, y);  // Template
      set_block_set_24(704, x, y);  // Ice
      set_block_set_24(736, x, y);  // Tan
      set_block_set_24(768, x, y);  // alien machine
      set_block_set_24(800, x, y);  // alien dark

   }

   if (isTileSetGroupVisible("Wires")) // and 24 industrial
   {
      // normal 16 4x4, but with extra custom tiles at the end
      if (x>11) { x=0; y+=4; } // need 5 empty columns, or start new row
      int t = 832;
      set_block_set_16_4x4(t, x, y);
      block_set_set(t+16, x, y+0); // volt meter H
      block_set_set(t+17, x, y+1); // current meter H
      block_set_set(t+18, x, y+2); // current meter V
      block_set_set(t+19, x, y+3); // cloner connection
      x+=1;
   }

   if (isTileSetGroupVisible("Blocks"))
   {
      set_block_set_16_4x4(1008, x, y); // Purple Platform Blocks
      set_block_set_16_4x4(992,  x, y); // Fat fuzzy pipes

      // grey slate bricks
      // normal 16 4x4, but with extra custom tiles at the end
      if (x>11) { x=0; y+=4; } // need 5 empty columns, or start new row
      set_block_set_16_4x4(880, x, y);
      block_set_set(855, x, y+0); // vline t
      block_set_set(856, x, y+1); // vline m
      block_set_set(857, x, y+2); // vline b
      x+=1;

   }

   if (isTileSetGroupVisible("Screen"))
   {
      set_block_set_16_4x4(928,  x, y); // wrought iron grate
      set_block_set_16_4x4(896,  x, y); // semi-solid screens
   }


   if (isTileSetGroupVisible("Dirt"))
   {
      set_block_set_16_4x4(912,  x, y); // Brown dirt and grass
      set_block_set_16_4x4(864,  x, y); // Red Dirt and Grass
      set_block_set_16_4x4(944,  x, y); // Red Dirt
      set_block_set_16_4x4(976,  x, y); // Grey Rock
   }


   if (isTileSetGroupVisible("Rare"))
   {
      set_block_set_16_4x4(960,  x, y); // Brown Bricks
      set_block_set_rainbow(496, x, y); // Rainbow
   }




   block_array_num_lines = lasty + 1;
}

void mwSelectionWindow::block_set_set(int t, int x, int y)
{
   block_array[y][x] = t | mBitmap.tileFlags[t];    // add to list with default flags

   if (y > lasty) lasty = y;
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


void mwSelectionWindow::set_block_set_main_blocks(int &x, int &y)
{
   // check at the start
   if (x>0) { x=0; y+=5; }

   // local copies
   int lx = x;
   int ly = y;



   // solid color blocks
   lx = x+1; ly = y+0;
   for (int c=97; c<=111; c++) block_set_add(c, lx, ly);


   // empty switch blocks
   lx = x+12; ly = y+1;
   for (int c=72; c<=75; c++) block_set_add(c, lx, ly);

   // solid switch blocks
   lx = x+12; ly = y+2;
   for (int c=76; c<=79; c++) block_set_add(c, lx, ly);

   // keyed blocks
   lx = x+8; ly = y+1;
   for (int c=80; c<=83; c++) block_set_add(c, lx, ly);

   // other solid blocks
   lx = x+0; ly = y+1;
   for (int c=129; c<=134; c++) block_set_add(c, lx, ly);

   // bomb and rocket trigger markers
   lx = x+6; ly = y+1;
   for (int c=42; c<=43; c++) block_set_add(c, lx, ly);

   // bombable
   lx = x+8; ly = y+2;
   for (int c=64; c<=67; c++) block_set_add(c, lx, ly);

   // ropes, ladders and vines
   lx = x+8; ly = y+3;
   for (int c=49; c<=57; c++) block_set_add(c, lx, ly);

   // arrows, ladders and vines
   lx = x+3; ly = y+3;
   for (int c=11; c<=15; c++) block_set_add(c, lx, ly);

   // my grass and flowers
   lx = x+1; ly = y+2;
   for (int c=1; c<=6; c++) block_set_add(c, lx, ly);



   // mega grass flowers, trees, cactus

   // 1x3 cactus
   lx = x+0; ly = y+4; block_set_add(34, lx, ly);
   lx = x+0; ly = y+3; block_set_add(33, lx, ly);
   lx = x+0; ly = y+2; block_set_add(32, lx, ly);


   // 1x2 cactus
   lx = x+1; ly = y+4; block_set_add(25, lx, ly);
   lx = x+1; ly = y+3; block_set_add(24, lx, ly);

   // 1x2 flower
   lx = x+2; ly = y+4; block_set_add(38, lx, ly);
   lx = x+2; ly = y+3; block_set_add(39, lx, ly);


   // grass, bushes, flowers
   lx = x+3; ly = y+4;
   for (int c=17; c<=23; c++) block_set_add(c, lx, ly);

   // railing
   lx = x+10; ly = y+4;
   for (int c=35; c<=37; c++) block_set_add(c, lx, ly);

   lx = x+13; ly = y+4;
   for (int c=40; c<=41; c++) block_set_add(c, lx, ly);



   // new row
   x=0; y+=5;
}



void mwSelectionWindow::set_block_set_3x3(int t, int &x, int &y)
{
   // check at the start
   if (x>13) { x=0; y+=3; }

   // top row hline
   block_set_set(t+0, x+0, y+0);
   block_set_set(t+1, x+1, y+0);
   block_set_set(t+2, x+2, y+0);

   // mid row vline
   block_set_set(t+3, x+0, y+1);
   block_set_set(t+4, x+1, y+1);
   block_set_set(t+5, x+2, y+1);

   // bottom row singles and cross
   block_set_set(t+6, x+0, y+2);
   block_set_set(t+7, x+1, y+2);
   block_set_set(t+8, x+2, y+2);

   x+=3;
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


// 16 tiles on 4 lines 4x4 - used for most 16 block tilesets
void mwSelectionWindow::set_block_set_16_4x4(int t, int &x, int &y)
{
   // check at the start
   if (x>12) { x=0; y+=4; }

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
   x+=4;
}

void mwSelectionWindow::set_block_set_24(int t, int &x, int &y)
{
   // check at the start
   if (x>0) { x=0; y+=2; }

   block_set_set(t+8,  x+0, y+0); // tl corner
   block_set_set(t+9,  x+1, y+0); // tr corner
   block_set_set(t+0,  x+2, y+0); // middle cross
   block_set_set(t+2,  x+3, y+0); // hline l
   block_set_set(t+3,  x+4, y+0); // hline m
   block_set_set(t+4,  x+5, y+0); // hline r
   block_set_set(t+12, x+6, y+0); // tee tl
   block_set_set(t+14, x+7, y+0); // tee tr

   block_set_set(t+10, x+0, y+1); // ll corner
   block_set_set(t+11, x+1, y+1); // lr corner
   block_set_set(t+1,  x+2, y+1); // standalone single
   block_set_set(t+5,  x+3, y+1); // vline t
   block_set_set(t+6,  x+4, y+1); // vline m
   block_set_set(t+7,  x+5, y+1); // vline b
   block_set_set(t+15, x+6, y+1); // tee ll
   block_set_set(t+13, x+7, y+1); // tee lr

   block_set_set(t+16, x+8,  y+0);
   block_set_set(t+17, x+9,  y+0);
   block_set_set(t+18, x+8,  y+1);
   block_set_set(t+19, x+9,  y+1);
   block_set_set(t+20, x+10, y+0);
   block_set_set(t+21, x+11, y+0);
   block_set_set(t+22, x+10, y+1);
   block_set_set(t+23, x+11, y+1);

   y+=2;
}


void mwSelectionWindow::set_block_set_rainbow(int t, int &x, int &y)
{
   // check at the start
   if (x>12) { x=0; y+=4; }

   block_set_set(t+8,  x+0, y+0);
   block_set_set(t+9,  x+1, y+0);
   block_set_set(t+12, x+2, y+0);
   block_set_set(t+13, x+3, y+0);

   block_set_set(t+10, x+0, y+1);
   block_set_set(t+11, x+1, y+1);
   block_set_set(t+14, x+2, y+1);
   block_set_set(t+15, x+3, y+1);

   block_set_set(t+0,  x+0, y+2);
   block_set_set(t+1,  x+1, y+2);
   block_set_set(t+4,  x+2, y+2);
   block_set_set(t+5,  x+3, y+2);

   block_set_set(t+2,  x+0, y+3);
   block_set_set(t+3,  x+1, y+3);
   block_set_set(t+6,  x+2, y+3);
   block_set_set(t+7,  x+3, y+3);

   x += 4;
}

void mwSelectionWindow::set_block_extended(int t, int &x, int &y)
{
   // check at the start
   if (x>0) { x=0; y+=4; }

   for (const auto& s : mTileSets.tileSets)
//      if (s.startIndex == t && s.extendedMode)
      if (s.startIndex == t && s.tileSetType == 48)
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

      }

   y+=4;

}


bool mwSelectionWindow::isTileSetGroupVisible(std::string name)
{
   for (int i=0; i<32; i++)
      if (tileSetGroups[i].name == name && tileSetGroups[i].visible) return true;
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





void mwSelectionWindow::draw(mwRect<int> &rect, int d, int have_focus)
{
   // calculate y positons

   // running y position, starting at first line below title bar
   int yc = rect.y1 + 11;

   // set special y position
   int select_window_special_height = mSelectionWindow.special_array_cur_lines*20 + 13;
   int select_window_special_y1 = yc;
   int select_window_special_y2 = select_window_special_y1 + select_window_special_height;
   if (mSelectionWindow.special_on) yc += select_window_special_height;

   // set block y positions
   int select_window_block_height = mSelectionWindow.block_array_num_lines*20 + 13;
   int select_window_block_y1 = yc;
   int select_window_block_y2 = select_window_block_y1 + select_window_block_height;
   if (mSelectionWindow.block_on) yc += select_window_block_height;


   // set text start y
   int select_window_text_y = yc;

   // set window height
   rect.setHeight(yc-rect.y1);


   int color = 9;


   // erase background
   rect.draw_filled_rectangle(mColor.pc[0]);


   // frame entire window
   rect.draw_rectangle(mColor.pc[color], 1);

   int x1 = rect.x1;
   int y1 = rect.y1;
   int x2 = rect.x2;

   // make rect for title bar
   mwRect<int> title_bar_rect = mwRect<int>::fromX1Y1X2Y2(x1, y1, x2, y1+11);

   // title bar background
   title_bar_rect.draw_filled_rectangle(mColor.pc[color+192]);

   // title bar frame
   title_bar_rect.draw_rectangle(mColor.pc[color], 1);

   // title bar title
   al_draw_textf(mFont.pr8, mColor.pc[9],  title_bar_rect.x1+2,   title_bar_rect.y1+2, 0, "Selection Window");

   // title bar controls
   int by1 = title_bar_rect.y1+2;
   if (mWidget.buttont(x2-10,  by1, x2-2,   9, 0,0,0,0, 0,-1,9,0, 0,0,0,d, "X"))       mLevelEditor.mWM.mW[2].active = 0;
   if (mWidget.buttont(x2-22,  by1, x2-14,  9, 0,0,0,0, 0,-1,9,0, 0,0,0,d, "?"))       mHelp.help("Selection Window");
   if (mWidget.buttont(x2-153, by1, x2-105, 9, 0,0,0,0, 0,-1,9,0, 0,1,0,d, "Tiles"))   mSelectionWindow.block_on = !mSelectionWindow.block_on;
   if (mWidget.buttont(x2-90,  by1, x2-34,  9, 0,0,0,0, 0,-1,9,0, 0,1,0,d, "Special")) mSelectionWindow.special_on = !mSelectionWindow.special_on;




   if (mSelectionWindow.special_on)
   {
      // make rect for special title bar
      mwRect<int> special_title_bar_rect = mwRect<int>::fromX1Y1X2Y2(x1, select_window_special_y1, x2, select_window_special_y1+11);

      // special title bar background color
      special_title_bar_rect.draw_filled_rectangle(mColor.pc[color+192]);

      // special title bar frame
      special_title_bar_rect.draw_rectangle(mColor.pc[color], 1);

      // special title bar title
      al_draw_textf(mFont.pr8, mColor.pc[9],  special_title_bar_rect.x1+2,   special_title_bar_rect.y1+2, 0, "Special Items");

      // special title bar controls
      by1 = special_title_bar_rect.y1+2;
      if (mWidget.buttont(x2-9, by1, x2-1, 9, 0,0,0,0, 0,-1,9,0, 0,0,0,d,"X")) mSelectionWindow.special_on = 0;
      if (mWidget.buttont(x2-41, by1, x2-33, 9, 0,0,0,0, 0,-1,9,0, 0,0,0,d,"+"))
         if (++mSelectionWindow.special_array_cur_lines > mSelectionWindow.special_array_num_lines) mSelectionWindow.special_array_cur_lines = mSelectionWindow.special_array_num_lines;
      if (mWidget.buttont(x2-25, by1, x2-17, 9, 0,0,0,0, 0,-1,9,0, 0,0,0,d,"-"))
         if (--mSelectionWindow.special_array_cur_lines < 1 )
         {
            mSelectionWindow.special_array_cur_lines++;
            mSelectionWindow.special_on = 0;
         }

      // draw special items
      for (int c=0; c<mSelectionWindow.special_array_cur_lines * 16; c++)
         if (c < (int) mSelectionWindow.pdes.size() && mSelectionWindow.pdes[c].type != -1)
         {
            int tn = mSelectionWindow.pdes[c].ia[1]; // default is the mBitmap.tile in PDEi[c][1]
            if (tn > 999) tn = mBitmap.zz[0][tn-1000]; // ans
            al_draw_bitmap(mBitmap.sprite[tn], x1+(c-((c/16)*16) )*20+1, select_window_special_y1+12+(c/16*20), 0 );
         }

      if (!d)
      {
         // make rect for special area
         mwRect<int> special_area_rect = mwRect<int>::fromX1Y1X2Y2(x1, select_window_special_y1+12, x2, select_window_special_y2-1);

         // is mouse on special area?
         int mx, my;
         if (special_area_rect.contains(mInput.mouse_x, mInput.mouse_y, mx, my))
         {
            int ret = ((my/20)*16) + mx/20;

            // is mouse pointing at valid pde?
            if (ret < (int) mSelectionWindow.pdes.size() && mSelectionWindow.pdes[ret].type != -1)
            {
               // convert to std::string and count lines
               std::string desc = mSelectionWindow.pdes[ret].desc;
               int tl = std::count(desc.begin(), desc.end(), '\n') + 1; // text lines

               // make rect for text area
               mwRect<int> text_area_rect = mwRect<int>::fromX1Y1X2Y2(x1, select_window_text_y, x2, select_window_text_y + 12 + tl*8);

               // draw text area
               text_area_rect.draw_filled_rectangle(mColor.pc[0]);
               text_area_rect.draw_rectangle(mColor.pc[color], 1);
               al_draw_line(x1, text_area_rect.y1 + 12, x2, text_area_rect.y1 + 11, mColor.pc[color], 1);
               al_draw_text(mFont.pr8, mColor.pc[9], x1+2, text_area_rect.y1+2, 0, "Description ");
               al_draw_multiline_text(mFont.pr8, mColor.pc[15], x1+2, select_window_text_y+12, 800, 0, 0, desc.c_str());

               if (mInput.mouse_b[1][0])
               {
                  while (mInput.mouse_b[1][0]) mEventQueue.proc(1);     // wait for release
                  int pn = mSelectionWindow.pdes[ret].ia[0];
                  if (pn < 200)
                  {
                     mEditorMain.draw_item_type = 5;
                     mEditorMain.draw_item_num = ret;
                  }
                  if (pn > 199) // Creator
                  {
                     switch (pn)
                     {
                        case 200: mObjectViewer.create_obj(2, 1, 0);  break; // door
                        case 201: mObjectViewer.create_obj(2, 5, 0);  break; // start
                        case 202: mObjectViewer.create_obj(2, 3, 0);  break; // exit
                        case 204: mObjectViewer.create_obj(3, 7, 0);  break; // vinepod
                        case 206: mObjectViewer.create_obj(2, 10,0);  break; // msg
                        case 207: mObjectViewer.create_obj(3, 9, 0);  break; // cloner
                        case 208: mLift.create_lift();  break; // lift

                        case 209: mItem.create_door(1); break; // single door item
                        case 210: mItem.create_door(2); break; // one way linked exit door
                        case 211: mItem.create_door(3); break; // two way door set

                        case 213: mObjectViewer.create_obj(2, 9, 0);  break; // trigger
                        case 214: mObjectViewer.create_obj(2, 16, 0); break; // block manip
                        case 215: mObjectViewer.create_obj(2, 17, 0); break; // block damage
                        case 217: mObjectViewer.create_obj(2, 13, 0); break; // timer
                        case 219: mObjectViewer.create_obj(2, 19, 0); break; // hider
                     }
                  }
               }
            }
         }
      }
   }


   if (mSelectionWindow.block_on)
   {
      // make rect for special title bar
      mwRect<int> block_title_bar_rect = mwRect<int>::fromX1Y1X2Y2(x1, select_window_block_y1, x2, select_window_block_y1+11);

      // special title bar background color
      block_title_bar_rect.draw_filled_rectangle(mColor.pc[color+192]);

      // special title bar frame
      block_title_bar_rect.draw_rectangle(mColor.pc[color], 1);

      // special title bar title
      al_draw_textf(mFont.pr8, mColor.pc[9],  block_title_bar_rect.x1+2,   block_title_bar_rect.y1+2, 0, "Tiles");

      // block title bar controls
      by1 = block_title_bar_rect.y1+2;

      if (mWidget.buttont(x2-9, by1, x2-1, 9, 0,0,0,0, 0,-1,9,0, 0,0,0,d,"X")) mSelectionWindow.block_on = 0;


      int x3 = x1 + 60;

      for (int i=0; i<32; i++)
         if (tileSetGroups[i].display_tile)
         {
            if (mWidget.mButtonTile(x3, by1-1, 10, tileSetGroups[i].display_tile, tileSetGroups[i].visible, d)) mSelectionWindow.fill_block_array();
            x3+=11;
         }

      // all on
      if (mWidget.buttont(x2-41, by1, x2-33, 9, 0,0,0,0, 0,-1,9,0, 0,0,0,d,"+"))
      {
         for (int i=0; i<32; i++) if (tileSetGroups[i].display_tile) tileSetGroups[i].visible = 1;
         mSelectionWindow.fill_block_array();
      }

      // all off
      if (mWidget.buttont(x2-25, by1, x2-17, 9, 0,0,0,0, 0,-1,9,0, 0,0,0,d,"-"))
      {
         for (int i=0; i<32; i++) if (tileSetGroups[i].display_tile) tileSetGroups[i].visible = 0;
         mSelectionWindow.fill_block_array();
      }

      // draw blocks
      for (int y=0; y<mSelectionWindow.block_array_num_lines; y++)
         for (int x=0; x<16; x++)
            al_draw_bitmap(mBitmap.tile[mSelectionWindow.block_array[y][x] & PM_BTILE_TILENUM_MASK], x1+x*20+1, select_window_block_y1+12+(y*20), 0 );

      if (!d)
      {
         // make rect for block area
         mwRect<int> block_area_rect = mwRect<int>::fromX1Y1X2Y2(x1, select_window_block_y1+12, x2, select_window_block_y2-1);

         // is mouse on block area?
         int mx, my;
         if (block_area_rect.contains(mInput.mouse_x, mInput.mouse_y, mx, my))
         {
            int ret = mSelectionWindow.block_array[my/20][mx/20];

            int syt = select_window_text_y;
            int tl = 3; // text lines
            int syt2 = syt+19+(8*tl);
            if (mEditorMain.show_flag_details) syt2 += 166;

            // make rect for text area
            mwRect<int> text_area_rect = mwRect<int>::fromX1Y1X2Y2(x1, syt, x2, syt2);

            // draw text area
            text_area_rect.draw_filled_rectangle(mColor.pc[0]);
            text_area_rect.draw_rectangle(mColor.pc[color], 1);
            al_draw_line(x1, text_area_rect.y1 + 12, x2, text_area_rect.y1 + 11, mColor.pc[color], 1);
            al_draw_text(mFont.pr8, mColor.pc[9], x1+2, text_area_rect.y1+2, 0, "Description ");

            // draw text for this block
            char msg[1024];
            mEditorMain.get_text_description_of_block_based_on_flags(ret, msg);
            al_draw_text (mFont.pr8, mColor.pc[15], x1+2, syt+14, 0, "---------------------");
            al_draw_textf(mFont.pr8, mColor.pc[15], x1+2, syt+22, 0, "Block %d - %s ", ret & PM_BTILE_TILENUM_MASK, msg);
            al_draw_text (mFont.pr8, mColor.pc[15], x1+2, syt+30, 0, "---------------------");

            int junk;
            if (mEditorMain.show_flag_details) mBitmapTools.draw_flags(x1+4, syt+38, ret, junk, 1, 0, 1);

            if ((mInput.mouse_b[1][0]) || (mInput.mouse_b[2][0]))
            {
               while (mInput.mouse_b[1][0]) mEventQueue.proc(1); // wait for release
               mEditorMain.draw_item_type = 1;
               mEditorMain.draw_item_num = ret;
            }
         }
      }
   }

   // do the main title bar highlight here so that other things will not draw over the bottom line
   if ((have_focus) && title_bar_rect.contains(mInput.mouse_x, mInput.mouse_y)) title_bar_rect.draw_rectangle(mColor.pc[14], 1);
}











/*
 *
 *
 *




   int syb = select_window_block_y1;


      int vx = (mInput.mouse_x-x1)/20; // column
      if (vx < 0) vx = 0;
      if (vx > 15) vx = 15;

      int syt = select_window_text_y;


      // check for mouse on block window
      if ( (mSelectionWindow.block_on) && (mInput.mouse_y > 14 + syb) && (mInput.mouse_y < 14 + syb + mSelectionWindow.block_array_num_lines * 20))
      {
         int vy = (mInput.mouse_y-syb-14)/20; // row
         int ret = mSelectionWindow.block_array[vy][vx];

         int tl = 6; // text lines
         int syt2 = syt+19+(8*tl);
         if (mEditorMain.show_flag_details) syt2 += 140;

         al_draw_filled_rectangle(x1, syt, x2, syt2, mColor.pc[0]); // erase
         al_draw_rectangle(x1, syt, x2, syt2, mColor.pc[9], 1);     // frame
         al_draw_rectangle(x1, syt, x2, syt+12, mColor.pc[9], 1); // title and frame
         al_draw_text(mFont.pr8, mColor.pc[9], x1+2, syt+2,  0, "Description");



         // draw text for this block
         char msg[1024];
         mEditorMain.get_text_description_of_block_based_on_flags(ret, msg);
         al_draw_text (mFont.pr8, mColor.pc[15], x1+2, syt+14, 0, "---------------------");
         al_draw_textf(mFont.pr8, mColor.pc[15], x1+2, syt+22, 0, "Block %d - %s ", ret & PM_BTILE_TILENUM_MASK, msg);
         al_draw_text (mFont.pr8, mColor.pc[15], x1+2, syt+30, 0, "---------------------");

         int junk;
         if (mEditorMain.show_flag_details) mBitmapTools.draw_flags(x1+4, syt+38, ret, junk, 1, 0, 1);

         if ((mInput.mouse_b[1][0]) || (mInput.mouse_b[2][0]))
         {
            while (mInput.mouse_b[1][0]) mEventQueue.proc(1); // wait for release
            mEditorMain.draw_item_type = 1;
            mEditorMain.draw_item_num = ret;
         }
      }


*/





