#include "libminiwebsvr.h"

#include <stdio.h>
#include <pthread.h>

void logger(char* message) // Logger function example
{
	printf("MiniWebSvr: %s\n", message);
}

int main()
{
	pthread_t server;
	server_config* config;

	config = (server_config*) initconfig(); // At first, get pointer to server configuration..
	config->logger_hook = &(logger); // You can set your own logger. It must be a function like "void func(char*)"
	config->thread_pool_adjust = 10; // Set a thread_pool_adjust size..

	config->port = 2255; // Port to listen

	server = server_start(); // Then start server
	while(1)
	{

	}

	return 0;
}
