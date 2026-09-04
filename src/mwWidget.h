// mwWidget.h

#include "mwRect.h"

struct listItem
{
   int value;
   std::string text;
};



class mwWidget
{
   private:
   void draw_widget_area(int x1, int y1, int x2, int y2, int q1);
   void draw_widget_text(int x1, int y1, int x2, int y2, int color, int left_justified, const char* msg);

public:


   bool mButtonToggle(int xType, int xa, int xb, int &ya, int bts, int r, int backgroundType, int frameType, int textType, int hcol, int highlight,
                     int &var, int v0, int v1, const char* t0, const char* t1, int bcol0, int bcol1, int tcol0, int tcol1, int fcol0, int fcol1, int disable_input);

   bool mButtonToggle(int xType, int xa, int xb, int yType, int ya, int yb, int r, int backgroundType, int frameType, int textType, int hcol, int highlight,
                     int &var, int v0, int v1, const char* t0, const char* t1, int bcol0, int bcol1, int tcol0, int tcol1, int fcol0, int fcol1, int disable_input);




   int togglec(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
                  int &var, const char* t, int text_col, int frame_col);

   int togglf(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
                  int &var, int flag, const char* t0, const char* t1 , int text_col0, int text_col1, int frame_col0, int frame_col1);

   int togglfc(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
               int &var, int flag, const char* t, int text_col, int frame_col);


   void togglec_log(int x1, int &y1, int x2, int bts, int q6, int ltn, int text_col, int frame_col);





   int mColorSelect(int xType, int xa, int xb, int &ya, int yb, int type, int &v1, int v2, const char* txt, int disable_input);
   int mColorSelect(int xType, int xa, int xb, int yType, int ya, int yb, int type, int &v1, int v2, const char* t, int disable_input);




   bool mButtonPD(int xType, int xa, int xb, int yType, int ya, int yb,  int r, int backgroundType, int bcol, int fcol, int hcol, int tcol, int text_just,  int type, int &var, int disable_input);

   bool mButtonPD(int xType, int xa, int xb, int &ya, int bts,  int r, int backgroundType, int bcol, int fcol, int hcol, int tcol, int text_just,  int type, int &var, int disable_input);



   void xyHelper(int xType, int xa, int xb, int yType, int ya, int yb, const char* txt, int &x1, int &y1, int &x2, int &y2);

   mwRect<int> xyHelper(int xType, int xa, int xb, int yType, int ya, int yb, const char* txt);


   bool mButton(mwRect<int> rect, int r, int backgroundType, int frameType, int textType, int bcol, int fcol, int tcol, int hcol, int highlight, const char* txt, int disable_input);
   bool mButton(int xType, int xa, int xb, int yType, int ya, int yb, int r, int backgroundType, int frameType, int textType, int bcol, int fcol, int tcol, int hcol, int highlight, const char* txt, int disable_input);
   bool mButton(int xType, int xa, int xb, int &ya, int bts, int r, int backgroundType, int frameType, int textType, int bcol, int fcol, int tcol, int hcol, int highlight, const char* txt, int disable_input);

   bool mButtonNB(int xType, int xa, int xb, int yType, int ya, int yb, int r, int backgroundType, int frameType, int textType, int bcol, int fcol, int tcol, int hcol, int highlight, const char* txt, int disable_input);

   bool mButtonCustom(int xType, int xa, int xb, int yType, int ya, int yb, int r, int backgroundType, int frameType, int textType, int bcol, int fcol, int tcol, int hcol, int highlight, int type, int &v1, int v2, int v3, int disable_input);


   bool mButtonSmallText(int xType, int xa, int xb, int yType, int ya, int yb, int r, int backgroundType, int frameType, int textType, int bcol, int fcol, int tcol, int hcol, int highlight, const char* txt);
   void drawWidgetSmallText(int x1, int y1, int x2, int y2, int color, int left_justified, const char* msg);


   bool          mCheckBox(int xType, int xa, int xb, int &ya, int bts, int frame_col, int &var, const char* t, int text_col, int box_col, bool disable_input);
   bool          mCheckBox(int xType, int xa, int xb, int yType, int ya, int yb, int frame_col, int &var, const char* t, int text_col, int box_col, bool disable_input);


   void mCheckBoxSmallText(int xType, int xa, int xb, int yType, int ya, int yb, int frame_col, int &var, const char* t, int text_col, int box_col);

   bool mButtonPlayerTile(int x1, int y1, int size, int tn, int fc, int hc, int highlight);

   bool mButtonTile(int x1, int y1, int size, int tn, bool &var, bool disable_input);

   bool mButtonTile2(int x1, int y1, int size, int tn, const char* t, bool disable_input);


