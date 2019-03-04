
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "ResourcePath.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <lua/lua.hpp>

using namespace std;

static lua_State *L;
static sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
static sf::Font font;

static string getFileContentsAsString(string filePath) {
    std::ifstream lusScriptStream(filePath);
    std::stringstream luaScriptBuffer;
    luaScriptBuffer << lusScriptStream.rdbuf();
    return luaScriptBuffer.str();
}

static string getLuaScriptAsString() {
    string luaScriptPath = resourcePath() + "script.lua";
    return getFileContentsAsString(luaScriptPath);
}

static int displayText(lua_State *L) {
    const char *textString = lua_tostring(L, 1);
    sf::Text text(textString, font, 30);
    text.setFillColor(sf::Color::Black);
    window.draw(text);
}

static int drawRectanlge(lua_State *L) {
    int positionX = lua_tointeger(L, 1);
    int positionY = lua_tointeger(L, 2);
    int width = lua_tointeger(L, 3);
    int height = lua_tointeger(L, 4);
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(width, height));
    rectangle.setOutlineColor(sf::Color::Red);
    rectangle.setOutlineThickness(5);
    rectangle.setPosition(positionX, positionY);
    window.draw(rectangle);
}

//static int __index(lua_State *L)
//{
//    const char *key = lua_tostring(L, 2);
//    char message[255];
//    sprintf(message, "you accessed property '%s' that does not exist", key);
//
//    lua_pushstring(L, message);
//    return 1;
//}

static int __index(lua_State *L)
{
    const char *key = lua_tostring(L, 2);
    char message[255];
    sprintf(message, "you accessed property '%s'", key);
    cout << message << endl;

    lua_createtable(L, 0, 0);
    lua_createtable(L, 0, 1);
    lua_pushcfunction(L, __index);
    lua_setfield(L, -2, "__index");
    lua_setmetatable(L, -2);
    
    return 1;
}

static void createTableAndSetMetatable(lua_State *L) {
    // see https://stackoverflow.com/questions/3449759/lua-c-api-and-metatable-functions
    lua_createtable(L, 0, 0);
    lua_createtable(L, 0, 1);
    lua_pushcfunction(L, __index);
    lua_setfield(L, -2, "__index");
    lua_setmetatable(L, -2);
    lua_setglobal(L, "game");
}


static void initializeLua() {
    L = luaL_newstate();
    luaopen_base(L);
    luaopen_table(L);
    luaopen_io(L);
    luaopen_string(L);
    luaopen_math(L);
    
    lua_register(L, "displayText", displayText);
    lua_register(L, "drawRectanlge", drawRectanlge);
    
    createTableAndSetMetatable(L);
}

static void loadLua() {
    initializeLua();
    string luaScript = getLuaScriptAsString();
    if (luaL_loadbuffer(L, luaScript.c_str(), strlen(luaScript.c_str()), "game") != 0 ||
        lua_pcall(L, 0, 0, 0) != 0) {
        printf("error loading: %s\n", lua_tostring(L, -1));
        exit(1);
    }
}

static void runLuaRenderFunction()
{
    lua_getglobal(L, "render");
    if (lua_pcall(L, 0, 0, 0) != 0) {
        printf("error running function: %s\n", lua_tostring(L, -1));
        exit(1);
    }
}

int main(int, char const**)
{
    loadLua();

    if (!font.loadFromFile(resourcePath() + "sansation.ttf")) {
        return EXIT_FAILURE;
    }

    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Escape pressed: exit
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }

        window.clear(sf::Color::White);
        runLuaRenderFunction();
        window.display();
    }

    return EXIT_SUCCESS;
}
