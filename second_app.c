#include <gst/gst.h>

int main(int argc, char *argv[]) {
  GstElement *pipeline;
  GMainLoop *loop;

  // Inicializa GStreamer
  gst_init(&argc, &argv);

  // Crea el bucle principal
  loop = g_main_loop_new(NULL, FALSE);

  // Define el pipeline de transmisión
  pipeline = gst_parse_launch(
    "autoaudiosrc ! audioconvert ! audioresample ! opusenc ! rtpopuspay ! udpsink host=127.0.0.1 port=5000",
    NULL);

  if (!pipeline) {
    g_printerr("Error al crear el pipeline\n");
    return -1;
  }

  // Cambia el estado del pipeline a "playing"
  gst_element_set_state(pipeline, GST_STATE_PLAYING);
  g_print("🎧 Transmitiendo audio a 127.0.0.1:5000...\n");

  // Ejecuta el bucle principal
  g_main_loop_run(loop);

  // Limpieza al salir
  g_print("🛑 Deteniendo transmisión\n");
  gst_element_set_state(pipeline, GST_STATE_NULL);
  gst_object_unref(pipeline);
  g_main_loop_unref(loop);

  return 0;
}
