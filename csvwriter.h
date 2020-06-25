#include <iostream>
#include <sstream>
using namespace std;
class CSVWriter
{
public:
    bool firstLine;
    string seperator;
    int valueCount;
    stringstream strings;
    CSVWriter(string seperator) {
        this->firstLine = true;
        this->seperator = seperator;
        this->valueCount = 0;
    }

    CSVWriter& add(const char* str) {
        return this->add(string(str));
    }

    CSVWriter& add(char* str) {
        return this->add(string(str));
    }

    CSVWriter& add(string str) {
        //if " character was found, escape it
        size_t pos = str.find("\"", 0);
        bool quofound = pos != string::npos;
        while (pos != string::npos) {
            str.insert(pos, "\"");
            pos = str.find("\"", pos + 2);
        }
        if (quofound) {
            str = "\"" + str + "\"";
        }
        else if (str.find(this->seperator) != string::npos) {
            str = "\"" + str + "\"";
        }
        return this->add<string>(str);
    }

    template<typename T>
    CSVWriter& add(T str) {
        if (valueCount > 0)
            this->strings << this->seperator;
        this->strings << str;
        this->valueCount++;

        return *this;
    }

    template<typename T>
    CSVWriter& operator<<(const T& t) {
        return this->add(t);
    }

    string getstr() {
        return strings.str();
    }

    friend ostream& operator<<(std::ostream& ostr, CSVWriter& csv) {
        return ostr << csv.getstr();
    }

    CSVWriter& addLine() {
        if (!this->firstLine) {
            strings << endl;
        }
        else {
            this->firstLine = false;
        }
        valueCount = 0;
        return *this;
    }
};