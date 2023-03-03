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
