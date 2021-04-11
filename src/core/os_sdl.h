#include <stdio.h>

#include <SDL.h>

static struct {
  SDL_Window* window;
  SDL_GLContext context;
  fn_quit* onQuitRequest;
  fn_focus* onWindowFocus;
  fn_resize* onWindowResize;
  fn_key* onKeyboardEvent;
  fn_text* onTextEvent;
} sdlState;

static void onError(int code, const char* description) {
  SDL_Log("GLFW error %d: %s", code, description);
}

static void onWindowClose() {
  if (sdlState.onQuitRequest) {
    sdlState.onQuitRequest();
  }
}

static void onWindowFocus(SDL_Window* window, int focused) {
  if (sdlState.onWindowFocus) {
    sdlState.onWindowFocus(focused);
  }
}

static void onWindowResize(SDL_Window* window, int width, int height) {
  if (sdlState.onWindowResize) {
    SDL_GL_GetDrawableSize(window, &width, &height);
    sdlState.onWindowResize(width, height);
  }
}

static void onKeyboardEvent(SDL_Event *event) {
  if (sdlState.onKeyboardEvent) {
    os_key key;
    switch (event->key.keysym.sym) {
      case SDLK_a: key = KEY_A; break;
      case SDLK_b: key = KEY_B; break;
      case SDLK_c: key = KEY_C; break;
      case SDLK_d: key = KEY_D; break;
      case SDLK_e: key = KEY_E; break;
      case SDLK_f: key = KEY_F; break;
      case SDLK_g: key = KEY_G; break;
      case SDLK_h: key = KEY_H; break;
      case SDLK_i: key = KEY_I; break;
      case SDLK_j: key = KEY_J; break;
      case SDLK_k: key = KEY_K; break;
      case SDLK_l: key = KEY_L; break;
      case SDLK_m: key = KEY_M; break;
      case SDLK_n: key = KEY_N; break;
      case SDLK_o: key = KEY_O; break;
      case SDLK_p: key = KEY_P; break;
      case SDLK_q: key = KEY_Q; break;
      case SDLK_r: key = KEY_R; break;
      case SDLK_s: key = KEY_S; break;
      case SDLK_t: key = KEY_T; break;
      case SDLK_u: key = KEY_U; break;
      case SDLK_v: key = KEY_V; break;
      case SDLK_w: key = KEY_W; break;
      case SDLK_x: key = KEY_X; break;
      case SDLK_y: key = KEY_Y; break;
      case SDLK_z: key = KEY_Z; break;
      case SDLK_0: key = KEY_0; break;
      case SDLK_1: key = KEY_1; break;
      case SDLK_2: key = KEY_2; break;
      case SDLK_3: key = KEY_3; break;
      case SDLK_4: key = KEY_4; break;
      case SDLK_5: key = KEY_5; break;
      case SDLK_6: key = KEY_6; break;
      case SDLK_7: key = KEY_7; break;
      case SDLK_8: key = KEY_8; break;
      case SDLK_9: key = KEY_9; break;

      case SDLK_SPACE: key = KEY_SPACE; break;
      case SDLK_RETURN: key = KEY_ENTER; break;
      case SDLK_TAB: key = KEY_TAB; break;
      case SDLK_ESCAPE: key = KEY_ESCAPE; break;
      case SDLK_BACKSPACE: key = KEY_BACKSPACE; break;
      case SDLK_UP: key = KEY_UP; break;
      case SDLK_DOWN: key = KEY_DOWN; break;
      case SDLK_LEFT: key = KEY_LEFT; break;
      case SDLK_RIGHT: key = KEY_RIGHT; break;
      case SDLK_HOME: key = KEY_HOME; break;
      case SDLK_END: key = KEY_END; break;
      case SDLK_PAGEUP: key = KEY_PAGE_UP; break;
      case SDLK_PAGEDOWN: key = KEY_PAGE_DOWN; break;
      case SDLK_INSERT: key = KEY_INSERT; break;
      case SDLK_DELETE: key = KEY_DELETE; break;
      case SDLK_F1: key = KEY_F1; break;
      case SDLK_F2: key = KEY_F2; break;
      case SDLK_F3: key = KEY_F3; break;
      case SDLK_F4: key = KEY_F4; break;
      case SDLK_F5: key = KEY_F5; break;
      case SDLK_F6: key = KEY_F6; break;
      case SDLK_F7: key = KEY_F7; break;
      case SDLK_F8: key = KEY_F8; break;
      case SDLK_F9: key = KEY_F9; break;
      case SDLK_F10: key = KEY_F10; break;
      case SDLK_F11: key = KEY_F11; break;
      case SDLK_F12: key = KEY_F12; break;

      case SDLK_BACKQUOTE: key = KEY_BACKTICK; break;
      case SDLK_MINUS: key = KEY_MINUS; break;
      case SDLK_EQUALS: key = KEY_EQUALS; break;
      case SDLK_LEFTBRACKET: key = KEY_LEFT_BRACKET; break;
      case SDLK_RIGHTBRACKET: key = KEY_RIGHT_BRACKET; break;
      case SDLK_BACKSLASH: key = KEY_BACKSLASH; break;
      case SDLK_SEMICOLON: key = KEY_SEMICOLON; break;
      case SDLK_QUOTE: key = KEY_APOSTROPHE; break;
      case SDLK_COMMA: key = KEY_COMMA; break;
      case SDLK_PERIOD: key = KEY_PERIOD; break;
      case SDLK_SLASH: key = KEY_SLASH; break;

      case SDLK_LCTRL: key = KEY_LEFT_CONTROL; break;
      case SDLK_LSHIFT: key = KEY_LEFT_SHIFT; break;
      case SDLK_LALT: key = KEY_LEFT_ALT; break;
      case SDLK_LGUI: key = KEY_LEFT_OS; break;
      case SDLK_RCTRL: key = KEY_RIGHT_CONTROL; break;
      case SDLK_RSHIFT: key = KEY_RIGHT_SHIFT; break;
      case SDLK_RALT: key = KEY_RIGHT_ALT; break;
      case SDLK_RGUI: key = KEY_RIGHT_OS; break;

      case SDLK_CAPSLOCK: key = KEY_CAPS_LOCK; break;
      case SDLK_SCROLLLOCK: key = KEY_SCROLL_LOCK; break;
      case SDLK_NUMLOCKCLEAR: key = KEY_NUM_LOCK; break;

      default: return;
    }
    os_button_action action = (event->type == SDL_KEYUP) ? BUTTON_RELEASED : BUTTON_PRESSED;
//    bool repeat = (a == GLFW_REPEAT);
    bool repeat = false;
    sdlState.onKeyboardEvent(action, key, event->key.keysym.scancode, repeat);
  }
}

