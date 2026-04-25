#ifndef MAIN_UNIX_H__
#define MAIN_UNIX_H__

// targets generic Linux desktops using SDL2 and Desktop OpenGL

#include "App.h"
#include "NinecraftApp.h"
#include "client/renderer/gles.h"
#include "platform/input/Keyboard.h"
#include "platform/input/Mouse.h"
#include "platform/input/Multitouch.h"
#include "AppPlatform_unix.h"

#include <SDL2/SDL.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <png.h>
#include <string>
#include <unistd.h>
#include <vector>

static bool g_running_unix = true;
bool g_mouseGrabbed = false;
bool g_windowActive = true;

SDL_Window* g_win = nullptr;
SDL_GLContext g_glContext = nullptr;
int g_winWidth = 848;
int g_winHeight = 480;

static int g_savedMouseX = 0, g_savedMouseY = 0;
static bool g_isWarping = false;

void platform_setMouseGrabbed(bool grab) {
	if (!g_win)
		return;
	if (grab == g_mouseGrabbed)
		return;

	if (grab) {
		SDL_GetMouseState(&g_savedMouseX, &g_savedMouseY);
		// Sync internal coordinates to avoid jump at start
		Mouse::feed(0, 0, (short)g_savedMouseX, (short)g_savedMouseY, 0, 0);
		Multitouch::feed(0, 0, (short)g_savedMouseX, (short)g_savedMouseY, 0);

		SDL_SetWindowGrab(g_win, SDL_TRUE);
		SDL_ShowCursor(SDL_FALSE);
		// Initial warp to center
		SDL_WarpMouseInWindow(g_win, g_winWidth / 2, g_winHeight / 2);
		g_isWarping = true;
	} else {
		SDL_ShowCursor(SDL_TRUE);
		SDL_SetWindowGrab(g_win, SDL_FALSE);
		// Only warp if we have focus and are not leaving, to restore the
		// pre-grab position.
		if (g_windowActive && (SDL_GetWindowFlags(g_win) & SDL_WINDOW_INPUT_FOCUS)) {
			// Immediate sync to prevent 1-frame lag in GUI logic
			Mouse::feed(0, 0, (short)g_savedMouseX, (short)g_savedMouseY, 0, 0);
			Multitouch::feed(0, 0, (short)g_savedMouseX, (short)g_savedMouseY, 0);

			SDL_WarpMouseInWindow(g_win, g_savedMouseX, g_savedMouseY);
			g_isWarping = true;
		}
	}
	g_mouseGrabbed = grab;
}

static unsigned char transformKey(SDL_Scancode scancode, SDL_Keycode sym) {
	if (scancode >= SDL_SCANCODE_F1 && scancode <= SDL_SCANCODE_F12)
		return Keyboard::KEY_F1 + (scancode - SDL_SCANCODE_F1);

	if (scancode >= SDL_SCANCODE_A && scancode <= SDL_SCANCODE_Z)
		return 'a' + (scancode - SDL_SCANCODE_A);
	if (scancode >= SDL_SCANCODE_1 && scancode <= SDL_SCANCODE_9)
		return '1' + (scancode - SDL_SCANCODE_1);
	if (scancode == SDL_SCANCODE_0)
		return '0';

	switch (sym) {
	case SDLK_ESCAPE:
		return 27;
	case SDLK_SPACE:
		return 32;
	case SDLK_RETURN:
		return 13;
	case SDLK_BACKSPACE:
		return 8;
	case SDLK_TAB:
		return 9;
	case SDLK_LSHIFT:
	case SDLK_RSHIFT:
		return Keyboard::KEY_LSHIFT;
	case SDLK_LCTRL:
	case SDLK_RCTRL:
		return Keyboard::KEY_LCTRL;
	case SDLK_UP:
		return 38;
	case SDLK_DOWN:
		return 40;
	case SDLK_LEFT:
		return 37;
	case SDLK_RIGHT:
		return 39;
	case SDLK_LALT:
	case SDLK_RALT:
		return 18;
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
			glViewport(0, 0, g_winWidth, g_winHeight);
			if (app)
				app->setSize(g_winWidth, g_winHeight);
		} else if (ev.window.event == SDL_WINDOWEVENT_FOCUS_GAINED || ev.window.event == SDL_WINDOWEVENT_ENTER) {
			g_windowActive = true;
		} else if (ev.window.event == SDL_WINDOWEVENT_FOCUS_LOST || ev.window.event == SDL_WINDOWEVENT_LEAVE) {
			g_windowActive = false;
		}
		break;

	case SDL_KEYDOWN:
	case SDL_KEYUP: {
		bool pressed = (ev.type == SDL_KEYDOWN);
		if (pressed && ev.key.keysym.sym == SDLK_F11) {
			Uint32 flags = SDL_GetWindowFlags(g_win);
			bool isFullscreen = (flags & (SDL_WINDOW_FULLSCREEN | SDL_WINDOW_FULLSCREEN_DESKTOP)) != 0;
			SDL_SetWindowFullscreen(g_win, isFullscreen ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
			break;
		}
		unsigned char transformed = transformKey(ev.key.keysym.scancode, ev.key.keysym.sym);
		if (transformed) {
			Keyboard::feed(transformed, pressed ? KeyboardAction::KEYDOWN : KeyboardAction::KEYUP);
		}
		break;
	}

	case SDL_TEXTINPUT: {
		for (int i = 0; ev.text.text[i] != '\0'; ++i) {
			unsigned char ch = static_cast<unsigned char>(ev.text.text[i]);
			if (ch >= 32)
				Keyboard::feedText(ch);
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
		if (button == SDL_BUTTON_LEFT)
			action = 1;
		else if (button == SDL_BUTTON_RIGHT)
			action = 2;

		if (action) {
			int state = (ev.type == SDL_MOUSEBUTTONDOWN) ? 1 : 0;
			Mouse::feed(action, state, Mouse::getX(), Mouse::getY());
			Multitouch::feed(action, state, Mouse::getX(), Mouse::getY(), 0);
		}
		break;
	}

	case SDL_MOUSEMOTION: {
		if (g_isWarping) {
			g_isWarping = false;
			break;
		}
		if (g_mouseGrabbed) {
			int xrel = ev.motion.x - (g_winWidth / 2);
			int yrel = ev.motion.y - (g_winHeight / 2);

			if (xrel != 0 || yrel != 0) {
				int nextX = Mouse::getX() + xrel;
				int nextY = Mouse::getY() + yrel;
				if (nextX < 0)
					nextX = 0;
				if (nextX > g_winWidth)
					nextX = g_winWidth;
				if (nextY < 0)
					nextY = 0;
				if (nextY > g_winHeight)
					nextY = g_winHeight;

				Mouse::feed(0, 0, (short)nextX, (short)nextY, (short)xrel, (short)yrel);
				Multitouch::feed(0, 0, (short)nextX, (short)nextY, 0);

				SDL_WarpMouseInWindow(g_win, g_winWidth / 2, g_winHeight / 2);
				g_isWarping = true;
			}
		} else {
			Mouse::feed(0, 0, (short)ev.motion.x, (short)ev.motion.y, 0, 0);
			Multitouch::feed(0, 0, (short)ev.motion.x, (short)ev.motion.y, 0);
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

#ifdef DEBUG
	const char* title = "Minecraft PE: Pewiku, v0.6.1-dev";
#else
	const char* title = "Minecraft PE: Pewiku, v0.6.1";
#endif

	g_win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
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
	SDL_ShowCursor(SDL_ENABLE);
#endif

	glInit();

	AppContext appContext;
	appContext.doRender = true;

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
		Multitouch::commit();
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