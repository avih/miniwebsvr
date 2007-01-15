/*  Miniweb - A small webserver
    Copyright (C) 2007  Nickolas Antonie Grigoriadis
    E-Mail: nagrigoriadis@gmail.com

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

#include "header.h"

void server_close(struct server_struct *inst) 
{
	closesocket(inst->sock);
	free(inst);
}

int server_charpos(const struct server_struct *inst, const char chr) 
{
	unsigned int i;
	
	for (i=inst->buffer_pos;i<inst->buffer_size;++i) 
	{
		if (inst->buffer[i] == chr) return i;
	}
	return -1;
}

int server_readln(struct server_struct *inst, char *str, const unsigned int strsize) 
{
	unsigned int strpos;
        int retval;

	// Check if in the buffer
	if ((strpos = server_charpos(inst,'\n')) != -1) 
	{
		// Whole string is in buffer
		strpos -= inst->buffer_pos+1;
		
		// Copy string to given buffer
		if (strpos > strsize) strpos=strsize;
		memcpy(str,inst->buffer+inst->buffer_pos,strpos);
		inst->buffer_pos+=strpos+2; 
	} 
	else 
	{
		// Copy remaining data in buffer
		strpos=inst->buffer_size-inst->buffer_pos;
		if (strpos > strsize) strpos=strsize;
		memcpy(str,inst->buffer+inst->buffer_pos,strpos);
	
		// Assemble rest of string out of multiple buffers
		while (strpos < strsize) 
		{
			// Get data from stream
			inst->buffer_pos=0;
			inst->buffer_size = recv(inst->sock,inst->buffer,SERVER_BUFFER_SIZE,0);

			if (inst->buffer_size == SOCKET_ERROR) 
			{
				fprintf(stderr,"recv() failed: error %d\n",WSAGetLastError());
				//server_close(inst);
				return -1;
			}
			
			retval=server_charpos(inst,'\n');
			if (retval == -1) 
			{
				// Consume whole buffer
				inst->buffer_pos=inst->buffer_size;
				memcpy(str+strpos,inst->buffer,inst->buffer_size);
				strpos+=inst->buffer_size;
			} 
			else 
			{
				// Partial buffer
				inst->buffer_pos=retval+1; // +1 to skip the NULL terminator of the string
				--retval; // -1 to stop before the detected endline
				if (retval>0) memcpy(str+strpos,inst->buffer,retval);
				strpos+=retval;
				break;
			}
			
			// Don't expect a full buffer to allow interactive (non-buffering) connections.
			// No data indicates some connection error, so quit.
			if (inst->buffer_size == 0) break;
		}
	}
	
	// remove \r if present just before the \n
	if (str[strpos] == '\r') --strpos;
	
	// Set the NULL terminator for the string
	++strpos;
	if (strpos > strsize) strpos=strsize;
	str[strpos]=0;
	return strpos-1;	
}

//#define dirname "."

void server_dirlist(struct server_struct *inst,char *dirname) 
{
	DIR *dir;
	struct dirent *ent;
	int retval;
	char Buffer[1024];
	char FBuffer[1024];
	char *cptr;
	struct stat statbuf;

//	setHeader_filename(inst,".html");

	retval=strlen(dirname);
	if (dirname[retval-1] != '/') 
	{
		dirname[retval] = '/';
		dirname[retval+1] = 0;
	}

	if ((dir = opendir(dirname)) == NULL)
	{
		retval=strlen(dirname);
		if (dirname[retval-1] == '/') dirname[retval-1] = 0;
		
		setHeader_respval(inst,404);
		printHeader(inst);
		return;
//		sprintf(Buffer,"HTTP/1.0 404 Not Found\r\nContent-Type: text/html; charset=ISO-8859-4\r\nServer: miniweb 0.02\r\n\r\n");
//		send(inst->sock,Buffer,strlen(Buffer),0);
		
//		sprintf(Buffer,"<HTML><HEAD><TITLE>404 Not Found: %s</TITLE></HEAD><BODY><H1>404 Not Found: %s<H1>\n",dirname,dirname);
//		send(inst->sock,Buffer,strlen(Buffer),0);
	} 
	else 
	{
		setHeader_filename(inst,".html");
		setHeader_respval(inst,201);
		printHeader(inst);
		
		sprintf(Buffer,"<HTML><HEAD><TITLE>Directory listing for %s</TITLE></HEAD><BODY><H1>Directory listing for '%s':</H1>\r\n",dirname,dirname);
		send(inst->sock,Buffer,strlen(Buffer),0);
	
		while ((ent = readdir(dir)) != NULL)
		if (strcmp(ent->d_name,".")!=0)
		if ((strcmp(dirname,"./")!=0) || (ent->d_name[0]!='.'))
		{
			strcpy(FBuffer,dirname);
			if (strcmp(ent->d_name,"..")!=0) 
			{
				strcat(FBuffer,ent->d_name);
				stat(FBuffer, &statbuf);
				
				if (statbuf.st_mode & S_IFDIR) 
				{
					retval=strlen(FBuffer);
					if (FBuffer[retval-1] != '/') 
					{
						FBuffer[retval] = '/';
						FBuffer[retval+1] = 0;
					}
				}
			}
			else
			{
				retval=strlen(dirname);
				if (FBuffer[retval-1] == '/') FBuffer[retval-1] = 0;
				if ((cptr=strrchr(FBuffer,'/')) != NULL) cptr[1] = 0;
				else strcpy(FBuffer,"/");
				
			}
			
			sprintf(Buffer,"<A href=\"%s\">%s</A><BR>\r\n",FBuffer+1,ent->d_name);
			send(inst->sock,Buffer,strlen(Buffer),0);
		}
	 
		closedir(dir);
		
		sprintf(Buffer,"<H3>MiniWeb web server</H3></BODY></HTML>");
		send(inst->sock,Buffer,strlen(Buffer),0);
	}
}

DWORD WINAPI server(struct server_struct *inst) 
{
	char Buffer[8192];
	char filename[1024];
	int retval,i,pos,tmp1,tmp2;
	FILE *in;
	
	// Initialize structure
	inst->buffer_pos=0;
	inst->buffer_size=0;
	filename[0]='.';
	filename[1]=0;
	clearHeader(inst);

	// Parse header
	retval = server_readln(inst,Buffer,sizeof(Buffer));
	sscanf(Buffer,"GET %s ",filename+1);
	
	if (filename[1] == 0)
	{
		sprintf(inst->logbuffer,"\"%s:%d\" ",inet_ntoa(inst->sin_addr),htons(inst->sin_port));
		setHeader_respval(inst,400);
		printHeader(inst);
		
		goto serverquit;
	}
	
	retval = strlen(filename);
	pos=0;
	for (i=0;i<retval;++i) {
		if (filename[i]=='%')
		{
			tmp1=filename[i+1];
			if ((tmp1-'a') >= 0) tmp1-='a'+10;
			else if ((tmp1-'A') >= 0) tmp1-='A'+10;
			else if ((tmp1-'0') >= 0) tmp1-='0';
			else tmp1 = -1;
			
			tmp2=filename[i+2];
			if ((tmp2-'a') > 0) tmp2-='a'+10;
			else if ((tmp2-'A') >= 0) tmp2-='A'+10;
			else if ((tmp2-'0') >= 0) tmp2-='0';
			else tmp2 = -1;
			
			if ((tmp1!=-1) && (tmp2!=-1)) 
			{
				Buffer[pos]=(char)(tmp1*16+tmp2);
				i+=2;
			}
			else Buffer[pos] = '%';
		}
		else Buffer[pos]=filename[i];
		++pos;
	}
	Buffer[pos]=0;
	strcpy(filename,Buffer);
	
	sprintf(inst->logbuffer,"\"%s:%d\" %s",inet_ntoa(inst->sin_addr),htons(inst->sin_port),filename);

	// Check for sub-root hacking, If found send a forbidden.
	if (strstr(filename,"/..")!=NULL) 
	{
		strcat(inst->logbuffer," ; ");
		setHeader_respval(inst,403);
		printHeader(inst);
		
		goto serverquit;
	}
	
/*	while ((retval = server_readln(inst,Buffer,sizeof(Buffer))) > 0) 
	{
		fprintf(stderr,"'%s'\n",Buffer);
		
		retval = send(inst->sock,Buffer,retval,0);
		if (retval == SOCKET_ERROR) 
		{
			fprintf(stderr,"send() failed: error %d\n",WSAGetLastError());
		}
		send(inst->sock,&nn,1,0);
		
	}*/
	if ((in = fopen(filename, "rb")) == NULL)
	{
		strcpy(Buffer,filename);
		retval=strlen(Buffer);
		if (Buffer[retval-1] == '/') {
			strcat(Buffer,"index.html");
			in = fopen(Buffer, "rb");
			if (in != NULL) 
			{
				strcat(inst->logbuffer,"[index.html]");
				strcat(filename,"index.html");
			}
		}
	}
	
	strcat(inst->logbuffer," ; ");
	
	if (in == NULL)
	{
		server_dirlist(inst,filename);
	}
	else 
	{
		int ret;
		setHeader_filename(inst,filename);
		setHeader_respval(inst,200);
		if (!fseek(in,0,SEEK_END))
		{
			// Supports seek
			sprintf(Buffer,"Content-Length: %ld\r\n",ftell(in));
			fseek(in,0,SEEK_SET);
		}
		setHeader_generic(inst,Buffer);
		printHeader(inst);
		ret=0;
		
		while ((retval=fread(Buffer, 1,sizeof(Buffer), in)) > 0) {
			ret=send(inst->sock,Buffer,retval,0);
			//printf("%d : %d\n",retval,ret);
			if (ret<retval) 
			{
				fclose(in);
				goto serverquit;
			}
		}

		fclose(in);
	}

serverquit:

	server_close(inst);
	return 0;
}
