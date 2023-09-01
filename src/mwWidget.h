// mwWidget.h

class mwWidget
{
   private:
   void draw_slider_frame(int x1, int y1, int x2, int y2, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7);
   void draw_slider_text(int x1, int y1, int x2, int y2, int q2, int q5, const char* msg);
   float get_slider_position(float sdx, float sul, float sll, int x1, int y1, int x2, int y2);
   float get_slider_position2(float sul, float sll, float sinc, int q4 ,int x1, int y1, int x2, int y2);
   float get_slider_position3(float f, float sul, float sll, float sinc, int q4, int x1, int y1, int x2, int y2);
   float draw_slider_bar(float sdx, float sul, float sll, int x1, int y1, int x2, int y2, int dm, int col);
   float draw_slider(int x1, int y1, int x2, int y2, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7, float sdx, float sul, float sll, int order, const char* msg);

   public:

   void slider0(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
                    int &var, float sul, float sll, float sinc, const char *txt, const char *txt2);
   void slideri(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
                    int &var, float sul, float sll, float sinc, const char *txt);
   void sliderf(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
                    float &var, float sul, float sll, float sinc, const char *txt);




   float get_slider_position2nb(float sul, float sll, float sinc, int q4 ,int x1, int y1, int x2, int y2);



   void sliderfnb(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
                 float &var, float sul, float sll, float sinc, const char *txt);






   int button(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7);

   int buttont(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7, const char* txt);
   int buttont_nb(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7, const char* txt);


   int buttontca(int xc, int &y1, int xd, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7, const char* txt);
   int buttontcb(int xc, int &y1, int xd, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7, const char* txt);


   int buttontt(int x1, int &y1, int x2, int bts, int tn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7, const char* txt);
   int buttonp(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7, int &var);
   void colsel(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7);
   int toggle(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
                  int &var, const char* t0, const char* t1 , int text_col0, int text_col1, int frame_col0, int frame_col1);
   int togglf(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
                  int &var, int flag, const char* t0, const char* t1 , int text_col0, int text_col1, int frame_col0, int frame_col1);

   int togglec(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
                  int &var, const char* t, int text_col, int frame_col);

   void togglec_log(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
               int ltn, int text_col, int frame_col);





};
extern mwWidget mWidget;












