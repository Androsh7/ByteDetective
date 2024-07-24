#include <iostream>
#include <fstream>
#include <string>
#include <regex>
using namespace std;

#define minstrlength 1 // any sequence of characters less than this is excluded from search
#define maxstrlength 65536 // max number of bytes to parse at once

#define networking_regex regex(R"(socket|(WS2_32|NETAPI|WSOCK(32)?)\.dll)", std::regex_constants::ECMAScript | std::regex_constants::icase)
#define file_regex regex(R"(/fstream|COMCTL32\.dll/gmi)", std::regex_constants::ECMAScript | std::regex_constants::icase)
#define persistence_regex regex(R"(abcdefghijk)", std::regex_constants::ECMAScript | std::regex_constants::icase)
#define base64encoding regex(R"([A-Za-z0-9+\/]{25,}=?=?)", std::regex_constants::ECMAScript | std::regex_constants::icase)
#define find_files regex(R"(([A-Z]:\\)?([\w!@#$%&()_+{}[\];',.\- ]+\\)+([\w!@#$%&()_+{}[\];',.\- ]+)?)", std::regex_constants::ECMAScript | std::regex_constants::icase)

void detectcapability(string instring, regex regstr, bool outbool, string printstring){
    smatch results;
    regex_search(instring, results, regstr);
    if (results.empty()) return;
    cout << printstring << endl;
    for (auto item : results){
        cout << item << endl;
    }
}

int main() {
    string filename = "GhidraTester.exe";
    ifstream binfile(filename, std::ios::binary);
    if (!binfile) {
        cerr << "Failed to open binary file!" << endl;
        return 1;
    }

    // defines capability flags
    bool networking = false;
    bool file = false;
    bool persistence = false;
    bool base64 = false;
    
    while (binfile){

        string search_str = "";

        // adds newline delimited character sequences to the search string until the string reaches the maxstrlength or reaches the end of the file
        while (binfile){
            
            // grabs printable characters from the 
            string outstring = "";
            while(binfile){
                char character;
                binfile.get(character);
                if (character >= 0x20 && character <= 0x7E){ // all valid ASCII characters
                    outstring += character;
                }
                else break;
            }

            if (search_str.length() < minstrlength) continue; // restart if the outstring is not large enough
            search_str += outstring;
            cout << outstring << endl;
            cout << outstring << endl;
            if (search_str.length() < maxstrlength) search_str += '\n'; // add a newline if there are more characters to come
            else break; // break from loop once max length is reached
        }

        // checks for capabilities
        detectcapability(search_str, networking_regex, networking, "----- NETWORKING CAPABILITY DETECTED -----");
        detectcapability(search_str, file_regex, file, "----- FILE EDITING CAPABILITY DETECTED -----");
        detectcapability(search_str, persistence_regex, persistence, "----- PERSISTENCE CAPABILITY DETECTED -----");
        detectcapability(search_str, find_files, NULL, "----- INTERESTING FILES FOUND -----");
    }
    binfile.close();
    return 0;
}
