#pragma once

#include <iostream>
#include <string>
#include <Windows.h>
#include <map>

#include "../Global.h"

namespace TDMP
{
    /// <summary>
    /// Max amount player for server. Default: 6
    /// </summary>
    const int MaxPlayers = 6;

    /// <summary>
    /// Current version of the game
    /// </summary>
    extern std::string Version;

    /// <summary>
    /// Whether or not game level (menu including) is loaded
    /// </summary>
    extern bool LevelLoaded;

    /// <summary>
    /// Vector of cached bodies since level load.
    /// This is made for sending (and applying) bodies to the clients. On level load, all bodies have same IDs, so they're not mixed
    /// </summary>
    extern std::vector<TDBody*> levelBodies;

    /// <summary>
    /// [BodyId] = id in levelBodies
    /// </summary>
    extern std::map<int, int> levelBodiesById;

    /// <summary>
    /// Used for the vehicle input sync
    /// </summary>
    struct Input
    {
        bool Ctrl; // Used for syncing that player is crouching

        bool W;
        bool S;
    };

    extern Input localInputData;

    /// <summary>
    /// Updates state of the game in TDMP.dll
    /// </summary>
    void SetGameState(int state);
    
    /// <summary>
    /// Game's state
    /// </summary>
    enum eGameState : int8_t
    {
        Splash = 1,
        Menu = 2,
        UIStart = 3,
        Playing = 4,
        Editor = 5,
        Quitting = 6
    };

    enum DataSendType
    {
        Reliable,
        Unreliable
    };

    /// <summary>
    /// Environment of code. Mostly used for console's prints
    /// </summary>
    enum class Env
    {
        Client,
        Server,
        /// <summary>
        /// Client and Server
        /// </summary>
        Shared,
        /// <summary>
        /// Means that we don't have to use any prefix
        /// </summary>
        None
    };
    namespace Debug
    {
        /// <summary>
        /// Prints a message in console with specific prefix
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="msg">Message</param>
        /// <param name="from">Environment from where was called this method. Default is Env::Shared</param>
        template<class T>
        inline void print(const T& msg, const Env& from = Env::Shared) {
            std::cout << (from == Env::Client ? "[CLIENT] " : from == Env::Server ? "[SERVER] " : from == Env::Shared ? "[SHARED] " : from == Env::None ? "" : "[UNKNOWN] ") << msg << "\n";
        }

        template<class T>
        inline void error(const T& m)
        {
            print("", Env::None);
            std::cout << "[ERROR] " << m << "\n";
            print("", Env::None);
        }
    };

    /// <summary>
    /// Initializes Steam API and other needed stuff
    /// </summary>
    void Init();

    /// <summary>
    /// Calls at fixed time (60Hz)
    /// </summary>
    void Tick();

    /// <summary>
    /// Calls each frame
    /// </summary>
    void Frame();

    void LuaTick();
    void LuaUpdate();

    /// <summary>
    /// Should be called from lua right after level was loaded. If we was alone in lobby -> then do not call this and close the lobby
    /// </summary>
    void Host();

    /// <summary>
    /// Leaves from lobby, from server(if valid), resets something if needed
    /// </summary>
    void Leave();

    /// <summary>
    /// Returns whether or not local player is on server
    /// </summary>
    bool ServerExists();

    /// <summary>
    /// Returns whether or not local player is server(host)
    /// </summary>
    bool IsServer();

    /// <summary>
    /// Gets nickname of player in steam
    /// </summary>
    /// <param name="steamID">Player which nick we need</param>
    /// <returns>Nickname of the player</returns>
    std::string GetPlayerNick(uint64_t steamID);

    /// <summary>
    /// Returns our SteamID
    /// </summary>
    uint64_t GetSteamID();
}