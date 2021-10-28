/* Rasse 2
 *
 * Kuvaus:
 * Ohjelma lukee Tampereen ratikkalinjoihin liittyviä tietoja
 * (linjan nimi, pysäkin nimi ja pysäkin etäisyys päätepysäkistä) tiedostosta,
 * tallentaa ne map -tietorakenteeseen ja antaa käyttäjälle mahdollisuuden
 * tehdä hakuja, lisäyksiä ja poistoja tietorakenteeseen.
 *
 * Ohjelman kirjoittaja
 * Nimi: Mika Valtonen
 * Opiskelijanumero: 166364
 * Käyttäjätunnus: valtone6
 * E-Mail: mika.valtonen@tuni.fi

* */

#include <helper_functions.hh>

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <utility>

using DataStruct = std::map<std::string, std::map<double,std::string>>;

void menu(DataStruct& stops_by_line);

// The most magnificent function in this whole program.
// Prints a RASSE
void print_rasse()
{
    std::cout <<
                 "=====//==================//===\n"
                 "  __<<__________________<<__   \n"
                 " | ____ ____ ____ ____ ____ |  \n"
                 " | |  | |  | |  | |  | |  | |  \n"
                 " |_|__|_|__|_|__|_|__|_|__|_|  \n"
                 ".|                  RASSE   |. \n"
                 ":|__________________________|: \n"
                 "___(o)(o)___(o)(o)___(o)(o)____\n"
                 "-------------------------------" << std::endl;
}



int main()
{
    print_rasse();

    // Lue tiedosto
    std::string filename;
    std::cout << "Give a name for input file: ";
    std::getline(std::cin, filename);
    std::vector<std::string> lines;

    if(!read_file(lines, filename))
    {
        return EXIT_FAILURE;
    }

    // Käytettävä tietorakenne:
    // map<linjan nimi <etäisyys lähtöpisteestä, pysäkin nimi>>
    DataStruct stops_by_line;

    // Tietojen muokkaus
    if(!parse_file(stops_by_line, lines))
    {
        return EXIT_FAILURE;
    }

    // Päävalikko
    menu(stops_by_line);

    return EXIT_SUCCESS;

}