//static void onTextEvent(SDL_Window* window, unsigned int codepoint) {
//  if (sdlState.onTextEvent) {
//    sdlState.onTextEvent(codepoint);
//  }
//}

static int convertMouseButton(os_mouse_button button) {
  switch (button) {
    case MOUSE_LEFT: return SDL_BUTTON_LEFT;
    case MOUSE_RIGHT: return SDL_BUTTON_RIGHT;
    default: return -1;
  }
}

static int convertKey(os_key key) {
  switch (key) {
    case KEY_W: return SDL_SCANCODE_W;
    case KEY_A: return SDL_SCANCODE_A;
    case KEY_S: return SDL_SCANCODE_S;
    case KEY_D: return SDL_SCANCODE_D;
    case KEY_Q: return SDL_SCANCODE_Q;
    case KEY_E: return SDL_SCANCODE_E;
    case KEY_UP: return SDL_SCANCODE_UP;
    case KEY_DOWN: return SDL_SCANCODE_DOWN;
    case KEY_LEFT: return SDL_SCANCODE_LEFT;
    case KEY_RIGHT: return SDL_SCANCODE_RIGHT;
    case KEY_ESCAPE: return SDL_SCANCODE_ESCAPE;
    case KEY_F5: return SDL_SCANCODE_F5;
    default: return SDL_SCANCODE_UNKNOWN;
  }
}

