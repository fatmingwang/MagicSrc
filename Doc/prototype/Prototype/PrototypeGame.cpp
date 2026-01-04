#include "PrototypeGame.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>

// Simple prototype implementation: load SVGs as textures (assume helper functions exist)

void PrototypeGame::Init()
{
    state.resources.meat = 200;
    state.resources.money = 500;
    state.resources.metal = 200;
    state.resources.wood = 200;
    state.nextUnitId = 1;
    // create 2 workers
    for (int i=0;i<2;i++) CreateUnit("worker");
}

Unit* PrototypeGame::CreateUnit(const std::string& kind)
{
    Unit u;
    u.id = state.nextUnitId++;
    u.kind = kind;
    u.level = 1;
    u.exp = 0;
    if (kind == "worker") { u.stats.HP=50; u.stats.attack=1; u.stats.defense=1; u.stats.speed=1.0f; u.stats.range=1; }
    else if (kind=="infantry") { u.stats.HP=120; u.stats.attack=10; u.stats.defense=5; u.stats.speed=1.0f; u.stats.range=1; }
    else if (kind=="cavalry") { u.stats.HP=150; u.stats.attack=14; u.stats.defense=4; u.stats.speed=1.4f; u.stats.range=1; }
    state.unitList.push_back(u);
    // return pointer to stored unit
    return &state.unitList.back();
}

void PrototypeGame::Update(float dt)
{
    // process queues (recruit/produce/research)
    // process gather assignments
    for (auto &q: state.queue)
    {
        q.finishAt -= dt;
    }
    state.queue.erase(std::remove_if(state.queue.begin(), state.queue.end(), [&](const QueueItem&qi){
        if (qi.finishAt<=0)
        {
            if (qi.type==QueueItem::RECRUIT)
            {
                CreateUnit(qi.target);
            }
            return true;
        }
        return false;
    }), state.queue.end());
}

void PrototypeGame::Render()
{
    // simple text output to console for now
    std::cout<<"Resources: meat="<<state.resources.meat<<" money="<<state.resources.money<<" metal="<<state.resources.metal<<" wood="<<state.resources.wood<<"\n";
    for (auto &u: state.unitList)
    {
        std::cout<<"Unit "<<u.id<<" kind="<<u.kind<<" HP="<<u.stats.HP<<" exp="<<u.exp<<"\n";
    }
}
