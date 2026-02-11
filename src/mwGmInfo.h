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
   std::vector<gmPlayerInfoRecord> gmPlayerInfo;


   void setTestData();

   void printTestData();


   void fill();

   void draw(int x1, int y1, int x2, int bts);


};

extern mwGmInfo mGmInfo;


