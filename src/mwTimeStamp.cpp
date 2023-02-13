// mwTimeStamp.cpp
#include "mwTimeStamp.h"
#include "pm.h"
#include "mwProgramState.h"


mwTimeStamp mwTS;

double t0=0;

void mwTimeStamp::init_timestamps(void)
{
   timestamps_index = 0;
   for (int i=0; i<10000; i++)
   {

      timestamps[i].frame0 = 0;
      timestamps[i].frame1 = 0;
      timestamps[i].frame2 = 0;
      timestamps[i].type   = 0;
      timestamps[i].t0     = 0;
      timestamps[i].t1     = 0;
      timestamps[i].t2     = 0;
   }
}

void mwTimeStamp::add_timestamp(int type, int f1, int f2, double t1, double t2)
{
   if (timestamps_index < 9998)
   {
      timestamps[timestamps_index].frame0 = mwPS.frame_num;
      timestamps[timestamps_index].frame1 = f1;
      timestamps[timestamps_index].frame2 = f2;
      timestamps[timestamps_index].type   = type;
      timestamps[timestamps_index].t0     = al_get_time();
      timestamps[timestamps_index].t1     = t1;
      timestamps[timestamps_index].t2     = t2;

      timestamps_index++;
   }
}

int mwTimeStamp::get_timestamp(int f, int type, double &res)
{
   for (int i=timestamps_index; i>=0; i--)
   {
      if ((timestamps[i].frame0 == f) && (timestamps[i].type == type))
      {
         res = timestamps[i].t0;
         return 1;
      }
   }
   return 0;
}



int mwTimeStamp::get_newest_timestamp(int type, double &res)
{
   for (int i=timestamps_index; i>=0; i--)
   {
      if (timestamps[i].type == type)
      {
         res = timestamps[i].t0;
         return 1;
      }
   }
   return 0;
}
int mwTimeStamp::get_delta(int f0, int type0, int f1, int type1, double &res)
{
   double t0 = -1, t1 = -1;
   for (int i=timestamps_index; i>=0; i--)
   {
      if ((timestamps[i].frame0 == f0) && (timestamps[i].type == type0)) t0 = timestamps[i].t0;
      if ((timestamps[i].frame0 == f1) && (timestamps[i].type == type1)) t1 = timestamps[i].t0;
   }
   if ((t0 != -1) && (t1 != -1))
   {
      res = t1-t0;
      return 1;
   }
   if (t0 == -1) printf("did not find timestamp type:%d for frame:%d\n", type0, f0);
   if (t1 == -1) printf("did not find timestamp type:%d for frame:%d\n", type1, f1);
   return 0;
}

