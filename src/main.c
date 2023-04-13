#include "shellb.h"
#include <stdbool.h>
#include <windows.h>

bool run = true;

void say_hi(int argc, argv_t* argv)
{
  // argc counts the command too.
  if (argc == 1)
  {
    shellb_print_error("invalid syntax!");
    return;
  }

  shellb_platform_write("Hi ");
  shellb_platform_write(argv[1].start);
  shellb_platform_write("!\n");
}

void quit()
{
  shellb_platform_write("Goodbye!\n");
  run = false;
}

void other_command()
{
  shellb_platform_write("Other command in another table!\n");
}

int main() {
  // ===
  const shellb_cmd_t commands[] = {
      {"hi", 1, 1, say_hi},
      {"quit", 0, 0, quit},
  };

  shellb_cmd_table_t cmdTable;
  cmdTable.cmd_count = 2;
  cmdTable.cmd_list = commands;

  // ====
  const shellb_cmd_t commands2[] = {
      {"other", 0, 0, other_command},
  };
  shellb_cmd_table_t cmdTable2;
  cmdTable2.cmd_count = 1;
  cmdTable2.cmd_list = commands2;

  shellb_init();
  shellb_register_cmd(&cmdTable);
  shellb_register_cmd(&cmdTable2);

  while(run)
  {
    shellb_wait_for_cmd();
    shellb_process_cmd();
    Sleep(20);
  }

  shellb_uninit();
  return 0;
}
