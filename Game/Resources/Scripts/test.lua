print("Hello from Lua")

local object = currentObject
local object2 = Scene:getObject("Reyalp")


Scene:addObject("addedObject")
local object3 = Scene:getObject("addedObject")

object3:addComponent("RenderComponent")
local renderComponent3 = object3:getComponent("RenderComponent")
renderComponent3:setShader("default")
renderComponent3:setTexture("default")


local renderComponent = object:getComponent("RenderComponent")
local transformComponent = object:getComponent("TransformComponent")
local audioListenerComponent = object:getComponent("AudioListenerComponent")
local audioPlayerComponent = object2:getComponent("AudioPlayerComponent")

local transformComponent2 = object2:getComponent("TransformComponent")

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
	
    transformComponent2.position:setX(-1)

    local pos = transformComponent.position:get()
    print("X:", pos.x, "Y:", pos.y, "Z:", pos.z)
else
    print("ERROR: transformComponent is nil")
end

audioPlayerComponent:setTexture("test")

function update(deltatime)
    transformComponent.rotation:setZ(250 * deltatime)
    transformComponent2.rotation:setX(250 * deltatime)

    audioPlayerComponent.position:setX(math.sin(deltatime * 10))
    audioPlayerComponent.position:setZ(math.cos(deltatime * 10))

    currentCamera.rotation:setZ(deltatime * 70)
    currentCamera.position:setZ(deltatime + 4)
    --currentCamera:setFov(deltatime * 30)
end