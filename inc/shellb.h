#ifndef SHELLB_H
#define SHELLB_H

#include <stdint.h>

#if defined(linux) || defined(__linux__) || defined(_WIN32)
#define SHELLB_BUFFER_SIZE 256
#define SHELLB_MAX_ARGS 16
#endif

typedef struct
{
  const char* start;
  uint8_t len;
} argv_t;

typedef void(*shellb_cmd_func_t)(int argc, argv_t* argv);

typedef struct {
  const char* name;
  uint8_t argc_min;
  uint8_t argc_max;
  shellb_cmd_func_t func;
} shellb_cmd_t;

typedef struct {
  uint8_t cmd_count;
  shellb_cmd_t* cmd_list;
} shellb_cmd_table_t;

extern shellb_cmd_table_t shellb_g_cmd_table;

void shellb_platform_uninit();
void shellb_platform_write(const char* str);
void shellb_platform_read();

void shellb_init();
void shellb_uninit();
void shellb_wait_for_cmd();
void shellb_process_cmd();
uint8_t shellb_create_argv(argv_t* argv);
shellb_cmd_t* shellb_get_cmd(const char* name, uint8_t len);

#endif //SHELLB_H