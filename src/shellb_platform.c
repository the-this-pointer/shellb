#include "shellb.h"
#include <stdbool.h>

bool shellb_g_reading = false;
extern char shellb_g_buffer[SHELLB_BUFFER_SIZE];

#if defined(linux) || defined(__linux__) || defined(_WIN32)
#include <stdio.h>
#include <pthread.h>
pthread_t g_platform_thread_id;

void shellb_platform_uninit()
{
  pthread_join(g_platform_thread_id, NULL);
}

void shellb_platform_write(const char* str)
{
  printf(str);
}

void* shellb_read_thread_func(void* arg)
{
  scanf(" %[^\n]", arg);
  shellb_g_reading = false;
  shellb_process_cmd();
  pthread_exit(NULL);
}

void shellb_platform_read()
{
  if (shellb_g_reading)
    return;
  shellb_g_reading = true;

  pthread_create(&g_platform_thread_id, NULL, shellb_read_thread_func, shellb_g_buffer);
}

#endif
