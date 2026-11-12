#pragma once

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

	LinkedList(size_t cnt, T min, T max, unsigned int seed = static_cast<unsigned int>(std::time(nullptr)));

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


template <typename T>
LinkedList<T>::Node::Node(const T& value, Node* new_next) : data(value), next(new_next) {}

template <typename T>
LinkedList<T>::LinkedList() : head(nullptr), tail(nullptr), size(0) {}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& other) : head(nullptr), tail(nullptr), size(0) {
	Node* cur = other.head;
	while (cur != nullptr) {
		push_tail(cur->data);
		cur = cur->next;
	}
}

template <typename T>
LinkedList<T>::LinkedList(size_t cnt, T min, T max, unsigned int seed) : head(nullptr), tail(nullptr), size(0) {
	std::srand(seed);
	for (size_t i = 0; i < cnt; ++i) {
		T rand_val = minl + static_cast<T>(std::rand()) / (static_cast<T>(RAND_MAX / (max - min + 1)));
		push_tail(rand_val);
	}
}

template <typename T>
LinkedList<T>::~LinkedList() {
	clear();
}

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& other) {
	if (this == &other) {
		return *this;
	}
	clear();
	Node* cur = other.head;
	while (cur != nullptr) {
		push_tail(cur->data);
		cur = cur->next;
	}
	return *this;
}


template <typename T>
T& LinkedList<T>::operator[](size_t index) {
	if (index < 0 || index > size) {
		throw std::out_of_range("Index out of range");
	}
	Node* cur = head;
	for (size_t i = 0; i < index; i++) {
		cur = cur->next;
	}
	return cur->data;
}

template <typename T>
const T& LinkedList<T>::operator[](size_t index) const {
	if (index < 0 || index > size) {
		throw std::out_of_range("Index out of range");
	}
	Node* cur = head;
	for (size_t i = 0; i < index; i++) {
		cur = cur->next;
	}
	return cur->data;
}

template <typename T>
void LinkedList<T>::clear() {
	while (head != nullptr) {
		Node* tmp = head;
		head = head->next;
		delete tmp;
	}
	tail = nullptr;
	size = 0;
}

template <typename T>
size_t LinkedList<T>::size() const {
	return size;
}

template <typename T>
void LinkedList<T>::push_tail(const T& value) {
	Node* new_node = new Node(value);
	if (head == nullptr) {
		head = tail = new_node;
	}
	else {
		tail->next = new_node;
		tail = new_node;
	}
	size++;
}

template <typename T>
void LinkedList<T>::push_tail(const LinkedList<T>& other) {
	if (other.head == nullptr) {
		return;
	}
	Node* cur = other.head;
	size_t cnt = other.size;
	for (size_t i == 0; i < cnt; ++i) {
		push_tail(cur->data);
		cur = cur->next;
	}
}

template <typename T>
void LinkedList<T>::push_head(const T& value) {
	Node* new_node = new Node(value, head);
	head = new_node;
	if (tail == nullptr) {
		tail = new_node;
	}
	size++;
}

template <typename T>
void LinkedList<T>::push_head(const LinkedList<T>& other) {
	if (other.head == nullptr) {
		return;
	}
	LinkedList<T> tmp;
	Node* cur = other.head;
	while (cur != nullptr) {
		tmp.push_tail(cur->data);
		cur = cur->next;
	}
	tmp.tail->next = head;
	head = tmp.head;
	if (tail == nullptr) {
		tail = tmp.tail;
	}
	size += tmp.size;

	tmp.head = nullptr;
	tmp.tail = nullptr;
	tmp.size = 0;
}

template <typename T>
void LinkedList<T>::pop_head() {
	if (head == nullptr) {
		return;
	}
	Node* tmp = head;
	head = head->next;
	delete tmp;
	size--;
	if (head == nullptr) {
		tail = nullptr;
	}
}

template <typename T>
void LinkedList<T>::pop_tail() {
	if (head == nullptr) return;
	if (head == tail) {
		delete head;
		head = tail = nullptr;
		size = 0;
		return;
	}
	Node* cur = head;
	while (cur->next != tail) {
		cur = cur->next;
	}
	delete tail;
	tail = cur;
	tail->next = nullptr;
	size--;
}

template <typename T>
void LinkedList<T>::del_node(const T& value) {
	while (head != nullptr && head->data == value) {
		pop_head();
	}
	if (head == nullptr) return;

	Node* cur = head;
	while (cur->next != nullptr) {
		if (cur->next->data == value) {
			Node* to_delete = cur->next;
			cur->next = to_delete->next;
			if (to_delete == tail) {
				tail = cur;
			}
			delete to_delete;
			size--;
		}
		else {
			cur = cur->next;
		}
	}
}

template <typename U>
std::ostream& operator<<(std::ostream& os, const LinkedList<U>& list) {
	typename LinkedList<U>::Node* cur = list.head;
	os << "[";
	while (cur != nullptr) {
		os << cur->data;
		if (cur->next != nullptr) {
			os << " -> ";
		}
		cur = cur->next;
	}
	os << "]";
	return os;
}

