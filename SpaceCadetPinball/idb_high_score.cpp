#include "pch.h"
#include "idb_high_score.h"

#ifdef __EMSCRIPTEN__

#include <emscripten.h>
#include <json/json.h>
#include "high_score.h"

extern "C"
{
    // C++ method bindings, prefix with _
    extern int ReadHighScores();
    extern int WriteHighScores(char* scores);
    extern int ClearHighScores();

    // JSEvents C++ bindings, prefix with _
    extern bool JSEvents_IsQueueEmpty();
    extern bool JSEvents_IsResolved(int handle);
    extern bool JSEvents_IsRejected(int handle);
    extern void* JSEvents_GetResolveValue(int handle);
    extern void* JSEvents_GetRejectValue(int handle);
    extern void JSEvents_FreeEvent(int handle);
}


// A simple asyncronous javascript event loop for C++
bool JSEventLoop()
{
    if (JSEvents_IsQueueEmpty())
        return true;

    // Main JS event loop.
    // An empty while loop seems to be the best way to pump the Emscripten event loop.
    // See: https://github.com/emscripten-core/emscripten/issues/9546
    while (!JSEvents_IsQueueEmpty())
    {
        emscripten_sleep(0);
    }

    return true;
}


int idb_high_score::read(high_score_struct* table)
{
    auto handle = ReadHighScores();
    if (!JSEventLoop())
        return -1;

    if (JSEvents_IsRejected(handle))
    {
        auto error = (char*)JSEvents_GetRejectValue(handle);
        printf("Error reading high scores: %s\n", error);
        free(error);
        JSEvents_FreeEvent(handle);
        return -1;
    }

    if (JSEvents_IsResolved(handle))
    {
        auto json_scores = (char*)JSEvents_GetResolveValue(handle);
        if (json_scores)
        {
            Json::Value root;
            Json::Reader reader;
            if (reader.parse(json_scores, root))
            {
                for (int i = 0; i < 5; i++)
                {
                    table[i].Score = root[i]["Score"].asInt();
                    const char* nameStr = root[i]["Name"].asCString();
                    strncpy(table[i].Name, nameStr, sizeof(table[i].Name) - 1);
                    table[i].Name[sizeof(table[i].Name) - 1] = '\0';
                }
            }
            free(json_scores);
        }
        JSEvents_FreeEvent(handle);
    }
    return 0;
}

int idb_high_score::write(high_score_struct* table)
{
    Json::Value root(Json::arrayValue);
    for (int i = 0; i < 5; i++)
    {
        Json::Value score;
        score["Name"] = table[i].Name;
        score["Score"] = table[i].Score;
        root.append(score);
    }

    Json::FastWriter writer;
    auto json_scores = writer.write(root);

    auto handle = WriteHighScores((char*)json_scores.c_str());
    if (!JSEventLoop())
        return -1;

    if (JSEvents_IsRejected(handle))
    {
        auto error = (char*)JSEvents_GetRejectValue(handle);
        printf("Error writing high scores: %s\n", error);
        free(error);
        JSEvents_FreeEvent(handle);
        return -1;
    }
    JSEvents_FreeEvent(handle);
    return 0;
}

int idb_high_score::clear()
{
    auto handle = ClearHighScores();
    if (!JSEventLoop())
        return -1;

    if (JSEvents_IsRejected(handle))
    {
        auto error = (char*)JSEvents_GetRejectValue(handle);
        printf("Error clearing high scores: %s\n", error);
        free(error);
        JSEvents_FreeEvent(handle);
        return -1;
    }
    JSEvents_FreeEvent(handle);
    return 0;
}

#endif // __EMSCRIPTEN__
