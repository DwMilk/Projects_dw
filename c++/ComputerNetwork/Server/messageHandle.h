#ifndef __MESSAGEHANDLE_H__
#define __MESSAGEHANDLE_H__
#include <unordered_map>
#include "function.h"
#include <functional>
#include "stdShared.h"
#include "stdTcpServer.h"
/* 函数注册表 */
class MessageHandle
{
public:
    using HandleFunction = std::function<void(const Msg &)>;
    /* 构造函数 */
    MessageHandle(const StdTcpSocketPtr & clientInfo);
    ~MessageHandle();
public:
    /* 处理信息 */
    void handleMessage(const Msg & msg);
private:
    Function m_function;
    /* 映射 */
    std::unordered_map<int,HandleFunction> m_handles;
};

#endif