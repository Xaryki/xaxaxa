#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include <commctrl.h>
#include <commdlg.h>
#include <vector>
#include "gplot++.h"
#include "stat_lab.cpp"
#include "boost.h"
#include <iostream>
#include <cmath>
#include <boost/math/distributions/chi_squared.hpp>
#include <boost/random.hpp>
#include <sstream>


//#pragma comment(lib,"ComCtl32.lib")

using namespace std;

boost::random::mt19937 gen; // Declare boost random generator globally
boost::random::normal_distribution<> dist(0.0, 1.0); // Declare boost normal distribution globally

// Declare the bartlett_test function
pair<double, double> bartlett_test(const vector<vector<double>>& samples);


bool printfile(string, HWND);
void GnuGraph(int nc, string Xname, string Yname, int* m, int* gtype, string* gtext, vector<double>x, vector<double>y);
void Graph(string);



TCHAR czClassName[] = "myClass";
TCHAR czFormName[] = "Borisenko I.V., Nikita K.I.";
HWND hwndA;
HWND hlistbox;
HWND hlistbox1;

//###############################################################################
std::vector<std::vector<double>> readDataFromFile(const std::string& filename) {
    std::vector<std::vector<double>> data;

    std::ifstream file(filename);
    if (!file.is_open()) {
        // Обработка ошибки при открытии файла
        MessageBox(NULL, "Failed to open file!", "Error", MB_OK | MB_ICONERROR);
        return data;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<double> row;
        std::istringstream iss(line);
        double value;
        while (iss >> value) {
            row.push_back(value);
        }
        data.push_back(row);
    }

    file.close();
    return data;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static HWND hListBox;
    static HWND hlistbox1;
    switch (message) {
    case WM_CREATE:
    {
        // Создание ListBox
        hlistbox1 = CreateWindow("LISTBOX", "", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL,
            10, 70, 400, 300, hwnd, NULL, NULL, NULL);


        break;
    }


    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case 2: // Кнопка "Test"
            if (HIWORD(wParam) == BN_CLICKED) {
                // Очистка ListBox перед выводом новых результатов
                SendMessage(hlistbox1, LB_RESETCONTENT, 0, 0);
                // Считывание данных из файла и вывод в листбокс
                ifstream inputFile("input.txt");
                string line;
                if (inputFile.is_open()) {
                    while (getline(inputFile, line)) {
                        SendMessage(hlistbox1, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(line.c_str()));
                    }
                    inputFile.close();
                }
                else {
                    MessageBox(hwnd, "Failed to open input.txt", "Error", MB_OK | MB_ICONERROR);
                }

                // Очистка ListBox перед выводом новых результатов
                SendMessage(hlistbox, LB_RESETCONTENT, 0, 0);

                // Получаем количество строк в ListBox1
                int itemCount = SendMessage(hlistbox1, LB_GETCOUNT, 0, 0);

                // Вектор для хранения данных из ListBox1
                vector<vector<double>> data;


                // Считываем данные из ListBox1
                char buffer[256]; // Буфер для хранения строки
                for (int i = 0; i < itemCount; ++i) {
                    // Получаем длину строки
                    int len = SendMessage(hlistbox1, LB_GETTEXTLEN, i, 0);
                    // Получаем текст строки из ListBox1
                    SendMessage(hlistbox1, LB_GETTEXT, i, reinterpret_cast<LPARAM>(buffer));
                    // Добавляем нуль-терминатор в конец строки
                    buffer[len] = '\0';
                    // Преобразуем строку в вектор чисел
                    vector<double> row;
                    char* token = strtok(buffer, " ");
                    while (token != NULL) {
                        row.push_back(atof(token));
                        token = strtok(NULL, " ");
                    }
                    // Добавляем вектор чисел в общий вектор данных
                    data.push_back(row);
                }

                // Выполнение Bartlett's test или других операций с данными
                if (!data.empty()) {
                    // Выполнение Bartlett's test
                    auto result = bartlett_test(data);

                    // Формирование строки с результатом
                    string result_message = "X2: " + to_string(result.first) + ", Critical X2: " + to_string(result.second);

                    // Вывод результата в ListBox
                    SendMessage(hlistbox, LB_ADDSTRING, 0, (LPARAM)result_message.c_str());
                }
                else {
                    // Обработка ошибки, если ListBox1 пуст
                    MessageBox(hwnd, "The ListBox is empty!", "Error", MB_OK | MB_ICONERROR);
                }
            }
            break;
        case 3: // Кнопка "Save"
            if (HIWORD(wParam) == BN_CLICKED) {
                // Получаем количество строк в ListBox
                int itemCount = SendMessage(hlistbox, LB_GETCOUNT, 0, 0);
                if (itemCount == LB_ERR) {
                    // Обработка ошибки
                    break;
                }

                // Создаем буфер для хранения текста
                const int BUFFER_SIZE = 4096;
                TCHAR buffer[BUFFER_SIZE];

                // Открываем файл для записи
                ofstream outfile("output.txt");
                if (!outfile.is_open()) {
                    // Обработка ошибки
                    break;
                }

                // Считываем текст из ListBox и записываем его в файл
                for (int i = 0; i < itemCount; ++i) {
                    SendMessage(hlistbox, LB_GETTEXT, i, (LPARAM)buffer);
                    outfile << buffer << endl;
                }

                // Закрываем файл
                outfile.close();

                MessageBox(hwnd, "Result saved to output.txt", "Success", MB_OK | MB_ICONINFORMATION);
            }
            break;
        default:
            break;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}



// Define the bartlett_test function
pair<double, double> bartlett_test(const vector<vector<double>>& samples) {
    // Number of samples
    int m = samples.size();

    // Total number of observations
    int N = 0;
    for (const auto& s : samples) {
        N += s.size();
    }

    // Sample variances
    vector<double> s_squared;
    for (const auto& s : samples) {
        double mean = 0.0;
        for (double x : s) {
            mean += x;
        }
        mean /= s.size();

        double variance = 0.0;
        for (double x : s) {
            variance += pow(x - mean, 2);
        }
        variance /= (s.size() - 1);

        s_squared.push_back(variance);
    }

    // Geometric mean of sample variances
    double s_squared_geom_mean = 0.0;
    for (double s : s_squared) {
        s_squared_geom_mean += log(s);
    }
    s_squared_geom_mean = exp(s_squared_geom_mean / m);


    // Degrees of freedom for each sample
    vector<int> n;
    for (const auto& s : samples) {
        n.push_back(s.size() - 1);
    }

    // Calculation of c
    double c_num = 0.0;
    for (int i : n) {
        c_num += 1.0 / (i - 1);
    }
    double c_denom = 1.0 / (N - m);
    double c = 1 + (1.0 / (3 * (m - 1))) * (c_num - c_denom);

    // Calculation of Bartlett's criterion
    double chi_squared = 0.0;
    for (int i = 0; i < m; i++) {
        chi_squared += n[i] * s_squared[i];
    }
    chi_squared = (1.0 / c) * (N - m) * log(chi_squared / (N - m) / s_squared_geom_mean);

    // Critical value from chi-squared distribution
    boost::math::chi_squared_distribution<> dist(m - 1);
    double chi_squared_critical = boost::math::quantile(dist, 0.95);

    return make_pair(chi_squared, chi_squared_critical);
}

//############################################################################

ATOM RegisterClass(HINSTANCE hInst, int cmdMode) {

    WNDCLASS cw;
    cw.style = CS_VREDRAW | CS_HREDRAW;
    cw.cbClsExtra = 0;
    cw.cbWndExtra = 0;
    cw.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
    cw.hCursor = LoadCursor(NULL, IDC_ARROW);
    cw.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    cw.hInstance = hInst;
    cw.lpfnWndProc = WndProc;
    cw.lpszClassName = czClassName;
    cw.lpszMenuName = NULL;
    RegisterClass(&cw);
    return 0;
}

//################################################################################ 
BOOL InitInstance(HINSTANCE hInst, int Mode) {

    hwndA = CreateWindowA(czClassName, czFormName, WS_OVERLAPPEDWINDOW, 10, 15, 900, 900, NULL, NULL, hInst, NULL);
    hlistbox = CreateWindow("LISTBOX", "Out", WS_VISIBLE | WS_CHILD | WS_HSCROLL | WS_VSCROLL | WS_BORDER, 10, 400, 535, 150, hwndA, HMENU(77), NULL, NULL);


    HWND button = CreateWindow("button", "Solve", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 15, 100, 50, hwndA, HMENU(2), hInst, NULL);  // Создаем кнопку для старта
    HWND saveButton = CreateWindow("button", "Save", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 150, 15, 100, 50, hwndA, HMENU(3), hInst, NULL);  // Создаем кнопку для сохранения


    ShowWindow(hwndA, Mode);
    UpdateWindow(hwndA);
    return TRUE;
}
//##################################################################################### 

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR IpszCmdParam, int cmdShow) {
    MSG msg;

    RegisterClass(hInst, cmdShow);
    InitInstance(hInst, cmdShow);

    HMENU MainMenu = CreateMenu();
    HMENU hm1 = CreatePopupMenu();
    HMENU hm2 = CreatePopupMenu();
    HMENU hm3 = CreatePopupMenu();
    HMENU hm4 = CreatePopupMenu();
    HMENU hm5 = CreatePopupMenu();

    //########################################################################################

    AppendMenu(MainMenu, MF_STRING | MF_POPUP, UINT_PTR(hm1), "&File"); {
        AppendMenu(hm1, MF_STRING, 1, "Quit (F8)");
    }
    AppendMenu(MainMenu, MF_STRING | MF_POPUP, 0, "&Help");
    SetMenu(hwndA, MainMenu);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

//###################################################################

bool printfile(string ff, HWND hlist) {
    string st;

    SendMessage(hlist, LB_RESETCONTENT, 0, 0);
    ifstream inp(ff);
    if (inp.is_open()) {
        while (!getline(inp, st).eof()) {
            SendMessage(hlist, LB_ADDSTRING, 0, LPARAM(st.c_str()));
        }
        //SetFocus(hlist);
        inp.close();
    }
    else {
        return false;
    }
    return true;
}
