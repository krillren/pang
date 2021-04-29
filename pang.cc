#include <iostream>
#include <random>
#include <cmath>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

const int LEFT_SIDE_MARGIN = 0;
const int NB_BUBBLES = 3 * 4;

const float BUBBLE_SIZE_FACTOR = 30.0f;
const float PLAYER_SPEED = 500.0f;
const float GRAPPLE_SPEED = 850.0f;

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
  float radius;
  bool visible = false;
  float Vy;
  float Vx;
};

RenderWindow window;

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

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
  
  /************************************************************************************************************************************************************************************/

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

  /************************************************************************************************************************************************************************************/

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

  /************************************************************************************************************************************************************************************/

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

    window.clear();
    window.draw(redirectBackground);

    for (int i = 0 ; i < NB_BUTTONS ; i++) {
      RectangleShape cbShape;
      cbShape.setSize({ redirectButton[i].width, redirectButton[i].height });
      cbShape.setPosition(redirectButton[i].x, redirectButton[i].y);
      (highlightedButton == i) ? cbShape.setTexture(&redirectButtonHighlightedTexture) : cbShape.setTexture(&redirectButtonTexture);
      window.draw(cbShape);
    }

    window.draw(menu);
    window.draw(playAgain);
    window.draw(exitGame);

    window.display();
    
  }

  return -1;
}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

int menu() {
  Vector2u size = window.getSize();

  Rectangle startButton;
  startButton.width = size.x / 3.0f;
  startButton.height = size.y / 4.0f;
  startButton.x = (size.x - startButton.width) / 2.0f;
  startButton.y = (size.y - startButton.height) / 2.0f;
  startButton.visible = true;

  /************************************************************************************************************************************************************************************/

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

  /************************************************************************************************************************************************************************************/

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

    window.clear();
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

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

int endOfGame(Circle bubble[], int NB_BUBBLES, Rectangle player, Rectangle grapple, bool win, int reasonOfGameOver) {
  Vector2u size = window.getSize();
  
  for (int i = 0 ; i < NB_BUBBLES ; i++) {
    bubble[i].visible = false;
  }

  player.visible = false;

  grapple.visible = false;

  /************************************************************************************************************************************************************************************/

  Texture gameOverBgdTexture;
  gameOverBgdTexture.setSmooth(true);

  switch (reasonOfGameOver) {
    case 0:
      if (!gameOverBgdTexture.loadFromFile("resources/images/gameOverWinBackground.png")) {
        cout<<"loading failure"<<endl;
      return 1;
      }
      
    break;

    case 1:
      if (!gameOverBgdTexture.loadFromFile("resources/images/gameOverCollisionBackground.png")) {
        cout<<"loading failure"<<endl;
      return 1;
      }
    break;

    case 2:
      if (!gameOverBgdTexture.loadFromFile("resources/images/gameOverTimeOutBackground.png")) {
        cout<<"loading failure"<<endl;
      return 1;
      }
    break;

    default:
    break;
  }

  Sprite gameOverBackground;
  Vector2u bgdSize = gameOverBgdTexture.getSize();
  gameOverBackground.setTexture(gameOverBgdTexture);
  gameOverBackground.setOrigin(size.x / bgdSize.x - 1.0f, size.y / bgdSize.y - 1.0f);

  /************************************************************************************************************************************************************************************/

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

    window.clear();
    window.draw(gameOverBackground);

    window.display();

    if (animationDuration >= 3.0f) {
      return 0;
    }
    
  }

  return -1;
}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

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

float clamp(float value, float min, float max){
  if (value <= min) {
    return min;
  } else if (value >= max) {
    return max;
  } else {
    return value;
  }
}

