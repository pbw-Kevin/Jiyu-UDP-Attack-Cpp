# JiYu-UDP-Attack-Cpp Header Docs

## Header: `GetParams.h`

- Desc: To get parameters from console call.

## Header: `ISocket.h`

- Desc: Implementation of basic socket functions, using Windows API.

## Header: `JiYu_Attack.h`

- Desc: Main class for JiYu_Attack.

### Class: `JiYu_Attack`

- Public:

    #### Array: `cmdCodePrefix[4]`

    - Type: `static const std::vector<int>`
    - Value: Too long to display.
    - Desc: Prefix of UDP Codes of commands.

    #### Enum: `cmdType`

    - Value:
        ```cpp
        {
            MSG = 0,
            CMD,
            REBOOT,
            SHUTDOWN
        }
        ```
- Private
    
    #### Variable: `logger`
    
    - Type: `Logger`
    - Desc: Logger to log information.

## Header: `JiYuAtk_lib.h`

- Desc: Supporting library for `JiYu_Attack` class.

### Function: `execCmd`

- Parameters:

    ```cpp
    (
        std::string cmd, // Command to execute
        Logger logger // Logger binded
    )
    ```

- Value: `std::string`
- Desc: Run command without echo and get returned content in `std::string`.