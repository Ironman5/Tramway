/* *
  * Ohjelman kirjoittaja
 * Nimi: Mika Valtonen
 * Opiskelijanumero: 166364
 * Käyttäjätunnus: valtone6
 * E-Mail: mika.valtonen@tuni.fi
* */

#ifndef HELPER_FUNCTIONS_HH
#define HELPER_FUNCTIONS_HH

#include "helper_functions.cpp"

bool read_file();
bool find_line();
bool find_stop();
bool find_distance();
bool add_line();
bool add_stop();
bool check_input();
bool parse_file();

std::vector<std::string> split_file();
std::vector<std::string> combine_words();


#endif // HELPER_FUNCTIONS_HH
