#include <iostream>
#include <string>
#include <iomanip>
#include <Windows.h>
#include <fstream>

using namespace std;

enum Specialnist { Kn, In, Me, PhI, Tn };
enum Kurs { I = 1, II, III, IV, V, VI };
string specialnistStr[]{ "Комп'ютерні науки", "Інформатика" ,  "Математика та економіка" , "Фізика та інформатика" , "Трудове навчання" };

struct Student
{
	char prizv[50];
	Kurs kurs;
	Specialnist specialnist;
	int physichs;
	int Math;
	union
	{
		int programming;
		int chusel_methods;
		int pedagog;
	};
};

void CreateBIN(string& filename);
void PrintBIN(string& filename);

void SortBIN(string& filename);
Student fRead(fstream& f, const int i);
void fWrite(fstream& f, const int i, const Student n);
void fChange(fstream& f, const int i, const int j);

int BinSearch(string& filename, const string prizv, const Specialnist specialnist, const int physichs);

void IndexSort(string& fname, string& fnameidx);
void PrintIndexSorted(string& filename, string& filenameidx);
int fReadIndex(fstream& f, const int pos);
void fWriteIndex(fstream& f, const int pos, const int n);
bool Compare(fstream& f, fstream& f_idx, const int j, const int pos_value);

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	string fname;
	string indexFile = "index.bin";

	cout << "Введіть назву файлу: ";
	getline(cin, fname);

	Student student;

	int menuItem;
	int ispecialnist;
	Specialnist specialnist;
	string prizv;
	int found;
	int physichs;

	do {
		cout << endl << endl << endl;
		cout << "Виберіть дію:" << endl << endl;
		cout << " [1] - введення даних з клавіатури" << endl;
		cout << " [2] - вивід даних на екран" << endl;
		cout << " [3] - фізичне впорядкування даних" << endl;
		cout << " [4] - бінарний пошук студента за спеціальністю, прізвищем та оцінок" << endl;
		cout << " [5] - індексне впорядкування та вивід даних" << endl;
		cout << " [0] - вихід та завершення роботи програми" << endl;
		cout << "Введіть значення: "; cin >> menuItem;
		cout << endl << endl << endl;
		switch (menuItem)
		{
			case 1:
				CreateBIN(fname);
				break;
			case 2:
				PrintBIN(fname);
				break;
			case 3:
				SortBIN(fname);
				break;
			case 4:
				cout << "Введіть ключі пошуку:" << endl;
				cout << " Спеціальність ( 0 - Комп'ютерні науки , 1 - Інформатика, 2 - Математика та економіка, 3 - Фізика та інформатика, 4 - Трудове навчання): ";
				cin >> ispecialnist;
				specialnist = (Specialnist)ispecialnist;
				cin.get(); 
				cin.sync(); 
				cout << " прізвище: "; getline(cin, prizv);
				cout << " Оцінка з фізики: "; cin >> physichs;
				cout << endl;

				if ((found = BinSearch(fname, prizv, specialnist, physichs)) != -1)
					cout << "Знайдено студента в позиції " << found + 1 << endl;
				else
					cout << "Шуканого студента не знайдено" << endl;
				break;
			case 5:
				IndexSort(fname, indexFile);
				PrintIndexSorted(fname, indexFile);
				break;
			case 0:
				break;
			default:
				cout << "Ви ввели помилкове значення! "
					"Слід ввести число - номер вибраного пункту меню" << endl;
		}
	} while (menuItem != 0);

	return 0;
}

void CreateBIN(string& filename)
{
	Student student;
	int specialnist, kurs;
	int c;

	ofstream fout(filename, ios::binary);

	if (!fout.is_open()) {
		cout << "Помилка відкриття файлу!" << endl;
		return;
	}

	cout << "Введіть кількість студентів: "; cin >> c;

	do
	{
		cin.get();
		cout << "Прізвище: "; cin.getline(student.prizv,49);

		cout << "Курс: "; cin >> kurs;
		student.kurs = (Kurs)kurs;

		cout << "Спеціальність (0 - Комп'ютерні науки, 1 - Інформатика, 2 - Математика та економіка, 3 - Фізика та інформатика, 4 - Трудове навчання): ";
		cin >> specialnist;
		student.specialnist = (Specialnist)specialnist;

		cout << "Оцінка з фізики: ";
		cin >> student.physichs;

		cout << "Оцінка з математики: ";
		cin >> student.Math;

		switch (student.specialnist)
		{
		case Kn:
			cout << "Оцінка з програмування: ";
			cin >> student.programming;
			break;
		case In:
			cout << "Оцінка з чисельних методів: ";
			cin >> student.chusel_methods;
			break;
		case Me:
		case PhI:
		case Tn:
			cout << "Оцінка з педагогіки: ";
			cin >> student.pedagog;
			break;
		}
		cout << endl;

		fout.write((char*)&student, sizeof(student));
		
		c--;
	} while (c > 0);

	fout.close();
}

