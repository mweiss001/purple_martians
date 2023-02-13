// mwQuickGraph.cpp

#include "pm.h"
#include "mwQuickGraph.h"
#include "mwFont.h"
#include "mwColor.h"

mwQuickGraph mwQG[10];

mwQuickGraph::mwQuickGraph()
{
   initialize(1);
}

void mwQuickGraph::initialize(int typ)
{
   width = 200;
   height = 20;
   type = typ;

   y_axis_min = 0;
   y_axis_max = 25;
   y_axis_rng = 25;

   for (int s=0; s<4; s++)
   {
      series[s].active = 0;
      series[s].color = 0;
      series[s].name[0] = 0;
      for (int i=0; i<1000; i++)
         series[s].data[i] = 0;
   }
}


void mwQuickGraph::add_data(int s, double d)
{
   // slide all the data down
   for (int i=width-1; i>0; i--)
      series[s].data[i] = series[s].data[i-1];
   series[s].data[0] = d;
 //  printf("0:%f 1:%f 2:%f\n", series[s].data[0], series[s].data[1], series[s].data[2]);
}


void mwQuickGraph::draw_graph(int x, int y)
{


   int bc = 12+192; // background color
   int fc = 12;  // frame color
   //int tc = 15;  // title color
   int lc = 15;  // y axis label color
   //int pc = 13;  // plot line color
   float ls = 0; // plot line size

   float gx1 = x;
   float gx2 = x+width;
   float gxc = (gx1 + gx2)/2;

   float gx13 = gx1 + (gx2-gx1)*1/4;
   float gx23 = gx1 + (gx2-gx1)*3/4;


   float gx16 = gx1 + (gx2-gx1)*1/6;
   float gx56 = gx1 + (gx2-gx1)*5/6;

   float gy1 = y;
   float gy2 = y+height;
   float gyc = (gy1 + gy2)/2;

   al_draw_filled_rectangle(gx1, gy1, gx2, gy2, mC.pc[bc]);     // fill
   al_draw_rectangle(       gx1, gy1, gx2, gy2, mC.pc[fc], 0);  // frame
   al_draw_line(            gx1, gyc, gx2, gyc, mC.pc[fc], 0);  // center line

   //if (type == 1) al_draw_textf(mF.pr8, mC.pc[series[0].color], gxc, gyc-4,  ALLEGRO_ALIGN_CENTER, "%s:%0.0f%%", series[0].name, series[0].data[0]);

   if (type == 1)
   {
      al_draw_textf(mF.pr8, mC.pc[series[1].color], gx16, gyc-4,  ALLEGRO_ALIGN_CENTER, "%s:%0.0f%%", series[1].name, series[1].data[0]);
      al_draw_textf(mF.pr8, mC.pc[series[2].color], gx56, gyc-4,  ALLEGRO_ALIGN_CENTER, "%s:%0.0f%%", series[2].name, series[2].data[0]);
      al_draw_textf(mF.pr8, mC.pc[series[3].color], gxc,  gyc-4,  ALLEGRO_ALIGN_CENTER, "%s:%0.0f%%", series[3].name, series[3].data[0]);
   }

   if (type == 2)
   {
      al_draw_textf(mF.pr8, mC.pc[series[0].color], gx13, gyc-4,  ALLEGRO_ALIGN_CENTER, "%s:%0.0f", series[0].name, series[0].data[0]);
      al_draw_textf(mF.pr8, mC.pc[series[1].color], gx23, gyc-4,  ALLEGRO_ALIGN_CENTER, "%s:%0.0f", series[1].name, series[1].data[0]);
   }

   // auto range
   int ymax = -9999;
   int ymin = 9999;
   for (int i=0; i<width-1; i++)
   {
      for (int s=0; s<4; s++)
         if (series[s].active)
         {
            if (series[s].data[i] > ymax) ymax = series[s].data[i];
            if (series[s].data[i] < ymin) ymin = series[s].data[i];
         }

   }



   if (type == 1)
   {
      y_axis_max = 100;
      if (ymax < 50)  y_axis_max = 50;
      if (ymax < 25)  y_axis_max = 25;
      y_axis_rng = y_axis_max - y_axis_min;
   }

   if (type == 2)
   {
      y_axis_min = ymin - 2;
      y_axis_max = ymax + 2;

      if (y_axis_min < -100) y_axis_min = -100;
      if (y_axis_max >  100) y_axis_max = 100;

      y_axis_rng = y_axis_max - y_axis_min;
   }

   al_draw_textf(mF.pixl, mC.pc[lc], gx2+2, gy1-6, 0, "%2.0f", y_axis_max);
   al_draw_textf(mF.pixl, mC.pc[lc], gx2+2, gyc-6, 0, "%2.0f", y_axis_min + y_axis_rng/2);
   al_draw_textf(mF.pixl, mC.pc[lc], gx2+2, gy2-6, 0, "%0.0f", y_axis_min);

   for (int i=0; i<width-1; i++)
   {

      for (int s=0; s<4; s++)
         if (series[s].active)
         {
            // get this point and next
            int x1 = gx2 - i;
            int x2 = gx2 - (i+1);

            double d1 = series[s].data[i];
            double d2 = series[s].data[i+1];

            // convert range
            double y1 = gy2 - ((d1 - y_axis_min) / y_axis_rng)*height;
            double y2 = gy2 - ((d2 - y_axis_min) / y_axis_rng)*height;

            al_draw_line(x1, y1, x2, y2, mC.pc[series[s].color], ls);
         }
   }
}

