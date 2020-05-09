#include <iostream>
#include <fstream>
#include <sstream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

std::string text_to_jsonstr(std::string data)
{
}

int main()
{
    std::ifstream wmData("test.txt");

    if (!wmData)
    {
        std::cout << "file not found" << std::endl;
        exit(0);
    }
    
    std::stringstream bStream;
    bStream << wmData.rdbuf();
    std::string fBuffer = bStream.str();

    // NOTE: text_to_jsonstr must work for anything else to work
    std::string jsonString = text_to_json_str(fBuffer);
    rapidjson::Document configData;
    if (configData.Parse(testPass).HasParseError())
    {
        std::cout << "Error while parsing" << std::endl;
    }

    return 0;
}