void os_poll_events() {
  if (sdlState.window) {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT) {
        onWindowClose();
      } else if (event.type == SDL_KEYUP || event.type == SDL_KEYDOWN) {
        onKeyboardEvent(&event);
      }
    }
//    glfwPollEvents();
  }
}

bool os_window_open(const os_window_config* config) {
  if (sdlState.window) {
    return true;
  }

  SDL_DisplayMode mode;
  SDL_Init(SDL_INIT_VIDEO);
  SDL_GetCurrentDisplayMode(0, &mode);

  int index;
  for(int index = 0; index < SDL_GetNumVideoDisplays(); ++index) {
    int should_be_zero = SDL_GetCurrentDisplayMode(index, &mode);
    if(should_be_zero != 0) {
      SDL_Log("Could not get display mode for video display #%d: %s", index, SDL_GetError());
    } else {
      SDL_Log("Display #%d: current display mode is %dx%dpx @ %dhz.", index, mode.w, mode.h, mode.refresh_rate);
//	    if ((mode.w == HMD_WIDTH && mode.h == HMD_HEIGHT) ||
//		(mode.w == HMD_HEIGHT && mode.h == HMD_WIDTH)) {
//		HMDindex = i;
//		HMDwidth = mode.w;
//		HMDheight = mode.h;
//		SDL_Log("HMD found");
//	    }
      break;
    }
  }

  SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 16);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);

  uint32_t width = config->width ? config->width : (uint32_t) mode.w;
  uint32_t height = config->height ? config->height : (uint32_t) mode.h;

//  sdlState.window = glfwCreateWindow(width, height, config->title, config->fullscreen ? monitor : NULL, NULL);

  sdlState.window = SDL_CreateWindow("VRDESKTOP", SDL_WINDOWPOS_UNDEFINED_DISPLAY(index), SDL_WINDOWPOS_UNDEFINED_DISPLAY(index),
                            width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS);

  if(!sdlState.window) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window creation fail : %s", SDL_GetError());
    return false;
  }

  sdlState.context = SDL_GL_CreateContext(sdlState.window);
  if (!sdlState.context) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create GL context : %s\n", SDL_GetError());
    return false;
  }

  SDL_GL_MakeCurrent(sdlState.window, sdlState.context);

//  if (config->icon.data) {
//    glfwSetWindowIcon(sdlState.window, 1, &(GLFWimage) {
//      .pixels = config->icon.data,
//      .width = config->icon.width,
//      .height = config->icon.height
//    });
//  }

//  glfwMakeContextCurrent(sdlState.window);
//  glfwSetWindowCloseCallback(sdlState.window, onWindowClose);
//  glfwSetWindowFocusCallback(sdlState.window, onWindowFocus);
//  glfwSetWindowSizeCallback(sdlState.window, onWindowResize);
//  glfwSetKeyCallback(sdlState.window, onKeyboardEvent);
//  glfwSetCharCallback(sdlState.window, onTextEvent);
  os_window_set_vsync(config->vsync);
  return true;
}

bool os_window_is_open() {
  return sdlState.window;
}

void os_window_get_size(int* width, int* height) {
  if (sdlState.window) {
    SDL_GetWindowSize(sdlState.window, width, height);
  } else {
    if (*width) *width = 0;
    if (*height) *height = 0;
  }
}

void os_window_get_fbsize(int* width, int* height) {
  if (sdlState.window) {
    SDL_GL_GetDrawableSize(sdlState.window, width, height);
  } else {
    if (*width) *width = 0;
    if (*height) *height = 0;
  }
}

