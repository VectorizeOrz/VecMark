#include"VecMark.h"

// Main process
int main(int argc, char* argv[]) {
    // Get input & output file name
    if(argc!=2)
        Error("Usage: vecmk XXX.md");

    string InputFileName = argv[1];
    if(InputFileName.size()<=3)
        Error("Input file name is too short");

    string FilePostfix = InputFileName.substr(InputFileName.size()-3, 3);
    if(FilePostfix!=".md")
        Error("Only support compile '.md' file");

    string OutputFileName = InputFileName.substr(0, InputFileName.size()-3) + ".html";

    VecMarkRun(InputFileName, OutputFileName);

    cout<<"Compile "<<InputFileName<<" to "<<OutputFileName<<" success !"<<endl;

    return 0;
}

void VecMarkRun(string InputFileName, string OutputFileName) {
    // Open input & output file
    ifstream InputFile(InputFileName);
    if(!InputFile.is_open())
        Error("Input file is not exist");

    ofstream OutputFile("XXX"  + OutputFileName);
    if(!OutputFile.is_open())
        Error("Create output file fail");

    // Read input file ,compile it and write to output file
    string LineText;
    vector<string> AllText;
    while(getline(InputFile, LineText)) {
        // Remove blank in back
        int EndIndex = LineText.size()-1;
        while(EndIndex>=0 && isspace(LineText[EndIndex])) {
            LineText.pop_back();
            EndIndex--;
        }

        AllText.push_back(LineText);
    }

    OutputFile<<Compile(AllText)<<endl;

    // Close input & output file
    InputFile.close();
    OutputFile.close();
}



// Compile
string Compile(vector<string>& AllText) {
    string AllHTML;
    bool InEmptyLine = false;

    // Look each line text
    for(size_t CurrentIndex=0; CurrentIndex<AllText.size();) {
        string LineText = AllText[CurrentIndex];

        // Empty
        if(LineText=="") {
            if(!InEmptyLine) {
                WriteBack(AllHTML, "<br>\n");
                InEmptyLine = true;
            }
            CurrentIndex++;

        // Text
        } else {
            InEmptyLine = false;
            char LineHead = LineText[0];
            
            // Headline
            if(LineHead == '#') {
                WriteBack(AllHTML, CompileHeadLine(LineText));
                CurrentIndex++;
            // Crossline
            } else if(LineText=="___" || LineText=="---" || LineText=="***") {
                WriteBack(AllHTML, "<hr>");
                CurrentIndex++;
            // Unordered list
            } else if(LineHead=='*' || LineHead=='+' || LineHead=='-') {
                WriteBack(AllHTML, CompileUnorderedList(LineText));
                CurrentIndex++;
            // Ordered list
            } else if(isdigit(LineHead)) {
                WriteBack(AllHTML, CompileOrderedList(AllText, CurrentIndex));
            // Image
            } else if(LineHead=='!') {
                WriteBack(AllHTML, CompileImage(LineText));
                CurrentIndex++;
            // Link
            } else if(LineHead=='[') {
                WriteBack(AllHTML, CompileLink(LineText));
                CurrentIndex++;
            // Reference
            } else if(LineHead=='>') {
                WriteBack(AllHTML, CompileReference(LineText));
                CurrentIndex++;
            // Codeblcok
            } else if(LineText=="```") {
                WriteBack(AllHTML, CompileCodeBlock(AllText, CurrentIndex));
            // EmbedHTML
            } else if(LineText=="<<") {
                WriteBack(AllHTML, CompileEmbedHTML(AllText, CurrentIndex));
            // Normalline
            } else {
                WriteBack(AllHTML, "<p>");
                WriteBack(AllHTML, CompileNormalLine(LineText));
                WriteBack(AllHTML, "</p>");
                CurrentIndex++;
            }

            WriteBack(AllHTML, "\n");
        }
    }
    
    return AllHTML;
}

