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

// 颜色表条目（BGR或BGRA格式）
typedef struct {
    uint8_t b, g, r, a;  // 根据colormapdepth确定是否包含Alpha
} RGBColor;

// 错误码
typedef enum {
    TGA_OK,
    TGA_ERR_FILE,
    TGA_ERR_HEADER,
    TGA_ERR_UNSUPPORTED,
    TGA_ERR_MEMORY
} TGAError;

// 打印错误信息
const char* tga_error_str(TGAError err) {
    switch (err) {
        case TGA_OK: return "Success";
        case TGA_ERR_FILE: return "File error";
        case TGA_ERR_HEADER: return "Invalid header";
        case TGA_ERR_UNSUPPORTED: return "Unsupported format";
        case TGA_ERR_MEMORY: return "Memory error";
        default: return "Unknown error";
    }
}

static TGAError tga_load(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) return TGA_ERR_FILE;

    TGAHeader header;
    if (fread(&header, sizeof(TGAHeader), 1, file) != 1) {
        fclose(file);
        return TGA_ERR_HEADER;
    }

      // 跳过ID字段（如果有）
    if (header.idlength > 0) {
        fseek(file, header.idlength, SEEK_CUR);
    }

    if (header.colormaptype == 1) {
        int color_size = (header.colormapdepth + 7) / 8;  // 计算每颜色字节数，+7是因为可能存在奇数位，向上取整
        int colormap_size = header.colormaplength * color_size;
        uint8_t* colormap_data = (uint8_t*)malloc(colormap_size);
        if (!colormap_data) {
            fclose(file);
            return TGA_ERR_MEMORY;
        }
        if (fread(colormap_data, 1, colormap_size, file) != colormap_size) {
            free(colormap_data);
            fclose(file);
            return TGA_ERR_HEADER;
        }

        free(colormap_data);
    }

    // 校验支持的格式（真彩色未压缩/RLE）
    if (header.imagetype != 2 && header.imagetype != 10) {
        fclose(file);
        return TGA_ERR_UNSUPPORTED;
    }

     // 计算像素数据大小
     int pixel_size = (header.pixeldepth + 7) / 8;
     int pixel_count = header.width * header.height;
     int data_size = pixel_count * pixel_size;

    // 读取像素数据
    uint8_t* compressed_data = NULL;
    if (header.imagetype == 10) {  // RLE压缩
        long data_start = ftell(file);
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        fseek(file, data_start, SEEK_SET);
        int compressed_size = file_size - data_start;
        compressed_data = (uint8_t*)malloc(compressed_size);
        if (!compressed_data) {
            fclose(file);
            return TGA_ERR_MEMORY;
        }
        if (fread(compressed_data, 1, compressed_size, file) != compressed_size) {
            free(compressed_data);
            fclose(file);
            return TGA_ERR_HEADER;
        }
    } else {  // 未压缩
        compressed_data = (uint8_t*)malloc(data_size);
        if (!compressed_data) {
            fclose(file);
            return TGA_ERR_MEMORY;
        }
        if (fread(compressed_data, 1, data_size, file) != data_size) {
            free(compressed_data);
            fclose(file);
            return TGA_ERR_HEADER;
        }
    }
    fclose(file);

    // 解压RLE数据（如果需要）
    uint8_t* pixel_data = NULL;
    if (header.imagetype == 10) {
        pixel_data = (uint8_t*)malloc(data_size);
        if (!pixel_data) {
            free(compressed_data);
            return TGA_ERR_MEMORY;
        }
        int src_pos = 0, dst_pos = 0;
        while (dst_pos < data_size) {
            uint8_t packet = compressed_data[src_pos++];
            uint8_t count = (packet & 0x7F) + 1;
            if (packet & 0x80) {  // RLE包
                if (src_pos + pixel_size > data_size) break;
                for (int i = 0; i < count; i++) {
                    memcpy(pixel_data + dst_pos, compressed_data + src_pos, pixel_size);
                    dst_pos += pixel_size;
                }
                src_pos += pixel_size;
            } else {  // 原始包
                int bytes_needed = count * pixel_size;
                if (src_pos + bytes_needed > data_size) break;
                memcpy(pixel_data + dst_pos, compressed_data + src_pos, bytes_needed);
                src_pos += bytes_needed;
                dst_pos += bytes_needed;
            }
        }
        free(compressed_data);
    } else {
        pixel_data = compressed_data;
    }

    int vertical = (header.descriptor & 0x20) ? 0 : 1; // 第5位表示垂直方向
    int horizontal = (header.descriptor & 0x10) ? 0 : 1; // 第4位表示水平方向

    if (vertical || horizontal) {
        uint8_t* flipped = (uint8_t*)malloc(data_size);
        if (!flipped) {
            if (compressed_data) free(compressed_data);
            return TGA_ERR_MEMORY;
        }

        int row_size = header.width * pixel_size;

        for (int y = 0; y < header.height; y++) {
            for (int x = 0; x < header.width; x++) {
                int src_x = horizontal ? header.width - 1 - x : x;
                int src_y = vertical ? header.height - 1 - y : y;
                memcpy(flipped + (y * header.width + x) * pixel_size,
                       pixel_data + (src_y * header.width + src_x) * pixel_size,
                       pixel_size);
            }
        }

        free(pixel_data);
        pixel_data = flipped;
    }
    // 填充输出结构
    image_t *image;
    image = (image_t*)malloc(sizeof(image_t));
    image->width = header.width;
    image->height = header.height;
    image->channels = pixel_size;
    image->buffer = pixel_data;

    return TGA_OK;
}

void image_free(image_t *image) {
    free(image->buffer);
    free(image);
}

image_t *image_load(const char *file, const char *type) {
    TGAError err = tga_load(file);
    if (err != TGA_OK) {
        printf("Error: %s\n", tga_error_str(err));
        return NULL;
    }
}
