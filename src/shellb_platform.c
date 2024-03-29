#include "shellb.h"
#include <stdbool.h>

extern bool shellb_g_reading;
extern bool shellb_g_command;
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
  shellb_g_command = true;
  pthread_exit(NULL);
}

void shellb_platform_read()
{
  pthread_create(&g_platform_thread_id, NULL, shellb_read_thread_func, shellb_g_buffer);
}

#endif
