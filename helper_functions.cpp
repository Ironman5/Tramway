/* *
  * Ohjelman kirjoittaja
 * Nimi: Mika Valtonen
 * Opiskelijanumero: 166364
 * Käyttäjätunnus: valtone6
 * E-Mail: mika.valtonen@tuni.fi
* */

#include "helper_functions.hh"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <utility>

const std::string READ_ERROR = "Error: File could not be read.";
const std::string EXIST_ERROR = "Error: Stop/line already exists.";
const std::string LINE_ERROR = "Error: Line could not be found.";
const std::string STOP_ERROR = "Error: Stop could not be found.";
const std::string INPUT_ERROR = "Error: Invalid input.";
const std::string LINE_ADDED = "Line was added.";
const std::string STOP_ADDED = "Stop was added.";
const std::string STOP_REMOVED = "Stop was removed from all lines.";
const std::string INVALID_FORMAT = "Error: Invalid format in file.";

using DataStruct = std::map<std::string, std::map<double,std::string>>;

// Lukee tiedot rivi kerrallaan tiedostosta ja palauttaa ne vektorina
bool read_file(std::vector<std::string>& lines, const std::string& filename)
{
    std::ifstream reader;
    reader.open(filename);

    if (!reader)
    {
        std::cout << READ_ERROR << std::endl;
        return false;
    }
    else
    {
        std::string line;

        while (std::getline(reader, line))
        {
            lines.push_back(line);
        }
    }

    reader.close();
    return true;
}

// Tarkistaa löytyykö linja jo tietorakenteesta.
bool find_line(DataStruct& stops_by_line,
               const std::string& line)
{
    if (stops_by_line.find(line) != stops_by_line.end())
    {
        return true;
    }

    return false;
}

// Tarkistaa löytyykö yhdistelmä (linja, pysäkki) jo tietorakenteesta.
bool find_stop(DataStruct& stops_by_line,
               const std::string& line,
               const std::string& stop)
{
    for (auto& lines: stops_by_line)
    {
        for(auto& stops: lines.second)
        {
            if(lines.first == line && stops.second == stop)
            {
                return true;
            }
        }
    }

    return false;
}

// Tarkistaa löytyykö pysäkki jo tietorakenteesta.
bool find_stop(DataStruct& stops_by_line,
               const std::string& stop)
{
    for (auto& lines: stops_by_line)
    {
        for(auto& stops: lines.second)
        {
            if(stops.second == stop)
            {
                return true;
            }
        }
    }

    return false;
}

// Tarkistaa löytyykö yhdistelmä (linja,pysäkki,etäisyys) jo tietorakenteesta.
bool find_distance(DataStruct& stops_by_line,
                   const std::string& line,
                   const std::string& stop,
                   const double distance)
{
    for (auto& lines: stops_by_line)
    {
        for(auto& stops: lines.second)
        {
            if(lines.first == line && stops.second == stop && stops.first == distance)
            {
                return true;
            }
        }
    }

    return false;
}


// Lisää uuden linjan, jollei sitä jo ole
bool add_line(DataStruct& stops_by_line,
              const std::string& line_name)
{
    if(stops_by_line.count(line_name) == 0)
    {
        stops_by_line[line_name] = {};
        return true;
    }

    return false;
}

// Lisää uuden pysäkin, jollei sitä jo ole
bool add_stop(DataStruct& stops_by_line,
              const std::string& line_name,
              const std::string& stop_name,
              const double distance)
{
    if(!find_line(stops_by_line, line_name))
    {
        add_line(stops_by_line,line_name);
    }

    if(!find_distance(stops_by_line,line_name, stop_name, distance))
    {
        stops_by_line[line_name][distance] = stop_name;
        return true;
    }

    return false;
}

// Lukee tiedot rivi kerrallaan, katkoo rivin tiedot välimerkien kohdalta
// ja palauttaa ne vektorina.
std::vector<std::string> split_file(const std::string& text,
                                    char separator = ' ',
                                    bool ignore_empty = false)
{
    std::vector<std::string> parts;
    std::string::size_type left = 0;
    std::string::size_type right = text.find(separator,left);

    while(right != std::string::npos)
    {
        std::string part = text.substr(left, right - left);

        if(!part.empty() || !ignore_empty) // if(part != "" || !ignore_empty)
        {
            parts.push_back(part);
        }

        left = right + 1;
        right = text.find(separator,left);
    }

    parts.push_back(text.substr(left, right - left));
    return parts;

}
// Tarkistaa syötteen oikeellisuuden
bool check_input(std::vector<std::string> input_line)
{
    if( input_line.at(0).empty() || input_line.at(1).empty() ||
        input_line.size()  < 2 || input_line.size() > 3)
    {
        std::cout << INVALID_FORMAT << std::endl;
        return false;
    }

    return true;
}

// Lukee tiedot rivi kerrallaan, jakaa tiedot ';'- merkistä
// ja tallentaa ne tietorakenteeseen.
bool parse_file(DataStruct& stops_by_line,
                const std::vector<std::string>& lines)
{
    for (const std::string& line:lines)
    {
        std::vector<std::string> fields = split_file(line,';');

        if(!check_input(fields))
        {
            return false;
        }

        std::string line_name = fields.at(0);
        std::string stop_name = fields.at(1);
        double distance = 0.0;

        if((fields.size() == 3 && !fields.at(2).empty()))
        {
            distance = std::stod(fields.at(2));
        }

        add_line(stops_by_line,line_name);
        add_stop(stops_by_line, line_name, stop_name, distance);
    }

    return true;
}

// Lukee sanat vektorista, yhdistää hipsulliset(")
// ja palauttaa ne vektorina.
std::vector<std::string> combine_words(std::vector<std::string>& splitted_text)
{
    std::vector<std::string> results;
    std::string combined_word = "";

    for(auto& word:splitted_text)
    {
        if(word.front() == '"' && word.back() == '"')
        {
            results.push_back(word.substr(1, word.length()-2));
        }

        else if(word.front() == '"')
        {
            word.erase(word.begin());
            combined_word += word;
        }
        else if(word.back() == '"')
        {
            word.pop_back();
            combined_word += " " + word;
            results.push_back(combined_word);
            combined_word.clear();
        }
        else
        {
            results.push_back(word);
        }
    }
    return results;
}

