#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <array>
#include <vector>
#include <random>

const unsigned int windowSize_x = 1000;
const unsigned int windowSize_y = 500;    // setarea dimensiunilor ferestrei
const unsigned int groundOffset = windowSize_y - 150.f; // setarea inaltimii la care se afla pamantul
int gameSpeed = 8; // setarea vitezei la care se intampla evenimentele
bool playerDead = false; 
bool playDeadSound = false; // conditii pentru inceperea jocului

struct Fps_s
{
    sf::Font font;
    sf::Text text;
    sf::Clock clock;
    int Frame;
    int fps;
};
class Fps
{
    Fps_s fps;
public:
    Fps()
        :fps()
    {
        if (fps.font.loadFromFile("rsrc/Fonts/font.ttf"))  // setarea fontului pentru afisaera nr. de cadre pe secunda
        {
            fps.text.setFont(fps.font);
        }
        fps.text.setCharacterSize(15); // setarea dimensiunii
        fps.text.setPosition(sf::Vector2f(fps.text.getCharacterSize() + 10.f, fps.text.getCharacterSize())); // setarea pozitiei in coltul din stanga sus
        fps.text.setFillColor(sf::Color(83, 83, 83)); // setarea culorii
    }
    void update() // functie pentru update a nr. de cadre per secunda
    {
        if (fps.clock.getElapsedTime().asSeconds() >= 1.f)
        {
            fps.fps = fps.Frame; fps.Frame = 0; fps.clock.restart();
        }
        fps.Frame++;
        fps.text.setString("FPS :- " + std::to_string(fps.fps));
    }
    void drawTo(sf::RenderWindow& window)
    {
        window.draw(fps.text); // afisare
    }

};


class SoundManager // clasa ce se ocupa de managementul sunetelor
{
public:
    sf::SoundBuffer dieBuffer;
    sf::SoundBuffer jumpBuffer;
    sf::SoundBuffer pointBuffer; 
    sf::Sound dieSound;
    sf::Sound jumpSound;
    sf::Sound pointSound; // initializarea sunetelor

    SoundManager()
        :dieBuffer(), jumpBuffer(), pointBuffer(), dieSound(), jumpSound(), pointSound()
    {
        dieBuffer.loadFromFile("rsrc/Sounds/die.wav");
        jumpBuffer.loadFromFile("rsrc/Sounds/jump.wav");
        pointBuffer.loadFromFile("rsrc/Sounds/point.wav"); // setarea sunetelor corespunzatoare

        dieSound.setBuffer(dieBuffer);
        jumpSound.setBuffer(jumpBuffer);
        pointSound.setBuffer(pointBuffer); // setarea bufferelor 
    }
};

class Ground // clasa ce se ocupa cu crearea pamantului
{
public:
    sf::Sprite groundSprite; // creare obiect
    sf::Texture groundTexture; // asociere textura
    int offset{ 0 };
    Ground()
        :groundSprite(), groundTexture()
    {
        if (groundTexture.loadFromFile("rsrc/Images/GroundImage.png")) // asociere textura
        {
            groundSprite.setTexture(groundTexture);
            groundSprite.setPosition(sf::Vector2f(0.f, windowSize_y - groundTexture.getSize().y - 50)); // pozitionarea verticala a pamantului
        }
    }

    void updateGround() // metoda ce reseteaza textura pamantului daca se ajunge la marginea texturii
    {
        if (playerDead == false)
        {
            if (offset > groundTexture.getSize().x - windowSize_x)
                offset = 0;

            offset += gameSpeed;
            groundSprite.setTextureRect(sf::IntRect(offset, 0, windowSize_x, windowSize_y));
        }

        if (playerDead == true)
            groundSprite.setTextureRect(sf::IntRect(offset, 0, windowSize_x, windowSize_y));

    }
    void reset() // functie de reset pentru folosirea in cazul in care jucatorul va pierde si va incepe o noua runda
    {
        offset = 0;
        groundSprite.setTextureRect(sf::IntRect(0, 0, windowSize_x, windowSize_y));
    }

};


