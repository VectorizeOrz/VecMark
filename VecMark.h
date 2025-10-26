#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
using namespace std;

void VecMarkRun(string InputFileName, string OutputFileName);

// Compile
string Compile(vector<string>& AllText);
string CompileNormalLine(string& LineText);
string CompileHeadLine(string& LineText);
string CompileUnorderedList(string& LineText);
string CompileOrderedList(vector<string>& AllText, size_t& CurrentIndex);
string CompileImage(string& LineText);
string CompileLink(string& LineText);
string CompileReference(string& LineText);
string CompileCodeBlock(vector<string>& AllText, size_t& CurrentIndex);
string CompileEmbedHTML(vector<string>& AllText, size_t& CurrentIndex);

// Help
void Error(string Message);
void WriteFront(string& Target, string Text);
void WriteBack(string& Target, string Text);
