/*

文件 ：main.cpp

作者 ：胡建伟(hjw@inno-gence.com)

简介 ：算术编码功能和性能测试

公司 ：四川创智联恒科技有限公司(www.inno-gence.com)

备注 ：无

日志 ：

20190527 - 新增    创建文件

*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "arith_code.h"

/**************************************************************************************************
* 函数 ： get_file_size
* 描述 ： 获取文件尺寸
* 输入 ：
* filepath                                       文件路径
* 输出 ： 无
* 返回 ： 文件尺寸
* 调用 ： 内部调用
* 备注 ： 尺寸单位为字节
---------------------------------------------------------------------------------------------------*/
static unsigned int get_file_size(const char * filepath)
{
    unsigned int size = 0U;                                                                        // 尺寸
    FILE * file = NULL;                                                                            // 文件指针

    fopen_s(&file, filepath, "r");                                                                 // 只读打开文件
    if (file)                                                                                      // 如果文件打开成功
    {
        fseek(file, 0L, SEEK_END);                                                                 // 将指针移动到文件尾部
        size = (unsigned int)ftell(file);                                                          // 获取指针当前位置（文件尺寸）
        fclose(file);                                                                              // 关闭文件
    }
    else                                                                                           // 如果文件打开失败
    {
    }

    return size;                                                                                   // 返回文件尺寸
}

/**************************************************************************************************
* 函数 ： init_path
* 描述 ： 初始化路径
* 输入 ：
* filepath                                       文件路径
* path_src                                       源文件路径
* path_comp                                      压缩文件路径
* path_uncomp                                    解压文件路径
* 输出 ： 特定格式的文件路径字符串
* 返回 ： 无
* 调用 ： 内部调用
* 备注 ： 无
---------------------------------------------------------------------------------------------------*/
static void init_path(const char * filepath, char * path_src, char * path_comp, char * path_uncomp)
{
    char driver[_MAX_DRIVE] = { 0 }, directory[_MAX_DIR] = { 0 };                                  // 驱动器、目录
    char filename[_MAX_FNAME] = { 0 }, suffix[_MAX_EXT] = { 0 };                                   // 文件名、后缀名

    _splitpath(filepath, driver, directory, filename, suffix);                                     // 解析文件路径
    sprintf_s(path_src, _MAX_PATH, "%s", filepath);                                                // 生成源文件路径
    sprintf_s(path_comp, _MAX_PATH, "%s%s%s_enc.solid", driver, directory, filename);              // 生成压缩文件路径
    sprintf_s(path_uncomp, _MAX_PATH, "%s%s%s_dec%s", driver, directory, filename, suffix);        // 生成解压文件路径
}

