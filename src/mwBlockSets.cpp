#include "mwBlockSets.h"
mwBlockSets mBlockSets;

void mwBlockSets::constructEmptySet()
{
   b.name = "";
   b.triggerIndexStart = 0;
   b.triggerIndexEnd   = 0;

   b.rectMode          = 0;
   b.hlineMode         = 0;
   b.vlineMode         = 0;

   b.fill              = 0;
   b.single            = 0;
   b.cornerUpperLeft   = 0;
   b.cornerUpperRight  = 0;
   b.cornerLowerLeft   = 0;
   b.cornerLowerRight  = 0;
   b.edgeLeft          = 0;
   b.edgeRight         = 0;
   b.edgeTop           = 0;
   b.edgeBottom        = 0;
   b.hLineLeft         = 0;
   b.hLineMiddle       = 0;
   b.hLineRight        = 0;
   b.vLineTop          = 0;
   b.vLineMiddle       = 0;
   b.vLineBottom       = 0;
}



// filled rectangle set
blockSet mwBlockSets::constructRectSet(std::string name, int i)
{
   b.name = name;
   b.triggerIndexStart = i;
   b.triggerIndexEnd   = i+15;
   b.fill              = i;
   b.single            = i+1;
   setDefaultRect(i+8);
   setHline(i, 2, 3, 4);
   setVline(i, 5, 6, 7);
   return b;
}


// empty frame set
blockSet mwBlockSets::constructFrameSet(std::string name, int i)
{
   b.name = name;
   b.triggerIndexStart = i;
   b.triggerIndexEnd   = i+15;
   b.fill              = -1;
   b.single            = i;
   setDefaultRect(i+8);
   setHline(i, 2, 3, 4);
   setVline(i, 5, 6, 7);
   b.edgeLeft          = i + 6;
   b.edgeRight         = i + 6;
   b.edgeTop           = i + 3;
   b.edgeBottom        = i + 3;
   return b;
}


void mwBlockSets::setDefaultRect(int i)
{
   b.rectMode          = 1;
   b.cornerUpperLeft   = i + 0;
   b.cornerUpperRight  = i + 1;
   b.cornerLowerLeft   = i + 2;
   b.cornerLowerRight  = i + 3;
   b.edgeLeft          = i + 4;
   b.edgeRight         = i + 5;
   b.edgeTop           = i + 6;
   b.edgeBottom        = i + 7;
}

void mwBlockSets::setHline(int i, int left, int middle, int right)
{
   b.hlineMode          = 1;
   b.hLineLeft          = i + left;
   b.hLineMiddle        = i + middle;
   b.hLineRight         = i + right;
}

void mwBlockSets::setVline(int i, int top, int middle, int bottom)
{
   b.vlineMode          = 1;
   b.vLineTop           = i + top;
   b.vLineMiddle        = i + middle;
   b.vLineBottom        = i + bottom;
}

// hline with addition of multiple hlines when drawing rect
blockSet mwBlockSets::constructHlineSet(std::string name, int i)
{
   constructEmptySet();
   b.name = name;
   b.triggerIndexStart = i;
   b.triggerIndexEnd   = i + 2;
   setHline(i, 0, 1, 2);

   b.rectMode          = 1;
   b.fill              = i + 1;
   b.cornerUpperLeft   = i + 0;
   b.cornerUpperRight  = i + 2;
   b.cornerLowerLeft   = i + 0;
   b.cornerLowerRight  = i + 2;
   b.edgeLeft          = i + 0;
   b.edgeRight         = i + 2;
   b.edgeTop           = i + 1;
   b.edgeBottom        = i + 1;

   return b;
}

void mwBlockSets::init()
{
   loaded = 1;

   blockSets.push_back(constructRectSet("brown and yellow thatch", 864));
   blockSets.push_back(constructRectSet("brain", 896));
   blockSets.push_back(constructRectSet("grey bricks", 928));
   blockSets.push_back(constructRectSet("brown bricks", 960));
   blockSets.push_back(constructRectSet("blue solid", 992));

   blockSets.push_back(constructFrameSet("experimental", 352));
   blockSets.push_back(constructRectSet("experimental2", 384));

   blockSets.push_back(constructFrameSet("purple pipe with open center", 800));
   blockSets.push_back(constructFrameSet("wires", 832));

   blockSets.push_back(constructHlineSet("lined platform", 161));
   blockSets.push_back(constructHlineSet("brown bricks", 177));




   constructEmptySet();
   b.name = "white chain";
   b.triggerIndexStart = 54;
   b.triggerIndexEnd   = 56;
   setVline(54, 0, 1, 2);
   blockSets.push_back(b);

   int i = 33;
   constructEmptySet();
   b.name = "semi-solid screen";
   b.triggerIndexStart = 33;
   b.triggerIndexEnd   = 40;
   setHline(i, 3, 4, 5);
   setVline(i, 0, 1, 2);
   b.rectMode          = 1;
   b.fill              = -1;
   b.cornerUpperLeft   = i + 6;
   b.cornerUpperRight  = i + 7;
   b.cornerLowerLeft   = i + 3;
   b.cornerLowerRight  = i + 5;
   b.edgeLeft          = i + 1;
   b.edgeRight         = i + 1;
   b.edgeTop           = i + 4;
   b.edgeBottom        = i + 4;
   blockSets.push_back(b);


   i = 672;
   constructEmptySet();
   b.name = "rainbows";
   b.triggerIndexStart = i;
   b.triggerIndexEnd   = i + 7;
   b.fill              = -1;
   setDefaultRect(i);
   blockSets.push_back(b);

   i = 704;
   constructEmptySet();
   b.name = "rainbows2";
   b.triggerIndexStart = i;
   b.triggerIndexEnd   = i + 7;
   b.fill              = -1;
   setDefaultRect(i);
   blockSets.push_back(b);


   i = 512;
   constructEmptySet();
   b.name = "purple pipe with solid center";
   b.triggerIndexStart = i;
   b.triggerIndexEnd   = i + 7;
   setDefaultRect(i);
   b.fill = i+16;
   // single lines are regular pipes
   setHline(802, 0, 1, 2);
   setVline(805, 0, 1, 2);
   blockSets.push_back(b);


   i = 520;
   constructEmptySet();
   b.name = "purple pipe with open center rev";
   b.triggerIndexStart = i;
   b.triggerIndexEnd   = 541;
   b.rectMode = 1;
   b.cornerUpperLeft   = i + 7;
   b.cornerUpperRight  = i + 6;
   b.cornerLowerLeft   = i + 5;
   b.cornerLowerRight  = i + 4;
   b.edgeLeft          = i - 3;
   b.edgeRight         = i - 4;
   b.edgeTop           = i - 1;
   b.edgeBottom        = i - 2;
   b.fill = -1;
   // single lines are regular pipes
   setHline(802, 0, 1, 2);
   setVline(805, 0, 1, 2);

   blockSets.push_back(b);

}


