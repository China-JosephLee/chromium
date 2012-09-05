# Copyright (c) 2012 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'targets': [
    {
      'target_name': 'test_support_content',
      'type': 'static_library',
      'defines!': ['CONTENT_IMPLEMENTATION'],
      'dependencies': [
        '../build/temp_gyp/googleurl.gyp:googleurl',
        'content_app',
        'content_browser',
        'content_common',
        '../net/net.gyp:net_test_support',
        '../skia/skia.gyp:skia',
        '../testing/gmock.gyp:gmock',
        '../testing/gtest.gyp:gtest',
        '<(webkit_src_dir)/Source/WebKit/chromium/WebKit.gyp:webkit',
        '../ui/surface/surface.gyp:surface',
        '../ui/ui.gyp:ui_test_support',
        '../webkit/support/webkit_support.gyp:appcache',
      ],
      'include_dirs': [
        '..',
      ],
      'sources': [
        'public/test/accessibility_test_utils_win.cc',
        'public/test/accessibility_test_utils_win.h',
        'public/test/browser_test.h',
        'public/test/browser_test_base.cc',
        'public/test/browser_test_base.h',
        'public/test/browser_test_utils.cc',
        'public/test/browser_test_utils.h',
        'public/test/content_test_suite_base.cc',
        'public/test/content_test_suite_base.h',
        'public/test/download_test_observer.cc',
        'public/test/download_test_observer.h',
        'public/test/js_injection_ready_observer.h',
        'public/test/mock_download_item.cc',
        'public/test/mock_download_item.h',
        'public/test/mock_download_manager.cc',
        'public/test/mock_download_manager.h',
        'public/test/mock_notification_observer.cc',
        'public/test/mock_notification_observer.h',
        'public/test/mock_render_process_host.cc',
        'public/test/mock_render_process_host.h',
        'public/test/mock_render_thread.cc',
        'public/test/mock_render_thread.h',
        'public/test/mock_resource_context.cc',
        'public/test/mock_resource_context.h',
        'public/test/render_view_fake_resources_test.cc',
        'public/test/render_view_fake_resources_test.h',
        'public/test/render_view_test.cc',
        'public/test/render_view_test.h',
        'public/test/render_widget_browsertest.cc',
        'public/test/render_widget_browsertest.h',
        'public/test/test_browser_context.cc',
        'public/test/test_browser_context.h',
        'public/test/test_browser_thread.cc',
        'public/test/test_browser_thread.h',
        'public/test/test_content_client_initializer.cc',
        'public/test/test_content_client_initializer.h',
        'public/test/test_file_error_injector.cc',
        'public/test/test_file_error_injector.h',
        'public/test/test_launcher.cc',
        'public/test/test_launcher.h',
        'public/test/test_navigation_observer.cc',
        'public/test/test_navigation_observer.h',
        'public/test/test_notification_tracker.cc',
        'public/test/test_notification_tracker.h',
        'public/test/test_renderer_host.cc',
        'public/test/test_renderer_host.h',
        'public/test/test_utils.cc',
        'public/test/test_utils.h',
        'public/test/unittest_test_suite.cc',
        'public/test/unittest_test_suite.h',
        'public/test/web_contents_tester.cc',
        'public/test/web_contents_tester.h',
        'app/startup_helper_win.cc',
        # TODO(phajdan.jr): All of those files should live in content/test (if
        # they're only used by content) or content/public/test (if they're used
        # by other embedders).
        'browser/download/mock_download_file.cc',
        'browser/download/mock_download_file.h',
        'browser/geolocation/arbitrator_dependency_factories_for_test.cc',
        'browser/geolocation/arbitrator_dependency_factories_for_test.h',
        'browser/geolocation/fake_access_token_store.cc',
        'browser/geolocation/fake_access_token_store.h',
        'browser/geolocation/mock_location_provider.cc',
        'browser/geolocation/mock_location_provider.h',
        'browser/renderer_host/media/mock_media_observer.cc',
        'browser/renderer_host/media/mock_media_observer.h',
        'browser/renderer_host/test_backing_store.cc',
        'browser/renderer_host/test_backing_store.h',
        'browser/renderer_host/test_render_view_host.cc',
        'browser/renderer_host/test_render_view_host.h',
        'browser/web_contents/test_web_contents.cc',
        'browser/web_contents/test_web_contents.h',
        'common/test_url_constants.cc',
        'common/test_url_constants.h',
        'gpu/gpu_idirect3d9_mock_win.cc',
        'gpu/gpu_idirect3d9_mock_win.h',
        'test/content_test_suite.cc',
        'test/content_test_suite.h',
        'test/gpu/gpu_test_config.cc',
        'test/gpu/gpu_test_config.h',
        'test/gpu/gpu_test_expectations_parser.cc',
        'test/gpu/gpu_test_expectations_parser.h',
        'test/mock_keyboard.cc',
        'test/mock_keyboard_driver_win.cc',
        'test/mock_keyboard_driver_win.h',
        'test/mock_keyboard.h',
        'test/mock_render_process.cc',
        'test/mock_render_process.h',
        'test/net/url_request_failed_job.cc',
        'test/net/url_request_failed_job.h',
        'test/net/url_request_mock_http_job.cc',
        'test/net/url_request_mock_http_job.h',
        'test/net/url_request_slow_download_job.cc',
        'test/net/url_request_slow_download_job.h',
        'test/net/url_request_slow_http_job.cc',
        'test/net/url_request_slow_http_job.h',
        'test/net/url_request_abort_on_end_job.cc',
        'test/net/url_request_abort_on_end_job.h',
        'test/test_content_browser_client.cc',
        'test/test_content_browser_client.h',
        'test/test_content_client.cc',
        'test/test_content_client.h',
        'test/test_render_view_host_factory.cc',
        'test/test_render_view_host_factory.h',
        'test/test_web_contents_view.cc',
        'test/test_web_contents_view.h',

        # TODO(phajdan.jr): Those files should be moved to webkit
        # test support target.
        '../webkit/appcache/appcache_test_helper.cc',
        '../webkit/appcache/appcache_test_helper.h',
        '../webkit/quota/mock_special_storage_policy.cc',
        '../webkit/quota/mock_special_storage_policy.h',
      ],
      'conditions': [
        ['OS == "win" or (toolkit_uses_gtk == 1 and selinux == 0)', {
          'dependencies': [
            '../sandbox/sandbox.gyp:sandbox',
          ],
        }],
        ['enable_webrtc==1', {
          'sources': [
            'renderer/media/mock_media_stream_dependency_factory.cc',
            'renderer/media/mock_media_stream_dependency_factory.h',
            'renderer/media/mock_media_stream_dispatcher.cc',
            'renderer/media/mock_media_stream_dispatcher.h',
            'renderer/media/mock_peer_connection_impl.cc',
            'renderer/media/mock_peer_connection_impl.h',
            'renderer/media/mock_web_peer_connection_00_handler_client.cc',
            'renderer/media/mock_web_peer_connection_00_handler_client.h',
            'renderer/media/mock_web_peer_connection_handler_client.cc',
            'renderer/media/mock_web_peer_connection_handler_client.h',
            'test/webrtc_audio_device_test.cc',
            'test/webrtc_audio_device_test.h',
          ],
          'dependencies': [
            '../third_party/libjingle/libjingle.gyp:libjingle_peerconnection',
            '../third_party/webrtc/modules/modules.gyp:audio_device',
            '../third_party/webrtc/modules/modules.gyp:video_capture_module',
            '../third_party/webrtc/system_wrappers/source/system_wrappers.gyp:system_wrappers',
            '../third_party/webrtc/video_engine/video_engine.gyp:video_engine_core',
            '../third_party/webrtc/voice_engine/voice_engine.gyp:voice_engine_core'],
        }],
        ['toolkit_uses_gtk == 1', {
          'dependencies': [
            '../build/linux/system.gyp:gtk',
          ],
        }],
        ['use_glib == 1', {
          'dependencies': [
            '../build/linux/system.gyp:glib',
          ],
        }],
        ['use_aura==1', {
          'dependencies': [
            '../ui/aura/aura.gyp:test_support_aura',
            '../ui/compositor/compositor.gyp:compositor',
          ],
        }],
        ['OS=="win"', {
          'dependencies': [
            '../third_party/iaccessible2/iaccessible2.gyp:iaccessible2',
          ],
        }],
        ['OS!="android"', {
          'dependencies': [
            '../third_party/libvpx/libvpx.gyp:libvpx',
          ],
        }],
      ],
    },
    {
      'target_name': 'content_unittests',
      'type': '<(gtest_target_type)',
      'defines!': ['CONTENT_IMPLEMENTATION'],
      'dependencies': [
        'content_common',
        'content_gpu',
        'content_plugin',
        'content_renderer',
        'test_support_content',
        'browser/speech/proto/speech_proto.gyp:speech_proto',
        'content_resources.gyp:content_resources',
        '../base/base.gyp:test_support_base',
        '../base/third_party/dynamic_annotations/dynamic_annotations.gyp:dynamic_annotations',
        '../crypto/crypto.gyp:crypto',
        '../gpu/gpu.gyp:gpu_unittest_utils',
        '../ipc/ipc.gyp:test_support_ipc',
        '../jingle/jingle.gyp:jingle_glue_test_util',
        '../media/media.gyp:media_test_support',
        '../media/media.gyp:shared_memory_support',
        '../net/net.gyp:net_test_support',
        '../skia/skia.gyp:skia',
        '../testing/gmock.gyp:gmock',
        '../testing/gtest.gyp:gtest',
        '../third_party/leveldatabase/leveldatabase.gyp:leveldatabase',
        '../third_party/libjingle/libjingle.gyp:libjingle',
        '<(webkit_src_dir)/Source/WebKit/chromium/WebKit.gyp:webkit',
        '../ui/gl/gl.gyp:gl',
        '../ui/ui.gyp:ui',
        '../v8/tools/gyp/v8.gyp:v8',
        '../webkit/support/webkit_support.gyp:appcache',
        '../webkit/support/webkit_support.gyp:blob',
        '../webkit/support/webkit_support.gyp:database',
        '../webkit/support/webkit_support.gyp:dom_storage',
        '../webkit/support/webkit_support.gyp:fileapi',
        '../webkit/support/webkit_support.gyp:glue',
        '../webkit/support/webkit_support.gyp:quota',
        '../webkit/support/webkit_support.gyp:webkit_base',
        '../webkit/support/webkit_support.gyp:webkit_media',
        '../webkit/webkit.gyp:test_shell_test_support',
      ],
      'include_dirs': [
        '..',
      ],
      'sources': [
        'browser/accessibility/browser_accessibility_mac_unittest.mm',
        'browser/accessibility/browser_accessibility_manager_unittest.cc',
        'browser/accessibility/browser_accessibility_win_unittest.cc',
        'browser/appcache/chrome_appcache_service_unittest.cc',
        'browser/browser_thread_unittest.cc',
        'browser/browser_url_handler_impl_unittest.cc',
        'browser/child_process_security_policy_unittest.cc',
        'browser/debugger/devtools_manager_unittest.cc',
        'browser/device_orientation/provider_unittest.cc',
        'browser/download/base_file_unittest.cc',
        'browser/download/byte_stream_unittest.cc',
        'browser/download/download_file_manager_unittest.cc',
        'browser/download/download_file_unittest.cc',
        'browser/download/download_id_unittest.cc',
        'browser/download/download_item_impl_unittest.cc',
        'browser/download/download_manager_impl_unittest.cc',
        'browser/download/file_metadata_unittest_linux.cc',
        'browser/download/save_package_unittest.cc',
        'browser/gamepad/gamepad_provider_unittest.cc',
        'browser/geolocation/device_data_provider_unittest.cc',
        'browser/geolocation/geolocation_provider_unittest.cc',
        'browser/geolocation/gps_location_provider_unittest_linux.cc',
        'browser/geolocation/location_arbitrator_unittest.cc',
        'browser/geolocation/network_location_provider_unittest.cc',
        'browser/geolocation/wifi_data_provider_common_unittest.cc',
        'browser/geolocation/wifi_data_provider_linux_unittest.cc',
        'browser/geolocation/wifi_data_provider_unittest_win.cc',
        'browser/geolocation/win7_location_api_unittest_win.cc',
        'browser/geolocation/win7_location_provider_unittest_win.cc',
        'browser/gpu/gpu_data_manager_impl_unittest.cc',
        'browser/host_zoom_map_impl_unittest.cc',
        'browser/hyphenator/hyphenator_message_filter_unittest.cc',
        'browser/in_process_webkit/indexed_db_quota_client_unittest.cc',
        'browser/in_process_webkit/indexed_db_unittest.cc',
        'browser/in_process_webkit/webkit_thread_unittest.cc',
        'browser/intents/intent_injector_unittest.cc',
        'browser/intents/internal_web_intents_dispatcher_unittest.cc',
        'browser/mach_broker_mac_unittest.cc',
        'browser/notification_service_impl_unittest.cc',
        'browser/plugin_loader_posix_unittest.cc',
        'browser/renderer_host/gtk_key_bindings_handler_unittest.cc',
        'browser/renderer_host/media/audio_input_device_manager_unittest.cc',
        'browser/renderer_host/media/audio_renderer_host_unittest.cc',
        'browser/renderer_host/media/media_stream_device_settings_unittest.cc',
        'browser/renderer_host/media/media_stream_dispatcher_host_unittest.cc',
        'browser/renderer_host/media/video_capture_controller_unittest.cc',
        'browser/renderer_host/media/video_capture_host_unittest.cc',
        'browser/renderer_host/media/video_capture_manager_unittest.cc',
        'browser/renderer_host/render_view_host_unittest.cc',
        'browser/renderer_host/render_widget_host_unittest.cc',
        'browser/renderer_host/render_widget_host_view_aura_unittest.cc',
        'browser/renderer_host/render_widget_host_view_mac_editcommand_helper_unittest.mm',
        'browser/renderer_host/render_widget_host_view_mac_unittest.mm',
        'browser/renderer_host/resource_dispatcher_host_unittest.cc',
        'browser/renderer_host/text_input_client_mac_unittest.mm',
        'browser/renderer_host/web_input_event_aura_unittest.cc',
        'browser/resolve_proxy_msg_helper_unittest.cc',
        'browser/site_instance_impl_unittest.cc',
        'browser/speech/chunked_byte_buffer_unittest.cc',
        'browser/speech/endpointer/endpointer_unittest.cc',
        'browser/speech/google_one_shot_remote_engine_unittest.cc',
        'browser/speech/google_streaming_remote_engine_unittest.cc',
        'browser/speech/speech_recognizer_unittest.cc',
        'browser/ssl/ssl_host_state_unittest.cc',
        'browser/system_message_window_win_unittest.cc',
        'browser/trace_subscriber_stdio_unittest.cc',
        'browser/web_contents/navigation_controller_impl_unittest.cc',
        'browser/web_contents/navigation_entry_impl_unittest.cc',
        'browser/web_contents/render_view_host_manager_unittest.cc',
        'browser/web_contents/web_contents_delegate_unittest.cc',
        'browser/web_contents/web_contents_impl_unittest.cc',
        'browser/web_contents/web_contents_view_mac_unittest.mm',
        'browser/web_contents/web_drag_dest_mac_unittest.mm',
        'browser/web_contents/web_drag_source_mac_unittest.mm',
        'browser/webui/web_ui_message_handler_unittest.cc',
        'common/android/address_parser_unittest.cc',
        'common/mac/attributed_string_coder_unittest.mm',
        'common/mac/font_descriptor_unittest.mm',
        'common/gpu/gpu_info_unittest.cc',
        'common/gpu/gpu_memory_manager_unittest.cc',
        'common/gpu/media/avc_config_record_builder_unittest.cc',
        'common/indexed_db/indexed_db_dispatcher_unittest.cc',
        'common/inter_process_time_ticks_converter_unittest.cc',
        'common/page_zoom_unittest.cc',
        'common/resource_dispatcher_unittest.cc',
        'common/sandbox_mac_diraccess_unittest.mm',
        'common/sandbox_mac_fontloading_unittest.mm',
        'common/sandbox_mac_unittest_helper.h',
        'common/sandbox_mac_unittest_helper.mm',
        'common/sandbox_mac_system_access_unittest.mm',
        'gpu/gpu_info_collector_unittest.cc',
        'gpu/gpu_info_collector_unittest_win.cc',
        'renderer/active_notification_tracker_unittest.cc',
        'renderer/android/email_detector_unittest.cc',
        'renderer/android/phone_number_detector_unittest.cc',
        'renderer/gpu/input_event_filter_unittest.cc',
        'renderer/hyphenator/hyphenator_unittest.cc',
        'renderer/media/audio_message_filter_unittest.cc',
        'renderer/media/audio_renderer_mixer_manager_unittest.cc',
        'renderer/media/capture_video_decoder_unittest.cc',
        'renderer/media/video_capture_impl_unittest.cc',
        'renderer/media/video_capture_message_filter_unittest.cc',
        'renderer/paint_aggregator_unittest.cc',
        'renderer/pepper/pepper_broker_impl_unittest.cc',
        'renderer/render_thread_impl_unittest.cc',
        'renderer/v8_value_converter_impl_unittest.cc',
        'test/gpu/gpu_test_config_unittest.cc',
        'test/gpu/gpu_test_expectations_parser_unittest.cc',
        'test/run_all_unittests.cc',
        '../webkit/appcache/manifest_parser_unittest.cc',
        '../webkit/appcache/appcache_unittest.cc',
        '../webkit/appcache/appcache_database_unittest.cc',
        '../webkit/appcache/appcache_group_unittest.cc',
        '../webkit/appcache/appcache_host_unittest.cc',
        '../webkit/appcache/appcache_quota_client_unittest.cc',
        '../webkit/appcache/appcache_request_handler_unittest.cc',
        '../webkit/appcache/appcache_response_unittest.cc',
        '../webkit/appcache/appcache_service_unittest.cc',
        '../webkit/appcache/appcache_storage_unittest.cc',
        '../webkit/appcache/appcache_storage_impl_unittest.cc',
        '../webkit/appcache/appcache_update_job_unittest.cc',
        '../webkit/appcache/appcache_url_request_job_unittest.cc',
        '../webkit/appcache/mock_appcache_policy.h',
        '../webkit/appcache/mock_appcache_policy.cc',
        '../webkit/appcache/mock_appcache_service.cc',
        '../webkit/appcache/mock_appcache_service.h',
        '../webkit/appcache/mock_appcache_storage.cc',
        '../webkit/appcache/mock_appcache_storage.h',
        '../webkit/appcache/mock_appcache_storage_unittest.cc',
        '../webkit/blob/blob_storage_controller_unittest.cc',
        '../webkit/blob/blob_url_request_job_unittest.cc',
        '../webkit/blob/local_file_stream_reader_unittest.cc',
        '../webkit/blob/shareable_file_reference_unittest.cc',
        '../webkit/database/database_connections_unittest.cc',
        '../webkit/database/database_quota_client_unittest.cc',
        '../webkit/database/databases_table_unittest.cc',
        '../webkit/database/database_tracker_unittest.cc',
        '../webkit/database/database_util_unittest.cc',
        '../webkit/database/quota_table_unittest.cc',
        '../webkit/dom_storage/dom_storage_area_unittest.cc',
        '../webkit/dom_storage/dom_storage_cached_area_unittest.cc',
        '../webkit/dom_storage/dom_storage_context_unittest.cc',
        '../webkit/dom_storage/dom_storage_database_unittest.cc',
        '../webkit/dom_storage/dom_storage_map_unittest.cc',
        '../webkit/dom_storage/session_storage_database_unittest.cc',
        '../webkit/fileapi/file_system_database_test_helper.cc',
        '../webkit/fileapi/file_system_database_test_helper.h',
        '../webkit/fileapi/file_system_directory_database_unittest.cc',
        '../webkit/fileapi/file_system_dir_url_request_job_unittest.cc',
        '../webkit/fileapi/file_system_file_util_unittest.cc',
        '../webkit/fileapi/file_system_mount_point_provider_unittest.cc',
        '../webkit/fileapi/file_system_origin_database_unittest.cc',
        '../webkit/fileapi/file_system_quota_client_unittest.cc',
        '../webkit/fileapi/file_system_url_unittest.cc',
        '../webkit/fileapi/file_system_url_request_job_unittest.cc',
        '../webkit/fileapi/file_system_usage_cache_unittest.cc',
        '../webkit/fileapi/file_system_util_unittest.cc',
        '../webkit/fileapi/file_writer_delegate_unittest.cc',
        '../webkit/fileapi/isolated_context_unittest.cc',
        '../webkit/fileapi/isolated_file_util_unittest.cc',
        '../webkit/fileapi/local_file_system_operation_write_unittest.cc',
        '../webkit/fileapi/local_file_system_operation_unittest.cc',
        '../webkit/fileapi/local_file_system_quota_unittest.cc',
        '../webkit/fileapi/local_file_system_test_helper.cc',
        '../webkit/fileapi/local_file_system_test_helper.h',
        '../webkit/fileapi/local_file_stream_writer_unittest.cc',
        '../webkit/fileapi/local_file_util_unittest.cc',
        '../webkit/fileapi/media/native_media_file_util_unittest.cc',
        '../webkit/fileapi/mock_file_system_options.cc',
        '../webkit/fileapi/mock_file_system_options.h',
        '../webkit/fileapi/obfuscated_file_util_unittest.cc',
        '../webkit/fileapi/sandbox_mount_point_provider_unittest.cc',
        '../webkit/fileapi/test_file_set.cc',
        '../webkit/fileapi/test_file_set.h',
        '../webkit/fileapi/webfilewriter_base_unittest.cc',
        '../webkit/media/buffered_data_source_unittest.cc',
        '../webkit/media/buffered_resource_loader_unittest.cc',
        '../webkit/media/cache_util_unittest.cc',
        '../webkit/media/crypto/proxy_decryptor_unittest.cc',
        '../webkit/media/skcanvas_video_renderer_unittest.cc',
        '../webkit/media/test_response_generator.cc',
        '../webkit/media/test_response_generator.h',
        '../webkit/mocks/mock_weburlloader.cc',
        '../webkit/mocks/mock_weburlloader.h',
        '../webkit/plugins/npapi/plugin_lib_unittest.cc',
        '../webkit/plugins/npapi/plugin_list_unittest.cc',
        '../webkit/plugins/npapi/plugin_utils_unittest.cc',        
        '../webkit/plugins/npapi/webplugin_impl_unittest.cc',
        '../webkit/quota/mock_quota_manager.cc',
        '../webkit/quota/mock_quota_manager.h',
        '../webkit/quota/mock_quota_manager_unittest.cc',
        '../webkit/quota/mock_storage_client.cc',
        '../webkit/quota/mock_storage_client.h',
        '../webkit/quota/quota_database_unittest.cc',
        '../webkit/quota/quota_manager_unittest.cc',
        '../webkit/quota/quota_temporary_storage_evictor_unittest.cc',
      ],
      'conditions': [
        ['enable_webrtc==1', {
          'sources': [
            'browser/renderer_host/p2p/socket_host_test_utils.h',
            'browser/renderer_host/p2p/socket_host_tcp_unittest.cc',
            'browser/renderer_host/p2p/socket_host_tcp_server_unittest.cc',
            'browser/renderer_host/p2p/socket_host_udp_unittest.cc',
            'renderer/media/media_stream_dispatcher_unittest.cc',
            'renderer/media/media_stream_impl_unittest.cc',
            'renderer/media/peer_connection_handler_jsep_unittest.cc',
            'renderer/media/rtc_video_decoder_unittest.cc',
            'renderer/media/webrtc_audio_device_unittest.cc',
          ],
          'dependencies': [
            '../third_party/libjingle/libjingle.gyp:libjingle_peerconnection',
            '../third_party/webrtc/modules/modules.gyp:video_capture_module',
            '../third_party/webrtc/system_wrappers/source/system_wrappers.gyp:system_wrappers',
            '../third_party/webrtc/video_engine/video_engine.gyp:video_engine_core',
            '../third_party/webrtc/voice_engine/voice_engine.gyp:voice_engine_core',
          ]
        }],
        # TODO(jrg): remove the OS=="android" section?
        # http://crbug.com/113172
        # Understand better how media_stream_ is tied into Chromium.
        ['enable_webrtc==0 and OS=="android"', {
          'sources/': [
            ['exclude', '^renderer/media/media_stream_'],
          ],
        }],
        ['input_speech==0', {
          'sources/': [
            ['exclude', '^browser/speech/'],
          ]
        }],
        ['notifications==0', {
           'sources!': [
             'renderer/active_notification_tracker_unittest.cc',
           ],
        }],
        ['use_x11 == 1', {
          'dependencies': [
            '../build/linux/system.gyp:dbus',
            '../dbus/dbus.gyp:dbus_test_support',
          ],
        }],
        ['OS=="win" and win_use_allocator_shim==1', {
          'dependencies': [
            '../base/allocator/allocator.gyp:allocator',
          ],
        }],
        ['OS=="win"', {
          'dependencies': [
            '../third_party/iaccessible2/iaccessible2.gyp:iaccessible2',
          ],
        }],
        ['OS=="mac"', {
          # These flags are needed to run the test on Mac.
          # Search for comments about "xcode_settings" in chrome_tests.gypi.
          'xcode_settings': {'OTHER_LDFLAGS': ['-Wl,-ObjC']},
        }],
        ['chromeos==1', {
          'sources/': [
            ['exclude', '^browser/renderer_host/gtk_key_bindings_handler_unittest.cc'],
          ],
        }],
        ['use_aura==1', {
          'dependencies': [
            '../ui/aura/aura.gyp:aura',
          ],
          'sources!': [
            'browser/accessibility/browser_accessibility_win_unittest.cc',
          ],
        }],
        ['OS == "android"', {
          'sources!': [
            'browser/geolocation/device_data_provider_unittest.cc',
            'browser/geolocation/gps_location_provider_unittest_linux.cc',
            'browser/geolocation/network_location_provider_unittest.cc',
            'browser/geolocation/wifi_data_provider_common_unittest.cc',
            'browser/geolocation/wifi_data_provider_linux_unittest.cc',
          ],
        }, { # OS != "android"
          'dependencies': [
            '../third_party/libvpx/libvpx.gyp:libvpx',
          ],
        }],
        ['OS == "android" and gtest_target_type == "shared_library"', {
          'dependencies': [
            '../testing/android/native_test.gyp:native_test_native_code',
          ],
        }],
      ],
    },
    {
      'target_name': 'content_browsertests',
      'type': 'executable',
      'defines!': ['CONTENT_IMPLEMENTATION'],
      'dependencies': [
        'content_gpu',
        'content_plugin',
        'content_renderer',
        'content_shell_lib',
        'content_shell_pak',
        'test_support_content',
        '../base/base.gyp:test_support_base',
        '../ipc/ipc.gyp:test_support_ipc',
        '../net/net.gyp:net_test_support',
        '../ppapi/ppapi_internal.gyp:ppapi_host',
        '../ppapi/ppapi_internal.gyp:ppapi_proxy',
        '../ppapi/ppapi_internal.gyp:ppapi_ipc',
        '../ppapi/ppapi_internal.gyp:ppapi_shared',
        '../ppapi/ppapi_internal.gyp:ppapi_unittest_shared',
        '../skia/skia.gyp:skia',
        '../testing/gmock.gyp:gmock',
        '../testing/gtest.gyp:gtest',
        '../ui/ui.gyp:ui',
        '../webkit/support/webkit_support.gyp:glue',
      ],
      'include_dirs': [
        '..',
      ],
      'defines': [
        'HAS_OUT_OF_PROC_TEST_RUNNER',
      ],
      'sources': [
        'browser/accessibility/cross_platform_accessibility_browsertest.cc',
        'browser/accessibility/dump_accessibility_tree_browsertest.cc',
        'browser/accessibility/dump_accessibility_tree_helper.cc',
        'browser/accessibility/dump_accessibility_tree_helper.h',
        'browser/accessibility/dump_accessibility_tree_helper_mac.mm',
        'browser/accessibility/dump_accessibility_tree_helper_win.cc',
        'browser/appcache/appcache_browsertest.cc',
        'browser/audio_browsertest.cc',
        'browser/child_process_security_policy_browsertest.cc',
        'browser/database_browsertest.cc',
        'browser/device_orientation/device_orientation_browsertest.cc',
        'browser/dom_storage/dom_storage_browsertest.cc',
	'browser/download/download_browsertest.cc',
        'browser/download/mhtml_generation_browsertest.cc',
        'browser/download/save_package_browsertest.cc',
        'browser/fileapi/file_system_browsertest.cc',
        'browser/in_process_webkit/indexed_db_browsertest.cc',
        'browser/in_process_webkit/indexed_db_layout_browsertest.cc',
        'browser/media_browsertest.cc',
        'browser/plugin_data_remover_impl_browsertest.cc',
        'browser/plugin_browsertest.cc',
        'browser/plugin_service_impl_browsertest.cc',
        'browser/renderer_host/render_view_host_browsertest.cc',
        'browser/renderer_host/render_view_host_manager_browsertest.cc',
        'browser/renderer_host/resource_dispatcher_host_browsertest.cc',
        'browser/session_history_browsertest.cc',
        'browser/speech/speech_recognition_browsertest.cc',
        'browser/webkit_browsertest.cc',
        'browser/worker_host/test/worker_browsertest.cc',
        'common/content_constants_internal.cc',
        'common/content_constants_internal.h',
        'renderer/browser_plugin/mock_browser_plugin.h',
        'renderer/browser_plugin/mock_browser_plugin.cc',
        'renderer/browser_plugin/mock_browser_plugin_manager.h',
        'renderer/browser_plugin/mock_browser_plugin_manager.cc',
        'renderer/browser_plugin/browser_plugin_browsertest.h',
        'renderer/browser_plugin/browser_plugin_browsertest.cc',
        'renderer/mouse_lock_dispatcher_browsertest.cc',
        'renderer/pepper/mock_renderer_ppapi_host.cc',
        'renderer/pepper/pepper_file_chooser_host_unittest.cc',
        'renderer/render_view_browsertest.cc',
        'renderer/render_view_browsertest_mac.mm',
        'renderer/renderer_accessibility_browsertest.cc',
        'test/content_browser_test.h',
        'test/content_browser_test.cc',
        'test/content_browser_test_utils.cc',
        'test/content_browser_test_utils.h',
        'test/content_browser_test_utils_mac.mm',
        'test/content_browser_test_test.cc',
        'test/content_test_launcher.cc',
        'test/layout_browsertest.cc',
        'test/layout_browsertest.h',
        'test/layout_test_http_server.cc',
        'test/layout_test_http_server.h',
      ],
      'conditions': [
        ['OS=="win"', {
          'resource_include_dirs': [
            '<(SHARED_INTERMEDIATE_DIR)/webkit',
          ],
          'sources': [
            'shell/resource.h',
            'shell/shell.rc',
            # TODO:  It would be nice to have these pulled in
            # automatically from direct_dependent_settings in
            # their various targets (net.gyp:net_resources, etc.),
            # but that causes errors in other targets when
            # resulting .res files get referenced multiple times.
            '<(SHARED_INTERMEDIATE_DIR)/net/net_resources.rc',
            '<(SHARED_INTERMEDIATE_DIR)/webkit/webkit_chromium_resources.rc',
            '<(SHARED_INTERMEDIATE_DIR)/webkit/webkit_resources.rc',
            '<(SHARED_INTERMEDIATE_DIR)/webkit/webkit_strings_en-US.rc',
          ],
          'dependencies': [
            '<(DEPTH)/net/net.gyp:net_resources',
            '<(DEPTH)/third_party/iaccessible2/iaccessible2.gyp:iaccessible2',
            '<(DEPTH)/webkit/support/webkit_support.gyp:webkit_resources',
            '<(DEPTH)/webkit/support/webkit_support.gyp:webkit_strings',
          ],
          'configurations': {
            'Debug_Base': {
              'msvs_settings': {
                'VCLinkerTool': {
                  'LinkIncremental': '<(msvs_large_module_debug_link_mode)',
                },
              },
            },
          },
        }],
        ['OS=="win" and win_use_allocator_shim==1', {
          'dependencies': [
            '../base/allocator/allocator.gyp:allocator',
          ],
        }],
        ['OS=="linux"', {
          'sources!': [
            'browser/accessibility/dump_accessibility_tree_browsertest.cc',
            'browser/accessibility/dump_accessibility_tree_helper.cc',
          ],
        }],
        ['OS=="mac"', {
          'dependencies': [
            'content_shell',  # Needed for Content Shell.app's Helper.
          ],
        }],
        ['use_aura==1', {
          'sources!': [
            'browser/accessibility/dump_accessibility_tree_browsertest.cc',
            'browser/accessibility/dump_accessibility_tree_helper_win.cc',
            'browser/accessibility/dump_accessibility_tree_helper.cc',
            'browser/plugin_browsertest.cc',
          ],
        }],
        ['target_arch!="arm"', {
          'dependencies': [
            # Runtime dependencies
            '../webkit/webkit.gyp:copy_npapi_test_plugin',
            '../webkit/webkit.gyp:pull_in_copy_TestNetscapePlugIn',
          ],
        }],
      ],
    },
  ],
  'conditions': [
    ['chromeos==1 or OS=="linux" or OS=="win" or OS=="mac"', {
      'targets': [
          {
            'target_name': 'video_decode_accelerator_unittest',
            'type': 'executable',
            'dependencies': [
              'content',
              '../base/base.gyp:base',
              '../testing/gtest.gyp:gtest',
              '../media/media.gyp:media',
              '../ui/ui.gyp:ui',
            ],
            'include_dirs': [
              '<(DEPTH)/third_party/angle/include',
            ],
            'sources': [
              'common/gpu/media/rendering_helper.h',
              'common/gpu/media/rendering_helper_mac.mm',
              'common/gpu/media/rendering_helper_gl.cc',
              'common/gpu/media/video_decode_accelerator_unittest.cc',
            ],
            'conditions': [
              ['target_arch=="arm"', {
                'include_dirs': [
                  '<(DEPTH)/third_party/openmax/il',
                ],
              }],
              ['OS=="mac"', {
                'sources!': [
                  'common/gpu/media/rendering_helper_gl.cc',
                ],
              }],
              ['OS=="win"', {
                'dependencies': [
                  '../third_party/angle/src/build_angle.gyp:libEGL',
                  '../third_party/angle/src/build_angle.gyp:libGLESv2',
                ],
              }],
              ['(OS=="win" and win_use_allocator_shim==1) or '
               '(os_posix == 1 and OS != "mac" and OS != "android" and '
               ' linux_use_tcmalloc==1)', {
                'dependencies': [
                  '../base/allocator/allocator.gyp:allocator',
                ],
              }],
              ['target_arch != "arm"', {
                 'dependencies': [
                   '../ui/gl/gl.gyp:gl',
                 ],
              }],
              ['target_arch != "arm" and (OS=="linux" or chromeos == 1)', {
                'include_dirs': [
                  '<(DEPTH)/third_party/libva',
                ],
              }],
            ],
          },
        ]
    }],
    ['chromeos == 1 or OS == "linux"', {
      'targets': [
        {
          'target_name': 'h264_parser_unittest',
          'type': 'executable',
          'dependencies': [
            'content_common',
            '../base/base.gyp:base',
            '../testing/gtest.gyp:gtest',
          ],
          'sources': [
            'common/gpu/media/h264_bit_reader_unittest.cc',
            'common/gpu/media/h264_parser_unittest.cc',
          ],
        }
      ],
    }],
    # Special target to wrap a gtest_target_type==shared_library
    # content_unittests into an android apk for execution.
    # See base.gyp for TODO(jrg)s about this strategy.
    ['OS == "android" and gtest_target_type == "shared_library"', {
      'targets': [
        {
          'target_name': 'content_unittests_apk',
          'type': 'none',
          'dependencies': [
            '../base/base.gyp:base_java',
            '../net/net.gyp:net_java',
            'content_java',
            'content_unittests',
          ],
          'variables': {
            'test_suite_name': 'content_unittests',
            'input_shlib_path': '<(SHARED_LIB_DIR)/<(SHARED_LIB_PREFIX)content_unittests<(SHARED_LIB_SUFFIX)',
          },
          'includes': [ '../build/apk_test.gypi' ],
        },
      ],
    }],
    ['OS == "android"', {
      'targets': [
        {
          'target_name': 'content_javatests',
          'type': 'none',
          'dependencies': [
            '../base/base.gyp:base_java',
            '../base/base.gyp:base_java_test_support',
            'content_common',
            'content_java',
          ],
          'variables': {
            'package_name': 'content_javatests',
            'java_in_dir': '../content/public/android/javatests',
          },
          'includes': [ '../build/java.gypi' ],
        },
        {
          'target_name': 'content_shell_test_apk',
          'type': 'none',
          'dependencies': [
            'content_shell_apk',
            'content_javatests',
            '../net/net.gyp:net_javatests',
            '../tools/android/forwarder/forwarder.gyp:forwarder',
          ],
          'copies': [
            {
              'destination': '<(PRODUCT_DIR)/content_shell_test/java/libs',
              'files': [
                '<(PRODUCT_DIR)/lib.java/chromium_base_javatests.jar',
                '<(PRODUCT_DIR)/lib.java/chromium_net_javatests.jar',
                '<(PRODUCT_DIR)/lib.java/chromium_content_javatests.jar',
              ],
            },
          ],
          'actions': [
            {
              'action_name': 'content_shell_test_generate_apk',
              'inputs': [
                '<(PRODUCT_DIR)/lib.java/chromium_base_javatests.jar',
                '<(PRODUCT_DIR)/lib.java/chromium_net_javatests.jar',
                '<(PRODUCT_DIR)/lib.java/chromium_content_javatests.jar',
                '<(DEPTH)/content/shell/android/javatests/content_shell_test_apk.xml',
                '<(DEPTH)/content/shell/android/javatests/AndroidManifest.xml',
                '<!@(find <(DEPTH)/content/shell/android/javatests/ -name "*.java")'
              ],
              'outputs': [
                '<(PRODUCT_DIR)/content_shell_test/ContentShellTest-debug.apk',
              ],
              'action': [
                'ant',
                '-DPRODUCT_DIR=<(ant_build_out)',
                '-DAPP_ABI=<(android_app_abi)',
                '-DANDROID_SDK=<(android_sdk)',
                '-DANDROID_SDK_ROOT=<(android_sdk_root)',
                '-DANDROID_SDK_TOOLS=<(android_sdk_tools)',
                '-DANDROID_SDK_VERSION=<(android_sdk_version)',
                '-DANDROID_TOOLCHAIN=<(android_toolchain)',
                '-DANDROID_GDBSERVER=<(android_gdbserver)',
                '-buildfile',
                '<(DEPTH)/content/shell/android/javatests/content_shell_test_apk.xml',
              ]
            }
          ],
        },
      ],
    }],
  ],
}
