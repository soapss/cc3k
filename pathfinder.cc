#include "pathfinder.h"
#include "enemy.h"
#include <iostream>
#include <cmath>
#include <queue>
#include <map>

using namespace std;

Pathfinder::Pathfinder() : Enemy{15, 5, 5, true, '!', "Pathfinder"} {}

void Pathfinder::move(int dx, int dy)
{
    Enemy::move(dx,dy);
}

std::pair<int, int> Pathfinder::bfs(CC3K &game, int enemyX, int enemyY)
{
    // Get player X and Y coordinates
    int playerX = game.getPlayer()->getX();
    int playerY = game.getPlayer()->getY();

    pair<int,int> start = make_pair(enemyX,enemyY);
    pair<int,int> destination = make_pair(playerX, playerY);

    // Queue of positions to visit
    queue<map<std::string,pair<int,int>>> q;
    q.push({{"current", start}, {"last", make_pair(-1,-1)}});

    std::map<pair<int,int>, pair<int,int>> visited;

    while (!q.empty())
    {
        map<std::string,pair<int,int>> cur = q.front();
        q.pop();


        if (!(cur["current"].first == start.first  && cur["current"].second == start.second) &&
           (!(cur["current"].first == destination.first && cur["current"].second == destination.second)) &&
            game.isOccupiedOrNotChamber(cur["current"].first, cur["current"].second))
        {
            continue;
        }



        if (visited.count(cur["current"]) != 0)
        {
            continue;
        }


        // Mark as visited
        visited[cur["current"]] = cur["last"];

        int curX = cur["current"].first;
        int curY = cur["current"].second;

        // If we found the destination
        if (curX == destination.first && curY == destination.second)
        {
            break;
        }


        // Otherwise, search the neighbours

        // north
        q.push({{"current", make_pair(curX, curY-1)}, {"last", cur["current"] }});
        // south
        q.push({{"current", make_pair(curX, curY+1)}, {"last", cur["current"] }});
        // west
        q.push({{"current", make_pair(curX-1, curY)}, {"last", cur["current"] }});
        // east
        q.push({{"current", make_pair(curX+1, curY)}, {"last", cur["current"] }});

        // northwest
        q.push({{"current", make_pair(curX-1, curY-1)}, {"last", cur["current"] }});
        // northeast
        q.push({{"current", make_pair(curX+1, curY-1)}, {"last", cur["current"] }});
        // southeast
        q.push({{"current", make_pair(curX-1, curY+1)}, {"last", cur["current"] }});
        // southwest
        q.push({{"current", make_pair(curX+1, curY+1)}, {"last", cur["current"] }});

        


    }

    // Walk the path back to obtain the original move
    vector<pair<int,int>> backtracedPath;

    pair<int,int> cur = destination;

    while (visited.count(cur) != 0 && visited[cur].first != -1)
    {
        if (cur.first != -1)
        {
            backtracedPath.push_back(cur);
        }
        cur = visited[cur];
    }
    if (backtracedPath.size() == 0)
    {

        return make_pair(enemyX,enemyY);
    }
    return backtracedPath.back();



}
