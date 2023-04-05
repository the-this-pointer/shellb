#include "shellb.h"
#include <string.h>
#include <stdbool.h>

char shellb_g_buffer[SHELLB_BUFFER_SIZE];
shellb_cmd_table_t shellb_g_cmd_table;

void shellb_init()
{
}

void shellb_uninit()
{
  shellb_platform_uninit();
}

void shellb_wait_for_cmd()
{
  shellb_platform_write(">");
  shellb_platform_read();
}

void shellb_process_cmd()
{
  argv_t argv[SHELLB_MAX_ARGS] = {0};
  uint8_t argc = shellb_create_argv(argv);
  if (argc == 0)
    return;

  shellb_cmd_t *cmd = shellb_get_cmd(argv[0].start, argv[0].len);
  if (!cmd)
    return;

  cmd->func(argc, argv);
}

uint8_t shellb_create_argv(argv_t* argv)
{
  uint8_t argc = 0;
  bool qouted = false;
  char* curr = shellb_g_buffer;
  while(*curr != '\0')
  {
    if (*curr == '"') {
      qouted ^= 1;
    }

    if (argv[argc].len == 0)
      argv[argc].start = curr;

    if ((*curr == ' ' && !qouted && argv[argc].len > 0) || (*curr == '"' && !qouted)) {
      argc++;
      if (argc >= SHELLB_MAX_ARGS)
        break;
    }

    argv[argc].len++;
  }
  argc++;
  return argc;
}

shellb_cmd_t* shellb_get_cmd(const char* name, uint8_t len)
{
  for (int i = 0; i < shellb_g_cmd_table.cmd_count; ++i) {
    shellb_cmd_t* curr = &shellb_g_cmd_table.cmd_list[i];
    uint8_t cmp_len = len <= strlen(curr->name)? len: strlen(curr->name);
    if (strncasecmp(name, curr->name, cmp_len) == 0)
      return curr;
  }

  return NULL;
}