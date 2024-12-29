#include <Parse_XML.h>

// Helper function to erase spaces from a string (assuming it's defined elsewhere)
string Erase_Spaces(const string &str)
{
  string result;
  for (char c : str)
  {
    if (!isspace(c))
      result += c;
  }
  return result;
}

// Helper function to check if a character is valid data
bool isData(char c)
{
  return !isspace(c) && c != '<' && c != '>';
}

// Function to parse XML data into a vector of strings
vector<string> XML_Parsing_Data(const string &data)
{
  bool Inside_Tag = false, Inside_Data = false, Outside_Tag = false, Outside_Data = false;
  vector<string> XML;
  string New_Str = "", Temp = "";

  for (unsigned int i = 0; i < data.length(); i++)
  {
    // Add Begin of Tag
    if (data[i] == '<')
    {
      Inside_Tag = true;
      Outside_Tag = false;
      Inside_Data = false;
      Outside_Data = false;
      if (New_Str != "")
      {
        XML.push_back(Erase_Spaces(New_Str));
        New_Str = "";
      }
    }

    // Add End of Tag
    else if (data[i] == '>')
    {
      Inside_Tag = false;
      Outside_Tag = true;
      Inside_Data = false;
      Outside_Data = false;
      New_Str += data[i];
      XML.push_back(New_Str);
      New_Str = "";
    }

    // Get Data characters and the middle spaces
    if (isData(data[i]) && !Inside_Tag)
    {
      Outside_Tag = false;
      Inside_Data = true;
      Outside_Data = false;
      New_Str += Temp;
      Temp = "";
    }

    // Add Spaces to Temporary string to check if they are mid spaces or last spaces
    if ((data[i] == ' ' || data[i] == '\n' || data[i] == '\t') && Inside_Data)
    {
      Inside_Tag = false;
      Inside_Data = false;
      Outside_Data = true;
      Temp += data[i];
    }

    // Add Tag Names and Attributes
    if (Inside_Tag)
    {
      New_Str += data[i];
    }

    // Add Tag Value without begin (Spaces or \t or \n)
    if (Inside_Data && !Outside_Tag)
    {
      New_Str += data[i];
    }
  }

  return XML;
}

// Function to read the XML file and parse its content
vector<string> Parse_XML_File(const string &file_path)
{
  ifstream file(file_path);
  if (!file.is_open())
  {
    cerr << "Error: Could not open file " << file_path << endl;
    return {};
  }

  string data = "", line;
  while (getline(file, line))
  {
    data += line + "\n"; // Preserve line breaks for accurate parsing
  }

  file.close();
  return XML_Parsing_Data(data);
}

char Get_Type(string str)
{
  if (str[0] == '<')
  {
    if (str[1] == '/')
      return 'C';
    else if (str[1] == '!' && str[2] == '-' && str[3] == '-')
      return 'C';
    return 'O';
  }
  return 'V';
}

string Get_Tag(string Open_Tag)
{
  string open = "";
  for (unsigned int i = 1; i < Open_Tag.length(); i++)
  {
    while (Open_Tag[i] != ' ' && Open_Tag[i] != '>')
    {
      open += Open_Tag[i++];
    }
    break;
  }
  return open;
}

bool Compare_Tags(string Open_Tag, string Close_Tag)
{
  string open = "";
  open = Get_Tag(Open_Tag);
  return (Erase_Spaces(open) == Erase_Spaces(Close_Tag.substr(2, Close_Tag.length() - 3)));
}

int checking_errors(vector<string> &strr, vector<string> &modified, vector<int> &error)
{
  int Error_count = 0;
  int current = 0;
  bool check = false;
  stack<string> s;
  string nn = "";

  for (unsigned int i = 0; i < strr.size(); i++)
  {
    if (current == i - 2)
      check = false;

    if (strr[i][0] != '<')
    {
      modified.push_back(strr[i]);
      continue;
    }

    if ((strr[i][0] == '<' && strr[i][1] == '!') || (strr[i][0] == '<' && strr[i][1] == '?'))
    { //<?dsf>  <!dsf>
      modified.push_back(strr[i]);
      continue;
    }

    if (strr[i][0] == '<' && strr[i][1] != '/' && strr[i][1] != '!')
    {
      if (!s.empty() && i != 0 && strr[i - 1][0] != '<')
      {
        error.push_back(i - 1);
        Error_count++;
        nn = s.top();
        nn.insert(1, "/");
        modified.push_back(nn); //<name> </name>
        s.pop();
      }
      s.push(strr[i]);
      modified.push_back(strr[i]);
    }
    else if (strr[i][0] == '<' && strr[i][1] == '/')
    {
      if (!s.empty())
      {
        string er = s.top();
        if (Compare_Tags(er, strr[i]))
        {
          s.pop();
          modified.push_back(strr[i]);
        }
        else
        {
          current = i;
          string er = s.top();
          string app = er.insert(1, "/");
          modified.push_back(app);
          s.pop();
          if (!check)
          {
            error.push_back(i);
            Error_count++;
          }
          check = true;
        }
      }
      else
      {
        // Unmatched closing tag
        error.push_back(i);
        Error_count++;
        modified.push_back(strr[i]);
      }
    }
  }

  // Add missing closing tags for unmatched opening tags
  while (!s.empty())
  {
    string m = s.top();
    m.insert(1, "/");
    modified.push_back(m);
    s.pop();
  }

  return Error_count;
}
void Write_XML_File(const string &file_path, const vector<string> &tokenized_data)
{
  ofstream file(file_path);
  if (!file.is_open())
  {
    cerr << "Error: Could not open file " << file_path << " for writing." << endl;
    return;
  }

  for (const string &element : tokenized_data)
  {
    file << element << endl; // Write each element to a new line
  }

  file.close();
  cout << "File written successfully to: " << file_path << endl;
}

 // Example usage
// int main()
// {
//   string file_path = "test.xml"; // Path to your XML file
//   string output_file = "output.xml";
//   vector<string> parsed_data = Parse_XML_File(file_path);

//     vector<string> correctedTags;
//     vector<int> errorIndices;

//     // Validate and correct the tags
//     int errorCount = checking_errors(parsed_data, correctedTags, errorIndices);

//     Write_XML_File(output_file , correctedTags);
//         // for (const string &element : correctedTags)
//         // {
//         //   cout << element << endl;
//         // }

//         return 0;
// }
