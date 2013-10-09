#include "xchat-plugin.h"

#define PNAME "StartupTray"
#define PDESC "Automatically minimizes X-Chat to the taskbar at startup."
#define PVERSION "0.1"

static xchat_plugin *ph;
static xchat_hook *connecting_hook;
static xchat_hook *server_lookup_hook;

static xchat_hook *timeout_hook;
static unsigned char minimized = 0;

static int print_cb(char *word[], void *user_data)
{
  if (0 == minimized)
  {
    // Minimize to taskbar on the first print
    minimized = 1;
    xchat_command(ph, "GUI ICONIFY");
  }
  // Only listen to the first print of each type
  xchat_unhook(ph, *((xchat_hook**)user_data));
  return XCHAT_EAT_NONE;
}

static int timeout_cb(void *user_data)
{
  xchat_unhook(ph, timeout_hook);
  // Mark it as minimized
  minimized = 1;
  return XCHAT_EAT_ALL;
}

int xchat_plugin_init(xchat_plugin *plugin_handle, char **plugin_name, char **plugin_desc, char **plugin_version, char *arg)
{
  ph = plugin_handle;
  *plugin_name = PNAME;
  *plugin_desc = PDESC;
  *plugin_version = PVERSION;
  
  // Set up hooks, when they are triggered the client will become minimized
  connecting_hook = xchat_hook_print(ph, "Connecting", XCHAT_PRI_NORM, print_cb, &connecting_hook);
  server_lookup_hook = xchat_hook_print(ph, "Server Lookup", XCHAT_PRI_NORM, print_cb, &server_lookup_hook);
  // TODO: Add more possible triggers?
  
  // Set up a timer to prevent it from minimizing if too long time has passed
  timeout_hook = xchat_hook_timer(ph, 10000, timeout_cb, NULL);
  
  xchat_print(ph, "StartupTray loaded\n");
  return 1;
}
