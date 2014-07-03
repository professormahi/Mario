#include "customizedview.h"

CustomizedView::CustomizedView(QWidget *parent) :
    QGraphicsView::QGraphicsView(parent)
{
    IsGameFinished =false;
    IsTimerRunning =false;
    IsMarioUp = false;
    HealthCount = 3;
    Score = 0;
    HealthInterval = 20;
    Scene=new QGraphicsScene(0,0,6000,this->height());
    this->setScene(Scene);
    QGraphicsRectItem* Land= Scene->addRect(0, this->height()+10, 6000,128);
    Land->setBrush(QBrush(QPixmap(":/new/PreFix/Soureces/baseWall.bmp")));

    Scene->setBackgroundBrush(QBrush(QPixmap(":/new/PreFix/Soureces/Sky.png")));

    Mario = Scene->addPixmap(QPixmap(":/new/PreFix/Soureces/Mario.png"));
    Mario->setPos(0, this->height()-55);

    srand(time(0));
    for (int var = 0; var < 10; ++var) {
        QGraphicsPixmapItem *Tmp = Scene->addPixmap(QPixmap(":/new/PreFix/Soureces/cloud.bmp"));
        Tmp->setPos((float)(rand()%10)/10*6000, rand()%30);
        Tmp->setZValue(-2);
    }

    for (int var = 0; var < 10; ++var) {
        QGraphicsPixmapItem *Tmp = Scene->addPixmap(QPixmap(":/new/PreFix/Soureces/hill.bmp"));
        Tmp->setPos((float)(rand()%10)/10*6000, this->height()-55);
        Tmp->setZValue(-2);
    }

    Castle = Scene->addPixmap(QPixmap(":/new/PreFix/Soureces/castle.bmp"));
    Castle->setPos(Scene->width()-260,this->height()-245);
    Castle->setZValue(-1);


    MakeMap("/home/mahdi/Documents/CPlusPlusCodes/MarioQGraphics/MakeMap.txt");
    QTimer::singleShot(10,this, SLOT(MovingEnemies()));
    CoinCollesions();
    CheckingTheSky();
}

int CustomizedView::ComputeY(int t)
{
    int g = 5;
    int v_0 = 6*g;
    int Max_t = v_0/g;
    if(t <= Max_t+1)
        return -g/2*t*t+v_0*t;
    else
        return -1*(-g/2*t*t+v_0*t);
}

void CustomizedView::MakeMap(QString MapAddress)
{
    fstream fin;
    fin.open(MapAddress.toStdString().c_str(), ios::in);
    while(fin.is_open())
    {
        int X;
        int Y;
        string Kind;
        fin>>X>>Y>>Kind;
        if(Kind == "Pipe")
        {
            QGraphicsPixmapItem *PipeTemp = new QGraphicsPixmapItem;
            PipeTemp->setPixmap(QPixmap(":/new/PreFix/Soureces/pipe.bmp"));
            PipeTemp->setPos(X,Y);
            Scene->addItem(PipeTemp);
            Pipes.append(PipeTemp);
        }
        else if(Kind == "Enemy")
        {
            QGraphicsPixmapItem *EnemyItem = new QGraphicsPixmapItem;
            EnemyItem->setPixmap(QPixmap(":/new/PreFix/Soureces/image.png"));
            EnemyItem->setPos(X,Y);
            Scene->addItem(EnemyItem);
            Enemies.append(EnemyItem);
            EnemiesFlags.append(true);
        }
        else if(Kind == "Coin")
        {
            QGraphicsPixmapItem *CoinItem = new QGraphicsPixmapItem;
            CoinItem->setPixmap(QPixmap(":/new/PreFix/Soureces/Coin.png"));
            CoinItem->setPos(X,Y);
            Scene->addItem(CoinItem);
            Coins.append(CoinItem);
        }
        else if(Kind == "Block")
        {
            QGraphicsPixmapItem *BlockItem = new QGraphicsPixmapItem;
            BlockItem->setPixmap(QPixmap(":/new/PreFix/Soureces/Block.bmp"));
            BlockItem->setPos(X,Y);
            Scene->addItem(BlockItem);
            Blocks.append(BlockItem);
        }
        else if(QString::fromStdString(Kind).toInt())
        {
            QGraphicsPixmapItem *CoinBoxItem = new QGraphicsPixmapItem;
            CoinBoxItem->setPixmap(QPixmap(":/new/PreFix/Soureces/CoinBox.png"));
            CoinBoxItem->setPos(X,Y);
            Scene->addItem(CoinBoxItem);
            CoinBoxes[CoinBoxItem]= QString::fromStdString(Kind).toInt();            
        }
        else
            break;
    }
}

