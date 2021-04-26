#include <iostream>
#include <random>
#include <cmath>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

struct Math {
  static float random() {
    static mt19937 engine(time(nullptr));
    uniform_real_distribution<float> dist(0.0f, 1.0f);
    return dist(engine);
  }
};

struct Rectangle {
  float x;
  float y;
  float width;
  float height;
  bool visible;
};

struct Circle {
  float x;
  float y;
  short state;
  int radius;
  bool visible = false;
  bool lastBoardTuchedX = false;
  bool lastBoardTuchedY = false;
};

/*
  variable globale creant une instance de RenderWindows
  qui sera initialise dans le main puis utilisee dans les fonctions
  menu et game
*/

RenderWindow window;

/*
  menu de fin de partie qui redirige le joueur sur le menu, sur le jeu ou qui permet de quitter la fenetre
*/

int redirectWhenGameEnded() {
  Vector2u size = window.getSize();

  const int NB_BUTTONS = 3;
  Rectangle redirectButton[NB_BUTTONS];

  for (int i = 0 ; i < NB_BUTTONS ; i++) {
    redirectButton[i].width = size.x / 2.0f;
    redirectButton[i].height = size.y / 6.0f;
    redirectButton[i].x = (size.x - redirectButton[i].width) / 2.0f;
    redirectButton[i].y = ((size.y / ((NB_BUTTONS + 1.0f)) * (i + 1)) - (redirectButton[i].height) / 2.0f);
    redirectButton[i].visible = true;
  }

  Texture redirectButtonTexture;
  if (!redirectButtonTexture.loadFromFile("resources/images/redirectButton.png")) {
    cout<<"loading failure"<<endl;
    return 1;
  }

  Texture redirectButtonHighlightedTexture;
  if (!redirectButtonHighlightedTexture.loadFromFile("resources/images/startButton.png")) {
    cout<<"loading failure"<<endl;
    return 1;
  }

  Texture redirectBgdTexture;
  if (!redirectBgdTexture.loadFromFile("resources/images/redirectBackground.png")) {
    cout<<"loading failure"<<endl;
    return 1;
  }
  redirectBgdTexture.setSmooth(true);

  Sprite redirectBackground;
  Vector2u bgdSize = redirectBgdTexture.getSize();
  redirectBackground.setTexture(redirectBgdTexture);
  redirectBackground.setOrigin(size.x / bgdSize.x - 1, size.y / bgdSize.y - 1);

  /********************************************************************************/

  Font montserratFont;

  if (!montserratFont.loadFromFile("resources/fonts/montserrat/Montserrat-Bold.ttf")) {
    cout<<"loading failure"<<endl;
    return -1;
  }

  const int TEXT_SIZES_ON_BUTTONS = 32;

  Text menu;

  menu.setFont(montserratFont);
  menu.setString("MENU");
  menu.setCharacterSize(TEXT_SIZES_ON_BUTTONS);
  menu.setPosition(size.x / 2 - menu.findCharacterPos(2).x, size.y / (NB_BUTTONS + 1.0f) - (redirectButton[0].height) / 7.0f);
  menu.setFillColor(Color::White);
  menu.setStyle(Text::Bold);

  Text playAgain;

  playAgain.setFont(montserratFont);
  playAgain.setString("PLAY AGAIN");
  playAgain.setCharacterSize(TEXT_SIZES_ON_BUTTONS);
  playAgain.setPosition(size.x / 2 - playAgain.findCharacterPos(5).x - 10, size.y / (NB_BUTTONS + 1.0f) * 2 - (redirectButton[1].height) / 7.0f);
  playAgain.setFillColor(Color::White);
  playAgain.setStyle(Text::Bold);

  Text exitGame;

  exitGame.setFont(montserratFont);
  exitGame.setString("EXIT GAME");
  exitGame.setCharacterSize(TEXT_SIZES_ON_BUTTONS);
  exitGame.setPosition(size.x / 2 - exitGame.findCharacterPos(5).x - 10, size.y / (NB_BUTTONS + 1.0f) * 3 - (redirectButton[2].height) / 7.0f);
  exitGame.setFillColor(Color::White);
  exitGame.setStyle(Text::Bold);

  /********************************************************************************/

  int highlightedButton = -1;

  while (window.isOpen()) {
    
    Event event;

    while (window.pollEvent(event)) {

      if (event.type == Event::Closed) {
        window.close();
      }

      if (event.type == sf::Event::MouseButtonPressed) {
        for (int i = 0 ; i < NB_BUTTONS ; i++) {
          if (event.mouseButton.x >= redirectButton[i].x && event.mouseButton.x <= redirectButton[i].x + redirectButton[i].width && event.mouseButton.y >= redirectButton[i].y && event.mouseButton.y <= redirectButton[i].y + redirectButton[i].height) {
            return i;
          }
        }
      }

      if (event.type == sf::Event::MouseMoved) {
        for (int i = 0 ; i < NB_BUTTONS ; i++) {
          if (event.mouseMove.x >= redirectButton[i].x && event.mouseMove.x <= redirectButton[i].x + redirectButton[i].width && event.mouseMove.y >= redirectButton[i].y && event.mouseMove.y <= redirectButton[i].y + redirectButton[i].height) {
            highlightedButton = i;
            break;
          } else {
            highlightedButton = -1;
          }
        }
      }
    }

    window.clear(Color::White);
    window.draw(redirectBackground);
    for (int i = 0 ; i < NB_BUTTONS ; i++) {
      RectangleShape cbShape;
      cbShape.setSize({ redirectButton[i].width, redirectButton[i].height });
      cbShape.setPosition(redirectButton[i].x, redirectButton[i].y);
      if (highlightedButton == i)
        cbShape.setTexture(&redirectButtonHighlightedTexture);
      else
        cbShape.setTexture(&redirectButtonTexture);
      window.draw(cbShape);
    }

    window.draw(menu);
    window.draw(playAgain);
    window.draw(exitGame);

    window.display();
    
  }

  return -1;
}

