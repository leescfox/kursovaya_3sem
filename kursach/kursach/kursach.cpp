#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <locale>
#include "timsort.h"
using namespace std;

struct element //Элемент очереди
{
	int V;
	struct element* next;
};

struct golova //Голова очереди
{
	struct element* verh;
};

void Kruskal(vector<triple> G); //Сам алгоритм Краскала
vector<vector<pair<int, int>>> Adjacency_list(vector<triple> G); //Список смежности
vector<vector<int>> Incidence_matrix(vector<triple> G); //Матрица инцидентности
vector<vector<int>> Adjacency_matrix(vector<triple> G); //Матрица смежности
vector<string> ListOfV(vector<triple> G); //Находит все вершины существующие в графе
int ourcase(bool check_V1, bool check_V2); //Вспомогательная к ListOfV
int Queue_pop(golova* Head); //Достаём из очереди
void Queue_push(int ind, golova* Head); //Добавляем в очередь
vector<pair<string, int>> DFS(vector<triple> G); //Обход в глубину
void DFS_Traverse(vector<vector<pair<int, int>>> L, vector<pair<string, int>>& Data, int i); //Вспомогательная для DFS
vector<pair<string, int>> BFS(vector<triple> G); //Обход в ширину
vector<triple> Input(); //Ввод данных

class DisjointSet //Система непересекающихся множеств
{
public:
	vector<vector<pair<string, string>>> DS{ 0, vector<pair<string, string>>(0, {"", ""}) };
	vector<int> Masses;
	pair<int, int> Pos;
	
	void MakeSet(DisjointSet& G, triple T) //Создаёт новое множество
	{
		G.DS.resize(G.DS.size() + 1);
		G.DS[G.DS.size() - 1].resize(1);
		if (T.V1.compare(T.V2) == 1)
			swap(T.V1, T.V2);
		G.DS[G.DS.size() - 1][0].first = T.V1;
		G.DS[G.DS.size() - 1][0].second = T.V2;
		G.Masses.resize(G.Masses.size() + 1);
		G.Masses[G.Masses.size() - 1] = T.mass;
	}

	void Unite(DisjointSet& G, int ind1, int ind2, triple T) //Объединяет два множества при помощи ребра между ними
	{
		int count1, count2, i;
		if (ind1 > ind2)
			swap(ind1, ind2);
		G.AddToSet(G, T, ind1);
		vector<pair<string, string>> V(G.DS[ind1].size() + G.DS[ind2].size());
		G.Masses[ind1] = G.Masses[ind1] + G.Masses[ind2]; 
		G.Masses.erase(G.Masses.begin() + ind2);
		count1 = count2 = 0;
		while (count1 < G.DS[ind1].size() && count2 < G.DS[ind2].size())
		{
			if (G.DS[ind1][count1].first.compare(G.DS[ind2][count2].first) == -1)
			{
				V[count1 + count2] = G.DS[ind1][count1];
				count1++;
			}
			else if (G.DS[ind1][count1].first.compare(G.DS[ind2][count2].first) == 1)
			{
				V[count1 + count2] = G.DS[ind2][count2];
				count2++;
			}
			else if (G.DS[ind1][count1].second.compare(G.DS[ind2][count2].second) == -1)
			{
				V[count1 + count2] = G.DS[ind1][count1];
				count1++;
			}
			else
			{
				V[count1 + count2] = G.DS[ind2][count2];
				count2++;
			}
		}
		while (count1 < G.DS[ind1].size())
		{
			V[count1 + count2] = G.DS[ind1][count1];
			count1++;
		}
		while (count2 < G.DS[ind2].size())
		{
			V[count1 + count2] = G.DS[ind2][count2];
			count2++;
		}
		G.DS[ind1] = V;
		G.DS.erase(G.DS.begin()+ind2);
	}