// Ohjelman päävalikko
void menu(DataStruct& stops_by_line)
{
    while(true)
    {
        std::string input_text = "";
        std::string cmd = "";
        std::string param = "";
        std::string line = "";
        std::string start = "";
        std::string stop = "";
        double distance = 0;

        std::cout << "tramway> ";
        std:: getline(std::cin,input_text);

        // Splittaa rivit ja yhdistää ne tarvittaessa
        std::vector<std::string> splitted_text = split_file(input_text);
        std::vector<std::string> params = combine_words(splitted_text);

        cmd = params.at(0);

        // Muuttaa sanan kirjaimet suurakkosiksi
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);

        // Lopettaa ohjelman
        if(cmd == "QUIT")
        {
            break;
        }
        // Tulostaa linjat
        else if(cmd == "LINES")
        {
            std::cout << "All tramlines in alphabetical order:" << std::endl;

            for (auto& x: stops_by_line) {
                std::cout <<  x.first << std::endl;
            }
        }
        // Tulostaa linjan pysäkit
        else if(cmd == "LINE")
        {
            if(params.size() != 2)
            {
                std::cout << INPUT_ERROR << std::endl;
                continue;
            }

            line = params.at(1);

            if(find_line(stops_by_line,line))
            {
                std::cout << "Line " << line
                          << " goes through these stops in the order they are listed:"
                          << std::endl;

                for (auto& lines: stops_by_line[line]) {
                    std::cout << "- " << lines.second
                              << " : " << lines.first << std::endl;
                }
            }
            else
            {
                std::cout << LINE_ERROR << std::endl;
            }

        }
        // Tulostaa pysäkit
        else if(cmd == "STOPS")
        {
            std::set<std::string> stops;

            std::cout << "All stops in alphabetical order:" << std::endl;

            for (auto& lines: stops_by_line) {
                for(auto& stop: lines.second)
                {
                    stops.insert(stop.second);
                }
            }

            for(auto& stop:stops){
                std::cout << stop << std::endl;
            }
        }
        // Tulostaa linjat, jolta pysäkki löytyy
        else if(cmd == "STOP")
        {
            if(params.size() != 2)
            {
                std::cout << INPUT_ERROR << std::endl;
                continue;
            }

            stop = params.at(1);

            if(find_stop(stops_by_line, stop))
            {
                std::cout << "Stop " << stop
                          << " can be found on the following lines:"
                          << std::endl;

                for (auto& lines: stops_by_line) {
                    {
                        for(auto& stops: lines.second)
                        {
                            if(stops.second == stop)
                            {
                                std::cout << "- "<< lines.first << std::endl;
                            }
                        }
                    }
                }
            }
            else
            {
                std::cout << STOP_ERROR << std::endl;
            }

        }
        // Tulostaa pysäkkien välisen etäisyyden
        else if(cmd == "DISTANCE")
        {
            if(params.size() != 4)
            {
                std::cout << INPUT_ERROR << std::endl;
                continue;
            }

            line = params.at(1);
            start = params.at(2);
            stop = params.at(3);
            std::pair<std::string, double> start_stop;
            std::pair<std::string, double> end_stop;

            if((find_stop(stops_by_line, line, start) &&
                find_stop(stops_by_line, line, stop)))
            {
                for (auto& lines: stops_by_line[line])
                {
                    if(lines.second == start)
                    {
                        start_stop = make_pair(lines.second, lines.first);
                    }
                    if(lines.second == stop)
                    {
                        end_stop = make_pair(lines.second, lines.first);
                    }
                }

                std::cout << "Distance between "<< start_stop.first
                          << " and " << end_stop.first << " is "
                          << std::abs((start_stop.second - end_stop.second))
                          << std::endl;
            }
        }
        // Lisää uuden linjan
        else if(cmd == "ADDLINE")
        {
            if(params.size() != 2)
            {
                std::cout << INPUT_ERROR << std::endl;
                continue;
            }

            line = params.at(1);

            if(find_line(stops_by_line, line))
            {
                std::cout << EXIST_ERROR << std::endl;
            }
            else
            {
                add_line(stops_by_line, line);
                std::cout << LINE_ADDED << std::endl;
            }
        }
        // Lisää uuden pysäkin
        else if(cmd == "ADDSTOP")
        {
            if(params.size() != 4)
            {
                std::cout << INPUT_ERROR << std::endl;
                continue;
            }

            line = params.at(1);
            start = params.at(2);
            distance = std::stod(params.at(3));

            if(find_distance(stops_by_line, line, start, distance))
            {
                std::cout << EXIST_ERROR << std::endl;
            }
            else
            {
                add_stop(stops_by_line, line, start, distance);
                std::cout << STOP_ADDED << std::endl;
            }

        }
        // Poistaa pysäkin
        else if(cmd == "REMOVE")
        {
            if(params.size() != 2)
            {
                std::cout << INPUT_ERROR << std::endl;
                continue;
            }

            stop = params.at(1);
            bool removed = false;

            if(!find_stop(stops_by_line, stop))
            {
                std::cout << STOP_ERROR << std::endl;
                continue;
            }

            while(find_stop(stops_by_line, stop))
            {
                for (auto& lines: stops_by_line) {
                    {
                        for(auto& stops: lines.second)
                        {
                            if(stops.second == stop &&
                                    lines.second.erase(stops.first))
                            {
                                removed = true;
                                break;
                            }
                        }
                    }
                }
            }

            if(removed)
            {
                std::cout << STOP_REMOVED << std::endl;
            }
        }

        else
        {
            std::cout << INPUT_ERROR << std::endl;
        }
    }
}
