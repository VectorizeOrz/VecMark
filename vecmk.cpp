#include <iostream>
#include <fstream>
using namespace std;

// Help
void Error(string Message)
{
    cout<<"[ERROR] "<<Message<<endl;
    exit(-1);
}

// Compile
string Compile(string LineText)
{
    // Remove blank in back
    int EndIndex = LineText.size()-1;
    while(EndIndex >= 0 && isspace(LineText[EndIndex]))
        LineText.pop_back();
    
    // 
    return "<HTML>"+LineText+"</HTML>";
}

// Main process
int main(int argc, char* argv[]) 
{
    // Get input & output file name
    if(argc != 2)
        Error("Usage: vecmk XXX.md");

    string InputFileName = argv[1];
    if(InputFileName.size()<=3)
        Error("Input file name is too short");

    string FilePostfix = InputFileName.substr(InputFileName.size()-3,3);
    if(FilePostfix != ".md")
        Error("Only support compile '.md' file");

    string OutputFileName = InputFileName.substr(0,InputFileName.size()-3)+".html";

    cout<<InputFileName<<" "<<OutputFileName<<endl;

    // Open input & output file
    ifstream InputFile(InputFileName);
    if(!InputFile.is_open())
        Error("Input file is not exist");

    ofstream OutputFile(OutputFileName);
    if(!OutputFile.is_open())
        Error("Create output file fail");

    // Read input file ,compile it and write to output file
    string LineText;
    while(getline(InputFile,LineText))
        OutputFile<<Compile(LineText)<<endl;

    // Close input & output file
    InputFile.close();
    OutputFile.close();

    cout<<"Compile "<<InputFileName<<" to "<<OutputFileName<<" success !"<<endl;

    return 0;
}
