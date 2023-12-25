#include <gst/gst.h>

// GStreamer全体に関わるようなデータを入れる構造体
typedef struct _PlayerData {
  GstElement *pipeline, *filesrc, *decodebin, *videoconvert, *autovideosink,
      *audioconvert, *audioresample, *autoaudiosink;
  GMainLoop *main_loop;
} PlayerData;

// コールバック関数
static gboolean pipeline_callback(GstBus *bus, GstMessage *message,
                                  gpointer rawdata) {
  PlayerData *data = (PlayerData *)rawdata;
  switch (GST_MESSAGE_TYPE(message)) {
  case GST_MESSAGE_EOS:
    // ファイルが終わりに達したらメインループを終了する
    g_main_loop_quit(data->main_loop);
    break;
  case GST_MESSAGE_ERROR:
    // エラーが発生したらエラーメッセージを出してからメインループを終了する
    GError *error;
    gchar *debug;
    gst_message_parse_error(message, &error, &debug);
    g_printerr("Error: %s", error->message);
    g_error_free(error);
    g_free(debug);
    g_main_loop_quit(data->main_loop);
    break;
  default:
    break;
  }
  return TRUE;
}

// pad-added に反応して呼ばれる関数
static void on_pad_added(GstElement *element, GstPad *pad, PlayerData *data) {
  gchar *pad_name = GST_PAD_NAME(pad);
  if (g_strcmp0(pad_name, "audio_0") == 0) {
    GstPad *sink = gst_element_get_static_pad(data->audioconvert, "sink");
    gst_pad_link(pad, sink);
  }
  if (g_strcmp0(pad_name, "video_0") == 0) {
    GstPad *sink = gst_element_get_static_pad(data->videoconvert, "sink");
    gst_pad_link(pad, sink);
  }
}

int main(int argc, char **argv) {
  // GStreamer の初期化
  gst_init(&argc, &argv);

  // 開くファイルをコマンドラインオプションから取得
  if (argc != 2) {
    g_printerr("Usage: ./playbin-proto3 <file>\n");
    return -1;
  }
  gchar *location = argv[1];

  // 各種データを入れる構造体を作成
  PlayerData data;

  // パイプラインの作成
  data.pipeline = gst_pipeline_new("first-pipeline");
  if (!data.pipeline) {
    g_printerr("Failed to create pipeline.\n");
    return -1;
  }

  // 要素を作ってパイプラインに追加する
  data.filesrc = gst_element_factory_make("filesrc", "file-src");
  data.decodebin = gst_element_factory_make("decodebin3", "decode-bin");
  data.videoconvert = gst_element_factory_make("videoconvert", "video-convert");
  data.autovideosink =
      gst_element_factory_make("autovideosink", "auto-video-sink");
  data.audioconvert = gst_element_factory_make("audioconvert", "audio-convert");
  data.audioresample =
      gst_element_factory_make("audioresample", "audio-resample");
  data.autoaudiosink =
      gst_element_factory_make("autoaudiosink", "auto-audio-sink");
  if (!data.filesrc || !data.decodebin || !data.videoconvert ||
      !data.autovideosink || !data.audioconvert || !data.audioresample ||
      !data.autoaudiosink) {
    g_printerr("Failed to create elements.\n");
    return -1;
  }
  gst_bin_add_many(GST_BIN(data.pipeline), data.filesrc, data.decodebin,
                   data.videoconvert, data.autovideosink, data.audioconvert,
                   data.audioresample, data.autoaudiosink, NULL);
  // 要素の間を繫ぐ
  gst_element_link(data.filesrc, data.decodebin);
  gst_element_link_many(data.videoconvert, data.autovideosink, NULL);
  gst_element_link_many(data.audioconvert, data.audioresample,
                        data.autoaudiosink, NULL);
  g_signal_connect(data.decodebin, "pad-added", G_CALLBACK(on_pad_added),
                   &data);
  // filesrc で開くファイルを指定
  g_object_set(G_OBJECT(data.filesrc), "location", location, NULL);

  // バスを取得
  GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE(data.pipeline));
  // バスにコールバックを設定
  guint bus_watch_id = gst_bus_add_watch(bus, pipeline_callback, &data);
  // バスを解放
  gst_object_unref(GST_OBJECT(bus));

  // 再生を開始
  gst_element_set_state(data.pipeline, GST_STATE_PLAYING);

  // メインループの作成と開始
  data.main_loop = g_main_loop_new(NULL, FALSE);
  g_main_loop_run(data.main_loop);

  // メインループを抜けたらパイプラインの状態をNULLにする
  gst_element_set_state(data.pipeline, GST_STATE_NULL);

  // リソースの解放
  gst_object_unref(GST_OBJECT(data.pipeline));
  g_source_remove(bus_watch_id);
  g_main_loop_unref(data.main_loop);
  return 0;
}
