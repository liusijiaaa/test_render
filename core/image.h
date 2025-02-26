#ifndef IMAGE_H
#define IMAGE_H

typedef struct {
    int width;
    int height;
    int channels;
    int pitch;  // 每行的步长
    unsigned char *buffer;
} image_t;

image_t *image_load(const char *file, const char *type);
void image_save(image_t *image, const char *file, const char *type);
void image_free(image_t *image);

void image_resize(image_t *image, int width, int height);

void image_flip_vertical(image_t *image);
void image_flip_horizontal(image_t *image);

#endif