	void AddToSet(DisjointSet& G, triple T, int ind) //Добавляет элемент в непустое множество
	{
		int i,j;
		G.Masses[ind] = G.Masses[ind] + T.mass;
		if (T.V1.compare(T.V2) == 1)
			swap(T.V1, T.V2);
		G.DS[ind].resize(G.DS[ind].size() + 1);
		i = 0;
		while (G.DS[ind][i].first.compare(T.V1) == -1 && i < G.DS[ind].size() - 1)
		{
			i++;
		}
		while (G.DS[ind][i].first.compare(T.V1) == 0 && G.DS[ind][i].second.compare(T.V2) == -1 && i < G.DS[ind].size() - 1)
		{
			i++;
		}
		for (j = G.DS[ind].size() - 1; j > i; j--)
		{
			G.DS[ind][j] = G.DS[ind][j - 1];
		}
		G.DS[ind][i] = { T.V1, T.V2 };
	}

	void Find(DisjointSet& G, triple T) //Находит индексы множеств в которых расположены концы ребра Т. Если ребра нигде нет, то индекс = -1
	{
		int num1, num2, i, j;
		num1 = num2 = -1;
		for (i = 0; i < G.DS.size() && (num1 == -1 || num2 == -1); i++)
		{
			for (j = 0; j < G.DS[i].size() && (num1 == -1 || num2 == -1); j++)
			{
				if (T.V1 == G.DS[i][j].first || T.V1 == G.DS[i][j].second)
				{
					num1 = i;
				}
				if (T.V2 == G.DS[i][j].first || T.V2 == G.DS[i][j].second)
				{
					num2 = i;
				}
			}
		}
		G.Pos = { num1, num2 };
	}
};

void Kruskal(vector<triple> G)
{
	int i;
	timsort(G);
	vector<string> Vlist = ListOfV(G);
	DisjointSet K;
	for (i = 0; i < G.size(); i++)
	{
		K.Find(K, G[i]);
		if (K.Pos.first == -1 && K.Pos.second == -1)
		{
			K.MakeSet(K, G[i]);
		}
		else if (K.Pos.first == -1 && K.Pos.second != -1)
		{
			K.AddToSet(K, G[i], K.Pos.second);
		}
		else if (K.Pos.first != -1 && K.Pos.second == -1)
		{
			K.AddToSet(K, G[i], K.Pos.first);
		}
		else if (K.Pos.first != K.Pos.second)
		{
			K.Unite(K, K.Pos.first, K.Pos.second, G[i]);
		}
		if (K.DS[0].size() + 1 >= Vlist.size())
			break;
	}
	cout << "Наше минимальное остовное дерево:" << endl;
	for (i = 0; i < K.DS[0].size(); i++)
	{
		cout << K.DS[0][i].first << " " << K.DS[0][i].second << endl;
	}
	cout << K.Masses[0] << endl;
}

vector<vector<pair<int, int>>> Adjacency_list(vector<triple> G)
{
	int i, j, k, count, ind1, ind2;
	timsort(G);
	vector<string> Vlist = ListOfV(G);
	vector<vector<pair<int, int>>> G2(Vlist.size(), vector<pair<int, int>>(0, { -1,0 }));
	for (i = 0; i < G.size(); i++)
	{
		ind1 = -1;
		ind2 = -1;
		count = 0;
		for (j = 0; j < Vlist.size() && count < 2; j++)
		{
			if (G[i].V1 == Vlist[j])
			{
				ind1 = j;
				count++;
			}
			if (G[i].V2 == Vlist[j])
			{
				ind2 = j;
				count++;
			}
		}
		G2[ind1].resize(G2[ind1].size() + 1);
		G2[ind1][G2[ind1].size() - 1] = { ind2, G[i].mass };
		G2[ind2].resize(G2[ind2].size() + 1);
		G2[ind2][G2[ind2].size() - 1] = { ind1, G[i].mass };
	}
	/* //Вывод закомментировал чтобы он не выводился. Но расскоментив можно посмотреть какой вообще список формируется
	for (i = 0; i < G2.size(); i++)
	{
		cout << Vlist[i] << " -> ";
		for (j = 0; j < G2[i].size(); j++)
		{
			cout << "(" << G2[i][j].first << ", " << G2[i][j].second << ") ";
		}
		cout << endl;
	}
	cout << endl; */
	return G2;
}

