* [➕ C/C++](#c/c++)
* [☁️ 计算机网络](#internet)
* [💾 数据库](#database)
* [📏 设计模式](#1)
* [&#x1F527; GDB](#2)

<h1 id="c/c++">C/C++</h1>

## 左值&右值

### 什么是左值、右值

不考虑引用以减少干扰：左值可以取地址、位于等号左边；而右值没法取地址，位于等号右边<br>
左右值的概念很清晰，有地址的变量就是左值，没有地址的字面值、临时值就是右值

`int a = 5;`


- a可以通过&取地址，位于等号左边，所以a是左值
- 5位于等号右边，5没法通过&取地址，所以5是个右值

```cpp
struct A {
    A(int a = 0) {
        a_ = a;
    }
    int a_;
};
A a = A();
```

- a可以通过&取地址，位于等号左边，所以a是左值
- A()是个临时值，没法通过&取地址，位于等号右边，所以A()是个右值

### 左值引用

能指向左值，不能指向右值的就是左值引用，左值引用无法指向右值

```cpp
int a = 5;
int &ref_a = a; //左值引用指向左值，编译通过
int &ref_a = 5; //左值引用指向右值，编译失败
```

但是，const左值引用是可以指向右值的：

`const int &ref_a = 5; //编译通过`

const左值引用不会修改指向值，因此可以指向右值，这也是为什么要使用`const &`作为函数参数的原因之一，如`std::vector`的`push_back`：

`void push_back (const value_type& val);`

如果没有const，`vec.push_back(5)`这样的代码就无法编译通过

### 右值引用

右值引用的标志是`&&`，可以指向右值，不能指向左值

```cpp
int &&ref_a_right = 5; //ok
int a = 5;
int &&ref_a_left = a; //编译不过，右值引用不可以指向左值
ref_a_right = 6; //右值引用的用途：可以修改右值
```

### 右值引用指向左值

```cpp
int a = 5; // a是个左值
int &ref_a_left = a; //左值引用指向左值
int &&ref_a_right = std::move(a); //通过move将左值转为右值，可以被右值引用指向
cout << a; //打印结果：5
```

看上去是左值a通过std::move移动到了右值ref_a_right中，那是不是a里边就没有值了？并不是，打印出a的值仍然是5

move是一个非常有迷惑性的函数，往往以为它能把一个变量里的内容移动到另一个变量，但事实上std::move移动不了什么，唯一的功能是把左值强制转化为右值，让右值引用可以指向左值。其实现等同于一个类型转换，所以，单纯的move不会有性能提升

## 函数指针&指针函数

**函数指针**
定义一个指向函数的指针变量，
`int (*p)(int x, int  y);`

**指针函数**
指的是函数的返回值是一个指针，
`int *p(int a,int b);`



<h1 id="database">数据库</h1>

## Redis

- Redis完全基于内存
- 数据结构简单
- 使用多路IO复用模型，非阻塞IO
- 单线程模型，无需考虑加锁放锁操作，不会出现死锁问题

### 通用命令
- KEYS 查询
- DEL 删除
- EXISTS 判断key是否存在
- EXPIRE 给key设置有效期，到期自动删除
- TTL 查询key的有效期

### redis过期策略
key的过期时间用Unix时间戳存储<br>
redis使用：定期删除 + 惰性删除

#### 定时删除
创建key的同时创建一个定时器，当定时器结束，立即删除
- 优点：对内存最友好，key一旦过期就可以立即删除
- 缺点：对CPU最不友好，当过期key比较多，删除操作会占用CPU

#### 定期删除
redis每隔一段时间（可在配置文件中设置），就在设置了过期时间的keys中随机抽查，如果过期了就将其删除
- 优点：可以通过设置频率来减少CPU负载，另外定期删除，也能有效释放过期的key
- 缺点：执行删除操作的频率不可控，太频繁或稀疏都不好

#### 惰性删除
设置key的过期时间后，不再管，当需要该key时，再检查是否过期，如果过期就删除，反之返回该key
- 优点：对CPU友好，只会在用到该键时才会做过期检查，对于用不到的key就不做过期检查
- 缺点：对内存不友好。若一个key已过期但未使用，那么该key一直储存在内存中；量一多，大量的过期key在内存中得不到释放，造成泄露（可见内存淘汰策略）

### 内存淘汰策略

当redis占用内存超过maxmemory时，会触发主动清理策略
- 针对设置了过期时间的key处理
	- volatile-ttl 筛选设置了过期时间的键值对，越早过期的越先被删除
	- volatile-random 筛选设置了过期时间的键值对，随机删除
	- volatile-lru 利用LRU算法筛选设置了过期时间的键值对进行删除
	- volatile-lfu 利用LFU算法筛选设置了过期时间的键值对进行删除
- 针对所有的key做处理
	- allkeys-random 在所有的键值对中进行随机删除
	- allkeys-lru 利用LRU算法对所有数据筛选后删除
	- allkeys-lfu 利用LFU算法对所有数据筛选后删除
- 不处理
	- noevent 不剔除任何数据，拒绝所有写入操作并返回错误信息，此时redis只响应读操作

#### LRU ＆ LFU
- LRU，最近最少使用(least recently used)，以最近一次访问时间作为参考，淘汰很久没被访问过的数据
- LFU，最不经常使用(least frequently used)，以次数作为参考，淘汰最近一段事件被访问次数最少的数据
绝大多数情况使用LRU，但当存在大量热点缓存数据时，LFU更佳

### 多路IO复用模型
- select：利用数组存储文件描述符，容量固定，需要轮询来判断是否发生了IO事件
- poll：链表存储文件描述符，可扩展，也需要轮询
- epoll：红黑树存储，利用事件通知（观察者模式）模型，只有发生了IO事件，应用程序才会进行IO操作

### redis对象类型
redis是key-value类型，支持string, list, hash, set, SortedSet（有序集合）

#### String
根据字符串的格式不同，可分为三类
| 编码类型 | 含义 |
| :----: | :----: |
| string | 普通字符串 |
| int | 整数类型，可自增自减 |
| float | 浮点类型，可自增自减 |

| 底层实现 | 含义 |
| :----: | :----: |
| int | 字符串长度小于等于20且能够转为整数 |
| embstr | 字符串长度小于等于44 |
| raw | 字符串长度大于44 |

为啥是44呢？因为cpu中每一个多级缓存的缓存行 为64字节，其他额外数据占据19，转义字符占1字节

- SET 添加或删除string类型的键值对
- GET 根据key获取值
- MSET 批量添加多个string型的键值对
- MGET 根据多个key获取值
- INCR 让int型数据自增1
- INCRBY 让int型自增指定步长
- INCRBYFLOAT 让float型自增指定步长
- SETNX 添加一个string型的键值对，前提是此key不存在，否则不添加
- SETEX 添加一个string型的键值对，并指定有效期

#### key结构
Redis允许有多个单词形成层级结构，多个单词之间用'：'隔开，比如：<br>
项目名：业务名：类型：id<br>
再将值序列化成json对象存储<br>
```cpp
SET Apple:iphone:A2022_1 '{"id":1, "name":"13pro", "color":"blue"}'
SET Apple:iphone:A2022_2 '{"id":2, "name":"13pro", "color":"white"}'
```

#### List

```
两种底层数据结构：
- quicklist 双向链表
- ziplist 压缩列表
```

- LPUSH key element 向列表左侧插入一个或多个元素
- LPOP key 移除并返回左侧的第一个元素，没有则返回nil
- RPUSH key element 向列表右侧插入一个或多个元素
- RPOP key 移除并返回右侧的第一个元素
- LRANGE key start end 返回一段角标范围内的所有元素
- BLPOP和BRPOP 与LPOP类似，只不过在没有元素时等待指定时间，而不是直接返回nil


#### Hash
值是无序字典（规定字典为 field-value对）

```
实现方式：
dict - 字典（节点数量大于512或字符串长度大于64）
ziplist - 压缩列表（节点数量小于等于512且字符串长度小于等于64）
```

- HSET key field value：添加或修改key的field值
- HGET key field：获取hash类型key的field的值
- HMSET：批量添加多个hash类型key的field的值
- HMGET：批量获取多个hash类型key的field的值
- HGETALL：获取hash类型的key中的所有的field和value
- HKEYS：获取一个hash类型的key中的所有的field
- HVALS：获取一个hash类型的key中的所有的value
- HINCRBY：让一个hash类型key的字段值自增并指定步长
- HSETNX：添加一个hash类型的key的field值，前提是这个field不存在，否则不执行

#### set
类似于一个没有value的无序字典

```
intset - 整数数组（元素都为整数且节点数量小于等于512）
dict - 字典（元素有一个不为整数或者数量大于512）
```

- SADD key member 向set中添加一个或多个元素
- SREM key member 移除set中的指定元素
- SCARD key 返回set中元素的个数
- SISMEMBER key member 判断一个元素是否存在于set中
- SMEMBERS 获取set中的所有元素
- SINTER key1 key2 求key1与key2的交集
- SDIFF key1 key2 求key1与key2的差集
- SUNION key1 key2 求key1和key2的并集

#### SortedSet（weight是权值）

```
skiplist - 跳表（数量大于128或者有一个字符串长度大于64）
ziplist - 压缩列表
```

- ZADD key weight member 添加一个或多个元素到SortedSet，若存在就更新权值
- ZREM key member 单次删除SortedSet中指定元素
- ZSCORE key member 获取SortedSet的指定元素的权值
- ZRANK key member 获取SortedSet的指定元素的排名
- ZCARD key 获取SortedSet中元素个数
- ZCOUNT key left right 统计权值在区间内所有元素的个数
- ZINCRBY key increment member 让SortedSet中的指定元素自增，步长为指定的increment值 
- ZRANGE key num1 num2 排序后获取指定排名范围内的元素（比如第2名-第10名）
- ZRANGEBYSCORE key min max 排序后，获取指定权值范围内的元素

### 相同数量情况下，分析hash和zset占用情况
- 如果节点数据≤128，hash和zset底层实现都是压缩列表
- 如果节点数据∈(128,512)，hash使用压缩列表，zset使用跳表，跳表时间复杂度O(log2n)，压缩列表O(n)，但跳表空间占用更高
- 如果节点数据>512，hash使用字典，zset使用跳表

### 备份如何执行
Rdis会单独创建(fork)一个子进程来进行持久化，先将数据写入到一个临时文件中，等持久化过程都结束了，再用这个临时文件替换上次持久化好的文件

### 持久化
Rdis是内存数据库，如果不将内存中的数据库状态保存到磁盘，那么一旦服务器进程退出，服务器中的数据库状态也会消失。所以Redis提供了持久化功能

#### RDB
redis将快照snapshot保存在dump.rdb文件中

![image](https://github.com/arqady01/cpp-interview/blob/main/database/image/RDB%E6%B5%81%E7%A8%8B.png)

##### RDB触发机制
- 满足config文件中save规则
- 执行flushall命令
- 退出redis

##### RDB优缺点
- 优点
    - dump.rdb是某一时间点上的数据快照，非常适合大规模数据恢复
    - 对数据完整性要求不高，可以复制到任何地方并加载，对灾难恢复、数据迁移很友好
    - 相比AOF性能更高，数据恢复速度更快
- 缺点
    - 需要一定的时间间隔进程操作，如果redis意外宕机，将丢失最近写入但未保存到快照中的那些数据
    - fork进程时，会占用一定的内存空间；而且父进程越多，fork时间也越长
    - 高并发时可能会频繁命中config中的save规则，导致fork操作和备份的频率变得不可控

#### AOF
将执行过的所有写入动作以日志形式记录下来（因为只有写才会对数据库发生改变），只许追加文件但不可以改写文件，redis启动之初会读取appendonly.aof文件重新构建数据

##### AOF优缺点
- 优点
    - 可以选择no， every second， always三种策略
        - always每次修改都会同步，文件完整性更好
        - every second是默认的策略，每秒同步一次，性能更优，但可能会丢失一秒的数据
        - no，从不同步，效率最高
- 缺点
    - aof文件通常比rdb文件更大，修复速度也更慢
    - 因为aof是写入操作，效率比rdb低，所以redis默认是用RDB做持久化

### 主从复制
将一台Redis服务器的数据（主节点master，写为主），复制到其他的Redis服务器（从节点slave，读为主），数据的复制只能由主节点到从节点（单向）

- 数据冗余：主从复制实现了数据的热备份，是持久化之外的一种数据冗余方式
- 故障恢复：当主节点出现问题时，可以由从节点提供服务，实现快速的故障恢复
- 负载均衡：在主从复制的基础上，配合读写分离，可以大大提高Redis服务器的并发量
- 高可用基石：除了上述作用以外，主从复制还是哨兵和集群能够实施的基础，因此主从复制是redis高可用的基础

#### 复制原理
slave连接到master后会发送一个sync同步命令，master启动后台的存盘进程，同时收集所有接收到的用于修改数据集命令，在后台进程执行完毕之后，master将传送整个数据文件到slave，并完成一次完全同步<br>

- 全量复制：salve在接收到数据库文件数据后，将其存盘并加载到内存中。
- 增量复制：master继续将新的所有收集到的修改命令依次传给slave,完成同步

### 哨兵模式
当master宕机后，需要手动把一台slave切换为主服务器，费事费力，还会造成一段时间内服务不可用，所以优先考虑哨兵模式<br>
哨兵作为一个独立的进程，通过哨兵发送命令，等待各redis服务器响应，从而监控多个redis实例

![image](https://github.com/arqady01/cpp-interview/blob/main/database/image/%E5%93%A8%E5%85%B5%E6%A8%A1%E5%BC%8F.png)

### 缓存雪崩
**大量缓存key**同一时间失效，导致请求打到数据库，造成后者宕机<br>
<br>举例<br>
双十一期间，所有用户打开淘宝就是进入首页，首页压力巨大，为提高并发，将网站首页数据都缓存到redis里，所有的key失效时间都是2小时，当天大量用户购物，结果2个小时过去了，redis中key缓存全部失效，此时redis中查询不到数据，只能去数据库中查询，造成数据库无法响应宕机

#### 解决方案
- 随机设置key失效时间，避免大量key集体失效
- 集群部署中，可将热点数据均匀分布在不同的redis库中也能预防key全部失效
- 定时器，在缓存失效前写入新的缓存

### 缓存穿透
redis缓存和数据库中**没有相关数据**（比如id < 0 的请求），redis中没有这样的数据，无法拦截，直接穿透到数据库

#### 解决方案
- 将不存在的数据缓存到redis中，设置键值对为null，并设置一个短期过期时间，避免过期时间过长影响正常用户使用
- 拉黑该P地址
- 对参数进行校验，拦截不合法参数
- 布隆过滤器 将所有可能存在的数据哈希映射到一个足够大的bitmap（位图）中，一定不存在的数据会被bitmap拦截掉，从而避免了对底层存储系统的查询压力

##### 布隆过滤器
布隆过滤器是一个很长的二进制向量和一系列hash映射函数。布隆过滤器可以用于检索一个元素是否在一个集合中，存在为1，不存在为0
- 优点
    - 可以高效查询，即某样东西一定不存在 或者 可能存在
    - 可以高效的进行插入
    - 相比于传统的List、Set、Map等数据结构，它占用空间更少，因为其本身并不存储任何数据（重点）
- 缺点
    - 返回的结果是存在误差的，但将误差率设置为无穷小影响性能
    - 一般不提供删除操作

### 缓存击穿
**某一个热点**key，不停的扛着高并发，当这个key在失效的一瞬间，持续的高并发就击破缓存打到数据库，导致宕机<br>
<br>举例<br>
阿里拍卖一块宝珀手表，设置有效期3小时，想着够拍卖了吧，谁知到2小时59分，还有很多人加价，突然该key过期，高并发请求直接到数据库，导致宕机

#### 解决方案
- 增大热点key的有效期
- 互斥锁，在第一个请求上用mutex锁住，其他请求拿不到锁就阻塞，等第一个请求完毕，再将数据缓存到redis，后面的线程解除阻塞发现有了缓存，直接走缓存

### redis缓存与数据库双写一致性

先更新数据库，再更新缓存（有缺陷）

![image](https://github.com/arqady01/cpp-interview/blob/main/database/image/%E5%85%88%E6%9B%B4%E6%96%B0%E6%95%B0%E6%8D%AE%E5%BA%93%E5%86%8D%E6%9B%B4%E6%96%B0%E7%BC%93%E5%AD%98.png)

先删除缓存，再更新数据库（有缺陷）

![image](https://github.com/arqady01/cpp-interview/blob/main/database/image/%E5%85%88%E5%88%A0%E7%BC%93%E5%AD%98%E5%86%8D%E6%9B%B4%E6%96%B0%E6%95%B0%E6%8D%AE%E5%BA%93.png)

先更新缓存，再更新数据库（有缺陷）<br>

**先更新数据库，再删除缓存**（合适的方案）


<h1 id="1">设计模式</h1>

## 单例模式

<h3>饿汉式</h3>
饿汉式不存在线程安全问题

```cpp
#include <iostream>
class Singoton {
public:
	static Singoton* getInstance() {
		return ptr;
	} //类中的静态函数只能访问类中静态成员变量
	void show() {
		std::cout << "hungry\n";
	}
	Singoton(const Singoton& s) = delete;
	Singoton& operator=(const Singoton& s) = delete;
private:
	Singoton() {};
	static Singoton* ptr; //声明
};
Singoton* Singoton::ptr = new Singoton; //定义

int main() { //测试
	Singoton* op = Singoton::getInstance();
	op->show();
}
```

<h3>懒汉式</h3>
懒汉式存在线程安全问题

```cpp
#include <iostream>
class Singoton {
public:
	static Singoton* getInstance() {
		if (ptr == nullptr) {
			ptr = new Singoton();
		}
		return ptr;
	} //类中的静态函数只能访问类中静态成员变量
	void show() {
		std::cout << "lazy\n";
	}
	Singoton(const Singoton& s) = delete;
	Singoton& operator=(const Singoton& s) = delete;
private:
	Singoton() {};
	static Singoton* ptr; //声明
};
Singoton* Singoton::ptr = nullptr; //定义

int main() { //测试
	Singoton* op = Singoton::getInstance();
	op->show();
}
```
试想，现在有两个线程调用了Singoton::getInstance()方法，线程A首先进行判空操作，于是new了一个Singoton对象，此时发生了上下文切换，调度权给到了线程B，线程B首先也进行判空操作，发现为空，于是也new了一个Singoton对象，就发生了问题。<br>
* 双重检查锁定解决线程安全问题

```cpp
#include <mutex>
class Singoton {
public:
	static Singoton* getInstance() {
		mtx.lock();
		if (ptr == nullptr) {
			ptr = new Singoton();
		}
		mtx.unlock();
		return ptr;
	} //类中的静态函数只能访问类中静态成员变量
	void show() {
		std::cout << "lazy\n";
	}
	Singoton(const Singoton& s) = delete;
	Singoton& operator=(const Singoton& s) = delete;
private:
	Singoton() {};
	static Singoton* ptr; //声明
	static std::mutex mtx;
};
Singoton* Singoton::ptr = nullptr; //定义
std::mutex Singoton::mtx;
```
假想现在有三个线程A B C，线程A首先加锁，再判断、new对象，若此时发生了上下文切换，调度权给了线程B和线程C，但是他没有锁资源，进入阻塞态，最终调度权回归线程A，将锁头解锁，于是线程B和线程C自然就解除阻塞，进行判空操作，此时就会因为判空失败而直接返回，线程安全<br>
但是效率太低了，进行双重检查，在加锁之前再进行一次判空操作：

```cpp
static Singoton* getInstance() {
	if (ptr == nullptr) {
		mtx.lock();
		if (ptr == nullptr) {
			ptr = new Singoton();
		}
		mtx.unlock();
	}
	return ptr;
}
```

* 原子操作弥补双重检查的bug
因为`new`操作符在汇编中是对应的机器指令可能会发生重排，依旧会产生线程安全问题，可以采用原子操作来弥补

```cpp
#include <iostream>
#include <mutex>
#include <atomic>
class Singoton {
public:
	static Singoton* getInstance() {
		Singoton* task = sptr.load(); //将sptr从原子中取出备用
		if (task == nullptr) {
			mtx.lock();
			task = sptr.load();
			if (task == nullptr) {
				task = new Singoton();
				sptr.store(task);
			}
			mtx.unlock();
		}
		task = sptr.load();
		return task;
	} //类中的静态函数只能访问类中静态成员变量
	void show() {
		std::cout << "lazy\n";
	}
	Singoton(const Singoton& s) = delete;
	Singoton& operator=(const Singoton& s) = delete;
private:
	Singoton() {};
	static std::mutex mtx;
	static std::atomic<Singoton*> sptr;
};
std::mutex Singoton::mtx;
std::atomic<Singoton*> Singoton::sptr = nullptr; //定义

int main() { //测试
	Singoton* op = Singoton::getInstance();
	op->show();
}
```

* 静态对象（需要编译器兼容C++11标准）
```cpp
#include <iostream>
class Singoton {
public:
	static Singoton* getInstance() {
		static Singoton sn; //必须静态，不然出了大括号其生命周期就结束
		return &sn; //取地址并返回
	}
	void show() {
		std::cout << "lazy\n";
	}
	Singoton(const Singoton& s) = delete;
	Singoton& operator=(const Singoton& s) = delete;
private:
	Singoton() {};
};

int main() { //测试
	Singoton* op = Singoton::getInstance();
	op->show();
}
```

C++11规定，当逻辑进入一个未被初始化的声明变量，所有的并发操作都必须等待该变量完成初始化。<br>
静态变量在全局/静态区，只要在分区内，只要数据等于0，就是未被初始化的，比如`int a = 0;`，`int b;`；但是`int c = 1;`就是初始化过的变量<br>
所以`static Singoton sn;`也是未初始化的静态局部变量，于是所有的并发操作都会被阻塞

<h2>简单工厂模式</h2>
* 优点
简单工厂的特点就是“简单粗暴”，通过一个含参的工厂方法，我们可以实例化任何产品类，上至飞机火箭，下至土豆面条，无所不能。所以简单工厂有一个别名：上帝类。
* 缺点
违反“封闭-开放原则”，简单工厂对于增加新的产品，无能为力，因为增加新产品只能通过修改工厂方法来实现，违反“封闭”原则。

```cpp
#include <iostream>
class AbstractPerson {
public:
	virtual void IQ() = 0;
	virtual void EQ() = 0;
	//动态绑定就是父类指针绑定子类对象，在delete父类指针时，若没重写，只会释放父类资源而不会释放子类资源
	virtual ~AbstractPerson() {};
};
class School : public AbstractPerson {
public:
	void IQ() override {
		std::cout << "学校期间涨20点IQ." << std::endl;
	}
	void EQ() override {
		std::cout << "学校期间涨3点EQ." << std::endl;
	}
};
class Social : public AbstractPerson {
public:
	void IQ() override {
		std::cout << "社会期间降5点IQ." << std::endl;
	}
	void EQ() override {
		std::cout << "社会期间涨20点EQ." << std::endl;
	}
};

//工厂类
enum class Type:char{Shazi,Lengzi}; //C++强类型枚举，数据类型为char型
class Factory {
public:
	AbstractPerson* createPerson(Type type) {
		AbstractPerson* ptr = nullptr;
		if (type == Type::Shazi)
			ptr = new School; //傻子就送去学校
		else if (type == Type::Lengzi)
			ptr = new Social; //愣子就送进社会
		return ptr;
	}
};
int main() {
	Factory factory;
	AbstractPerson* p = factory.createPerson(Type::Shazi); //父类指针指向子类对象，实现动态绑定
	p->IQ();
	p->EQ();
}
```

<h2>工厂模式</h2>

```cpp
#include <iostream>
class AbstractPerson {
public:
	virtual void IQ() = 0;
	virtual void EQ() = 0;
	//动态绑定就是父类指针绑定子类对象，在delete父类指针时，若没重写，只会释放父类资源而不会释放子类资源
	virtual ~AbstractPerson() {};
};
class School : public AbstractPerson {
public:
	void IQ() override {
		std::cout << "学校期间涨20点IQ." << std::endl;
	}
	void EQ() override {
		std::cout << "学校期间涨3点EQ." << std::endl;
	}
};
class Social : public AbstractPerson {
public:
	void IQ() override {
		std::cout << "社会期间降5点IQ." << std::endl;
	}
	void EQ() override {
		std::cout << "社会期间涨20点EQ." << std::endl;
	}
};

enum class Type:char{Shazi,Lengzi}; //C++强类型枚举，数据类型为char型
//不同职责工厂的父类类
class AbstractFactory {
public:
	virtual AbstractPerson* createPerson() = 0;
	virtual ~AbstractFactory() {}
};
//加IQ的工厂类
class IQFactory : public AbstractFactory {
public:
	AbstractPerson* createPerson() {
		return new School;
	}
};
//加EQ的工厂类
class EQFactory : public AbstractFactory {
public:
	AbstractPerson* createPerson() {
		return new Social;
	}
};
int main() {
	AbstractFactory* af = new IQFactory;
	AbstractPerson* p = af->createPerson();
	p->IQ();
	p->EQ();
}
```

<h2>抽象工厂模式</h2>

```cpp
#include <iostream>
#include <string>
//船体
class shipBody {
public:
	virtual std::string getBody() = 0;
	virtual ~shipBody() {}; //虚析构函数
};
//钢制船
class steelBoot : public shipBody {
public:
	std::string getBody() override {
		return std::string("钢铁船身.");
	}
};
//合金船
class alloyBoot : public shipBody {
public:
	std::string getBody() override {
		return std::string("合金船身.");
	}
};

//引擎
class engine {
public:
	virtual std::string getEngine() = 0;
	virtual ~engine() {};
};
//柴油动力
class oilEngine : public engine {
public:
	std::string getEngine() override {
		return std::string("柴油动力.");
	}
};
//核动力
class atomicEngine : public engine {
public:
	std::string getEngine() override {
		return std::string("核动力.");
	}
};

//船
class ship {
public:
	ship(shipBody* s, engine* e) : m_shipBody(s), m_engine(e) {}
	~ship() {
		delete m_shipBody;
		delete m_engine;
		m_shipBody = nullptr;
		m_engine = nullptr;
	}
	std::string getInfo() {
		std::string info = m_shipBody->getBody() + m_engine->getEngine();
		return info;
	}
private:
	shipBody* m_shipBody;
	engine* m_engine;
};

//抽象工厂类
class AbstractFactory {
public:
	virtual ship* createShip() = 0;
	virtual ~AbstractFactory() {};
};
//造最基本的船
class basicShip : public AbstractFactory {
public:
	ship* createShip() override {
		ship* sp = new ship(new steelBoot, new oilEngine);
		return sp;
	}
};
//造最先进的船
class ultraShip : public AbstractFactory {
public:
	ship* createShip() override {
		ship* sp = new ship(new alloyBoot, new atomicEngine);
		return sp;
	}
};
int main() {
	//我要买最先进的船！
	AbstractFactory* asf = new basicShip;
	ship* sip = asf->createShip();
	std::cout << sip->getInfo() << std::endl;
	delete sip;
	delete asf;
}
```

<h1 id="2">GDB调试</h1>

### 运行
- run(r)：运行程序，当遇到断点后，程序会在断点处停止运行，等待下一步命令
- continue(c)：继续执行，到下一个断点处（或运行结束）
- next(n)：单步跟踪程序，当遇到函数调用时，也不进入此函数体
- step(s)：单步调试如果有函数调用，则进入函数；与命令n不同，n是不进入调用的函数的
- until：当你厌倦了在一个循环体内单步跟踪时，这个命令可以运行程序直到退出循环体
- until+行号： 运行至某行，不仅仅用来跳出循环
- finish： 运行程序，直到当前函数完成返回，并打印函数返回时的堆栈地址和返回值及参数值等信息
- call 函数(参数)：调用程序中可见的函数，并传递“参数”，如：call gdb_test(55)
- quit(q)：退出gdb

### 断点
- break n (简写b n):在第n行处设置断点
- b fn1 if a＞b：条件断点设置
- break func (b func)：在函数func()的入口处设置断点
- delete n：删除第n个断点
- disable n：暂停第n个断点
- enable n：开启第n个断点
- info b：显示当前程序的断点设置情况
- delete breakpoints：清除所有断点

### 查看运行信息
- bt: 当前运行的堆栈列表
- bt backtrace: 显示当前调用堆栈
- up/down 改变堆栈显示的深度
- set args 参数:指定运行时的参数
- show args：查看设置好的参数
- info program：来查看程序的是否在运行，进程号，被暂停的原因

### 调试正在运行中的程序
新建一个窗口，`ps -ef|grep a.out`显示正在运行的a.out程序的线程ID，`gdb a.out -p ID号`进入gdb调试

### 结束进程
- killall根据名称终止正在运行的进程，而kill根据进程ID号（PID）终止进程
- pidof a.out：找到进程的PID，或者`ps -ef|grep a.out`
- kill -9 2551 2514 ：中止PID为2511和2514的进程，或者`killall -9 a.out`
