/*  MiniWebsvr XML - A small xml parser
    Copyright (C) 2007  Nickolas Antonie Grigoriadis and Johan Strydom
    E-Mail: grigi_ at users.sourceforge.net and fyrewolfe at users.sourceforge.net

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
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "xml.h"

#ifndef APP_DEBUG
#define APP_DEBUG
#endif

int aTask = 0;

char lang_delimiters[] = "><\n?";
char ws_delimeters[] = "<\n\"' ";

char* subp(char* aSubject,int aStart, int aLength)
{
      char* retVal = 0;
 
      if(aLength > (strlen(aSubject) - aStart)) aLength = strlen(aSubject);
 
      retVal = malloc(aLength+1);
      
      int i=0;
      for(i=0;i<aLength;i++)
      {
       retVal[i] = aSubject[i];
      }
 
      return retVal;     
}

int parseXml(char* aLine, char* xPath)
{
    //TODO: parse accoring to xPath
    int aLineSize = 0;
    char** aline = explode(lang_delimiters,aLine,&aLineSize);
    int i = 0;
    for(i=0;i<aLineSize;i++)
    {
        switch(aTask)
        {
            case 0: default:
            {
                //Get XML Declaration
                char * pch;
                pch = strstr(aline[i],"xml");
                 #ifdef APP_DEBUG
                     if(pch != 0) 
                     {
                      printf(">>> xml declaration found <<<\n");
                      printf("Line: %s\n",aline[i]);
                     }
                #endif
                //TODO: getVersion
                //TODO: getEncoding
                aTask++;
            }break;
            case 1:
            {
             #ifdef APP_DEBUG
                  printf("TODO: Parse Element\n");
                  printf("Line: %s\n",aline[i]);
             #endif
            }break;
        }
    }
      
    if(aTask == 0) printf("Unable to locate xml declaration\n");
    freeArray(aline,aLineSize);
}

void xmlCleanup(struct xmlDoc* xD)
{
     if(xD->version != 0)
     {
        free(xD->version);
        xD->version = 0;
     }
     
     if(xD->encoding != 0)
     {
        free(xD->encoding);
        xD->encoding = 0;
     }
	 
	 //TODO: Elemets cleanup
}

void freeArray(char** aArray,int aSize)
{
    int i = 0;
    for(i =0 ;i < aSize; i++)
    {
     char* tmpPtr = (char *)aArray[i];
     free(tmpPtr);
    }
    free(aArray);
}

char** explode(char* delimeters,char* string, int* aSize)
{
        char** stringArray = 0;
        int aCount = 1;
        
        char* buffer = strtok(string,delimeters);
        
        while(buffer != 0)
        {
           if(aCount > 1) 
           {
            char** tmpArr = calloc(aCount,sizeof(char*));
            //assert(tmpArr);
            if(tmpArr == 0)
            {
              #ifdef APP_DEBUG
                  printf("Memory Error in explode('%s','%s','%d') on tmpArr\n",delimeters,string,aSize);
              #endif
              return(0);
            }
            memcpy(tmpArr,stringArray,(aCount-1)*sizeof(char*));
            free(stringArray);
            stringArray = 0;
            stringArray = calloc(aCount,sizeof(char*));
            //assert(stringArray);
            if(stringArray == 0)
            {
              #ifdef APP_DEBUG
                  printf("Memory Error in explode('%s','%s','%d') on stringArray\n",delimeters,string,aSize);
              #endif
              return(0);
            }
            memcpy(stringArray,tmpArr,(aCount-1)*sizeof(char*));
            free(tmpArr);
            tmpArr = 0;
            char* tmpbuff = malloc(strlen(buffer)+1); //+1 for \0 terminator
            //assert(tmpbuff);
            if(tmpbuff == 0)
            {
              #ifdef APP_DEBUG
                  printf("Memory Error in explode('%s','%s','%d') on tmpbuff\n",delimeters,string,aSize);
              #endif
              return(0);
            }
            stringArray[aCount-1] = memcpy ( tmpbuff, buffer, strlen(buffer)+1 );
           }
           else
           {
              stringArray = calloc(aCount,sizeof(char*)); //memory block = (aCount * sizeof(char *))
              char* tmpbuff = malloc(strlen(buffer)+1); //+1 for \0 terminator
              //assert(tmpbuff);
              if(tmpbuff == 0)
              {
                #ifdef APP_DEBUG
                printf("Memory Error in explode('%s','%s','%d') on tmpbuff\n",delimeters,string,aSize);
                #endif
              }
              stringArray[aCount-1] = memcpy ( tmpbuff, buffer, strlen(buffer)+1 );
           }
           
         aCount++;
         buffer = strtok(0,delimeters);
        }

        *aSize = aCount-1;
        return stringArray;
}

