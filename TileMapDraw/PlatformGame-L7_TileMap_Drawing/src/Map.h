#pragma once

#include "Module.h"
#include <list>
#include <vector>

struct MapLayer
{
    // L07: TODO 1: Add the info to the MapLayer Struct

    int id;
    std::string name;
    int width;
    int height;

    std::vector<int>tiles;

    // L07: TODO 6: Short function to get the gid value of i,j
    unsigned int Get(int i, int j)
    {
        return(tiles[(j * width) + i]);
    }
};

// L06: TODO 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!


struct TileSet
{
    int firstGid;
    std::string name;
    int tileWidth;
    int tileHeight;
    int spacing;
    int margin;
    int tileCount;
    int columns;
    SDL_Texture* texture;

    // L07: TODO 7: Implement the method that receives the gid and returns a Rect
    SDL_Rect GetRect(unsigned int gid) {
        SDL_Rect rect = { 0 };
           
        int relativeIndex = gid - firstGid;

        rect.w = tileWidth;
        rect.h = tileHeight;
        rect.x = (relativeIndex % columns)*(tileWidth + spacing) + margin;
        rect.y = (relativeIndex / columns) * (tileHeight + spacing) + margin;

        return rect;
    }

};

// L06: TODO 1: Create a struct needed to hold the information to Map node
struct MapData
{
	int width;
	int height;
	int tileWidth;
	int tileHeight;
    std::list<TileSet*> tilesets;

    // L07: TODO 2: declare a list of MapLayer  in the MapData struct
    std::list<MapLayer*> layers;
};

class Map : public Module
{
public:

    Map();

    // Destructor
    virtual ~Map();

    // Called before render is available
    bool Awake();

    // Called before the first frame
    bool Start();

    // Called each loop iteration
    bool Update(float dt);

    // Called before quitting
    bool CleanUp();

    // Load new map
    bool Load(std::string path, std::string mapFileName);

    // L07: TODO 8: Create a method that translates x,y coordinates from map positions to world positions
    Vector2D MapToWorld(int x, int y);

public: 
    std::string mapFileName;
    std::string mapPath;

private:
    bool mapLoaded;
    // L06: DONE 1: Declare a variable data of the struct MapData
    MapData mapData;
};