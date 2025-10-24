# Jiyu-UDP-Attack-Cpp Header Docs

## Header: `GetParams.h`

- Desc: To get parameters from console call.

### Struct: `ParamOpt`

- Desc: Struct of options to get a parameter.
- Members:

    #### Variable: `id`
    - Desc: Identifier of a parameter.
    - Type: `std::string`
    
    #### Variable: `opt`
    - Desc: Option character. (eg. Option `-o` corresponds `'o'` in `opt`.)
    - Type: `char`
    
    #### Variable: `longopt`
    - Desc: Long option string. (eg. Option '--help' corresponds `"help"` in `longopt`.)
    - Type: `std::string`
    
    #### Variable: `withParam`
    - Desc: Determine whether an extra parameter is needed. (eg. Set it to `true` if an option like `-o example` is used.)
    - Type: `bool`
    
    #### Variable: `defaultValue`
    - Desc: Default value of the extra parameter.
    - Type: `std::string`

### Struct: `ParamRet`

- Desc: Returned value of getting a parameter.
- Members:

    #### Variable: `id`
    - Desc: Identifier of a parameter.
    - Type: `std::string`
    - Default value: `""`
    
    #### Variable: `exists`
    - Desc: Determine whether the parameter does exist.
    - Type: `bool`
    - Default value: `false`
    
    #### Variable: `value`
    - Desc: Value of the extra parameter.
    - Type: `std::string`
    - Default value: `""`

### Function: `GetParams`
- Desc: Get parameters from command line.
- Parameters:
    ```cpp
    (
        int argc, // The first argument of main()
        char *argv[], // The second argument of main()
        std::vector<ParamOpt> opts // Parameter options' list
    )
    ```
- Type: `std::vector<ParamRet>`

### Function: `GetParamfromParams`
- Desc: Get the specific parameter from parameters.
- Parameters:
    ```cpp
    (
        std::string id, // Identifier of the parameter.
        std::vector<ParamRet> rets // Parameters
    )
    ```
- Type: `ParamRet`

## Header: `ISocket.h`

- Desc: Implementation of basic socket UDP functions, using Windows API.

### Struct: `StudentPort`

- Desc: Struct of Student Terminal's port information.
- Members:

    #### Variable: `pid`
    - Desc: Student Terminal's pid.
    - Type: `int`
    
    #### Variable: `ip`
    - Desc: The IP address that is used by Student Terminal to listen.
    - Type: `std::string`
    
    #### Variable: `port`
    - Desc: The port that is used by Student Terminal to listen.
    - Type: `int`

### Class: `ISocket`
- Public:
    #### Constructor: `ISocket`
    - Parameters:
        ```cpp
        (
            Logger* logger // Logger to log
        )
        ```
    
    #### Destructor: `~ISocket`
    - Parameters: None

    #### Variable: `localIP`
    - Desc: Your local IP which runs Student Terminal. Generated after calling constructor.
    - Type: `std::string`
    - Default value: `""`

    #### Function: `GetLocalIPs`
    - Desc: Get all your local IPs.
    - Parameters: None
    - Type: `std::vector<std::string>`

    #### Function: `getStudentPorts`
    - Desc: To find ports information which Student Terminal listens to.
    - Parameters: None
    - Type: `std::vector<StudentPort>`

    #### Function: `send`
    - Desc: Send UDP message to the target IP.
    - Parameters:
        ```cpp
        (
            std::string IP,
            int port,
            std::vector<BYTE> data
        )
        ```
    - Type: `int`
    - Value: 
        | Value | Desc |
        |-------|------|
        | `0` | Message was sent successfully. |
        | `1` | Failed to send message. Refer to the logger for more information. |

- Private:
    #### Variable: `wsd`
    - Desc: Used while `WSAStartup`.
    - Type: `WSADATA`

    #### Variable: `optval`
    - Desc: Used while `setsockopt`.
    - Type: `int`
    - Default value: `1`

    #### Variable: `client`
    - Desc: Socket client.
    - Type: `SOCKET`

    #### Variable: `logger`
    - Desc: Logger to log information.
    - Type: `Logger*`

## Header: `JiYu_Attack.h`

- Desc: Main class for Jiyu-UDP-Attack-Cpp.

### Class: `JiYu_Attack`

