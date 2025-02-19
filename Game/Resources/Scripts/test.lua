print("Hello from Lua")

local object = Scene:getObject("Player")
--local object = currentObject

--object:addComponent("RenderComponent")
local renderComponent = object:getComponent("RenderComponent")
local transformComponent = object:getComponent("TransformComponent")

if renderComponent then
    renderComponent:setShader("reversedColors")
    renderComponent:setTexture("modelface")
else
    print("ERROR: renderComponent is nil")
end

if transformComponent then
    --transformComponent.position:set(0, 1, 0)
    transformComponent.position:setY(0.2)
    transformComponent.position:setX(1)

    local pos = transformComponent.position:get()
    print("X:", pos.x, "Y:", pos.y, "Z:", pos.z)
else
    print("ERROR: transformComponent is nil")
end

function update(deltatime)
    transformComponent.rotation:setX(250 * deltatime)
end