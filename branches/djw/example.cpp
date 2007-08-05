#include <cstdio>
#include <cstring>
#include <dirent.h>
#include <ctime>

#include "miniwebsvr.h"

void logger(void *param, const char *message)
{
	fprintf(stderr, "%s\n", message);	// send error messages to the error stream
}
void generate_sample_page(void *param, miniwebsvr_connection connection)
{
	const int line_size = 1000;
	char line[line_size], get_line[line_size] = "";
	int line_length;

	// read header
	while (0 < (line_length = miniwebsvr_read_line(connection, line, line_size)))
	{
		if (strncmp(line, "GET ", 4) == 0)
			strcpy(get_line, line);
	}

	// send response headers
	strncpy(line, "HTTP/1.1 201 Created\15\12Content-Type: text/html\15\12Server: MiniWebSvr/0.0.8 (djw branch)\15\12", line_size);
	miniwebsvr_send(connection, line, strlen(line));
	time_t curtime;
	struct tm *loctime;
	curtime = time (NULL);
	loctime = gmtime (&curtime);
	strftime(line, line_size,"Date: %a, %d %b %Y %I:%M:%S GMT\15\12\15\12",loctime);
	miniwebsvr_send(connection, line, strlen(line));

	// send HTML

	strncpy(line, "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2//EN\">\15\12<HTML><HEAD><TITLE>Sample Page</TITLE></HEAD><BODY><h1>Sample Page</h1><p>This is a simple sample page to show you that the server is working (even if it isn't doing anything useful).</p><p>Your browser sent the following request:</p><tt>", line_size);
	miniwebsvr_send(connection, line, strlen(line));
	miniwebsvr_send(connection, get_line, strlen(get_line));
	strftime(line, line_size,"</tt><p>This page was generated on %a, %d %b %Y %I:%M:%S GMT</p></BODY></HTML>",loctime);
	miniwebsvr_send(connection, line, strlen(line));
}

int main(int argc, char *argv[])
{
	miniwebsvr_listen(8080, 0, generate_sample_page, 0, logger, 0);
	return 0;
}


