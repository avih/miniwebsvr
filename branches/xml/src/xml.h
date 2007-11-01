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

#ifndef XML_H
#define XML_H

#ifndef XML_DEBUG
        #define XML_DEBUG
#endif

#include <stdio.h>

#define DEFAULT_XMLFILE "example.xml"

    struct xml_attribute
    {
       char* name;
       char* value;
    };
    
    struct xml_element
    {
     char* name;
     char* text;
     int size_attributes;
     int size_children;
     struct xml_attribute** attributes;
     struct xml_element** children;
     struct xml_element* parent;
    };
    
    struct xmlDoc
    {
         char* version;
         char* encoding;
         struct xml_element* root;  
    };
    
    int parseXml(char *, char *);
    char** explode(char *,char *,int*);
    char* subp(char*,int, int);
    
    void freeArray(char**,int);
    
    void xmlCleanup(struct xmlDoc*);
	
	FILE* xmlOpen(int, char **);
    
#endif
