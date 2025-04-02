#include "classes.h"
#include <vector>


struct Person
{
	std::string name;
	int age;
};

std::ostream& operator<<(std::ostream& output, const Person p) {
	output << "Name: " << p.name << " Age : " << p.age << std::endl;
	return output;
}


int main()
{
	
	SinglyLinkedlist<int> s;
	SinglyLinkedlist<int> sss;
	SinglyLinkedlist<int> kkk;
	SinglyLinkedlist<std::string> lll;

	lll.insertNode("abvgd"); //string testing
	lll.insertNode("abcd");
	lll.SaveToFile("test1.txt");
	lll.sort();
	lll.print();
	SinglyLinkedlist<Person> ppp; //structure testing
	Person a, b;
	a.name = "Name1";
	a.age = 3;
	b.name = "Name2";
	b.age = 4;
	ppp.insertNode(a);
	ppp.insertNode(b);
	ppp.print();



	
	s.insertNode(5);
	s.insertNode(4);
	s.insertNode(3);
	s.insertNode(2);
	s.insertNode(1);

	
	s.print();
	s.sort();  //sort() testing
	s.print();

	//ss = s;
	SinglyLinkedlist<int> ss(s);
	ss.print();
	s.insertNode(9);
	ss.print();
	s.print();
	std::cout << "==================================" << std::endl;
	std::cout << "Testing of several things" << std::endl;
	ss.insertNode(-1);
	ss.print();
	ss.sort();
	ss.print();
	sss = std::move(ss);  //move operator testing
	sss.print();
	ss.print();

	std::cout << "One: " << s.get(0)->data << std::endl;

	std::cout << "==================================" << std::endl;
	std::cout << "Serialization testing" << std::endl; //serialization testing
	s.SaveToFile("test0.txt");
	kkk.ReadFromFile("test0.txt");
	s.print();
	kkk.print();
	std::cout << "==================================" << std::endl;
	std::cout << "Move constructor testing" << std::endl; //move constructor testing
	std::vector<SinglyLinkedlist<int>> v;
	v.push_back(SinglyLinkedlist<int>(s));
	v[0].print();
	
	return 0;
}