bool CustomizedView::PipeCollisions()
{
    for (int i = 0; i < Pipes.length(); ++i) {
        if(Mario->collidesWithItem(Pipes[i]))
        {
            return true;
        }
    }
    return false;
}

bool CustomizedView::EnemyCollesions()
{
    for (int i = 0; i < Enemies.count(); ++i) {
        if(Mario->collidesWithItem(Enemies[i]))
        {
            return true;
        }
    }
    return false;
}

bool CustomizedView::CoinBoxCollesions()
{
    for (int i = CoinBoxes.count() -1 ; i >= 0 ; --i) {
        if(Mario->collidesWithItem(CoinBoxes.keys()[i]))
        {
            if(CoinBoxes.values()[i] != 0)
            {
                CoinBoxes[CoinBoxes.keys()[i]] = CoinBoxes.values()[i] - 1 ;
                CurrentCoin = Scene->addPixmap(QPixmap(":/new/PreFix/Soureces/Coin.png"));
                CurrentCoin->setPos(CoinBoxes.keys()[i]->x(),CoinBoxes.keys()[i]->y());
                QTimer::singleShot(20,this,SLOT(CoinCollectingFromBoxes()));
                if(CoinBoxes.values()[i] == 0)
                {
                    Scene->removeItem(CoinBoxes.keys()[i]);
                    CoinBoxes.remove(CoinBoxes.keys()[i]);
                }
                Score ++;
                return true;
            }
        }
    }
    return false;
}

bool CustomizedView::EnemyCollesionsFromUp()
{
    for (int i = Enemies.count() -1; i >= 0; --i) {
        if(Mario->collidesWithItem(Enemies[i]))
        {
            HealthCount ++;
            Score += 3;
            Scene->removeItem(Enemies[i]);
            Enemies.removeAt(i);
            return true;
        }
    }
    return false;
}

bool CustomizedView::BlockCollesions()
{
    for( int i=Blocks.count() -1; i>=0; --i)
    {
        if(Mario->collidesWithItem(Blocks[i]))
            return true;
    }
    return false;
}

void CustomizedView::CoinCollesions()
{
    for(int i= Coins.length()-1 ; i>=0 ;i--)
    {
        if(Mario->collidesWithItem(Coins[i]))
        {
            Scene->removeItem(Coins[i]);
            Coins.removeAt(i);
            Score++;
        }
    }
    QTimer::singleShot(10,this,SLOT(CoinCollesions()));
}

void CustomizedView::CoinCollectingFromBoxes()
{
    Scene->removeItem(CurrentCoin);
    delete CurrentCoin;
}

void CustomizedView::CheckingTheSky()
{
    if(!PipeCollisions() && !BlockCollesions())
    {
        if(!IsTimerRunning)
        {
            IsMarioUp = false;
//            Mario->setY(426);
            while(true)
            {
                if(!PipeCollisions())
                    break;
                if(!BlockCollesions() )
                    break;
                if(Mario->y() != 426)
                    break;
                if(Mario->y() != 425)
                    break;

                Mario->setY(Mario->y()+1);
            }
        }
    }
    QTimer::singleShot(10, this, SLOT(CheckingTheSky()));
}

