#pragma once

#include "IDisplay.h"

namespace otterus_editor
{
    class MenuDisplay: public IDisplay
    {

    public:
        MenuDisplay() = default;
        virtual ~MenuDisplay() = default;

        virtual void Draw() override;
    };

}