vector<vector<int>> Incidence_matrix(vector<triple> G)
{
	int i, j, count;
	timsort(G);
	vector<string> Vlist = ListOfV(G);
	vector<vector<int>> G2(G.size(), vector<int>(Vlist.size(), 0));
	for (i = 0; i < G.size(); i++)
	{
		count = 0;
		for (j = 0; j < Vlist.size() && count < 2; j++)
		{
			if (G[i].V1 == Vlist[j] || G[i].V2 == Vlist[j])
			{
				G2[i][j] = G[i].mass;
				count++;
			}
		}
	}
	for (i = 0; i < Vlist.size(); i++)
		cout << Vlist[i] << " ";
	cout << endl;
	for (i = 0; i < G2.size(); i++)
	{
		for (j = 0; j < Vlist.size(); j++)
			cout << G2[i][j] << " ";
		cout << endl;
	}
	cout << endl;
	return G2;
}

vector<vector<int>> Adjacency_matrix(vector<triple> G)
{
	int i, j, k, check;
	timsort(G);
	vector<string> Vlist = ListOfV(G);
	vector<vector<int>> G2(Vlist.size(), vector<int>(Vlist.size(), -1));
	for (i = 0; i < G.size(); i++)
	{
		check = 0;
		for (j = 0; j < Vlist.size() && check == 0; j++)
		{
			if (Vlist[j] == G[i].V1 || Vlist[j] == G[i].V2)
			{
				for (k = j + 1; k < Vlist.size() && check == 0; k++)
				{
					if (Vlist[k] == G[i].V1 || Vlist[k] == G[i].V2)
					{
						check = 1;
						G2[j][k] = G2[k][j] = G[i].mass;
					}
				}
			}
		}
	}
	cout << "  ";
	for (i = 0; i < Vlist.size(); i++)
		cout << Vlist[i] << " ";
	cout << endl;
	for (i = 0; i < Vlist.size(); i++)
	{
		cout << Vlist[i] << " ";
		for (j = 0; j < Vlist.size(); j++)
		{
			if (G2[i][j] == -1)
				cout << "- ";
			else
				cout << G2[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
	return G2;
}

vector<string> ListOfV(vector<triple> G)
{
	int i, j;
	bool check_V1, check_V2;
	vector<string> Vlist(0, "");
	for (i = 0; i < G.size(); i++)
	{
		check_V1 = 0;
		check_V2 = 0;
		for (j = 0; j < Vlist.size() && (check_V1 * check_V2) == 0; j++)
		{
			if (Vlist[j] == G[i].V1)
				check_V1 = 1;
			if (Vlist[j] == G[i].V2)
				check_V2 = 1;
		}
		switch (ourcase(check_V1, check_V2))
		{
		case 1:
			Vlist.resize(Vlist.size() + 2);
			Vlist[Vlist.size() - 2] = G[i].V1;
			Vlist[Vlist.size() - 1] = G[i].V2;
			break;
		case 2:
			Vlist.resize(Vlist.size() + 1);
			Vlist[Vlist.size() - 1] = G[i].V1;
			break;
		case 3:
			Vlist.resize(Vlist.size() + 1);
			Vlist[Vlist.size() - 1] = G[i].V2;
			break;
		case 4:
			break;
		}
	}
	return Vlist;
}

int ourcase(bool check_V1, bool check_V2)
{
	if (check_V1 == 0 && check_V2 == 0)
		return 1;
	if (check_V1 == 0 && check_V2 == 1)
		return 2;
	if (check_V1 == 1 && check_V2 == 0)
		return 3;
	if (check_V1 == 1 && check_V2 == 1)
		return 4;
}

int Queue_pop(golova* Head)
{
	int num;
	element* T = Head->verh;
	if (T->next != NULL)
	{
		while (T->next->next != NULL)
		{
			T = T->next;
		}
		num = T->next->V;
		delete T->next;
		T->next = NULL;
	}
	else
	{
		num = T->V;
		delete T;
		Head->verh = NULL;
	}
	return num;
}

void Queue_push(int ind, golova* Head)
{
	element* elem = new element;
	elem->V = ind;
	if (Head->verh == NULL)
	{
		Head->verh = elem;
		elem->next = NULL;
	}
	else
	{
		elem->next = Head->verh;
		Head->verh = elem;
	}
}

vector<pair<string, int>> DFS(vector<triple> G)
{
	int i;
	timsort(G);
	vector<string> Vlist = ListOfV(G);
	vector<vector<pair<int, int>>> L = Adjacency_list(G);
	vector<pair<string, int>> Data(Vlist.size(), { "White", -1 });
	for (i = 0; i < Vlist.size(); i++)
	{
		if (Data[i].first == "White")
			DFS_Traverse(L, Data, i);
	}
	cout << "Обход в глубину:" << endl;
	for (i = 0; i < Vlist.size(); i++)
	{
		if (Data[i].second != -1)
			cout << "Вершина: " << Vlist[i] << ". Её родитель: " << Vlist[Data[i].second] << endl;
		else
			cout << "Вершина: " << Vlist[i] << ". У неё нет родителей." << endl;
	}
	cout << endl;
	return Data;
}

void DFS_Traverse(vector<vector<pair<int, int>>> L, vector<pair<string, int>>& Data, int i)
{
	int j;
	Data[i].first = "Grey";
	for (j = 0; j < L[i].size(); j++)
	{
		if (Data[L[i][j].first].first == "White")
		{
			Data[L[i][j].first].second = i;
			DFS_Traverse(L, Data, L[i][j].first);
		}
	}
	Data[i].first = "Black";
}

vector<pair<string, int>> BFS(vector<triple> G)
{
	int i, num;
	timsort(G);
	vector<string> Vlist = ListOfV(G);
	vector<vector<pair<int, int>>> L = Adjacency_list(G);
	vector<pair<string, int>> Data(Vlist.size(), { "White", -1 });
	struct golova* Head = new struct golova;
	Head->verh = NULL;
	Data[0].first = "Grey";
	Queue_push(0, Head);
	while (Head->verh != NULL)
	{
		num = Queue_pop(Head);
		for (i = 0; i < L[num].size(); i++)
		{
			if (Data[L[num][i].first].first == "White")
			{
				Data[L[num][i].first].second = num;
				Data[L[num][i].first].first = "Grey";
				Queue_push(L[num][i].first, Head);
			}
		}
		Data[num].first = "Black";
	}
	cout << "Обход в ширину:" << endl;
	for (i = 0; i < Vlist.size(); i++)
	{
		if (Data[i].second != -1)
			cout << "Вершина: " << Vlist[i] << ". Её родитель: " << Vlist[Data[i].second] << endl;
		else
			cout << "Вершина: " << Vlist[i] << ". У неё нет родителей." << endl;
	}
	cout << endl;
	return Data;
}

vector<triple> Input()
{
	int i, num;
	string temp;
	vector <triple> G(0);
	cout << "Введите '1', чтобы ввести данные с клавиатуры. Введите любое другое число, чтобы ввести данные из файла: ";
	cin >> num;
	cin.ignore(32767, '\n');
	if (num == 1)
	{
		cout << "Вводите данные. Чтобы остановить ввод, введите Stop:" << endl;
		for (i = 0; true; i++)
		{
			getline(cin, temp);
			if (temp == "Stop" || temp == "STOP" || temp == "stop")
				break;
			G.resize(G.size() + 1);
			G[i].V1 = temp.substr(0, temp.find(" "));
			temp.erase(0, temp.find(" ") + 1);
			G[i].V2 = temp.substr(0, temp.find(" "));
			temp.erase(0, temp.find(" ") + 1);
			G[i].mass = stoi(temp);
		}
	}
	else
	{
		ifstream File;
		cout << "Введите путь к файлу: ";
		getline(cin, temp);
		File.open(temp);
		if (File)
		{
			i = 0;
			while (!File.eof())
			{
				G.resize(G.size() + 1);
				File >> G[i].V1 >> G[i].V2 >> G[i].mass;
				i++;
			}
		}
		else
		{
			cout << "Не удалось открыть файл." << endl;
		}
	}
	return G;
}

int main() //Программа работает с корректным вводом без петель.
{
	setlocale(LC_ALL, "rus");
	vector<triple> G = Input();
	if (G.size() > 0)
	{
		Kruskal(G);
	}
	return 0;
}