#include "os.h"
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>

#ifdef LOVR_USE_SDL
#include "os_sdl.h"
#else
#include "os_glfw.h"
#endif

#define NS_PER_SEC 1000000000ULL

bool os_init() {
  return true;
}

void os_destroy() {
#ifdef LOVR_USE_SDL
  SDL_Quit();
#else
  glfwTerminate();
#endif
}

const char* os_get_name() {
  return "Linux";
}

uint32_t os_get_core_count() {
  return sysconf(_SC_NPROCESSORS_ONLN);
}

void os_open_console() {
  //
}

double os_get_time() {
  struct timespec t;
  clock_gettime(CLOCK_MONOTONIC, &t);
  return (double) t.tv_sec + (t.tv_nsec / (double) NS_PER_SEC);
}

void os_sleep(double seconds) {
  seconds += .5e-9;
  struct timespec t;
  t.tv_sec = seconds;
  t.tv_nsec = (seconds - t.tv_sec) * NS_PER_SEC;
  while (nanosleep(&t, &t));
}

void os_request_permission(os_permission permission) {
  //
}

void os_on_permission(fn_permission* callback) {
  //
}

size_t os_get_home_directory(char* buffer, size_t size) {
  const char* path = getenv("HOME");

  if (!path) {
    struct passwd* entry = getpwuid(getuid());
    if (!entry) {
      return 0;
    }
    path = entry->pw_dir;
  }

  size_t length = strlen(path);
  if (length >= size) { return 0; }
  memcpy(buffer, path, length);
  buffer[length] = '\0';
  return length;
}

size_t os_get_data_directory(char* buffer, size_t size) {
  const char* xdg = getenv("XDG_DATA_HOME");

  if (xdg) {
    size_t length = strlen(xdg);
    if (length < size) {
      memcpy(buffer, xdg, length);
      buffer[length] = '\0';
      return length;
    }
  } else {
    size_t cursor = os_get_home_directory(buffer, size);
    if (cursor > 0) {
      buffer += cursor;
      size -= cursor;
      const char* suffix = "/.local/share";
      size_t length = strlen(suffix);
      if (length < size) {
        memcpy(buffer, suffix, length);
        buffer[length] = '\0';
        return cursor + length;
      }
    }
  }

  return 0;
}

size_t os_get_working_directory(char* buffer, size_t size) {
  return getcwd(buffer, size) ? strlen(buffer) : 0;
}

size_t os_get_executable_path(char* buffer, size_t size) {
  ssize_t length = readlink("/proc/self/exe", buffer, size - 1);
  if (length >= 0) {
    buffer[length] = '\0';
    return length;
  } else {
    return 0;
  }
}

size_t os_get_bundle_path(char* buffer, size_t size, const char** root) {
  *root = NULL;
  return os_get_executable_path(buffer, size);
}
