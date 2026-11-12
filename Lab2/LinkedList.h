#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <ctime>


template <typename T>
class LinkedList {
private:
	struct Node {
		T data;
		Node* next;
		Node(const T& value, Node* new_next = nullptr) : data(value), next(new_next) {}
	};

	Node* head;
	Node* tail;
	size_t size;

public:
	LinkedList();

	LinkedList(const LinkedList<T>& other);

	LinkedList(size_t count, T min, T max, unsigned int seed = static_cast<unsigned int>(std::time(nullptr)));

	~LinkedList();


	LinkedList<T>& operator=(const LinkedList<T>& other);

	T& operator[](size_t index);

	const T& operator[](size_t index) const;


	void clear();

	size_t size() const;


	void push_tail(const T& value);

	void push_tail(const LinkedList<T>& other);

	void push_head(const T& value);

	void push_head(const LinkedList<T>& other);


	void pop_tail();

	void pop_head();

	void del_node(const T& value);


	template <typename U>
	friend std::ostream& operator<<(std::ostream& os, const LinkedList<U>& list);
};
