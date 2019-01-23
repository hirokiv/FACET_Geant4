//++++++++++++++++++++++++++++++++++++++++++++++
//  CSVReader.hh
//++++++++++++++++++++++++++++++++++++++++++++++
#ifndef CSVReader_h
#define CSVReader_h 1
#include <string>
#include <vector>

/*
 * A class to read data from a csv file.
 */

class CSVReader
{
        std::string fileName;
        std::string delimeter;

public:
        CSVReader(std::string filename, std::string delm = ",") :
                        fileName(filename), delimeter(delm)
        { };
        // Function to fetch data from a CSV File
        std::vector<std::vector<std::string> > getData();
 //       CSVReader :: getData();
};


#endif

