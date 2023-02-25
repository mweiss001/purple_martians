// mwMulticolorLine.h

class mwMulticolorLine
{
   public:
   int draw_line2(double x1, double y1, double x2, double y2, float thickness, int c0_val, int c0_col, int c1_val, int c1_col, int &line_color_offset);
   int draw_line3(double x1, double y1, double x2, double y2, float thickness, int c0_val, int c0_col, int c1_val, int c1_col, int c2_val, int c2_col, int &line_color_offset);

   private:
   int check_and_draw(double x1, double y1, double line_length, double line_xinc, double line_yinc, double za, double zb, int col, float thickness, int &segments_drawn, int &lco);

};
extern mwMulticolorLine mMulticolorLine;


