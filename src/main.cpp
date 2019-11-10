#include <vlc/vlc.h>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <thread>

using namespace std;
using namespace cv;

struct context {
  uint8_t *data;
  mutex mtx;
};

static void *lock(void *data, void **p_pixels) {
  struct context *ctx = static_cast<struct context *>(data);

  ctx->mtx.lock();
  *p_pixels = ctx->data;

  return *p_pixels;
}

static void unlock(void *data, void *id, void *const *p_pixels) {
  struct context *ctx = static_cast<struct context *>(data);

  ctx->mtx.unlock();
}

void display(void *opaque, void *picture) {
  Mat frame = Mat(160, 240, CV_8UC3, picture);
  //  resize(frame, frame, frame.size() / 4);
  cvtColor(frame, frame, COLOR_RGB2BGR);
  imshow("Show", frame);
  waitKey(1);
}

static const char *const VLC_ARGV[] = {"--no-xlib", "--no-audio"};

int main(int argc, char *argv[]) {
  libvlc_instance_t *inst =
      libvlc_new(sizeof(VLC_ARGV) / sizeof(*VLC_ARGV), VLC_ARGV);
  // libvlc_media_new_path
  libvlc_media_t *m = libvlc_media_new_location(
      inst, "rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov");

  libvlc_media_player_t *mp = libvlc_media_player_new_from_media(m);
  libvlc_media_release(m);

  //  libvlc_video_set_scale(mp, 0.2f);

  struct context ctx;
  ctx.data = new unsigned char[240 * 160 * 3];

  libvlc_video_set_callbacks(mp, lock, unlock, display, &ctx);
  libvlc_video_set_format(mp, "RV24", 240, 160, 240 * 3);

  //  libvlc_media_player_set_rate(mp, 1.f);
  libvlc_audio_set_mute(mp, true);
  libvlc_media_player_play(mp);

  while (libvlc_media_player_get_state(mp) != libvlc_Ended)
    this_thread::sleep_for(chrono::milliseconds(1));

  libvlc_media_player_stop(mp);
  libvlc_media_player_release(mp);
  libvlc_release(inst);

  delete[] ctx.data;

  return 0;
}
