#include "pm.h"
#ifdef NETPLAY

int NetworkDriver = -1;

/* Initialize libnet and setup a driver to use. Returns 0 on success. */
int NetworkInit() {

	NET_DRIVERLIST drivers;
	NET_DRIVERNAME *drivernames;

   /* Tell libnet to initialize itself. */
	net_init();

	/* Tell libnet to configure itself. */
	net_loadconfig(NULL);

   /* Detect drivers in the "Internet" class and get their names. */

	drivers = net_detectdrivers(net_classes[NET_CLASS_INET].drivers);
	drivernames = net_getdrivernames(drivers);

	/* Use first detected driver. */
	NetworkDriver = drivernames[0].num;

	/* Always clean up. */
	free(drivernames);

	/* Got a valid driver? */
	if(NetworkDriver >= 0) {

		if(net_initdriver(NetworkDriver)) {
			return 0;
		} else {
			/* Error: Couldn't initialize the network driver. */
			return -1;
		}

	} else {
		/* Error: Found no driver in the internet network class. */
		return -1;
	}
}
#endif