/**************************************************************************************************
* 函数 ： test_func
* 描述 ： 测试文件压缩/解压的功能和性能
* 输入 ：
* filepath                                       文件路径
* 输出 ： 压缩文件和解压文件
* 返回 ： 无
* 调用 ： 内部调用
* 备注 ： 无
---------------------------------------------------------------------------------------------------*/
static void test_func(const char * filepath)
{
    char path_src[_MAX_PATH] = { 0 }, path_comp[_MAX_PATH] = { 0 }, path_uncomp[_MAX_PATH] = { 0 };// 文件路径（源文件、压缩文件和解压文件）
    unsigned char * buffer_src = NULL, * buffer_comp = NULL, * buffer_uncomp = NULL;               // 缓存区（源文件、压缩文件和解压文件）
    FILE * file_src = NULL, * file_comp = NULL, * file_uncomp = NULL;                              // 文件指针（源文件、压缩文件和解压文件）
    unsigned int file_size = 0U, len_src = 0U, len_comp = 0U, len_uncomp = 0U;                     // 文件尺寸，缓存区尺寸（源文件、压缩文件和解压文件）
    unsigned int tick_begin = 0U, tick_end = 0U;                                                   // 动作开始时刻，动作结束时刻

    /* 初始化路径 */
    printf("file path : %s\n", filepath);
    init_path(filepath, path_src, path_comp, path_uncomp);                                         // 初始化文件路径

    /* 读取文件 */
    printf("loading-------------------------------------------------------------\n");
    file_size = get_file_size(path_src);                                                           // 获取文件尺寸
    buffer_src = (unsigned char*)malloc(file_size);                                                // 分配源文件缓存区
    fopen_s(&file_src, path_src, "rb");                                                            // 只读打开源文件
    len_src = fread(buffer_src, sizeof(char), file_size, file_src);                                // 读取源文件
    printf("len_src = %10u, len_comp = %10u, len_uncomp = %10u\n", len_src, len_comp, len_uncomp);
    printf("entropy : %lf\n", arith_entropy(buffer_src, len_src));
    printf("limit size : %u\n", (unsigned int)(len_src * arith_entropy(buffer_src, len_src)));
    fclose(file_src);                                                                              // 关闭源文件

    /* 压缩文件 */
    printf("compress------------------------------------------------------------\n");
    len_comp = arith_compress_bound(buffer_src, len_src);                                          // 计算算术压缩缓存区边界
    buffer_comp = (unsigned char*)malloc(len_comp);                                                // 分配压缩文件缓存区
    tick_begin = GetTickCount();                                                                   // 记录压缩前时刻
    arith_compress(buffer_comp, &len_comp, buffer_src, len_src);                                   // 压缩文件
    tick_end = GetTickCount();                                                                     // 记录压缩后时刻
    printf("len_src = %10u, len_comp = %10u, len_uncomp = %10u\n", len_src, len_comp, len_uncomp);
    printf("entropy : %lf\n", arith_entropy(buffer_comp, len_comp));
    printf("elapsed : %u ms\n", tick_end - tick_begin);
    fopen_s(&file_comp, path_comp, "wb");                                                          // 只写打开压缩文件
    fwrite(buffer_comp, sizeof(char), len_comp, file_comp);                                        // 写入压缩文件
    fclose(file_comp);                                                                             // 关闭压缩文件

    /* 解压文件 */
    printf("uncompress----------------------------------------------------------\n");
    len_uncomp = len_src;                                                                          // 设置解压缓存区长度
    buffer_uncomp = (unsigned char*)malloc(len_uncomp);                                            // 分配解压缓存区
    tick_begin = GetTickCount();                                                                   // 记录解压前时刻
    arith_uncompress(buffer_uncomp, &len_uncomp, buffer_comp, len_comp);                           // 解压文件
    tick_end = GetTickCount();                                                                     // 记录解压后时刻
    printf("len_src = %10u, len_comp = %10u, len_uncomp = %10u\n", len_src, len_comp, len_uncomp);
    printf("entropy : %lf\n", arith_entropy(buffer_uncomp, len_uncomp));
    printf("elapsed : %u ms\n", tick_end - tick_begin);
    printf("====================================================================\n");
    fopen_s(&file_uncomp, path_uncomp, "wb");                                                      // 只写打开解压文件
    fwrite(buffer_uncomp, sizeof(char), len_uncomp, file_uncomp);                                  // 写入解压文件
    fclose(file_uncomp);                                                                           // 关闭解压文件

    /* 释放内存 */
    free(buffer_src);                                                                              // 释放源文件缓存区
    free(buffer_comp);                                                                             // 释放压缩文件缓存区
    free(buffer_uncomp);                                                                           // 释放解压文件缓存区
}

/**************************************************************************************************
* 函数 : main
* 描述 : 主函数
* 输入 :
* argc                                           参数个数
* argv                                           参数列表
* 输出 : 无
* 返回 : 错误代码
* 调用 : 外部调用
* 备注 : 无
---------------------------------------------------------------------------------------------------*/
int main(int argc, char * argv[])
{
    for (int i = 1; i < argc; ++i)                                                                 // 对输入的文件逐个处理
    {
        test_func(argv[i]);                                                                        // 处理当前路径指向的文件
    }

    getchar();                                                                                     // 等待键入
    return 0;                                                                                      // 正常返回
}
