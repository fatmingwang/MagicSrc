#pragma once

#include "MiniGameResult.h"
#include <string>

class cIMiniGame : public NamedTypedObject
{
public:
    virtual ~cIMiniGame() {}
    virtual std::string GetName() const = 0;
    // start game, optionally with seed or node id
    virtual void Start(int e_seed = 0) = 0;
    // update per frame or tick (dt in seconds)
    virtual void Update(double e_dt) = 0;
    // input handling (simple prototype)
    virtual void OnClick(int e_x, int e_y) = 0;
    // returns true if finished
    virtual bool IsFinished() const = 0;
    virtual sMiniGameResult GetResult() const = 0;
};
