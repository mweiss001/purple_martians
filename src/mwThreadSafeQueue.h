
#include "mwPlayer.h"

#include "sqlite3.h"

#include <thread>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>

#include "mwRollingAverage.h"


class mwThreadSafeQueue
{
   public:
      mwThreadSafeQueue();
      void add();

   private:
      std::jthread m_thread;

      std::queue<client_status_buffer_row> q;

      std::mutex m;
      std::condition_variable cv;
      void sqlite_consumer(std::stop_token stoken);
      bool pop(client_status_buffer_row &row);

      std::deque<client_status_buffer_row> q2;
      void dumpQ2(sqlite3 *db, sqlite3_stmt *client_status_insert_stmt);

      mwRollingAverage time_ra;


};

extern mwThreadSafeQueue mThreadSafeQueue;
