#include "shellb.h"
#include <string.h>
#include <stdbool.h>

bool shellb_g_reading = false;
bool shellb_g_command = false;
char shellb_g_buffer[SHELLB_BUFFER_SIZE];
shellb_cmd_table_t* shellb_cmd_table;

void shellb_init()
{
  shellb_cmd_table = NULL;

  shellb_print_header();
}

void shellb_uninit()
{
  shellb_platform_uninit();
}

void shellb_wait_for_cmd()
{
  if(shellb_g_reading || shellb_g_command)
    return;

  shellb_g_reading = true;
  shellb_g_command = false;

  shellb_platform_write("> ");
  shellb_platform_read();
}

void shellb_process_cmd()
{
  if(shellb_g_reading)
    return;
  shellb_g_command = false;

  argv_t argv[SHELLB_MAX_ARGS] = {0};
  uint8_t argc = shellb_create_argv(argv);
  if (argc == 0) {
    shellb_print_error("invalid input");
    return;
  }

  shellb_cmd_t *cmd = shellb_get_cmd(argv[0].start, argc);
  if (!cmd) {
    shellb_print_error("invalid command");
    return;
  }

  cmd->func(argc, argv);
}

void shellb_print_header()
{
  shellb_platform_write("=============================\n");
  shellb_platform_write("  __       __  \n"
                        " (  / _ /// _) \n"
                        "__)/)(-((/(_)  \n"
                        "               \n");
  shellb_platform_write("Build: " __DATE__ " " __TIME__ "\n");
  shellb_platform_write("=============================\n");
}

void shellb_print_error(const char* error)
{
  shellb_platform_write("ERROR: ");
  shellb_platform_write(error);
  shellb_platform_write("\n");
}

void shellb_register_cmd(shellb_cmd_table_t* commands)
{
  shellb_cmd_table_t** curr = &shellb_cmd_table;
  if (!*curr)
  {
    *curr = commands;
    return;
  }

  while ((*curr)->_next)
    curr = (*curr)->_next;

  (*curr)->_next = commands;
}

uint8_t shellb_create_argv(argv_t* argv)
{
  uint8_t argc = 0;
  bool qouted = false;
  bool cmd = false;
  char* curr = shellb_g_buffer;
  while(*curr != '\0')
  {
    if (*curr == '"') {
      qouted ^= 1;
    }

    if (!cmd)
      argv[argc].start = curr;

    if ((*curr == ' ' && !qouted && cmd) || (*curr == '"' && !qouted)) {
      argc++;
      *curr = '\0';
      cmd = false;
      if (argc >= SHELLB_MAX_ARGS)
        break;
    }
    else if (!cmd && *curr != ' ' && !(*curr == '"' && qouted))
      cmd = true;
    curr++;
  }
  argc++;
  return argc;
}

shellb_cmd_t* shellb_get_cmd(const char* name, uint8_t argc)
{
  shellb_cmd_table_t* curr_table = shellb_cmd_table;
  while (curr_table) {
    for (int i = 0; i < curr_table->cmd_count; ++i) {
      shellb_cmd_t* curr = &curr_table->cmd_list[i];
      uint8_t cmp_len = strlen(name) <= strlen(curr->name)? strlen(name): strlen(curr->name);
      if (strncasecmp(name, curr->name, cmp_len) == 0)
        return curr;
    }
    curr_table = curr_table->_next;
  }

  return NULL;
}