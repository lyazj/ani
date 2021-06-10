# 用户文档

## 类定义

### Vector模板类

非常简化的向量模板类。目前支持线性运算，功能类似`numpy`中的`array`，与`std::vector`不同。模板定义和拷贝控制成员类似`std::array`。

#### 声明

    template<class T, size_t n> class Vector;

#### 拷贝控制

    (implicitly declared default/list/copy/move constructor)
    (implicitly declared copy/move assignment operator)
    (implicitly declared destructor)

同`std::array`，使用聚合类(`aggregate`)隐式定义的拷贝控制成员，需要注意默认初始化问题。

在`C++11`及以后标准下支持列表初始化。

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
    std::istream &operator<<(std::istream &, Vector<T, n> &);

输入输出运算底层调用数据类型`T`定义的输入输出运算符，流状态的改变由后者决定。调用者有义务检查流状态的有效性。
