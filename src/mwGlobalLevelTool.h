// mwGlobalLevelTool.h

class mwGlobalLevelTool
{
   public:
   void execute();
   void countTiles();

   void changeTileNumber(int oldNumber, int newNumber, int level = 0);


   int tileCount[1024][400] = {};

   private:
   void show_block_list(int blt[]);
   void remove_unused_tiles(int blt[]);
};
extern mwGlobalLevelTool mGlobalLevelTool;



