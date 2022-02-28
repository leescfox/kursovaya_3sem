using namespace std;

struct triple {
	int mass;
	string V1;
	string V2;
};

struct stck
{
	int left;
	int size;
	struct stck* next;
};
struct head
{
	struct stck* top;
};

void push(head* HEAD, int lev, int s)
{
	stck* stack = new stck;
	stack->left = lev;
	stack->size = s;
	stack->next = HEAD->top;
	HEAD->top = stack;
}

void pop(stck* elem, head* HEAD)
{
	stck* p = HEAD->top;
	if (elem == HEAD->top)
	{
		HEAD->top = elem->next;
		delete elem;
	}
	else if (elem->next == NULL)
	{
		while (p->next != elem)
			p = p->next;
		delete elem;
		p->next = NULL;
	}
	else
	{
		while (p->next != elem)
			p = p->next;
		p->next = elem->next;
		delete elem;
	}
}

int getminrun(int k)
{
	int m = 0;
	while (k >= 64)
	{
		m = m | (k & 1);
		k = k >> 1;
	}
	return (k + m);
}

void InsertionSort(vector<struct triple>& A, int left, int right)
{
	int i, j;
	struct triple value;
	for (i = left + 1; i < right; i++)
	{
		value = A[i];
		for (j = i; j > left && A[j - 1].mass > value.mass; j--)
		{
			A[j] = A[j - 1];
		}
		A[j] = value;
	}
}

int binary_search(vector<struct triple>& temporary, int begin, int end, int it)
{
	int left, right;
	left = begin;
	right = end;
	while (left != right)
	{
		if (temporary[(left + right) / 2].mass <= it && temporary[(left + right) / 2 + 1].mass > it)
			return ((left + right) / 2 + 1);
		if (temporary[(left + right) / 2].mass <= it)
		{
			left = (left + right) / 2 + 1;
		}
		else right = (left + right) / 2;
	}
	return right;
}

void merge(vector<struct triple>& A, int left1, int size1, int left2, int size2) //fix copy!!!
{
	int i, count1, count2, in1, in2, num;
	vector<struct triple> temporary(size1);
	copy(&A[left1], &A[left1 + size1], temporary.begin());
	count1 = 0;
	count2 = 0;
	in1 = 0;
	in2 = 0;
	while (count1 < size1 && count2 < size2)
	{

		if (A[left2 + count2].mass < temporary[count1].mass)
		{
			A[left1 + count1 + count2] = A[left2 + count2];
			in2++;
			in1 = 0;
		}
		else
		{
			A[left1 + count1 + count2] = temporary[count1];
			in1++;
			in2 = 0;
		}

		if (in1 >= 7 && count1 < size1)
		{
			if (count1 == size1 - 1)
				break;
			i = 1;
			while (true)
			{
				if (count1 + i >= size1)
				{
					count1++;
					copy(&temporary[count1], &temporary[count1 - 1 + i / 2], &A[left1 + count1 + count2]);
					count1 = count1 - 1 + i / 2;
					A[left1 + count1 + count2] = temporary[count1];
					count1++;
					if (count1 < size1)
					{
						if (temporary[size1 - 1].mass < A[left2 + count2].mass)
						{
							copy(&temporary[count1], &temporary[size1 - 1], &A[left1 + count1 + count2]);
							count1 = size1 - 1;
							A[left1 + count1 + count2] = temporary[count1];
							count1 = size1;
							break;
						}
						else
						{
							num = binary_search(temporary, count1, size1 - 1, A[left2 + count2].mass);
							copy(&temporary[count1], &temporary[num], &A[left1 + count1 + count2]);
							count1 = num;
							in1 = 0;
							in2 = 1;
							break;
						}
					}
					else break;
				}
				if (temporary[count1 + i].mass < A[left2 + count2].mass)
					i = i * 2;
				else if (i != 1)
				{
					num = binary_search(temporary, count1 + i / 2, count1 + i, A[left2 + count2].mass);
					count1++;
					copy(&temporary[count1], &temporary[num], &A[left1 + count1 + count2]);
					count1 = num;
					in1 = 0;
					in2 = 0;
					break;
				}
				else
				{
					count1++;
					A[left1 + count1 + count2] = A[left2 + count2];
					count2++;
					in1 = 0;
					in2 = 1;
					break;
				}
			}
		}
		else if (in1 > 0)
			count1++;

		else if (in2 >= 7 && count2 < size2)
		{
			if (count2 == size2 - 1)
				break;
			i = 1;
			while (true)
			{
				if (count2 + i >= size2)
				{
					count2++;
					copy(&A[left2 + count2], &A[left2 + count2 - 1 + i / 2], &A[left1 + count1 + count2]);
					count2 = count2 - 1 + i / 2;
					A[left1 + count1 + count2] = A[left2 + count2];
					count2++;
					if (count2 < size2)
					{
						if (A[left2 + size2 - 1].mass < temporary[count1].mass)
						{
							copy(&A[left2 + count2], &A[left2 + size2 - 1], &A[left1 + count1 + count2]);
							count2 = size2 - 1;
							A[left1 + count1 + count2] = A[left2 + count2];
							count2 = size2;
							break;
						}
						else
						{
							num = binary_search(A, left2 + count2, left2 + size2 - 1, temporary[count1].mass);
							copy(&A[left2 + count2], &A[num], &A[left1 + count1 + count2]);
							count2 = num - left2;
							in1 = 1;
							in2 = 0;
							break;
						}
					}
					else break;
				}
				if (A[left2 + count2 + i].mass < temporary[count1].mass)
					i = i * 2;
				else if (i != 1)
				{
					num = binary_search(A, left2 + count2 + i / 2, left2 + count2 + i, temporary[count1].mass);
					count2++;
					copy(&A[left2 + count2], &A[num], &A[left1 + count1 + count2]);
					count2 = num - left2;
					in1 = 0;
					in2 = 0;
					break;
				}
				else
				{
					count2++;
					A[left1 + count1 + count2] = temporary[count1];
					count1++;
					in1 = 1;
					in2 = 0;
					break;
				}
			}
		}
		else if (in2 > 0)
			count2++;
	}

	if (count1 < size1)
	{
		copy(&temporary[count1], &temporary[size1 - 1], &A[left1 + count1 + count2]);
		count1 = size1 - 1;
		A[left1 + count1 + count2] = temporary[count1];
	}
	else
	{
		copy(&A[left2 + count2], &A[left2 + size2 - 1], &A[left1 + count1 + count2]);
		count2 = size2 - 1;
		A[left1 + count1 + count2] = A[left2 + count2];
	}
}

