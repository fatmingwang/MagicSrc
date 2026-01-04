#pragma once
#include <vector>
#include <string>

struct Stats{int HP;int attack;int defense;float speed;int range;};
struct Unit{int id;std::string kind;int level;int exp;Stats stats;};
struct Resources{int meat;int money;int metal;int wood;};

struct QueueItem{enum Type{RECRUIT,PRODUCE,RESEARCH};Type type;std::string target;float finishAt;};

struct GameState{Resources resources;int nextUnitId;std::vector<Unit> unitList;std::vector<QueueItem> queue;};

class PrototypeGame{
public:
    GameState state;
    void Init();
    void Update(float dt);
    void Render();
    Unit* CreateUnit(const std::string& kind);
};
