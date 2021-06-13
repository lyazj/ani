# ano

高能对撞机的动画模拟

详细说明参见：[用户文档](USER.md#用户文档)

## 外部依赖

python3 (with numpy, matplotlib, imageio, particle), a C++ Compiler (e.g. g++)

### on Windows
mingw32-make (TDM-GCC recommended, especially for 32-bit python.exe on 64-bit Win-Platform)

### Linux
make

## 使用方法

【步骤一】安装外部依赖并确保软件目录下所有依赖的可访问性。检验步骤如下：

在Windows上可使用`cmd`切换（`cd /d`）至软件目录下执行`where python g++ mingw32-make`指令。

在Linux上可用`whereis python3 g++ make`。

在任意平台下可使用`pip list`查看安装的python软件包或直接在python交互界面上进行`import`测试。

我们需要保证`numpy`，`matplotlib`，`imageio`，`particle`这几个软件包已正确安装。

【步骤二】（可选）使用你生成的pythia8日至文件替换软件目录下的input.txt。

【步骤三】在软件目录或其中的bin目录下，执行Concept：

在Windows上可在cmd中切换至软件目录执行`python bin/Concept`，或在`bin`目录下执行`python Concept`。

在Linux上可直接执行`bin/Concept`或`./Concept`。

## 功能介绍

Concepy用于生成高能对撞机动画的概念模型。程序将自动调用Makefile中的指令生成库文件libano.so（在Windows上实际为dll，为了方便统一扩展名为.so），而后调用C接口函数simulate根据input.txt中的信息生成粒子运动模拟的数据，存储于output.txt中，最后使用matplotlib对每一时段（phase）进行可视化处理并使用imageio将前一步骤生成的离散的phase_\*.png文件整合为concept.gif。

软件对时段的划分方法是一个基本的递推关系，即规定系统(system, no = 0)的时段为0，所有粒子的时段均为其所有母亲时段的最大值加一。另外软件假设当任意粒子的女儿出现时它就被消灭了，每个时段仅画出并画出所有已经产生且未被消灭的粒子。

在概念模型中，每一个时段仅作出一张png图像，在20fps的帧率下对样本input.txt处理仅得到48个时段共2.4秒的动画。在每个时段中都假设所有粒子是匀速运动的，即忽略了它们之间的相互作用。动画中没有区别地将所有粒子表示为颜色随机、大小相等的圆点，后续将添加更多的特性将它们区别开来。

（未完待续）
