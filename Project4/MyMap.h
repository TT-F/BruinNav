// MyMap.h

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.

#ifndef MyMap_h
#define MyMap_h

#include <iostream>
#include"support.h"

template<typename KeyType, typename ValueType>
class MyMap
{
public:
	MyMap();
	~MyMap();
	void clear();
	int size() const;
	void associate(const KeyType& key, const ValueType& value);

	// for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

	// for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}

	// C++11 syntax for preventing copying and assignment
	MyMap(const MyMap&) = delete;
	MyMap& operator=(const MyMap&) = delete;

private:
	struct node
	{
		KeyType m_key;
		ValueType m_value;
		node* left; //smaller 
		node* right; //larger
	};
	int m_size; 
	node *root;
	void destory_tree(node* leaf);
	int size_helper(node* point) const;
	ValueType* find_helper(const KeyType& key, node*leaf) const;
	void associate_helper(const KeyType & key, const ValueType & value, node* leaf);
	//node* newNode(const KeyType& key, const ValueType& value);
};

template<typename KeyType, typename ValueType>
inline MyMap<KeyType, ValueType>::MyMap()
{
	root = nullptr;
	m_size = 0;
}

template<typename KeyType, typename ValueType>
inline MyMap<KeyType, ValueType>::~MyMap()
{
	clear();
}

template<typename KeyType, typename ValueType>
inline void MyMap<KeyType, ValueType>::clear()
{
	destory_tree(root);
	root = nullptr;
}

template<typename KeyType, typename ValueType>
inline int MyMap<KeyType, ValueType>::size() const
{
	return size_helper(root);
}

template<typename KeyType, typename ValueType>
inline void MyMap<KeyType, ValueType>::associate(const KeyType & key, const ValueType & value)
{
	/*
	if (root == nullptr)
	{
		std::cerr << "aa" << std::endl;
		root = newNode(key, value);
		return;
	}
	std::cerr << "aab" << std::endl;*/
	associate_helper(key, value, root);
}

template<typename KeyType, typename ValueType>
inline const ValueType * MyMap<KeyType, ValueType>::find(const KeyType & key) const
{
	return find_helper(key, root);
}

template<typename KeyType, typename ValueType>
inline void MyMap<KeyType, ValueType>::destory_tree(node * leaf)
{
	if (leaf != nullptr)
	{
		destory_tree(leaf->left);
		destory_tree(leaf->right);
		delete leaf;
		m_size--;
		
	}
	return;
}

template<typename KeyType, typename ValueType>
inline int MyMap<KeyType, ValueType>::size_helper(node * point) const
{
	/*int count = 0;
	if (point != nullptr)
	{
		count++;
		count += size_helper(point->left);
		count += size_helper(point->right);
	}
	else
		return 0;
	return count;*/
	return m_size;
}

template<typename KeyType, typename ValueType>
inline ValueType * MyMap<KeyType, ValueType>::find_helper(const KeyType & key, node * leaf) const
{
	if (leaf != nullptr)
	{
		if (leaf->m_key < key) //if leaf's key is smaller than the key, go to right
			return find_helper(key, leaf->right);
		else if (leaf->m_key > key) //if leaf's key is larger than the key, go to left 
			return find_helper(key, leaf->left);
		else
		{
			ValueType* ptr;
			ptr = &leaf->m_value;
			return ptr;
		}
	}
	return nullptr;
}

template<typename KeyType, typename ValueType>
inline void MyMap<KeyType, ValueType>::associate_helper(const KeyType & key, const ValueType & value, node * leaf)
{

	if (leaf == nullptr)
	{
		//std::cerr << "aa" << std::endl;
		root = new node; 
		root->m_key = key;
		root->m_value = value; 
		root->right = nullptr;
		root->left = nullptr;
		m_size++;
		//root = newNode(key, value);
	}
	else if (key == leaf->m_key)
	{
		//std::cerr << "b" << std::endl;
		leaf->m_value = value;
	}
	else if (key < leaf->m_key)
	{
		//std::cerr << "c" << std::endl;
		if (leaf->left != nullptr)
			associate_helper(key, value, leaf->left);
		else
		{
			leaf->left = new node;
			leaf->left->m_key = key;
			leaf->left->m_value = value;
			leaf->left->left = nullptr;
			leaf->left->right = nullptr;
			m_size++;
		}
	}
	else if (key > leaf->m_key)
	{
		//std::cerr << "d" << std::endl;
		if (leaf->right != nullptr)
			associate_helper(key, value, leaf->right);
		else
		{
			leaf->right = new node;
			leaf->right->m_key = key;
			leaf->right->m_value = value;
			leaf->right->left = nullptr;
			leaf->right->right = nullptr;
			m_size++;
		}
	}
}
//
//template<typename KeyType, typename ValueType>
//typename MyMap<KeyType, ValueType>::node * MyMap<KeyType, ValueType>::newNode(const KeyType & key, const ValueType & value)
//{
//	node* k = new node;
//	k->m_key = key;
//	k->m_value = value;
//	k->left = nullptr;
//	k->right = nullptr;
//	return k;
//}



#endif MyMap_h


