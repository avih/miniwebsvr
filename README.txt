The purpose of this branch is to redesign miniwebsvr as a library.

So far we have a generic TCP server which uses a callback function for logging and another to respond to each connection. The example program in example.cpp implements these two functions to create an HTTP server which generates a sample page. That's not very useful yet, as it doesn't serve even serve static HTML pages other than this sample, but it is still a work-in-progress.

The next step I'll take is to copy the HTTP code from the main branch of miniwebsvr and use that to make "stock" HTTP handler and logger callback functions. Once that is done is will be possible to create a simple fully-working web server using the following code:

	#include "miniwebsvr.h"
	int main(int argc, char *argv[])
	{
		miniwebsvr_listen(8080, 0, miniwebsvr_stock_http, ".", miniwebsvr_stock_file_log, "miniwebsvr.log");
		return 0;
	}
