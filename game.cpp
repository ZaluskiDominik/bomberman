#include "game.h"
#include <fstream>
#include <QMessageBox>
#include <algorithm>
#include <ctime>
#include "flame.h"

//every field is a square
//length of the side of the square
extern int fieldSize;

//list with undestroyable obstacles
QList<QGraphicsPixmapItem*> obstacles;

//list witch desroyable wooded chests
QList<QGraphicsPixmapItem*> chests;

//list with players in game
QList<player*> gamePlayers;

//lists with bombs placed currently at the scene
QList<bomb*> bombs;

game::game(QWidget* parent, const playerData *playersData, int numPlayers)
    :QDialog(parent, Qt::WindowCloseButtonHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint), numPowerups(15)
{
    //handle keybord events
    grabKeyboard();
    scene=nullptr;

    setWindowTitle("Bomberman");

    load_pixmaps();
    chest::load_pixmaps();

    //load the map to memory
    if (!load_map())
    {
        //an error occured
        parentWidget()->show();
        QMessageBox::warning(parentWidget(), "Error", "Could not open file map.txt", QMessageBox::Ok);
        deleteLater();
        return;
    }

    //draw graphicsView's frame made of brick
    draw_bricks();

    create_powerups();

    //draw players
    spawn_players(playersData, numPlayers);
}

game::~game()
{
    //delete players
    for (auto i=gamePlayers.begin() ; i!=gamePlayers.end() ; i++)
        (*i)->deleteLater();
    gamePlayers.clear();

    //clear all lists
    chests.clear();
    obstacles.clear();
    bombs.clear();

    chest::free_pixmaps();

    if (scene!=nullptr)
        scene->clear();
}

void game::setup_graphics()
{
    //create graphics scene
    scene=new QGraphicsScene(this);
    view=new QGraphicsView(scene, this);

    view->setGeometry(0, 0, width(), height());
    scene->setSceneRect(0, 0, view->width(), view->height());
    view->show();

    //set view's scroll bars off
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //set background image
    scene->setBackgroundBrush(QImage(":/images/img/fields/road.jpg"));
}

void game::load_pixmaps()
{
    fields[0].load(":/images/img/fields/obstacle.png");
    fields[1].load(":/images/img/fields/brick.jpg");
    fields[2].load(":/images/img/fields/chest.png");
}

QPixmap game::get_field_pixmap(fieldType name, int size)
{
    QPixmap pixmap;
    //choose pixmap based on name
    if (name==Obstacle)
        pixmap=fields[0];
    else if (name==Brick)
        pixmap=fields[1];
    else if (name==Chest)
        pixmap=fields[2];

    //scale pixmap
    return pixmap.scaled(size, size);
}

bool game::load_map()
{
    std::ifstream file("map.txt");

    if (!file.is_open())
        //error file isn't open
        return false;

    //read width and height(in number of fields)
    file>>w>>h;
    //two additional rows and columns for brick
    w+=2;
    h+=2;

    //set window resolution
    setFixedSize(fieldSize * w, fieldSize * h);

    //create graphics scene
    setup_graphics();

    //read fields and draw them
    draw_fields(file);

    //exit without errors
    return true;
}

void game::draw_fields(std::ifstream& file)
{
    //number determinating which field this is
    int field;
    for (int i=1 ; i<h-1 ; i++)
    {
        for (int j=1 ; j<w-1 ; j++)
        {
            file>>field;
            if (field==Obstacle)
            {
                obstacles.append(new QGraphicsPixmapItem(get_field_pixmap(Obstacle, fieldSize)));
                obstacles.back()->setPos(j * fieldSize, i * fieldSize);
                scene->addItem(obstacles.back());
            }
            else if (field==Chest)
            {
                chests.append(new chest);
                chests.back()->setPos(j * fieldSize, i * fieldSize);
                scene->addItem(chests.back());
            }
        }
    }
}

void game::draw_bricks()
{
    //vertical
    for (int i=0 ; i<h ; i++)
    {
        QGraphicsPixmapItem* item[2]={new QGraphicsPixmapItem(get_field_pixmap(Brick, fieldSize)), new QGraphicsPixmapItem(get_field_pixmap(Brick, fieldSize))};
        item[0]->setPos(0, i * fieldSize);
        scene->addItem(item[0]);
        item[1]->setPos(view->width() - fieldSize, i * fieldSize);
        scene->addItem(item[1]);
    }

    //horizontal
    for (int i=1 ; i<w-1 ; i++)
    {
        QGraphicsPixmapItem* item[2]={new QGraphicsPixmapItem(get_field_pixmap(Brick, fieldSize)), new QGraphicsPixmapItem(get_field_pixmap(Brick, fieldSize))};
        item[0]->setPos(i * fieldSize, 0);
        scene->addItem(item[0]);
        item[1]->setPos(i * fieldSize, view->height() - fieldSize);
        scene->addItem(item[1]);
    }
}

