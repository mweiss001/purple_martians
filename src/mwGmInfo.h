// mwGmInfo.h


struct gmPlayerHitEnemy
{
   int frame{};
   int enemyNum{};
   int shotFiredFrame{};
   int hitsLeft{}; // 0 - dead
};


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
   std::vector<gmPlayerHitEnemy> enemyHits{};
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

      int testFire(int f, int p);



   private:
      void clear();
      void findPlayerTracks();
      void findPlayerTracksLastMoves();
      int getGmPlayerInfoIndex(int p, int f);

      void findDeaths();
      void findPurpleCoins();
      void findEnemyHits();

};

extern mwGmInfo mGmInfo;


