#define minstrlength 4 // any sequence of characters less than this is excluded from search
#define maxstrlength 65536 // max number of bytes to parse at once
#define networking_regex regex(R"(socket|(WS2_32|NETAPI|WSOCK(32)?)\.dll)", std::regex_constants::ECMAScript | std::regex_constants::icase)
#define file_regex regex(R"(/fstream|COMCTL32\.dll/gmi)", std::regex_constants::ECMAScript | std::regex_constants::icase)
#define persistence_regex regex(R"(a)", std::regex_constants::ECMAScript | std::regex_constants::icase)
#define base64encoding regex(R"([A-Za-z0-9+\/]{25,}=?=?)", std::regex_constants::ECMAScript | std::regex_constants::icase)
#define find_files regex(R"(([A-Z]:\\|\/)?([\w!@#$%&()_+{}[\];',.\- ]+(\\|\.))+)", std::regex_constants::ECMAScript | std::regex_constants::icase)
