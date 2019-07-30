/*

文件 ： arith_coding.h

作者 ： 胡建伟(hjw@inno-gence.com)

简介 ： 基于算术编码的压缩/解压

公司 ： 四川创智联恒科技有限公司(www.inno-gence.com)

备注 ： 无

日志 ：

20190527 - 新增    创建文件

*/

#ifndef ARITH_CODING_H
#define ARITH_CODING_H

#ifdef __cplusplus
extern "C" {
#endif

#include "arith_define.h"

#define          HAPPY_ENTROPY          0.99                                                       // 满意的信息熵
#define          SAD_ENTROPY            0.01                                                       // 极不满意的信息熵

/* 信息熵 */
double arith_entropy(const void * data, unsigned int n);

/* 计算算术编码缓存区边界 */
unsigned int arith_compress_bound(const unsigned char * data, unsigned int data_len);

/* 算术编码压缩 */
void arith_compress(unsigned char * dest, unsigned int * dest_len, const unsigned char * src, unsigned int src_len);

/* 算术编码解压 */
void arith_uncompress(unsigned char * dest, unsigned int * dest_len, const unsigned char * src, unsigned int src_len);

#ifdef __cplusplus
}
#endif

#endif
