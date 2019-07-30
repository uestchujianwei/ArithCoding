/*

文件 ： arith_model.h

作者 ： 胡建伟(hjw@inno-gence.com)

简介 ： 算术编码算法的算法模型

公司 ： 四川创智联恒科技有限公司(www.inno-gence.com)

备注 ： 无

日志 ：

20190527 - 新增    创建文件

*/

#include "arith_model.h"


/**************************************************************************************************
* 函数 : init_adapt_model
* 描述 : 初始化自适应模型
* 输入 :
* coder                                          算术编解码器指针
* 输出 : 经过初始化的自适应模型
* 返回 : 无
* 调用 : 外部调用
* 备注 : 无
---------------------------------------------------------------------------------------------------*/
void init_adapt_model(ARITH_CODER * coder)
{
    unsigned int i = 0U;                                                                           // 计数器

    for (i = 0; i < NO_OF_CHARS; ++i)                                                              // 初始化索引
    {
        coder->char_to_index[i] = i + 1;
        coder->index_to_char[i + 1] = (unsigned char)i;
    }
    for (i = 0; i <= NO_OF_SYMBOLS; ++i)                                                           // 初始化频率分布为全1
    {
        coder->freq[i] = 1;
        coder->cum_freq[i] = NO_OF_SYMBOLS - i;
    }
    coder->freq[0] = 0;                                                                            // 使freq[0]与freq[1]不同
}

/**************************************************************************************************
* 函数 : update_model
* 描述 : 更新模型
* 输入 :
* coder                                          算术编解码器指针
* symbol                                         当前符号
* 输出 : 更新后的模型
* 返回 : 无
* 调用 : 外部调用
* 备注 : 无
---------------------------------------------------------------------------------------------------*/
void update_model(ARITH_CODER * coder, unsigned int symbol)
{
    unsigned int i = 0U, cum = 0U;                                                                 // 计数器
    unsigned int ch_i = 0U, ch_symbol = 0U;                                                        // 字符和符号

    if (coder->cum_freq[0] >= MAX_FREQUENCY)                                                       // 如果频率计数器溢出
    {
        for (cum = 0, i = NO_OF_SYMBOLS; i <= NO_OF_SYMBOLS; --i)                                  // 将频率计数器减半
        {
            coder->freq[i] = (coder->freq[i] + 1) >> 1;
            coder->cum_freq[i] = cum;
            cum += coder->freq[i];
        }
    }
    else                                                                                           // 如果频率计数器未溢出
    {
    }
    for (i = symbol; coder->freq[i] == coder->freq[i - 1]; --i);                                   // 找到symbol对应的频率分布
    if (i < symbol)                                                                                // 如果当前索引值小于符号
    {
        ch_i = coder->index_to_char[i];
        ch_symbol = coder->index_to_char[symbol];
        coder->index_to_char[i] = (unsigned char)ch_symbol;
        coder->index_to_char[symbol] = (unsigned char)ch_i;
        coder->char_to_index[ch_i] = symbol;
        coder->char_to_index[ch_symbol] = i;
    }
    else                                                                                           // 如果当前索引值大于等于符号
    {
    }
    ++coder->freq[i];                                                                              // 频次累加1
    while (i)                                                                                      // 以下的所有频次累加1
    {
        ++coder->cum_freq[--i];
    }
}
