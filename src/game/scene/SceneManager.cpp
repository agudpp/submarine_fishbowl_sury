/*
 * SceneManager.cpp
 *
 *  Created on: Oct 30, 2014
 *      Author: agustin
 */

#include "SceneManager.h"

#include <game/enemies/FishEnemyUnit.h>
#include <game/elements/BombElement.h>


#define EXPLOSION_EFFECT_COUNT      3
#define COUNTER_EFFECT_COUNT        3
#define COUNtER_ENEMIES_FISHES      12
#define COUNTER_ELEMENTS_MINES      12

// update the size macro
#define UPDATE_SIZE(so)\
    {\
    const sf::Vector2f screenSize = sceneToScreenPos(so->sceneSize());\
    so->shape().setSize(screenSize);\
    }

namespace game {

////////////////////////////////////////////////////////////////////////////
bool
SceneManager::initEffects(void)
{
    // here we will create all the effects we thing we will use

    // for now we will create 3 explosions?
    Effect::EffectType et = Effect::EffectType::ET_EXPLOSION;
    for (unsigned int i = 0; i < EXPLOSION_EFFECT_COUNT; ++i) {
        Effect* effect = new Effect(et);
        if (!effect->setTextureFromFile("./media/game/effects/explosion/explosion.png")) {
            debugERROR("Error loading texture for explosions\n");
            return false;
        }
        if (!effect->init("./media/game/effects/explosion/explosion.txt")) {
            debugERROR("Error loading the animations for the explosions\n");
            return false;
        }
        effect->setSize(sf::Vector2f(0.3, 0.2));
        // update the size
        UPDATE_SIZE(effect);
        m_effectQueues[et].push(effect);
    }

    ////////////////////////////////////////////////////////////////////////////
    // for now we will create 3 explosions?
    et = Effect::EffectType::ET_3SEC_COUNTER;
    for (unsigned int i = 0; i < COUNTER_EFFECT_COUNT; ++i) {
        Effect* effect = new Effect(et);
        if (!effect->setTextureFromFile("./media/game/effects/conteo/conteo.png")) {
            debugERROR("Error loading texture for conteo effect\n");
            return false;
        }
        if (!effect->init("./media/game/effects/conteo/conteo.txt")) {
            debugERROR("Error loading the animations for the conteo effect\n");
            return false;
        }
        effect->setSize(sf::Vector2f(0.3, 0.2));
        UPDATE_SIZE(effect);
        m_effectQueues[et].push(effect);
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////
bool
SceneManager::initEnemies(void)
{
    EnemyUnit::EnemyType et = EnemyUnit::EnemyType::FISH;
    for (unsigned int i = 0; i < COUNtER_ENEMIES_FISHES; ++i) {
        FishEnemyUnit* enemy = new FishEnemyUnit();
        if (!enemy->init()) {
            debugERROR("Error initializing the fish enemies\n");
            return false;
        }
        UPDATE_SIZE(enemy);
        m_enemiesQueues[et].push(enemy);
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////
bool
SceneManager::initElements(void)
{
    ElementObject::Type et = ElementObject::Type::EOT_MINE;
    for (unsigned int i = 0; i < COUNTER_ELEMENTS_MINES; ++i) {
        BombElement* bomb = new BombElement();
        if (!bomb->init()) {
            debugERROR("Error initializing the bomb element\n");
            return false;
        }
        UPDATE_SIZE(bomb);
        m_elementObjQueues[et].push(bomb);
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////
void
SceneManager::updateEffects(float timeFrame)
{
    // lets update it and remove if we don't will need anymore the effect
    for (unsigned int i = 0; i < m_activeEffects.size(); ++i) {
        Effect* effect = m_activeEffects[i];
        if (!effect->update(timeFrame)) {
            // we need to remove this
            common::Helper::remAndSwapIfExists(effect, m_activeEffects);
            --i;

            // put it again in the correct queue
            ASSERT(effect->effectType() < Effect::EffectType::ET_MAX);
            m_effectQueues[effect->effectType()].push(effect);
            removeSceneObject(effect);
        }
    }
}

////////////////////////////////////////////////////////////////////////////
void
SceneManager::updateEnemies(float timeFrame)
{
    // update all the enemies
    for (unsigned int i = 0; i < m_enemies.size(); ++i) {
        EnemyUnit* enemy = m_enemies[i];
        if (!enemy->update(timeFrame)) {
            // we need to remove this
            common::Helper::remAndSwapIfExists(enemy, m_enemies);
            --i;

            // put it again in the correct queue
            ASSERT(enemy->enemyType() < EnemyUnit::EnemyType::COUNT);
            m_enemiesQueues[enemy->enemyType()].push(enemy);
            removeSceneObject(enemy);
        }
    }

}

////////////////////////////////////////////////////////////////////////////
void
SceneManager::updateElements(float timeFrame)
{
    for (unsigned int i = 0; i < m_elementObjs.size(); ++i) {
        ElementObject* element = m_elementObjs[i];
        if (!element->update(timeFrame)) {
            // we need to remove this
            common::Helper::remAndSwapIfExists(element, m_elementObjs);
            --i;

            // put it again in the correct queue
            ASSERT(element->elementType() < ElementObject::Type::EOT_COUNT);
            m_elementObjQueues[element->elementType()].push(element);
            removeSceneObject(element);
        }
    }
}

////////////////////////////////////////////////////////////////////////////
void
SceneManager::updateExternals(float timeFrame)
{
    for (unsigned int i = 0; i < m_externals.size(); ++i) {
        if (!m_externals[i]->update(timeFrame)) {
            // remove this
            common::Helper::remAndSwapElem(i, m_externals);
            --i;
            removeSceneObject(m_externals[i]);
        }
    }
}

////////////////////////////////////////////////////////////////////////////
void
SceneManager::performCollisions(void)
{
    // since we have everything hardcoded in this ugly shit design we will
    // go in deep with the shit here
    // we want to detect:
    // 1) Player against all enemies.
    // 2) Player against all elements
    // 3) Elements against all enemies.

    ASSERT(m_player != 0);
    if (m_player->areCollisionsEnabled()) {
        // 1)
        for (unsigned int i = 0; i < m_enemies.size(); ++i) {
            ASSERT(m_enemies[i] != 0);
            EnemyUnit* enemy = m_enemies[i];
            if (enemy->areCollisionsEnabled() &&
                m_player->collide(*enemy)) {
                // we need to substract life or points or do something.
                m_player->enemyHit(enemy);
                break;
            }
        }

        // 2)
        for (unsigned int i = 0; i < m_elementObjs.size(); ++i) {
            ElementObject* element = m_elementObjs[i];
            ASSERT(element != 0);

            if (element->areCollisionsEnabled() &&
                m_player->collide(*element)) {
                // let the element know about it
                element->objectIntersect(true, m_player);
            }

            // 3)
            if (element->areCollisionsEnabled()) {
                for (unsigned int j = 0; j < m_enemies.size(); ++j) {
                    EnemyUnit* enemy = m_enemies[j];
                    ASSERT(enemy != 0);
                    if (enemy->areCollisionsEnabled() &&
                        enemy->collide(*element)) {
                        element->objectIntersect(false, enemy);
                    }
                }
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
SceneManager::SceneManager() :
    m_renderTarget(0)
,   m_player(0)
{
    ElementObject::setSceneManager(this);
}

////////////////////////////////////////////////////////////////////////////
SceneManager::~SceneManager()
{
    // destroy all the effects we have
    stopAllEffects();
    for (int i = 0; i < Effect::EffectType::ET_MAX; ++i) {
        EffectQueue& q = m_effectQueues[i];
        while (!q.empty()) {
            delete q.front();
            q.pop();
        }
    }

    // destroy enemies
    for (unsigned int i = 0; i < m_enemies.size(); ++i) {
            EnemyUnit* enemy = m_enemies[i];
            m_enemiesQueues[enemy->enemyType()].push(enemy);
    }
    m_enemies.clear();
    for (int i = 0; i < EnemyUnit::EnemyType::COUNT; ++i) {
        EnemyQueue& q = m_enemiesQueues[i];
        while (!q.empty()) {
            delete q.front();
            q.pop();
        }
    }

    // delete the elements
    for (unsigned int i = 0; i < m_elementObjs.size(); ++i) {
        ElementObject* element = m_elementObjs[i];
        m_elementObjQueues[element->elementType()].push(element);
    }
    for (int i = 0; i < ElementObject::Type::EOT_COUNT; ++i) {
        ElementsObjtQueue& q = m_elementObjQueues[i];
        while (!q.empty()) {
            delete q.front();
            q.pop();
        }
    }

}

////////////////////////////////////////////////////////////////////////////
bool
SceneManager::init(const InitData& data)
{
    m_screenSize = data.screenSize;
    // scene to screen
    m_sceneToScreenTf.x = static_cast<float>(m_screenSize.width);
    m_sceneToScreenTf.y = static_cast<float>(m_screenSize.height);
    // screen to scene:
    // m_sceneToScreenTf.x = 1.f / static_cast<float>(m_screenSize.width);
    // m_sceneToScreenTf.y = 1.f / static_cast<float>(m_screenSize.height);

    m_renderTarget = data.renderTarge;
    ASSERT(m_renderTarget != 0);

    // init the different things
    if (!initEffects()) {
        debugERROR("Error initializing the effects\n");
        return false;
    }

    if (!initEnemies()) {
        debugERROR("Error initializing the enemies\n");
        return false;
    }

    if (!initElements()) {
        debugERROR("Error initializing the elements\n");
        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////
// General api for scene objects


void
SceneManager::addExternalSceneObject(SceneObject* so)
{
    ASSERT(so != 0);
    m_externals.push_back(so);
    addSceneObject(so);
}
////////////////////////////////////////////////////////////////////////////
void
SceneManager::removeExternalSceneObject(SceneObject* so)
{
    ASSERT(so != 0);
    common::Helper::remAndSwapIfExists(so, m_externals);
    removeSceneObject(so);
}

////////////////////////////////////////////////////////////////////////////
const Effect*
SceneManager::playEffect(Effect::EffectType effectType,
                         const sf::Vector2f& position,
                         const SceneObject* attach)
{
    ASSERT(effectType < Effect::EffectType::ET_MAX);

    EffectQueue& queue = m_effectQueues[effectType];
    // check if we have that effect
    if (queue.empty()) {
        debugWARNING("We cannot reproduce that effect we have no more of that %d.\n",
                     effectType);
        return 0;
    }

    // get the effect and start it on where we want
    Effect* effect = queue.front();
    queue.pop();
    effect->config(position, attach);
    m_activeEffects.push_back(effect);

    // add the object to be rendered in the scnee
    addSceneObject(effect);

    return effect;
}

////////////////////////////////////////////////////////////////////////////
void
SceneManager::stopAllEffects(void)
{
    for (unsigned int i = 0; i < m_activeEffects.size(); ++i) {
        // put the effects in the associated queue
        ASSERT(m_activeEffects[i] != 0);
        ASSERT(m_activeEffects[i]->effectType() < Effect::EffectType::ET_MAX);
        EffectQueue& queue = m_effectQueues[m_activeEffects[i]->effectType()];
        queue.push(m_activeEffects[i]);
        removeSceneObject(m_activeEffects[i]);
    }
    m_activeEffects.clear();
}

////////////////////////////////////////////////////////////////////////////

const EnemyUnit*
SceneManager::createEnemy(EnemyUnit::EnemyType enemyType)
{
    ASSERT(enemyType < EnemyUnit::EnemyType::COUNT);

    // we will try to create an enemy if we have
    EnemyQueue& queue = m_enemiesQueues[enemyType];
    if (queue.empty()) {
        debugWARNING("We don't have an enemy to create it\n");
        return 0;
    }

    EnemyUnit* enemy = queue.front();
    queue.pop();
    ASSERT(enemy != 0);

    // add it to the enemies active vector and configure it
    FishEnemyUnit* fishUnit = static_cast<FishEnemyUnit*>(enemy);
    // set random position
    const float xrand = static_cast<float>(std::rand() % 999) / 999.f;
    const float rndSecs = 3.f + (std::rand() % 4);
    const float y = 1.f + fishUnit->sceneSize().y;
    fishUnit->configure(sf::Vector2f(xrand,y), FishEnemyUnit::MovingDir::DIR_UP, rndSecs);

    m_enemies.push_back(enemy);

    // add it to the scene object
    addSceneObject(enemy);

    return enemy;
}

////////////////////////////////////////////////////////////////////////////
const ElementObject*
SceneManager::createElement(ElementObject::Type type, const sf::Vector2f& position)
{
    ASSERT(type < ElementObject::Type::EOT_COUNT);

    // we will try to create an element if we have
    ElementsObjtQueue& queue = m_elementObjQueues[type];
    if (queue.empty()) {
        debugWARNING("We don't have an element to create it\n");
        return 0;
    }

    ElementObject* element = queue.front();
    queue.pop();
    ASSERT(element != 0);

    // add it to the element active vector and configure it
    BombElement* bomb = static_cast<BombElement*>(element);
    // set random position
    bomb->configure(position);

    m_elementObjs.push_back(bomb);

    // add it to the scene object
    addSceneObject(bomb);

    return bomb;
}

////////////////////////////////////////////////////////////////////////////

void
SceneManager::update(float timeFrame)
{
    // clear the render queues
    for (unsigned int i = 0; i < SceneObject::RenderLayer::RL_COUNT; ++i) {
        m_renderQueues[i].clear();
    }

    // first of all update the logic here
    updateEffects(timeFrame);
    updateExternals(timeFrame);
    updateEnemies(timeFrame);
    updateElements(timeFrame);

    // now we will render them but before that we need to update also the
    // position / size of the object from scene coordinates to screen ones
    for (unsigned int i = 0; i < m_sceneObjs.size(); ++i) {
        SceneObject* so = m_sceneObjs[i];
        sf::RectangleShape& shape = so->shape();

        // set the correct position and size
        const sf::Vector2f screenPos = sceneToScreenPos(so->position());
        shape.setPosition(screenPos);

        const sf::Vector2f screenSize = sceneToScreenPos(so->sceneSize());
        shape.setSize(screenSize);
        shape.setOrigin(screenSize * 0.5f);

        // put this element also in the associated render queue
        ASSERT(so->renderLayer() < SceneObject::RenderLayer::RL_COUNT);
        if (so->isVisible()) {
            m_renderQueues[so->renderLayer()].push_back(so);
        }
    }


    // perform collisions after updating the positions
    performCollisions();

    // now we will render the objects in the correct layers.
    // clear the render target before
    ASSERT(m_renderTarget != 0);
    m_renderTarget->clear(sf::Color::Black);
    for (SceneObjectsVec& rq : m_renderQueues) {
        for (unsigned int i = 0; i < rq.size(); ++i) {
            m_renderTarget->draw(rq[i]->shape());
        }
    }
}


} /* namespace game */
