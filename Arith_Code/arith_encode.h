/*

文件 ： arith_encode.h

作者 ： 胡建伟(hjw@inno-gence.com)

简介 ： 算术编码算法的编码模块

公司 ： 四川创智联恒科技有限公司(www.inno-gence.com)

备注 ： 无

日志 ：

20190527 - 新增    创建文件

*/

#ifndef ARITH_ENCODE_H
#define ARITH_ENCODE_H

#include "arith_define.h"

/* 重置编码器输入输出计数器 */
void reset_encode_IO(ARITH_CODER * coder);

/* 开始编码 */
void start_encode(ARITH_CODER * coder, unsigned char * dest, unsigned int dest_len, const unsigned char * src, unsigned int src_len);

/* 编码一个符号 */
void encode_symbol(ARITH_CODER * coder, unsigned int symbol);

/* 结束编码 */
unsigned int done_encode(ARITH_CODER * coder);

#endif