/*
  fonction de menu
  menu permet de quitter le jeu ou de commencer une partie
*/

int menu() {
  Vector2u size = window.getSize();

  Rectangle startButton;
  startButton.width = size.x / 3.0f;
  startButton.height = size.y / 4.0f;
  startButton.x = (size.x - startButton.width) / 2.0f;
  startButton.y = (size.y - startButton.height) / 2.0f;
  startButton.visible = true;

  Texture startButtonTexture;
  if (!startButtonTexture.loadFromFile("resources/images/startButton.png")) {
    cout<<"loading failure"<<endl;
    return 1;
  }

  Texture startButtonHighlightedTexture;
  if (!startButtonHighlightedTexture.loadFromFile("resources/images/startButtonHighlighted.png")) {
    cout<<"loading failure"<<endl;
    return 1;
  }

  Texture menuBgdTexture;
  if (!menuBgdTexture.loadFromFile("resources/images/menuBackground.png")) {
    cout<<"loading failure"<<endl;
    return 1;
  }
  menuBgdTexture.setSmooth(true);

  Sprite menuBackground;
  Vector2u bgdSize = menuBgdTexture.getSize();
  menuBackground.setTexture(menuBgdTexture);
  menuBackground.setOrigin(size.x / bgdSize.x - 1.0f, size.y / bgdSize.y - 1.0f);

  bool highlighted = false;

  while (window.isOpen()) {
    
    Event event;

    while (window.pollEvent(event)) {

      if (event.type == Event::Closed) {
        window.close();
      }

      if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.x >= startButton.x && event.mouseButton.x <= startButton.x + startButton.width && event.mouseButton.y >= startButton.y && event.mouseButton.y <= startButton.y + startButton.height) {
          return 1;
        }
      }

      if (event.type == sf::Event::MouseMoved) {
        highlighted = (event.mouseMove.x >= startButton.x && event.mouseMove.x <= startButton.x + startButton.width && event.mouseMove.y >= startButton.y && event.mouseMove.y <= startButton.y + startButton.height) ? true : false;
      }
    }

    window.clear(Color::White);
    window.draw(menuBackground);

    if (startButton.visible) {
      RectangleShape bShape;
      bShape.setSize({ startButton.width, startButton.height });
      bShape.setPosition(startButton.x, startButton.y);
      (highlighted) ? bShape.setTexture(&startButtonHighlightedTexture) : bShape.setTexture(&startButtonTexture);
      window.draw(bShape);
    }

    window.display();
    
  }

  return -1;
}

/*
  fonction de fin de la partie
  permet de rendre invisible toutes les entitees
*/