string CompileNormalLine(string& LineText)
{
    if(LineText=="") return "";
    
    string NormalLineHTML;
    char LineHead = LineText[0];

    auto IsSpecial = [](char& TestC) {
        return TestC=='*' || TestC=='_' || TestC=='~' ||TestC =='`';
    };

    // Italic & Bold 1
    if(LineHead=='*') {
        // **...
        if(LineText.size()>1 && LineText[1]=='*') {
            size_t I = 2;
            while(I<LineText.size() && LineText[I]!='*') I++;
            // **...**...
            if(I+1<LineText.size() && LineText[I+1]=='*') {
                string LeftText = LineText.substr(2,I-2);
                I += 2;
                string RightText = LineText.substr(I,LineText.size()-I); 
                WriteBack(NormalLineHTML, "<strong>");
                WriteBack(NormalLineHTML, CompileNormalLine(LeftText));
                WriteBack(NormalLineHTML, "</strong>");
                WriteBack(NormalLineHTML, CompileNormalLine(RightText));
            } else {
                string RemainText = LineText.substr(1,LineText.size()-1);
                WriteBack(NormalLineHTML, "*");
                WriteBack(NormalLineHTML, CompileNormalLine(RemainText));
            }
        // *... 
        } else {
            size_t I = 1;
            while(I<LineText.size() && LineText[I]!='*') I++;
            //*...*...
            if(I<LineText.size() && LineText[I]=='*') {
                string LeftText = LineText.substr(1,I-1);
                I++;
                string RightText = LineText.substr(I,LineText.size()-I);
                WriteBack(NormalLineHTML, "<em>");
                WriteBack(NormalLineHTML, CompileNormalLine(LeftText));
                WriteBack(NormalLineHTML, "</em>");
                WriteBack(NormalLineHTML, CompileNormalLine(RightText));
            } else {
                string RemainText = LineText.substr(1,LineText.size()-1);
                WriteBack(NormalLineHTML, "*");
                WriteBack(NormalLineHTML, CompileNormalLine(RemainText));
            }
        }

    // Italic & Bold 2 (Same as 1)
    } else if(LineHead=='_') {
        // __...
        if(LineText.size()>1 && LineText[1]=='_') {
            size_t I = 2;
            while(I<LineText.size() && LineText[I]!='_') I++;
            // __...__...
            if(I+1<LineText.size() && LineText[I+1]=='_') {
                string LeftText = LineText.substr(2,I-2);
                I += 2;
                string RightText = LineText.substr(I,LineText.size()-I); 
                WriteBack(NormalLineHTML, "<strong>");
                WriteBack(NormalLineHTML, CompileNormalLine(LeftText));
                WriteBack(NormalLineHTML, "</strong>");
                WriteBack(NormalLineHTML, CompileNormalLine(RightText));
            } else {
                string RemainText = LineText.substr(1,LineText.size()-1);
                WriteBack(NormalLineHTML, "_");
                WriteBack(NormalLineHTML, CompileNormalLine(RemainText));
            }
        // _... 
        } else {
            size_t I = 1;
            while(I<LineText.size() && LineText[I]!='_') I++;
            //_..._...
            if(I<LineText.size() && LineText[I]=='_') {
                string LeftText = LineText.substr(1,I-1);
                I++;
                string RightText = LineText.substr(I,LineText.size()-I);
                WriteBack(NormalLineHTML, "<em>");
                WriteBack(NormalLineHTML, CompileNormalLine(LeftText));
                WriteBack(NormalLineHTML, "</em>");
                WriteBack(NormalLineHTML, CompileNormalLine(RightText));
            } else {
                string RemainText = LineText.substr(1,LineText.size()-1);
                WriteBack(NormalLineHTML, "_");
                WriteBack(NormalLineHTML, CompileNormalLine(RemainText));
            }
        }

    // Strickout
    } else if(LineHead=='~') {
        // ~~...
        if(LineText.size()>1 && LineText[1]=='~') {
            size_t I = 2;
            while(I<LineText.size() && LineText[I]!='~') I++;
            // ~~...~~...
            if(I+1<LineText.size() && LineText[I+1]=='~') {
                string LeftText = LineText.substr(2,I-2);
                I += 2;
                string RightText = LineText.substr(I,LineText.size()-I); 
                WriteBack(NormalLineHTML, "<s>");
                WriteBack(NormalLineHTML, CompileNormalLine(LeftText));
                WriteBack(NormalLineHTML, "</s>");
                WriteBack(NormalLineHTML, CompileNormalLine(RightText));
            } else {
                string RemainText = LineText.substr(1,LineText.size()-1);
                WriteBack(NormalLineHTML, "~");
                WriteBack(NormalLineHTML, CompileNormalLine(RemainText));
            }
        // ~...
        } else {
            string RemainText = LineText.substr(1,LineText.size()-1);
            WriteBack(NormalLineHTML, "~");
            WriteBack(NormalLineHTML, CompileNormalLine(RemainText));
        }

    // CodeText
    } else if(LineHead=='`') {
        size_t I = 1;
        while(I<LineText.size() && LineText[I]!='`') I++;
        if(I<LineText.size() && LineText[I]=='`') {
            string LeftText = LineText.substr(1,I-1);
            I++;
            string RightText = LineText.substr(I,LineText.size()-I);
            WriteBack(NormalLineHTML, "<code>");
            WriteBack(NormalLineHTML, CompileNormalLine(LeftText));
            WriteBack(NormalLineHTML, "</code>");
            WriteBack(NormalLineHTML, CompileNormalLine(RightText));
        } else {
            string RemainText = LineText.substr(1,LineText.size()-1);
            WriteBack(NormalLineHTML, "`");
            WriteBack(NormalLineHTML, CompileNormalLine(RemainText));
        }
        
    // Word
    } else {
        size_t I = 1;
        while(I<LineText.size() && !IsSpecial(LineText[I])) I++;
        string LeftText = LineText.substr(0,I);
        string RightText = LineText.substr(I,LineText.size()-I);
        WriteBack(NormalLineHTML, LeftText);
        WriteBack(NormalLineHTML, CompileNormalLine(RightText));
    }

    return NormalLineHTML;
}

