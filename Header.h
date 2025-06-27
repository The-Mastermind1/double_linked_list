
#pragma once
#include<iostream>
#include<functional>
#include<type_traits>
#include<new>
#include<algorithm>
#include<utility>
#include<initializer_list>
#include<cstdlib>
#include"Macros.h"

_PANAGIOTIS_BEGIN
template<typename _Ty>
class double_linked_list final {
private:
	class list_node final {
	public:
		_Ty data;
		list_node* next;
		list_node* prev;
		list_node() :data{}, next{}, prev{}
		{
		}
		list_node(const _Ty& item)noexcept(noexcept(data = item)) :next{}, prev{}
		{
			data = item;
		}
		list_node(_Ty&& item)noexcept(noexcept(data = std::move(item))) :next{}, prev{}
		{
			data = std::move(item);
		}
	};
	list_node* head;
	list_node* tail;
	std::size_t count;
	//
	template<typename _Valty>
	bool push_back_node(_Valty&& _Val) {
		list_node* ptr{ new(std::nothrow)list_node{std::forward<_Valty>(_Val)} };
		if (ptr != nullptr) {
			ptr->prev = tail;
			if (head == nullptr) {
				head = ptr;
			}
			else {
				tail->next = ptr;
			}
			tail = ptr;
			count++;
			return true;
		}
		return false;
	}
	//
	//
	void clear()noexcept {
		list_node* ptr{ };
		while (head != nullptr) {
			ptr = head;
			head = head->next;
			delete ptr;
		}
		head = tail = nullptr;
		count = 0;
	}
	//
	//
	template<typename _Valty>
	bool push_front_node(_Valty&& _Val) {
		list_node* ptr{ new(std::nothrow)list_node{std::forward<_Valty>(_Val)} };
		if (ptr != nullptr) {
			ptr->next = head;
			if (head == nullptr) {
				tail = ptr;
			}
			else {
				head->prev = ptr;
			}

			head = ptr;
			count++;
			return true;
		}
		return false;
	}
	//
	//
	void pop_front_node() {
		if (count != 0) {
			list_node* ptr{ head };
			head = head->next;
			delete ptr;
			if (head == nullptr) {
				tail = nullptr;
			}
			else {
				head->prev = nullptr;
			}
			count--;
			return;
		}
		throw pop_from_an_empty_list{ "tried to pop from an empty list" };
	}
	//
	//
	void pop_back_node() {
		if (count != 0) {
			count--;
			if (count == 0) {
				delete head;
				head = tail = nullptr;
			}
			else {
				tail->prev->next = nullptr;
				list_node* ptr{ tail };
				tail = tail->prev;
				delete ptr;

			}

			return;
		}
		throw pop_from_an_empty_list{ "tried to pop from an empty list" };
	}
	//
	/*void reverse_list()noexcept {
		if (count < 2)return;
		tail = head;
		list_node* ptr1{ head };
		list_node* ptr2{ head->next };
		while (ptr2 != nullptr) {
			ptr1->next = ptr2->next;
			ptr2->next = head;
			head = ptr2;
			ptr2 = ptr1->next;
		}
		head->prev = nullptr;
		tail->next = nullptr;
		list_node* prev{ head };
		list_node* curr{ head->next };
		while (curr != nullptr) {
			curr->prev = prev;
			prev = curr;
			curr = curr->next;
		}

		return;
	}*/
	//
	void reverse_list() noexcept {
		if (count < 2)return;
		tail = head;
		list_node* ptr1{ head };
		list_node* ptr2{ head->next };
		while (ptr2 != nullptr) {
			//ptr2->prev = ptr1;
			ptr1->next = ptr2->next;
			if (ptr2->next != nullptr) {
				ptr2->next->prev = ptr1;
			}
			ptr2->next = head;
			head->prev = ptr2;
			ptr2->prev = nullptr;
			head = ptr2;

			ptr2 = ptr1->next;


		}

	}
	//
	//
	void insert_node(const _Ty& data) {
		list_node* ptr{ new (std::nothrow)list_node{data} };
		if (ptr == nullptr)return;
		list_node* prev{ nullptr };
		list_node* curr{ head };
		while (curr != nullptr && curr->data < data) {
			prev = curr;
			curr = curr->next;
		}
		count++;
		if (prev == nullptr) {
			ptr->next = head;
			if (head == nullptr) {
				tail = ptr;
			}
			else {
				head->prev = ptr;
			}
			head = ptr;
			return;
		}
		else  {
			prev->next = ptr;
			ptr->prev = prev;
			ptr->next = curr;
			if (curr != nullptr)
				curr->prev = ptr;
			else
				tail = ptr;
		}
		return;


	}
	//
	//
	bool add_unique_node(const _Ty& data) {
		list_node* ptr{ new (std::nothrow)list_node{data} };
		if (ptr == nullptr)return false;
		list_node* prev{ nullptr };
		list_node* curr{ nullptr };
		while (curr != nullptr && curr->data < data) {
			prev = curr;
			curr = curr->next;
		}
		list_node* ptr1{ curr };
		while (ptr1 != nullptr) {
			if (ptr1->data == data)return false;
			ptr1 = ptr1->next;
		}
		count++;
		if (prev == nullptr) {
			ptr->next = head;
			if (head == nullptr) {
				tail = ptr;
			}
			else {
				head->prev = ptr;
			}
			head = ptr;
			return;
		}
		else {
			prev->next = ptr;
			ptr->prev = prev;
			ptr->next = curr;
			if (curr != nullptr)
				curr->prev = ptr;
			else
				tail = ptr;
		}
		return true;
		
	}
	//
	//
	void erase_all(const _Ty& data)noexcept {
		while (head != nullptr && head->data == data) {
			pop_front();
		}
		if (head != nullptr) {
			list_node* prev{ head };
			list_node* curr{ head->next };
			while (curr != nullptr) {
				if (curr->data == data) {
					count--;
					prev->next = curr->next;
					if (curr->next != nullptr) {
						curr->next->prev = prev;
					}
					
					delete curr;
					curr = prev->next;

				}
				else {
					prev = curr;
					curr = curr->next;
				}
			}
			tail = prev;
		}
	}
	// 
	// 	
	void erase_first(const _Ty&data)noexcept {
		list_node* prev{ nullptr };
		list_node* curr{ head };
		while (curr != nullptr && curr->data != data) {
			prev = curr;
			curr = curr->next;
		}
		if (prev == nullptr &&curr!=nullptr) {
			pop_front();
			return;
		}
		if (prev!=nullptr &&curr != nullptr) {
			count--;
			prev->next = curr->next;
			if (curr->next != nullptr) {
				curr->next->prev = prev;
			}
			else {
				tail = prev;
			}
			delete curr;
		}
		
	}
	//
	//
	void erase_duplicates() {
		if (count < 2)return;
		list_node* prev{ head };
		list_node* curr{ head->next };
		while (curr != nullptr) {
			if (prev->data == curr->data) {
				count--;
				prev->next = curr->next;
				if (curr->next != nullptr) {
					curr->next->prev = prev;
				}
				delete curr;
				curr = prev->next;
		


			}
			else {
				prev = prev->next;
				curr = curr->next;
			}
		}
		tail = prev;
		return;
	}
	//
	//
	void merge_in_place(double_linked_list<_Ty>& other)noexcept {
		if (this == &other)return;
		if (empty() && other.empty())return;
		if (!is_ascending() && !other.is_ascending())return;
		list_node* dummy{ new (std::nothrow)list_node{} };
		if (dummy == nullptr)return;
		list_node* ptr{ dummy };
		list_node* curr1{ head };
		list_node* curr2{ other.head };
		while (curr1 != nullptr && curr2 != nullptr) {
			if (curr1->data <= curr2->data) {
				ptr->next = curr1;
				curr1->prev = ptr;
				ptr = ptr->next;
				curr1 = curr1->next;
			}
			else {
				ptr->next = curr2;
				curr2->prev = ptr;
				ptr = ptr->next;
				curr2 = curr2->next;
			}
		}
		
		if (curr1 == nullptr && curr2 != nullptr) {
			ptr->next = curr2;
			curr2->prev = ptr;
			tail = other.tail;
		}
		if (curr2 == nullptr && curr1 != nullptr) {
			ptr->next = curr1;
			curr1->prev = ptr;
		}
		
		head = dummy->next;
		head->prev = nullptr;
		count = count + other.count;
		delete dummy;
		other.head = other.tail = nullptr;
		other.count = 0;
	}
	//
	//
	std::size_t merge_in_place(const double_linked_list<_Ty> &other1
	,const double_linked_list<_Ty> &other2) {
		if (this == &other1 || this == &other2)return 0;
		if (other1.empty() && other2.empty())return 0;
		if (!other1.is_ascending() && !other2.is_ascending())return 0;

		clear();
		list_node* curr1{ other1.head };
		list_node* curr2{ other2.head };
		while (curr1 != nullptr && curr2 != nullptr) {
			if (curr1->data <= curr2->data) {
				if (!push_back(curr1->data)) {
					clear();
					return 0;
				}
				curr1 = curr1->next;
			}
			else {
				if (!push_back(curr2->data)) {
					clear();
					return 0;
				}
				curr2 = curr2->next;
			}
		}
		while (curr1 != nullptr) {
			if (!push_back(curr1->data)) {
				clear();
				return 0;
			}
			curr1 = curr1->next;
		}
		while (curr2 != nullptr) {
			if (!push_back(curr2->data)) {
				clear();
				return 0;
			}
			curr2 = curr2->next;
		}
		return count;//the count of nodes that merged
	}
	//

public:
	double_linked_list()noexcept;
	double_linked_list(const std::initializer_list<_Ty>& l);
	double_linked_list(const double_linked_list<_Ty>& other);
	double_linked_list(double_linked_list<_Ty>&& other)noexcept;
	~double_linked_list()noexcept;
	double_linked_list<_Ty>& operator=(const double_linked_list<_Ty>& Other)&;
	double_linked_list<_Ty>& operator=(double_linked_list<_Ty>&& Other) & noexcept;
	bool push_back(const _Ty& data);
	bool push_back(_Ty&& data);
	bool push_front(const _Ty& data);
	bool push_front(_Ty&& data);
	void pop_front();
	void pop_back();
	bool is_ascending()const noexcept;
	bool is_descending()const noexcept;
	bool is_sorted()const noexcept;
	bool empty()const noexcept;

