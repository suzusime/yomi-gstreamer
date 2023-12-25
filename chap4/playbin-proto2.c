#include <gst/gst.h>

// メインループをグローバル変数として持つ
GMainLoop *main_loop;

// コールバック関数
static gboolean pipeline_callback(GstBus *bus, GstMessage *message,
                                  gpointer data) {
  switch (GST_MESSAGE_TYPE(message)) {
  case GST_MESSAGE_EOS:
    // ファイルが終わりに達したらメインループを終了する
    g_main_loop_quit(main_loop);
    break;
  case GST_MESSAGE_ERROR:
    // エラーが発生したらエラーメッセージを出してからメインループを終了する
    GError *error;
    gchar *debug;
    gst_message_parse_error(message, &error, &debug);
    g_printerr("Error: %s\n", error->message);
    g_error_free(error);
    g_free(debug);
    g_main_loop_quit(main_loop);
    break;
  default:
    break;
  }
  return TRUE;
}

int main(int argc, char **argv) {
  // GStreamer の初期化
  gst_init(&argc, &argv);

  // 開くファイルをコマンドラインオプションから取得
  if (argc != 2) {
    g_printerr("Usage: ./playbin-proto2 <URI>\n");
    return -1;
  }
  gchar *uri = argv[1];

  // パイプラインの作成
  GstElement *pipeline = gst_pipeline_new("first-pipeline");
  if (!pipeline) {
    g_printerr("Failed to create pipeline.\n");
    return -1;
  }

  // 要素 playbin3 を作ってパイプラインに追加する
  GstElement *playbin = gst_element_factory_make("playbin3", "play-binary");
  if (!playbin) {
    g_printerr("Failed to create playbin.\n");
    return -1;
  }
  gst_bin_add(GST_BIN(pipeline), playbin);

  // playbin で開くファイルを指定
  g_object_set(G_OBJECT(playbin), "uri", uri, NULL);

  // バスを取得
  GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
  // バスにコールバックを設定
  guint bus_watch_id = gst_bus_add_watch(bus, pipeline_callback, NULL);
  // バスを解放
  gst_object_unref(GST_OBJECT(bus));

  // 再生を開始
  gst_element_set_state(pipeline, GST_STATE_PLAYING);

  // メインループの作成と開始
  main_loop = g_main_loop_new(NULL, FALSE);
  g_main_loop_run(main_loop);

  // メインループを抜けたらパイプラインの状態をNULLにする
  gst_element_set_state(pipeline, GST_STATE_NULL);

  // リソースの解放
  gst_object_unref(GST_OBJECT(pipeline));
  g_source_remove(bus_watch_id);
  g_main_loop_unref(main_loop);
  return 0;
}
