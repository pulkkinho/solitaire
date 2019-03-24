#include "cardslot.hh"

#include <iostream>
#include <QDrag>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QPainter>


CardSlot::CardSlot(CheckFunction function, QWidget *parent, bool top):
    QFrame(parent), topCard_(nullptr), checkFunction_(function), top(top)
{
    // Tällä sallitaan asioiden tiputtaminen tähän widgettiin.
    setAcceptDrops(true);

    setMinimumSize(180, 260);
    setMaximumWidth(180);
    setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);

}

// Kortin lisääminen cardslotiin.
void CardSlot::addCard(Card *card)
{
    if (topCard_ == nullptr){
        card->setParent(this);
    }
    else {
        card->setParent(topCard_);
        topCard_->stackCard(card);
    }
    topCard_= card;
}

// Suoritetaan, kun jotakin raahataan tämän CardSlotin päälle.
void CardSlot::dragEnterEvent(QDragEnterEvent *event)
{
    // Jos haluat testailla ohjelmaa niin, että näet konsolista, koska tämä
    // suoritetaan, poista kommenttimerkit seuraavalta riviltä.
    //std::cout << "CardSlot::dragEnterEvent" << std::endl;

    if (event->mimeData()->hasFormat("text/plain")) {
        event->setDropAction(Qt::MoveAction);
        event->accept();

    } else {
        event->ignore();
    }
}

// Suoritetaan, kun jotakin liikutetaan tämän CardSlotin rajojen sisällä.
void CardSlot::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain")) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        event->ignore();
    }
}

// Suoritetaan, kun jotain pudotetaan cardslotiin (raahauksen päättyessä).
void CardSlot::dropEvent(QDropEvent *event)
{
    // Tarkastetaan, onko sisältääkö pudotettu elementti tekstiä (korttidataa) ja
    // onko raahauksen aloituspaikka eri kuin pudotuspaikka.
    if (event->mimeData()->hasFormat("text/plain") && event->source() != this ) {

        // Cardslotilla on funktio-osoitin checkFunction_, jonka osoittama funktio
        // tarkastaa, voiko pelisääntöjen mukaan pudotettavan/t kortin/t pudottaa slotiin.
        // Ko. funktiolle annetaan parametrina kaksi stringiä, joista ensimmäinen
        // sisältää korttipaikassa päälimmäisenä olevan kortin tiedot ja toinen
        // lisättävistä korteista sen tiedot, joka olisi tulossa päälimmäisenä
        // olevan kortin päälle.

        // Pyydetään eventiltä tiedot siitä, mitä kortteja ollaan pudottamassa.
        QStringList newCardsData = event->mimeData()->text().split(";");

        // Muodostetaan QStringList-olio, johon tulevat tiedot slotissa olevista korteista.
        QStringList existingCardsData;
        if (topCard_ != nullptr){
            existingCardsData = QString::fromStdString(topCard_->getCardData()).split(";");
        } else {
            existingCardsData.append("");
        }

        // Tarkastetaan checkFunction_ -osoittimen päässä olevaa funktiota käyttäen, onko
        // pudotus sallittu pelisääntöjen mukaan.
        if(checkFunction_(existingCardsData.back().toStdString(), newCardsData.back().toStdString())){
            // Jos pudotus halutaan hyväksyä, lisätään raahatut kortit tähän slotiin.
            std::list<Card*> newCards;

            parseNewCards(newCards, newCardsData);
            if(newCards.size() != 0){
                setupNewCards(newCards);
                event->acceptProposedAction();
                }


            }



     else {
        event->ignore();
    }updateSlots();

}}

