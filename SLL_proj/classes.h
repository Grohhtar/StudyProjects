#pragma once
#ifndef CLASSES_H
#define CLASSES_H
#include <iostream> 
#include <fstream>
#include <string>

	template <typename T>
	//! A class representing node.
	class Node
	{
	public:

		//! Variable which holds data stored in the node.
		T data; 
		//! Pointer to another node.
		std::shared_ptr< Node<T>> next;

		//! A default constructor
		Node()
		{
			data = 0;
			next = nullptr;
		}

		//! A constructor
		/*!
		\param data information to be stored in the node.
		*/
		Node(T data)
		{
			this->data = data;
			this->next = nullptr;
		}

		//! Destructor.
		~Node() //destructor
		{

		};

		
	};

	template <typename T>
	//! A class representing a SLL.
	class SinglyLinkedlist 
	{
		//! Pointer to the head of the list.
		std::shared_ptr<Node<T>> head;

	public:

		//! Default constructor.
		SinglyLinkedlist() { head = nullptr; }; 

		//! Copy constructor.
		SinglyLinkedlist(SinglyLinkedlist& SLL) 
		{
			if (SLL.head != nullptr)
			{
				this->head = std::shared_ptr<Node<T>>(new Node<T>);
				this->head->data = SLL.head->data;
				std::shared_ptr<Node<T>> t(SLL.head->next);
				std::shared_ptr<Node<T>> t1(this->head);
				while (t != nullptr)
				{
					std::shared_ptr<Node<T>> NewNode(new Node<T>);
					NewNode->data = t->data;
					t1->next = NewNode;
					t1 = t1->next;
					t = t->next;
				}
			}
			else
			{
				head = nullptr;
			}
		};

		//! Move constructor.
		SinglyLinkedlist(SinglyLinkedlist&& SLL) //move constructor
		{
			std::shared_ptr< Node<T>> t = SLL.head;

			if (SLL.head != nullptr)
			{
				this->head = std::shared_ptr<Node<T>>(new Node<T>);
				this->head->data = SLL.head->data;
				std::shared_ptr<Node<T>> t(SLL.head->next);
				std::shared_ptr<Node<T>> t1(this->head);
				while (t != nullptr)
				{
					std::shared_ptr<Node<T>> NewNode(new Node<T>);
					NewNode->data = t->data;
					t1->next = NewNode;
					t1 = t1->next;
					t = t->next;
				}
				SLL.head = nullptr;
			}
			else
			{
				head = nullptr;
			}

		};

		//! Destructor.
		~SinglyLinkedlist() //destructor
		{

		};

		//! A function which adds a new element to the container.
		/*!
		\param data information to be stored in the new node.
		*/
		void insertNode(T data)
		{
			std::shared_ptr<Node<T>> NewNode(new Node<T>(data));

			if (head == nullptr)
			{
				head = NewNode;
				return;
			}

			std::shared_ptr<Node<T>> t(head);
			while (t->next != nullptr)
			{
				t = t->next;
			}
			t->next = NewNode;
		};

		//! A function which prints the whole list.
		void print() 
		{
			std::shared_ptr<Node<T>> t(head);

			if (head == nullptr)
			{
				std::cout << "List is empty" << std::endl;
				return;
			}

			while (t != nullptr)
			{
				std::cout << t->data << " ";
				t = t->next;
			}
			std::cout << std::endl;
		}; 

		//! An overloaded assignment operator.
		void operator=(const SinglyLinkedlist& SLL) 
		{
			if (SLL.head != nullptr) {
				this->head = std::shared_ptr<Node<T>>(new Node<T>);
				this->head->data = SLL.head->data;
				std::shared_ptr<Node<T>> t(SLL.head->next);
				std::shared_ptr<Node<T>> t1(this->head);
				while (t != nullptr) {
					std::shared_ptr< Node<T>> NewNode(new Node<T>);
					NewNode->data = t->data;
					t1->next = NewNode;
					t1 = t1->next;
					t = t->next;
				}
			}
			else
			{
				head = nullptr;
			}
		}; 

		//! An overloaded move operator.
		SinglyLinkedlist& operator=(SinglyLinkedlist&& SLL)
		{
			std::shared_ptr<Node<T>> t(SLL.head);
			if (SLL.head != nullptr)
			{
				this->head = std::shared_ptr<Node<T>>(new Node<T>);
				this->head->data = SLL.head->data;
				std::shared_ptr<Node<T>> t(SLL.head->next);
				std::shared_ptr<Node<T>> t1(this->head);
				while (t != nullptr)
				{
					std::shared_ptr<Node<T>> NewNode(new Node<T>);
					NewNode->data = t->data;
					t1->next = NewNode;
					t1 = t1->next;
					t = t->next;
				}

				SLL.head = nullptr;
			}
			else
			{
				head = nullptr;
			}

			return *this;
		};

		//! A function searches for a specific element in the container.
		/*!
		\param val value of the searched element.
		\return the searched element, if not presented the head is returned.
		*/
		std::shared_ptr<Node<T>> get(T val) 
		{
			std::shared_ptr<Node<T>> t(head);

			if (head == nullptr)
			{
				std::cout << "List is empty" << std::endl;
				return nullptr;
			}

			while (t != nullptr)
			{
				if (t->data == val)
				{
					return t;
				}
				t = t->next;
			}
			std::cout << "Element is not presented" << std::endl;
			return head;
		};


		//! A function sorting the list in ascending order, bubble sort algorithm is used.
		void sort() 
		{
			std::shared_ptr<Node<T>> t(head);
			std::shared_ptr<Node<T>> c1(head);
			std::shared_ptr<Node<T>> c2(head);
			int l = 0;

			while (t != nullptr)
			{
				t = t->next;
				l++;
			}

			for (int i = 0; i < l; i++)
			{
				for (int j = 0; j < l - 1; j++)
				{
					if (c1->data < c2->data)
					{
						std::swap(c1->data, c2->data);
					}

					c2 = c2->next;
				}

				c2 = head;
				c1 = c2->next;
				for (int k = 0; k < i; k++)
				{
					c1 = c1->next;
				}
			}

		}; 

		//! A function which saves the current state of the structure to a file.
		/*!
		\param fname name of the file, where data will be stored.
		\return true if succesfuul, false if no.
		*/
		bool SaveToFile(std::string fname) 
		{
			std::ofstream fout(fname, std::ios::out | std::ios::binary);

			std::shared_ptr<Node<T>> t(head);
			int l = 0;
			while (t != nullptr)
			{
				t = t->next;
				l++;
			}

			t = head;

			if (!fout)
			{
				return false;
			}

			fout.write((char*)&l, sizeof(l));

			while (t != nullptr)
			{
				fout.write((char*)&t->data, sizeof(t->data));
				t = t->next;
			}

			fout.close();
			return true;
		};

		//! A function which loades state of the structure from a file.
		/*!
		\param fname name of the file, where data is stored.
		\return true if succesfuul, false if no.
		*/
		bool ReadFromFile(std::string fname)
		{
			std::ifstream fin(fname, std::ios::in | std::ios::binary);
			int l;
			T buffer;

			if (!fin)
			{
				return false;
			}

			head = nullptr;

			fin.read((char*)&l, sizeof(int));

			for (int i = 0; i < l; i++)
			{
				fin.read((char*)&buffer, sizeof(T));
				insertNode(buffer);

			}

			fin.close();

			return true;
		};
	};

#endif