// mwPDE.h

class mwPDE
{
   private:
   void pde_swap(int s1, int s2);
   void pde_sort();
   void edit_text(int e);

   public:
   int load();
   void save();
   void run();

   int   PDEi[100][32];
   float PDEf[100][16];
   char  PDEt[100][20][40];

};
extern mwPDE mPDE;



