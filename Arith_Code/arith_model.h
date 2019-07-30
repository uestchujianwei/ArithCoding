/*

文件 ： arith_model.h

作者 ： 胡建伟(hjw@inno-gence.com)

简介 ： 算术编码算法的算法模型

公司 ： 四川创智联恒科技有限公司(www.inno-gence.com)

备注 ： 无

日志 ：

20190527 - 新增    创建文件

*/

#ifndef ARITH_MODEL_H
#define ARITH_MODEL_H

#include "arith_define.h"

/* 初始化自适应模型 */
void init_adapt_model(ARITH_CODER * coder);

/* 更新模型 */
void update_model(ARITH_CODER * coder, unsigned int symbol);

#endif
