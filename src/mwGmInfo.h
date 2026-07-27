// mwGmInfo.h

struct gmPlayerInfoRecord
{
   int playerNum;
   int playerCol;
   std::string playerName;
   int startFrame;
   int endFrame;
   bool noEnd;
   int lastMoveFrame;

   std::vector<int> deaths;
   std::vector<int> purpleCoins;
};

class mwGmInfo
{
   public:
      void fill();

      bool addEventsToDatabase{};
      int lastFrame{};
      std::vector<gmPlayerInfoRecord> gmPlayerInfo;


   int levelDoneFrame{};
   int levelDonePlayer{};



   private:
      void clear();
      void findPlayerTracks();

      void findPlayerTracksLastMoves();



      int getGmPlayerInfoIndex(int p, int f);

      void findDeaths();
      void addDeath(int p, int f);

      void findPurpleCoins();
      void addPurpleCoin(int p, int f);

};

extern mwGmInfo mGmInfo;


