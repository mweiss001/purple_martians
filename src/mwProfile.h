// mwProfile.h
#include <string>
#include <vector>

struct profilePoint
{
   std::string name;
   double time;
};

class mwProfile
{
   std::vector<profilePoint> profilePoints;

public:

   mwProfile() // default constructor
   {
      profilePoints.clear();
      add("init");
   };
   void add(std::string name)
   {
      double t = al_get_time();
      profilePoints.push_back({name, t});

   };
   void show()
   {
      double previous_time = 0;
      for (auto tmp: profilePoints)
      {
         double t = tmp.time;
         double current_time = t - previous_time;
         previous_time = t;
         printf("[%0.1f] - [%0.1f]   --  %s \n", t*1000, current_time*1000, tmp.name.c_str());
      }
   };
};