void os_window_set_vsync(int interval) {
//#if EMSCRIPTEN
//  glfwSwapInterval(1);
//#else
//  glfwSwapInterval(interval);
//#endif
}

void os_window_swap() {
  SDL_GL_SwapWindow(sdlState.window);
}

fn_gl_proc* os_get_gl_proc_address(const char* function) {
  return (fn_gl_proc*) SDL_GL_GetProcAddress(function);
}

void os_on_quit(fn_quit* callback) {
  sdlState.onQuitRequest = callback;
}

void os_on_focus(fn_focus* callback) {
  sdlState.onWindowFocus = callback;
}

void os_on_resize(fn_resize* callback) {
  sdlState.onWindowResize = callback;
}

void os_on_key(fn_key* callback) {
  sdlState.onKeyboardEvent = callback;
}

void os_on_text(fn_text* callback) {
  sdlState.onTextEvent = callback;
}

void os_get_mouse_position(double* x, double* y) {
  if (sdlState.window) {
    int ix, iy;
    SDL_GetMouseState(&ix, &iy);
    *x = ix;
    *y = iy;
  } else {
    *x = *y = 0.;
  }
}

void os_set_mouse_mode(os_mouse_mode mode) {
  if (sdlState.window) {
//    int m = (mode == MOUSE_MODE_GRABBED) ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL;
//    glfwSetInputMode(sdlState.window, GLFW_CURSOR, m);
    SDL_ShowCursor((mode == MOUSE_MODE_GRABBED) ? SDL_DISABLE : SDL_ENABLE);
  }
}

bool os_is_mouse_down(os_mouse_button button) {
  return sdlState.window ? (SDL_GetMouseState(NULL, NULL) &  SDL_BUTTON(convertMouseButton(button))) : false;
}

bool os_is_key_down(os_key key) {
  const Uint8 *state = SDL_GetKeyboardState(NULL);
  return sdlState.window ?  state[convertKey(key)] : false;
}

#ifdef _WIN32
HANDLE os_get_win32_window() {
  return (HANDLE) glfwGetWin32Window(sdlState.window);
}

HGLRC os_get_context() {
  return glfwGetWGLContext(sdlState.window);
}
#endif

#ifdef LOVR_LINUX_EGL
//PFNEGLGETPROCADDRESSPROC os_get_egl_proc_addr() {
//  return (PFNEGLGETPROCADDRESSPROC) glfwGetProcAddress;
//}

//EGLDisplay os_get_egl_display() {
//  return glfwGetEGLDisplay();
//}

//EGLContext os_get_egl_context() {
//  return glfwGetEGLContext(sdlState.window);
//}

//EGLConfig os_get_egl_config() {
//  EGLDisplay dpy = os_get_egl_display();
//  EGLContext ctx = os_get_egl_context();
//  EGLint cfg_id = -1;
//  EGLint num_cfgs = -1;
//  EGLConfig cfg = NULL;
//  PFNEGLQUERYCONTEXTPROC eglQueryContext = (PFNEGLQUERYCONTEXTPROC)glfwGetProcAddress("eglQueryContext");
//  PFNEGLCHOOSECONFIGPROC eglChooseConfig = (PFNEGLCHOOSECONFIGPROC)glfwGetProcAddress("eglChooseConfig");

//  eglQueryContext(dpy, ctx, EGL_CONFIG_ID, &cfg_id);
//  EGLint attrs [4] = {
//    EGL_CONFIG_ID, cfg_id,
//    EGL_NONE, EGL_NONE,
//  };
//  eglChooseConfig(dpy, attrs, &cfg, 1, &num_cfgs);
//  return cfg;
//}
#endif

#ifdef LOVR_LINUX_X11
//Display* os_get_x11_display() {
//  return glfwGetX11Display();
//}

//GLXDrawable os_get_glx_drawable() {
//  return glfwGetGLXWindow(sdlState.window);
//}

//GLXContext os_get_glx_context() {
//  return glfwGetGLXContext(sdlState.window);
//}
#endif