	void delete_first(const _Ty& data)noexcept;

	bool add_unique(const _Ty& data);

	void delete_duplicates() noexcept;
	void delete_all(const _Ty& data)noexcept;

	std::size_t size()const noexcept;
	void insert(const _Ty& data);
	void reverse()noexcept;
	void show();

	void merge( double_linked_list<_Ty>& other)noexcept;

	std::size_t merge(const double_linked_list<_Ty>& other1,
		const double_linked_list<_Ty>& other2)noexcept;
	std::size_t count_items()const noexcept;
	_NODISCARD _Ty&& back()&& {
		if (count == 0) {
			throw tried_to_access_an_empty_list_{ "tried to access  an empty list" };
		}
		return std::move(tail->data);

	}
	_NODISCARD const _Ty&& back()const&& {
		if (count == 0) {
			throw tried_to_access_an_empty_list_{ "tried to access  an empty list" };
		}
		return std::move(tail->data);

	}
	_NODISCARD const _Ty& back()const& {
		if (count == 0) {
			throw tried_to_access_an_empty_list_{ "tried to access  an empty list" };
		}
		return tail->data;

	}
	_NODISCARD _Ty& back()& {
		if (count == 0) {
			throw tried_to_access_an_empty_list_{ "tried to access  an empty list" };
		}
		return tail->data;

	}

