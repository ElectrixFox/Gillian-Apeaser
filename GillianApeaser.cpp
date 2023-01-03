#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#pragma GCC diagnostic ignored "-Wwrite-strings"

#define ccout cout << '\n'
#define vcout(x) ccout << #x << ": " << x
#define scout static int staticout = 0; if(staticout == 0) cout << '\n'


int WriteFunctionNames(char** text, int linecount);
int FindAllFunctions(char** text, int linecount, int* funcpos = nullptr);

int StringAppend(char** str, char* toappend);

int MarkText(char** text, char** identifierlist, int start, int linecount);
int AddEndTag(char* tag, char** datbuffer, int at);

int FindAnyInArray(char** tofind, char* in, int complex = 1);
int FindClosingBrace(char** buffer, int linecount, int stratpos = 0);

int OutBuffer(char** buffer, int linecount, int showlinecount = 0);
int OutLine(char** buffer, int at, int ln = 0);

int GetText(char FileName[32], char** datbuffer);
int WriteText(char FileName[32], char** datbuffer, int linecount);

int lc;

char* identifiers[] = { "if", "while", "for", "switch" };
char* endings[] = { " //endif", " //endwhile", " //endfor", " //endcase" };
char atestsets[16][16] = { "TestSetA1.txt", "TestSetA2.txt", "TestSetA3.txt", "TestSetA4.txt", "TestSetA5.txt"};
char btestsets[16][16] = { "TestSetB1.txt", "TestSetB2.txt", "TestSetB3.txt", "TestSetB4.txt", "TestSetB5.txt"};

int main()
{
char** dat = (char**)(malloc(sizeof(char*) * 256));

int tc = 1;
int teststyle;

cout << "\n1: A\n2: B\nEnter test style: ";
cin >> teststyle;

cout << "Enter test number: ";
cin >> tc;

if(teststyle == 1)
    {
    lc = GetText(atestsets[tc-1], dat);
    }
if(teststyle == 2)
    {
    lc = GetText(btestsets[tc-1], dat);
    }

cout << "line count: " << lc;

cout << "\n\n============\n" << "== Before ==" << "\n============\n\n";
OutBuffer(dat, lc, 1);


cout << "\n\n============\n" << "== Middle ==" << "\n============\n\n";
WriteFunctionNames(dat, lc);

//MarkText(dat, identifiers, 0, lc);

cout << "\n\n===========\n" << "== After ==" << "\n===========\n\n";
OutBuffer(dat, lc, 1);

WriteText("TestSetB1Out.txt", dat, lc);


ccout << "Normal exit";

return 0;
}

int WriteFunctionNames(char** text, int linecount)
{
char type[10];
int totallen = 0;
char* concfuncnames;
char name[64];
char fnames[128][128];
int fpositions[128];
int fcount = FindAllFunctions(text, linecount, fpositions);

for (int i = 0; i < fcount; i++)
    {
    sscanf(text[fpositions[i]], "%s %s", type, name);

    if(strstr(name, "(") != nullptr)
        {
        name[strlen(name)-3] = '\0';
        strcpy(fnames[i], name);
        }
    
    totallen = totallen + strlen(name);
    }

concfuncnames = (char*)malloc(sizeof(char) * 256);
memset(concfuncnames, 0, 256);

for (int i = 0; i < fcount; i++)
    {
    strcat(concfuncnames, fnames[i]);
    
    if(i + 1 < fcount)
        {
        strcat(concfuncnames, ", ");
        }
    }

for (int i = 0; i < linecount; i++)
    {
    if(strstr(text[i], "Functions: ") != nullptr)
        {
        text[i] = (char*)malloc((strlen(concfuncnames) + 12) * sizeof(char));
        strcpy(text[i], "Functions: ");
        strncat(text[i], concfuncnames, strlen(concfuncnames));
        free(concfuncnames);
        return 1;
        }
    }


lc += 1;
text[linecount] = (char*)malloc(sizeof(char) * 256);
text[lc] = (char*)malloc(sizeof(char) * 256);

for (int i = linecount; i >= 0; i--)
    {
    strcpy(text[i + 1], text[i]);
    OutLine(text, i, 1);
    
    }

OutBuffer(text, lc, 1);

text[0] = (char*)malloc((strlen(concfuncnames) + 12) * sizeof(char));
strcpy(text[0], "Functions: ");
strncat(text[0], concfuncnames, strlen(concfuncnames));

vcout(text[1]);

return 0;
}

