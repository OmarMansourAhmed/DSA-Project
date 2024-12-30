#include <prettifying_XML.h>

string repeatString(const std::string& str, int times) {
    std::string result;
    for (int i = 0; i < times; ++i) {
        result += str;
    }
    return result;
}

void prettifyXML(const string& inputFile, const string& outputFile) {
    ifstream input(inputFile);
    ofstream output(outputFile);

    if (!input.is_open()||!output.is_open()) {
        cout << "Can't open the input or the output file.\nTry Again!!" << endl;
        return;
    }

    stack<string> tag;
    string inside;  //take words between tags
    string XML;
    string formatted_XML;

    while (getline(input,XML)){
        for(char c:XML){
            if (c=='<'){
                if(!inside.empty()){
                    formatted_XML += repeatString("    ",tag.size())+inside+"\n";
                    inside.clear();
                }
                inside = inside + c;
            }else if(c=='>'){
                inside = inside + c;
                if (inside[1]=='/'){ //closed tag
                    tag.pop();
                    formatted_XML += repeatString("    ",tag.size())+inside+"\n";
                }else if(inside[inside.size()-2]=='/'){ //self closed tag
                    formatted_XML += repeatString("    ",tag.size())+inside+"\n";
                }else{ //open tag
                    formatted_XML += repeatString("    ",tag.size())+inside+"\n";
                    tag.push(inside);
                }
                inside.clear();
            }else{
                inside = inside + c;
            }
        }
        output << formatted_XML;
        formatted_XML.clear();
    }
        
    input.close();
    output.close();
    cout << "XML prettified successfully and saved." << endl;
}
