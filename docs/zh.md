# Jiyu-UDP-Attack-Cpp 头文件文档

## 头文件：`GetParams.h`

- 描述：用于获取控制台调用时的参数。

### 结构体：`ParamOpt`

- 描述：用于获取参数的选项结构体。
- 成员：

    #### 变量：`id`
    - 描述：一个参数的标识。
    - 类型：`std::string`
    - 默认值：`""`
    
    #### 变量：`opt`
    - 描述：参数字符。（例子：一个 `-o` 参数对应一个值为 `'o'` 的 `opt`。）
    - 类型：`char`
    - 默认值：`0`
    
    #### 变量：`longopt`
    - 描述：长参数字符串。（例子：一个 `--help` 参数对应一个值为 `"help'` 的 `longopt`。）（可选）
    - 类型：`std::string`
    - 默认值：`""`
    
    #### 变量：`withParam`
    - 描述：选择是否需要获取该参数的附加内容。（例子：如果需要获取一个类似 `-o example` 的选项，将其设置为 `true`。）（可选）
    - 类型：`bool`
    - 默认值：`false`
    
    #### 变量：`defaultValue`
    - 描述：附加内容的默认值。（可选）
    - 类型：`std::string`
    - 默认值：`""`

### 结构体：`ParamRet`

- 描述：获取一个参数时返回的结构体。
- 成员：

    #### 变量：`id`
    - 描述：一个参数的标识符。
    - 类型：`std::string`
    - 默认值：`""`
    
    #### 变量：`exists`
    - 描述：确认该参数是否存在。
    - 类型：`bool`
    - 默认值：`false`
    
    #### 变量：`value`
    - 描述：额外内容的值。
    - 类型：`std::string`
    - 默认值：`""`

### 函数：`GetParams`
- 描述：从命令行获取参数。
- 参数：
    ```cpp
    (
        int argc, // main() 函数的第一个参数
        char *argv[], // main() 函数的第二个参数
        std::vector<ParamOpt> opts // 参数选项结构体列表
    )
    ```
- 类型：`std::vector<ParamRet>`

### 函数：`GetParamfromParams`
- 描述：从参数列表获取特定参数。
- 参数：
    ```cpp
    (
        std::string id, // 参数的标识符
        std::vector<ParamRet> rets // 参数列表
    )
    ```
- 类型：`ParamRet`

## 头文件：`ISocket.h`

- 描述：基于 Windows API 的基本 Socket UDP 函数的实现。

### 类：`ISocket`
- 公开成员：
    #### 构造函数：`ISocket`
    - 参数：
        ```cpp
        (
            Logger logger // Logger to log
        )
        ```
    
    #### Destructor: `~ISocket`
    - 参数：无

    #### 变量：`localIP`
    - 描述：你正在使用极域终端的本地 IP。在调用构造函数后生成。
    - 类型：`std::string`
    - 默认值：`""`

    #### 函数：`GetLocalIPs`
    - 描述：获取你所有的本地 IP。
    - 参数：无
    - 类型：`std::vector<std::string>`

    #### 函数：`getStudentPorts`
    - 描述：使用指定的 IP 获取极域终端的监听端口。
    - 参数：
        ```cpp
        (
            std::string IP = "" // 指定的 IP
        )
        ```
    - 类型：`std::vector<int>`

    #### 函数：`send`
    - 描述：向目标 IP 发送 UDP 数据包。
    - 参数：
        ```cpp
        (
            std::string IP,
            int port,
            std::vector<BYTE> data
        )
        ```
    - 类型：`int`
    - 值：
        +-------+------+
        | 值 | 描述 |
        |-------|------|
        | `0` | 数据包成功发送。 |
        | `1` | 数据表发送失败。在 Logger 中查看详细信息。 |

- 私密成员：
    #### 变量：`wsd`
    - 描述：在调用函数 `WSAStartup` 时使用。
    - 类型：`WSADATA`

    #### 变量：`optval`
    - 描述：在调用函数 `setsockopt` 时使用。
    - 类型：`int`
    - 默认值：`1`

    #### 变量：`client`
    - 描述：Socket 客户端。
    - 类型：`SOCKET`

    #### 变量：`logger`
    - 描述：输出信息的 Logger。
    - 类型：`Logger`

## 头文件：`JiYu_Attack.h`

- 描述：Jiyu-UDP-Attack-Cpp 的主要类。

### 类：`JiYu_Attack`

