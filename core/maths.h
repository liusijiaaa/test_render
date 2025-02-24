#ifndef MATHS_H
#define MATHS_H

/* 二维向量结构体，用于存储2D空间中的坐标/向量/纹理坐标等数据 */
typedef struct {float x, y;} vec2_t;

/* 三维向量结构体，用于存储3D空间中的坐标/向量/颜色(RGB)等数据 */
typedef struct {float x, y, z;} vec3_t;

/* 四维向量结构体，用于存储齐次坐标/颜色(RGBA)等数据
   x: X轴分量
   y: Y轴分量
   z: Z轴分量
   w: 齐次坐标的w分量或颜色的Alpha通道 */
typedef struct {float x, y, z, w;} vec4_t;

/* 四元数结构体，用于表示三维空间旋转
   x: 四元数虚部i分量
   y: 四元数虚部j分量
   z: 四元数虚部k分量
   w: 四元数实部标量分量 */
typedef struct {float x, y, z, w;} quat_t;

/* 3x3矩阵结构体，用于表示三维空间的线性变换
   m: 二维数组存储的矩阵元素，行优先顺序
      可用于存储旋转、缩放等变换矩阵 */
typedef struct {float m[3][3];} mat3_t;

/* 4x4矩阵结构体，用于表示三维空间的仿射变换
   m: 二维数组存储的矩阵元素，行优先顺序
      常用于存储模型视图投影矩阵等复合变换 */
typedef struct {float m[4][4];} mat4_t;

/* float related functions */
/**
 * @brief 返回两个浮点数中的较小值
 * 
 * @param a 第一个浮点数
 * @param b 第二个浮点数
 * @return float 较小的浮点数
 */
float float_min(float a, float b);

/**
 * @brief 返回两个浮点数中的较大值
 * 
 * @param a 第一个浮点数
 * @param b 第二个浮点数
 * @return float 较大的浮点数
 */
float float_max(float a, float b);

/**
 * @brief 在两个浮点数之间进行线性插值
 * 
 * @param a 起始浮点数
 * @param b 结束浮点数
 * @param t 插值因子，决定插值结果接近a还是b
 * @return float 插值结果
 */
float float_lerp(float a, float b, float t);

/**
 * @brief 将浮点数限制在指定的最小值和最大值之间
 * 
 * @param f 待限制的浮点数
 * @param min 最小值
 * @param max 最大值
 * @return float 限制后的浮点数
 */
float float_clamp(float f, float min, float max);

/**
 * @brief 将浮点数限制在0到1之间
 * 
 * @param f 待限制的浮点数
 * @return float 限制后的浮点数
 */
float float_saturate(float f);

/**
 * @brief 将无符号字符转换为浮点数
 * 
 * @param value 无符号字符值
 * @return float 转换后的浮点数
 */
float float_from_uchar(unsigned char value);

/**
 * @brief 将浮点数转换为无符号字符
 * 
 * @param value 浮点数值
 * @return unsigned char 转换后的无符号字符
 */
unsigned char float_to_uchar(float value);

/**
 * @brief 将sRGB颜色空间的浮点数转换为线性颜色空间
 * 
 * @param value sRGB颜色值
 * @return float 线性颜色值
 */
float float_srgb2linear(float value);

/**
 * @brief 将线性颜色空间的浮点数转换为sRGB颜色空间
 * 
 * @param value 线性颜色值
 * @return float sRGB颜色值
 */
float float_linear2srgb(float value);

/**
 * @brief 使用ACES色调映射曲线转换浮点数
 * 
 * @param value 待转换的浮点数
 * @return float 转换后的浮点数
 */
float float_aces(float value);

/**
 * @brief 打印浮点数的值
 * 
 * @param name 浮点数的名称
 * @param f 待打印的浮点数
 */
void float_print(const char *name, float f);

/* vec2 related functions */
/**
 * 创建一个新的二维向量。
 * 
 * @param x 向量的x分量。
 * @param y 向量的y分量。
 * @return 新创建的二维向量。
 */
vec2_t vec2_new(float x, float y);

/**
 * 计算两个二维向量的最小值。
 * 
 * @param a 第一个向量。
 * @param b 第二个向量。
 * @return 一个新的向量，其分量是输入向量分量的最小值。
 */
vec2_t vec2_min(vec2_t a, vec2_t b);

/**
 * 计算两个二维向量的最大值。
 * 
 * @param a 第一个向量。
 * @param b 第二个向量。
 * @return 一个新的向量，其分量是输入向量分量的最大值。
 */
vec2_t vec2_max(vec2_t a, vec2_t b);

/**
 * 向量加法。
 * 
 * @param a 第一个向量。
 * @param b 第二个向量。
 * @return 两个向量相加的结果。
 */
vec2_t vec2_add(vec2_t a, vec2_t b);

/**
 * 向量减法。
 * 
 * @param a 被减向量。
 * @param b 减向量。
 * @return 两个向量相减的结果。
 */
vec2_t vec2_sub(vec2_t a, vec2_t b);

/**
 * 向量与标量的乘法。
 * 
 * @param v 被乘的向量。
 * @param factor 乘数。
 * @return 乘法结果向量。
 */
vec2_t vec2_mul(vec2_t v, float factor);

/**
 * 向量与标量的除法。
 * 
 * @param v 被除的向量。
 * @param divisor 除数。
 * @return 除法结果向量。
 */
vec2_t vec2_div(vec2_t v, float divisor);

/**
 * 计算向量的长度（模）。
 * 
 * @param v 输入向量。
 * @return 向量的长度。
 */
float vec2_length(vec2_t v);

