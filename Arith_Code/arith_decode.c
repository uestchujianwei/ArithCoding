/*

文件 ： arith_decode.h

作者 ： 胡建伟(hjw@inno-gence.com)

简介 ： 算术编码算法的解码模块

公司 ： 四川创智联恒科技有限公司(www.inno-gence.com)

备注 ： 无

日志 ：

20190527 - 新增    创建文件

*/

#include <memory.h>
#include "arith_decode.h"
#include "arith_model.h"


/**************************************************************************************************
* 函数 : input_bit
* 描述 : 输入一个压缩数据位
* 输入 :
* coder                                          算术解码器指针
* 输出 : 无
* 返回 : 一个数据位（0或1）
* 调用 : 内部调用
* 备注 : 无
---------------------------------------------------------------------------------------------------*/
static unsigned int input_bit(ARITH_CODER * coder)
{
    unsigned int bit = 0U;                                                                         // 计数器

    if (0 == coder->bits_to_go)                                                                    // 如果缓存区已空
    {
        coder->buffer = coder->data_in[coder->in_counter++];                                       // 读入新数据
        coder->bits_to_go = 8;                                                                     // 缓存区有效位数为1字节（8位）
    }
    else                                                                                           // 如果缓存区不为空
    {
    }
    bit = coder->buffer & 1;                                                                       // 取出缓存区中下1位
    coder->buffer >>= 1;                                                                           // 缓存区右移1位
    --coder->bits_to_go;                                                                           // 缓存区有效位数递减1

    return bit;                                                                                    // 返回读取到的数据位（1/0）
}

/**************************************************************************************************
* 函数 : reset_decode_IO
* 描述 : 重置解码器输入输出计数器
* 输入 :
* coder                                          算术解码器指针
* 输出 : 无
* 返回 : 无
* 调用 : 外部调用
* 备注 : 进行块解码时使用，将输入输出计数器归零
---------------------------------------------------------------------------------------------------*/
void reset_decode_IO(ARITH_CODER * coder)
{
    coder->in_counter = 0;                                                                         // 重置输出计数器
    coder->out_counter = 0;                                                                        // 重置输入计数器
}

/**************************************************************************************************
* 函数 : start_decode
* 描述 : 开始解码
* 输入 :
* coder                                          算术解码器指针
* dest                                           目标地址
* dest_len                                       目标地址内存大小
* src                                            源地址
* src_len                                        源地址内存大小
* 输出 : 无
* 返回 : 无
* 调用 : 外部调用
* 备注 : 解码之前对解码器进行初始化
---------------------------------------------------------------------------------------------------*/
void start_decode(ARITH_CODER * coder, unsigned char * dest, unsigned int dest_len, const unsigned char * src, unsigned int src_len)
{
    unsigned int i = 0U;                                                                           // 计数器

    coder->data_in = src;                                                                          // 设置输入地址
    coder->data_in_len = src_len;                                                                  // 输入数据长度设置为源长度
    coder->data_out = dest;                                                                        // 设置输出地址
    coder->data_out_len = dest_len;                                                                // 输出数据长度设置为目标长度
    coder->bits_to_follow = 0;                                                                     // 待输出数据位数初始化为0
    coder->buffer = 0;                                                                             // 数据缓存区初始化为空
    coder->bits_to_go = 0;                                                                         // 可用数据位初始化为0
    coder->in_counter = 0;                                                                         // 输入计数器初始化为0
    coder->out_counter = 0;                                                                        // 输出计数器初始化为0
    coder->low = 0;                                                                                // low初始化为其最小值
    coder->high = TOP_VALUE;                                                                       // high初始化为其最大值
    coder->magic = 0;                                                                              // 为读取输入数据首先将magic归零
    for (i = 1; i <= BITS_IN_REGISTER; ++i)                                                        // 读取输入数据的头部
    {
        coder->magic = (coder->magic << 1) + input_bit(coder);
    }
}

/**************************************************************************************************
* 函数 : decode_symbol
* 描述 : 解码一个符号
* 输入 :
* coder                                          算术解码器指针
* 输出 : 无
* 返回 : 解码出来的符号
* 调用 : 外部调用
* 备注 : 无
---------------------------------------------------------------------------------------------------*/
unsigned int decode_symbol(ARITH_CODER * coder)
{
    unsigned int range = 0U;                                                                       // 当前区间
    unsigned int cum = 0U, symbol = 0U;                                                            // 累计频次，当前字符

    range = coder->high - coder->low + 1;                                                          // 计算当前区间
    cum = ((coder->magic - coder->low + 1) * coder->cum_freq[0] - 1) / range;                      // 根据当前区间计算累计频次
    for (symbol = 1; coder->cum_freq[symbol] > cum; ++symbol);                                     // 根据累计频次搜索当前字符
    coder->high = coder->low + (range * coder->cum_freq[symbol - 1]) / coder->cum_freq[0] - 1;     // 更新区间上限
    coder->low = coder->low + (range * coder->cum_freq[symbol]) / coder->cum_freq[0];              // 更新区间下限
    for (;;)                                                                                       // 对区间范围进行标准化
    {
        if (coder->high < HALF)                                                                    // 区间上限小于1/2
        {
        }
        else if (coder->low >= HALF)                                                               // 区间下限大于等于1/2
        {
            coder->magic -= HALF;
            coder->low -= HALF;
            coder->high -= HALF;
        }
        else if (coder->low >= FIRST_QTR && coder->high < THIRD_QTR)                               // 区间下限大于等于1/4且区间上限小于3/4
        {
            coder->magic -= FIRST_QTR;
            coder->low -= FIRST_QTR;
            coder->high -= FIRST_QTR;
        }
        else                                                                                       // 标准化结束
        {
            break;
        }
        coder->low = coder->low << 1;                                                              // 区间下限左移1位
        coder->high = (coder->high << 1) + 1;                                                      // 区间上限左移1位且加1以防止边界错误
        coder->magic = (coder->magic << 1) + input_bit(coder);                                     // 读入新的数据位
    }

    return symbol;                                                                                 // 返回解码出的字符
}

/**************************************************************************************************
* 函数 : done_decode
* 描述 : 结束解码
* 输入 :
* coder                                          算术解码器指针
* 输出 : 剩余的字符
* 返回 : 解码长度
* 调用 : 外部调用
* 备注 : 结束后整个解码器被清空
---------------------------------------------------------------------------------------------------*/
unsigned int done_decode(ARITH_CODER * coder)
{
    unsigned int ret = coder->out_counter;                                                         // 返回值为输出计数器

    memset(coder, 0, sizeof(ARITH_CODER));                                                         // 将解码器清空

    return ret;                                                                                    // 返回最终解码输出字符数
}
