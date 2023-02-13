// n_network.cpp

#include "pm.h"

#include "n_network.h"


int NetworkDriver = -1;

// Initialize libnet and setup a driver to use. Returns 0 on success.
int NetworkInit() {

	NET_DRIVERLIST drivers;
	NET_DRIVERNAME *drivernames;

   // initialize libnet
	net_init();

	// configure libnet
	net_loadconfig(NULL);

   // detect drivers in the internet class
	drivers = net_detectdrivers(net_classes[NET_CLASS_INET].drivers);
	drivernames = net_getdrivernames(drivers);

	// use first detected driver
	NetworkDriver = drivernames[0].num;

	// clean up
	free(drivernames);

	// valid driver?
	if(NetworkDriver >= 0)
   {
		if(net_initdriver(NetworkDriver)) return 0;
      else
      {
         // Error: Couldn't initialize network driver
         return -1;
      }

   }
   else
   {
      // Error: Found no driver in the internet network class
      return -1;
   }
}
