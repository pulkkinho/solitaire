#include "gamerules.hh"
#include "card.hh"

// Funktio saa parametrinaan alla olevan kortin ja päälle tulevan kortin
// tiedot merkkijonomuodossa.
bool GameRules::checkIfHeart(std::string bottom, std::string top)
{
    // Tämä funktio ei tarkastele, mitä alle on jo laitettu, vaan pelkästään
    // sitä, mitä päälle ollaan laittamassa. Tässä kerrotaan kääntäjälle,
    // että ensimmäinen parametreista on tarkoituksella käyttämättä.
    Q_UNUSED(bottom);

    std::string suitString = top.substr(top.find_first_of(",")+1,1);
    return stoi(suitString) == HEART;
}

bool GameRules::checkIfAce(std::string bottom, std::string top)
{
    // Tämä funktio ei tarkastele, mitä alle on jo laitettu, vaan pelkästään
    // sitä, mitä päälle ollaan laittamassa. Tässä kerrotaan kääntäjälle,
    // että ensimmäinen parametreista on tarkoituksella käyttämättä.
    Q_UNUSED(bottom);

    std::string valueString = top.substr(0, top.find_first_of(","));
    return stoi(valueString) == 1;
}

bool GameRules::checkIfKing(std::string bottom, std::string top)
{
    // Tämä funktio ei tarkastele, mitä alle on jo laitettu, vaan pelkästään
    // sitä, mitä päälle ollaan laittamassa. Tässä kerrotaan kääntäjälle,
    // että ensimmäinen parametreista on tarkoituksella käyttämättä.
    Q_UNUSED(bottom);

    std::string valueString = top.substr(0, top.find_first_of(","));
    return stoi(valueString) == 13;
}

bool GameRules::checkIfSpades(std::string bottom, std::string top)
{
    // Tämä funktio ei tarkastele, mitä alle on jo laitettu, vaan pelkästään
    // sitä, mitä päälle ollaan laittamassa. Tässä kerrotaan kääntäjälle,
    // että ensimmäinen parametreista on tarkoituksella käyttämättä.
    Q_UNUSED(bottom);

    std::string suitString = top.substr(top.find_first_of(",")+1,1);
    return stoi(suitString) == SPADE;
}

bool GameRules::checkIfDiamonds(std::string bottom, std::string top)
{
    // Tämä funktio ei tarkastele, mitä alle on jo laitettu, vaan pelkästään
    // sitä, mitä päälle ollaan laittamassa. Tässä kerrotaan kääntäjälle,
    // että ensimmäinen parametreista on tarkoituksella käyttämättä.
    Q_UNUSED(bottom);

    std::string suitString = top.substr(top.find_first_of(",")+1,1);
    return stoi(suitString) == DIAMOND;
}

bool GameRules::checkIfClubs(std::string bottom, std::string top)
{
    // Tämä funktio ei tarkastele, mitä alle on jo laitettu, vaan pelkästään
    // sitä, mitä päälle ollaan laittamassa. Tässä kerrotaan kääntäjälle,
    // että ensimmäinen parametreista on tarkoituksella käyttämättä.
    Q_UNUSED(bottom);

    std::string suitString = top.substr(top.find_first_of(",")+1,1);
    return stoi(suitString) == CLUB;
}


//tarkastetaan onko kortti punainen
bool GameRules::checkIfRed(std::string bottom, std::string top)
{

    std::string valueString = top.substr(0, top.find_first_of(","));
    std::string valueStringBottom = bottom.substr(0, bottom.find_first_of(","));
    std::string suitString = top.substr(top.find_first_of(",")+1,1);

    if(stoi(valueString) + 1 == stoi(valueStringBottom)){
        if(stoi(suitString) == DIAMOND){
            return true;
        }
        if(stoi(suitString) == HEART){
            return true;
        }
        else
            return false;
    }
    else
        return false;
}


//tarkastetaan onko kortti musta
bool GameRules::checkIfBlack(std::string bottom, std::string top)
{
    std::string valueString = top.substr(0, top.find_first_of(","));
    std::string valueStringBottom = bottom.substr(0, bottom.find_first_of(","));
    std::string suitString = top.substr(top.find_first_of(",")+1,1);
    if(stoi(valueString) + 1 == stoi(valueStringBottom)){
        if(stoi(suitString) == DIAMOND){
            return false;
        }
        if(stoi(suitString) == HEART){
            return false;
    }
        else
            return true;
    }
    else
        return false;
}



//tarkastetaan onko kortti samaa maata ja yhtä isompi kuin alla oleva
bool GameRules::checkIfHigher(std::string bottom, std::string top)
{

    std::string suitString = top.substr(top.find_first_of(",")+1,1);
    std::string suitStringBottom = bottom.substr(bottom.find_first_of(",")+1,1);
    std::string valueString = top.substr(0, top.find_first_of(","));
    std::string valueStringBottom = bottom.substr(0, bottom.find_first_of(","));
    if((suitString) == (suitStringBottom)){
        if(stoi(valueString) == stoi(valueStringBottom) + 1){
            return true;
        }
        else
            return false;
    }
    else
        return false;
}
