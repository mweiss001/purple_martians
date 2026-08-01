// mwGmInfo.h

struct gmPlayerInfoRecord
{
   int playerNum{};
   int playerCol{};
   std::string playerName{};
   int startFrame{};
   int endFrame{};
   bool noEnd{};
   int lastMoveFrame{};

   std::vector<int> playerDeaths{};
   std::vector<int> coinsCollected{};
   std::vector<int> enemiesKilled{};
   std::vector<int> enemiesShot{};



};

struct gmPlayerHitEnemy
{
   int frame{};
   int enemyNum{};
   bool type{}; // ( 0 = shot  1 = explosion)
   int indexNum{}; // shotNum or itemNum
   int shotFiredFrame{};
   int hitsLeft{}; // 0 - dead
};


class mwGmInfo
{
   public:
      void fill();
      void add();

      bool completed{};
      int lastFrame{};

      int levelDoneFrame{};
      int levelDonePlayer{};

      int playerDeaths{};
      int coinsCollected{};
      int enemiesKilled{};
      int enemiesShot{};

      std::vector<gmPlayerInfoRecord> gmPlayerInfo{};


      int countTotalPurpleCoinsCollected();



   private:
      void clear();
      void findPlayerTracks();
      void findPlayerTracksLastMoves();
      int getGmPlayerInfoIndex(int p, int f);

      void findDeaths();
      void findPurpleCoins();
      void findEnemiesKilled();
      void findEnemiesShot();

};

extern mwGmInfo mGmInfo;


