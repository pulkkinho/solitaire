#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include "deck.hh"
#include "opendeck.hh"
#include "gamerules.hh"

#include "cardslot.hh"
#include <vector>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:
    void setupLayout();

    //Vectori cardsloteille
    std::vector<CardSlot*> cardslots;

    Deck* deck_;
    OpenDeck* pickedCards_;
};

#endif // MAINWINDOW_HH
