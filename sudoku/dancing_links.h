#pragma once

template<size_t C, size_t R>
class DLX
{
private:
	int sum[C], ans[R];
	struct Node
	{
		int r, c;
		Node *left, *right, *up, *down;
	} *col[C], pool[C * R], *alloc;

	Node* new_Node(int r, int c)
	{
		Node* ret = alloc++;
		ret->r = r, ret->c = c;
		ret->left = ret->right = ret->down = ret->up = nullptr;
		return ret;
	}

	void cover(int c)
	{//覆盖
		col[c]->left->right = col[c]->right;
		col[c]->right->left = col[c]->left;
		for (auto i = col[c]->down; i != col[c]; i = i->down)
			for (auto j = i->right; j != i; j = j->right)
			{
				j->up->down = j->down;
				j->down->up = j->up;
				sum[j->c]--;
			}
	}

	void recover(int c)
	{//恢复
		for (auto i = col[c]->up; i != col[c]; i = i->up)
			for (auto j = i->left; j != i; j = j->left)
			{
				j->up->down = j->down->up = j;
				sum[j->c]++;
			}
		col[c]->left->right = col[c]->right->left = col[c];
	}

	bool dfs(int d)
	{//递归回溯
		if (col[0]->right == col[0])
		{
			ans[0] = d;
			return true;
		}
		int c = col[0]->right->c;
		for (auto i = col[0]->right; i != col[0]; i = i->right)
			if (sum[i->c] < sum[c]) c = i->c;
		cover(c);
		for (auto i = col[c]->down; i != col[c]; i = i->down)
		{
			ans[d] = i->r;
			for (auto j = i->right; j != i; j = j->right)
				cover(j->c);
			if (dfs(d + 1)) return true;
			for (auto j = i->left; j != i; j = j->left)
				recover(j->c);
		}
		recover(c);
		return false;
	}

public:
	void clear()
	{
		alloc = pool;
		memset(sum, 0, sizeof sum);
		for (int i = 0; i < C; i++)
			col[i] = new_Node(0, i);
		for (int i = 0; i < C; i++)
		{
			col[i]->up = col[i]->down = col[i];
			col[i]->left = col[(i + C - 1) % C];
			col[i]->right = col[(i + 1) % C];
		}
	}

	template<typename IteratorT>
	void add_row(int r, IteratorT first, IteratorT last)
	{//在十字链表中添加整行
		if (std::distance(first, last) == 0) return;
		Node *head = new_Node(r, *first), *prev = head;
		head->up = col[*first]->up;
		head->down = col[*first];
		head->up->down = head->down->up = head;
		sum[*first]++;
		for (auto i = std::next(first); i != last; ++i)
		{
			Node* curr = new_Node(r, *i);
			curr->up = col[*i]->up;
			curr->down = col[*i];
			curr->up->down = curr->down->up = curr;
			curr->left = prev;
			prev->right = curr;
			sum[*i]++;
			prev = curr;
		}
		head->left = prev;
		prev->right = head;
	}

	bool get_pos(std::vector<int>& res)
	{
		if (!dfs(1)) return false;
		for (int i = 1; i < ans[0]; i++)
			res.push_back(ans[i]);
		return true;
	}
};
