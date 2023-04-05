#include "shellb.h"

int main() {
  shellb_init();

  shellb_wait_for_cmd();
  shellb_wait_for_cmd();

  shellb_uninit();
  return 0;
}
