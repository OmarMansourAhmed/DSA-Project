#include "minifying.h"
#include <fstream>
#include <string>
#include <iostream>

void minifying(const std::string &input_file, const std::string &output_file) {
    std::ifstream input_var(input_file);
    std::ofstream output_var(output_file);
    std::string temp1 = "";
    std::string temp2 = "";
    bool flag = false;

    if (!input_var.is_open()) {
        throw std::runtime_error("Cannot open the input file: " + input_file);
    }

    while (std::getline(input_var, temp1)) {
        for (char c : temp1) {
            if (c == '>') {
                flag = true;
                temp2 += c;
                continue;
            }
            if (c == ' ' && flag)
                continue;
            else
                flag = false;

            temp2 += c;
            if (c == '/') {
                for (int i = temp2.length() - 3; i >= 0; i--) {
                    if (temp2[i] == ' ') {
                        temp2.erase(i, 1);
                    } else
                        break;
                }
            }
        }
    }
    output_var << temp2;
    input_var.close();
    output_var.close();
}
