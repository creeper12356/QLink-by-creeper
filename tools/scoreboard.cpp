#include "scoreboard.h"
#include "objects/role.h"

ScoreBoard::ScoreBoard(QWidget* parent, Role *p):
    QWidget(parent) ,
    displayer(this),
    idBoard(this)
{
    idBoard.setAlignment(Qt::AlignCenter);
    setPlayer(p);
}

ScoreBoard::~ScoreBoard()
{
    qDebug() << "~ScoreBoard";
}

ScoreBoard &ScoreBoard::operator=(const ScoreBoard &other)
{
    this->setPlayer(other._player);
    this->setScore(other.score());
    return *this;
}

ScoreBoard &ScoreBoard::operator+=(int aScore)
{
    this->setScore(score() + aScore);
    return *this;
}

ScoreBoard &ScoreBoard::operator-=(int mScore)
{
    this->setScore(score() - mScore);
    return *this;
}

void ScoreBoard::add(int aScore)
{
    (*this) += aScore;
}

void ScoreBoard::minus(int mScore)
{
    (*this) -= mScore;
}

int ScoreBoard::score() const
{
    return displayer.intValue();
}

const Role *ScoreBoard::player() const
{
    return _player;
}

void ScoreBoard::setPlayer(Role *p)
{
    _player = p;
    if(p == nullptr){
        this->hide();
        return ;
    }
    this->show();
    idBoard.setText(p->id);
}
void ScoreBoard::setScore(int s)
{
    displayer.display(s);
}
void ScoreBoard::paintEvent(QPaintEvent *event)
{
    //设置数字显示器大小
    displayer.setGeometry(0,height() * 0.25,width(),height() * 0.75);

    //显示玩家id
    idBoard.setGeometry(0,0,width(),height() * 0.25);
    QFont font("Consolas");
    font.setPixelSize(qMin(idBoard.width() / idBoard.text().length(),idBoard.height()));
    idBoard.setFont(font);
}

