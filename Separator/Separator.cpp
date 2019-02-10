// Separator.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h" // Пока пустой заголовочный файл
#include <iostream> // Консольный ввод-вывод
#include <fstream> // Ввод в файл, вывод из файла
#include <string> // Работа со строками
#include <regex> // Регулярные выражения
#include <filesystem> // Работа с файлами, например копирование

using namespace std;
namespace fs = experimental::filesystem;

// Вывести вектор
void v_print(vector<string>& vec)
{
	for (int i = 0; i < vec.size(); i++)
		cout << vec[i] << endl;
}

cmatch separate_inp_name(string& inp_name)
{
	cmatch separated;
	regex regular("(_)([\\d]+)(_)([\\w\\.]+)"); // (_)(любые цифры; от 1 символа)(_)(люые буквы и цифры, а также _ и точка; от 1 символа)
	if (regex_match(inp_name.c_str(), separated, regular))
		return separated;
}

int separate(string& inp_name)
{
	string readout; // Строка из считываемого файла name
	string search = "q_ve"; // Подстрока в readout, с которой сверяемся при отделении данных
	int number = 0; // Порядковый номер данных с едиными начальными условиями
	int flag = 0; // Флаг, уведомляющий о начале нового блока данных
	int info_strings = 2; // Кол-во строк с информацией о начальных условиях для каждого блока данных

	cmatch separated_inp_name = separate_inp_name(inp_name);
	string file_name = separated_inp_name[4];
	string dir_name = separated_inp_name[2];
	if (file_name != "Q_l.dat")
	{
		cout << inp_name << " has inappropriate name" << endl;
		return 1;
	}
	string out_name = dir_name + "_" + to_string(number) + "_" + file_name;

	ifstream readFile(inp_name);
	ofstream outFile(out_name);
	
	cout << "I've started to separate " << inp_name << endl;

	while (getline(readFile, readout))
	{
		if (readout.substr(0, search.length()) == search)
			outFile << readout << endl;
		else
		{
			if (flag == info_strings) // Новый блок данных
			{
				outFile.close();
				number += 1;
				flag = 1;
				out_name = dir_name + "_" + to_string(number) + "_" + file_name;
				outFile.open(out_name);
				outFile << readout << endl;
			}
			else
			{
				outFile << readout << endl;
				flag++;
			}
		}
	}

	readFile.close();
	outFile.close();
	fs::remove(inp_name);
	return 0;
}

int main()
{
	vector<string> files; // Вектор строк - файлов
	string s; // Вспомогательная переменная при считывании объектов в папке
	size_t found; // Указатель на найденную подстроку
	string inp_name;

	// Смотрим, какие файлы и папки внутри указанной директории
	for (const auto & p : fs::directory_iterator("./"))
	{
		s = p.path().string(); // Путь к объекту, преобразованный в строку
		if (!is_directory(p)) // Если файл (не папка)
		{
			// Выделяем имя файла
			found = s.find_last_of("/\\");
			inp_name = s.substr(found + 1);

			found = inp_name.find_last_of("."); // Номер символа, с которого начинается формат файла

			if (inp_name.substr(found) == ".dat") // Если формат .dat
				files.push_back(inp_name); // Добавить в конец вектора строку - имя файла с расширением
		}
	}

	cout << "I've found:" << endl;
	v_print(files);
	cout << endl;

	for (int i = 0; i < files.size(); i++)
		separate(files[i]);

	cout << "I've finished all my work" << endl;
	system("pause");
}
