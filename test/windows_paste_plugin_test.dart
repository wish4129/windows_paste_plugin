import 'package:flutter_test/flutter_test.dart';
import 'package:windows_paste_plugin/windows_paste_plugin.dart';
import 'package:windows_paste_plugin/windows_paste_plugin_platform_interface.dart';
import 'package:windows_paste_plugin/windows_paste_plugin_method_channel.dart';
import 'package:plugin_platform_interface/plugin_platform_interface.dart';

class MockWindowsPastePluginPlatform
    with MockPlatformInterfaceMixin
    implements WindowsPastePluginPlatform {

  @override
  Future<String?> getPlatformVersion() => Future.value('42');
}

void main() {
  final WindowsPastePluginPlatform initialPlatform = WindowsPastePluginPlatform.instance;

  test('$MethodChannelWindowsPastePlugin is the default instance', () {
    expect(initialPlatform, isInstanceOf<MethodChannelWindowsPastePlugin>());
  });

  test('getPlatformVersion', () async {
    WindowsPastePlugin windowsPastePlugin = WindowsPastePlugin();
    MockWindowsPastePluginPlatform fakePlatform = MockWindowsPastePluginPlatform();
    WindowsPastePluginPlatform.instance = fakePlatform;

    expect(await windowsPastePlugin.getPlatformVersion(), '42');
  });
}
