/*

文件 ： arith_encode.h

作者 ： 胡建伟(hjw@inno-gence.com)

简介 ： 算术编码算法的编码模块

公司 ： 四川创智联恒科技有限公司(www.inno-gence.com)

备注 ： 无

日志 ：

20190527 - 新增    创建文件

*/

#include <memory.h>
#include "arith_encode.h"
#include "arith_model.h"


/**************************************************************************************************
* 函数 : output_bit
* 描述 : 输出一个压缩数据位
* 输入 :
* coder                                          算术编码器指针
* bit                                            输出的数据位
* 输出 : 一个数据位（0或1）
* 返回 : 无
* 调用 : 内部调用
* 备注 : bit为0则输出0，bit不为0则输出1
---------------------------------------------------------------------------------------------------*/
static void output_bit(ARITH_CODER * coder, unsigned int bit)
{
    coder->buffer >>= 1;                                                                           // 缓存区右移1位
    if (bit)                                                                                       // 如果bit不为0
    {
        coder->buffer |= 0x80;                                                                     // 输出1
    }
    else                                                                                           // 如果bit为0
    {
    }
    if (--coder->bits_to_go == 0)                                                                  // 如果缓存区已满
    {
        coder->data_out[coder->out_counter++] = (unsigned char)coder->buffer;                      // 将缓存区的数据输出
        coder->bits_to_go = 8;                                                                     // 缓存区可用数据位为1字节（8位）
    }
    else                                                                                           // 如果缓存区未满
    {
    }
}

/**************************************************************************************************
* 函数 : output_bits_follow
* 描述 : 输出压缩数据位
* 输入 :
* coder                                          算术编码器指针
* bit                                            输出的数据位
* 输出 : 接下来需要数据位
* 返回 : 无
* 调用 : 内部调用
* 备注 : bit为0则输出0，bit不为0则输出1（输出可能不止一位，包含保留位）
---------------------------------------------------------------------------------------------------*/
static void output_bits_follow(ARITH_CODER * coder, unsigned int bit)
{
    output_bit(coder, bit);                                                                        // 输出一个数据位
    for (; coder->bits_to_follow; --coder->bits_to_follow)                                         // 循环将数据标准化产生的数据位全部输出
    {
        output_bit(coder, !bit);
    }
}

/**************************************************************************************************
* 函数 : reset_encode_IO
* 描述 : 重置编码器输入输出计数器
* 输入 :
* coder                                          算术编码器指针
* 输出 : 无
* 返回 : 无
* 调用 : 外部调用
* 备注 : 进行块编码时使用，将输入输出计数器归零
---------------------------------------------------------------------------------------------------*/
void reset_encode_IO(ARITH_CODER * coder)
{
    coder->in_counter = 0;                                                                         // 重置输入计数器
    coder->out_counter = 0;                                                                        // 重置输出计数器
}

/**************************************************************************************************
* 函数 : start_encode
* 描述 : 开始编码
* 输入 :
* coder                                          算术编码器指针
* dest                                           目标地址
* dest_len                                       目标地址内存大小
* src                                            源地址
* src_len                                        源地址内存大小
* 输出 : 无
* 返回 : 无
* 调用 : 外部调用
* 备注 : 编码之前对编码器进行初始化
---------------------------------------------------------------------------------------------------*/
void start_encode(ARITH_CODER * coder, unsigned char * dest, unsigned int dest_len, const unsigned char * src, unsigned int src_len)
{
    coder->data_in = src;                                                                          // 设置输入地址
    coder->data_in_len = src_len;                                                                  // 输入数据长度设置为源长度
    coder->data_out = dest;                                                                        // 设置输出地址
    coder->data_out_len = dest_len;                                                                // 输出数据长度设置为目标长度
    coder->bits_to_follow = 0;                                                                     // 待输出数据位数初始化为0
    coder->buffer = 0;                                                                             // 数据缓存区初始化为空
    coder->bits_to_go = 8;                                                                         // 可用数据位初始化为1字节（8位）
    coder->in_counter = 0;                                                                         // 输入计数器初始化为0
    coder->out_counter = 0;                                                                        // 输出计数器初始化为0
    coder->low = 0;                                                                                // low初始化为其最小值
    coder->high = TOP_VALUE;                                                                       // high初始化为其最大值
    coder->magic = 0;                                                                              // magic初始化为0
}

/**************************************************************************************************
* 函数 : encode_symbol
* 描述 : 编码一个符号
* 输入 :
* coder                                          算术编码器指针
* symbol                                         需要编码的符号（可能是字符，也可能是结束符）
* 输出 : symbol的算术编码
* 返回 : 无
* 调用 : 外部调用
* 备注 : 无
---------------------------------------------------------------------------------------------------*/
void encode_symbol(ARITH_CODER * coder, unsigned int symbol)
{
    unsigned int range = 0U;                                                                       // 当前区间

    range = coder->high - coder->low + 1;                                                          // 计算当前区间
    coder->high = coder->low + (range * coder->cum_freq[symbol - 1]) / coder->cum_freq[0] - 1;     // 更新区间上限
    coder->low = coder->low + (range * coder->cum_freq[symbol]) / coder->cum_freq[0];              // 更新区间下限
    for (;;)                                                                                       // 对区间范围进行标准化
    {
        if (coder->high < HALF)                                                                    // 区间上限小于1/2
        {
            output_bits_follow(coder, 0);
        }
        else if (coder->low >= HALF)                                                               // 区间下限大于等于1/2
        {
            output_bits_follow(coder, 1);
            coder->low -= HALF;
            coder->high -= HALF;
        }
        else if (coder->low >= FIRST_QTR && coder->high < THIRD_QTR)                               // 区间下限大于等于1/4且区间上限小于3/4
        {
            ++coder->bits_to_follow;
            coder->low -= FIRST_QTR;
            coder->high -= FIRST_QTR;
        }
        else                                                                                       // 标准化结束
        {
            break;
        }
        coder->low = coder->low << 1;                                                              // 区间下限左移1位
        coder->high = (coder->high << 1) + 1;                                                      // 区间上限左移1位且加1以防止边界错误
    }
}

/**************************************************************************************************
* 函数 : done_encode
* 描述 : 结束编码
* 输入 :
* coder                                          算术编码器指针
* 输出 : 剩余的算术编码
* 返回 : 编码长度
* 调用 : 外部调用
* 备注 : 结束后整个编码器被清空
---------------------------------------------------------------------------------------------------*/
unsigned int done_encode(ARITH_CODER * coder)
{
    unsigned int ret = 0U;                                                                         // 返回值为输出计数器

    encode_symbol(coder, EOF_SYMBOL);                                                              // 将结束符写入编码数据中
    ++coder->bits_to_follow;                                                                       // 待编码位数累加1
    if (coder->low < FIRST_QTR)                                                                    // 如果当前区间小于1/4
    {
        output_bits_follow(coder, 0);
    }
    else                                                                                           // 如果当前区间大于等于1/4
    {
        output_bits_follow(coder, 1);
    }
    coder->buffer >>= coder->bits_to_go;                                                           // 缓存区右移
    coder->data_out[coder->out_counter++] = (unsigned char)coder->buffer;                          // 将缓存区的数据输出
    ret = coder->out_counter;                                                                      // 设置返回值为输出计数器
    memset(coder, 0, sizeof(ARITH_CODER));                                                         // 将解码器清空

    return ret;                                                                                    // 返回最终编码输出字符数
}
