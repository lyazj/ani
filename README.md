# ano

高能对撞机的动画模拟

源代码详细说明请看：[用户文档](USER.md#用户文档)。

## 外部依赖

* a Linux OS (e.g. Ubuntu)
* python3 with numpy, matplotlib, imageio, particle
* a C++ Compiler (e.g. g++)
* make

## 使用步骤

1. 打开shell（bash或dash），获取项目的完整拷贝：

       $ git clone https://github.com/lyazj/ani.git
       $ cd ani

   开头的美元符号`$`为shell的提示符，你也可能看到井号（`#`）。

2. 安装外部依赖并确保软件目录下所有依赖的可访问性。以Ubuntu为例：

   * 安装python第三方库：

         $ sudo apt-get install python3-pip
         $ pip install numpy matplotlib imageio particle

   * 安装C++编译器和make：（以g++为例）

         $ sudo apt install g++ make

   * 检查是否安装成功：

         $ python3
         >>> import numpy
         >>> import matplotlib
         >>> import imageio
         >>> import particle
         >>> exit()  # or just press Ctrl-D **once**
         $ g++ --version
         $ make --version

     成功的标志为执行第一条指令可进入python3（观察到有版本和版权等信息输出，提示符变为三个大于号），进行各import操作不报错，退回shell执行最后两条指令看到版本信息输出。

3. （可选）使用你生成的pythia8日至文件替换软件目录下的input.txt。

4. 在软件目录或其中的bin目录下，执行Concept，比如我们刚才已经进入了软件目录，则可以执行：

       $ bin/Concept

   或：

       $ pushd bin; ./Concept

## 功能介绍

Concept用于生成高能对撞机动画的概念模型。程序将自动调用Makefile中的指令生成库文件libani.so，而后调用其中的C接口函数simulate根据input.txt中的信息生成粒子运动模拟的数据，存储于output.txt中，最后使用matplotlib对每一时段（phase）进行可视化处理并使用imageio将前一步骤生成的离散的phase_\*.png文件整合为concept.gif。Concept默认的工作目录是软件目录下的run目录，其输入输出都默认在此处进行。

软件对时段的划分方法是一个基本的递推关系，即规定系统(system, no = 0)的时段为0，所有粒子的时段均为其所有母亲时段的最大值加一。另外软件假设当粒子的任意女儿出现时这个粒子就被消灭了，每个时段仅画出并画出所有已经产生且未被消灭的粒子。

在概念模型中，每一个时段仅作出一张png图像，在低帧率下有卡顿感，后续将添加补间效果。我们把每个时段中的所有粒子都处理为匀速直线运动，即在较短时间内忽略了各种相互作用。
