#include "PrototypeGame.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <chrono>

using namespace std::chrono;

// Simple prototype implementation: resource interaction added

PrototypeGame::PrototypeGame()
{
}
PrototypeGame::~PrototypeGame()
{
}

void ResourceManager::InitSampleNodes()
{
    nodes.clear();
    ResourceNode n1; n1.id=1; n1.type=ResourceType::WOOD; n1.yieldMax=500; n1.yieldCur=500; n1.gatherRate=2; n1.regenRate=0.5; n1.respawnDelay=60; n1.quality=1; n1.depleted=false; n1.lastDepletedAt=0; n1.x=200; n1.y=100; nodes.push_back(n1);
    ResourceNode n2; n2.id=2; n2.type=ResourceType::METAL; n2.yieldMax=300; n2.yieldCur=300; n2.gatherRate=1.2; n2.regenRate=0.2; n2.respawnDelay=600; n2.quality=2; n2.depleted=false; n2.lastDepletedAt=0; n2.x=500; n2.y=120; nodes.push_back(n2);
}

ResourceNode* ResourceManager::GetNodeById(int id)
{
    for (auto &n : nodes) if (n.id==id) return &n;
    return nullptr;
}

void ResourceManager::Update(float dt)
{
    float now = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
    for (auto &n : nodes)
    {
        if (n.depleted)
        {
            if (now - n.lastDepletedAt > n.respawnDelay)
            {
                // regenerate gradually
                n.yieldCur += n.regenRate * dt;
                if (n.yieldCur >= n.yieldMax)
                {
                    n.yieldCur = n.yieldMax; n.depleted=false;
                    std::cout<<"Node "<<n.id<<" regenerated"<<std::endl;
                }
            }
        }
    }
}


void PrototypeGame::Init()
{
    state.resources.meat = 200;
    state.resources.money = 500;
    state.resources.metal = 200;
    state.resources.wood = 200;
    state.nextUnitId = 1;
    resourceManager.InitSampleNodes();
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
    u.x = 100; u.y = 80; // base position
    if (kind == "worker") { u.stats.HP=50; u.stats.attack=1; u.stats.defense=1; u.stats.speed=40.f; u.stats.range=1; }
    else if (kind=="infantry") { u.stats.HP=120; u.stats.attack=10; u.stats.defense=5; u.stats.speed=50.f; u.stats.range=1; }
    else if (kind=="cavalry") { u.stats.HP=150; u.stats.attack=14; u.stats.defense=4; u.stats.speed=70.f; u.stats.range=1; }
    u.taskPhase = -1; u.taskTargetNodeId=-1; u.taskTimer=0; u.taskCollected=0;
    state.unitList.push_back(u);
    // return pointer to stored unit
    return &state.unitList.back();
}

Unit* PrototypeGame::FindIdleWorker()
{
    for (auto &u : state.unitList)
    {
        if (u.kind=="worker" && u.taskPhase==-1) return &u;
    }
    return nullptr;
}

void PrototypeGame::OnClickResourceNode(int nodeId)
{
    // require 50 food to send worker
    if (state.resources.meat < 50)
    {
        std::cout<<"Not enough food to start a gather job (need 50)"<<std::endl;
        return;
    }
    Unit* w = FindIdleWorker();
    if (!w)
    {
        std::cout<<"No idle worker available"<<std::endl;
        return;
    }
    ResourceNode* n = resourceManager.GetNodeById(nodeId);
    if (!n || n->depleted)
    {
        std::cout<<"Invalid or depleted node"<<std::endl;
        return;
    }
    // pay 50 food
    state.resources.meat -= 50;
    // assign task: travel -> gather -> return
    w->taskPhase = 0; // travel
    w->taskTargetNodeId = nodeId;
    // compute travel time simplified: distance / speed
    float dist = Distance(w->x,w->y,n->x,n->y);
    w->taskTimer = dist / w->stats.speed; // seconds
    w->taskCollected = 0;
    std::cout<<"Worker "<<w->id<<" dispatched to node "<<nodeId<<" travelTime="<<w->taskTimer<<std::endl;
}

void PrototypeGame::Update(float dt)
{
    resourceManager.Update(dt);
    // process queues
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

    // process worker tasks
    for (auto &u : state.unitList)
    {
        if (u.kind!="worker") continue;
        if (u.taskPhase==-1) continue;
        ResourceNode* n = resourceManager.GetNodeById(u.taskTargetNodeId);
        if (!n) { u.taskPhase=-1; continue; }
        if (u.taskPhase==0)
        {
            u.taskTimer -= dt;
            if (u.taskTimer<=0)
            {
                u.taskPhase=1; // gather
                u.taskTimer = 0;
                std::cout<<"Worker "<<u.id<<" arrived at node "<<n->id<<std::endl;
            }
        }
        else if (u.taskPhase==1)
        {
            if (n->depleted) { u.taskPhase=2; u.taskTimer = Distance(u.x,u.y,n->x,n->y)/u.stats.speed; continue; }
            // gather amount
            float gatherAmount = u.stats.speed * 0.05f * n->gatherRate * (1.0f + 0.1f*(n->quality-1)) * dt; // simplified
            if (gatherAmount > n->yieldCur) gatherAmount = n->yieldCur;
            n->yieldCur -= gatherAmount;
            u.taskCollected += gatherAmount;
            if (n->yieldCur <= 0) { n->depleted=true; n->lastDepletedAt = duration_cast<seconds>(system_clock::now().time_since_epoch()).count(); std::cout<<"Node "<<n->id<<" depleted\n"; }
            // simulate carrying: after collecting some threshold, return
            if (u.taskCollected >= 20.0f)
            {
                u.taskPhase=2; // return
                float distBack = Distance(u.x,u.y,n->x,n->y);
                u.taskTimer = distBack / u.stats.speed;
            }
        }
        else if (u.taskPhase==2)
        {
            u.taskTimer -= dt;
            if (u.taskTimer<=0)
            {
                // upload collected resources to base
                // map resource type to state
                if (n->type==ResourceType::WOOD) state.resources.wood += int(u.taskCollected);
                else if (n->type==ResourceType::METAL) state.resources.metal += int(u.taskCollected);
                else if (n->type==ResourceType::FOOD) state.resources.meat += int(u.taskCollected);
                else if (n->type==ResourceType::MONEY) state.resources.money += int(u.taskCollected);
                std::cout<<"Worker "<<u.id<<" returned with "<<u.taskCollected<<" from node "<<n->id<<"\n";
                u.taskCollected = 0;
                u.taskPhase=-1;
            }
        }
    }
}

void PrototypeGame::Render()
{
    // simple text output to console for now
    std::cout<<"Resources: meat="<<state.resources.meat<<" money="<<state.resources.money<<" metal="<<state.resources.metal<<" wood="<<state.resources.wood<<"\n";
    for (auto &u: state.unitList)
    {
        std::cout<<"Unit "<<u.id<<" kind="<<u.kind<<" HP="<<u.stats.HP<<" exp="<<u.exp;
        if (u.kind=="worker") std::cout<<" task="<<u.taskPhase<<" target="<<u.taskTargetNodeId<<" collected="<<u.taskCollected;
        std::cout<<"\n";
    }
    for (auto &n: resourceManager.nodes)
    {
        std::cout<<"Node "<<n.id<<" type="<< (int)n.type <<" cur="<<n.yieldCur<<" depleted="<<n.depleted<<" pos="<<n.x<<","<<n.y<<"\n";
    }
}
