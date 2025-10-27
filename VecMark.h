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
void Write(string& Target, string Text);

string CSSFrontStr = 
R"(
<!DOCTYPE html>
<html>

<head>
<style>
body {
    margin: 0;
    padding: 5px;
    background: #000000;
    color: white;
    font-size: 1.5em;
}
.container{
    max-width: 50%;
    background: rgb(5, 0, 10);
    padding: 20px;
    margin: 0 auto;
    border-radius: 8px;
    box-shadow: 0 0 10px rgba(50, 0, 255, 0.6);
}
hr {
    height: 2px;
    background: pink;
    color: pink;
    border-radius: 2px;
    box-shadow: 0 0 8px rgba(255, 255, 255, 0.8);
    margin: 1em 0;
}
img {
    display: block;
    margin: 0 auto;
}
a{
    color: blue;
    text-decoration: none;
}
blockquote{ 
    background: black;
    border-left: 8px solid yellow;
    border-radius: 3px;
    padding: 6px 6px;
    margin-left: 0;
    margin-right: 0;
}
pre code, code {
    background: rgb(60, 60, 60);
    color: aquamarine;
    border-radius: 5px;
    border: 3px solid #ffffff;
}
pre code { 
    padding: 15px;
    display: block;
}
</style>
</head>



<body>
<div class="container">
)";

string CSSBackStr =
R"(
</div>
</body>

</html>
)";
