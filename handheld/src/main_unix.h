#ifndef MAIN_UNIX_H__
#define MAIN_UNIX_H__

// targets generic Linux desktops using SDL2 and Desktop OpenGL

#include "client/renderer/gles.h"
#include "App.h"
#include "NinecraftApp.h"
#include "platform/input/Mouse.h"
#include "platform/input/Multitouch.h"
#include "platform/input/Keyboard.h"

#include <SDL2/SDL.h>
#include <png.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

static bool g_running_unix = true;
static bool g_mouseGrabbed = false;

static SDL_Window* g_win = nullptr;
static SDL_GLContext g_glContext = nullptr;
static int g_winWidth = 848;
static int g_winHeight = 480;

void platform_setMouseGrabbed(bool grab) {
    if (!g_win) return;
    if (grab == g_mouseGrabbed) return;
    g_mouseGrabbed = grab;

    if (grab) {
        SDL_SetRelativeMouseMode(SDL_TRUE);
    } else {
        SDL_SetRelativeMouseMode(SDL_FALSE);
        SDL_WarpMouseInWindow(g_win, g_winWidth / 2, g_winHeight / 2);
    }
}

static unsigned char transformKey(SDL_Keycode keysym) {
    if (keysym >= SDLK_a && keysym <= SDLK_z) return keysym;
    if (keysym >= SDLK_0 && keysym <= SDLK_9) return keysym;
    
    switch(keysym) {
        case SDLK_ESCAPE: return 27;
        case SDLK_SPACE: return 32;
        case SDLK_RETURN: return 13;
        case SDLK_BACKSPACE: return 8;
        case SDLK_TAB: return 9;
        case SDLK_LSHIFT:
        case SDLK_RSHIFT: return Keyboard::KEY_LSHIFT;
        case SDLK_LCTRL:
        case SDLK_RCTRL: return Keyboard::KEY_LCTRL;
        case SDLK_UP: return 38;
        case SDLK_DOWN: return 40;
        case SDLK_LEFT: return 37;
        case SDLK_RIGHT: return 39;
        case SDLK_LALT:
        case SDLK_RALT: return 18;
    }
    return 0;
}

static void handleSDLEvent(SDL_Event& ev, App* app) {
    switch (ev.type) {
    case SDL_QUIT:
        g_running_unix = false;
        break;
    case SDL_WINDOWEVENT:
        if (ev.window.event == SDL_WINDOWEVENT_RESIZED || ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
            g_winWidth = ev.window.data1;
            g_winHeight = ev.window.data2;
            if (app) app->setSize(g_winWidth, g_winHeight);
        } else if (ev.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
            if (!g_mouseGrabbed) {
                SDL_WarpMouseInWindow(g_win, g_winWidth / 2, g_winHeight / 2);
            }
        }
        break;
    case SDL_KEYDOWN:
    case SDL_KEYUP: {
        bool pressed = (ev.type == SDL_KEYDOWN);
        unsigned char transformed = transformKey(ev.key.keysym.sym);
        if (transformed) {
            Keyboard::feed(transformed, pressed ? KeyboardAction::KEYDOWN : KeyboardAction::KEYUP);
        }
        break;
    }
    case SDL_TEXTINPUT: {
        for (int i = 0; ev.text.text[i] != '\0'; ++i) {
            unsigned char ch = static_cast<unsigned char>(ev.text.text[i]);
            if (ch >= 32) Keyboard::feedText(ch);
        }
        break;
    }
    case SDL_MOUSEWHEEL: {
        if (ev.wheel.y > 0) {
            Mouse::feed(3, 0, Mouse::getX(), Mouse::getY(), 0, 1); 
        } else if (ev.wheel.y < 0) {
            Mouse::feed(3, 0, Mouse::getX(), Mouse::getY(), 0, -1);
        }
        break;
    }
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP: {
        int button = ev.button.button;
        int action = 0;
        if (button == SDL_BUTTON_LEFT) action = 1;
        else if (button == SDL_BUTTON_RIGHT) action = 2;
        
        if (action) {
            int state = (ev.type == SDL_MOUSEBUTTONDOWN) ? 1 : 0;
            Mouse::feed(action, state, Mouse::getX(), Mouse::getY());
            Multitouch::feed(action, state, Mouse::getX(), Mouse::getY(), 0);
        }
        break;
    }
    case SDL_MOUSEMOTION: {
        if (g_mouseGrabbed) {
            Mouse::feed(0, 0, Mouse::getX(), Mouse::getY(), ev.motion.xrel, ev.motion.yrel);
            Multitouch::feed(0, 0, Mouse::getX(), Mouse::getY(), 0);
        } else {
            Mouse::feed(0, 0, ev.motion.x, ev.motion.y, 0, 0);
            Multitouch::feed(0, 0, ev.motion.x, ev.motion.y, 0);
        }
        break;
    }
    default:
        break;
    }
}

