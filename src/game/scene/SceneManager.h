/*
 * SceneManager.h
 *
 *  Created on: Oct 30, 2014
 *      Author: agustin
 */

#ifndef SCENEMANAGER_H_
#define SCENEMANAGER_H_

#include <vector>
#include <queue>

#include <SFML/Graphics/RenderTarget.hpp>

#include <common/helpers/Helper.h>
#include <common/debug/DebugUtil.h>
#include <game/effects/Effect.h>
#include <game/enemies/EnemyUnit.h>

#include "SceneObject.h"


namespace game {

class SceneManager
{
public:

    struct InitData {
        // the render target where we will draw all the scene
        sf::RenderTarget* renderTarge;
        // to map the scene {(0,0), (1,1)} -> screenSize
        sf::IntRect screenSize;
    };

public:
    SceneManager();
    ~SceneManager();


    ////////////////////////////////////////////////////////////////////////////
    // Init

    // @brief Init the scene manager.
    // @param data      The init data
    // @return true on success | false otherwise
    //
    bool
    init(const InitData& data);

    ////////////////////////////////////////////////////////////////////////////
    // General api for scene objects

    // @brief Add / remove an external scene object that will be updated
    //        until the "update" method returns false.
    //
    void
    addExternalSceneObject(SceneObject* so);
    void
    removeExternalSceneObject(SceneObject* so);

    ////////////////////////////////////////////////////////////////////////////
    // Effect API

    // @brief Start a new effect of a specific type in a certain position
    //        and giving the object to attach it if we want.
    // @param effectType    The effect type we want to reproduce
    // @param position      The position where we want to play it (scene).
    // @param attach        The scene object to where we want to attach the
    //                      effect or Null if not.
    // @return the associated effect | 0 on error
    // @note after calling this method the effect will be reproduced
    //       automatically
    //
    const Effect*
    playEffect(Effect::EffectType effectType,
               const sf::Vector2f& position,
               const SceneObject* attach = 0);

    // @brief Stop all the effects
    //
    void
    stopAllEffects(void);

    ////////////////////////////////////////////////////////////////////////////
    // Enemies API

    // @brief Create a new enemy of an specific type (the logic of where
    //        will be created will be handled by the scene manager)
    // @param enemyType     The type of enemy to create
    // @return the associated enemy | 0 on error
    // @note after calling this method the effect will be reproduced
    //       automatically
    //
    const EnemyUnit*
    createEnemy(EnemyUnit::EnemyType enemyType);

    ////////////////////////////////////////////////////////////////////////////

    // @brief Main update method to update all the logic of the scene.
    //
    void
    update(float timeFrame);

private:

    // @brief Init all the type of effects we have.
    //
    bool
    initEffects(void);

    // @brief Init the different type of enemies
    //
    bool
    initEnemies(void);

    // @brief Update all the effects and effect logic we have.
    //
    void
    updateEffects(float timeFrame);

    // @brief Update the enemies
    //
    void
    updateEnemies(float timeFrame);

    // @brief Update the external scene objects
    //
    void
    updateExternals(float timeFrame);

    // @brief Coordinates transformation method, from scene coordinates to
    //        screen coordinates
    //        The scene will be between 0,0 (top left) to (1,1) bottom right.
    // @param scenePos      To be transformed.
    // @return screenPos already transformed
    //
    inline sf::Vector2f
    sceneToScreenPos(const sf::Vector2f& scenePos) const;
    inline sf::Vector2f
    sceneToScreenSize(const sf::Vector2f& sceneSize) const;

    // @brief This method should be called everytime we add something to the
    //        scene / remove from it (effect / objects / units / whatever).
    // @param so        The scene object
    //
    inline void
    addSceneObject(SceneObject* so);
    inline void
    removeSceneObject(SceneObject* so);

private:
    typedef std::queue<Effect*> EffectQueue;
    typedef std::vector<SceneObject*> SceneObjectsVec;
    typedef std::queue<EnemyUnit*> EnemyQueue;
    typedef std::vector<EnemyUnit*> Enemyvec;

    sf::RenderTarget* m_renderTarget;
    sf::IntRect m_screenSize;
    sf::Vector2f m_sceneToScreenTf;
    SceneObjectsVec m_sceneObjs;        // all the objects we will render
    SceneObjectsVec m_renderQueues[SceneObject::RenderLayer::RL_COUNT];

    // enemies
    EnemyQueue m_enemiesQueues[EnemyUnit::EnemyType::COUNT];
    Enemyvec m_enemies;


    // external
    SceneObjectsVec m_externals;

    // effects
    std::vector<Effect*> m_activeEffects;
    EffectQueue m_effectQueues[Effect::EffectType::ET_MAX];
};












////////////////////////////////////////////////////////////////////////////////
// Inline
//

inline sf::Vector2f
SceneManager::sceneToScreenPos(const sf::Vector2f& scenePos) const
{
    return sf::Vector2f(scenePos.x * m_sceneToScreenTf.x,
                        scenePos.y * m_sceneToScreenTf.y);
}
inline sf::Vector2f
SceneManager::sceneToScreenSize(const sf::Vector2f& sceneSize) const
{
    return sf::Vector2f(sceneSize.x * m_sceneToScreenTf.x,
                        sceneSize.y * m_sceneToScreenTf.y);
}

inline void
SceneManager::addSceneObject(SceneObject* so)
{
    ASSERT(so != 0);
    ASSERT(common::Helper::findElemVec(so, m_sceneObjs) < 0);
    m_sceneObjs.push_back(so);
}
inline void
SceneManager::removeSceneObject(SceneObject* so)
{
    ASSERT(so != 0);
    common::Helper::remAndSwapIfExists(so, m_sceneObjs);
}

} /* namespace game */

#endif /* SCENEMANAGER_H_ */
