#ifndef CONF_H_
#define CONF_H_
#include <stdint.h>
typedef struct {
  int image_width;
  int image_height;
  uint8_t *pixels;
} conf_t;
#define IMG_WIDTH(c) (c).image_width
#define IMG_HEIGHT(c) (c).image_height
#define PIXELS(c) (c).pixels
typedef void (*api_t)(conf_t c);
void main02_2(conf_t c);
void main02_3(conf_t c);
void main03_3(conf_t c);
#ifdef CONF_IMPL
api_t apis[] = {
    main02_2,
    main02_3,
    main03_3,
};
const int napis = sizeof(apis) / sizeof(apis[0]);
#endif /*CONF_IMPL*/
#endif /*CONF_H_*/
