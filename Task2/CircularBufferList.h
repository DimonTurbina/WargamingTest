#include <iostream>
#include <memory>


template<typename ElementType>
class Node {
	using ENode = Node<ElementType>;

public:
	ElementType data;
	std::shared_ptr<ENode> _next;
public:
	Node() = default;

	Node(const Node& right)
	{
		assign(right);
	}

	Node(Node&& right) noexcept
	{
		assign(std::move_if_noexcept(right));
	}

	Node& operator=(const Node& right)
	{
		assign(right);
		return *this;
	}

	Node& operator=(Node&& right) noexcept
	{
		assign(std::move_if_noexcept(right));
		return *this;
	}

private:
	void assign(const Node& right)
	{
		data = right.data;
		_next = nullptr;
	}

	void assign(Node&& right) noexcept
	{
		data = std::move_if_noexcept(right.data);
		_next = nullptr;
		right._next = nullptr;
	}
};

template<typename ElementType>
class CircularBuffer {
	using ENode = Node<ElementType>;

	std::shared_ptr<ENode> _head;
	std::shared_ptr<ENode> _tail;
	unsigned int _size = 0;
	unsigned int _curSize = 0;

public:

	CircularBuffer(unsigned int size) : _size(size){

		std::shared_ptr<ENode> current(new ENode);
		std::shared_ptr<ENode> start(new ENode);
		current->_next = current;
		start = current;

		this->_head = start;

		for (size_t i = 1; i < _size; ++i) {
			std::shared_ptr<ENode> tmp(new ENode);
			_tail = tmp;
			tmp->_next = start;
			current->_next = tmp;
			current = current->_next;
		}
	}

	void push(const ElementType& value) {
		_tail = _tail->_next;
		_tail->data = value;
		if (_curSize == _size) _head = _head->_next;
		if(_curSize < _size) _curSize++;
	}

	void pop() {
		if (_curSize <= 0) throw std::out_of_range("Empty array!");
		_head = _head->_next;
		_curSize--;
	}

	unsigned int size() const noexcept {
		return _size;
	}

	bool isEmpty() const noexcept {
		if (_curSize == 0) { return true; }
		else return false;
	}

	ElementType first() const noexcept {
		return _head->data;
	}

	void clear() {
		while (!this->isEmpty()) {
			this->pop();
		}
	}
};