#include "conf.h"
// #include <stdio.h>
void main02_3(conf_t c) {
  // Image
  const int image_width = IMG_WIDTH(c);
  const int image_height = IMG_HEIGHT(c);
  // Render
  uint8_t *p = PIXELS(c);
  for (int j = image_height - 1; j >= 0; --j) {
    // fprintf(stderr, "Scanlines remaining: %d\n", j);
    for (int i = 0; i < image_width; ++i) {
      double r = (double)i / (image_width - 1);
      double g = (double)j / (image_height - 1);
      double b = 0.25;

      int ir = 255.999 * r;
      int ig = 255.999 * g;
      int ib = 255.999 * b;
      *p++ = ir;
      *p++ = ig;
      *p++ = ib;

      //   printf("%d %d %d\n", ir, ig, ib);
    }
  }
}