string CompileHeadLine(string& LineText) {
    string HeadLineHTML;
    int HeadLineLevel = 0;

    size_t I = 0;
    while(LineText[I]=='#' && I<LineText.size()) {
        HeadLineLevel++;
        I++;
    }

    if(I<LineText.size() && LineText[I]==' ' && HeadLineLevel<=6) {
        string HeadLineLevelStr = to_string(HeadLineLevel);
        WriteBack(HeadLineHTML, "<h"+HeadLineLevelStr+">");

        string NormalLineText = LineText.substr(I+1,LineText.size()-(I+1));
        WriteBack(HeadLineHTML, CompileNormalLine(NormalLineText));
        
        WriteBack(HeadLineHTML, "</h"+HeadLineLevelStr+">");
    } else WriteBack(HeadLineHTML, CompileNormalLine(LineText));

    return HeadLineHTML;
}

string CompileUnorderedList(string& LineText) {
    string ListHTML;

    if(LineText.size()>1 && LineText[1]==' ') {
        WriteBack(ListHTML, "<ul>\n<li>");
        
        string NormalLineText = LineText.substr(2,LineText.size()-2);
        WriteBack(ListHTML, CompileNormalLine(NormalLineText));

        WriteBack(ListHTML, "</li>\n</ul>");
    } else WriteBack(ListHTML, CompileNormalLine(LineText));

    return ListHTML;
}

string CompileOrderedList(vector<string>& AllText, size_t& CurrentIndex) {
    string ListHTML;
    vector<string> ListText;

    size_t I = CurrentIndex;
    while(1) {
        if(I>=AllText.size()) break;
        string LineText = AllText[I];

        if(LineText=="") {
            I++;
            continue;
        }
        if(!isdigit(LineText[0])) break;

        size_t J = 0;
        while(J<LineText.size() && isdigit(LineText[J]))
            J++;
        if(J>=LineText.size() || LineText[J]!='.') break;
        J++;
        if(J>=LineText.size() || LineText[J]!=' ') break;
        J++;

        string NormalLineText = LineText.substr(J,LineText.size()-J);
        ListText.push_back(CompileNormalLine(NormalLineText));

        I++;
    }

    if(!ListText.empty()) {
        WriteBack(ListHTML, "<ol>\n");
        for(string& text:ListText)
            WriteBack(ListHTML, "<li>"+text+"</li>\n");
        WriteBack(ListHTML, "</ol>");

        if(I<AllText.size()) 
            WriteBack(ListHTML, "\n");
    }

    if(I<AllText.size()) 
        WriteBack(ListHTML, CompileNormalLine(AllText[I]));
    
    CurrentIndex = I+1;
    return ListHTML;
}