int gameOverAnimation() {
  Vector2u size = window.getSize();

  Texture gameOverBgdTexture;
  if (!gameOverBgdTexture.loadFromFile("resources/images/gameOverBackground.png")) {
    cout<<"loading failure"<<endl;
    return 1;
  }
  gameOverBgdTexture.setSmooth(true);

  Sprite gameOverBackground;
  Vector2u bgdSize = gameOverBgdTexture.getSize();
  gameOverBackground.setTexture(gameOverBgdTexture);
  gameOverBackground.setOrigin(size.x / bgdSize.x - 1.0f, size.y / bgdSize.y - 1.0f);

  Clock clock;

  float animationDuration = 0.0f;

  while (window.isOpen()) {

    Event event;

    while (window.pollEvent(event)) {

      if (event.type == Event::Closed) {
        window.close();
      }
    }

    animationDuration += clock.restart().asSeconds();

    if (animationDuration >= 2) {
      return 0;
    }

    window.clear(Color::White);
    window.draw(gameOverBackground);
    window.display();
    
  }

  return -1;
}

int endOfGame(Circle bubble[], int NB_BUBBLES, Rectangle player, Rectangle grapple, bool win) {
  Vector2u size = window.getSize();
  
  for (int i = 0 ; i < NB_BUBBLES ; i++) {
      bubble[i].visible = false;
  }

  player.visible = false;

  grapple.visible = false;

  /********************************************************************************/

  Font montserratFont;

  if (!montserratFont.loadFromFile("resources/fonts/montserrat/Montserrat-Bold.ttf")) {
    cout<<"loading failure"<<endl;
    return -1;
  }

  const int TEXT_SIZES_ON_BUTTONS = 32;

  Text winMessage;

  winMessage.setFont(montserratFont);
  winMessage.setString("\tYOU'VE WIN\nI DINDN'T KNOW THAT THIS GAME WAS WINNABLE\nYOU'RE THE BOSS");
  winMessage.setCharacterSize(TEXT_SIZES_ON_BUTTONS);
  winMessage.setPosition(size.x / 2 - winMessage.findCharacterPos(29).x, size.y / 2.0f);
  winMessage.setFillColor(Color::White);
  winMessage.setStyle(Text::Bold);

  Text lossMessage;

  lossMessage.setFont(montserratFont);
  lossMessage.setString("\tYOU'VE LOST\nI'M PRETTY SURE YOU'LL WIN NEXT TIME");
  lossMessage.setCharacterSize(TEXT_SIZES_ON_BUTTONS);
  lossMessage.setPosition(size.x / 2 - lossMessage.findCharacterPos(27).x, size.y / 2.0f);
  lossMessage.setFillColor(Color::Blue);
  lossMessage.setStyle(Text::Bold);

  /********************************************************************************/

  Clock clock;

  float animationDuration = 0.0f;

  if (win) {

    do {
      animationDuration += clock.restart().asSeconds();
      window.clear(Color::White);
      window.draw(winMessage);
      window.display();
    } while (animationDuration <= 2);

    gameOverAnimation();
  } else {
    do {
      animationDuration += clock.restart().asSeconds();
      window.clear(Color::White);
      window.draw(lossMessage);
      window.display();
    } while (animationDuration <= 2);

    gameOverAnimation();
  }

  return 0;
}

bool bubbleCollisionWithOneBubble(Circle bubble1, Circle bubble2) {
  int test = (bubble1.x - bubble2.x) * (bubble1.x - bubble2.x) + (bubble1.y - bubble2.y) * (bubble1.y - bubble2.y);
  if (test > (bubble1.radius + bubble2.radius)*(bubble1.radius + bubble2.radius))
    return false;
  else
    return true;
}

bool bubbleCollisionBubbles(Circle bubble[], int index) {
  for (int i = 0 ; i < index ; i++) {
    if (bubble[i].visible && bubbleCollisionWithOneBubble(bubble[i], bubble[index])) {
      return true;
    }
  }

  return false;
}

bool rectangleCollisionOneBubble(Rectangle entity, Circle bubble){
/*
  float xNear = clamp(bubble.x, entity.x, entity.x + entity.width);
  float yNear = clamp(bubble.y, entity.y, entity.y + entity.height);
  float distanceX = bubble.x - xNear;
  float distanceY = bubble.y - yNear;

  return((distanceX * distanceX) + (distanceY * distanceY) < (bubble.radius * bubble.radius));
*/
return false;
}

