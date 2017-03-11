// MyMap.h

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.

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
		KeyType key;
		ValueType value; 
		node* left; //smaller 
		node* right; //larger
	};
	node* root; 
	void destory_tree(node* leaf);
	int size_helper(node* point);
	ValueType* find_helper(const KeyType& key, node*leaf);
	void associate_helper(const KeyType & key, const ValueType & value, node* leaf);
};

template<typename KeyType, typename ValueType>
inline MyMap<KeyType, ValueType>::MyMap()
{
	root = nullptr; 
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
}

template<typename KeyType, typename ValueType>
inline int MyMap<KeyType, ValueType>::size() const
{
	return size_helper(root);
}

template<typename KeyType, typename ValueType>
inline void MyMap<KeyType, ValueType>::associate(const KeyType & key, const ValueType & value)
{
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
		destory_tree(leaft->right);
		delete leaf;
	}
	return; 
}

template<typename KeyType, typename ValueType>
inline int MyMap<KeyType, ValueType>::size_helper(node * point)
{
	int count = 0;
	if (point != nullptr)
	{
		count += size_helper(point->left);
		count += size_helper(point->right);
		count++;
	}
	return count; 
}

template<typename KeyType, typename ValueType>
inline ValueType * MyMap<KeyType, ValueType>::find_helper(const KeyType & key, node * leaf)
{
	if (leaf != nullptr)
	{
		if (leaf->key < key) //if leaf's key is smaller than the key, go to right
			return find_helper(key, leaf->right);
		else if (leaf->key > key) //if leaf's key is larger than the key, go to left 
			return find_helper(key, leaf->left);
		else
			return leaf->value; 
	}
	return nullptr;
}

template<typename KeyType, typename ValueType>
inline void MyMap<KeyType, ValueType>::associate_helper(const KeyType & key, const ValueType & value, node * leaf)
{
	if (find_helper(key, leaf) != nullptr) //the key exists 
		find_helper(key, leaf)->value = value;  //replace existing value 
	else
	{
		if (key < leaf->key)
		{
			if (leaf->left != nullptr)
				associate_helper(key, value, leaf->left);
			else
			{
				leaf->left = new node;
				leaf->left->key = key;
				leaf->left->value = value; 
				leaf->left->left = nullptr;
				leaf->left->right = nullptr;
			}
		}
		else if (key > leaf->key)
		{
			if (leaf->right != nullptr)
				associate_helper(key, value, leaf->right);
			else
			{
				leaf->right = new node; 
				leaf->right->key = key;
				leaf->right->value = value; 
				leaf->right->left = nullptr; 
				leaf->right->right = nullptr;
			}
		}
	}
}

