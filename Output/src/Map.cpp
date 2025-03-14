
#include "Engine.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Log.h"
#include "Physics.h"
#include "Player.h"
#include "Scene.h"

#include <math.h>

Map::Map() : Module(), mapLoaded(false)
{
    name = "map";
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake()
{
    name = "map";
    LOG("Loading Map Parser");

    return true;
}

bool Map::Start() {

    return true;
}

bool Map::Update(float dt)
{
    bool ret = true;

    if (mapLoaded) {

        // L07 TODO 5: Prepare the loop to draw all tiles in a layer + DrawTexture()
        // iterate all tiles in a layer
        
        for (const auto& mapLayer : mapData.layers) {
            if (mapLayer->properties.GetProperty("Draw") != NULL &&
                mapLayer->properties.GetProperty("Draw")->value == true) {

                for (int i = 0; i < mapData.width; i++) {
                    for (int j = 0; j < mapData.height; j++) {

                        int gid = mapLayer->Get(i, j);
                        if (gid != 0) {
                            TileSet* tileSet = GetTilesetFromTileId(gid);
                            if (tileSet != nullptr) {
                                SDL_Rect tileRect = tileSet->GetRect(gid);

                                // Convertir coordenadas del mapa a coordenadas de pantalla
                                Vector2D mapCoord = MapToWorld(i, j);

                                // Aplicar el efecto parallax en base a la c�mara
                                int renderX = (int)(mapCoord.getX() - (Engine::GetInstance().render->camera.x * mapLayer->parallaxX));
                                int renderY = (int)(mapCoord.getY() - (Engine::GetInstance().render->camera.y * mapLayer->parallaxY));

                                Engine::GetInstance().render->DrawTexture(tileSet->texture, renderX, renderY, &tileRect);
                            }
                        }
                    }
                }
            }
        }
        
    }

    return ret;
}

// L09: TODO 2: Implement function to the Tileset based on a tile id
TileSet* Map::GetTilesetFromTileId(int gid) const
{
	TileSet* set = nullptr;

    for (const auto& tileset : mapData.tilesets) {
    	if (gid >= tileset->firstGid && gid < (tileset->firstGid + tileset->tileCount)) {
			set = tileset;
			break;
		}
    }

    return set;
}

// Called before quitting
bool Map::CleanUp()
{
    LOG("Unloading map");

    DestroyColliders();

    // L06: TODO 2: Make sure you clean up any memory allocated from tilesets/map
    for (const auto& tileset : mapData.tilesets) {
        delete tileset;
    }
    mapData.tilesets.clear();

    // L07 TODO 2: clean up all layer data
    for (const auto& layer : mapData.layers)
    {
        delete layer;
    }
    mapData.layers.clear();

    return true;
}

// Load new map
bool Map::Load(std::string path, std::string fileName)
{
    bool ret = false;

    // Assigns the name of the map file and the path
    mapFileName = fileName;
    mapPath = path;
    std::string mapPathName = mapPath + mapFileName;

    pugi::xml_document mapFileXML;
    pugi::xml_parse_result result = mapFileXML.load_file(mapPathName.c_str());

    if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", mapPathName.c_str(), result.description());
		ret = false;
    }
    else {

        // L06: TODO 3: Implement LoadMap to load the map properties
        // retrieve the paremeters of the <map> node and store the into the mapData struct
        mapData.width = mapFileXML.child("map").attribute("width").as_int();
        mapData.height = mapFileXML.child("map").attribute("height").as_int();
        mapData.tileWidth = mapFileXML.child("map").attribute("tilewidth").as_int();
        mapData.tileHeight = mapFileXML.child("map").attribute("tileheight").as_int();

        // L06: TODO 4: Implement the LoadTileSet function to load the tileset properties
       
        //Iterate the Tileset
        for(pugi::xml_node tilesetNode = mapFileXML.child("map").child("tileset"); tilesetNode!=NULL; tilesetNode = tilesetNode.next_sibling("tileset"))
		{
            //Load Tileset attributes
			TileSet* tileSet = new TileSet();
            tileSet->firstGid = tilesetNode.attribute("firstgid").as_int();
            tileSet->name = tilesetNode.attribute("name").as_string();
            tileSet->tileWidth = tilesetNode.attribute("tilewidth").as_int();
            tileSet->tileHeight = tilesetNode.attribute("tileheight").as_int();
            tileSet->spacing = tilesetNode.attribute("spacing").as_int();
            tileSet->margin = tilesetNode.attribute("margin").as_int();
            tileSet->tileCount = tilesetNode.attribute("tilecount").as_int();
            tileSet->columns = tilesetNode.attribute("columns").as_int();

			//Load the tileset image
			std::string imgName = tilesetNode.child("image").attribute("source").as_string();
            tileSet->texture = Engine::GetInstance().textures->Load((mapPath+imgName).c_str());

			mapData.tilesets.push_back(tileSet);
		}

        // L07: TODO 3: Iterate all layers in the TMX and load each of them
        for (pugi::xml_node layerNode = mapFileXML.child("map").child("layer"); layerNode != NULL; layerNode = layerNode.next_sibling("layer")) {

            // L07: TODO 4: Implement the load of a single layer 
            //Load the attributes and saved in a new MapLayer
            MapLayer* mapLayer = new MapLayer();
            mapLayer->id = layerNode.attribute("id").as_int();
            mapLayer->name = layerNode.attribute("name").as_string();
            mapLayer->width = layerNode.attribute("width").as_int();
            mapLayer->height = layerNode.attribute("height").as_int();

            //L09: TODO 6 Call Load Layer Properties
            LoadProperties(layerNode, mapLayer->properties);

            //Iterate over all the tiles and assign the values in the data array
            for (pugi::xml_node tileNode = layerNode.child("data").child("tile"); tileNode != NULL; tileNode = tileNode.next_sibling("tile")) {
                mapLayer->tiles.push_back(tileNode.attribute("gid").as_int());
            }

            // Dentro del bucle que itera sobre cada capa en Load()
            if (layerNode.attribute("parallaxx"))
                mapLayer->parallaxX = layerNode.attribute("parallaxx").as_float();
            else
                mapLayer->parallaxX = 1.0f; // Valor por defecto

            if (layerNode.attribute("parallaxy"))
                mapLayer->parallaxY = layerNode.attribute("parallaxy").as_float();
            else
                mapLayer->parallaxY = 1.0f; // Valor por defecto

            //add the layer to the map
            mapData.layers.push_back(mapLayer);
        }

        // L08 TODO 3: Create colliders
        // L08 TODO 7: Assign collider type
        // Later you can create a function here to load and create the colliders from the map
            for (const auto& mapLayer : mapData.layers) {
                //Check if the property Draw exist get the value, if it's true draw the lawyer
                if (mapLayer->properties.GetProperty("Collisions") != NULL && mapLayer->properties.GetProperty("Collisions")->value == true) {
                    for (int i = 0; i < mapData.width; i++) {
                        for (int j = 0; j < mapData.height; j++) {

                            // L07 TODO 9: Complete the draw function
                            //Get the gid from tile
                            int gid = mapLayer->Get(i, j);
                            //Check if the gid is different from 0 - some tiles are empty
                            if (gid == 1957) {
                                Vector2D mapCoord = MapToWorld(i, j);
                                PhysBody* platform = Engine::GetInstance().physics.get()->CreateRectangle(mapCoord.getX() + mapData.tileWidth / 2, mapCoord.getY() + mapData.tileHeight - 25, mapData.tileWidth, mapData.tileHeight / 2, STATIC);
                                platform->ctype = ColliderType::PLATFORM;
                                colliders.push_back(platform);
                            }
                        }
                    }
                }
            }

            for (const auto& mapLayer : mapData.layers) {
                //Check if the property Draw exist get the value, if it's true draw the lawyer
                if (mapLayer->properties.GetProperty("Collisions") != NULL && mapLayer->properties.GetProperty("Collisions")->value == true) {
                    for (int i = 0; i < mapData.width; i++) {
                        for (int j = 0; j < mapData.height; j++) {

                            // L07 TODO 9: Complete the draw function
                            //Get the gid from tile
                            int gid = mapLayer->Get(i, j);
                            //Check if the gid is different from 0 - some tiles are empty
                            if (gid == 1958) {
                                Vector2D mapCoord = MapToWorld(i, j);
                                PhysBody* platform = Engine::GetInstance().physics.get()->CreateRectangle(mapCoord.getX() + mapData.tileWidth / 2, mapCoord.getY() + mapData.tileHeight, mapData.tileWidth, mapData.tileHeight, STATIC);
                                platform->ctype = ColliderType::DEATH;
                                colliders.push_back(platform);
                            }
                        }
                    }
                }
            }
            for (const auto& mapLayer : mapData.layers) {
                //Check if the property Draw exist get the value, if it's true draw the lawyer
                if (mapLayer->properties.GetProperty("NewLvl") != NULL && mapLayer->properties.GetProperty("NewLvl")->value == true) {
                    for (int i = 0; i < mapData.width; i++) {
                        for (int j = 0; j < mapData.height; j++) {

                            // L07 TODO 9: Complete the draw function
                            //Get the gid from tile
                            int gid = mapLayer->Get(i, j);
                            //Check if the gid is different from 0 - some tiles are empty
                            if (gid == 3036) {
                                Vector2D mapCoord = MapToWorld(i, j);
                                PhysBody* platform = Engine::GetInstance().physics.get()->CreateRectangleSensor(mapCoord.getX() + mapData.tileWidth / 2, mapCoord.getY() + mapData.tileHeight, mapData.tileWidth, mapData.tileHeight, STATIC);
                                platform->ctype = ColliderType::NEWLVL;
                                colliders.push_back(platform);

                            }
                        }
                    }
                }
            }
            for (const auto& mapLayer : mapData.layers) {
                //Check if the property Draw exist get the value, if it's true draw the lawyer
                if (mapLayer->properties.GetProperty("Checkpoint") != NULL && mapLayer->properties.GetProperty("Checkpoint")->value == true) {
                    for (int i = 0; i < mapData.width; i++) {
                        for (int j = 0; j < mapData.height; j++) {

                            // L07 TODO 9: Complete the draw function
                            //Get the gid from tile
                            int gid = mapLayer->Get(i, j);
                            //Check if the gid is different from 0 - some tiles are empty
                            if (gid == 3035) {
                                Vector2D mapCoord = MapToWorld(i, j);
                                PhysBody* platform = Engine::GetInstance().physics.get()->CreateRectangleSensor(mapCoord.getX() + mapData.tileWidth / 2, mapCoord.getY() + mapData.tileHeight, mapData.tileWidth, mapData.tileHeight, KINEMATIC);
                                platform->ctype = ColliderType::CHECKPOINT;
                                colliders.push_back(platform);
                            }
                        }
                    }
                }
            }
            for (const auto& mapLayer : mapData.layers) {
                //Check if the property Draw exist get the value, if it's true draw the lawyer
                if (mapLayer->properties.GetProperty("FinalBoss") != NULL && mapLayer->properties.GetProperty("FinalBoss")->value == true) {
                    for (int i = 0; i < mapData.width; i++) {
                        for (int j = 0; j < mapData.height; j++) {

                            // L07 TODO 9: Complete the draw function
                            //Get the gid from tile
                            int gid = mapLayer->Get(i, j);
                            //Check if the gid is different from 0 - some tiles are empty
                            if (gid == 1958) {
                                Vector2D mapCoord = MapToWorld(i, j);
                                PhysBody* platform = Engine::GetInstance().physics.get()->CreateRectangleSensor(mapCoord.getX() + mapData.tileWidth / 2, mapCoord.getY() + mapData.tileHeight, mapData.tileWidth, mapData.tileHeight, KINEMATIC);
                                platform->ctype = ColliderType::FINALBOSS;
                                colliders.push_back(platform);
                            }
                        }
                    }
                }
            }
        
        ret = true;

        // L06: TODO 5: LOG all the data loaded iterate all tilesetsand LOG everything
        if (ret == true)
        {
            LOG("Successfully parsed map XML file :%s", fileName.c_str());
            LOG("width : %d height : %d", mapData.width, mapData.height);
            LOG("tile_width : %d tile_height : %d", mapData.tileWidth, mapData.tileHeight);

            LOG("Tilesets----");

            //iterate the tilesets
            for (const auto& tileset : mapData.tilesets) {
                LOG("name : %s firstgid : %d", tileset->name.c_str(), tileset->firstGid);
                LOG("tile width : %d tile height : %d", tileset->tileWidth, tileset->tileHeight);
                LOG("spacing : %d margin : %d", tileset->spacing, tileset->margin);
            }
            			
            LOG("Layers----");

            for (const auto& layer : mapData.layers) {
                LOG("id : %d name : %s", layer->id, layer->name.c_str());
				LOG("Layer width : %d Layer height : %d", layer->width, layer->height);
            }   
        }
        else {
            LOG("Error while parsing map file: %s", mapPathName.c_str());
        }

        if (mapFileXML) mapFileXML.reset();

    }

    mapLoaded = ret;
    return ret;
}

