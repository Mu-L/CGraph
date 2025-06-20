/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GElementDefine.h
@Time: 2022/4/9 15:31
@Desc: 
***************************/

#ifndef CGRAPH_GELEMENTDEFINE_H
#define CGRAPH_GELEMENTDEFINE_H

#include "../GraphObject.h"

CGRAPH_NAMESPACE_BEGIN

const static CSize CGRAPH_DEFAULT_LOOP_TIMES = 1;               // 默认循环次数信息
const static CLevel CGRAPH_DEFAULT_ELEMENT_LEVEL = 0;           // 默认的element级别，用于控制init函数
const static CIndex CGRAPH_DEFAULT_BINDING_INDEX = -1;          // 默认绑定线程id，-1表示不绑定
const static CMSec CGRAPH_DEFAULT_ELEMENT_TIMEOUT = 0;          // 默认element的超时时间

enum class GElementType {
    ELEMENT = 0x00000000,                    // 元素
    NODE = 0x00010000,                       // 节点
    GROUP = 0x00020000,                      // 组
    CLUSTER = 0x00020001,                    // 簇
    REGION = 0x00020002,                     // 区域
    CONDITION = 0x00020004,                  // 条件
    SOME = 0x00020008,                       // 部分
    MUTABLE = 0x0002000A,                    // 异变
    MULTI_CONDITION = 0x00020014,            // 多条件
    ADAPTER = 0x00040000,                    // 适配器
    FUNCTION = 0x00040001,                   // 函数
    SINGLETON = 0x00040002,                  // 单例
    FENCE = 0x00040004,                      // 栅栏
    COORDINATOR = 0x00040008,                // 协调
};


enum class GElementState {
    NORMAL = 0x0000,                          // 正常状态
    CANCEL = 0x1001,                          // 取消状态
    SUSPEND = 0x1002,                         // 暂停状态（同 v3.0.0版本之前的 YIELD 含义相同）
    TIMEOUT = 0x1010,                         // 超时状态
};
using GPipelineState = GElementState;         // pipeline 同element共享状态


enum class GElementTimeoutStrategy {
    AS_ERROR = 0,                             // 当做异常处理（默认）
    HOLD_BY_PIPELINE = 1,                     // pipeline run执行完成之前，等待结束
    NO_HOLD = 2,                              // 不等待结束。非特殊场景，强烈不推荐使用，不排除个别平台会出现崩溃的情况
};


CGRAPH_INTERNAL_NAMESPACE_BEGIN
enum class GElementShape {
    NORMAL = 0,                               // 普通的元素
    LINKABLE = 1,                             // 后继是唯一元素的情况（类似 list）
    ROOT = 2,                                 // 所有后继元素，只有一个前驱的情况（类似tree root）
    TAIL = 3,                                 // 尾部节点
};
CGRAPH_INTERNAL_NAMESPACE_END

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GELEMENTDEFINE_H
