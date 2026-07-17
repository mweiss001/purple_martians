// mwGlobalLevelTool.h

class mwGlobalLevelTool
{
   public:
   void execute();

   int tileCount[NUM_TILES][400] = {};
   void countTiles();


   void replaceTileIfMatch(int &tile, int match, int replace);
   void changeBlockNumber(int oldNum, int newNum);
   void changeTileNumberRange(int oldNumber, int newNumber, int range = 1);

};
extern mwGlobalLevelTool mGlobalLevelTool;