int main(int argc, char** argv) {
    if (argc > 0 && argv[0]) {
        std::string path = argv[0];
        std::size_t pos = path.find_last_of('/');
        if (pos != std::string::npos) {
            path = path.substr(0, pos);
            chdir(path.c_str());
        }
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    g_winWidth = 848;
    g_winHeight = 480;

    g_win = SDL_CreateWindow("Minecraft PE 0.6.1 (Pewiku)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                             g_winWidth, g_winHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
    
    if (!g_win) {
        fprintf(stderr, "Failed to create SDL window: %s\n", SDL_GetError());
        return 1;
    }

    g_glContext = SDL_GL_CreateContext(g_win);
    if (!g_glContext) {
        fprintf(stderr, "Failed to create GL context: %s\n", SDL_GetError());
        return 1;
    }
    
    SDL_GL_MakeCurrent(g_win, g_glContext);

#ifdef PLATFORM_DESKTOP
    SDL_ShowCursor(SDL_DISABLE);
#endif
    
    glInit();

    AppContext appContext;
    appContext.doRender = true;

    class AppPlatform_unix : public AppPlatform {
    public:
        bool isTouchscreen() { return false; }
        TextureData loadTexture(const std::string& filename_, bool textureFolder) {
            TextureData out;
            std::string filename = textureFolder ? std::string("data/images/") + filename_ : filename_;
            std::ifstream source(filename.c_str(), std::ios::binary);
            if (source) {
                png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
                if (!pngPtr)
                    return out;

                png_infop infoPtr = png_create_info_struct(pngPtr);
                if (!infoPtr) {
                    png_destroy_read_struct(&pngPtr, NULL, NULL);
                    return out;
                }

                png_set_read_fn(pngPtr, (png_voidp)&source, [](png_structp pngPtr, png_bytep data, png_size_t length) {
                    ((std::istream*)png_get_io_ptr(pngPtr))->read((char*)data, length);
                });
                png_read_info(pngPtr, infoPtr);
                out.w = png_get_image_width(pngPtr, infoPtr);
                out.h = png_get_image_height(pngPtr, infoPtr);
                png_bytep* rowPtrs = new png_bytep[out.h];
                out.data = new unsigned char[4 * out.w * out.h];
                out.memoryHandledExternally = false;
                int rowStrideBytes = 4 * out.w;
                for (int i = 0; i < out.h; ++i) {
                    rowPtrs[i] = (png_bytep)&out.data[i * rowStrideBytes];
                }
                png_read_image(pngPtr, rowPtrs);
                png_destroy_read_struct(&pngPtr, &infoPtr, (png_infopp)0);
                delete[] (png_bytep)rowPtrs;
                source.close();
                return out;
            } else {
                LOGI("Couldn't find file: %s\n", filename.c_str());
                return out;
            }
        }
        int getScreenWidth()  { return g_winWidth; }
        int getScreenHeight() { return g_winHeight; }
        float getPixelsPerMillimeter() { return 4.0f; }
        void saveScreenshot(const std::string&, int, int) {}
        void finish() {}
    };

    AppPlatform_unix* platform = new AppPlatform_unix();
    appContext.platform = platform;

    App* app = new MAIN_CLASS();
    ((MAIN_CLASS*)app)->externalStoragePath = std::string(getenv("HOME") ? getenv("HOME") : ".") + "/.pewiku/";
    ((MAIN_CLASS*)app)->externalCacheStoragePath = ((MAIN_CLASS*)app)->externalStoragePath;

    app->init(appContext);
    app->setSize(g_winWidth, g_winHeight);

    SDL_StartTextInput();

    while (g_running_unix && !app->wantToQuit()) {
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            handleSDLEvent(ev, app);
        }
        app->update();
        SDL_GL_SwapWindow(g_win);
    }

    SDL_StopTextInput();

    delete app;
    platform->finish();
    delete platform;

    SDL_GL_DeleteContext(g_glContext);
    SDL_DestroyWindow(g_win);
    SDL_Quit();

    return 0;
}

#endif /*MAIN_UNIX_H__*/