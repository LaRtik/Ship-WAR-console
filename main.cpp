#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

string firstPlayerField[100];
string secondPlayerField[100];
string firstPlayerCaption = "                P L A Y E R  1              ";
string secondPlayerCaption = "                P L A Y E R  2              ";
string aLotOfSpaces = "";
char firstPlayerMap[11][11];
char secondPlayerMap[11][11];
int numberOfShips[5];


void fillField()
{
    for (int i = 0; i<= 11; i++)
    {
        for (int j = 0; j<= 11; j++)
        {
            firstPlayerMap[i][j] = ' ';
            secondPlayerMap[i][j] = ' ';
        }
    }
}

void setFirstEmptyField()
{
    firstPlayerField[0] = "   | A | B | C | D | E | F | G | H | I | J |";
    firstPlayerField[1] = "   ----------------------------------------|";
    firstPlayerField[2] = "|   |   |   |   |   |   |   |   |   |   |";
    for (int i = 3; i < 21; i+=2)
    {
        firstPlayerField[i] = firstPlayerField[1];
        firstPlayerField[i+1] = " " + to_string(i/2 + 1) + " " + firstPlayerField[2];
    }
    firstPlayerField[20].erase(firstPlayerField[20].begin());
    firstPlayerField[2] = " 1 " + firstPlayerField[2];
}

void setSecondEmptyField()
{
    for (int i = 0; i < 21; i++)
    {
        secondPlayerField[i] = firstPlayerField[i];
     //   cout << secondPlayerField[i] << endl;
    }

    firstPlayerField[21] = firstPlayerField[1];
    secondPlayerField[21] = firstPlayerField[21];
}

void setSpacesSize(int spacesSize)
{
    for (int i = 0; i < spacesSize; i++) aLotOfSpaces += " ";
}

void setConsoleSize()
{
    HWND window_header = GetConsoleWindow();
	SetWindowPos(window_header, HWND_TOP, 200, 100, 1200, 650, NULL);
}

void PrintFields()
{
	cout << firstPlayerCaption + aLotOfSpaces + secondPlayerCaption + "\n\n";
    for (int i = 0; i < 22; i++)
    {
        cout << firstPlayerField[i] + aLotOfSpaces + secondPlayerField[i] << endl;
    }
}

bool checkPosition(string position)
{
    if (position.size() > 7) return false;
    if (position[0] != '<' || position[3] != '<') return false;
    if (position[1] < 'A' && position[1] > 'J') return false;
    if (position[2] != '>') return false;
    if (position[4] < '1' && position[4] > '9') return false;
    if (position[5] == '0' && position[4] != '1') return false;
    if (position[6] == '>' && position[5] != '0') return false;
    if (position[5] != '>' && position[4] != '1') return false;
    return true;
}

bool checkPlayerFirstShipPoint(int x, int y)
{
    for (int i = x - 1; i <= x + 1; i++)
    {
        for (int j = y - 1; j <= y + 1; j++)
        {
            if (firstPlayerMap[i][j] == 'x') return false;
        }
    }
    return true;
}

bool checkPlayerSecondShipPoint(int x, int y)
{
    for (int i = x - 1; i <= x + 1; i++)
    {
        for (int j = y - 1; j <= y + 1; j++)
        {
            if (secondPlayerMap[i][j] == 'x') return false;
        }
    }
    return true;
}


bool tryToPlacePlayerFirstShip(string position, string direction, int deck)
{
    int y = position[1] - 'A' + 1;
    int x;
    if (position[5] == '0') x = 10;
    else x = position[4] - '0';
    if (direction == "V")
    {
        int remain = deck - 1 - (10 - x);
        if (remain > 0)
        {
            for (int i = x - remain; i < x - remain + deck; i++)
            {
                if (!checkPlayerFirstShipPoint(i,y)) return false;
            }

            for (int i = x - remain; i < x - remain + deck; i++)
            {
                firstPlayerMap[i][y] = '.';
            }
            return true;
        }
        else
        {
            for (int i = x; i < x + deck; i++)
            {
                if (!checkPlayerFirstShipPoint(i,y)) return false;
            }

            for (int i = x; i < x + deck; i++)
            {
                firstPlayerMap[i][y] = '.';
            }
            return true;
        }
    }
    else
    {
        int remain = deck - 1 - (10 - y);
        if (remain > 0)
        {
            for (int i = y - remain; i <= y - remain + deck; i++)
            {
                if (!checkPlayerFirstShipPoint(x,i)) return false;
            }

            for (int i = y - remain; i <= y - remain + deck; i++)
            {
                firstPlayerMap[x][i] = '.';
            }
            return true;
        }
        else
        {
            for (int i = y; i < y + deck; i++)
            {
                if (!checkPlayerFirstShipPoint(x,i)) return false;
            }

            for (int i = y; i < y + deck; i++)
            {
                firstPlayerMap[x][i] = '.';
               /* cout << x << " " << i << endl;
                Sleep(3000);*/
            }
            return true;
        }
    }
}

