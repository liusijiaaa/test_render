#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "image.h"

// 禁用结构体对齐，确保二进制紧凑
#pragma pack(push, 1)
typedef struct {
    uint8_t  idlength;          // ID字段长度（0表示无ID）
    uint8_t  colormaptype;      // 颜色表类型（0=无，1=有）
    uint8_t  imagetype;         // 图像类型（1=索引色未压缩，2=真彩色未压缩，9=索引色RLE，10=真彩色RLE）
    uint16_t colormaporigin;    // 颜色表起始索引（小端序）
    uint16_t colormaplength;    // 颜色表项数（小端序）
    uint8_t  colormapdepth;     // 每个颜色表项的位数（如24）
    uint16_t xorigin;           // 图像X原点（小端序）
    uint16_t yorigin;           // 图像Y原点（小端序）
    uint16_t width;             // 图像宽度（小端序）
    uint16_t height;            // 图像高度（小端序）
    uint8_t  pixeldepth;        // 每个像素的位数（如24, 32）
    uint8_t  descriptor;        // 描述符（方向、Alpha位数）
} TGAHeader;
#pragma pack(pop)
static int read_byte(FILE *file){
    int byte = fgetc(file);
    if ( byte == EOF)
    {
        printf("Error reading file");
    }
    return byte;
}

static void load_tga_rle(FILE *file, unsigned char *buffer, int buffer_size, int channels)
{
    unsigned char *pixel = (unsigned char*)malloc(channels);
    int buffer_count = 0;
    int i, j;
    do
    {
        int chunk_count = read_byte(file);
        if (chunk_count < 128)  //非RLE直接读进去,TGA官方标准
        {
            chunk_count = chunk_count + 1;
            if (buffer_count + chunk_count * channels > buffer_size)
            {
                printf("tga:raw packet too large");
            }
            for (int i = 0; i < chunk_count; i++)
            {
                for (int j = 0; i < channels; j++)
                {
                    buffer[buffer_count] = read_byte(file);
                    buffer_count+=1;
                }
            }
        }else{
            chunk_count = chunk_count - 128 + 1;
            for (j = 0; j < channels; j++) {
                pixel[j] = read_byte(file);
            }
            for (int i = 0; i < chunk_count; i++)
            {
                for (int j = 0; j < channels; j++)
                {
                    buffer[buffer_count] = pixel[j];
                    buffer_count+=1;
                }
            }
        }
        
    } while (buffer_count < buffer_size);
    
    free(pixel);
}

static image_t *tga_load(const char* filename) {
    FILE *file;
    TGAHeader tga_header;
    int width, height, channels, pixeldepth;
    int image_type;
    int buffer_size;
    unsigned char *buffer;
    image_t *image;
    int count;

    file = fopen(filename, "rb");   //> 二进制打开文件
    if (file == NULL)
    {
        printf("Error opening file");
    }

    count = fread(&tga_header, sizeof(TGAHeader), 1, file); // > 读取文件头到结构体tga_header
    if (count != sizeof(TGAHeader)) //校验读取
    {
        printf("Error reading header");
    }

    width = tga_header.width;
    height = tga_header.height;
    if (width <=0 || height <= 0)
    {
        printf("Error width/height vaule");
    }
    pixeldepth = tga_header.pixeldepth; 
    channels = pixeldepth / 8;
    if (pixeldepth != 8 && pixeldepth != 16 && pixeldepth != 24 && pixeldepth != 32)
    {
        printf("Error pixeldepth vaule");
    }

    buffer_size = width * height * channels;
    buffer = (unsigned char*)malloc(buffer_size);
    image_type = tga_header.imagetype;
    if (image_type == 2 || image_type == 3) //未压缩直接读到buffer中
    {
        count = fread(buffer, buffer_size, 1, file);
        if (count != buffer_size)
        {
            printf("Error reading image data");
        }
    }else if (image_type == 10 || image_type == 11)
    {
        load_tga_rle(file, buffer, buffer_size, channels);
    }else {
        printf("Error image type");
    }
    fclose(file);
    image = (image_t*)malloc(sizeof(image_t));
    image->buffer = buffer;
    image->channels = channels;
    image->height = height;
    image->pitch = width * channels;
    image->width = width;
    
    if (!(tga_header.descriptor & 0x20))    // 垂直翻转
    {
        image_flip_vertical(image);
    }

    if (!(tga_header.descriptor & 0x10))    //水平翻转
    {
        image_flip_horizontal(image);
    }
    return image;
}

void image_free(image_t *image) {
    free(image->buffer);
    free(image);
}

image_t *image_load(const char *file, const char *type) {
    return tga_load(file);
}

void image_flip_vertical(image_t *image) {
    int half_height = image->height / 2;
    int i, j, k;
    for (i = 0; i < half_height; i++)
    {
        for (j = 0; j < image->width; j++)
        {
            int index_start = i * image->pitch + j * image->channels;
            int index_end = (image->height - i - 1) * image->pitch + j * image->channels;
            for (k = 0; k < image->channels; k++)
            {
                image->buffer[index_start + k] ^= image->buffer[index_end + k];
                image->buffer[index_end + k] ^= image->buffer[index_start + k];
                image->buffer[index_start + k] ^= image->buffer[index_end + k];
            }
        }
    }

}

void image_flip_horizontal(image_t *image) {
    int half_width = image->width / 2;
    int i, j, k;
    for (i = 0; i < image->height; i++)
    {
        for (j = 0; j < half_width; j++)
        {
            int index_start = i * image->pitch + j * image->channels;
            int index_end = i * image->pitch + (image->width - j - 1) * image->channels;
            for (k = 0; k < image->channels; k++)
            {
                image->buffer[index_start + k] ^= image->buffer[index_end + k];
                image->buffer[index_end + k] ^= image->buffer[index_start + k];
                image->buffer[index_start + k] ^= image->buffer[index_end + k];
            }   
        }   
    }
}

