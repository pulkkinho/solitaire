#include <QPainter>
#include <QMouseEvent>
#include <QSizePolicy>
#include <vector>
#include "card.hh"

unsigned const FRONT_INDEX = 1;
unsigned const BACK_INDEX = 0;
unsigned const CARD_WIDTH = 177;
unsigned const CARD_HEIGHT = 249;
unsigned const DEFAULT_STACK_OFFSET = 35;
//pelikorttien merkit
std::vector<std::string> values = {"A", "2", "3", "4", "5", "6","7", "8", "9", "10", "J", "Q", "K"};
QColor const CARD_FRONT_COLOR = QColor(250,239,168,255);
QSize const SUIT_PIXMAP_SIZE = QSize(30,30);

Card::Card(CardSuit suit, unsigned value, QWidget *parent) :
    QFrame(parent),
    suit_(suit),
    value_(value),
    open_(false),
    canOpen_(false),
    backPixmap_(":/cards/card-back-177x249.png"),
    stackedCard_(nullptr)
{
    setBaseSize(CARD_WIDTH, CARD_HEIGHT);
    setMinimumSize(CARD_WIDTH, CARD_HEIGHT);
    setMaximumWidth(CARD_WIDTH);
    QSizePolicy policy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    setSizePolicy(policy);
    setupLayout();

}

bool Card::isOpen()
{
    return open_;
}

void Card::allowOpen()
{
    canOpen_ = true;
}

void Card::stackCard(Card *card)
{
    card->setParent(this);
    card->move(0, DEFAULT_STACK_OFFSET);

    stackedCard_ = card;
    connect(stackedCard_, &Card::resized, this, &Card::expand);
    expand();

}

void Card::removeStackedCards()
{
    stackedCard_ = nullptr;
}

std::string Card::getCardData()
{
    std::string rVal;
    if(stackedCard_ != nullptr) {
        rVal += stackedCard_->getCardData() + ";";
    }
    rVal += std::to_string(value_) +
            "," +
            std::to_string(suit_) +
            "," +
            std::to_string(open_)
            ;
    return rVal;
}

QLabel* Card::getCurrentSideLabel()
{
    if (open_){
        return findChild<QLabel*>(QString ("front"));
    } else {
        return findChild<QLabel*>(QString ("back"));
    }
}

// Käännetään kortti ympäri.
void Card::turn()
{
    open_ = !open_;
    if (open_){
        findChild<QLabel*>(QString ("front"))->show();
        findChild<QLabel*>(QString ("back"))->hide();
        canOpen_ = false;
    }

}

// Kasvatetaan widgetin piirtoalueen kokoa.
void Card::expand()
{
    setGeometry(x(), y(), width(), height() + DEFAULT_STACK_OFFSET);
    emit resized();
}

// Suoritetaan, kun widgettiä kaksoisklikataan.
void Card::mouseDoubleClickEvent(QMouseEvent *event)
{

    //Kortti käännetään vain jos se voidaan avata
    //ja se ei ole vielä auki.
    if(canOpen_ && isOpen() == false){
        turn();
        event->accept();
    }
    else {
        event->ignore();
    }
}

void Card::setupLayout()
{
    setSuitPixmap();
    setupBack();
    setupFront();
}

// Asettaa kortin attribuuttiin oikean maakuvan, että sitä voidaan käyttää piirtämisessä.
void Card::setSuitPixmap()
{
    switch (suit_) {
    case SPADE:
        suitPixmap_ = QPixmap(":/cards/spade-300px.png").scaled(SUIT_PIXMAP_SIZE, Qt::KeepAspectRatio);
        break;
    case CLUB:
        suitPixmap_ = QPixmap(":/cards/club-300px.png").scaled(SUIT_PIXMAP_SIZE, Qt::KeepAspectRatio);
        break;
    case DIAMOND:
        suitPixmap_ = QPixmap(":/cards/diamond-300px.png").scaled(SUIT_PIXMAP_SIZE, Qt::KeepAspectRatio);
        break;
    case HEART:
        suitPixmap_ = QPixmap(":/cards/heart-300px.png").scaled(SUIT_PIXMAP_SIZE, Qt::KeepAspectRatio);
        break;
    default:
        break;
    }
}

// Piirtääkortin takapuolen widgettiin.
void Card::setupBack()
{
    // Muodostetaan uusi QLabel-olio, jonka sisällöksi laitetaan kortiolion
    // attribuuttiin talletettu QPixmap-olio ja näytetään label widgetissä.
    QLabel* backImage = new QLabel(this);
    backImage->setObjectName("back");
    backImage->setPixmap(backPixmap_);
    backImage->show();
}

// Piirtää kortin etupuolen widgettiin.
void Card::setupFront()
{
    // Muodostetaan uusi QLabel-olio.
    QLabel* frontImage = new QLabel(this);
    frontImage->setObjectName("front");

    // Muodostetaan uusi QPixmap-olio, johon aletaan piirtää kortin etupuolta.
    QPixmap frontPixmap(backPixmap_.size());
    frontPixmap.fill(Qt::transparent);
    QPainter painter (&frontPixmap);

    // Piirretään kortin pohja, eli kulmista pyöristetty suorakaide.
    QPainterPath path;
    path.addRoundedRect(QRectF(1, 1, frontPixmap.width()-1, frontPixmap.height()-2),
                        15, 15, Qt::RelativeSize);
    painter.fillPath(path, CARD_FRONT_COLOR);
    painter.drawPath(path);

    // Piirretään kortin arvo ja maan kuvio.
    painter.setPen(QPen(Qt::black));
    QFont font = painter.font();
    font.setPixelSize(suitPixmap_.height());
    painter.setFont(font);

    //Määritetään oikea merkki (2-10, A, J, Q, K)
    std::string value = values[value_-1];

    //Maa ja lukuarvo piirretään kortin ala- ja ylälaitoihin.
    painter.drawPixmap(frontPixmap.width()/2-suitPixmap_.width(), 5,
                       suitPixmap_, 0, 0, 0, 0);
    painter.drawText(frontPixmap.width()/2, 0, suitPixmap_.width() + 5,
                     suitPixmap_.height() + 5, Qt::AlignCenter, QString::fromStdString(value));
    painter.drawPixmap(frontPixmap.width()/2-suitPixmap_.width(), 210,
                       suitPixmap_, 0, 0, 0, 0);
    painter.drawText(frontPixmap.width()/2, 210, suitPixmap_.width() + 5,
                     suitPixmap_.height() + 5, Qt::AlignCenter, QString::fromStdString(value));

    // Asetetaan piirretty kuva näkyviin widgetissä.
    frontImage->setPixmap(frontPixmap);
    frontImage->hide();
}
