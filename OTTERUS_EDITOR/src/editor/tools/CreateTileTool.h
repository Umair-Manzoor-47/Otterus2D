#pragma once
#include "TileTool.h"

namespace otterus_editor{
    class CreateTileTool: public TileTool
    {

    public:
        CreateTileTool();
        virtual ~CreateTileTool() = default;

        virtual void Create() override;
        virtual void Draw() override;



    };

}