- 公开成员：

    #### 构造函数：`JiYu_Attack`
    - 参数：无

    #### 数组：`cmdCodePrefix[4]`

    - 描述：指令的 UDP 数据表前缀。
    - 类型：`static const std::vector<int>`
    - 值：值太长，不便展示。

    #### Enum 枚举类型：`cmdType`

    - 描述：`cmdCodePrefix` 中的指令对应的下标
    - 值：
        ```cpp
        {
            MSG = 0,
            CMD,
            REBOOT,
            SHUTDOWN
        }
        ```

    #### 常量：`nc_ps_url`
    - 描述：下载 Powercat Script 的 URL。
    - 类型：`static std::string`
    - 值：`"https://pastebin.com/raw/u7zARPaN"`

    #### 变量：`client`
    - 描述：Socket 客户端。
    - 类型：`ISocket`

    #### 函数：`IPParser`
    - 描述：用于将 IP 解析为完整 IP 列表的函数。
    - 参数：
        ```cpp
        (
            std::string rawIP // 原始 IP
        )
        ```
    - 类型：`std::vector<std::string>`

    #### 函数：`sendCmd`
    - 描述：向目标 IP 发送 CMD 指令。
    - 参数：
        ```cpp
        (
            std::string rawIP,
            int port,
            std::string cmd // 要发送的 CMD 指令
        )
        ```
    - 类型：`int`

    #### 函数：`sendMsg`
    - 描述：向目标 IP 发送消息。
    - 参数：
        ```cpp
        (
            std::string rawIP,
            int port,
            std::string msg // 要发送的消息
        )
        ```
    - 类型：`int`

    #### 函数：`sendShutdown`
    - 描述：向目标 IP 发送关机指令。
    - 参数：
        ```cpp
        (
            std::string rawIP,
            int port
        )
        ```
    - 类型：`int`

    #### 函数：`sendReboot`
    - 描述：向目标 IP 发送重启指令。
    - 参数：
        ```cpp
        (
            std::string rawIP,
            int port
        )
        ```
    - 类型：`int`

    #### 函数：`netcat`
    - 描述：激活并连接到目标 IP 的 Netcat。目标 IP 应为某个指定的独立 IP。
    - 参数：
        ```cpp
        (
            std::string IP,
            int port,
            int ncport // Netcat 监听的端口
        )
        ```
    - 类型：`int`

    #### 函数：`breakScreenControl`
    - 描述：脱离屏幕控制。
    - 参数：无
    - 类型：`int`

    #### 函数：`continueScreenControl`
    - 描述：恢复屏幕控制。
    - 参数：无
    - 类型：`int`

- 私密成员：
    
    #### 变量：`logger`
    
    - 描述：用于输出信息的 Logger。
    - 类型：`Logger`

### 函数：`netcat_remote`
- 描述：用于在远程机器上激活 Netcat 的函数。
- 参数：
    ```cpp
    (
        LPVOID lpParameter
    )
    ```
- 类型：`DWORD WINAPI`

## 头文件：`JiYuAtk_lib.h`

- 描述：`JiYu_Attack` 头文件的支持库。

### 函数：`execCmd`

- 描述：运行 CMD 命令并将返回内容存储至 `std::string`，而不在控制台上显示。
- 参数：
    ```cpp
    (
        std::string cmd, // 要执行的 CMD 命令
        Logger logger // 绑定的 Logger
    )
    ```
- 类型：`std::string`

### 函数：`strToInt`

- 描述：将 `std::string` 转换为 `int`。
- 参数：
    ```cpp
    (
        std::string str
    )
    ```
- 类型：`int`

## 头文件：`Logger.h`
- 描述：以更简单明了的方式输出信息的 Logger。

### 类：`Logger`

- 公开成员：
    #### 构造函数：`Logger`
    - 参数：
    ```cpp
    (
        FILE* fp // 指向输出文件的指针
    )
    ```

    #### Destructor: `~Logger`
    - 参数：无

    #### Enum 枚举类型：`LogLevel`
    - 描述：Logger 的不同输出等级
    - 值：
    ```cpp
    {
        DEBUG = 1,
        INFO,
        WARNING,
        IERROR,
        CRITICAL,
        NONE
    }
    ```

    #### 函数：`getTime`
    - 描述：在 `std::string` 中以 `HH:MM:SS` 形式返回当前时间
    - 参数：无
    - 类型：`static std::string`

    #### 函数：`setLevel`
    - 描述：设置输出的最小等级。
    - 参数：
        ```cpp
        (
            int level // 最小等级
        )
        ```
    - 类型：`void`

    #### 函数：`getLevel`
    - 描述：获取当前输出的最小等级。
    - 参数：无
    - 类型：`int`

    #### 函数：`log`
    - 描述：输出信息
    - 参数：
        ```cpp
        (
            int level, // 输出等级
            std::string content, // 类似 printf 格式的输出内容
            Args... args // 类似 printf 格式的额外参数
        )
        ```
    - 类型：`template<typename... Args> void`

- 私密成员：
    #### 变量：`fp`
    - 描述：用于输出信息的文件指针。
    - 类型：`FILE*`

    #### 变量：`curLevel`
    - 描述：当前的最小输出等级。
    - 类型：`int`
    - 默认值：`WARNING`