   bool mButtonTile3(int xType, int xa, int xb, int yType, int ya, int yb, int r, int backgroundType, int frameType, int textType, int bcol, int fcol, int tcol, int hcol, int highlight, int x1, int y1, int size, int tn, const char* txt, int disable_input);



   bool mTrackInt(int xType, int xa, int xb, int yType, int ya, int yb,
                         int r, int backgroundType, int frameType, int textType,
                         int bcol, int fcol, int bar_col, int tcol, int hcol, int highlight,
                         int text_just, int &var, int ul, int ll, const char *txt, int disable_input);

   void mStepper(int xType, int xa, int xb, int yType, int ya, int yb,
                 int r, int backgroundType, int frameType, int textType,
                 int bcol, int fcol, int tcol, int hcol, int highlight,
                 int text_just, int &var, int ul, int ll, int inc, const char *txt);


   void mSliderFloat(int xType, int xa, int xb, int yType, int ya, int yb,
                     int r, int backgroundType, int frameType, int textType,
                     int bcol, int fcol, int bar_col, int tcol, int hcol, int highlight,
                     int text_just, float &var, float sul, float sll, float sinc, const char *txt, int update, bool disable_input);

   void mSliderInt(int xType, int xa, int xb, int yType, int ya, int yb,
                   int r, int backgroundType, int frameType, int textType, int bcol, int fcol, int bar_col, int tcol, int hcol, int highlight,
                   int text_just, int &var, float sul, float sll, float sinc, const char *txt, int update, bool disable_input);

   void mSliderInt0(int xType, int xa, int xb, int yType, int ya, int yb,
                   int r, int backgroundType, int frameType, int textType, int bcol, int fcol, int bar_col, int tcol, int hcol, int highlight,
                   int text_just, int &var, float sul, float sll, float sinc, const char *txt, const char *txt0, int update, bool disable_input);

   float drawSlider(int x1, int y1, int x2, int y2,
                    int r, int backgroundType,
                    int bcol, int fcol, int bar_col, int tcol, int text_just,
                    float sdx, float sul, float sll, int order, const char *msg);

   float drawSliderBar(float sdx, float sul, float sll, int x1, int y1, int x2, int y2, int dm, int col);

   float getSliderPosition(float sdx, float sul, float sll, int x1, int y1, int x2, int y2);

   float getSliderPositionMouse(float sul, float sll, float sinc, int x1, int y1, int x2, int y2);

   float getSliderPositionMouseAlt(float f, float sul, float sll, float sinc, int x1, int y1, int x2, int y2);

   void updateCall(int update);



   void mStepSliderFloat(int xType, int xa, int xb, int yType, int ya, int yb,
                         int r, int backgroundType, int frameType, int textType,
                         int bcol, int fcol, int bar_col, int tcol, int hcol, int highlight,
                         int text_just, float &var, float ul, float ll, float slinc, float stinc1, float stinc2, const char *txt, int update, bool disable_input);

   void mStepSliderInt(int xType, int xa, int xb, int yType, int ya, int yb,
                 int r, int backgroundType, int frameType, int textType,
                 int bcol, int fcol, int bar_col, int tcol, int hcol, int highlight,
                 int text_just, int &var, int ul, int ll, int slinc, int stinc1, int stinc2, const char *txt, int update, bool disable_input);

   void mStepSliderInt0(int xType, int xa, int xb, int yType, int ya, int yb,
                 int r, int backgroundType, int frameType, int textType,
                 int bcol, int fcol, int bar_col, int tcol, int hcol, int highlight,
                 int text_just, int &var, int ul, int ll, int slinc, int stinc1, int stinc2, const char *txt, const char *txt2, int update, bool disable_input);



   bool colorClickSlider(int type, float x1, float &y1, float x2, float bts, float &val, bool &hover, float &hover_val, ALLEGRO_COLOR c, bool &
                         changed_flag, bool display_only = false);

   void mToolTip(int xType, int xa, int xb, int yType, int ya, int yb, int r, int backgroundType, int frameType, int textType, int bcol, int fcol, int tcol, const char* txt, int tx1, int ty1, int tx2, int ty2);


   void mDropDownSetNextItem(std::vector<listItem> listItems, int & var, bool rollOver);
   void mDropDownSetPrevItem(std::vector<listItem> listItems, int & var, bool rollOver);
   void mDropDownDrawMain(mwRect<int> f, int r, std::string text, int textJust, int btype, int bcol, int fcol, bool crop);
   bool mDropDown(int xType, int xa, int xb, int yType, int ya, int yb, int r, int tjust, int btype, int bcol, int fcol, int hcol, std::vector<listItem> listItems, int & var, int d);



   bool mCheckBoxWithToolTip(int xType, int xa, int xb, int yType, int ya, int yb, int frame_col, int &var, const char* t, int text_col, int box_col, bool disable_input, const char* tt);



};
extern mwWidget mWidget;












