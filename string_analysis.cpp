#include <iostream>
#include <fstream>
#include <string>
#include <regex>
using namespace std;

#define dll_list_len 13
const string know_dll_list[dll_list_len] = {"HAL", "NTDLL", "KERNEL32",          "GDI32", "USER32",      "COMCTL32", "WS2_32", "WSOCK",  "WSOCK32", "COMDLG32", "ADVAPI32", "NETAPI32", "OLE32"};
const string know_dll_use[dll_list_len] =  {"BASE" "BASE",	"FILE_IO|PROC_MGMT", "GUI",   "GUI|KEYDATA", "FILE_IO",  "SOCKET", "SOCKET", "SOCKET",  "APP",      "REGISTER", "SOCKET"  , "OTHER"};

int main(int argc, char* argv[]) {
    cout << "----- StringAnalysis V0.2 by: Androsh7 ------\n";

    // checks for 2 to 3 arguments
    if (argc < 2 || argc > 5){
        cerr << "Invalid Arguments, expected between 1 and 3 arguments and received " << argc - 1 << " arguments\n";
        return 1;
    }

    // command flags
    bool bintostr = false;
    bool analysis = false;
    bool debug = false;
    bool help = false;

    // fills out flags according to user entered flags
    int argcount = 1;
    for (argcount = 1; argcount < argc; argcount++){
        string argument = argv[argcount];
        cout << argument << '\n';
        int argsize = argument.size();
        if (argument.at(0) != '-' && argument.at(0) != '/'){
            break;
        }
        
        // single character arguments (short hand)
        if (argument.at(1) != '-'){
            if (argument.find('b') < argsize) bintostr = true;
            if (argument.find('a') < argsize) analysis = true;
            if (argument.find('d') < argsize) debug = true;
            if (argument == "/?") help = true;
        }

        // string arguments (long hand)
        else if (argument.at(1) == '-'){
            if (argument == "--bintostr") bintostr = true;
            else if (argument == "--analysis") analysis = true;
            else if (argument == "--help") help = true;
            else if (argument == "--debug") debug = true;
        }
    }

    string filename = "";
    int minstrlen = 4;
    if (bintostr || analysis){
        if (argcount >= argc){
            cerr << "INSUFFICIENT ARGUMENTS: No filename was specified\n";
            return 1;
        }
        filename = string(argv[argcount]);
    }

    if (debug){
        cout << "User Selection flags:\n"
        << "bintostr: " << bintostr << '\n'
        << "analysis: " << analysis << '\n'
        << "help: " << help << '\n'
        << "debug: " << debug << '\n'
        << "filename: " << filename << '\n';
    }

    if (!bintostr && !analysis && !help) {
        cout << "No valid option provided\n\n";
        help = true;
    }

    // help arguments
    if (help){
        cout
        << "malware_analysis V0.2: a tool made by Androsh7 for analyzing executable binaries and determining potential capabilities\n\nSyntax:\n" 
        << "\t-b, --bintostr {binaryfile} [minstrlen] : this reads an executable binary and trys to grab strings of a length equal to or greater than minstrlen The strings are then written to a new file which is made by appending '.string' to the filename i.e: malware.exe becomes malware.exe.string. Note the default for minstrlen is 4 characters\n"
        << "\t-a, --analysis {stringfile} : this reads a string file (create with option -b) and creates a report and writes it to a newfile named by appending '.report' to the filename i.e: malware.exe.string becomes malware.exe.string.report\n"
        << "\t-d, --debug : this prints optional debug optional (similar to -v verbose option on linux commands)\n"
        << "\t/\?, --help : brings up this help menu\n";
        return 0;
    }
    // read binary executable and write strings to text file
    if (bintostr){

        // opens file specified with filename and creates new file: 'filename.string' for the output
        ifstream binfile(filename, std::ios::binary);
        ofstream strdata(filename + ".string", std::ios::out);
        if (!binfile) {
            cerr << "Failed to open binary file!" << endl;
            return 2;
        }
        if (debug) cout << "successfully opened " << filename << " for reading\n";
        if (!strdata.is_open()){
            cerr << "Failed to open output file!" << endl;
            return 2;
        }
        if (debug) cout << "successfully opened " << filename + ".string" << " for writing\n";
        
        // reads the binfile and returns each ASCII string matching the minstrlength to the  foundstrings.txt file
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

            if (outstring.length() < minstrlen) continue; // restart if the outstring is not large enough
            strdata << outstring << '\n';
            //if (debug_mode) cout << outstring << endl;
        }
        binfile.close();
        strdata.close();

        filename += ".string";
    }
    
    // perform analysis on a .string file and generates a report
    if (analysis){
        // check flags
        vector<string> base64;
        vector<string> dll;
        vector<string> web;
        bool networking;
        bool registry;

        ifstream stringfile;
        stringfile.open(filename, std::ios::in);
        if (!stringfile.is_open()){
            cerr << "Failed to open file " << filename << " for reading\n";
            return 2;
        }
        cout << "Successfully opened " << filename << " for reading\n";

        int linenum = 0;
        while (stringfile.is_open() && !stringfile.eof()){
            string line;
            getline(stringfile, line);

            if (linenum % 10000 == 0 && linenum != 0){
                cout << "Reading line " << linenum << endl;
            }
            
            // Simple DLL detection
            if (regex_match(line,regex(R"(\w+\.dll)", regex::icase))){
                string outstr = "Line " + to_string(linenum) + " - " + line;
                dll.push_back(outstr);
            }

            // Base64 encoding detection
            if (regex_match(line,regex(R"([a-zA-Z0-9+/]{30,}=?=?|[a-zA-Z0-9+/]{10,}==?)"))){
                string outstr = "Line " + to_string(linenum) + " - " + line;
                base64.push_back(outstr);
            }
            linenum++;
        }
        cout << "Closed file " << filename << "\n\n";
        cout << "----- REPORT -----\n"
        << "Line numbers based on " << filename << "\n";
        
        // print DLLS
        cout << "DLLS found:\n";
        for (int i = 0; i < dll.size(); i++){
            cout << '\t' << dll.at(i) << '\n';
        }

        // print Base64 Encoding
        cout << "Base64 Encoding Detected:\n";
        for (int i = 0; i < base64.size(); i++){
            cout << '\t' << base64.at(i) << '\n';
        }
    }
}
