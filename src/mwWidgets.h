// mwWidgets.h

void draw_slider_frame(int x1, int y1, int x2, int y2, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7);
void draw_slider_text(int x1, int y1, int x2, int y2, int q2, int q5);
float get_slider_position(float sdx, float sul, float sll, int x1, int y1, int x2, int y2);
float get_slider_position2(float sul, float sll, float sinc, int q4 ,int x1, int y1, int x2, int y2);
float get_slider_position3(float f, float sul, float sll, float sinc, int q4, int x1, int y1, int x2, int y2);
float draw_slider_bar(float sdx, float sul, float sll, int x1, int y1, int x2, int y2, int dm, int col);
float draw_slider(int x1, int y1, int x2, int y2, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7, float sdx, float sul, float sll, int order);
void mdw_slider0(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
                 int &var, float sul, float sll, float sinc, const char *txt, const char *txt2);
void mdw_slideri(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
                 int &var, float sul, float sll, float sinc, const char *txt);
void mdw_sliderf(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
                 al_fixed &var, float sul, float sll, float sinc, const char *txt);
void mdw_sliderd(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
                 float &var, float sul, float sll, float sinc, const char *txt);
void set_trigger_event(int i, int ev0, int ev1, int ev2, int ev3);
int get_frame_size(int num);
void set_frame_size(int num, int frame_size);
int mdw_button(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7);

int mdw_buttont(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7, const char* txt);
int mdw_buttont_nb(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7, const char* txt);


int mdw_buttontca(int xc, int &y1, int xd, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7, const char* txt);

int mdw_buttontt(int x1, int &y1, int x2, int bts, int tn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7, const char* txt);
void mdw_buttonp(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7, int &var);
void mdw_colsel(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7);
int mdw_toggle(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
               int &var, const char* t0, const char* t1 , int text_col0, int text_col1, int frame_col0, int frame_col1);
int mdw_togglf(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
               int &var, int flag, const char* t0, const char* t1 , int text_col0, int text_col1, int frame_col0, int frame_col1);

int mdw_togglec(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
               int &var, const char* t, int text_col, int frame_col);

