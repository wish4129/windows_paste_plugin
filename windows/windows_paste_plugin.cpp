#include "windows_paste_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>
#include <flutter/event_channel.h>
#include <flutter/event_sink.h>

#include <memory>
#include <sstream>

namespace windows_paste_plugin {

// Add this line to declare event_sink
std::unique_ptr<flutter::EventSink<flutter::EncodableValue>> event_sink;

class WindowsPasteStreamHandler : public flutter::StreamHandler<flutter::EncodableValue> {
 public:
  WindowsPasteStreamHandler() = default;
  virtual ~WindowsPasteStreamHandler() = default;

  std::unique_ptr<flutter::StreamHandlerError<flutter::EncodableValue>> OnListenInternal(
      const flutter::EncodableValue* arguments,
      std::unique_ptr<flutter::EventSink<flutter::EncodableValue>>&& events) override {
    event_sink = std::move(events);
    return nullptr;
  }

  std::unique_ptr<flutter::StreamHandlerError<flutter::EncodableValue>> OnCancelInternal(
      const flutter::EncodableValue* arguments) override {
    event_sink.reset();
    return nullptr;
  }
};

// static
void WindowsPastePlugin::RegisterWithRegistrar(
    flutter::PluginRegistrarWindows *registrar) {
  auto channel =
      std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
          registrar->messenger(), "windows_paste_plugin",
          &flutter::StandardMethodCodec::GetInstance());

  auto event_channel =
      std::make_unique<flutter::EventChannel<flutter::EncodableValue>>(
          registrar->messenger(), "windows_paste_events",
          &flutter::StandardMethodCodec::GetInstance());

  auto plugin = std::make_unique<WindowsPastePlugin>();

  channel->SetMethodCallHandler(
      [plugin_pointer = plugin.get()](const auto &call, auto result) {
        plugin_pointer->HandleMethodCall(call, std::move(result));
      });

  event_channel->SetStreamHandler(std::make_unique<WindowsPasteStreamHandler>());

  registrar->AddPlugin(std::move(plugin));
}

namespace {
HHOOK keyboard_hook = NULL;

// Helper function to convert wstring to UTF-8 string
std::string wstring_to_utf8(const std::wstring& wstr) {
    if (wstr.empty()) {
        return std::string();
    }
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string str(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &str[0], size_needed, NULL, NULL);
    return str;
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
  if (nCode >= 0) {
    if (wParam == WM_KEYDOWN) {
      KBDLLHOOKSTRUCT* pKbdStruct = (KBDLLHOOKSTRUCT*)lParam;
      if (pKbdStruct->vkCode == 'V' && GetAsyncKeyState(VK_CONTROL) & 0x8000) {
        // Ctrl+V detected, get clipboard text
        if (OpenClipboard(NULL)) {
          HANDLE hData = GetClipboardData(CF_UNICODETEXT);
          if (hData != NULL) {
            wchar_t* pszText = static_cast<wchar_t*>(GlobalLock(hData));
            if (pszText != NULL) {
              // Convert wchar_t* to UTF-8 string
              std::wstring wstr(pszText);
              std::string utf8_str = wstring_to_utf8(wstr);
              
              // Send clipboard text through event channel
              if (event_sink) {
                event_sink->Success(flutter::EncodableValue(utf8_str));
              }
              GlobalUnlock(hData);
            }
          }
          CloseClipboard();
        }
      }
    }
  }
  return CallNextHookEx(keyboard_hook, nCode, wParam, lParam);
}

}  // namespace

WindowsPastePlugin::WindowsPastePlugin() {}

WindowsPastePlugin::~WindowsPastePlugin() {}

void WindowsPastePlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue> &method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  if (method_call.method_name().compare("initialize") == 0) {
    // Set up the keyboard hook
    keyboard_hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    if (keyboard_hook == NULL) {
      result->Error("HOOK_FAILED", "Failed to set keyboard hook");
    } else {
      result->Success();
    }
  } else if (method_call.method_name().compare("getPlatformVersion") == 0) {
    std::ostringstream version_stream;
    version_stream << "Windows ";
    if (IsWindows10OrGreater()) {
      version_stream << "10+";
    } else if (IsWindows8OrGreater()) {
      version_stream << "8";
    } else if (IsWindows7OrGreater()) {
      version_stream << "7";
    }
    result->Success(flutter::EncodableValue(version_stream.str()));
  } else {
    result->NotImplemented();
  }
}

}  // namespace windows_paste_plugin
