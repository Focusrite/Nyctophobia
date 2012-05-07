#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>

#include "Level.h"
#include "Object.h"
#include "Graphics.h"
#include "Gamestate.h"
#include <string>
#include "Wall.h"
#include "BasicLight.h"
#include "FileInput.h"
#include "Player.h"
#include "Terrain.h"
#include "Effects.h"


using namespace rapidxml;

int Level::mIdCounter = 0;

Level::Level()
{
	mInEditor = false;
}

Level::~Level()
{
	clear();
}

void Level::draw()
{
	for(int i = 0; i < mTerrainList.size(); i++)
	{
		if(!gGameState->drawingToAlpha())
		{
			gGraphics->getFXhandler()->setTerrainBlendMap(mTerrainList[i]->getBlendTexture());
			gGraphics->drawBlendedTexture(mTerrainList[i]->getTexture(), mWidth/2, mHeight/2, mWidth, mHeight, 0, NULL, gGraphics->getAlphaTexture()->d3dTex(), mWidth/40);
			gGraphics->getFXhandler()->setTerrainBlendMap(0);
		}
	}
	for(int i = 0; i < mObjectList.size(); i++)
	{
		//Only draw if right layer is set
		if(mObjectList[i]->getDrawToAlpha()==gGameState->drawingToAlpha())
			mObjectList[i]->draw();
	}
}

void Level::update(float dt)
{
	for(int i = 0; i < mObjectList.size(); i++)
	{
		Object* o = mObjectList[i];
		if(o->isDead()) {
			removeObject(o);
			continue;
		}
		o->update(dt);
	}
}

void Level::addObject(Object* object)
{
	object->setId(mIdCounter);
	object->setLevel(this);
	mObjectList.push_back(object);
}

void Level::removeObject(Object* object)
{
	for(int i = 0; i<mObjectList.size(); i++)
	{
		if(object->getId() == mObjectList.at(i)->getId())
			delete mObjectList.at(i);
			mObjectList.erase(mObjectList.begin()+i);
	}
}

void Level::clear()
{
	for(int i = 0; i < mObjectList.size(); i++)
	{
		delete mObjectList[i];
	}
	for(int i = 0; i < mTerrainList.size(); i++)
	{
		delete mTerrainList[i];
	}
}

void Level::loadLevel(const char* path)
{
	file<> file(path);
	xml_document<> doc;
	doc.parse<parse_full>(file.data());

	//Load objects
	xml_node<>* objects = doc.first_node("level")->first_node("objects");
	xml_node<>* cur_obj = objects->first_node("object");
	while(cur_obj != 0)
	{
		ObjectType type = (ObjectType) atoi(cur_obj->first_attribute("type")->value());
		if(type == WALL) 
		{
			Vector start=Vector(atoi(cur_obj->first_attribute("x")->value()), atoi(cur_obj->first_attribute("y")->value()));
			xml_node<>* data = cur_obj->first_node();
			Vector end=Vector(atoi(data->first_attribute("ex")->value()), atoi(data->first_attribute("ey")->value()));
			Wall* wall = new Wall(start,end);
			this->addObject(wall);
		}
		else if(type == LIGHT)
		{
			Vector pos=Vector(atoi(cur_obj->first_attribute("x")->value()), atoi(cur_obj->first_attribute("y")->value()));
			xml_node<>* data = cur_obj->first_node();
			int range = atoi(data->first_attribute("range")->value());
			float intensity = atof(data->first_attribute("intensity")->value());
			BasicLight* light = new BasicLight(pos.x,pos.y);
			light->setIntensity(intensity);
			light->setRange(range);
			this->addObject(light);
		}
		cur_obj = cur_obj->next_sibling();
	}
	
	//Load level info
	xml_node<>* levelData = doc.first_node("level")->first_node("levelData");
	mName= std::string(levelData->first_attribute("name")->value());
	mWidth= atoi(levelData->first_attribute("width")->value());
	mHeight= atoi(levelData->first_attribute("height")->value());

	//Load player - do from savefile
	xml_node<>* player = levelData->first_node("player");
	Player *p = new Player(atoi(player->first_attribute("x")->value()), atoi(player->first_attribute("y")->value()));
	addObject(p);

	//Load terrain
	xml_node<>* terrain = levelData->first_node("terrain");
	cur_obj = terrain->first_node("texture");
	while(cur_obj != 0)
	{
		Terrain *t = new Terrain();
		t->setTexture(gGraphics->loadTexture(cur_obj->first_attribute("texturePath")->value()));
		t->setBlendTexture(gGraphics->loadTexture(cur_obj->first_attribute("blendPath")->value()));
		mTerrainList.push_back(t);
		cur_obj = cur_obj->next_sibling();
	}
}