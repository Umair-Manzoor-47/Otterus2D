-- Math Utilities

function clamp(val, min_val, max_val)
    if val < min_val then return min_val end
    if val > max_val then return max_val end
    return val
end

function lerp(a, b, t)
    return a + (b - a) * t
end

function GetDigit(num, digit)
    local n = 10 ^ digit
    local n1 = 10 ^ (digit - 1)
    return math.floor((num % n) / n1)
end

function GetRandomPosition()
    local w = WindowWidth and WindowWidth() or 640
    local h = WindowHeight and WindowHeight() or 480
    return vec2(
        math.random(w) + w,
        math.random(h) + h
    )
end

function GetRandomVelocity(min_speed, max_speed)
    return vec2(
        math.random(min_speed, max_speed),
        math.random(min_speed, max_speed)
    )
end