- Public:

    #### Constructor: `JiYu_Attack`
    - Parameters: None

    #### Array: `cmdCodePrefix[4]`

    - Desc: Prefix of UDP Codes of commands.
    - Type: `static const std::vector<BYTE>`
    - Value: Too long to display.

    #### Array: `cmdContentBegin[2]`

    - Desc: Begin indices of filling prefix of UDP Codes of commands.
    - Type: `static const int`
    - Value: `{56, 578}`

    #### Enum: `cmdType`

    - Desc: Indexes of command types in `cmdCodePrefix`.
    - Value:
        ```cpp
        {
            MSG = 0,
            CMD,
            REBOOT,
            SHUTDOWN
        }
        ```

    #### Constant: `nc_ps_url`
    - Desc: URL to download powercat script.
    - Type: `static std::string`
    - Value: `"https://pastebin.com/raw/u7zARPaN"`

    #### Variable: `client`
    - Desc: Socket client.
    - Type: `ISocket*`

    #### Function: `IPParser`
    - Desc: Function to parse raw IP into complete IP list.
    - Parameters:
        ```cpp
        (
            std::string rawIP // The raw IP
        )
        ```
    - Type: `std::vector<std::string>`

    #### Function: `sendCmd`
    - Desc: Send command to the target IP.
    - Parameters:
        ```cpp
        (
            std::string rawIP,
            int port,
            std::string cmd // The command to send
        )
        ```
    - Type: `int`
    - Value: 
        | Value | Desc |
        |-------|------|
        | `0` | Command was sent successfully. |
        | `1` | Failed to send command. The function `sendto` in the class `ISocket` failed. |
        | `4` | Illegal IP address. |

    #### Function: `sendMsg`
    - Desc: Send message to the target IP.
    - Parameters:
        ```cpp
        (
            std::string rawIP,
            int port,
            std::string msg // The message to send
        )
        ```
    - Type: `int`
    - Value: 
        | Value | Desc |
        |-------|------|
        | `0` | Message was sent successfully. |
        | `1` | Failed to send message. The function `sendto` in the class `ISocket` failed. |
        | `4` | Illegal IP address. |

    #### Function: `sendShutdown`
    - Desc: Send shutdown to the target IP.
    - Parameters:
        ```cpp
        (
            std::string rawIP,
            int port
        )
        ```
    - Type: `int`
    - Value: 
        | Value | Desc |
        |-------|------|
        | `0` | Command was sent successfully. |
        | `1` | Failed to send command. The function `sendto` in the class `ISocket` failed. |
        | `4` | Illegal IP address. |

    #### Function: `sendReboot`
    - Desc: Send reboot to the target IP.
    - Parameters:
        ```cpp
        (
            std::string rawIP,
            int port
        )
        ```
    - Type: `int`
    - Value: 
        | Value | Desc |
        |-------|------|
        | `0` | Command was sent successfully. |
        | `1` | Failed to send command. The function `sendto` in the class `ISocket` failed. |
        | `4` | Illegal IP address. |

    #### Function: `netcat`
    - Desc: Enable netcat and connect to the target IP. The IP should **not** be a segment.
    - Parameters:
        ```cpp
        (
            std::string IP,
            int port,
            int ncport // The port which netcat listens to
        )
        ```
    - Type: `int`
    - Value: 
        | Value | Desc |
        |-------|------|
        | `0` | Netcat started. |
        | `4` | Illegal IP address. |

    #### Function: `breakScreenControl`
    - Desc: Get out of screen control.
    - Parameters: None
    - Type: `int`
    - Value: 
        | Value | Desc |
        |-------|------|
        | `0` | Command started. |
        | `3` | Access denied. |

    #### Function: `continueScreenControl`
    - Desc: Recover screen control.
    - Parameters: None
    - Type: `int`
    - Value: 
        | Value | Desc |
        |-------|------|
        | `0` | Command started. |
        | `3` | Access denied. |

- Private:
    
    #### Variable: `logger`
    
    - Desc: Logger to log information.
    - Type: `Logger*`

### Function: `netcat_remote`
- Desc: Function to activate netcat on the remote machine.
- Parameters:
    ```cpp
    (
        LPVOID lpParameter
    )
    ```
- Type: `DWORD WINAPI`
    - Value: 
        | Value | Desc |
        |-------|------|
        | `0` | Command was sent successfully. |
        | `1` | Failed to send command. The function `sendto` in the class `ISocket` failed. |
        | `4` | Illegal IP address. |

## Header: `JiYuAtk_lib.h`

- Desc: Supporting library for `JiYu_Attack` class.

### Function: `strToInt`

- Desc: Convert `std::string` into `int`.
- Parameters:
    ```cpp
    (
        std::string str
    )
    ```
- Type: `int`

### Function: `formatANSIString`

- Desc: Convert ANSI `std::string` into UTF-8 sequence.
- Parameters:
    ```cpp
    (
        const std::string str
    )
    ```
- Type: `std::vector<BYTE>`

### Function: `getProcessIdByName`

- Desc: Get the process identifiers by its name.
- Parameters:
    ```cpp
    (
        const std::string& processName // The name of the process
    )
    ```
- Type: `std::vector<DWORD>`

### Function: `IPDwordToString`

- Desc: Convert the IP address in `DWORD` format into `std::string`.
- Parameters:
    ```cpp
    (
        DWORD ip
    )
    ```
- Type: `std::string`

## Header: `Logger.h`
- Desc: Logger to log information in an easier and clearer way.

### Class: `Logger`

- Public:
    #### Constructor: `Logger`
    - Parameters:
    ```cpp
    (
        FILE* fp, // File pointer to log in
        int level = Warning
    )
    ```

    #### Destructor: `~Logger`
    - Parameters: None

    #### Enum: `LogLevel`
    - Desc: Different log levels in the logger.
    - value:
    ```cpp
    {
        Debug = 1,
        Info,
        Warning,
        Error,
        Critical,
        None
    }
    ```

    #### Function: `getTime`
    - Desc: Get current time in the string in the format `HH:MM:SS`.
    - Parameters: None
    - Type: `static std::string`

    #### Function: `setLevel`
    - Desc: Set minimum level to log.
    - Parameters:
        ```cpp
        (
            int level // The minimum level
        )
        ```
    - Type: `void`

    #### Function: `getLevel`
    - Desc: Get current minimum log level.
    - Parameters: None
    - Type: `int`

    #### Function: `log`
    - Desc: Log information.
    - Parameters:
        ```cpp
        (
            int level, // Log level
            std::string content, // Log content, using printf-like format
            Args... args // Parameters in printf-like format
        )
        ```
    - Type: `template<typename... Args> void`

- Private:
    #### Variable: `fp`
    - Desc: File pointer of the logger to log in.
    - Type: `FILE*`

    #### Variable: `curLevel`
    - Desc: Current minimum log level of the logger.
    - Type: `int`
    - Default value: `Warning`