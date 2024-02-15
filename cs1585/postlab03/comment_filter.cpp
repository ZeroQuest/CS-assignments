#include<iostream>
#include<string>

using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::istream;

int main() 
{
    // Write your program here.
    
   string text = "start";
   //char delim = '#';

   do
   {
     getline(cin, text);

     if(text.front() != '#')
     {
       // getline(cin, text);
        cout << text;
     }

   } while (cin.peek() != EOF);

    return 0;
}