vector<struct triple> timsort(vector<struct triple>& A)
{
	unsigned int i, m;
	int j, minrun, left;
	int stacksize = 0;
	head* HEAD = new head;
	HEAD->top = NULL;
	minrun = getminrun(A.size());
	left = 0;
	i = 0;
	while (i < A.size())
	{
		i++;
		j = 2;
		if (i < A.size())
		{
			if (A[i].mass >= A[i - 1].mass)
			{
				while (A[i].mass >= A[i - 1].mass)
				{
					j++;
					i++;
					if (i >= A.size())
						break;
				}
			}
			else
			{
				while (A[i].mass < A[i - 1].mass && i < A.size())
				{
					j++;
					i++;
					if (i >= A.size())
						break;
				}
				for (m = left; m < (i - left - 1) / 2; m++)
				{
					swap(A[left], A[i - left - 1]);
				}
			}
			if (j < minrun)
			{
				if (i + minrun - j <= A.size())
					i = i + minrun - j;
				else
					i = A.size();
			}
		}
		InsertionSort(A, left, i);
		push(HEAD, left, i - left);
		stacksize++;
		if (stacksize > 2)
		{
			while (HEAD->top->next->next->size <= HEAD->top->next->size + HEAD->top->size || HEAD->top->next->size <= HEAD->top->size)
			{
				if (HEAD->top->size < HEAD->top->next->next->size)
				{
					merge(A, HEAD->top->next->left, HEAD->top->next->size, HEAD->top->left, HEAD->top->size);
					HEAD->top->next->size = HEAD->top->size + HEAD->top->next->size;
					pop(HEAD->top, HEAD);
				}
				else
				{
					merge(A, HEAD->top->next->next->left, HEAD->top->next->next->size, HEAD->top->next->left, HEAD->top->next->size);
					HEAD->top->next->next->size = HEAD->top->next->size + HEAD->top->next->next->size;
					pop(HEAD->top->next, HEAD);
				}

				stacksize = stacksize - 1;
				if (stacksize < 3)
					break;
			}
		}
		left = i;
	}
	while (stacksize > 2)
	{
		if (HEAD->top->size < HEAD->top->next->next->size)
		{
			merge(A, HEAD->top->next->left, HEAD->top->next->size, HEAD->top->left, HEAD->top->size);
			HEAD->top->next->size = HEAD->top->size + HEAD->top->next->size;
			pop(HEAD->top, HEAD);
		}
		else
		{
			merge(A, HEAD->top->next->next->left, HEAD->top->next->next->size, HEAD->top->next->left, HEAD->top->next->size);
			HEAD->top->next->next->size = HEAD->top->next->size + HEAD->top->next->next->size;
			pop(HEAD->top->next, HEAD);
		}
		stacksize = stacksize - 1;
	}
	if (stacksize > 1)
	{
		merge(A, HEAD->top->next->left, HEAD->top->next->size, HEAD->top->left, HEAD->top->size);
		pop(HEAD->top, HEAD);
	}
	if (HEAD->top)
	{
		pop(HEAD->top, HEAD);
		/*cout << "Sorted array:\n";
		for (i = 0; i < A.size(); i++)
			cout << A[i].mass << " " << A[i].V1 << " " << A[i].V2 << endl;
		cout << endl; */
	}
	//else cout << "You didn't enter any elements" << endl;
	return A;
}

