/*
 * FishEnemyUnit.cpp

 *
 *  Created on: Oct 15, 2014
 *      Author: agustin
 */

#include "FishEnemyUnit.h"

#include <vector>

#include <common/debug/DebugUtil.h>



#define FEU_SPRITE_FILE     "./media/game/enemies/pez/pez.png"
#define FEU_ANIM_DIE        "muere"
#define FEU_ANIM_SWIM       "nada"



namespace game {

FishEnemyUnit::FishEnemyUnit()
{
    setSize(sf::Vector2f(0.2f, 0.1f));
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
    animFiles.push_back("./media/game/enemies/pez/" FEU_ANIM_DIE ".txt");
    animFiles.push_back("./media/game/enemies/pez/" FEU_ANIM_SWIM ".txt");
    if (!setTextureFromFile(FEU_SPRITE_FILE) ||
        !loadAnimsFromFiles(animFiles)) {
        debugERROR("Error loading FishEnemyUnit\n");
        return false;
    }

    // select the swim animation
    if (!m_animHandler.animExists(FEU_ANIM_DIE)) {
        debugERROR("The fish enemy hasn't the animations %s we need\n", FEU_ANIM_DIE);
        return false;
    }

    if (!m_animHandler.animExists(FEU_ANIM_SWIM)) {
        debugERROR("The fish enemy hasn't the animations %s we need\n", FEU_ANIM_SWIM);
        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
void
FishEnemyUnit::configure(const sf::Vector2f& beginPos,
                         MovingDir dir,
                         float velocitySecs)
{
    // configure the point mover
    sf::Vector2f dest(0.f, 0.f);
    if (dir == MovingDir::DIR_DOWN) {
        dest.y += 1.5f;
    } else if (dir == MovingDir::DIR_UP) {
        dest.y -= 1.5f;
    }
    dest += beginPos;
    setPosition(beginPos);
    m_mover.configure(beginPos, dest, velocitySecs);

    // reset life and state
    setLife(100);
    setCollisionsEnable(true);
    m_state = State::MOVING;

    // start animation
    m_animHandler.changeAnimation(FEU_ANIM_SWIM);
    m_animHandler.setAnimLoop(true);

    // we need to check here if we need to mirror or not
}

////////////////////////////////////////////////////////////////////////////////
bool
FishEnemyUnit::update(float timeFrame)
{
    // update the fish until we die or we go out from the window
    if (m_state == State::DEAD) {
        return false;
    }

    // update the animation
    m_animHandler.update(timeFrame);
    m_mover.update(timeFrame);

    // check if we are dying
    if (m_state == State::DYING) {
        // nothing to do, just wait
        return true;
    }

    // if not check if we just die?
    if (life() <= 0) {
        m_state = State::DYING;
        m_animHandler.changeAnimation(FEU_ANIM_DIE);
        m_animHandler.setAnimLoop(false);
        // disable collisions since we don't want to check this events anymore
        setCollisionsEnable(false);
        return true;
    }

    // check if we arrive the destination point and stop updating this
    if (m_mover.arrivedDestination()) {
        // finish
        return false;
    }

    // update the position of the enemy
    setPosition(m_mover.currentPosition());

    return true;
}

} /* namespace game */
