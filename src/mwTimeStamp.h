// mwTimeStamp.h


struct timestamp
{
   int frame0;
   int frame1;
   int frame2;
   int type;
   double t0;
   double t1;
   double t2;
};


class mwTimeStamp
{
   public:
   void init_timestamps(void);

   void add_timestamp(int type, int f1, int f2, double t1, double t2);
   int get_timestamp(int f, int type, double &res);
   int get_newest_timestamp(int type, double &res);
   int get_delta(int f0, int type0, int f1, int type1, double &res);




   double timestamp_frame_start;
   int timestamps_index;
   struct timestamp timestamps[10000];

};
extern mwTimeStamp mwTS;