int FindAllFunctions(char** text, int linecount, int* funcpos)
{
char* functypes[] = { "int", "void", "char"};

int top = 0;
int* funcpositions = (int*)(malloc(sizeof(int) * 256));

for (int i = 0; i < linecount; i++)
    {
    if(FindAnyInArray(functypes, text[i], 0) != -1)
        {
        funcpositions[top] = i;
        ccout << text[i];
        top++;
        }
    }

if(funcpos != nullptr)
    {
    memcpy(funcpos, funcpositions, sizeof(int) * top);
    }


return top;
}

int MarkText(char** text, char** identifierlist, int start, int linecount)
{
int cbpos = 0;
int find = 0;

ccout << "Starting identifier search";

// Replace lc with some function to get a buffer column count
for (int i = start; i < linecount; i++)
    {
    find = FindAnyInArray(identifierlist, text[i]);
    if(find != -1)
        ccout << "Found " << identifierlist[find] << " on line " << i + 1 << " " << text[i];

    if(find != -1)
        {
        cbpos = FindClosingBrace(text, linecount, i + 2);
        AddEndTag(endings[find], text, cbpos);
        }
    }

return 0;
}

int FindClosingBrace(char** buffer, int linecount, int startpos)
{
int ignoreNbraces = 0;

for (int i = startpos; i < linecount; i++)
    {
    if(strstr(buffer[i], "{") != nullptr)
        {
        ignoreNbraces = ignoreNbraces + 1;
        }
    if(strstr(buffer[i], "}") != nullptr)
        {
        if(ignoreNbraces == 0)
            {
            return i;
            }
        else
            {
            ignoreNbraces--;
            }

        }
    }
}

int GetText(char FileName[32], char** datbuffer)
{
ifstream fin(FileName, ios::out);
string linedata;
int index = 0;

while(getline(fin, linedata))
        {
        datbuffer[index] = (char*)malloc(sizeof(char) * (linedata.length() + 32));
        strcpy(datbuffer[index], linedata.c_str());
        ++index;
        }

fin.close();
return index;
}

int WriteText(char FileName[32], char** datbuffer, int linecount)
{
ofstream fout(FileName, ios::out);

for (int i = 0; i < linecount; i++)
    {
    fout.write(datbuffer[i], strlen(datbuffer[i]));
    fout.write("\n", 1);
    }

fout.close();
return 0;
}

int AddEndTag(char* tag, char** datbuffer, int at)
{
char* tmpbuff;

if(strstr(datbuffer[at], tag) != nullptr)
    {
    return -1;
    }

tmpbuff = (char*)malloc(sizeof(char*) * 128);

strcpy(tmpbuff, datbuffer[at]);
strcpy(&tmpbuff[strlen(datbuffer[at])], tag);

strcpy(datbuffer[at], tmpbuff);

free(tmpbuff);

return 1;
}

int FindAnyInArray(char** tofind, char* in, int complex)
{
char* tmpchar = (char*)malloc(sizeof (char*) * 128);

for (int i = 0; i < 3; i++)
    {
    if(strstr(in, tofind[i]) != nullptr)
        { 
        if(!complex)
            {
            free(tmpchar);
            return i;
            }

        sscanf(in, "%s", tmpchar);

        if(strncmp(tmpchar, tofind[i], 2) == 0)
            {
            free(tmpchar);                
            return i;
            }

        }

    }

return -1;
}

int StringAppend(char** str, char* toappend)
{
char* tmpbuf;

tmpbuf = (char*)malloc(sizeof(char*) * 256);
//realloc(str, sizeof(char) * (strlen(*str) + strlen(toappend)));

char*& n_str = *str;

int stlen = strlen(n_str);
strncpy(tmpbuf, n_str, strlen(n_str));

const char* app = toappend;
strncpy(&tmpbuf[stlen], app, strlen(app));
strncpy(n_str, tmpbuf, strlen(tmpbuf));

free(tmpbuf);
return 1;
}

int OutLine(char** buffer, int at, int ln)
{
cout << "\n";

if(ln == 1)
    {
    cout << "line " << at + 1 << ": ";
    }

cout << buffer[at];

return 0;
}

int OutBuffer(char** buffer, int linecount, int showlinecount)
{

cout << "\n";
for(int i = 0; i < linecount; i++)
    {
    if(showlinecount == 1)
        {
        cout << i+1 << "\t";  
        }
    cout << buffer[i] << '\n';
    }

return 0;
}
