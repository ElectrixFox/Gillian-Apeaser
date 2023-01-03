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

int GetText(char FileName[32]);
//int MarkText(char buffer[256][256], int linecount, int start = 0, char* endTag = "");

int MarkText(char text[256][256]);
int FindIdentifiers(char text[256][256], char* identifier, int start, int linecount);

int FindAnyInArray(char tofind[16][16], char* in);
int FindClosingBrace(char buffer[256][256], int linecount, int stratpos = 0);

int FindFirst(char buffer[256][256], char* item, int startingat = 0);
int FindNth(char buffer[256][256], char* item, int startingat, int n);

int GetCountOf(char buffer[256][256], char* item);
int GetCountOf(char buffer[256][256], char* item, int*& locations);

int OutBuffer(char buffer[256][256], int showlinecount = 0);
int OutLine(char buffer[256][256], int at, int ln = 0);
int GetLineCount(char buffer[256][256]);

int AddEndTag(char* tag, char datbuffer[256][256], int at);

char dat[256][256];
int lc;

char identifiers[16][16] = { "if", "while", "for", "switch" };
char endings[16][16] = { " //endif", " //endwhile", " //endfor", " //endcase" };
char testsets[16][16] = { "TestSet1.txt", "TestSet2.txt", "TestSet3.txt", "TestSet4.txt" };

int main()
{
int tc = 1;
cout << "Enter test number: ";
cin >> tc;


lc = GetText(testsets[tc-1]);
cout << "line count: " << lc;

cout << "\n\n============\n" << "== Before ==" << "\n============\n\n";
OutBuffer(dat, 1);


cout << "\n\n============\n" << "== Middle ==" << "\n============\n\n";
MarkText(dat);


cout << "\n\n===========\n" << "== After ==" << "\n===========\n\n";
OutBuffer(dat, lc-1);

return 0;
}

int MarkText(char text[256][256])
{
FindIdentifiers(text, "if", 0, lc);
FindIdentifiers(text, "while", 0, lc);

}

int FindIdentifiers(char text[256][256], char* identifier, int start, int linecount)
{
int cbpos = 0;

ccout << "Starting identifier search";

// Replace lc with some function to get a buffer column count
for (int i = start; i < linecount; i++)
    {
    if(strstr(text[i], identifier) != nullptr)
        {
        cbpos = FindClosingBrace(text, linecount, i + 2);
        AddEndTag(" //endtag", text, cbpos);
        }
    }

return 0;
}

int FindClosingBrace(char buffer[256][256], int linecount, int startpos)
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

int GetText(char FileName[32])
{
ifstream fin(FileName, ios::out);
string linedata;
int index = 0;

while(getline(fin, linedata))
        {
        strcpy(dat[index], linedata.c_str());
        ++index;
        }

fin.close();
return index;
}

int OutLine(char buffer[256][256], int at, int ln)
{
cout << "\n";

if(ln == 1)
    {
    cout << "line " << at + 1 << ": ";
    }


cout << buffer[at];
}

int OutBuffer(char buffer[256][256], int showlinecount)
{
int lc = GetLineCount(buffer);

cout << "\n";
for(int i = 0; i < lc; i++)
    {
    if(showlinecount == 1)
        {
        cout << i+1 << "   ";  
        }
    cout << dat[i] << '\n';
    }

return 0;
}

int GetLineCount(char buffer[256][256])
{
int index = lc;

return index;
}

int AddEndTag(char* tag, char datbuffer[256][256], int at)
{
char* tmpbuff;
tmpbuff = (char*)malloc(sizeof(char*) * 128);

strcpy(tmpbuff, datbuffer[at]);
strcpy(&tmpbuff[strlen(datbuffer[at])], tag);

strcpy(datbuffer[at], tmpbuff);

free(tmpbuff);
}

int FindAnyInArray(char tofind[16][16], char* in)
{
for (int i = 0; i < 3; i++)
    {
        if(strstr(in, tofind[i]) != nullptr)
            {
            return i;
            }
    }

return -1;
}