bool tryToPlacePlayerSecondShip(string position, string direction, int deck)
{
    int y = position[1] - 'A' + 1;
    int x;
    if (position[5] == '0') x = 10;
    else x = position[4] - '0';
    if (direction == "V")
    {
        int remain = deck - 1 - (10 - x);
        if (remain > 0)
        {
            for (int i = x - remain; i < x - remain + deck; i++)
            {
                if (!checkPlayerSecondShipPoint(i,y)) return false;
            }

            for (int i = x - remain; i < x - remain + deck; i++)
            {
                secondPlayerMap[i][y] = '.';
            }
            return true;
        }
        else
        {
            for (int i = x; i < x + deck; i++)
            {
                if (!checkPlayerSecondShipPoint(i,y)) return false;
            }

            for (int i = x; i < x + deck; i++)
            {
                secondPlayerMap[i][y] = '.';
            }
            return true;
        }
    }
    else
    {
        int remain = deck - 1 - (10 - y);
        if (remain > 0)
        {
            for (int i = y - remain; i <= y - remain + deck; i++)
            {
                if (!checkPlayerSecondShipPoint(x,i)) return false;
            }

            for (int i = y - remain; i <= y - remain + deck; i++)
            {
                secondPlayerMap[x][i] = '.';
            }
            return true;
        }
        else
        {
            for (int i = y; i < y + deck; i++)
            {
                if (!checkPlayerSecondShipPoint(x,i)) return false;
            }

            for (int i = y; i < y + deck; i++)
            {
                secondPlayerMap[x][i] = '.';
               /* cout << x << " " << i << endl;
                Sleep(3000);*/
            }
            return true;
        }
    }
}

void updateFields()
{
    for (int i = 1; i<= 10; i++)
    {
        for (int j = 1; j <= 10; j++)
        {
            firstPlayerField[i * 2][1 + j * 4] = firstPlayerMap[i][j];
          //  cout << firstPlayerMap[i][j];
            secondPlayerField[i * 2][1 + j * 4] = secondPlayerMap[i][j];
        }
       // cout << endl;
    }
}

void removeDotsFromField()
{
    for (int i = 0; i < 11; i++)
    {
        for (int j = 0; j < 11; j++)
        {
            if (firstPlayerMap[i][j] == '.') firstPlayerMap[i][j] = ' ';
            if (secondPlayerMap[i][j] == '.') secondPlayerMap[i][j] = ' ';
        }
    }
    updateFields();
}

void replaceDotsInFields()
{
    for (int i = 0; i < 11; i++)
    {
        for (int j = 0; j < 11; j++)
        {
            if (firstPlayerMap[i][j] == '.') firstPlayerMap[i][j] = 'x';
            if (secondPlayerMap[i][j] == '.') secondPlayerMap[i][j] = 'x';
        }
    }
    updateFields();
}

void setPrintFirstPlayerShips()
{
    cout << "\n\n\n";

    for (int i = 4; i >= 1; i--)
    {
        for (int j = numberOfShips[i]; j >= 1; j--)
        {
            string direction;
            cout << firstPlayerCaption << endl;
            cout << "Place your " << i << "-deck ship. " << i << "-deck ships remained: " << j << endl;
            if (i != 1)
            {
                cout << "Choose the direction of your ship [V/H]: ";
                cin >> direction;
                if (direction != "V" && direction != "H")
                {
                    cout << "\n[ERROR] Incorrect direction. Restarting entering information in 3 seconds...";
                    Sleep(3000);
                    system("cls");
                    PrintFields();
                    setPrintFirstPlayerShips();
                    return;
                }
            }

            cout << "Choose the position of your ship [<letter><number>]: ";
            string position;
            cin >> position;
            if (!checkPosition(position))
            {
                cout << "\n[ERROR] Incorrect position. Restarting entering information in 3 seconds...";
                Sleep(3000);
                system("cls");
                PrintFields();
                setPrintFirstPlayerShips();
                return;
            }

            if (!tryToPlacePlayerFirstShip(position, direction, i))
            {
                cout << "\n[ERROR] Can't place a ship in selected position. Restarting entering information in 3 seconds...";
                Sleep(3000);
                system("cls");
                PrintFields();
                setPrintFirstPlayerShips();
                return;
            }

            system("cls");
            updateFields();
            PrintFields();
            cout << "\n\n\n";
            cout << "Player 1. Place your " << i << "-deck ship. " << i << "-deck ships remained: " << j << endl;
            cout << "Choose the direction of your ship [V/H]: " << direction;
            cout << "\nChoose the position of your ship [<letter><number>]: " << position;
            cout << "\nPosition selected! Accept position? (changes displayed on your field {.}) [Y/N]: ";
            string choice;
            cin >> choice;
            if (choice != "Y" && choice != "N")
            {
                cout << "\n[ERROR] Invalid input. Restarting entering information in 3 seconds...";
                Sleep(3000);
                system("cls");
                removeDotsFromField();
                PrintFields();
                setPrintFirstPlayerShips();
                return;
            }

            else if (choice == "N")
            {
                cout << "\n[SUCCESS] Removing selected position. Restarting entering information in 3 seconds...";
                Sleep(3000);
                system("cls");
                removeDotsFromField();
                PrintFields();
                setPrintFirstPlayerShips();
                return;
            }
            else
            {
                cout << "\n[SUCCESS] Commiting changes...";
                Sleep(3000);
                system("cls");
                replaceDotsInFields();
                PrintFields();
                numberOfShips[i]--;
                setPrintFirstPlayerShips();
                return;
            }
        }
    }
}

