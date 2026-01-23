#pragma once

#include "IMiniGame.h"
#include <string>

namespace MiniGameFactory
{
    // create by name, ownership transferred to caller
    cIMiniGame* Create(const std::string& name, int e_param = 0);
}