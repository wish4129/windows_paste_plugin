#include "include/windows_paste_plugin/windows_paste_plugin_c_api.h"

#include <flutter/plugin_registrar_windows.h>

#include "windows_paste_plugin.h"

void WindowsPastePluginCApiRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  windows_paste_plugin::WindowsPastePlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