void PrintBIN(string& filename)
{
	ifstream fin(filename, ios::in | ios::binary);

	if (!fin) {
		cout << "Помилка відкритя файлу!" << endl;
		return;
	}

	cout << "===========================================================================================================================" << endl;
	cout << "| № | Прізвище    | Курс |      Спеціальність      | Фізика | Математика |  Програмування  | Чисельні методи | Педагогіка |" << endl;
	cout << "---------------------------------------------------------------------------------------------------------------------------" << endl;


	Student student;
	int i = 1;
	while (fin.read((char*)&student, sizeof(student)))
	{
		cout << "| " << setw(1) << right << i << " ";
		cout << "| " << setw(12) << left << student.prizv
			<< "| " << setw(4) << right << student.kurs << " "
			<< "| " << setw(24) << left << specialnistStr[student.specialnist]
			<< "| " << setw(7) << right << student.physichs
			<< "| " << setw(11) << right << student.Math;

		switch (student.specialnist)
		{
		case Kn:
			cout << "| " << setw(15) << student.programming << right << " | " << setw(18) << " | " << setw(12) << " |" << endl;
			break;
		case In:
			cout << "| " << setw(18) << " | " << setw(15) << student.chusel_methods << right << " | " << setw(12) << " |" << endl;
			break;
		case Me:
		case PhI:
		case Tn:
			cout << "| " << setw(18) << " | " << setw(18) << " | " << setw(10) << student.pedagog << right << " |" << endl;
			break;
		}

		i++;
	}
	cout << "===========================================================================================================================" << endl;
	cout << endl;

	fin.close();
}

void fWrite(fstream& f, const int i, const Student n)
{
	f.seekp(i * (long)sizeof(n)); // встановили вказівник
	f.write((char*)&n, sizeof(n)); // записали значення
}

Student fRead(fstream& f, const int i)
{
	Student n;
	f.seekg(i * (long)sizeof(n)); // встановили вказівник
	f.read((char*)&n, sizeof(n)); // прочитали значення
	return n;
}

void fChange(fstream& f, const int i, const int j)
{
	Student x = fRead(f, i);
	Student y = fRead(f, j);
	fWrite(f, i, y);
	fWrite(f, j, x);
}

void SortBIN(string& fname)
{ 
	fstream f(fname, ios::binary | ios::in | ios::out);
	Student a,b;

	f.seekg(0, ios::end);
	int size = f.tellg() / (long)sizeof(a);
	f.seekg(0, ios::beg);

	for (int i0 = 1; i0 < size; i0++)
		for (int i1 = 0; i1 < size - i0; i1++)
		{
			a = fRead(f, i1);
			b = fRead(f, i1 + 1);
			if ((a.physichs < b.physichs)
				||
				(a.physichs == b.physichs && a.specialnist > b.specialnist)
				||
				(a.physichs == b.physichs && a.specialnist == b.specialnist && a.prizv > b.prizv))
			{
				fChange(f, i1, i1 + 1);
			}
		}
	f.seekp(0, ios::end);
}

int BinSearch(string& filename, const string prizv, const Specialnist specialnist, const int physichs)
{
	fstream f(filename, ios::binary | ios::in);

	if (!f.is_open())
	{
		cout << "Помилка відкриття файлу: " << filename << "!" << endl;
		return -1;
	}

	f.seekg(0, ios::end);
	long N = (long)f.tellg() / (long)sizeof(Student);
	f.seekg(0, ios::beg);

	int L = 0, R = N - 1, m;

	do 
	{
		m = (L + R) / 2;
		Student p = fRead(f, m);
		if (p.physichs == physichs && p.specialnist == specialnist && p.prizv == prizv)
			return m;
		if ((p.physichs > physichs)
			||
			(p.physichs == physichs && p.specialnist < specialnist)
			||
			(p.physichs == physichs && p.specialnist == specialnist && p.prizv < prizv))
		{
			L = m + 1;
		}
		else
		{
			R = m - 1;
		}
	} while (L <= R);

	f.close();
	return -1;
}

