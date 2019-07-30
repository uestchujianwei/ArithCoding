/*

文件 ： arith_define.h

作者 ： 胡建伟(hjw@inno-gence.com)

简介 ： 算术编码算法的公用定义

公司 ： 四川创智联恒科技有限公司(www.inno-gence.com)

备注 ： 无

日志 ：

20190527 - 新增    创建文件

*/

#ifndef ARITH_DEFINE_H
#define ARITH_DEFINE_H

#define    BITS_IN_REGISTER       16                                                               // 寄存器位数
#define    TOP_VALUE              ((1<<BITS_IN_REGISTER) - 1)                                      // 寄存器允许的最大值
#define    FIRST_QTR              (TOP_VALUE/4 + 1)                                                // 四分之一范围
#define    HALF                   (2*FIRST_QTR)                                                    // 四分之二范围
#define    THIRD_QTR              (3*FIRST_QTR)                                                    // 四分之三范围
#define    NO_OF_CHARS            256                                                              // 字母表中的字符数
#define    EOF_SYMBOL             (NO_OF_CHARS + 1)                                                // 特殊符号（结束符）
#define    NO_OF_SYMBOLS          (NO_OF_CHARS + 1)                                                // 符号表中的符号数
#define    MAX_FREQUENCY          16383                                                            // 频数上限(2^14-1)

typedef struct
{
    const unsigned char * data_in;                                                                 // 数据输入指针
    unsigned char * data_out;                                                                      // 数据输出指针
    unsigned char index_to_char[NO_OF_SYMBOLS];                                                    // 索引到字符映射
    unsigned int char_to_index[NO_OF_CHARS];                                                       // 字符到索引映射
    unsigned int freq[NO_OF_SYMBOLS + 1];                                                          // 频率表
    unsigned int cum_freq[NO_OF_SYMBOLS + 1];                                                      // 累计频率表
    unsigned int bits_to_follow;                                                                   // 待输出的数据位数
    unsigned int buffer;                                                                           // 缓存区
    unsigned int bits_to_go;                                                                       // 缓存区待操作位数计数器
    unsigned int low;                                                                              // 下边界
    unsigned int high;                                                                             // 上边界
    unsigned int magic;                                                                            // 算术编码窗口
    unsigned int in_counter;                                                                       // 输入数据计数器
    unsigned int data_in_len;                                                                      // 输入数据的长度
    unsigned int out_counter;                                                                      // 输出数据计数器
    unsigned int data_out_len;                                                                     // 输出数据长度
}ARITH_CODER;                                                                                      // 算术编码器

#endif