// Suoritetaan, kun cardslotia klikataan (eli raahauksen alkaessa).
void CardSlot::mousePressEvent(QMouseEvent *event)
{
    // 1) Metodin alkuosa suoritetaan, kun cardslotia klikataan.

    // Tämä estää ohjelmaa kaatumasta, kun tyhjää widgettiä klikataan.
    if (!childAt(event->pos()) || topCard_ == nullptr){
        return;
    }

    // Otetaan osoittimen päähän se kortti, jonka kohdalla klikkaus-event suoritettiin.
    Card* card = dynamic_cast<Card*>(childAt(event->pos())->parent());
    if (!card){
        return;  // Jos klikkauskohdassa ei ole korttia, lopetetaan.
    }
    if (!card->isOpen()){
        return;  // Jos kyseinen kortti on suljettuna, ei klikatessa tehdä mitään.
    }

    // Talletetaan muuttujaan kortin kuva.
    QPixmap pixmap = *card->getCurrentSideLabel()->pixmap();

    // Muodostetaan kortista QMimeData-olio, jonka sisällöksi laitetaan kortin
    // tekstimuotoinen esitys.
    QMimeData *mimeData = new QMimeData;
    mimeData->setText(QString::fromStdString(card->getCardData()));

    // Muodostetaan uusi raahaus-event, johon laitetaan edellä tallennetut tiedot.
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap);

    // Tässä määritellään miten kortista napataan kiinni.
    drag->setHotSpot(QPoint(pixmap.width()/2, pixmap.height()/2));
    // Tämä ei ole olennaisinta sisältöä, mutta jos haluat tutkia hotspotin ideaa,
    // voit kokeilla korvata edellisen rivin jollakin seuraavista:
    // drag->setHotSpot();
    // drag->setHotSpot(event->pos());
    // drag->setHotSpot(event->pos() - card->pos());

    // Asetetaan harmautettu korttikuva väliaikaiseksi korttikuvaksi raahaamisen ajaksi.
    QPixmap tempPixmap = pixmap;
    QPainter painter;
    painter.begin(&tempPixmap);
    painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
    painter.end();
    card->getCurrentSideLabel()->setPixmap(tempPixmap);

    // 2) Aloitetaan raahaus ja tarkastellaa onnistuiko.

    if (drag->exec( Qt::MoveAction) == Qt::MoveAction) {
        // Tämä suoritetaan, jos raahaus onnistui.
        card->getCurrentSideLabel()->setPixmap(pixmap);

        if (card->parent() == this){
            // Jos poistettu kortti oli pohjimmainen
            topCard_ = nullptr;
        } else {
            // Jos korttipaikkaan jäi vielä kortteja, poistetaan poistuneet kortit
            topCard_ = static_cast<Card*>(card->parent());
            topCard_->removeStackedCards();
        }
        // Poistetaan kortti ja sen päällä mahdollisesti olleet kortit.
        card->setParent(nullptr);
        card->setAttribute(Qt::WA_DeleteOnClose);
        card->close();
    } else {
        // Tämä suoritetaan, jos raahaus epäonnistui.
        card->getCurrentSideLabel()->setPixmap(pixmap);
        card->show();
    }
    updateSlots();
}

// Apumetodi, jota CardSlot::dropEvent kutsuu korttidatan jäsentämiseksi.
void CardSlot::parseNewCards(std::list<Card *> &newCards, QStringList &newCardsData)
{
    // Jäsennetään korttien data ja luodaan sitä vastaavat uudet kortit.
    for (QString cardData: newCardsData){
        QStringList splitted =  cardData.remove(";").split(",");
        if(splitted.isEmpty()){
            break;
        }
        //Jos yritetään asettaa useampi kuin yksi kortti (eli pino)
        //ruudun ylälaidan korttipaikkaan, lopetetaan
        if(newCardsData.size() > 1 && top == true){
                break;
            }

        QString value = splitted.at(0);
        QString suit = splitted.at(1);
        QString open = splitted.at(2);
        Card* card = new Card(static_cast<CardSuit> (suit.toUInt()), value.toUInt(), this);
        newCards.push_front(card);
        card->show();
        if (open.toInt()){
            card->turn();
        }
    }
}

// Apumetodi, jota CardSlot::dropEvent kutsuu uusien korttien asettelemiseksi.
void CardSlot::setupNewCards(std::list<Card *> &newCards)
{

    // Asetetaan kortteja yläriviin, kun siellä on jo kortteja
    if (topCard_ != nullptr && top == true){
        (newCards.front())->setParent(topCard_);
        topCard_ = newCards.front();
        topCard_->show();
    }

    // Asetellaan uudet, luodut kortit sen mukaan, mitä kortteja korttipaikalla oli jo.
    // Jos lisätään vain yksi kortti tyhjään paikkaan
    if (topCard_ == nullptr && newCards.size() == 1) {
        topCard_ = newCards.front();
    }

    // Jos lisätään useampia kortteja tyhjään paikkaan
    else if(topCard_ == nullptr && newCards.size() > 1 && top == false) {
        topCard_ = newCards.front();
        newCards.pop_front();
        for (auto card: newCards){
            topCard_->stackCard(card);
            topCard_  = card;
            card->show();
        }
    }

    // Jos lisätään kortteja paikkaan, jossa on jo kortteja.
    else if(topCard_ != nullptr && top == false){
        for (auto card: newCards){
            topCard_->stackCard(card);
            topCard_  = card;
            card->show();
        }
    }
}



//Päivittää sallitut kortit pelisääntöjen mukaisiksi
//jokaisen kortin pudotuksen jälkeen.
void CardSlot::updateSlots(){

    if(top == true){
        if(topCard_ == nullptr){
            checkFunction_ = &GameRules::checkIfAce;
        }
        else
            checkFunction_ = &GameRules::checkIfHigher;
    }


    else{
        if(topCard_ == nullptr){
         checkFunction_ = &GameRules::checkIfKing;
        }
        else if(topCard_->suit_ == DIAMOND){
                checkFunction_ = &GameRules::checkIfBlack;
            }
        else if(topCard_->suit_ == HEART){
                    checkFunction_ = &GameRules::checkIfBlack;
            }
        else
            checkFunction_ = &GameRules::checkIfRed;
        }
    if(topCard_ != nullptr){
        topCard_->allowOpen();
    }

}

