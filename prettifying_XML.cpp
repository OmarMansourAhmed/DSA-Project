#include <prettifying_XML.h>

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
                    formatted_XML.append(string(tag.size(),'\t')+inside+"\n");
                    inside.clear();
                }
                inside = inside + c;
            }else if(c=='>'){
                inside = inside + c;
                if (inside[1]=='/'){ //closed tag
                    tag.pop();
                    formatted_XML.append(string(tag.size(),'\t')+inside+"\n");
                }else if(inside[inside.size()-2]=='/'){ //self closed tag
                    formatted_XML.append(string(tag.size(),'\t')+inside+"\n");
                }else{ //open tag
                    formatted_XML.append(string(tag.size(),'\t')+inside+"\n");
                    tag.push(inside);
                }
                inside.clear();
            }else{
                inside = inside + c;
            }
        }
    }
    
    output << formatted_XML;
    
    input.close();
    output.close();
    cout << "XML prettified successfully and saved." << endl;
}