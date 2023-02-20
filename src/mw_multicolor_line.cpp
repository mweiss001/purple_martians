// mw_mulitcolor_line.cpp
#include "pm.h"
#include "mwColor.h"


int check_and_draw(double x1, double y1, double line_length, double line_xinc, double line_yinc, double za, double zb, int col, float thickness, int &segments_drawn, int &lco)
{
   int debug_print = 0;
   char t[1024] = {0};
   sprintf(t, "\n");
   int skip = 0;
   int end_adj = 0;
   // these checks mean that the segment is entirely not on the line
   if (za > line_length) skip = 1; // za after line end
   if (zb < 0)           skip = 1; // zb before line start

   if (skip)
   {
      if (debug_print) sprintf(t, "skip\n");
   }
   else  // at least one point is on the line
   {
      if (za < 0) // za is before line start
      {
         if (debug_print) sprintf(t, "start_adj\n");
         za = 0;
      }
      if (zb > line_length) // zb is after line end
      {
         lco = line_length - zb;
         if (debug_print) sprintf(t, "end_adj - lco:%d\n", lco);
         zb = line_length;
         end_adj = 1;
      }
      double xa = x1 + za * line_xinc; // convert to screen coordinates
      double ya = y1 + za * line_yinc;
      double xb = x1 + zb * line_xinc;
      double yb = y1 + zb * line_yinc;
      if (debug_print) printf("%s   drawn at za:%3.0f zb:%3.0f\n", t, za, zb);
      al_draw_line(xa, ya, xb, yb, mC.pc[col], thickness);
      segments_drawn++;
   }
   return end_adj;
}

int mw_draw_line3(double x1, double y1, double x2, double y2, float thickness, int c0_val, int c0_col, int c1_val, int c1_col, int c2_val, int c2_col, int &line_color_offset)
{
   int debug_print = 0;

   if (debug_print) printf("\nmw_dl x1:%3.1f y1:%3.1f x2:%3.1f y2:%3.1f ", x1, y1, x2, y2);

   int done = 0;
   int lco = 0; // local lco to pass to sub fucntion
   int segments_drawn = 0;

   int color_span = c0_val + c1_val + c2_val;
   if ((color_span < 1) || ((c1_col == 0) && (c2_col == 0))) al_draw_line(x1, y1, x2, y2, mC.pc[c0_col], thickness);
   else
   {
      if (debug_print) printf("lco:%d color_span:%d\n", line_color_offset, color_span); // line_color_offset is in pixels, from 0 to color_span
      double line_xspan = x2-x1;
      double line_yspan = y2-y1;
      double line_length = sqrt(pow(line_xspan, 2) + pow(line_yspan, 2));
      if (line_length != 0)
      {
         double line_xinc = line_xspan / line_length; // one unit of these are one unit of vector move
         double line_yinc = line_yspan / line_length;
         int section = 0;
         while (!done)
         {
            if (debug_print) printf("Section:%d\n", section);
            double section_z_start = ((section-1) * color_span) - line_color_offset;

            double za = section_z_start;   // start position for drawing 1st segment
            double zb = za + c0_val;
            if (debug_print) printf("1st segment za:%3.0f zb:%3.0f ", za, zb);
            if (check_and_draw(x1, y1, line_length, line_xinc, line_yinc, za, zb, c0_col, thickness, segments_drawn, lco))
            {
                done = 1;
                line_color_offset = lco - c2_val - c1_val;
            }
            if (!done)
            {
               za = section_z_start + c0_val;   // start position for drawing 2nd segment
               zb = za + c1_val;
               if (debug_print) printf("2nd segment za:%3.0f zb:%3.0f ", za, zb);
               if (check_and_draw(x1, y1, line_length, line_xinc, line_yinc, za, zb, c1_col, thickness, segments_drawn, lco))
               {
                   done = 1;
                   line_color_offset = lco - c2_val;
               }
               if (!done)
               {
                  za = section_z_start + c0_val + c1_val;   // start position for drawing 3rd segment
                  zb = za + c2_val;
                  if (debug_print) printf("3rd segment za:%3.0f zb:%3.0f ", za, zb);
                  if (check_and_draw(x1, y1, line_length, line_xinc, line_yinc, za, zb, c2_col, thickness, segments_drawn, lco))
                  {
                      done = 1;
                      line_color_offset = lco;
                  }
               }
            }
            section++;
         }
      }
   }
   return segments_drawn;
}

int mw_draw_line2(double x1, double y1, double x2, double y2, float thickness, int c0_val, int c0_col, int c1_val, int c1_col, int &line_color_offset)
{
   int debug_print = 0;

   if (debug_print) printf("\nmw_dl x1:%3.1f y1:%3.1f x2:%3.1f y2:%3.1f ", x1, y1, x2, y2);

   int done = 0;
   int lco = 0; // local lco to pass to sub fucntion
   int segments_drawn = 0;

   int color_span = c0_val + c1_val;
   if ((color_span < 1) || (c1_col == 0)) al_draw_line(x1, y1, x2, y2, mC.pc[c0_col], thickness);
   else
   {
      if (debug_print) printf("lco:%d color_span:%d\n", line_color_offset, color_span); // line_color_offset is in pixels, from 0 to color_span
      double line_xspan = x2-x1;
      double line_yspan = y2-y1;
      double line_length = sqrt(pow(line_xspan, 2) + pow(line_yspan, 2));

      if (line_length != 0)
      {
         //printf("lxs:%f lys:%f  ls:%f\n", line_xspan, line_yspan, line_span);

         double line_xinc = line_xspan / line_length; // one unit of these are one unit of vector move
         double line_yinc = line_yspan / line_length;
         int section = 0;
         while (!done)
         {
            if (debug_print) printf("Section:%d\n", section);
            double section_z_start = ((section-1) * color_span) - line_color_offset;

            double za = section_z_start;   // start position for drawing 1st segment
            double zb = za + c0_val;
            if (debug_print) printf("1st segment za:%3.0f zb:%3.0f ", za, zb);
            if (check_and_draw(x1, y1, line_length, line_xinc, line_yinc, za, zb, c0_col, thickness, segments_drawn, lco))
            {
                done = 1;
                line_color_offset = lco - c1_val;
            }
            if (!done)
            {
               za = section_z_start + c0_val;   // start position for drawing 2nd segment
               zb = za + c1_val;
               if (debug_print) printf("2nd segment za:%3.0f zb:%3.0f ", za, zb);
               if (check_and_draw(x1, y1, line_length, line_xinc, line_yinc, za, zb, c1_col, thickness, segments_drawn, lco))
               {
                   done = 1;
                   line_color_offset = lco;
               }
            }
            section++;
         }
      }
   }
   return segments_drawn;
}
