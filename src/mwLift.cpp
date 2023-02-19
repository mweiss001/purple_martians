// mwLift.cpp

#include "pm.h"
#include "mwLift.h"


mwLift mwLifts;


mwLift::mwLift()
{
   initialize();
}

void mwLift::initialize(void)
{
   strcpy (lift_step_type_name[0], "BAD!");
   strcpy (lift_step_type_name[1], "Move");
   strcpy (lift_step_type_name[2], "Wait");
   strcpy (lift_step_type_name[3], "Prox");
   strcpy (lift_step_type_name[4], "End ");
   strcpy (lift_step_type_name[5], "Trig");
}
