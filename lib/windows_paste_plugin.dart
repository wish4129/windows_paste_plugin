import 'dart:async';
import 'package:flutter/services.dart';
import 'windows_paste_plugin_platform_interface.dart';

class WindowsPastePlugin {
  static const MethodChannel _channel = MethodChannel('windows_paste_plugin');
  static const EventChannel _eventChannel = EventChannel('windows_paste_events');

  Future<String?> getPlatformVersion() {
    return WindowsPastePluginPlatform.instance.getPlatformVersion();
  }

  Stream<String> get onPaste {
    return _eventChannel.receiveBroadcastStream().map((dynamic event) => event as String);
  }

  Future<void> initialize() async {
    await _channel.invokeMethod('initialize');
  }
}

final windowsPastePlugin = WindowsPastePlugin();