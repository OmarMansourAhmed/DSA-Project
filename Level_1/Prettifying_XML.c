#include<iostream>
#include<stack>
#include<string>
using namespace std;



string prettifying(const string& XML){
    stack<string> tag;
    string inside;  //take words between tags
    string formatted_XML;

    
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
            }else{ //open tag
                formatted_XML.append(string(tag.size(),'\t')+inside+"\n");
                tag.push(inside);
            }
            inside.clear();
        }else{
            inside = inside + c;
        }
    }
    return formatted_XML;
}

int main(){
    string XML = "<users><user><id>5</id><name>Alice</name><posts><post>Post 1</post><post>Post 2</post></posts></user><user><id>10</id><name>Bob</name><posts><post>Post 3</post></posts></user></users>";
    string formatted_XML = prettifying(XML);
    cout<<"Da l gamal ya habeby\n"<<formatted_XML;

    return 0;
}