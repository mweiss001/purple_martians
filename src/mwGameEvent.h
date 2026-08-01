// mwGameEvent.h

struct game_event
{
   int frame;
   int ev;
   int x;
   int y;
   int z1;
   int z2;
   int z3;
   int z4;
};

class mwGameEvent
{
   public:
   void add(int ev, int x, int y, int z1, int z2, int z3, int z4);
   std::vector<game_event> game_events;
};
extern mwGameEvent mGameEvent;


