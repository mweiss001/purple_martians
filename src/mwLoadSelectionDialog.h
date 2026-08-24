
#include "mwListBox.h"
#include "mwRect.h"
#include "mwWindow.h"
#include "mwWindowManager.h"

class mwLoadSelectionDialog
{

public:

   int run();

private:

   mwWindowManager mWM = mwWindowManager(3);
   void draw_file_select(mwWindow w);
   void draw_selection(mwWindow &w);
   void redraw_callback();

   mwListBox mListBox;

   int quit = 0;
   int retval = 0;





};

extern mwLoadSelectionDialog mLoadSelectionDialog;


