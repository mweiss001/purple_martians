// mwGlobalLevelTool.h

class mwGlobalLevelTool
{
   public:
   void execute();
   void countTiles();

   int tileCount[1024][400] = {};

   private:
   void show_block_list(int blt[]);
   void remove_unused_tiles(int blt[]);
};
extern mwGlobalLevelTool mGlobalLevelTool;



