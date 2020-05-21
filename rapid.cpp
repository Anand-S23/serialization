#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "../rapidjson/document.h"
#include "../rapidjson/writer.h"
#include "../rapidjson/stringbuffer.h"

const std::vector<std::string> split(const std::string &line, const char &c)
{
    std::string buffer{""};
    std::vector<std::string> words;
    
    for(auto letter : line)
    {
        if(letter != c)
        {
            buffer += letter;
        }
        else if(letter == c && buffer != "")
        {
            words.push_back(buffer);
            buffer = "";
        }
    }

    if(buffer != "")
    {
        words.push_back(buffer);
    }
    
    return words;
}

std::string text_to_json_str(std::string data)
{
    // -- Provinces --
    // name
    // feilds - own, player_units, enemy_units
    // settlements
    //   -  name
    //   -  own
    //   -  units
    //        -  name
    //        -  hp
    //        -  team
    //        -  class
    //        -  Directional movement

    std::string Result = "";
    std::istringstream f(data);
    std::string line;    

    std::getline(f, line); // # of Provinces
    std::string::size_type sz;   
    int numOfProv = std::stoi (line, &sz);

    Result += "{\"provinces\":[";
    // NOTE: condtion should be i < numOfProv
    for (int i = 0; i < 1; ++i) 
    {
        Result += "{";

        std::getline(f, line); // line = Province #

        std::getline(f, line); // line = Name: [name] 
        std::vector<std::string> splitLine{split(line, ' ')};
        Result += "\"name\":\"" + splitLine[1] + "\",";

        std::getline(f, line); // line = Settlements:
        std::getline(f, line); // line = Field

        std::getline(f, line); // line = [own #]
        Result += "\"field\":{\"own\":" + line + ",\"player_unit\":[";

        std::getline(f, line); // [unit]  or End_Unit
        // TODO: figure out if player or enemy units
        while (line != "End_Unit") 
        {
            //TODO: Handle if player unit
            //TODO: Handle if enemy unit
            std::getline(f, line); // [unit] or End_Unit
        }

        Result += "]},\"settlements\":";
        std::getline(f, line); // [settlement name]
        while (line != "End")
        {
            Result += "{\"name\":\"" + line + "\",";

            std::getline(f, line); // [sttlement own #]
            Result += "\"own\":" + line + ",\"units\":[";

            std::getline(f, line); // [unit name] [class] Team: [team #]
            // TODO: Fix infinte while loop
            //while (line != "End_Unit")
            //{
                std::vector<std::string> unitInfo{split(line, ' ')};

                if (unitInfo.size() >= 6)
                {
                    Result += "{\"name\":\"" + unitInfo.at(0) + "\",\"HP\":10,";
                    Result += "\"team\":" + unitInfo.at(3) + ",";

                    if (unitInfo[1] == "0") {Result += "\"class\":\"Archer\",";}
                    if (unitInfo[1] == "1") {Result += "\"class\":\"Soldier\",";}
                }

                // TODO: fix bad alloc
                /*
                std::vector<std::string> movement;
                std::string movement[8] = {};
                for (int j = 0; j < 4; ++j)
                {
                    std::getline(f, line);
                    std::getline(f, line);
                    movement[j] = line;
                    std::getline(f, line);
                    movement[j+1] = line;
                }

                Result += "\"NORTH_MVMNT\":[" + movement[6] + "," + movement[7] + "],";
                Result += "\"EAST_MVMNT\":[" + movement[0] + "," + movement[1] + "],";
                Result += "\"SOUTH_MVMNT\":[" + movement[2] + "," + movement[3] + "],";
                Result += "\"WEST_MVMNT\":[" + movement[4] + "," + movement[5] + "],"; 
                */

                Result += "},";
                std::getline(f, line);
                std::getline(f, line);
                //}
        }
    }

    std::cout << Result << std::endl;
    return Result; 
}

int main()
{
    std::ifstream wmData("test.txt");
    
    /*
    FILE* fp = fopen("test.json", "r");
    fseek(fp, 0, SEEK_END);
    size_t filesize = (size_t)ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char* buffer = (char*)malloc(filesize + 1);
    size_t readLength = fread(buffer, 1, filesize, fp);
    buffer[readLength] = '\0';
    fclose(fp);
    */

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
    // TODO: move ^ jsonString to a file

    rapidjson::Document configData;
    if (configData.ParseInsitu((char *)jsonString.c_str()).HasParseError())
    {
        std::cout << "Error while parsing" << std::endl;
    }
    else
    {
        std::cout << jsonString << std::endl;
    }


    return 0;
}