void game::create_powerups()
{
    QList<int> chestTaken;
    int randChest;
    for (int i=0 ; i<numPowerups ; i++)
    {
        randChest=rand() % chests.size();
        if (chestTaken.contains(randChest))
            i--;
        else
        {
            powerup* item=new powerup;
            if (i % powerup::numDiffPowers==0)
                item->set_powerType(powerup::powerupType::NewBomb);
            else if (i % powerup::numDiffPowers==1)
                item->set_powerType(powerup::powerupType::Boots);
            else if (i % powerup::numDiffPowers==2)
                item->set_powerType(powerup::powerupType::BiggerRange);

            item->setPos(chests[randChest]->pos());
            scene->addItem(item);
        }
    }
}

void game::spawn_players(const playerData* data, int numPlayers)
{
    player::movingDist=fieldSize/10;
    for (int i=0 ; i<numPlayers ; i++)
    {
        gamePlayers.append(new player(data[i], scene));
        QObject::connect(gamePlayers[i], SIGNAL(drawFlamesRequest(bomb*)), this, SLOT(onDrawFlameRequest(bomb*)));
    }
}

void game::create_flame_line(QPoint direction, const bomb& b)
{
    QPoint flamePos(b.x(), b.y());
    int numFlames=b.range;
    if (direction.x()==fieldSize)
    {
        //middle flame will be created
        numFlames++;
        flamePos-=direction;
    }

    for (int j=0 ; j<numFlames ; j++)
    {
        flamePos+=direction;

        flame* newFlame=new flame;
        newFlame->setPos(flamePos);
        scene->addItem(newFlame);

        auto collide=newFlame->collidingItems(Qt::IntersectsItemBoundingRect);
        //go through the list in reversed order
        for (auto i=collide.rbegin() ; i!=collide.rend() ; i++)
        {
            if (typeid(**i)==typeid(player))
                static_cast<player*>(*i)->explosion_hit();
            else if (is_obstacle(*i) || ( (*i)->x()==0 ) || ( (*i)->x()==width() - fieldSize ) || ( (*i)->y()==0) || ( (*i)->y()==height() - fieldSize ) )
            {
                //obstacle or brick
                //stop drawing flames, delete flame added at last
                newFlame->deleteLater();
                return;
            }
            else if (typeid(**i)==typeid(bomb))
            {

            }
            else if (typeid(**i)==typeid(chest))
            {
                //stop drawing flames, explode chest
                chest* c=static_cast<chest*>(*i);
                if (!c->inDestruction)
                {
                    chests.removeOne(c);
                    c->inDestruction=true;
                    c->explode();
                }
                newFlame->deleteLater();
                return;
            }
            else if (typeid(**i)==typeid(powerup))
            {
                powerup* item=static_cast<powerup*>(*i);
                if (item->is_pickable())
                    delete item;
                else
                    item->set_as_pickable();
            }
        }
    }
}

bool game::is_obstacle(const QGraphicsItem * const item)
{
    for (auto i =obstacles.begin() ; i!=obstacles.end() ; i++)
        if ((*i)==item)
            return true;
    return false;
}

bool game::is_chest(const QGraphicsItem * const item)
{
    for (auto i=chests.begin() ; i!=chests.end() ; i++)
        if ((*i)==item)
            return true;
    return false;
}

//KEY EVENTS
void game::keyPressEvent(QKeyEvent *e)
{
    //return if event for that key was repeated
    if (e->isAutoRepeat())
        return;

    //let each player handle pressing keys
    for (int i=0 ; i<gamePlayers.size() ; i++)
        gamePlayers[i]->key_pressed(e->key());
}

void game::keyReleaseEvent(QKeyEvent *e)
{
    if (e->isAutoRepeat())
        return;

    //let each player handle releasing keys
    for (int i=0 ; i<gamePlayers.size() ; i++)
        gamePlayers[i]->key_released(e->key());
}

void game::onDrawFlameRequest(bomb *b)
{
    //create flames
    create_flame_line(QPoint(-fieldSize, 0), *b);
    create_flame_line(QPoint(fieldSize, 0), *b);
    create_flame_line(QPoint(0, -fieldSize), *b);
    create_flame_line(QPoint(0, fieldSize), *b);

    //delete the bomb
    b->deleteLater();
}
