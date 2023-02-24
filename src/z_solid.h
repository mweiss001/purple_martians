// z_solid.h

//int is_solid( int b, int c, int type);
//int is_solidu(int b, int c, int type);
int is_right_solid(int solid_x, int solid_y, int lift_check, int type);
int is_left_solid( int solid_x, int solid_y, int lift_check, int type);
int is_down_solid( int solid_x, int solid_y, int lift_check, int type);
int is_up_solid(   int solid_x, int solid_y, int lift_check, int type);

float is_up_solidf(   float fx, float fy, float fmove, int dir);
float is_down_solidf( float fx, float fy, float fmove, int dir);
float is_left_solidf( float fx, float fy, float fmove, int dir);
float is_right_solidf(float fx, float fy, float fmove, int dir);
