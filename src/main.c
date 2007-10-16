#include <stdio.h>
#include "xml.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

char buffer[255] = "";
struct xmlDoc xml;

int aTask = 0;

#define APP_DEBUG 1

int main() 
{
    sprintf(xml.version,"");
    sprintf(xml.encoding,"");
    
    //TODO: read lines from file
    char line[] = "<?xml version=\"1.0\" encoding='UTF-8'?>\n<!--Comment-->\n<root>\n<main color=\"#FF9900\">Main Value</main>\n<woof meeaw /></root>\n";
    #if APP_DEBUG    
        printf("Size of line: %d\n",sizeof(line));
        printf("String Length: %d\n\n",strlen(line));        
    #endif
    
    parse(line,0);
    //cleanup    
    cleanup();
    
    int input = 0;
    scanf("%d",&input);
    
    return 0;
}

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

int parse(char* aLine, char* xPath)
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
                 #if APP_DEBUG
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
             #if APP_DEBUG
                  printf("TODO: Parse Element\n");
                  printf("Line: %s\n",aline[i]);
             #endif
            }break;
        }
    }
      
    if(aTask == 0) printf("Unable to locate xml declaration\n");
    freeArray(aline,aLineSize);
}

void cleanup(void)
{
     if(xml.version != 0)
     {
        free(xml.version);
        xml.version = 0;
     }
     
     if(xml.encoding != 0)
     {
        free(xml.encoding);
        xml.encoding = 0;
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
              #if APP_DEBUG
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
              #if APP_DEBUG
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
              #if APP_DEBUG
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
                #if APP_DEBUG
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
