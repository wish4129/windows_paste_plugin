
import 'windows_paste_plugin_platform_interface.dart';

class WindowsPastePlugin {
  Future<String?> getPlatformVersion() {
    return WindowsPastePluginPlatform.instance.getPlatformVersion();
  }
}