int fReadIndex(fstream& f, const int pos)
{
	int n;
	f.seekg(pos * (long)sizeof(n));
	f.read((char*)&n, sizeof(n));
	return n;
}

void fWriteIndex(fstream& f, const int pos, const int n)
{
	f.seekp(pos * (long)sizeof(n));
	f.write((char*)&n, sizeof(n));
}

bool Compare(fstream& f, fstream& f_idx, const int j, const int pos_value)
{
	int pos1 = fReadIndex(f_idx, j);

	Student n1 = fRead(f, pos1);
	Student n_value = fRead(f, pos_value);

	bool result = false;

	if ((n1.physichs < n_value.physichs)
		||
		(n1.physichs == n_value.physichs && n1.specialnist > n_value.specialnist)
		||
		(n1.physichs == n_value.physichs && n1.specialnist == n_value.specialnist && n1.prizv > n_value.prizv))
	{
		result = true;
	}

	return result;
}

void IndexSort(string& fname, string& fnameidx)
{
	fstream f(fname, ios::binary | ios::in);
	if (!f.is_open())
	{
		cout << "Помилка відкриття файлу: " << fname << "!" << endl;
		return;
	}
	fstream f_idx_create(fnameidx, ios::binary | ios::out);
	f_idx_create.close();
	fstream f_idx(fnameidx, ios::binary | ios::in | ios::out);
	if (!f_idx.is_open())
	{
		cout << "Помилка відкриття файлу: " << fnameidx << "!" << endl;
		return;
	}

	f.seekg(0, ios::end);
	long N = (long)f.tellg() / (long)sizeof(Student);
	f.seekg(0, ios::beg);

	for (int i = 0; i < N; i++)
		f_idx.write((char*)&i, sizeof(i));

	int i, j, value;
	for (i = 1; i < N; i++)
	{
		value = fReadIndex(f_idx, i);

		for (j = i - 1;
			j >= 0 && Compare(f, f_idx, j, value);
			j--)
		{
			fWriteIndex(f_idx, j + 1, fReadIndex(f_idx, j));
		}
		fWriteIndex(f_idx, j + 1, value);
	}
	f.close();
	f_idx.close();

}

void PrintIndexSorted(string& filename, string& filenameidx)
{
	fstream f(filename, ios::binary | ios::in);
	if (!f.is_open())
	{
		cout << "Помилка відкриття файлу: " << filename << "!" << endl;
		return;
	}

	fstream indexF(filenameidx, ios::binary | ios::in);
	if (!indexF.is_open())
	{
		cout << "Помилка відкриття файлу: " << filenameidx << "!" << endl;
		return;
	}

	Student student;

	
	cout << "===========================================================================================================================" << endl;
	cout << "| № | Прізвище    | Курс |      Спеціальність      | Фізика | Математика |  Програмування  | Чисельні методи | Педагогіка |" << endl;
	cout << "---------------------------------------------------------------------------------------------------------------------------" << endl;

	int i = 1;
	int n = 0; 
	while (indexF.read((char*)&n, sizeof(n)))
	{
		student = fRead(f, n);

		cout << "| " << setw(1) << right << i << " ";
		cout << "| " << setw(12) << left << student.prizv
			<< "| " << setw(4) << right << student.kurs << " "
			<< "| " << setw(24) << left << specialnistStr[student.specialnist]
			<< "| " << setw(7) << right << student.physichs
			<< "| " << setw(11) << right << student.Math;

		switch (student.specialnist)
		{
		case Kn:
			cout << "| " << setw(15) << student.programming << right << " | " << setw(18) << " | " << setw(12) << " |" << endl;
			break;
		case In:
			cout << "| " << setw(18) << " | " << setw(15) << student.chusel_methods << right << " | " << setw(12) << " |" << endl;
			break;
		case Me:
		case PhI:
		case Tn:
			cout << "| " << setw(18) << " | " << setw(18) << " | " << setw(10) << student.pedagog << right << " |" << endl;
			break;
		}

		i++;
	}
	cout << "===========================================================================================================================" << endl;
	cout << endl;

	f.close();
	indexF.close();
}