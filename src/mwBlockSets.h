#ifndef PM_MWBLOCKSETS_H
#define PM_MWBLOCKSETS_H

#include <string>
#include <vector>

struct blockSet
{
   std::string name;
   int triggerIndexStart;
   int triggerIndexEnd;

   int rectMode;
   int hlineMode;
   int vlineMode;

   int fill;
   int single;

   int cornerUpperLeft;
   int cornerUpperRight;
   int cornerLowerLeft;
   int cornerLowerRight;
   int edgeLeft;
   int edgeRight;
   int edgeTop;
   int edgeBottom;

   int hLineLeft;
   int hLineMiddle;
   int hLineRight;

   int vLineTop;
   int vLineMiddle;
   int vLineBottom;

};


class mwBlockSets
{
   private:

      void init();
      std::vector<blockSet> blockSets;

      struct blockSet b;

      blockSet constructRectSet(std::string name, int i);
      blockSet constructFrameSet(std::string name, int i);
      blockSet constructHlineSet(std::string name, int i);

      void constructEmptySet();
      void setDefaultRect(int i);

      void setHline(int i, int left, int middle, int right);
      void setVline(int i, int top, int middle, int bottom);

      int loaded = 0;

   public:
      void draw(int bx1, int bx2, int by1, int by2, int drawItemNum, int drawItemFlags, int level[][100]);
      bool compareBlock(int rb, int cb, int set);
};

extern mwBlockSets mBlockSets;

#endif //PM_MWBLOCKSETS_H