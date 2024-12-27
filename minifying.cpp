#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void minifying(string const &input_file, string const &output_file);

int main()
{
    string s = "./sample.xml";
    string o = "./sample_output.xml";

    minifying(s, o);
}

void minifying(string const &input_file, string const &output_file)
{
    ifstream input_var(input_file);
    ofstream output_var(output_file);
    string temp1 = ""; // Empty string to store lines from xml file
    string temp2 = "";
    bool flag = false;
    // check if the file is opened successfully
    if (!input_var.is_open()){
        cout << "Can not open the file please, Try again!";
        return;
    }
    char previous ;
    while (getline(input_var, temp1)){
        for(char c : temp1){
            if(c == '>') {
                flag = true ;
                temp2 = temp2 + c ;
                continue;
            }
            if(c == ' ' && flag)
                continue;
            else
                flag = false ;

            temp2 = temp2+c;
            if(c == '/') {
                for (int i = temp2.length() - 3; i >= 0; i--) {
                    if(temp2[i]==' '){
                        temp2.erase(i,1);
                    }
                    else
                        break;
                }
            }
        }
    }
    output_var << temp2 ;
    input_var.close();
    output_var.close();
}
