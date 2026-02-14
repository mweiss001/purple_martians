// mwGmInfo.h

struct gmPlayerInfoRecord
{
   int playerNum;
   int playerCol;
   std::string playerName;
   int startFrame;
   int endFrame;
};

class mwGmInfo
{

   public:

   void fill();
   void showTimeline(int x1, int y1, int x2, int bts, int col);

   void showTimelineTracks(int y1, int x2, int bts, int ls, int x1);


   int lastFrame{};

   std::vector<gmPlayerInfoRecord> gmPlayerInfo;


   private:


   void clear();
   void findLevelDone();
   int levelDoneFrame{};
   int levelDonePlayer{};
};

extern mwGmInfo mGmInfo;


