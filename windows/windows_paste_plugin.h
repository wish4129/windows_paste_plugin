#ifndef FLUTTER_PLUGIN_WINDOWS_PASTE_PLUGIN_H_
#define FLUTTER_PLUGIN_WINDOWS_PASTE_PLUGIN_H_

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>

#include <memory>

namespace windows_paste_plugin {

class WindowsPastePlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

  WindowsPastePlugin();

  virtual ~WindowsPastePlugin();

  // Disallow copy and assign.
  WindowsPastePlugin(const WindowsPastePlugin&) = delete;
  WindowsPastePlugin& operator=(const WindowsPastePlugin&) = delete;

  // Called when a method is called on this plugin's channel from Dart.
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
};

}  // namespace windows_paste_plugin

#endif  // FLUTTER_PLUGIN_WINDOWS_PASTE_PLUGIN_H_