void CustomizedView::keyPressEvent(QKeyEvent *event)
{
    if(!IsGameFinished)
    {
        int k = event->key();        
        if(k==16777236) // Rigth
        {
            if(!IsTimerRunning && PipeCollisions())
            {
                while (PipeCollisions())
                    Mario->setPos(Mario->x()-1, Mario->y());
                return;
            }
            Mario->setPixmap(QPixmap(":/new/PreFix/Soureces/Mario.png"));
            Mario->setPos(Mario->x()+10, Mario->y());
        }
        else if(k==16777234)//Left
        {
            if(!IsTimerRunning && PipeCollisions())
            {
                while (PipeCollisions())
                    Mario->setPos(Mario->x()+1, Mario->y());
                return;
            }
            Mario->setPixmap(QPixmap(":/new/PreFix/Soureces/MarioBack.png"));
            Mario->setPos(Mario->x()-10, Mario->y());
        }
        else if(k==16777235)//Up
        {
            IsTimerRunning = true;
            QTimer::singleShot(100,this,SLOT(TimerShot()));
        }
        IsGameFinished=Mario->collidesWithItem(Castle);
        if(!PipeCollisions() && !BlockCollesions())
        {
            if(!IsTimerRunning)
            {
                IsMarioUp = false;
                Mario->setY(426);
            }
        }
    }
    else
    {
        EndOfGame = Scene->addSimpleText("Hurray! You Won ;) And your score is:"+QString::number(Score)+",  "+QString::number(HealthCount)+" Healths left");
        EndOfGame->setFont(QFont("Arial",30,-1,true));
        EndOfGame->setBrush(QBrush(Qt::red));
        EndOfGame->setPos(Scene->width()-this->width()/2-EndOfGame->boundingRect().width()/2, Scene->height()/2);
        QTimer::singleShot(100, this,SLOT(EndTimer()));
    }
}

void CustomizedView::TimerShot()
{
    t++;
    int Y = ComputeY(t);        
    bool CoinBoxRes = CoinBoxCollesions();
    if(!PipeCollisions() || IsMarioUp)
    {
        Mario->setPos(Mario->x(),Mario->y()-4*Y/7);
    }

    if(Y<0 && PipeCollisions())
    {
        while(BlockCollesions())
            Mario->setY(Mario->y()-1);
        Mario->setY(Mario->y()-1);
        IsMarioUp = true;
        t =0;
        return;
    }

    if(Y>0 && PipeCollisions())
    {
        if(IsMarioUp)
        {
            IsMarioUp ==false;
            return;
        }
        Mario->setPos(Mario->x(), 426);
        t= 0;
        return ;
    }

    if(CoinBoxRes && Y>0 )
    {
        Mario->setPos(Mario->x(), 426);
        t= 0;
        return ;
    }

    if(Y<0)
    {
        EnemyCollesionsFromUp();
    }

    if(Y>0 && BlockCollesions())
    {
        Mario->setPos(Mario->x(), 426);
        t= 0;
        return ;
    }

    if(Y<0 && BlockCollesions())
    {
        while(BlockCollesions())
            Mario->setY(Mario->y()-1);
        Mario->setY(Mario->y()+1);
         t =0;
         return;
    }

    if(Y==0)
    {
        t= 0;
        IsTimerRunning = false;
        return;
    }
    QTimer::singleShot(100,this,SLOT(TimerShot()));
}

void CustomizedView::EndTimer()
{
    static int ToEnd=0;
    EndOfGame->setPen(QPen(QColor(255,0,0,ToEnd)));
    ToEnd++;
    if(ToEnd == 255)
        ToEnd=0;
    QTimer::singleShot(100, this,SLOT(EndTimer()));
}

void CustomizedView::MovingEnemies()
{
    HealthInterval++;
    if(EnemyCollesions() && HealthInterval >= 20)
    {
        HealthCount --;
        HealthInterval =0;
    }
    if(HealthInterval > 100)
    {
        HealthInterval =20;
    }
    if(HealthCount <= 0)
    {
        QMessageBox::information(this,"GameOver!","You have lose your 3 Healthes\nYour score is : "+QString::number(Score));
        exit(0);
    }
    for (int i = 0; i < Enemies.length(); ++i) {
        bool Collesion;
        for (int j = 0; j < Pipes.length(); ++j) {
            Collesion = Enemies[i]->collidesWithItem(Pipes[j]);
            if(Collesion)
                break;
        }
        if(Collesion)
            EnemiesFlags[i] = !EnemiesFlags[i];

        if(EnemiesFlags[i])
            Enemies[i]->setPos(Enemies[i]->x() - 10, Enemies[i]->y());
        else
            Enemies[i]->setPos(Enemies[i]->x() + 10, Enemies[i]->y());
    }
    QTimer::singleShot(50,this, SLOT(MovingEnemies()));
}
