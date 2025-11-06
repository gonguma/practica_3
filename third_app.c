#include <gst/gst.h>

int main(int argc, char *argv[]) {
  GstElement *pipeline;
  GMainLoop *loop;

  // Inicializa GStreamer
  gst_init(&argc, &argv);

  // Crea el bucle principal
  loop = g_main_loop_new(NULL, FALSE);

  // Define el pipeline de transmisión de video
  pipeline = gst_parse_launch(
    "filesrc location=/opt/nvidia/deepstream/deepstream/samples/streams/sample_1080p_h264.mp4 ! qtdemux ! h264parse ! "
    "nvv4l2decoder ! queue ! mux.sink_0 "
    "nvstreammux name=mux width=1920 height=1080 batch-size=1 ! queue ! nvvideoconvert ! "
    "nvinfer config-file-path=/opt/nvidia/deepstream/deepstream-6.0/samples/configs/deepstream-app/config_infer_primary.txt "
    "model-engine-file=/home/grq/resnet10.caffemodel_b1_gpu0_fp16.engine ! queue ! "
    "nvdsosd process-mode=HW_MODE ! queue ! tee name=t "
    "t. ! queue ! nvoverlaysink sync=false "
    "t. ! queue ! nvvideoconvert ! nvv4l2h264enc ! h264parse ! rtph264pay config-interval=1 pt=96 ! "
    "udpsink host=192.168.0.255 port=5000", NULL);

  if (!pipeline) {
    g_printerr("Error al crear el pipeline\n");
    return -1;
  }

  // Cambia el estado del pipeline a "playing"
  gst_element_set_state(pipeline, GST_STATE_PLAYING);
  g_print("Transmitiendo solo video...\n");

  // Ejecuta el bucle principal
  g_main_loop_run(loop);

  // Limpieza al salir
  g_print("Deteniendo transmisión\n");
  gst_element_set_state(pipeline, GST_STATE_NULL);
  gst_object_unref(pipeline);
  g_main_loop_unref(loop);

  return 0;
}