class Obstacle // clasa ce seteaza marginile unui obstacol
{
public:
    sf::Sprite obstacleSprite;
    sf::FloatRect obstacleBounds{ 0.f, 0.f, 0.f, 0.f };
    Obstacle(sf::Texture& texture)
        :obstacleSprite(), obstacleBounds()
    {
        obstacleSprite.setTexture(texture);
        obstacleSprite.setPosition(sf::Vector2f(windowSize_x, groundOffset));
    }
};

class Obstacles // clasa ce contine variatiile de obstacole
{
public:
    std::vector<Obstacle> obstacles;

    sf::Time spawnTimer;
    sf::Texture obstacleTexture_1;
    sf::Texture obstacleTexture_2;
    sf::Texture obstacleTexture_3;
    int randomNumber{ 0 };

    Obstacles()
        :spawnTimer(sf::Time::Zero)
    {
        obstacles.reserve(5);

        if (obstacleTexture_1.loadFromFile("rsrc/Images/Cactus1.png"))
        {
            std::cout << "loaded cactus Image 1 " << std::endl;
        }

        if (obstacleTexture_2.loadFromFile("rsrc/Images/Cactus2.png"))
        {
            std::cout << "Loaded cactus Image 2" << std::endl;
        }

        if (obstacleTexture_3.loadFromFile("rsrc/Images/Cactus3.png"))  // incarcarea texturilor
        {
            std::cout << "Loaded cactus Image 3" << std::endl;

        }

    }

    void update(sf::Time& deltaTime) // functie pentru pozitionarea aleatorie a obstacolelor
    {
        spawnTimer += deltaTime;
        if (spawnTimer.asSeconds() > 0.5f + gameSpeed / 8)
        {
            randomNumber = (rand() % 3) + 1;
            if (randomNumber == 1)
            {
                obstacles.emplace_back(Obstacle(obstacleTexture_1));
            }
            if (randomNumber == 2)
            {
                obstacles.emplace_back(Obstacle(obstacleTexture_2));
            }
            if (randomNumber == 3)
            {
                obstacles.emplace_back(Obstacle(obstacleTexture_2));
            }
            spawnTimer = sf::Time::Zero;
        }

        if (playerDead == false) // daca playerul nu este mort, se vor genera obstacole
        {
            for (int i = 0; i < obstacles.size(); i++)
            {
                obstacles[i].obstacleBounds = obstacles[i].obstacleSprite.getGlobalBounds();
                obstacles[i].obstacleBounds.width -= 10.f;
                obstacles[i].obstacleSprite.move(-1 * gameSpeed, 0.f);
                if (obstacles[i].obstacleSprite.getPosition().x < -150.f)
                {
                    std::vector<Obstacle>::iterator obstacleIter = obstacles.begin() + i;
                    obstacles.erase(obstacleIter);
                }
            }
        }

        if (playerDead == true)  // in caz contrar nu se vor genera obstacole
        {
            for (auto& obstacles : obstacles)
            {
                obstacles.obstacleSprite.move(0.f, 0.f);
            }
        }

    }

    void drawTo(sf::RenderWindow& window) // afisarea pe ecran
    {
        for (auto& obstacles : obstacles)
        {
            window.draw(obstacles.obstacleSprite);
        }
    }

    void reset() // resetare in cazul in care jucatorul va pierde
    {
        obstacles.erase(obstacles.begin(), obstacles.end());
    }
};



class Dino // clasa pentru personajul principal
{
public:
    sf::Sprite dino; // initializare obiect
    sf::Vector2f dinoPos{ 0.f, 0.f }; // setare pozitie
    sf::Vector2f dinoMotion{ 0.f, 0.f };
    sf::Texture dinoTex;
    sf::FloatRect dinoBounds;
    SoundManager soundManager;
    std::array<sf::IntRect, 6> frames;
    sf::Time timeTracker;
    int animationCounter{ 0 };

    Dino()
        :dino(), dinoTex(), soundManager(), timeTracker()
    {
        if (dinoTex.loadFromFile("rsrc/Images/PlayerSpriteSheet.png")) // setare textura
        {
            dino.setTexture(dinoTex);
            for (int i = 0; i < frames.size(); i++)
                frames[i] = sf::IntRect(i * 90, 0, 90, 95); // creare a unui vector ce contine cadrele personajului pentru animatie
            dino.setTextureRect(frames[0]);
            dinoPos = dino.getPosition();
        }
        else
        {
            std::cout << "Error loading the PlayerSprite texture" << std::endl; // eroare in care nu se poate incarca textura sau nu se gaseste sursa
        }
    }

