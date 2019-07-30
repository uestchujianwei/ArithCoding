/*

文件 ： arith_decode.h

作者 ： 胡建伟(hjw@inno-gence.com)

简介 ： 算术编码算法的解码模块

公司 ： 四川创智联恒科技有限公司(www.inno-gence.com)

备注 ： 无

日志 ：

20190527 - 新增    创建文件

*/

#ifndef ARITH_DECODE_H
#define ARITH_DECODE_H

#include "arith_define.h"

/* 重置解码器输入输出计数器 */
void reset_decode_IO(ARITH_CODER * coder);

/* 开始解码 */
void start_decode(ARITH_CODER * coder, unsigned char * dest, unsigned int dest_len, const unsigned char * src, unsigned int src_len);

/* 解压一个符号 */
unsigned int decode_symbol(ARITH_CODER * coder);

/* 结束解码 */
unsigned int done_decode(ARITH_CODER * coder);

#endif
