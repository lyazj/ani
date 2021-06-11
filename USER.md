# 用户文档

## 类定义

### `Vector`模板类

非常简化的向量模板类。这里的向量，指的是数学和物理上的向量，即一维张量。其功能类似`numpy`中的`array`，但目前仅提供线性运算支持。与`std::vector`本质上不同，数据结构、初始化和拷贝控制方式类似`std::array`。

`Vector`模板类定义在头文件`Vector.h`中。

#### 声明

    template<class T, size_t n> struct Vector;

#### 初始化和拷贝控制成员

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

输出算符函数实现为以单个空格分隔对成员`data`中的每个元素调用输出算符函数向流中写入数据，其行为同样依赖用户传入的流的状态。特别地，当元素个数（对应第二个模板参数`n`）为1时，没有分隔符输出；当元素个数为0时，直接退回流引用而不进行任何输出操作（我们将其实现为模板函数的重载）。

### `Particle_Base`类

粒子的基类，定义在头文件`Particle.h`中。用于支持直接从输入流中读取`pythia8`的日志信息。每个`Particle_Base`类完整而原始地包含了`pythia8`日志提供的一个粒子的信息。

#### 拷贝控制

    (implicitly declared default/copy/move constructor)
    (implicitly declared copy/move assignment operator)
    virtual ~Particle_Base() { }

除留空的虚析构函数外，使用编译器合成的拷贝控制成员。

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

#### 成员函数

    Vector<double, 3> getv() const { return p / e; }

注意在相对论情形下，我们没有使用动量除以静质量计算速度。

根据狭义相对论关系有：

<img src="https://latex.codecogs.com/gif.latex?\vec%20p%20=%20\frac{m\vec%20v}{\sqrt{1%20-%20(v%20/%20c)^2}},\%20E%20=%20\frac{mc^2}{\sqrt{1%20-%20(v%20/%20c)^2}}"/>

则：

<img src="https://latex.codecogs.com/gif.latex?\vec%20v%20/%20c%20=%20\frac{\vec%20p%20/%20({\rm%20GeV}/c)}{E%20/%20{\rm%20GeV}}"/>

可见在高能物理常用的单位制下使用动量除以能量计算粒子速度是比较合适的。

#### 输入输出

    std::istream &operator>>(std::istream &, Particle_Base &);

输入操作符函数从指定的`pythia8`日志流中顺序读取粒子信息，用户需要确保输入信息和顺序与程序定义匹配。一般而言，每次输入操作应对应`pythia8`日志文件粒子信息的一行，这一行必须包含完整有效的一个粒子的信息，不能是标题行、总计行或是其他无关内容。该行应以一个粒子的`no`开头，同一个粒子的`m`结束，以至少一个空白字符分隔，共包含 `15`段连续的信息。例如：

    0 90 (system) -11 0 0 0 0 0 0 0.000 0.000 0.000 13000.000 13000.000

由于`Particle_Base`类仅存储从`pythia8`日志流中读取的原始信息，为其定义输出操作没有意义。

### `Particle`类

粒子类，`Particle_Base`的子类。用于对`pythia8`日志中提供的粒子信息进行进一步的加工。该类定义在头文件`Particle.h`中，部分成员函数实现于`Particle.cpp`中。

#### 定义

    struct Particle : public Particle_Base {
      // ...
    };

#### 相关宏定义

    #define PHASE_UNDEF ((size_t)-1)
    #define PHASE_MIN   ((size_t) 0)

注意`(size_t) 0`为`size_t`类型元素的最小值，`(size_t)-1`为`size_t`类型元素的最大值，这对后面我们的运算规则十分重要。

#### 数据成员

    std::set<size_t> mothers;
    std::set<size_t> daughters;

粒子的母亲和女儿们。程序使用`>>`算符读入`Particle`类时将根据基类`Particle_Base`中同名元素的值为`mothers`和`daughters`赋值，后者可能与前者显著不同。注意到，二者被声明为不同的类型，这是因为每个粒子真实的母亲和女儿数量都不尽相同，而我们定义的`Particle::mothers`和`Particle::daughters`需要反映真实的情况，这有别于`Particle_Base`中同名元素的结果。因为后者来自`pythia8`的日志文件，在日志文件中每个粒子的`mothers`和`daughters`有且仅有两列，其信息分开来看都是不完整的。

后文将提到程序使用`System`类控制程序所有的行为，其中定义了类型成员`System::Subsystem`，为`std::vector<Particle>`的类型别名。程序对每个`Subsystem`实例中的`Particle`有以下要求：

1. 任何一对母女关系都同时存在于母粒子的`Particle::daughters`和女粒子的`Particle::mothers`中
2. `Particle::daughters`中不含有`0`
3. 编号为`0`的粒子（其`name`为`system`）没有母亲
4. 如果`Particle_Base::mothers`中仅含有`0`元素，则`Particle::mothers`中仅含有`0`
5. 如果`Particle_Base::mothers`中含有非`0`元素，则`Particle::mothers`中不含有`0`

根据以上规则可以唯一地由`Particle_Base`中的`mothers`和`daughters`得到`Particle`中的同名元素。

我们约定规则2、3、4、5由`Particle`类的`>>`算符保证，而规则1由`System`类的行为保证。则经`System`类处理之前，每个`Particle`实例中的`mothers`和`daughters`信息都可能是不完整的。

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