// L07: TODO 8: Create a method that translates x,y coordinates from map positions to world positions
Vector2D Map::MapToWorld(int x, int y) const
{
    Vector2D ret;

    ret.setX(x * mapData.tileWidth);
    ret.setY(y * mapData.tileHeight);

    return ret;
}

// L10: TODO 5: Add method WorldToMap to obtain  map coordinates from screen coordinates 
Vector2D Map::WorldToMap(int x, int y) {

    Vector2D ret(0, 0);

    ret.setX(x / mapData.tileWidth);
    ret.setY(y / mapData.tileHeight);

    return ret;
}

// L09: TODO 6: Load a group of properties from a node and fill a list with it
bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
    bool ret = false;

    for (pugi::xml_node propertieNode = node.child("properties").child("property"); propertieNode; propertieNode = propertieNode.next_sibling("property"))
    {
        Properties::Property* p = new Properties::Property();
        p->name = propertieNode.attribute("name").as_string();
        p->value = propertieNode.attribute("value").as_bool(); // (!!) I'm assuming that all values are bool !!

        properties.propertyList.push_back(p);
    }

    return ret;
}

MapLayer* Map::GetNavigationLayer() {
    for (const auto& layer : mapData.layers) {
        if (layer->properties.GetProperty("Navigation") != NULL &&
            layer->properties.GetProperty("Navigation")->value) {
            return layer;
        }
    }

    return nullptr;
}

// L09: TODO 7: Implement a method to get the value of a custom property
Properties::Property* Properties::GetProperty(const char* name)
{
    for (const auto& property : propertyList) {
        if (property->name == name) {
			return property;
		}
    }

    return nullptr;
}

void Map::DestroyColliders()
{
    for (int i = 0; i < colliders.size(); i++)
    {
        Engine::GetInstance().physics->DeletePhysBody(colliders[i]);
    }
}