/**
 * 计算向量到由两个点定义的线段的最短距离。
 * 
 * @param start 线段的起点。
 * @param end 线段的终点。
 * @param v 输入向量。
 * @return 向量到线段的最短距离。
 */
float vec2_edge(vec2_t start, vec2_t end, vec2_t v);

/**
 * 打印向量的名称和值。
 * 
 * @param name 向量的名称。
 * @param v 要打印的向量。
 */
void vec2_print(const char *name, vec2_t v);

/* vec3 related functions */
// 创建一个新的三维向量
vec3_t vec3_new(float x, float y, float z);

// 从一个四维向量中转换出三维向量
vec3_t vec3_from_vec4(vec4_t v);

// 计算两个三维向量的最小值向量
vec3_t vec3_min(vec3_t a, vec3_t b);

// 计算两个三维向量的最大值向量
vec3_t vec3_max(vec3_t a, vec3_t b);

// 计算两个三维向量的和
vec3_t vec3_add(vec3_t a, vec3_t b);

// 计算两个三维向量的差
vec3_t vec3_sub(vec3_t a, vec3_t b);

// 计算三维向量与一个标量的乘积
vec3_t vec3_mul(vec3_t v, float factor);

// 计算三维向量与一个标量的商
vec3_t vec3_div(vec3_t v, float divisor);

// 计算三维向量的负向量
vec3_t vec3_negate(vec3_t v);

// 计算三维向量的长度
float vec3_length(vec3_t v);

// 计算三维向量的单位向量
vec3_t vec3_normalize(vec3_t v);

// 计算两个三维向量的点积
float vec3_dot(vec3_t a, vec3_t b);

// 计算两个三维向量的叉积
vec3_t vec3_cross(vec3_t a, vec3_t b);

// 对两个三维向量进行线性插值
vec3_t vec3_lerp(vec3_t a, vec3_t b, float t);

// 饱和三维向量的每个分量，使其在0到1之间
vec3_t vec3_saturate(vec3_t v);

// 计算两个三维向量的调制结果
vec3_t vec3_modulate(vec3_t a, vec3_t b);

// 打印三维向量的名称和值
void vec3_print(const char *name, vec3_t v);

/* vec4 related functions */
// 创建一个新的四维向量
vec4_t vec4_new(float x, float y, float z, float w);

// 从一个三维向量和一个w分量创建一个新的四维向量
vec4_t vec4_from_vec3(vec3_t v, float w);

// 添加两个四维向量
vec4_t vec4_add(vec4_t a, vec4_t b);

// 减去两个四维向量
vec4_t vec4_sub(vec4_t a, vec4_t b);

// 将四维向量乘以一个标量
vec4_t vec4_mul(vec4_t v, float factor);

// 将四维向量除以一个标量
vec4_t vec4_div(vec4_t v, float divisor);

// 在两个四维向量之间进行线性插值
vec4_t vec4_lerp(vec4_t a, vec4_t b, float t);

// 饱和四维向量的每个分量到[0,1]范围
vec4_t vec4_saturate(vec4_t v);

// 对两个四维向量进行逐元素相乘
vec4_t vec4_modulate(vec4_t a, vec4_t b);

// 打印四维向量的名称和值
void vec4_print(const char *name, vec4_t v);

/* quat related functions */
quat_t quat_new(float x, float y, float z, float w);
float quat_dot(quat_t a, quat_t b);
float quat_length(quat_t q);
quat_t quat_normalize(quat_t q);
quat_t quat_slerp(quat_t a, quat_t b, float t);
void quat_print(const char *name, quat_t q);

/* mat3 related functions */
mat3_t mat3_identity(void);
mat3_t mat3_from_cols(vec3_t c0, vec3_t c1, vec3_t c2);
mat3_t mat3_from_mat4(mat4_t m);
mat3_t mat3_combine(mat3_t m[4], vec4_t weights);
vec3_t mat3_mul_vec3(mat3_t m, vec3_t v);
mat3_t mat3_mul_mat3(mat3_t a, mat3_t b);
mat3_t mat3_inverse(mat3_t m);
mat3_t mat3_transpose(mat3_t m);
mat3_t mat3_inverse_transpose(mat3_t m);
void mat3_print(const char *name, mat3_t m);

/* mat4 related functions */
mat4_t mat4_identity(void);
mat4_t mat4_from_quat(quat_t q);
mat4_t mat4_from_trs(vec3_t t, quat_t r, vec3_t s);
mat4_t mat4_combine(mat4_t m[4], vec4_t weights);
vec4_t mat4_mul_vec4(mat4_t m, vec4_t v);
mat4_t mat4_mul_mat4(mat4_t a, mat4_t b);
mat4_t mat4_inverse(mat4_t m);
mat4_t mat4_transpose(mat4_t m);
mat4_t mat4_inverse_transpose(mat4_t m);
void mat4_print(const char *name, mat4_t m);

/* transformation matrices */
mat4_t mat4_translate(float tx, float ty, float tz);
mat4_t mat4_scale(float sx, float sy, float sz);
mat4_t mat4_rotate(float angle, float vx, float vy, float vz);
mat4_t mat4_rotate_x(float angle);
mat4_t mat4_rotate_y(float angle);
mat4_t mat4_rotate_z(float angle);
mat4_t mat4_lookat(vec3_t eye, vec3_t target, vec3_t up);
mat4_t mat4_ortho(float left, float right, float bottom, float top,
                  float near, float far);
mat4_t mat4_frustum(float left, float right, float bottom, float top,
                    float near, float far);
mat4_t mat4_orthographic(float right, float top, float near, float far);
mat4_t mat4_perspective(float fovy, float aspect, float near, float far);

#endif
