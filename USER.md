# 用户文档

* [类定义](#类定义)
  * [`Vector`模板类](#Vector模板类)
  * [`Particle_Base`类](#Particle_Base类)
  * [`Particle`类](#Particle类)
  * [`Particle_Printer`类](#Particle_Printer类)
  * [`System`类](#System类)
* [接口函数定义](#接口函数定义)
  * [`C++`接口函数](#C接口函数)
  * [`C`接口函数（兼容`python`）](#C接口函数兼容python)

## 类定义

### `Vector`模板类

非常简化的向量模板类。这里的向量，指的是数学和物理上的向量，即一维张量。其功能类似`numpy`中的`array`，但目前仅提供线性运算支持。与`std::vector`本质上不同，模板定义、数据结构、初始化和拷贝控制方式类似`std::array`。

`Vector`模板类定义在头文件`Vector.h`中。

#### 声明

    template<class T, size_t n> struct Vector;

#### 初始化和拷贝控制

    (implicitly declared default/copy/move constructor)
    (implicitly declared copy/move assignment operator)
    (implicitly declared destructor)

同`std::array`，遵循[聚合类(`aggregate`)隐式定义的初始化规则](https://en.cppreference.com/w/cpp/language/aggregate_initialization)，在`C++11`及以后标准下支持直接/拷贝列表初始化。需要注意默认初始化可能带来随机初始值问题。

##### 如何方便地初始化和为`Vector`对象赋值

构造默认初始化的`Vector`对象（包含垃圾值）：

    Vector<double, 3> v;

使用初始化列表对类型实例进行值初始化（避免垃圾值）：

    Vector<double, 3> v1{ };
    Vector<double, 3> v2 = { };
    Vector<double, 3> v3{0.0};
    Vector<double, 3> v4 = {1.0, 2.0};

使用既有`Vector`对象对新对象进行拷贝初始化：

    auto v5(v4);
    auto v6{v5};
    auto v7 = v6;
    auto v8 = {v7};

使用列表初始化器对`Vector`对象进行拷贝赋值：

    v = { };
    v1 = {3.0, 4.0};
    v2 = {5.0, 6.0, 7.0};

#### 数据成员

    T data[n];

公开数据成员，可以直接访问。

#### 成员访问

    T &operator[](size_t i);
    const T &operator[](size_t i) const;

仅支持下标运算符，与公开数据成员data的下标运算等价。

#### 复合赋值运算（成员函数）

    Vector &operator+=(const Vector &);
    Vector &operator-=(const Vector &);
    Vector &operator*=(T);
    Vector &operator/=(T);

向量的加法和数乘运算，运算结果直接赋值到第一个操作数上，退回第一个操作数的引用。

#### 线性运算（非成员函数）

    template<class T, size_t n>
    Vector<T, n> operator+(Vector<T, n>, const Vector<T, n> &);

    template<class T, size_t n>
    Vector<T, n> operator-(Vector<T, n>, const Vector<T, n> &);

    template<class T, size_t n>
    Vector<T, n> operator*(Vector<T, n>, T);
    template<class T, size_t n>
    Vector<T, n> operator*(T, Vector<T, n>);

    template<class T, size_t n>
    Vector<T, n> operator/(Vector<T, n>, T);

线性运算的类外函数版本，退回运算结果而不改变操作数。

#### 输入输出

    template<class T, size_t n>
    std::istream &operator>>(std::istream &, Vector<T, n> &);
    template<class T, size_t n>
    std::ostream &operator<<(std::ostream &, const Vector<T, n> &);

输入算符函数实现为连续`n`次调用数据类型`T`定义的输入算符函数从流中取得数据，其行为与流的状态（是否有效、是否跳过空白字符等等）有关，其行为的良定义由用户保证。用户有义务检查流状态的正确性。另外，**程序不保证退回无效状态的输入流时被作用的`Vector`对象状态不发生变化**。

输出算符函数实现为以单个空格分隔对成员`data`中的每个元素调用输出算符函数向流中写入数据，其行为同样依赖用户传入的流的状态。特别地，当元素个数（对应第二个模板参数`n`）为`1`时，没有分隔符输出；当元素个数为`0`时，直接退回流引用而不进行任何输出操作（我们将其实现为模板函数的重载）。

### `Particle_Base`类

粒子的基类，定义在头文件`Particle.h`中。用于支持直接从输入流中读取`pythia8`的日志信息。每个`Particle_Base`类完整而原始地包含了`pythia8`日志提供的一个粒子的信息。

#### 初始化和拷贝控制

    (implicitly declared default/copy/move constructor)
    (implicitly declared copy/move assignment operator)
    virtual ~Particle_Base() { }

除留空的虚析构函数外（后续可能支持多态特性），使用编译器合成的拷贝控制成员。

一般来说，用户不需要直接构造和使用`Particle_Base`类，即使构造也一般不应提供初始值，而是使用程序为`Particle_Base`类定义的输入算符为对象赋值。

#### 数据成员

    size_t no;

粒子的编号。对于`pythia8`的日志文件而言，粒子的编号从`0`（`system`）开始，顺序排列。

    int id;

粒子的身份码。由`pythia8`定义。不同种类的粒子对应不同的身份码。

    std::string name;

粒子的名称。不同种类的粒子对应不同的名称。已消灭的粒子名称外包围一对圆括号。

    int status;

粒子的状态值。由`pythia8`定义。已消灭的粒子状态值为负数。

    size_t mothers[2];

从日志文件取得的粒子`mothers`信息，有固定长度`2`。可能含有重复或缺失，也可能含有无意义的`0`。

    size_t daughters[2];

从日志文件获得的粒子`daughters`信息，有固定长度`2`。同样该信息不是完善的。

    short colours[2];

粒子的色荷信息。将原始数据（三位阿拉伯数字）读入时可能出现进制问题，有待进一步改进。目前未使用该信息。（注意以`0`开头的数字可能被认为是八进制）

    Vector<double, 3> p;

粒子的动量信息，存储为直角坐标系下的三个分量。其单位一般为GeV/c。

    double e;

粒子的能量信息。其单位一般为GeV。

    double m;

粒子的静质量信息。其单位一般为GeV/c^2。

#### 函数成员 

    Vector<double, 3> getv() const { return p / e; }

注意在相对论情形下，我们没有使用动量除以静质量计算速度。

根据狭义相对论关系有：

<img src="https://latex.codecogs.com/gif.latex?\vec{p}=\frac{m\vec{v}}{\sqrt{1-(v/c)^2}},\%20E=\frac{mc^2}{\sqrt{1-(v/c)^2}}"/>

则：

<img src="https://latex.codecogs.com/gif.latex?\vec{v}/c=\frac{\vec{p}/({\rm%20GeV}/c)}{E/{\rm%20GeV}}"/>

可见在高能物理常用的单位制下使用动量除以能量计算粒子速度是比较合理的。

#### 输入输出

    std::istream &operator>>(std::istream &, Particle_Base &);

输入操作符函数从指定的`pythia8`日志流中顺序读取粒子信息，用户需要确保输入信息和顺序与程序定义匹配。一般而言，每次输入操作应对应`pythia8`日志文件中粒子信息的一行，这一行必须包含完整有效的一个粒子的信息，不能是标题行、总计行或是其他无关内容。该行应以一个粒子的`no`开头，同一个粒子的`m`结束，以至少一个空白字符分隔，共包含 `15`段连续的信息。例如：

    0 90 (system) -11 0 0 0 0 0 0 0.000 0.000 0.000 13000.000 13000.000

由于`Particle_Base`类仅存储从`pythia8`日志流中读取的原始信息，为其定义输出操作没有意义。

### `Particle`类

粒子类，`Particle_Base`的派生类，用于存储程序根据`Particle_Base`基类中提供的信息进行计算仿真所获得的结果。该类定义在头文件`Particle.h`中，部分成员函数实现于`Particle.cpp`中。

#### 定义

    struct Particle : public Particle_Base {
      // ...
    };

#### 相关宏定义

    #define PHASE_UNDEF ((size_t)-1)
    #define PHASE_MIN   ((size_t) 0)

注意`(size_t) 0`为`size_t`类型元素的最小值，`(size_t)-1`为`size_t`类型元素的最大值，这对一些函数的代码实现很有帮助。

#### 初始化和拷贝控制

    (implicitly declared default/copy/move constructor)
    (implicitly declared copy/move assignment operator)
    (implicitly declared virtual destructor)

没有显式定义的拷贝控制成员。用户一般也不需要使用包含数据成员初始值的初始化器初始化`Particle`对象，同样我们为`Particle`类定义了输入算符函数。

#### 数据成员

所有的数据成员都是公有的。

    std::set<size_t> mothers;
    std::set<size_t> daughters;

粒子的母亲和女儿们。程序使用`>>`算符读入`Particle`类时将根据基类`Particle_Base`中同名元素的值为`mothers`和`daughters`赋值，后者可能与前者显著不同。注意到，基类和派生类中它们被声明为不同的类型，这是因为每个粒子真实的母亲和女儿数量都不尽相同，而我们希望派生类中的成员反映真实的情况，这有别于`Particle_Base`中同名元素的结果（它们来自`pythia8`的日志文件，在日志文件中每个粒子的`mothers`和`daughters`有且仅有两列，其信息分开来看都是不完整的，且可能具有`0`占位填充）。

后文将提到用户可以使用`System`类控制程序所有的行为，其中定义了类型成员`System::Subsystem`，为`std::vector<Particle>`的类型别名。程序对每个`Subsystem`实例中的`Particle`有以下要求：

1. 任何一对母女关系都同时存在于母粒子的`Particle::daughters`和女粒子的`Particle::mothers`中
2. `Particle::daughters`中不含有`0`
3. 编号为`0`的粒子（其`name`为`system`）没有母亲
4. 如果`Particle_Base::mothers`中仅含有`0`元素，则`Particle::mothers`中仅含有`0`
5. 如果`Particle_Base::mothers`中含有非`0`元素，则`Particle::mothers`中不含有`0`

根据以上规则可以唯一地由`Particle_Base`中的`mothers`和`daughters`得到`Particle`中的同名元素。

我们约定规则2、3、4、5由`Particle`类的`>>`算符保证，而规则1由`System`类的行为保证。则经`System`类处理之前，每个`Particle`实例中的`mothers`和`daughters`信息也可能是不完整的。

    Vector<double, 3> r;

粒子的坐标信息，由`>>`算符赋`0`值，由`System`类进行后续操作。

    Vector<double, 3> v;

粒子的速度信息，由`>>`算符赋值为基类成员函数`Particle_Base::getv()`的运算结果。原则上程序的其他部分不应改动该值。

    size_t phase;

粒子出现的时段数(phase)，由`>>`算符赋值为`PHASE_UNDEF`。

    size_t death;

粒子死亡的时段数(phase)，由`>>`算符赋值为`PHASE_UNDEF`。

#### 输入输出

    std::istream &operator>>(std::istream &, Particle &);

派生类`Particle`的输入函数首先调用基类`Particle_Base`的输入函数读入信息，检查流状态有效后根据上面的规则为其所有成员赋值。同样地，**当用户接收到无效状态的输入流退回时，应当放弃使用被作用对象中被赋予的无意义的值**。

下面我们将定义`Particle`类的输出操作。

### `Particle_Printer`类

用于向输出流打印`Particle`对象中的信息，定义在头文件`Particle.h`中，部分成员函数实现于`Particle.cpp`中。

#### 初始化和拷贝控制

    Particle_Printer(std::ostream &os,
        size_t no_width, size_t name_width, size_t e_width, size_t phase_width);
    (implicitly deleted default constructor)
    (implicitly declared copy/move constructor)
    (implicitly deleted copy/move assignment operator)
    (implicitly declared destructor)

`Particle_Printer`类的直接构造函数的第一个形参为待绑定输出流的引用，该引用将绑定到`Particle_Printer`对象的同名私有成员上。为了将输出数据对齐，我们还需要向`Particle_Printer`类的直接构造函数传入`4`个代表宽度的`size_t`实参，它们的值将被该构造函数赋给被构造对象的同名私有成员。

与`C++`标准库中的输出流一样，`Particle_Printer`类不可进行拷贝/移动赋值，稍有区别的是，`Particle_Printer`类允许拷贝/移动构造对象，这是因为我们拷贝对象时并没有拷贝输出流，而是使新对象的输出流引用与被拷贝对象的输出流引用绑定到了相同的地址上。不太准确地说，就是拷贝了“流引用”。

#### 数据成员

    std::ostream &os;
    size_t no_width, name_width, e_width, phase_width;

均为私有成员，其值或绑定地址在对象创建时确定，之后不可更改。

#### 特殊控制变量

    constexpr struct { } hdrp = { };
    constexpr struct { } endp = { };

与`std::endl`类似，使用`<<`作用`Particle_Printer`对象和`endp`，将向`Particle_Printer`对象绑定的输出流中发送一个`endl`控制符并退回被作用`Particle_Printer`对象的引用。

同样地，控制变量`hdrp`对`Particle_Printer`对象的作用定义为向输出流中写入粒子列表的头信息，退回`Particle_Printer`对象的引用以待用户继续写入粒子信息（不会自动换行）。

使用示例：

    using std::endl;
    extern Particle p;
    extern Particle_Printer pout;
    pout << hdrp << endp;
    pout << p << endp << "Wow!~~" << endl;

这两个控制变量为（两个不同的）匿名字面值常量类(`constexpr`)的成员，且被定义为字面值常量(`constexpr`)。这样的定义有望避免链接错误。

目前程序没有引入新的命名空间，上述两个特殊变量定义在全局作用域中。

#### 输出操作

    Particle_Printer &operator<<(const decltype(hdrp) &);
    Particle_Printer &operator<<(const decltype(endp) &);

其行为见[特殊控制变量](#特殊控制变量)部分说明。

    Particle_Printer &operator<<(const Particle &);

向绑定的输出流中打印粒子信息。

目前程序以两个空格为分隔，依次打印粒子的编号、名称、`x, y, z`三个方向的位置和速度、粒子的能量、静质量、死亡时段数，共`11`段连续的信息。这些信息对动画的制作是基本而关键的。

### `System`类

控制系统类，用于控制程序面向用户的主要功能。`System`类定义在头文件`System.h`中，大多数功能实现于源文件`System.cpp`中。

#### 类型成员（公有）

    typedef std::vector<Particle> Subsystem;

每个`std::vector<Particle>`对象通常有序地包含`pythia8`一个日志列表中的所有粒子，我们将其定义为程序处理的一个子系统（`Subsystem`）。

    typedef std::vector<std::vector<size_t>> Index;

索引类型定义。这是一个二维索引结构，其第一个下标为粒子出现的时期，称为时段数（`phase`）。对`Index`对象进行下标运算得到一个`std::vector<size_t>`对象的引用，称为子索引。每个子索引无序地包含了一个时段数下所有粒子的编号（`no`）。

#### 初始化和拷贝控制

    (implicitly declared default/copy/move constructor)
    (implicitly declared copy/move assignment operator)
    (implicitly declared destructor)

`System`类无显式定义的拷贝控制成员，但为部分数据成员提供了类内初始值，它们对`System`对象的状态是十分重要的。用户对`System`对象的操作主要通过成员函数和输入输出运算完成。

#### 公有数据成员

    std::string description;

存储来自`pythia8`日志的介绍信息，由输入算符获取。

    Subsystem hard;
    Subsystem complete;

程序需要处理的两个子系统，即硬过程和完整过程，由成员函数控制访问和修改，用户一般无需直接修改。

    size_t prolong = 0;

指定在最大时段数的粒子处理完毕后需要延长的时段数，默认为0，用户可以直接修改。

#### 公有函数成员

    bool build_index();
    const Index &get_hard_index();
    const Index &get_complete_index();

与索引相关的成员函数。

`build_index`为用户手动控制生成索引的函数，强制程序立即生成全部的索引信息。生成成功时退回真值，失败时退回假值。**用户有义务检查函数的退回值，程序负责保证退回假值时`System`对象的状态不发生改变**。

`get_*_index`为用户获取索引的函数，为节省资源，函数退回常量引用。程序负责判断索引是否已经生成，如未生成则先生成索引，生成一次后再次获取无需再次生成。生成失败则抛出`std::runtime_error`异常，**用户有义务编写异常处理相关的代码，程序负责保证抛出异常时`System`对象的状态不发生改变**。

    bool build_information();

用户手动控制生成索引和动画仿真所有信息的函数，目前原则上不会退回假值，即可以确保成功调用。

#### 输入输出

    std::istream &operator>>(std::istream &is, System &s);
    std::ostream &operator<<(std::ostream &os, System &s);

`System`类最重要的两个函数。

对`System`对象进行输入操作时，用户需要确保程序可以从输入流中读取完整的`pythia8`日志信息，且没有其它无关的信息。程序确保输入操作符函数退回时所需信息（目前为`pythia8`日志中的介绍部分、硬散射过程的粒子信息、完整过程的粒子信息）全部收集完毕，但不会一直向流中读取数据直到输入流处于无效状态。并且，函数退回前会对每个`Subsystem`中的信息进行加工，确保满足[`Particle`类数据成员](#数据成员-2)说明中列出的5条约定。

对`System`对象进行输出操作时，程序会自动调用`build_*`函数进行自我处理，该过程可能抛出异常。程序经过建立索引（时段划分）、计算粒子生命周期和每个时段开始时的位置和速度等过程后，将运算结果写入输出流中。每次`System`对象的输出操作将获得四段信息，分别为硬过程每时段新增粒子的信息、硬过程每时段所有存在的粒子的信息，以及软过程相应的两块信息。每串信息首尾都有长串星花（`*`）包围的说明信息分隔，方便用户进行切割处理。

## 接口函数定义

接口函数在头文件`Interface.h`中声明，在源文件`Interface.cpp`中定义。

简单地说，动画仿真的完整接口除去错误检查外只需要两步操作：

1. 创建`System`对象
2. 进行`System`对象的输入和输出运算

### `C++`接口函数

    int simulate(std::istream &is, std::ostream &os, int prolong = 0);
    int simulate(const std::string &infile, const std::string &outfile, int prolong = 0);

用户需要传入3个参数。首先，用户需要使用前两个参数指定程序的输入输出源，它们可以是`C++`输入输出流，也可以是文件名称。紧接着用户需要使用第三个参数（可选，默认为0）指定所有的时段处理完后需要延长的时段。在延长的时段中，所有存在的粒子都做匀速直线运动。当传入的`prolong`为负值时，它被程序处理为`0`。

需要注意的是，程序不检查输出流的状态，当输出流处于无效状态（这也可能由用户指定没有写权限的文件造成）时，用户无法获得程序的任何正常输出。

头文件`Interface.h`使用了条件编译技术，当使用`C`编译器编译且宏`__cplusplus`未定义时`C++`接口函数不可见。

### `C`接口函数（兼容`python`）

    int simulate(const char *infile, const char *outfile);
    int simulate_prolong(const char *infile, const char *outfile, int prolong);

它们的功能与`C++`版本完全相同，只是函数名称和参数类型有所区别。这样的定义保证了程序的兼容性。

在`python3`中，你可以这样调用：

    import ctypes
    simulate_prolong = ctypes.CDLL('libano.so').simulate_prolong
    simulate_prolong(b'input.txt', b'output.txt', 15)

其中`libano.so`包含`simulate_prolong`函数的二进制代码。它可以使用`Makefile`产生。

请注意，我们需要使用字节串（`bytes`）而非字符串（`str`）作为传递给函数的第一和第二个参数。

使用`C++`编译器编译时，只有声明为`extern "C"`的符号才对`CDLL`可见。故我们也使用了条件编译技术在`C++`环境下将`C`接口函数声明为`extern "C"`。
