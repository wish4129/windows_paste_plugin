import 'package:plugin_platform_interface/plugin_platform_interface.dart';

import 'windows_paste_plugin_method_channel.dart';

abstract class WindowsPastePluginPlatform extends PlatformInterface {
  /// Constructs a WindowsPastePluginPlatform.
  WindowsPastePluginPlatform() : super(token: _token);

  static final Object _token = Object();

  static WindowsPastePluginPlatform _instance = MethodChannelWindowsPastePlugin();

  /// The default instance of [WindowsPastePluginPlatform] to use.
  ///
  /// Defaults to [MethodChannelWindowsPastePlugin].
  static WindowsPastePluginPlatform get instance => _instance;

  /// Platform-specific implementations should set this with their own
  /// platform-specific class that extends [WindowsPastePluginPlatform] when
  /// they register themselves.
  static set instance(WindowsPastePluginPlatform instance) {
    PlatformInterface.verifyToken(instance, _token);
    _instance = instance;
  }

  Future<String?> getPlatformVersion() {
    throw UnimplementedError('platformVersion() has not been implemented.');
  }
}
