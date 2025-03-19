#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_video.h>

static void LogSdlError(const char* message) noexcept;

int main(int argCount, char* args[])
{
    if(!SDL_SetAppMetadata("BlaueSim", "0.0.1", "com.hyfloac.blauesim"))
    {
        LogSdlError("Error setting app metadata");
        return 1;
    }

    if(!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_CREATOR_STRING, "hyfloac"))
    {
        LogSdlError("Error setting app metadata");
        return 1;
    }

    if(!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_COPYRIGHT_STRING, "Copyright (c) 2025 hyfloac"))
    {
        LogSdlError("Error setting app metadata");
        return 1;
    }

    if(!SDL_Init(SDL_INIT_VIDEO))
    {
        LogSdlError("Error initializing video subsystem");
        return 2;
    }

    SDL_Window* window = SDL_CreateWindow("BlaueSim", 800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN);

    if(!window)
    {
        LogSdlError("Error creating window");
        return 3;
    }

    bool isRunning = true;

    while(isRunning)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                    isRunning = false;
                    break;
                case SDL_EVENT_QUIT:
                    isRunning = false;
                    break;
                default:
                    break;
            }
        }
    }

    SDL_DestroyWindow(window);

    SDL_QuitSubSystem(SDL_INIT_VIDEO);

    SDL_Quit();

    return 0;
}

static void LogSdlError(const char* message) noexcept
{
    const char* error = SDL_GetError();
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s: %s", message, error);
}
