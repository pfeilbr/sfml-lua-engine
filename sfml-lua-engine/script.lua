local name = "Sir"
print("Hello " .. name);

function sum(a, b)
    return a+b
end

print("sum(1,2) = " .. sum(1,2))

local counter = 0

function render()
    counter = counter + 1
    local message = "render() function here, i've been called " .. counter .. " times"
    print(message)
    displayText(message)

    -- x, y, width, height
    drawRectanlge(10, 80, 100, 50)
end
