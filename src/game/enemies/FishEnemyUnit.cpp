/*
 * FishEnemyUnit.cpp
 *
 *  Created on: Oct 15, 2014
 *      Author: agustin
 */

#include <vector>

#include <common/debug/DebugUtil.h>

#include <game/enemies/FishEnemyUnit.h>


#define FEU_SPRITE_FILE     "pez.png"
#define FEU_ANIM_DIE        "muere"
#define FEU_ANIM_SWIM       "nada"



namespace game {

FishEnemyUnit::FishEnemyUnit()
{
    // TODO Auto-generated constructor stub

}

FishEnemyUnit::~FishEnemyUnit()
{
    // TODO Auto-generated destructor stub
}

////////////////////////////////////////////////////////////////////////////////
bool
FishEnemyUnit::init(void)
{
    // we will try to load all the animations we need for this
    std::vector<std::string> animFiles;
    animFiles.push_back(FEU_ANIM_DIE ".txt");
    animFiles.push_back(FEU_ANIM_SWIM ".txt");
    if (!loadSpriteFromFile(FEU_SPRITE_FILE) ||
        !loadAnimsFromFiles(animFiles)) {
        debugERROR("Error loading FishEnemyUnit\n");
    }

    // select the swim animation
    if (!m_animHandler.animExists(FEU_ANIM_DIE) ||
        !m_animHandler.animExists(FEU_ANIM_SWIM)) {
        debugERROR("The fish enemy hasn't the animations we need\n");
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool
FishEnemyUnit::update(float timeFrame)
{
    GameEntity::update(timeFrame);

    // check if we are dead or we are outside of the screen
    if (!isAlive()) {
        return false;
    }

    return true;
}

} /* namespace game */
