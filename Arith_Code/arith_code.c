/*

文件 ： arith_coding.c

作者 ： 胡建伟(hjw@inno-gence.com)

简介 ： 基于算术编码的压缩/解压

公司 ： 四川创智联恒科技有限公司(www.inno-gence.com)

备注 ： 无

日志 ：

20190527 - 新增    创建文件

*/

#include <math.h>
#include "arith_code.h"
#include "arith_model.h"
#include "arith_encode.h"
#include "arith_decode.h"


/**************************************************************************************************
* 函数 : arith_entropy
* 描述 : 信息熵
* 输入 :
* data                                           信息空间中的数据
* n                                              数据量
* 输出 : 无
* 返回 : 熵
* 调用 : 外部调用
* 备注 : 无
---------------------------------------------------------------------------------------------------*/
double arith_entropy(const void * data, unsigned int n)
{
    register unsigned int i = 0U;                                                                  // 计数器
    unsigned int hist[256] = { 0 };                                                                // 概率表
    const unsigned char * ptr_ch = (const unsigned char*)data;                                     // 字节数据指针
    double ret = 0.0;                                                                              // 返回值

    for (i = 0; i < n; ++i)                                                                        // 概率统计
    {
        ++hist[(unsigned char)ptr_ch[i]];                                                          // 此符号概率累加
    }
    for (i = 0; i < 256; ++i)                                                                      // 积分计算信息熵
    {
        if (hist[i])                                                                               // 如果此符号概率不为0
        {
            ret += ((double)hist[i] / (double)n) * (double)(log(1.0 / ((double)hist[i] / (double)n)) / log(2.0));// 信息熵公式
        }
        else                                                                                       // 如果此符号概率为0
        {
        }
    }

    return ret * 0.125;                                                                            // 返回信息熵
}

/**************************************************************************************************
* 函数 : arith_compress_bound
* 描述 : 计算算术编码缓存区边界
* 输入 :
* data                                           数据
* data_len                                       数据长度
* 输出 : 无
* 返回 : 算术编码缓存区边界
* 调用 : 外部调用
* 备注 : 无
---------------------------------------------------------------------------------------------------*/
unsigned int arith_compress_bound(const unsigned char * data, unsigned int data_len)
{
    return (unsigned int)(data_len * (arith_entropy(data, data_len) + SAD_ENTROPY)) + 256;         // 返回最大数据长度
}

/**************************************************************************************************
* 函数 : arith_compress
* 描述 : 算术编码压缩
* 输入 :
* dest                                           目标地址
* dest_len                                       目标地址内存大小
* src                                            源地址
* src_len                                        源地址内存大小
* 输出 : 编码得到的字符
* 返回 : 否
* 调用 : 外部调用
* 备注 : 压缩前需要准备较大的dest空间，压缩完成后dest_len将变成真正的压缩后数据大小
---------------------------------------------------------------------------------------------------*/
void arith_compress(unsigned char * dest, unsigned int * dest_len, const unsigned char * src, unsigned int src_len)
{
    unsigned int i = 0U, symbol = 0U;                                                              // 计数器，编码字符
    unsigned char ch = 0U;                                                                         // 当前字符
    ARITH_CODER coder = { 0 };                                                                     // 编码器

    init_adapt_model(&coder);                                                                      // 初始化自适应模型
    start_encode(&coder, dest, *dest_len, src, src_len);                                           // 开始编码
    for (i = 0; i < src_len; ++i)                                                                  // 对源数据进行逐个编码
    {
        ch = coder.data_in[i];                                                                     // 读取当前字符
        symbol = coder.char_to_index[ch];                                                          // 将当前字符映射到编码字符
        encode_symbol(&coder, symbol);                                                             // 编码当前字符
        update_model(&coder, symbol);                                                              // 更新模型以自适应数据的概率特征
    }
    *dest_len = done_encode(&coder);                                                               // 结束编码并返回输出数据长度
}

/**************************************************************************************************
* 函数 : arith_uncompress
* 描述 : 算术编码解压
* 输入 :
* dest                                           目标地址
* dest_len                                       目标地址内存大小
* src                                            源地址
* src_len                                        源地址内存大小
* 输出 : 解码得到的字符
* 返回 : 否
* 调用 : 外部调用
* 备注 : 压缩前需要准备较大的dest空间，解压完成后dest_len将变成真正的解压后数据大小
---------------------------------------------------------------------------------------------------*/
void arith_uncompress(unsigned char * dest, unsigned int * dest_len, const unsigned char * src, unsigned int src_len)
{
    unsigned int ch = 0U, symbol = 0U;                                                             // 计数器，解码字符
    ARITH_CODER coder = { 0 };                                                                     // 解压器

    init_adapt_model(&coder);                                                                      // 初始化自适应模型
    start_decode(&coder, dest, *dest_len, src, src_len);                                           // 开始解码
    for (;;)                                                                                       // 循环直到解码出结束符
    {
        symbol = decode_symbol(&coder);                                                            // 解码出一个字符
        if (symbol == EOF_SYMBOL)                                                                  // 如果解出的符号是结束符
        {
            break;                                                                                 // 结束解码
        }
        else                                                                                       // 如果解出的符号不是结束符
        {
        }
        ch = coder.index_to_char[symbol];                                                          // 将当前解码字符映射为输出字符
        coder.data_out[coder.out_counter++] = (unsigned char)ch;                                   // 输出解码出来的字符
        update_model(&coder, symbol);                                                              // 更新模型以还原编码时的模型变化过程
    }
    *dest_len = done_decode(&coder);                                                               // 结束解码并返回输出数据长度
}