void setPrintSecondPlayerShips()
{
    cout << "\n\n\n";

    for (int i = 4; i >= 1; i--)
    {
        for (int j = numberOfShips[i]; j >= 1; j--)
        {
            string direction;
            cout << secondPlayerCaption << endl;
            cout << "Place your " << i << "-deck ship. " << i << "-deck ships remained: " << j << endl;
            if (i != 1)
            {
                cout << "Choose the direction of your ship [V/H]: ";
                cin >> direction;
                if (direction != "V" && direction != "H")
                {
                    cout << "\n[ERROR] Incorrect direction. Restarting entering information in 3 seconds...";
                    Sleep(3000);
                    system("cls");
                    PrintFields();
                    setPrintSecondPlayerShips();
                    return;
                }
            }

            cout << "Choose the position of your ship [<letter><number>]: ";
            string position;
            cin >> position;
            if (!checkPosition(position))
            {
                cout << "\n[ERROR] Incorrect position. Restarting entering information in 3 seconds...";
                Sleep(3000);
                system("cls");
                PrintFields();
                setPrintSecondPlayerShips();
                return;
            }

            if (!tryToPlacePlayerSecondShip(position, direction, i))
            {
                cout << "\n[ERROR] Can't place a ship in selected position. Restarting entering information in 3 seconds...";
                Sleep(3000);
                system("cls");
                PrintFields();
                setPrintFirstPlayerShips();
                return;
            }

            system("cls");
            updateFields();
            PrintFields();
            cout << "\n\n\n";
            cout << "Player 1. Place your " << i << "-deck ship. " << i << "-deck ships remained: " << j << endl;
            cout << "Choose the direction of your ship [V/H]: " << direction;
            cout << "\nChoose the position of your ship [<letter><number>]: " << position;
            cout << "\nPosition selected! Accept position? (changes displayed on your field {.}) [Y/N]: ";
            string choice;
            cin >> choice;
            if (choice != "Y" && choice != "N")
            {
                cout << "\n[ERROR] Invalid input. Restarting entering information in 3 seconds...";
                Sleep(3000);
                system("cls");
                removeDotsFromField();
                PrintFields();
                setPrintFirstPlayerShips();
                return;
            }

            else if (choice == "N")
            {
                cout << "\n[SUCCESS] Removing selected position. Restarting entering information in 3 seconds...";
                Sleep(3000);
                system("cls");
                removeDotsFromField();
                PrintFields();
                setPrintFirstPlayerShips();
                return;
            }
            else
            {
                cout << "\n[SUCCESS] Commiting changes...";
                Sleep(3000);
                system("cls");
                replaceDotsInFields();
                PrintFields();
                numberOfShips[i]--;
                setPrintFirstPlayerShips();
                return;
            }
        }
    }
}




int main()
{
    fillField();
    setFirstEmptyField();
    setSecondEmptyField();
    setSpacesSize(50);
    setConsoleSize();
    PrintFields();
    numberOfShips[1] = 4;
    numberOfShips[2] = 3;
    numberOfShips[3] = 2;
    numberOfShips[4] = 1;
    setPrintFirstPlayerShips();
    numberOfShips[1] = 4;
    numberOfShips[2] = 3;
    numberOfShips[3] = 2;
    numberOfShips[4] = 1;
    setPrintSecondPlayerShips();





    return 0;
}