bool rectangleCollisionOneBubble(Rectangle entity, Circle bubble){
  /*
  float closestX = clamp(bubble.x, entity.x, entity.x + entity.width);
  float closestY = clamp(bubble.y, entity.y, entity.y + entity.height);

  float distanceX = bubble.x - closestX;
  float distanceY = bubble.y - closestY;

  float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);
  return distanceSquared < (bubble.radius * bubble.radius);
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

void allocationOfNewBubbles(int start, int end, Circle bubble[], int newState, int grapplePosX, Vector2u size, float posBubble) {
  for (int i = start ; i <= end ; i++) {
    bubble[i].state = newState;
    bubble[i].radius = bubble[i].state * BUBBLE_SIZE_FACTOR + 10;
    bubble[i].x = (i % 2 == 0) ? grapplePosX - bubble[i].radius : grapplePosX + bubble[i].radius;
    bubble[i].y = posBubble;
    if (i == start) {
      bubble[i].Vx = (i % 2 == 0) ? -1.0f : 1.0f;
    } else {
      bubble[i].Vx = (bubble[start].Vx == 1.0f) ? -1.0f : 1.0f;
    }
    bubble[i].Vy = 0.0f;
    bubble[i].visible = true;
  }
}

void splitTheBubbles(Circle bubble[], int bubbleHit, int grapplePosX) {
  Vector2u size = window.getSize();

  switch (bubble[bubbleHit].state) {
    case 3:
      allocationOfNewBubbles(bubbleHit, bubbleHit + 1, bubble, 2, grapplePosX, size, bubble[bubbleHit].y);
      bubble[bubbleHit + 2].state = -1;
      bubble[bubbleHit + 3].state = -1;
    break;

    case 2:
      if (bubbleHit % 4 == 0 && bubble[bubbleHit + 2].state == -1) {
        allocationOfNewBubbles(bubbleHit + 2, bubbleHit + 3, bubble, 1, grapplePosX, size, bubble[bubbleHit].y);
        bubble[bubbleHit].visible = false;
      } else if (bubbleHit % 4 == 0 && bubble[bubbleHit + 2].state != -1) {
        allocationOfNewBubbles(bubbleHit, bubbleHit + 1, bubble, 1, grapplePosX, size, bubble[bubbleHit].y);
      } else if (bubbleHit % 4 != 0 && bubble[bubbleHit + 2].state == -1) {
        bubble[bubbleHit].visible = false;
        allocationOfNewBubbles(bubbleHit + 1, bubbleHit + 2, bubble, 1, grapplePosX, size, bubble[bubbleHit].y);
      } else if (bubbleHit % 4 != 0 && bubble[bubbleHit + 2].state != -1) {
        allocationOfNewBubbles(bubbleHit - 1, bubbleHit, bubble, 1, grapplePosX, size, bubble[bubbleHit].y);
      }
    break;

    case 1:
      bubble[bubbleHit].visible = false;
      bubble[bubbleHit].state = 0;
    break;

    default:
      return;
    break;
  }
}

string setTimer(float timer) {
  int intTimerForDisplay = (int)(timer);
  string timerToDisplay = "";

  timerToDisplay = to_string(intTimerForDisplay / 60);
  timerToDisplay += " : ";
  if (intTimerForDisplay % 60 < 10) {timerToDisplay += "0";}
  timerToDisplay += to_string(intTimerForDisplay % 60);

  return timerToDisplay;
}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

int game() {
  Vector2u size = window.getSize();

  Rectangle player;
  player.width = size.x / 16.0f;
  player.height = size.y / 5.0f;
  player.x = (size.x - player.width) / 2.0f + LEFT_SIDE_MARGIN;
  player.y = size.y - player.height;
  player.visible = true;
  
  Rectangle grapple;
  grapple.width = size.x / 100.0f;
  grapple.height = size.y;
  grapple.x = player.x + player.width + LEFT_SIDE_MARGIN;
  grapple.y = size.y + 1.0f;
  grapple.visible = true;

  Circle bubble[NB_BUBBLES];

  for (int i = 0 ; i < NB_BUBBLES ; ++i) {
    if (i % 4 == 0) {
      do {
        bubble[i].state = 3;
        bubble[i].Vx = (Math::random() < 0.5f) ? 1.0f : -1.0f;
        bubble[i].Vy = 0;
        bubble[i].radius = bubble[i].state * BUBBLE_SIZE_FACTOR + 10;
        bubble[i].x = bubble[i].radius + Math::random() * size.x + LEFT_SIDE_MARGIN;
        bubble[i].y = bubble[i].radius + Math::random() * size.y;
        bubble[i].visible = true;

      } while (bubble[i].x > size.x - bubble[i].radius ||
               bubble[i].y > size.y / 2 ||
               bubbleCollisionBubbles(bubble, i));
    }
  }

  /************************************************************************************************************************************************************************************/

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

  /************************************************************************************************************************************************************************************/

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

  /************************************************************************************************************************************************************************************/

  bool leftKey = false;
  bool rightKey = false;
  bool spaceKey = false;
  bool enterKey = false;
  
  bool gameStarted = false;
  bool collisionGrappleOnBubble = false;
  bool topTuched = false;

  int bubbleLeft = NB_BUBBLES;
  int bubbleHit = -1;

  float timer = 100.0f;
  float endCounter = 0.0f;

  float gravity = 700.0f;

  Clock clock;

  while (window.isOpen()) {

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

          case Keyboard::Space:
            spaceKey = false;
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

    float pDistance = PLAYER_SPEED * dt;
    float gDistance = GRAPPLE_SPEED * dt;
    
    if (!gameStarted) {
      timerDisplay.setString(setTimer(timer));
    }

    if (!gameStarted && enterKey) {
      gameStarted = true;
      preGameMessage.setString("");
    }

    if (gameStarted) {
      
      timer -= dt;

      timerDisplay.setString(setTimer(timer));

      if (rectangleShapeCollisionWithBubbles(player, bubble, NB_BUBBLES, &bubbleHit)) {
        gameStarted = false;

        endOfGame(bubble, NB_BUBBLES, player, grapple, false, 1);
      
        return 0;
      }

      if (rectangleShapeCollisionWithBubbles(grapple, bubble, NB_BUBBLES, &bubbleHit)) {
        splitTheBubbles(bubble, bubbleHit, grapple.x + grapple.width / 2.0f);
        collisionGrappleOnBubble = true;
      }

      if (grapple.y <= 0.0f || collisionGrappleOnBubble) {
        grapple.x = player.x + player.width / 2.0f + grapple.width / 2.0f;
        grapple.y = size.y + 1;
        collisionGrappleOnBubble = false;
        topTuched = false;
      }

      if (leftKey && player.x > 0.0f + LEFT_SIDE_MARGIN) {
        player.x -= pDistance;
      }

      if (rightKey && player.x + player.width < size.x) {
        player.x += pDistance;
      }

      if (spaceKey || topTuched) {
        grapple.y -= gDistance;
        topTuched = true;
      } else {
        grapple.x = player.x + player.width / 2.0f + grapple.width / 2.0f;
      }

      bubbleLeft = 0;

      for (int i = 0 ; i < NB_BUBBLES ; i++) {
        if (bubble[i].visible) {
          bubbleLeft++;
          
          if (bubble[i].x + bubble[i].radius > size.x || bubble[i].x - bubble[i].radius < 0 + LEFT_SIDE_MARGIN) {
            
            bubble[i].Vx = -bubble[i].Vx;
          }

          bubble[i].x = bubble[i].x + bubble[i].Vx;
          bubble[i].Vy = bubble[i].Vy + gravity * dt;

          if ((bubble[i].y + bubble[i].Vy * dt) + bubble[i].radius > size.y || (bubble[i].y + bubble[i].Vy * dt) - bubble[i].radius < 0) {

            bubble[i].Vy = - bubble[i].Vy;
            bubble[i].Vy = bubble[i].Vy + gravity * dt;
            bubble[i].y = bubble[i].y + bubble[i].Vy * dt;

          } else {

            bubble[i].y = bubble[i].y + bubble[i].Vy * dt;

          }

          
  
        }
      }
    }

    if (timer <= 0.0f && bubbleLeft > 0) {
      gameStarted = false;

      endCounter += dt;

      if (endCounter >= 1.0f) {

        endOfGame(bubble, NB_BUBBLES, player, grapple, false, 2);

        return 0;
      }

    } else if (timer > 0.0f && bubbleLeft <= 0) {
      gameStarted = false;

      endCounter += dt;

      if (endCounter >= 1.0f) {

        endOfGame(bubble, NB_BUBBLES, player, grapple, true, 0);

        return 0;
      }
    }

    window.clear();
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
        bShape.setOrigin(bubble[i].radius, bubble[i].radius);
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

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

int main() {
  window.create(VideoMode(1280, 800), "Pang.V.alpha0.2");
  window.setVerticalSyncEnabled(true);

  if (menu()) {
    game();
  }

  while (window.isOpen()) {

    switch (redirectWhenGameEnded()) {
      case 0:
        if (menu()) {
          game();
        }
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