string CompileImage(string& LineText) {
    string ImageHTML;
    string ImageSrc;
    string ImageAlt;

    size_t Begin = 1;
    if(Begin>=LineText.size() || LineText[Begin]!='[') 
        return CompileNormalLine(LineText);
    Begin++;

    size_t End = Begin;
    while(End<LineText.size() && LineText[End]!=']') End++;
    if(End>=LineText.size() || LineText[End]!=']')
        return CompileNormalLine(LineText);
    ImageAlt = LineText.substr(Begin,End-Begin);
    
    Begin = End + 1;
    if(Begin>=LineText.size() || LineText[Begin]!='(')
        return CompileNormalLine(LineText);
    Begin++;
    
    End = Begin;
    while(End<LineText.size() && LineText[End]!=')') End++;
    if(End>=LineText.size() || LineText[End]!=')')
        return CompileNormalLine(LineText);
    ImageSrc = LineText.substr(Begin,End-Begin);

    WriteBack(ImageHTML, "<img");
    WriteBack(ImageHTML, " src=\""+ImageSrc+"\"");
    WriteBack(ImageHTML, " alt=\""+ImageAlt+"\"");
    WriteBack(ImageHTML, ">");

    return ImageHTML;
}

string CompileLink(string& LineText) {
    string LinkHTML;
    string LinkHref;
    string LinkText;

    size_t Begin = 1;
    size_t End = Begin;
    while(End<LineText.size() && LineText[End]!=']') End++;
    if(End>=LineText.size() || LineText[End]!=']')
        return CompileNormalLine(LineText);
    LinkText = LineText.substr(Begin,End-Begin);

    Begin = End + 1;
    if(Begin>=LineText.size() || LineText[Begin]!='(')
        return CompileNormalLine(LineText);
    Begin++;

    End = Begin;
    while(End<LineText.size() && LineText[End]!=')') End++;
    if(End>=LineText.size() || LineText[End]!=')')
        return CompileNormalLine(LineText);
    LinkHref = LineText.substr(Begin,End-Begin);
    
    WriteBack(LinkHTML, "<a href=\""+LinkHref+"\">");
    WriteBack(LinkHTML, LinkText);
    WriteBack(LinkHTML, "</a>");

    return LinkHTML;
}

string CompileReference(string& LineText) {
    string ReferenceHTML;

    if(LineText.size()>1 && LineText[1]==' ') {
        WriteBack(ReferenceHTML, "<blockquote>");

        string NormalLineText = LineText.substr(2,LineText.size()-2);
        WriteBack(ReferenceHTML, CompileNormalLine(NormalLineText));

        WriteBack(ReferenceHTML, "</blockquote>");
    } else WriteBack(ReferenceHTML, CompileNormalLine(LineText));

    return ReferenceHTML;
}

string CompileCodeBlock(vector<string>& AllText, size_t& CurrentIndex) {
    string CodeBlcokHTML;
    string CodeText;

    size_t I = CurrentIndex+1;
    while(I<AllText.size() && AllText[I]!="```") {
        CodeText += AllText[I] + "\n";
        I++;
    }

    if(I<AllText.size() && AllText[I]=="```") {
        WriteBack(CodeBlcokHTML, "<pre><code>");
        WriteBack(CodeBlcokHTML, CodeText);
        WriteBack(CodeBlcokHTML, "</code></pre>");
        CurrentIndex = I+1;
    } else {
        WriteBack(CodeBlcokHTML, "```");
        CurrentIndex++;
    }

    return CodeBlcokHTML;
}

string CompileEmbedHTML(vector<string>& AllText, size_t& CurrentIndex) {
    string EmbedHTML;
    
    size_t I = CurrentIndex+1;
    while(I<AllText.size() && AllText[I]!=">>") {
        EmbedHTML += AllText[I] + "\n";
        I++;
    }

    if(I<AllText.size() && AllText[I]==">>") {
        EmbedHTML.pop_back();
        CurrentIndex = I+1;
    } else {
        EmbedHTML = "<<";
        CurrentIndex++;
    }

    return EmbedHTML;
}



// Help
void Error(string Message) {
    cout<<"[ERROR] "<<Message<<endl;
    exit(-1);
}

void WriteFront(string& Target, string Text) {
    Target = Text + Target;
}

void WriteBack(string& Target, string Text) {
    Target = Target + Text;
}
