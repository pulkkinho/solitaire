#ifndef GAMERULES_HH
#define GAMERULES_HH
#include <string>

namespace GameRules {

    // Tarkastusfunktio, joka varmistaa, että päälle laitetaan vain herttaa.
    bool checkIfHeart(std::string bottom, std::string top);
    // Tarkastusfunktio, joka varmistaa, että päälle laitetaan ässä (1)
    bool checkIfAce(std::string bottom, std::string top);
    //Tarkastusfunktio, joka varmistaa, että päälle laitetaan vain ristiä
    bool checkIfClubs(std::string bottom, std::string top);
    // Tarkastusfunktio, joka varmistaa, että päälle laitetaan vain ruutua
    bool checkIfDiamonds(std::string bottom, std::string top);
    // Tarkastusfunktio, joka varmistaa, että päälle laitetaan vain pataa.
    bool checkIfSpades(std::string bottom, std::string top);
    // Tarkastusfunktio, joka varmistaa, että päälle laitetaan vain punaista.
    bool checkIfRed(std::string bottom, std::string top);
    // Tarkastusfunktio, joka varmistaa, että päälle laitetaan vain kuningas (13).
    bool checkIfKing(std::string bottom, std::string top);
    // Tarkastusfunktio, joka varmistaa, että päälle laitetaan samaa maata ja yhtä isompi
    bool checkIfHigher(std::string bottom, std::string top);
    // Tarkastusfunktio, joka varmistaa, että päälle laitetaan vain mustaa
    bool checkIfBlack(std::string bottom, std::string top);
}

#endif // GAMERULES_HH
