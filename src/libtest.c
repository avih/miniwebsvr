#include "os_compat.h"
#include "server.h"

#include "libminiwebsvr.h"

#include <stdio.h>
#include <pthread.h>

server_config* conf = NULL;

void logger(char* message) // Logger function example
{
	printf("MiniWebSvr: %s\n", message);
}

void php_hook(struct server_struct* inst, char* filename, char* buffer, int size)
{
	FILE *php;
	char line[5000], command[255];// = "/usr/bin/php ";
	
	strcat(&command, "/usr/bin/php-cgi ");

	filename = strrchr(filename, '/');
	filename++;

	strcat(&command, filename);
	
	DebugMSG(command);

	setHeader_respval(inst, 200);
//	setHeader_filename(inst, filename);

	buffer = malloc(1024);
	printHeader(inst, 0, buffer, 1000, 0);
	
	send(inst->sock, buffer, 1000, MSG_NOSIGNAL);

	php = (FILE*)popen(command,"r");

	while ( fgets( line, sizeof line, php))
	{
		send(inst->sock, line, strlen(line), MSG_NOSIGNAL);
   	}
	pclose(php);

	printf("PHP Hook called\n");
}

int main()
{
	pthread_t server;
	
	conf = (server_config*) initconfig(); // At first, get pointer to server configuration..
	conf->logger_hook = &(logger); // You can set your own logger. It must be a function like "void func(char*)"
	conf->thread_pool_adjust = 10; // Set a thread_pool_adjust size..

	conf->port = 2255; // Port to listen
	
	conf->filehook = malloc(sizeof(hook_list)); // This feature needs many work, before we can use it. Maybe hash-table or tree will be much faster, than simple table in memory
	conf->filehook->next = NULL;
	conf->filehook->fileext = "php";
	conf->filehook->hook = &php_hook;

	server = server_start(); // Then start server
	while(1)
	{

	}

	return 0;
}
