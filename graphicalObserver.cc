#include <iostream>
#include "graphicalObserver.h"
#include "window.h"
#include "cc3k.h"
#include <memory>

using namespace std;

GraphicalObserver::GraphicalObserver(CC3K *subject, int width, int height)
    : subject{subject}, width{width}, height{height}
{
  // Launch a new XWindow
  theWindow = make_shared<Xwindow>(SCALE * (width), SCALE * (height));
}

GraphicalObserver::~GraphicalObserver()
{
}

// Returns true if the char ch is an upper case char
bool isUpper(char ch)
{
  return (ch >= 'A' && ch <= 'Z');
}

// Returns true if the char ch is an lower case char
bool isLower(char ch)
{
  return (ch >= 'a' && ch <= 'z');
}

// Returns true if the char ch is a digit
bool isDigit(char ch)
{
  return (ch >= '0' && ch <= '9');
}

void GraphicalObserver::notify()
{

  int playerX = subject->getPlayer()->getX();
  int playerY = subject->getPlayer()->getY();

  for (int i = 0; i < height; ++i)
  {
    for (int j = 0; j < width; ++j)
    {
      char val = subject->getState(j, i).first;
      unsigned long col = Xwindow::White;

      // Bonus: Add fog
      // If the position is outside of the circle centered at the player, output fog
      // x^2 + 5y^2 > 48
      if (subject->getFog() && (playerX - j) * (playerX - j) + 5 * (playerY - i) * (playerY - i) > 48)
      {
        theWindow->drawImage(theWindow->wallImg, j * SCALE, i * SCALE);
      }
      else
      {
        if (val == Cell::WALL_HORIZONTAL || val == Cell::WALL_VERTICAL)
        {
          theWindow->drawImage(theWindow->wallImg, j * SCALE, i * SCALE);
        }
        else if (val == '@')
        {
          // Draw the floor

          theWindow->drawImage(theWindow->pcImg, j * SCALE, i * SCALE);
        }
        else if (val == Cell::PASSAGE)
        {
          col = Xwindow::Green;
          theWindow->drawImage(theWindow->passageImg, j * SCALE, i * SCALE);
        }
        else if (val == Cell::TILE)
        {
          theWindow->drawImage(theWindow->floorImg, j * SCALE, i * SCALE);
        }
        else if (val == Cell::DOOR)
        {
          col = Xwindow::Green;
          theWindow->drawImage(theWindow->doorImg, j * SCALE, i * SCALE);
        }
        else if (val == 'P')
        {
          theWindow->drawImage(theWindow->potionImg, j * SCALE, i * SCALE);
        }
        else if (val == 'G')
        {
          theWindow->drawImage(theWindow->goldImg, j * SCALE, i * SCALE);
        }
        else if (val == '\\')
        {
          theWindow->drawImage(theWindow->stairwayImg, j * SCALE, i * SCALE);
        }

        else if (val == ' ')
        {
          theWindow->drawImage(theWindow->spaceImg, j * SCALE, i * SCALE);

          // col = Xwindow::Black;
          // theWindow->fillRectangle(j * SCALE, i * SCALE, SCALE, SCALE, col);
        }
        else if (val == '@')
        {
          // theWindow->fillRectangle(j * SCALE, i * SCALE, SCALE, SCALE, col);
          theWindow->drawImage(theWindow->pcImg, j * SCALE, i * SCALE);
        }
        else if (val == 'M')
        {
          theWindow->drawImage(theWindow->merchantImg, j * SCALE, i * SCALE);
        }
        else if (val == 'O')
        {
          theWindow->drawImage(theWindow->orcImg, j * SCALE, i * SCALE);
        }
        else if (val == 'H')
        {
          theWindow->drawImage(theWindow->humanImg, j * SCALE, i * SCALE);
        }
        else if (val == 'L')
        {
          theWindow->drawImage(theWindow->halflingImg, j * SCALE, i * SCALE);
        }
        else if (val == 'D')
        {
          theWindow->drawImage(theWindow->dragonImg, j * SCALE, i * SCALE);
        }
        else if (val == 'E')
        {
          theWindow->drawImage(theWindow->elfImg, j * SCALE, i * SCALE);
        }
        else if (val == 'W')
        {
          theWindow->drawImage(theWindow->dwarfImg, j * SCALE, i * SCALE);
        }      
        else if (val == '!')
        {
          theWindow->drawImage(theWindow->pathfinderImg, j * SCALE, i * SCALE);
        }   
      }
    }
  }
}