    void update(sf::Time& deltaTime, std::vector<Obstacle>& obstacles) // functie de update pentru update a pozitiilor obiectelor din cadru
    {
        dinoPos = dino.getPosition();
        dinoBounds = dino.getGlobalBounds();
        dinoBounds.height -= 15.f; dinoBounds.width -= 10.f;
        timeTracker += deltaTime;
        for (auto& obstacles : obstacles)
        {
            if (dinoBounds.intersects(obstacles.obstacleBounds)) // if statement pentru coliziunea intre personaj si obstacole
            {
                playerDead = true;
            }
        }



        if (!playerDead) // functie pentru cazul in care personajul nu este mort
        {
            walk();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) == true && dinoPos.y >= windowSize_y - 150.f) // la apasarea tastei space se va seta textura frames[1]
            {
                animationCounter = 0;
                dinoMotion.y = -20.f; dino.setTextureRect(frames[1]);
                soundManager.jumpSound.play();   // actionarea sunetului pentru jump
            }

            if (dinoPos.y < windowSize_y - 150.f)
            {
                dinoMotion.y += 1.f; dino.setTextureRect(frames[1]);
            }

            if (dinoPos.y > windowSize_y - 150.f)
            {
                dino.setPosition(sf::Vector2f(dino.getPosition().x, windowSize_y - 150.f));
                dinoMotion.y = 0.f;

            }

            dino.move(dinoMotion);
        }
        if (playerDead == true)
        {
            dinoMotion.y = 0.f;
            dino.setTextureRect(frames[3]);    // setarea texturii nr. 3
            if (timeTracker.asMilliseconds() < 300.f)
            {
                soundManager.dieSound.play();  // actionarea sunetului pentru player dead

                
            }
            else
            {
                
               
                timeTracker = sf::Time::Zero;
               
             
            }
            soundManager.dieSound.stop();
        }

    }

    void walk()
    {
        for (int i = 0; i < frames.size() - 3; i++)
            if (animationCounter == (i + 1) * 3)
                dino.setTextureRect(frames[i]);  // setarea texturilor pentru animatie

        if (animationCounter >= (frames.size() - 2) * 3)
            animationCounter = 0;

        animationCounter++;
    }
    void reset()  // resetarea texturilor
    {
        dinoMotion.y = 0; dino.setPosition(sf::Vector2f(dino.getPosition().x, windowSize_y - 150.f));
        dino.setTextureRect(frames[0]);

    }

};




class Scores // clasa pentru salvarea scorului si comparare
{
public:
    sf::Text previousScoreText;
    sf::Text HIText;
    sf::Text scoresText;
    sf::Font scoresFont;
    SoundManager soundManager;
    short scores{ 0 };
    short previousScore{ 0 };
    short scoresIndex{ 0 };
    short scoresDiff{ 0 };
    short scoresInital;

    Scores()
        :scoresFont(), scoresText(), previousScoreText(), scoresInital(), soundManager()
    {
        if (scoresFont.loadFromFile("rsrc/Fonts/Font.ttf"))   // setarea fontului
        {
            scoresText.setFont(scoresFont);
            scoresText.setCharacterSize(15);
            scoresText.setPosition(sf::Vector2f(windowSize_x / 2 + windowSize_x / 4 + 185.f, scoresText.getCharacterSize() + 10.f));
            scoresText.setFillColor(sf::Color(83, 83, 83));

            previousScoreText.setFont(scoresFont);
            previousScoreText.setCharacterSize(15);
            previousScoreText.setPosition(sf::Vector2f(scoresText.getPosition().x - 100.f, scoresText.getPosition().y));
            previousScoreText.setFillColor(sf::Color(83, 83, 83));

            HIText.setFont(scoresFont);
            HIText.setCharacterSize(15);
            HIText.setPosition(sf::Vector2f(previousScoreText.getPosition().x - 50.f, previousScoreText.getPosition().y));
            HIText.setFillColor(sf::Color(83, 83, 83));
        }
        HIText.setString("HI");
        scoresInital = 0;
    }

