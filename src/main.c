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
#include "xml.h"

struct xmlDoc xml;

int main(int argc, char **argv) 
{
    xml.version=0;
    xml.encoding=0;
    
    //read file
	FILE* pFile = xmlOpen(argc,argv);
	
	if(!pFile) printf("XML file not found");
	if(pFile)
	{
	    char* line = 0;
        fseek (pFile , 0 , SEEK_END);
        int fSize = ftell(pFile);
        rewind (pFile);

        line = (char*) malloc (sizeof(char)*fSize);
        
        if(!line)
        {
            #ifdef XML_DEBUG
                    printf("Memory Error in main() with malloc()%d",fSize);
            #endif
        }
        if(line)
        {       
            int result = fread (line,1,fSize,pFile);
            
            if(result != fSize)
            {
    	    #ifdef XML_DEBUG    
                printf("Error reading file data from file. Length %d vs %d (%d)\n",result,fSize,sizeof(char));
                printf("String Length: %d\n\n",strlen(line));
                printf("%s\n\n",line);        
    	    #endif
            }
    	    
    	    parseXml(line,0);
    	    
    	    fclose(pFile);
    	    //cleanup    
    	    xmlCleanup(&xml);
       	    free(line);
       	    line = 0;
        }
	}
	
    int input = 0;
    scanf("%d",&input);
    
    return 0;
}
