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

#ifndef APP_DEBUG
#define APP_DEBUG
#endif

int main() 
{
    sprintf(xml.version,"");
    sprintf(xml.encoding,"");
    
    //TODO: read lines from file
    char line[] = "<?xml version=\"1.0\" encoding='UTF-8'?>\n<!--Comment-->\n<root>\n<main color=\"#FF9900\">Main Value</main>\n<woof meeaw /></root>\n";
    #ifdef APP_DEBUG    
        printf("Size of line: %d\n",sizeof(line));
        printf("String Length: %d\n\n",strlen(line));        
    #endif
    
    parseXml(line,0);
    //cleanup    
    xmlCleanup(&xml);
    
    int input = 0;
    scanf("%d",&input);
    
    return 0;
}
