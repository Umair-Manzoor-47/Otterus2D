GameData = {}
GameData.__index = GameData

function GameData:Create()
    local this = {
        m_MaxLives = 5,
        m_NumLives = 3,
        m_Scores = 0,
        m_HighScore = 0,
        m_Wave = 1,
        m_ExtraLifeThreshold = 10000
    }
    setmetatable(this, self)
    return this
end

function GameData:Reset()
    self.m_NumLives = 3
    self.m_Scores = 0
    self.m_Wave = 1
    self.m_ExtraLifeThreshold = 10000
end

function GameData:IsGameOver()
    return self.m_NumLives <= 0
end

function GameData:MaxLives()
    return self.m_MaxLives
end

function GameData:NumLives()
    return self.m_NumLives
end

function GameData:AddLife()
    self.m_NumLives = math.min(self.m_NumLives + 1, self.m_MaxLives)
end

function GameData:RemoveLife()
    self.m_NumLives = math.max(self.m_NumLives - 1, 0)
end

function GameData:AddScore(value)
    self.m_Scores = self.m_Scores + value
    if self.m_Scores > self.m_HighScore then
        self.m_HighScore = self.m_Scores
    end
    if self.m_Scores >= self.m_ExtraLifeThreshold then
        self:AddLife()
        self.m_ExtraLifeThreshold = self.m_ExtraLifeThreshold + 10000
    end
end

function GameData:GetScore()
    return self.m_Scores
end

function GameData:GetHighScore()
    return self.m_HighScore
end

function GameData:GetWave()
    return self.m_Wave
end

function GameData:NextWave()
    self.m_Wave = self.m_Wave + 1
end

gData = GameData:Create()