    void update()
    {
        if (playerDead == false)
        {
            scoresIndex++;    // incrementarea scorului pe masura ce jucatorul joaca
            if (scoresIndex >= 5)
            {
                scoresIndex = 0;
                scores++;
            }
            scoresDiff = scores - scoresInital;
            if (scoresDiff > 100)
            {
                scoresInital += 100;
                gameSpeed += 1;                     // pe masura ce scorul se mareste viteza jocului se va mari
                soundManager.pointSound.play();    // actionarea sunetului pentru scor
            }

            scoresText.setString(std::to_string(scores));   
            previousScoreText.setString(std::to_string(previousScore));
        }

    }

    void reset()  // resetarea scorului
    {
        if (scores > previousScore)
            previousScore = scores;
        if (scores < previousScore)
            previousScore = previousScore;


        previousScoreText.setString(std::to_string(previousScore));
        scores = 0;
    }

};

class RestartButton  //clasa pentru butonul de restart
{
public:
    sf::Sprite restartButtonSprite;
    sf::FloatRect restartButtonSpriteBounds;
    sf::Texture restartButtonTexture;
    sf::Vector2f mousePos;
    bool checkPressed{ false };

    RestartButton()
        :restartButtonSprite(), restartButtonTexture(), mousePos(0.f, 0.f), restartButtonSpriteBounds()
    {
        if (restartButtonTexture.loadFromFile("rsrc/Images/RestartButton.png"))   // incarcarea texturii
        {
            restartButtonSprite.setTexture(restartButtonTexture);
            restartButtonSprite.setPosition(sf::Vector2f(windowSize_x / 2 - restartButtonTexture.getSize().x / 2, windowSize_y / 2));
            restartButtonSpriteBounds = restartButtonSprite.getGlobalBounds();
        }
    }
};



class Clouds // clasa pentru setarea norilor
{
public:
    std::vector<sf::Sprite> clouds;
    sf::Time currTime;
    sf::Texture cloudTexture;
    std::random_device dev;
    std::mt19937 rng{ dev() };


    Clouds()
        :cloudTexture(), clouds(), currTime(), dev()
    {
        if (cloudTexture.loadFromFile("rsrc/Images/Clouds.png")) // incarcarea texturilor
        {
            std::cout << "Loaded CloudTexture" << std::endl; // daca se incarca se va afisa mesajul corespunzator
        }
        clouds.reserve(4);
        clouds.emplace_back(sf::Sprite(cloudTexture));
        clouds.back().setPosition(sf::Vector2f(windowSize_x, windowSize_y / 2 - 40.f));  // setarea pe axa vertiale
    }

    void updateClouds(sf::Time& deltaTime)
    {

        currTime += deltaTime;
        if (currTime.asSeconds() > 8.f)
        {
            clouds.emplace_back(sf::Sprite(cloudTexture));

            std::uniform_int_distribution<std::mt19937::result_type> dist6(windowSize_y / 2 - 200, windowSize_y / 2 - 50); // setarea pe axa verticala a texturilor ce va varia intre windowsSize_y/2-200 si windowsSize_y/2-50
            clouds.back().setPosition(sf::Vector2f(windowSize_x, dist6(rng))); // 

            currTime = sf::Time::Zero;
        }


        for (int i = 0; i < clouds.size(); i++) // accesarea vectorului clouds
        {
            if (playerDead == false)
                clouds[i].move(sf::Vector2f(-1.f, 0.f));
            if (playerDead == true)
                clouds[i].move(sf::Vector2f(-0.5f, 0.f));

            if (clouds[i].getPosition().x < 0.f - cloudTexture.getSize().x)
            {
                std::vector<sf::Sprite>::iterator cloudIter = clouds.begin() + i;
                clouds.erase(cloudIter);
            }
        }
    }


    void drawTo(sf::RenderWindow& window) // randarea texturilor pe ecran
    {
        for (auto& clouds : clouds)
        {
            window.draw(clouds);
        }
    }

};