bool rectangleShapeCollisionWithBubbles(Rectangle entity, Circle bubble[], int nbBubbles, int *bubbleHit) {
  for (int i = 0 ; i < nbBubbles ; i++) {
    if (bubble[i].visible) {
      if (rectangleCollisionOneBubble(entity, bubble[i])) {
        *bubbleHit = i;
        return true;
      }
    }
  }
    
  return false;
}

/*
  gerer absolument le respawn des bulles
*/

int splitTheBubbles(Circle bubble[], int NB_BUBBLES, int bubbleHit, int playerPosX, const int BUBBLE_SIZE_FACTOR) {
  Vector2u size = window.getSize();

  switch (bubble[bubbleHit].state) {
    case 3:
    for (int i = bubbleHit ; i <= bubbleHit + 1 ; i++) {
      bubble[i].state = 2;
      bubble[i].radius = BUBBLE_SIZE_FACTOR * bubble[i].state;
      bubble[i].x = (i % 2 == 0) ? playerPosX - bubble[i].radius : playerPosX + bubble[i].radius;
      bubble[i].y = size.y / 5;
      bubble[i].visible = true;
      bubble[i].lastBoardTuchedX = (Math::random() < 0.5f) ? true : false;
      bubble[i].lastBoardTuchedY = (Math::random() < 0.5f) ? true : false;
    }
    break;

    case 2:
    if (bubbleHit % 4 == 0) {
      if (bubble[bubbleHit + 1].visible) {
        for (int i = bubbleHit + 2; i <= bubbleHit + 3 ; i++) {
          bubble[i].state = 1;
          bubble[i].radius = BUBBLE_SIZE_FACTOR * bubble[i].state;
          bubble[i].x = (i % 2 == 0) ? playerPosX - bubble[i].radius : playerPosX + bubble[i].radius;
          bubble[i].y = size.y / 5;
          bubble[i].visible = true;
          if (i == bubbleHit + 2) {
            bubble[i].lastBoardTuchedX = (Math::random() < 0.5f) ? true : false;
            bubble[i].lastBoardTuchedY = (Math::random() < 0.5f) ? true : false;
          } else {
            bubble[i].lastBoardTuchedX = (bubble[bubbleHit + 2].lastBoardTuchedX) ? false : true;
            bubble[i].lastBoardTuchedY = (bubble[bubbleHit + 2].lastBoardTuchedY) ? false : true;
          }
        }
        bubble[bubbleHit].visible = false;
      } else {
         for (int i = bubbleHit ; i <= bubbleHit + 1 ; i++) {
          bubble[i].state = 1;
          bubble[i].radius = BUBBLE_SIZE_FACTOR * bubble[i].state;
          bubble[i].x = (i % 2 == 0) ? playerPosX - bubble[i].radius : playerPosX + bubble[i].radius;
          bubble[i].y = size.y / 5;
          bubble[i].visible = true;
          if (i == bubbleHit) {
            bubble[i].lastBoardTuchedX = (Math::random() < 0.5f) ? true : false;
            bubble[i].lastBoardTuchedY = (Math::random() < 0.5f) ? true : false;
          } else {
            bubble[i].lastBoardTuchedX = (bubble[bubbleHit].lastBoardTuchedX) ? false : true;
            bubble[i].lastBoardTuchedY = (bubble[bubbleHit].lastBoardTuchedY) ? false : true;
          }
        }
      }
    } else {
      if (bubble[bubbleHit + 1].visible) {
        for (int i = bubbleHit + 2; i <= bubbleHit + 3 ; i++) {
          bubble[i].state = 1;
          bubble[i].radius = BUBBLE_SIZE_FACTOR * bubble[i].state;
          bubble[i].x = (i % 2 == 0) ? playerPosX - bubble[i].radius : playerPosX + bubble[i].radius;
          bubble[i].y = size.y / 5;
          bubble[i].visible = true;
          if (i == bubbleHit + 2) {
            bubble[i].lastBoardTuchedX = (Math::random() < 0.5f) ? true : false;
            bubble[i].lastBoardTuchedY = (Math::random() < 0.5f) ? true : false;
          } else {
            bubble[i].lastBoardTuchedX = (bubble[bubbleHit + 2].lastBoardTuchedX) ? false : true;
            bubble[i].lastBoardTuchedY = (bubble[bubbleHit + 2].lastBoardTuchedY) ? false : true;
          }
        }
        bubble[bubbleHit].visible = false;
      } else {
        for (int i = bubbleHit ; i <= bubbleHit + 1 ; i++) {
          bubble[i].state = 1;
          bubble[i].radius = BUBBLE_SIZE_FACTOR * bubble[i].state;
          bubble[i].x = (i % 2 == 0) ? playerPosX - bubble[i].radius : playerPosX + bubble[i].radius;
          bubble[i].y = size.y / 5;
          bubble[i].visible = true;
          if (i == bubbleHit) {
            bubble[i].lastBoardTuchedX = (Math::random() < 0.5f) ? true : false;
            bubble[i].lastBoardTuchedY = (Math::random() < 0.5f) ? true : false;
          } else {
            bubble[i].lastBoardTuchedX = (bubble[bubbleHit].lastBoardTuchedX) ? false : true;
            bubble[i].lastBoardTuchedY = (bubble[bubbleHit].lastBoardTuchedY) ? false : true;
          }
        }
      }
    }
    break;

    case 1:
      bubble[bubbleHit].visible = false;
      bubble[bubbleHit].state = 0;
    break;

    default:
      return -1;
    break;
  }

   return 0;
}

