#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>

using namespace std;

class Database
{

public:
    // Functions
    static int CreateDatabase();
    static int SampleData();

    // time convert
    time_t static ConvertToUnixTime(const string &datetimeStr); // "%Y-%m-%d %H:%M:%S"

    // covnert Unix time to string
    string static ConvertDatetimeToString(const time_t &unixTime);
};

#endif
