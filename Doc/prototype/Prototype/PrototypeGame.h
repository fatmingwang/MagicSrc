#pragma once
#include <vector>
#include <string>

enum class ResourceType { WOOD, METAL, FOOD, MONEY };

struct Stats{int HP;int attack;int defense;float speed;int range;};
struct Unit{
    int id; std::string kind; int level; int exp; Stats stats;
    // position for simple travel calculation
    float x, y;
    // task state
    int taskPhase; // -1 idle, 0 travelToNode,1 gather,2 returnToBase
    int taskTargetNodeId; float taskTimer; float taskCollected;
    Unit():id(0),kind(""),level(1),exp(0),x(0),y(0),taskPhase(-1),taskTargetNodeId(-1),taskTimer(0),taskCollected(0){}
};
struct Resources{int meat;int money;int metal;int wood;};

struct QueueItem{enum Type{RECRUIT,PRODUCE,RESEARCH};Type type;std::string target;float finishAt;};

struct GameState{Resources resources;int nextUnitId;std::vector<Unit> unitList;std::vector<QueueItem> queue;};

struct ResourceNode{
    int id;
    ResourceType type;
    float yieldMax;
    float yieldCur;
    float gatherRate; // per second per worker
    float regenRate;
    float respawnDelay;
    int quality;
    bool depleted;
    float lastDepletedAt;
    float x,y;
};

class ResourceManager{
public:
    std::vector<ResourceNode> nodes;
    void InitSampleNodes();
    ResourceNode* GetNodeById(int id);
    void Update(float dt);
};

class PrototypeGame{
public:
    GameState state;
    ResourceManager resourceManager;
    PrototypeGame();
    ~PrototypeGame();
    void Init();
    void Update(float dt);
    void Render();
    Unit* CreateUnit(const std::string& kind);

    // new API for resource interaction
    // Simulate clicking a resource node to start a gather job that costs 50 food
    void OnClickResourceNode(int nodeId);

private:
    // helpers
    Unit* FindIdleWorker();
    float Distance(float x1,float y1,float x2,float y2){float dx=x1-x2, dy=y1-y2; return std::sqrt(dx*dx+dy*dy);}    
};
