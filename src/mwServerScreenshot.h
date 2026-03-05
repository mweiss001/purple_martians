#ifndef PM_MWSERVERSCREENSHOT_H
#define PM_MWSERVERSCREENSHOT_H

class mwServerScreenshot
{
   public:
      void start();

   private:
      void save();
      bool busy = false;
};
extern mwServerScreenshot mServerScreenshot;
#endif //PM_MWSERVERSCREENSHOT_H