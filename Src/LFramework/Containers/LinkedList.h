#pragma once

#include <cstdint>
#include <cstddef>

class LinkedList;

class LinkedListItem {
public:
	LinkedListItem* next = nullptr;
	LinkedListItem* previous = nullptr;
	LinkedList* owner = nullptr;
};


class LinkedList {
public:

	LinkedList():_size(0){
		_head.next = &_head;
		_head.previous = &_head;
		_head.owner = this;
	}

	bool pushFront(LinkedListItem* newItem){
		if (newItem->owner == nullptr) {
			newItem->owner = this;
			newItem->next = _head.next;
			newItem->previous = &_head;
			_head.next->previous = newItem;
			_head.next = newItem;
			++_size;
			return true;
		}
		return false;
	}

	bool pushBack(LinkedListItem* newItem) {
		if (newItem->owner == nullptr) {
			newItem->owner = _head.owner;
			newItem->previous = _head.previous;
			newItem->next = &_head;
			_head.previous->next = newItem;
			_head.previous = newItem;
			++_size;
			return true;
		}
		return false;
	}

	size_t size() const {
		return _size;
	}

	void remove(LinkedListItem* item) {
		if (item->owner == this) {
			item->next->previous = item->previous;
			item->previous->next = item->next;
			--_size;
			item->next = nullptr;
			item->previous = nullptr;
			item->owner = nullptr;
		}
	}

	template<typename T>
	void foreach(T functor) {
		auto current = _head.next;
		while (current != &_head) {
			functor(current);
			current = current->next;
		}
	}
private:
	size_t _size;
	LinkedListItem _head;
};

