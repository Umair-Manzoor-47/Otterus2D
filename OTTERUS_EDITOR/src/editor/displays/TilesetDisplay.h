#pragma once

#include "IDisplay.h"
#include <string>

namespace otterus_editor {

class TilesetDisplay : public IDisplay {
private:
    std::string m_Tileset{""};
    int m_Selected{ -1 };

public:
    TilesetDisplay()  = default;
    ~TilesetDisplay() = default;

    virtual void Draw() override;

    inline void SetTileset(const std::string& tileset) { m_Tileset = tileset;}
    inline const std::string& GetTileset() const { return m_Tileset; }

};

}