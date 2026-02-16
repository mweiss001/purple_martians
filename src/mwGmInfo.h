// mwGmInfo.h

struct gmPlayerInfoRecord
{
   int playerNum;
   int playerCol;
   std::string playerName;
   int startFrame;
   int endFrame;
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


   private:
      void clear();
      void findPlayerTracks();

      int getGmPlayerInfoIndex(int p, int f);

      void findDeaths();
      void addDeath(int p, int f);

      void findPurpleCoins();
      void addPurpleCoin(int p, int f);

      int levelDoneFrame{};
      int levelDonePlayer{};
};

extern mwGmInfo mGmInfo;


