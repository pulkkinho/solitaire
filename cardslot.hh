#ifndef CARDSLOT_HH
#define CARDSLOT_HH

#include "gamerules.hh"
#include "card.hh"

#include <list>
#include <string>
#include <QFrame>

QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDropEvent;
QT_END_NAMESPACE

using CheckFunction = bool(*)(std::string, std::string);

class CardSlot: public QFrame
{
public:
    // Rakentaja ja metodi, jolla slotiin voidaan lisätä kortti.
    CardSlot(CheckFunction function, QWidget* parent = 0, bool top = false);
    void addCard(Card* card);
    Card* topCard_;
    CheckFunction checkFunction_;
    //Onko cardslot ruudun ylä- vai alaosassa,
    //tätä tarvitaan oikean säännön määrittämiseen
    bool top = false;
    void updateSlots();

    // Itse toteutetut Qt:n widgetin raahaukseen liittyvät metodit.
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    // Apumetodeita dropEventin käyttöön.
    void parseNewCards(std::list<Card*>& newCards, QStringList& newCardsData);
    void setupNewCards(std::list<Card*>& newCards);





};

#endif // CARDSLOT_HH