// passed a rect to draw from level editor
// bx and by are block positions in the level array
// rect is guaranteed to have bx1 < bx2 and by1 < by2
// if drawItem is part of a block set, use appropriate block from that set
void mwBlockSets::draw(int bx1, int bx2, int by1, int by2, int drawItemNum, int drawItemFlags, int level[][100])
{
   if (!loaded) init();

   int bw = bx2-bx1; // width
   int bh = by2-by1; // height

   int drawBlock = drawItemNum; // default

   // single block
   if (bw==0 && bh==0) level[bx1][by1] = drawBlock | drawItemFlags; // single block 1 x 1

   // vertical line
   if (bw==0 && bh>0)
   {
      int a = bx1;
      for (int b=by1; b<by2+1; b++) // cycle the range
      {
         drawBlock = drawItemNum; // default
         // search for block set
         for (const auto& s : blockSets)
            if (drawItemNum >= s.triggerIndexStart && drawItemNum <= s.triggerIndexEnd && s.vlineMode)
            {
               drawBlock = s.vLineMiddle;
               if (b == by1) drawBlock = s.vLineTop;
               if (b == by2) drawBlock = s.vLineBottom;
            }
         // draw the block
         level[a][b] = drawBlock |= drawItemFlags;
      }
   }

   // horizontal line
   if (bw>0 && bh==0)
   {
      int b = by1;
      for (int a=bx1; a<bx2+1; a++) // cycle the range
      {
         drawBlock = drawItemNum; // default
         // search for block set
         for (const auto& s : blockSets)
            if (drawItemNum >= s.triggerIndexStart && drawItemNum <= s.triggerIndexEnd && s.hlineMode)
            {
               drawBlock = s.hLineMiddle;
               if (a == bx1) drawBlock = s.hLineLeft;
               if (a == bx2) drawBlock = s.hLineRight;
            }
         // draw the block
         level[a][b] = drawBlock |= drawItemFlags;
      }
   }

   // rectangle where w and h are both > 0
   if (bw>0 && bh>0)
   {
      for (int a=bx1; a<bx2+1; a++)
         for (int b=by1; b<by2+1; b++)
         {
            drawBlock = drawItemNum; // default
            for (const auto& s : blockSets)
               if (drawItemNum >= s.triggerIndexStart && drawItemNum <= s.triggerIndexEnd && s.rectMode)
               {
                  drawBlock = s.fill;

                  if (b == by1) drawBlock = s.edgeTop;
                  if (b == by2) drawBlock = s.edgeBottom;

                  if (a == bx1)
                  {
                     if (b == by1)      drawBlock = s.cornerUpperLeft;
                     else if (b == by2) drawBlock = s.cornerLowerLeft;
                     else               drawBlock = s.edgeLeft;
                  }
                  if (a == bx2)
                  {
                     if (b == by1)      drawBlock = s.cornerUpperRight;
                     else if (b == by2) drawBlock = s.cornerLowerRight;
                     else               drawBlock = s.edgeRight;
                  }
               }

            // draw the block
            if (drawBlock != -1) level[a][b] = drawBlock |= drawItemFlags;
         } // end of cycle block range
   }
}


// from tile helper
// do blocks match exactly, or are they part of the same set?
bool mwBlockSets::compareBlock(int rb, int cb, int set)
{
   if (!loaded) init();

   // remove flags for comparison
   int r = rb & 1023;
   int c = cb & 1023;

   // exact match
   if (r == c) return true;

   if (set) // check if both belong to same set
   {
      for (const auto& s : blockSets)
         if (r >= s.triggerIndexStart && r <= s.triggerIndexEnd && c >= s.triggerIndexStart && c <= s.triggerIndexEnd) return true;      
   }
   return false;
}


