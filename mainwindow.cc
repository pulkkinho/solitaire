#include <QFrame>
#include <QHBoxLayout>

#include "mainwindow.hh"
#include "cardslot.hh"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    setupLayout();
    connect(deck_, &Deck::cardPicked, pickedCards_, &OpenDeck::addCard);

}

MainWindow::~MainWindow()

{

}

void MainWindow::setupLayout()
{
    // Pääikkunan isoimpana rakenteena on frame, joka sisältää...
    QFrame* frame = new QFrame;
    QVBoxLayout* frameLayout = new QVBoxLayout(frame);

    // ... alarivin ja ylärivin.
    QHBoxLayout* bottomRowLayout = new QHBoxLayout();
    QHBoxLayout* topRowLayout = new QHBoxLayout();
    frameLayout->addLayout(topRowLayout);
    frameLayout->addLayout(bottomRowLayout);


  //  hitMeButton_.setObjectName("hitMeButton");

  //  topRowLayout->addWidget(&hitMeButton_);

    // Luodaan pakkaoliot.
    deck_ = new Deck(this);
    pickedCards_ = new OpenDeck(this);

    // Lisätään yläriville suljettu ja avoin pakka...
    topRowLayout->addWidget(deck_);
    topRowLayout->addWidget(pickedCards_);

    //Luodaan 7 cardslotia ruudun alaosaan ja lisätään
    //ne vectoriin
    for(int counter = 1; counter <= 7; ++counter){
        CardSlot* slot = (new CardSlot(&GameRules::checkIfHeart, this));
        cardslots.push_back(slot);
        slot->top = false;
        bottomRowLayout->addWidget(slot);
    }

    //Lisätään kortit pöydälle ylösalaisin,
    //päällimäinen kortti käännetään oikeinpäin.
    for(int rows = 0; rows <=6; ++rows){
        for(int rowcounter = rows; rowcounter <=6; ++rowcounter){

        (cardslots[6-rows])->addCard(deck_->pickCard());
        }
        (cardslots[6-rows])->topCard_->turn();
        (cardslots[6-rows])->updateSlots();
    }

    setCentralWidget(frame);

    //Luodaaan ylärivin cardslotit
    for(int rows = 0; rows <4; ++rows){
        CardSlot* slot = (new CardSlot(&GameRules::checkIfAce, this, true));
        cardslots.push_back(slot);
        slot->top = true;
        topRowLayout->addWidget(slot);
    }
}




