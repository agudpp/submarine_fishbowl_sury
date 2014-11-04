/*
 * SceneManager.cpp
 *
 *  Created on: Oct 30, 2014
 *      Author: agustin
 */

#include "SceneManager.h"


#define EXPLOSION_EFFECT_COUNT      3
#define COUNTER_EFFECT_COUNT        3

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
        m_effectQueues[et].push(effect);
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
////////////////////////////////////////////////////////////////////////////
SceneManager::SceneManager() :
    m_renderTarget(0)
{
    // TODO Auto-generated constructor stub

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

        // put this element also in the associated render queue
        ASSERT(so->renderLayer() < SceneObject::RenderLayer::RL_COUNT);
        if (so->isVisible()) {
            m_renderQueues[so->renderLayer()].push_back(so);
        }
    }

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
