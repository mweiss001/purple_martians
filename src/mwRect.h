// mwRect.h
#ifndef MWRECT_H
#define MWRECT_H


template <typename T>
class mwRect
{
public:
   T x1;
   T y1;
   T x2;
   T y2;
   T w;
   T h;



private:

   // default constructor:
//   mwRect() : x1(0), y1(0), x2(0), y2(0), w(0), h(0) {}
   mwRect(T posX1, T posY1, T rectWidth, T rectHeight) { setXYWH(posX1, posY1, rectWidth, rectHeight); }


public:

   static mwRect fromX1Y1WH(   T x1, T y1, T w,  T h )  { return mwRect<T>(x1,    y1,      w,  h      );  }
   static mwRect fromX2Y2WH(   T x2, T y2, T w,  T h )  { return mwRect<T>(x2-w,  y2-h,    w,  h      );  }
   static mwRect fromX1Y1X2Y2( T x1, T y1, T x2, T y2)  { return mwRect<T>(x1,    y1,  x2-x1,  y2-y1  );  }


   void setWH(T rectWidth, T rectHeight)
   {
      w = rectWidth;
      h = rectHeight;
      x2 = x1 + w;
      y2 = y1 + h;
   }

   void setX1Y1(T x, T y)
   {
      x1 = x;
      y1 = y;
      x2 = x1 + w;
      y2 = y1 + h;
   }



   void setX2Y2WH(T posX2, T posY2, T rectWidth, T rectHeight)
   {
      x2 = posX2;
      y2 = posY2;
      w = rectWidth;
      h = rectHeight;
      x1 = x2 - w;
      y1 = y2 - h;
   }
   void setXYWH(T posX1, T posY1, T rectWidth, T rectHeight)
   {
      x1 = posX1;
      y1 = posY1;
      w = rectWidth;
      h = rectHeight;
      x2 = x1 + w;
      y2 = y1 + h;
   }
   void setX1Y1X2Y2(T posX1, T posY1, T posX2, T posY2)
   {
      x1 = posX1;
      y1 = posY1;
      x2 = posX2;
      y2 = posY2;
      w = posX2 - posX1;
      h = posY2 - posY1;
   }

   void round()
   {
      float factor = std::pow(10.0f, 0);
      x1 = std::round(x1 * factor) / factor;
      x2 = std::round(x2 * factor) / factor;
      y1 = std::round(y1 * factor) / factor;
      y2 = std::round(y2 * factor) / factor;

//      w = std::round(w * factor) / factor;
//      h = std::round(h * factor) / factor;


      w = x2 - x1;
      h = y2 - y1;



   }
   void trunc()
   {
      x1 = (int) x1;
      y1 = (int) y1;
      x2 = (int) x2;
      y2 = (int) y2;
      w = (int) w;
      h = (int) h;
   }

   void move(T xMove, T yMove )
   {
      x1+= xMove;
      y1+= yMove;
      x2 = x1 + w;
      y2 = y1 + h;
   }

   void resize(T wAdj, T hAdj )
   {
      w+= wAdj;
      h+= hAdj;
      x2 = x1 + w;
      y2 = y1 + h;
   }

   void setWidth(T newWidth)
   {
      w = newWidth;
      x2 = x1 + w;
   }

   void setHeight(T newHeight)
   {
      h = newHeight;
      y2 = y1 + h;
   }



   void multiply(T mul)
   {
      x1 *= mul;
      y1 *= mul;
      x2 *= mul;
      y2 *= mul;
      w = x2 - x1;
      h = y2 - y1;
   }



   void enforceLimits(T xMin, T yMin, T xMax, T yMax)
   {
      if (x1<xMin) x1 = xMin;
      if (x2<xMin) x2 = xMin;
      if (x1>xMax) x1 = xMax;
      if (x2>xMax) x2 = xMax;

      if (y1<yMin) y1 = yMin;
      if (y2<yMin) y2 = yMin;
      if (y1>yMax) y1 = yMax;
      if (y2>yMax) y2 = yMax;

      w = x2 - x1;
      h = y2 - y1;
   }



   void set_clipping_rectangle()
   {
      al_set_clipping_rectangle(x1, y1, w, h);
   }

   void print()
   {
      //printf("x1:%f  y1:%f    x2:%f  y2:%f   w:%f h:%f\n", x1, y1, x2, y2, w, h);
      printf("x1:%f  x2:%f  w:%f  --  y1:%f  y2:%f  h:%f\n", x1, x2, w, y1, y2, h);
   }




   T getArea()      const { return w * h; }
   T getPerimeter() const { return 2 * (w + h); }


   T XCenter()   const { return x1 + w/2; }
   T YCenter()   const { return y1 + h/2; }



   void swap(T& a, T& b)
   {
      T temp = a;
      a = b;
      b = temp;
   }





   void ensure_x1y1_less_than_x2y2()
   {
      if (x1 > x2) swap(x1, x2);
      if (y1 > y2) swap(y1, y2);
   }




   void draw_filled_rectangle(ALLEGRO_COLOR color, int size_adj = 0)
   {
      al_draw_filled_rectangle(x1-size_adj, y1-size_adj, x2+size_adj, y2+size_adj, color);
   }

   void draw_rectangle(ALLEGRO_COLOR color, float thickness, int size_adj = 0)
   {
      al_draw_rectangle(x1-size_adj, y1-size_adj, x2+size_adj, y2+size_adj, color, thickness);
   }

   void draw_rounded_rectangle(float rx, float ry, ALLEGRO_COLOR color, float thickness, int size_adj = 0)
   {
      al_draw_rounded_rectangle(x1-size_adj, y1-size_adj, x2+size_adj, y2+size_adj, rx, ry, color, thickness);
   }

   void draw_filled_rounded_rectangle(float rx, float ry, ALLEGRO_COLOR color, int size_adj = 0)
   {
      al_draw_filled_rounded_rectangle(x1-size_adj, y1-size_adj, x2+size_adj, y2+size_adj, rx, ry, color);
   }




   // check if a point is inside the rectangle
   bool contains(T x, T y) const
   {
      return (x >= x1 && x <= x2 && y >= y1 && y <= y2);
   }



   // check if a point is inside the rectangle
   bool contains(T x, T y, T &relx, T &rely) const
   {
      if (x >= x1 && x <= x2 && y >= y1 && y <= y2)
      {
         relx = x - x1;
         rely = y - y1;
         return true;
      }
      return false;
   }





   // Check if this rectangle overlaps with another rectangle
   bool intersects(const mwRect<T>& other) const
   {
      return (x1 < other.x2 && x2 > other.x1 && y1 < other.y2 && y2 > other.y1);
   }
};

#endif