	_NODISCARD const _Ty& front()const& {
		if (count == 0) {
			throw tried_to_access_an_empty_list_{ "tried to access  an empty list" };
		}
		return head->data;

	}
	_NODISCARD _Ty& front()& {
		if (count == 0) {
			throw tried_to_access_an_empty_list_{ "tried to access  an empty list" };
		}
		return head->data;

	}
	_NODISCARD _Ty&& front()&& {
		if (count == 0) {
			throw tried_to_access_an_empty_list_{ "tried to access  an empty list" };
		}
		return std::move(head->data);

	}
	_NODISCARD const _Ty&& front()const&& {
		if (count == 0) {
			throw tried_to_access_an_empty_list_{ "tried to access  an empty list" };
		}
		return std::move(head->data);

	}
	//
	//
	void add_front(const double_linked_list<_Ty>& other) {
		if (other.empty())return;
		list_node* ptr{ other.head };
		while (ptr != nullptr) {
			if (!push_front(ptr->data)) {
				clear();
				break;

			}
			ptr = ptr->next;
		}
	}
	//
	//
	void add_front(const double_linked_list<_Ty>& other) {
		if (other.empty())return;
		list_node* ptr{ other.head };
		while (ptr != nullptr) {
			if (!push_back(ptr->data)) {
				clear();
				break;

			}
			ptr = ptr->next;
		}

	}





};
//
template<typename _Ty>
double_linked_list<_Ty>::double_linked_list()noexcept :head{}, tail{}, count{}
{
}
//
//
template<typename _Ty>
double_linked_list<_Ty>::double_linked_list(double_linked_list<_Ty>&& other)
noexcept :head{}, count{}, tail{}
{
	std::swap(head, other.head);
	std::swap(tail, other.tail);
	std::swap(count, other.count);

}
//
//
template<typename _Ty>
double_linked_list<_Ty>::~double_linked_list()noexcept {
	clear();
}
//
//
template<typename _Ty>
bool double_linked_list<_Ty>::push_back(const _Ty& data) {
	return push_back_node(data);
}
//
//
template<typename _Ty>
bool double_linked_list<_Ty>::push_back(_Ty&& data) {
	return push_back_node(std::move(data));
}
//
//
template<typename _Ty>
void double_linked_list<_Ty>::show() {
	list_node* ptr{ head };
	while (ptr != nullptr) {
		std::cout << ptr->data << '\n';
		ptr = ptr->next;
	}
	std::cout << "\n";
	ptr = tail;
	while (ptr != nullptr) {
		std::cout << ptr->data << '\n';
		ptr = ptr->prev;
	}
	return;
}
//
//
template<typename _Ty>
bool double_linked_list<_Ty>::push_front(const _Ty& data) {
	return push_front_node(data);
}
//
//
template<typename _Ty>
bool double_linked_list<_Ty>::push_front(_Ty&& data) {
	return push_front_node(data);
}
//
template<typename _Ty>
void double_linked_list<_Ty>::pop_front() {
	pop_front_node();
}
//
//
template<typename _Ty>
void double_linked_list<_Ty>::pop_back() {
	pop_back_node();
}
//
//
template<typename _Ty>
std::size_t double_linked_list<_Ty>::size()const noexcept {
	return count;
}
//
//
template<typename _Ty>
bool double_linked_list<_Ty>::empty()const noexcept {
	return count == 0;
}
//
//
template<typename _Ty>
void double_linked_list<_Ty>::reverse()noexcept {
	reverse_list();
}
//
//
template<typename _Ty>
double_linked_list<_Ty>::double_linked_list(const double_linked_list<_Ty>& other) :head{}, count{}, tail{} {
	list_node* ptr{ head };
	while (ptr != nullptr) {
		if (!push_back(ptr->data)) {
			clear();
			break;
		}
		ptr = ptr->next;
	}
}
//
//
template<typename _Ty>
double_linked_list<_Ty>& double_linked_list<_Ty>::
operator=(double_linked_list<_Ty>&& other) & noexcept {
	clear();
	std::swap(head, other.head);
	std::swap(count, other.count);
	std::swap(tail, other.tail);
	return *this;
}
//
//
template<typename _Ty>
double_linked_list<_Ty>& double_linked_list<_Ty>::
operator=(const double_linked_list<_Ty>& other)& {
	if (this != &other) {
		list_node* prev1{ nullptr };
		list_node* prev2{ nullptr };
		list_node* curr1{ head };
		list_node* curr2{ other.head };
		while (curr1 != nullptr && curr2 != nullptr) {
			curr1->data = curr2->data;
			prev1 = curr1;
			prev2 = curr2;
			curr1 = curr1->next;
			curr2 = curr2->next;
		}
		if (prev1 == nullptr && curr2 != nullptr || curr2 != nullptr && curr1 == nullptr) {
			while (curr2 != nullptr) {
				if (!push_back(curr2->data)) {
					clear();
					break;
				}
				curr2 = curr2->next;
			}
			return *this;
		}
		if (prev2 == nullptr && curr1 != nullptr) {
			clear();
			return *this;
		}
		if (curr1 != nullptr && curr2 == nullptr) {//size>other.size,sbhno perita
			prev1->next = nullptr;
			tail = prev1;
			//apo ekei kai pera sbhno ypoloipa
			while (curr1 != nullptr) {
				list_node* ptr{ curr1 };
				curr1 = curr1->next;
				count--;
				delete ptr;
			}
			return *this;
		}

	}
	return *this;
}
//
//
template<typename _Ty>
double_linked_list<_Ty>::double_linked_list(const std::initializer_list<_Ty>& l) :head{}, tail{}, count{} {
	const _Ty* ptr{ l.begin() };
	for (std::size_t i = 0; i < l.size(); i++) {
		if (!push_back(*ptr)) {
			clear();
			break;
		}
		ptr++;
	}
}
//
//
template <typename _Ty>
bool double_linked_list<_Ty>::
is_ascending() const noexcept
{
	if (count < 2) return true;

	list_node* CurrNode = head;
	list_node* NextNode = head->next;

	do
	{
		if (NextNode->data <= CurrNode->data) return false;
		CurrNode = NextNode;
		NextNode = NextNode->next;
	} while (NextNode != nullptr);

	return true;
}
//
//
template <typename _Ty>
bool double_linked_list<_Ty>::
is_descending() const noexcept
{
	if (count < 2) return true;

	list_node* CurrNode = head;
	list_node* NextNode = head->Next;

	do
	{
		if (CurrNode->data <= NextNode->data) return false;
		CurrNode = NextNode;
		NextNode = NextNode->next;
	} while (NextNode != nullptr);

	return true;
}
//
//
template <typename _Ty>
bool double_linked_list<_Ty>::
is_sorted() const noexcept
{
	if (count < 2) return true;

	list_node* CurrNode = head;
	list_node* NextNode = head->Next;
	bool		Asc = true;
	bool		Desc = true;

	do
	{
		Asc = Asc && (CurrNode->data <= NextNode->data);
		Desc = Desc && (NextNode->data <= CurrNode->data);
		CurrNode = NextNode;
		NextNode = NextNode->next;
	} while (NextNode != nullptr);

	return (Asc || Desc);
}
//
//
template<typename _Ty>
std::size_t double_linked_list<_Ty>::count_items()const noexcept {
	list_node* ptr{ head };
	std::size_t node_count{ 0 };
	while (ptr != nullptr) {
		node_count++;
		ptr = ptr->next;
	}
	return node_count;
}
//
//
template<typename _Ty>
void double_linked_list<_Ty>::insert(const _Ty& data) {
	insert_node(data);
}
//
//
template<typename _Ty>
bool double_linked_list<_Ty>::add_unique(const _Ty& data) {
	return add_unique_node(data);
}
//
//
template<typename _Ty>
void double_linked_list<_Ty>::delete_duplicates() noexcept{
	erase_duplicates();
}
//
//
template<typename _Ty>
void double_linked_list<_Ty>::delete_all(const _Ty& data)noexcept {
	erase_all(data);

}
//
//
template<typename _Ty>
void double_linked_list<_Ty>::delete_first(const _Ty& data)noexcept {
	erase_first(data);
}
//
//
template<typename _Ty>
void double_linked_list<_Ty>::merge(double_linked_list<_Ty>& other)noexcept {
	merge_in_place(other);
}
//
//
template<typename _Ty>
std::size_t double_linked_list<_Ty>::merge(const double_linked_list<_Ty>& other1,const double_linked_list<_Ty>& other2)noexcept {
	return merge_in_place(other1, other2);
}
//
_PANAGIOTIS_END