/*
  fonction de jeu
  cette fonction est l'ensemble d'une partie de jeu
  partie principale du programme
*/

int game () {
  Vector2u size = window.getSize();

  const int LEFT_SIDE_MARGIN = 0;

  Rectangle player;
  player.width = size.x / 16.0f;
  player.height = size.y / 5.0f;
  player.x = (size.x - player.width) / 2 + LEFT_SIDE_MARGIN;
  player.y = size.y - player.height;
  player.visible = true;
  
  Rectangle grapple;
  grapple.width = size.x / 50.0f;
  grapple.height = size.y * (3.0f / 4.0f);
  grapple.x = player.x + player.width + LEFT_SIDE_MARGIN;
  grapple.y = size.y + grapple.height;
  grapple.visible = true;

  /*
    ici on multiplie le nombre initial de bulles par 4
    car il y a 3 stades possibles pour les bulles
    a chaque nouveau stade, les bulles se divisent en 2
    donc au 3 eme stade il y a 2 * 2 fois plus de bulles qu au 1er
    1 * 2 * 2 = 4
  */

  const int NB_BUBBLES = 3 * 4;
  const int BUBBLE_SIZE_FACTOR = (int)((5.53f * (size.x * size.y)) / 265472);

  Circle bubble[NB_BUBBLES];

  for (int i = 0 ; i < NB_BUBBLES - 1; ++i) {
    if (i % 4 == 0) {
      do {
        bubble[i].x = Math::random() * size.x + LEFT_SIDE_MARGIN;
        bubble[i].y = Math::random() * size.y;
        bubble[i].state = 3;
        bubble[i].radius = BUBBLE_SIZE_FACTOR * bubble[i].state;
        bubble[i].visible = true;

        bubble[i].lastBoardTuchedX = (Math::random() < 0.5f) ? true : false;
        bubble[i].lastBoardTuchedY = (Math::random() < 0.5f) ? true : false;

      } while (bubble[i].x > size.x - bubble[i].radius * 2 + LEFT_SIDE_MARGIN ||
               bubble[i].y > size.y / 2 ||
               bubbleCollisionBubbles(bubble, i));
    }
  }

  Texture grappleTexture;
  if (!grappleTexture.loadFromFile("resources/images/grapple.png")) {
    cout<<"loading failure"<<endl;
    return -1;
  }

  Texture playerTexture;
  if (!playerTexture.loadFromFile("resources/images/player.png")) {
    cout<<"loading failure"<<endl;
    return -1;
  }

  Texture bubbleTexture;
  if (!bubbleTexture.loadFromFile("resources/images/bubble.png")) {
    cout<<"loading failure"<<endl;
    return -1;
  }
  bubbleTexture.setSmooth(true);

  Texture bgdTexture;
  if (!bgdTexture.loadFromFile("resources/images/background.png")) {
    cout<<"loading failure"<<endl;
    return -1;
  }
  bgdTexture.setSmooth(true);

  Sprite background;
  Vector2u bgdSize = bgdTexture.getSize();
  background.setTexture(bgdTexture);
  background.setOrigin(size.x / bgdSize.x -1, size.y / bgdSize.y -1);

  /********************************************************************************/

  Font montserratFont;

  if (!montserratFont.loadFromFile("resources/fonts/montserrat/Montserrat-Bold.ttf")) {
    cout<<"loading failure"<<endl;
    return -1;
  }

  Text timerDisplay;

  timerDisplay.setFont(montserratFont);
  timerDisplay.setString("");
  timerDisplay.setCharacterSize(32);
  timerDisplay.setPosition(30.0f, 50.0f);
  timerDisplay.setFillColor(Color::White);
  timerDisplay.setStyle(Text::Bold);

  Text preGameMessage;

  const int PRE_GAME_MESSAGE_CHARACTER_SIZE = (int)(size.x * size.y / 10000);

  preGameMessage.setFont(montserratFont);
  preGameMessage.setString("PRESS ENTER TO PLAY");
  preGameMessage.setCharacterSize(PRE_GAME_MESSAGE_CHARACTER_SIZE);
  preGameMessage.setPosition(size.x / 2 - (preGameMessage.findCharacterPos(9).x + preGameMessage.findCharacterPos(10).x) / 2, size.y / 2 - PRE_GAME_MESSAGE_CHARACTER_SIZE);
  preGameMessage.setFillColor(Color::Blue);
  preGameMessage.setStyle(Text::Bold);

  /********************************************************************************/

  bool leftKey = false;
  bool rightKey = false;
  bool spaceKey = false;
  bool enterKey = false;
  bool gameStarted = false;
  bool collisionGrappleOnBubble = false;
  int bubbleLeft = NB_BUBBLES;
  int bubbleHit = -1;

  float counterGrapple = 0.0f;
  float timer = 360.0f;
  int intTimerForDisplay = (int)(timer);
  int previousTimer = (int)(timer);
  string timerToDisplay = "";

  Clock clock;

  while (window.isOpen()) {

    /*
      peut etre deplacer Event event hors de la boucle while pour la fenetre
    */

    Event event;

    while (window.pollEvent(event)) {

      if (event.type == Event::Closed) {
        window.close();
      }

      if (event.type == Event::KeyPressed) {

        switch (event.key.code) {
          case Keyboard::Left:
            leftKey = true;
            rightKey = false;
          break;

          case Keyboard::Right:
            rightKey = true;
            leftKey = false;
          break;

          case Keyboard::Space:
            if (grapple.y >= size.y)
              spaceKey = true;
          break;

          case Keyboard::Enter:
              enterKey = true;
          break;

          default:
          break;
        }
      }

      if (event.type == Event::KeyReleased) {
        switch (event.key.code) {
          case Keyboard::Left:
            leftKey = false;
          break;

          case Keyboard::Right:
            rightKey = false;
          break;

          case Keyboard::Enter:
            enterKey = false;
          break;

          default:
          break;
        }
      }
    }

    float dt = clock.restart().asSeconds();

    const float pSpeed = 500.0f;
    float pDistance = pSpeed * dt;

    const float gSpeed = 850.0f;
    float gDistance = gSpeed * dt;

    const float bSpeed = 100.0f;
    float bDistance = bSpeed * dt;
    
    if (!gameStarted) {
      timerToDisplay = to_string(intTimerForDisplay / 60);
      timerToDisplay += " : ";
      if (intTimerForDisplay % 60 < 10) {timerToDisplay += "0";}
      timerToDisplay += to_string(intTimerForDisplay % 60);
      timerDisplay.setString(timerToDisplay);
    }

    if (!gameStarted && enterKey) {
      gameStarted = true;
      preGameMessage.setString("");
    }

    if (gameStarted) {
      
      /*
        reglage du timer
      */

      timer -= dt;
      intTimerForDisplay = (int)(timer);


      if (previousTimer > timer) {
        timerToDisplay = to_string(intTimerForDisplay / 60);
        timerToDisplay += " : ";
        if (intTimerForDisplay % 60 < 10) {timerToDisplay += "0";}
        timerToDisplay += to_string(intTimerForDisplay % 60);
      }

      previousTimer = intTimerForDisplay;

      timerDisplay.setString(timerToDisplay);

      /*
        fin du reglage
      */

      if (collisionGrappleOnBubble) {
        spaceKey = false;
        collisionGrappleOnBubble = false;
      }

      bubbleLeft = 0;

      if (leftKey && player.x > 0 + LEFT_SIDE_MARGIN) {
        player.x -= pDistance;
      }

      if (rightKey && player.x + player.width < size.x) {
        player.x += pDistance;
      }

      if (spaceKey && grapple.y + grapple.height > size.y) {
        grapple.y -= gDistance;

        if (grapple.y + grapple.height < size.y)
          grapple.y = size.y - grapple.height;
      }

      if (spaceKey) {
        counterGrapple += dt;
        if (counterGrapple >= 1.0f)
          spaceKey = false;
      } else {
        counterGrapple = 0.0f;
        if (grapple.y < size.y) {
          grapple.y += gDistance;
        } else {
          grapple.x = player.x + player.width;
          grapple.y = size.y;
        }
      }

      for (int i = 0 ; i < NB_BUBBLES ; i++) {
        if (bubble[i].visible) {
          bubbleLeft++;

          if (bubble[i].x + bubble[i].radius * 2 >= size.x) {
            bubble[i].lastBoardTuchedX = true;
          } else if (bubble[i].x <= 0 + LEFT_SIDE_MARGIN) {
            bubble[i].lastBoardTuchedX = false;
          }

          if (bubble[i].lastBoardTuchedX) {
            bubble[i].x -= bDistance;
          } else {
            bubble[i].x += bDistance;
          }

          if (bubble[i].y + bubble[i].radius * 2 >= size.y) {
            bubble[i].lastBoardTuchedY = true;
          } else if (bubble[i].y <= 0) {
            bubble[i].lastBoardTuchedY = false;
          }

          if (bubble[i].lastBoardTuchedY) {
            bubble[i].y -= bDistance;
          } else {
            bubble[i].y += bDistance;
          }
        }
      }
    }

    //cout<<bubbleLeft<<endl;

    if (rectangleShapeCollisionWithBubbles(grapple, bubble, NB_BUBBLES, &bubbleHit)) {
      splitTheBubbles(bubble, NB_BUBBLES, bubbleHit, player.x + player.width / 2.0f, BUBBLE_SIZE_FACTOR);
      collisionGrappleOnBubble = true;
    }


    if (rectangleShapeCollisionWithBubbles(player, bubble, NB_BUBBLES, &bubbleHit)) {
      cout<<bubbleHit<<endl; //to delete
      gameStarted = false;

      endOfGame(bubble, NB_BUBBLES, player, grapple, false);
      
      return 0;
    }

    if (timer <= 0.0f && bubbleLeft > 0) {
      gameStarted = false;

      endOfGame(bubble, NB_BUBBLES, player, grapple, false);

      return 0;

    } else if (timer > 0.0f && bubbleLeft <= 0) {
      gameStarted = false;

      endOfGame(bubble, NB_BUBBLES, player, grapple, true);

      return 0;
    }

    window.clear(Color::White);
    window.draw(background);

    if (player.visible) {
      RectangleShape pShape;
      pShape.setSize({ player.width, player.height });
      pShape.setPosition(player.x, player.y);
      pShape.setTexture(&playerTexture);
      window.draw(pShape);
    }

    for (int i = 0 ; i < NB_BUBBLES ; i++) {
      if (bubble[i].visible) {
        CircleShape bShape;
        bShape.setRadius(bubble[i].radius);
        bShape.setPosition(bubble[i].x, bubble[i].y);
        bShape.setTexture(&bubbleTexture);
        window.draw(bShape);
      }
    }

    if (grapple.visible) {
      RectangleShape gShape;
      gShape.setSize({ grapple.width, grapple.height });
      gShape.setPosition(grapple.x, grapple.y);
      gShape.setTexture(&grappleTexture);
      window.draw(gShape);
    }

    if (!gameStarted) {
      window.draw(preGameMessage);
    }

    window.draw(timerDisplay);

    window.display();
  }

  return -1;
}

/*
  fonction main permettant d aller sur le menu
  de faire une partie ou de quitter le programme
*/

int main() {
  window.create(VideoMode(1280, 800), "Pang");
  window.setVerticalSyncEnabled(true);

  if (menu()) {
    game();
  }

  while (window.isOpen()) {

    switch (redirectWhenGameEnded()) {
      case 0:
        menu();
      break;

      case 1:
        game();
      break;

      case 2:
        window.close();
      break;

      default:
      break;
    }
  }

  return 0;
}
