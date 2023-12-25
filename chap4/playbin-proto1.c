#include <gst/gst.h>

int main(int argc, char **argv) {
  // GStreamer の初期化
  gst_init(&argc, &argv);

  // 開くファイルをコマンドラインオプションから取得
  if (argc != 2) {
    g_printerr("Usage: ./playbin-proto1 <URI>\n");
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
  GstElement *playbin = gst_element_factory_make("playbin3", "play-bin");
  if (!playbin) {
    g_printerr("Failed to create playbin.\n");
    return -1;
  }
  gst_bin_add(GST_BIN(pipeline), playbin);

  // playbin で開くファイルを指定
  g_object_set(G_OBJECT(playbin), "uri", uri, NULL);

  // リソースの解放
  gst_object_unref(GST_OBJECT(pipeline));
  return 0;
}
