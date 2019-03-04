local name = "Sir"
print("Hello " .. name);

function sum(a, b)
    return a+b
end

print("sum(1,2) = " .. sum(1,2))

local counter = 0

function render()
    if true then
        return 0
    end

    counter = counter + 1
    local message = "render() function here, i've been called " .. counter .. " times"
    print(message)
    displayText(message)

    -- x, y, width, height
    drawRectanlge(10, 80, 100, 50)

end

person = {name = "wyatt", age = 13}
for key,value in pairs(person) do
    print("key="..key, "value="..value)
end

print(game.Workspace.Part.Name)

print(game.Workspace.Part2.Name)

-- game.Workspace.Part1
-- game.Workspace.Part2
-- game.Players
-- game.LocalPlayers


print(game.Players.LocalPlayer.Name)

getglobal print
getglobal game
getfield -1 Players
getfield -1 LocalPlayer
getfield -1 Name
pcall 1,0,0
