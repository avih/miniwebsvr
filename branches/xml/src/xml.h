
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

int parse(char *, char *);
char** explode(char *,char *,int*);
char* subp(char*,int, int);

char lang_delimiters[] = "><\n?";
char ws_delimeters[] = "<\n\"' ";

void freeArray(char**,int);

void cleanup(void);
