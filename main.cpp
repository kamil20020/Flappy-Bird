#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include <vector>
#include <cmath>

using namespace sf;
using namespace std;

class Player{

    RectangleShape player;

    float speedX;
    float speedY;
    float speedDown;

    public:

    Player(){

        player.setPosition(0, 200);
        player.setSize(Vector2f(50, 50));
        player.setFillColor(Color::Green);

        speedY = 20;
        speedX = 10;
        speedDown = 10;
    }

    void movePlayer(View &view){

        player.move(speedX, 0);

        if(player.getPosition().y + player.getSize().y + speedDown <= view.getSize().y){

            player.move(0, speedDown);
        }

        if(Keyboard::isKeyPressed(Keyboard::Up)){

            if(player.getPosition().y - speedY >= 0){

                player.move(0, -speedY);
            }
        }
    }

    RectangleShape returnPlayer(){

        return player;
    }

};

class Box{

    RectangleShape box;

    static float sizeTop;
    static float positionX;
    static int numberOfBoxes;

    public:

    Box(RenderWindow &window, bool doTop, float boxDelayX, float boxDelayY){

        if(doTop == true){

            box.setPosition(positionX, 0);
            box.setSize(Vector2f(100, rand()%int(window.getSize().y - boxDelayY)));
            box.setFillColor(Color::Yellow);

            sizeTop = box.getSize().y;
        }
        else{

            box.setPosition(positionX, sizeTop + boxDelayY);
            box.setSize(Vector2f(100, window.getSize().y - box.getPosition().y));
            box.setFillColor(Color::Red);

            positionX = positionX + box.getSize().x + boxDelayX;
        }
    }

    RectangleShape &returnBox(){

        return box;
    }

    static float &returnSizeTop(){

        return sizeTop;
    }

    static float &returnPositionX(){

        return positionX;
    }
};

float Box::sizeTop = 0;
float Box::positionX = 0;
int Box::numberOfBoxes = 0;

class Background{

    shared_ptr <Player> player;

    vector <shared_ptr <Box>> boxes;

    float boxDelayX;
    float boxDelayY;

    public:

    Background(RenderWindow &window){

        player = make_shared<Player>();

        setBoxes(window);
    }

    void setBoxes(RenderWindow &window){

        boxDelayX = 300;
        boxDelayY = 250;

        int howMuchOnAWindow = floor(window.getSize().x/boxDelayX);

        Box::returnPositionX() = boxDelayX;

        for(int i=0; i < howMuchOnAWindow; i++){

            boxes.push_back(make_shared<Box>(window, true, boxDelayX, boxDelayY));
            boxes.push_back(make_shared<Box>(window, false, boxDelayX, boxDelayY));
        }
    }

    void doBoxAppear(RenderWindow &window, View view){

        if(boxes.back() -> returnBox().getPosition().x + boxes.back() -> returnBox().getSize().x < view.getCenter().x + window.getPosition().x - boxDelayX){

            boxes.erase(boxes.begin());
            boxes.erase(boxes.begin());

            boxes.push_back(make_shared<Box>(window, true, boxDelayX, boxDelayY));
            boxes.push_back(make_shared<Box>(window, false, boxDelayX, boxDelayY));
        }
    }

    bool playerCollisionWithBoxes(){

        for(unsigned int i=0; i < boxes.size(); i++){

            if(player -> returnPlayer().getGlobalBounds().intersects(boxes.at(i) -> returnBox().getGlobalBounds())){

                return true;
            }
        }

        return false;
    }

    void lose(RenderWindow &window){

        window.close();
    }

    vector<shared_ptr <Box>> &returnBoxes(){

        return boxes;
    }

    shared_ptr <Player> &returnPlayer(){

        return player;
    }

};

int main()
{
    srand(time(0));

    RenderWindow window(VideoMode(1000, 600), "SFML works!");

    window.setFramerateLimit(30);

    View view(FloatRect(0.f, 0.f, 1000.f, 600.f));

    view.setCenter(window.getSize().x/2, 0);

    shared_ptr <Background> bg = make_shared<Background>(window);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::Resized){

                FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);

                window.setView(View(visibleArea));
            }
        }

        float x = bg -> returnPlayer() -> returnPlayer().getPosition().x - bg -> returnPlayer() -> returnPlayer().getSize().x + window.getSize().x/2;

        view.setCenter(x, window.getSize().y/2);

        bg -> returnPlayer() -> movePlayer(view);

        window.clear();

        window.setView(view);

        bg -> doBoxAppear(window, view);

        window.draw(bg -> returnPlayer() -> returnPlayer());

        for(unsigned int i=0; i < bg -> returnBoxes().size(); i++){

            window.draw(bg -> returnBoxes().at(i) -> returnBox());
        }

        if(bg -> playerCollisionWithBoxes()){

            bg -> lose(window);
        }

        window.display();
    }

    return 0;
}