class GameState // clasa ce contine toate celelalte clase pentru desfasurarea logica a jocului
{
public:
    Fps fps;
    Dino dino;
    Ground ground;
    Obstacles obstacles;
    Scores scores;
    Clouds clouds;
    RestartButton restartButton;
    RestartButton restartKeySpace;
    sf::Font gameOverFont;
    sf::Text gameOverText;
    sf::Vector2f mousePos{ 0.f, 0.f };

    GameState()
        :fps(), dino(), ground(), obstacles(), scores(), clouds(), gameOverFont(), gameOverText()
    {
        gameOverFont.loadFromFile("rsrc/Fonts/Font.ttf"); // incarcarea fontului pentru textul Game Over
        gameOverText.setFont(gameOverFont);
        dino.dino.setPosition(sf::Vector2f(windowSize_x / 2 - windowSize_x / 4, windowSize_y - 150.f));
        gameOverText.setString("Game Over"); 
        gameOverText.setPosition(sf::Vector2f(restartButton.restartButtonSprite.getPosition().x - gameOverText.getCharacterSize(),
            restartButton.restartButtonSprite.getPosition().y - 50));
        gameOverText.setFillColor(sf::Color(83, 83, 83));
    }
    void setMousePos(sf::Vector2i p_mousePos) 
    {
        mousePos.x = p_mousePos.x;
        mousePos.y = p_mousePos.y;
    }

    void update(sf::Time deltaTime) // updatarea jocului in functie de ce tasta se apasa
    {
        restartButton.checkPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);  // in cazul in care se va apasa left click se va restarta jocul
        restartKeySpace.checkPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Space); // in cazul in care se apasa tasta space se va restarta jocul
        if ((playerDead == true &&
            restartButton.checkPressed == true)||(playerDead == true && restartKeySpace.checkPressed == true))
        {
            ground.reset();
            obstacles.reset();
            dino.reset();
            scores.reset();

            playerDead = false;
        }

        
        else
        {
            ground.updateGround();
            obstacles.update(deltaTime);
            dino.update(deltaTime, obstacles.obstacles);
            clouds.updateClouds(deltaTime);
            scores.update();
        }
        fps.update();
    }

    void drawTo(sf::RenderWindow& window)
    {
        if (playerDead == true)  // daca jucatorul nu va fi mort se vor afisa toate componentele
        {
            clouds.drawTo(window);
            window.draw(ground.groundSprite);
            obstacles.drawTo(window);
            window.draw(scores.scoresText);
            window.draw(scores.previousScoreText);
            window.draw(scores.HIText);
            window.draw(dino.dino);
            window.draw(gameOverText);
            window.draw(restartButton.restartButtonSprite);
            fps.drawTo(window);
        }
        else  // daca nu se vor afisa doar cateva dintre ele
        {
            clouds.drawTo(window);
            window.draw(ground.groundSprite);
            obstacles.drawTo(window);
            window.draw(scores.scoresText);
            window.draw(scores.previousScoreText);
            window.draw(scores.HIText);
            window.draw(dino.dino);
            fps.drawTo(window);
        }
    }

};


int main()  // functie in care se creaza fereastra jocului
{
    sf::RenderWindow window(sf::VideoMode(windowSize_x, windowSize_y), "Google Chrome"); // setarea videomode cu valorile windowssize_x si y, si denumirea ferestrei
    window.setVerticalSyncEnabled(true); // setare sincronizare verticala

    GameState gameState;
    window.setFramerateLimit(60); // setarea nr maxim de cadre pe secunda

    
    sf::Event event; // crearea unui obiect de tip evenniment
    sf::Time deltaTime; // timpul de derulare
    sf::Clock deltaTimeClock; // ceas pentru cronometrare

    while (window.isOpen()) // cat timp fereastra este deschisa
    {
        while (window.pollEvent(event)) // se va executa evenimentul 
        {
            if (event.type == sf::Event::Closed)
                window.close();
            gameState.setMousePos(sf::Mouse::getPosition(window));
        }
        deltaTime = deltaTimeClock.restart();

        gameState.update(deltaTime); // update a timpului de derulare

        window.clear(sf::Color::White);
        gameState.drawTo(window);
        window.display(); // display a contentului
    }
}