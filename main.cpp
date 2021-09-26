#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

string firstPlayerField[100]; // field represented in console for first player
string secondPlayerField[100]; // field represented in console for second player
string emptyField[100]; // reserved empty field;
string firstPlayerCaption = "                P L A Y E R  1              "; // caption for console
string secondPlayerCaption = "                P L A Y E R  2              ";
string aLotOfSpaces = "";
char firstPlayerMap[12][12]; // data of first player map
char secondPlayerMap[12][12]; // data of second player map
int numberOfShips[5]; // count of each type of ship (1-deck, 2-deck and e.g.)
int firstPlayerShipsRemained = 10;
int secondPlayerShipsRemained = 10;
bool firstPlayerShipsPlaced = false;
bool secondPlayerShipsPlaced = false;


void fillField() /// clear the data map
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

void setFirstEmptyField() /// make first player field empty (no ships placed)
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

void setSecondEmptyField() /// make second player field empty (no ships placed)
{
    for (int i = 0; i < 21; i++)
    {
        secondPlayerField[i] = firstPlayerField[i];
        emptyField[i] = firstPlayerField[i];
    }

    firstPlayerField[21] = firstPlayerField[1];
    secondPlayerField[21] = firstPlayerField[21];
}

void setSpacesSize(int spacesSize) /// set the number of spaced between two fields in console
{
    for (int i = 0; i < spacesSize; i++) aLotOfSpaces += " ";
}

void setConsoleSize() /// set default size of console window
{
    HWND window_header = GetConsoleWindow();
	SetWindowPos(window_header, HWND_TOP, 200, 100, 1200, 650, NULL);
}

void printAddDots() /// add dots around killed ships
{
    for (int i = 1; i <= 10; i++)
    {
        for (int j = 1; j <= 10; j++)
        {
            if (firstPlayerMap[i][j] == 'K')
            {
                for (int l = i - 1; l <= i + 1; l++)
                {
                    for (int k = j - 1; k <= j + 1; k++)
                    {
                        if (firstPlayerMap[l][k] == ' ') firstPlayerMap[l][k] = '.';
                    }
                }
            }

            if (secondPlayerMap[i][j] == 'K')
            {
                for (int l = i - 1; l <= i + 1; l++)
                {
                    for (int k = j - 1; k <= j + 1; k++)
                    {
                        if (secondPlayerMap[l][k] == ' ') secondPlayerMap[l][k] = '.';
                    }
                }
            }
        }
    }

    for (int i = 1; i <= 10; i++)
    {
        for (int j = 1; j <= 10; j++)
        {
            if (firstPlayerMap[i][j] == '.') firstPlayerField[i * 2][1 + j * 4] = '.'; // formula [i * 2][1 + j * 4] connects the display field and data map
            if (secondPlayerMap[i][j] == '.') secondPlayerField[i * 2][1 + j * 4] = '.';
        }
    }
}

void PrintFields() /// print fields in console
{
	cout << firstPlayerCaption + aLotOfSpaces + secondPlayerCaption + "\n\n";
    for (int i = 0; i < 22; i++)
    {
        cout << firstPlayerField[i] + aLotOfSpaces + secondPlayerField[i] << endl;
    }
}

bool checkPosition(string position) /// check the entered position in accordance to format "LETTERnumber"
{
    if (position.size() < 2 || position.size() > 3) return false;
    if (position[0] < 'A' || position[0] > 'J') return false;
    if (position.size() == 2)
    {
        if (position[1] < '0' || position[1] > '9') return false;
    }
    else
    {
        if (position[1] != '1' && position[2] != '0') return false;
    }
    return true;
}

bool checkPlayerFirstShipPoint(int x, int y) /// check the position to place the ship (no ships in radius 1) for FIRST PLAYER
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

bool checkPlayerSecondShipPoint(int x, int y) /// check the position to place the ship (no ships in radius 1) for SECOND PLAYER
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


bool tryToPlacePlayerFirstShip(string position, string direction, int deck) /// try to place a ship in position = position, direction = direction, size = deck)
{
    int y = position[0] - 'A' + 1;
    int x;
    if (position[2] == '0') x = 10;
    else x = position[1] - '0';
    if (direction == "V")
    {
        int remain = deck - 1 - (10 - x); // squares needed more to place the ship from position to the down corner of the field
        if (remain > 0) // needed more squares
        {
            for (int i = x - remain; i < x - remain + deck; i++)
            {
                if (!checkPlayerFirstShipPoint(i,y)) return false; // check the point to place a ship
            }

            for (int i = x - remain; i < x - remain + deck; i++)
            {
                firstPlayerMap[i][y] = '.';
            }
            return true;
        }
        else // no more squares needed
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
    else // if direction = H
    {
        int remain = deck - 1 - (10 - y); // squares needed more to place the ship from position to the right corner of the field
        if (remain > 0) // needed more squares
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
        else // no more squares nedeed
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

bool tryToPlacePlayerSecondShip(string position, string direction, int deck) /// the same as tryToPlacePlayerFirstShip, but for second player
{
    int y = position[0] - 'A' + 1;
    int x;
    if (position[2] == '0') x = 10;
    else x = position[1] - '0';
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
                /*cout << x << " " << i << endl;
                Sleep(3000);*/
            }
            return true;
        }
    }
}

void updateFields() /// updating information in fields using data from data maps)
{
    for (int i = 1; i<= 10; i++)
    {
        for (int j = 1; j <= 10; j++)
        {
            /// formula [i * 2][1 + j * 4] connects the field and the data map
            if (!firstPlayerShipsPlaced) firstPlayerField[i * 2][1 + j * 4] = firstPlayerMap[i][j];
            if (!secondPlayerShipsPlaced) secondPlayerField[i * 2][1 + j * 4] = secondPlayerMap[i][j];
        }
    }
}

void removeDotsFromField() /// remove dots from field to reuse them in other way
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

void replaceDotsInFields() /// reuse dots in other way
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

void setPrintFirstPlayerShips() /// placing ships for first player
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
                    Sleep(3000); // wait for 3 seconds to read the printed information
                    system("cls"); // clear console window
                    PrintFields();
                    setPrintFirstPlayerShips();
                    return;
                }
            }

            cout << "Choose the position of your ship [LETTERnumber]: ";
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
            cout << firstPlayerCaption << endl;
            cout << "Place your " << i << "-deck ship. " << i << "-deck ships remained: " << j << endl;
            if (i != 1) cout << "Choose the direction of your ship [V/H]: " << direction;
            cout << "\nChoose the position of your ship [LETTERnumber]: " << position;
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

void setPrintSecondPlayerShips() /// placing ships for second player
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

            cout << "Choose the position of your ship [LETTERnumber]: ";
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
                setPrintSecondPlayerShips();
                return;
            }

            system("cls");
            updateFields();
            PrintFields();
            cout << "\n\n\n";
            cout << secondPlayerCaption << endl;
            cout << "Place your " << i << "-deck ship. " << i << "-deck ships remained: " << j << endl;
            if (i != 1) cout << "Choose the direction of your ship [V/H]: " << direction;
            cout << "\nChoose the position of your ship [LETTERnumber]: " << position;
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
                setPrintSecondPlayerShips();
                return;
            }

            else if (choice == "N")
            {
                cout << "\n[SUCCESS] Removing selected position. Restarting entering information in 3 seconds...";
                Sleep(3000);
                system("cls");
                removeDotsFromField();
                PrintFields();
                setPrintSecondPlayerShips();
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
                setPrintSecondPlayerShips();
                return;
            }
        }
    }
}

void turnBattleModeFirstPlayer() /// clear the first player field before battle begins to not see opponent's ships
{
    for (int i = 0; i < 21; i++)
    {
        firstPlayerField[i] = emptyField[i];
    }
    cout << "\n[WARNING] Now it's time to place the ships of second player. Get ready!";
    Sleep(3000);
    system("cls");
    PrintFields();
}

void turnBattleModeSecondPlayer() /// clear the second player field before battle begins to not see opponent's ships
{
    for (int i = 0; i < 21; i++)
    {
        secondPlayerField[i] = emptyField[i];
    }
    cout << "\n[ATTENTION] Now the battle begins. Good luck & have fun!";
    Sleep(3000);
    system("cls");
    PrintFields();
}

bool checkFirstPlayerKilledShip(int x, int y)  /// checking first player ship dead with a part in position x, y
{
    /// Check ship is killed
    int i = x, j = y;
    while (firstPlayerMap[i][j] == 'X' || firstPlayerMap[i][j] == 'x')
    {
        if (firstPlayerMap[i][j] == 'x') return false;
        i++;
    }

    i = x, j = y;
    while (firstPlayerMap[i][j] == 'X' || firstPlayerMap[i][j] == 'x')
    {
        if (firstPlayerMap[i][j] == 'x') return false;
        i--;
    }

    i = x, j = y;
    while (firstPlayerMap[i][j] == 'X' || firstPlayerMap[i][j] == 'x')
    {
        if (firstPlayerMap[i][j] == 'x') return false;
        j--;
    }

    i = x, j = y;
    while (firstPlayerMap[i][j] == 'X' || firstPlayerMap[i][j] == 'x')
    {
        if (firstPlayerMap[i][j] == 'x') return false;
        j++;
    }


    /// Paint killed ship
    i = x, j = y;
    while (firstPlayerMap[i][j] == 'X' || firstPlayerMap[i][j] == 'x' || firstPlayerMap[i][j] == 'K')
    {
        if (firstPlayerMap[i][j] == 'X')
        {
            firstPlayerMap[i][j] = 'K';
            firstPlayerField[i * 2][1 + j * 4] = 'X';
        }
        i++;
    }

    i = x, j = y;
    while (firstPlayerMap[i][j] == 'X' || firstPlayerMap[i][j] == 'x' || firstPlayerMap[i][j] == 'K')
    {
        if (firstPlayerMap[i][j] == 'X')
        {
            firstPlayerMap[i][j] = 'K';
            firstPlayerField[i * 2][1 + j * 4] = 'X';
        }
        i--;
    }

    i = x, j = y;
    while (firstPlayerMap[i][j] == 'X' || firstPlayerMap[i][j] == 'x' || firstPlayerMap[i][j] == 'K')
    {
        if (firstPlayerMap[i][j] == 'X')
        {
            firstPlayerMap[i][j] = 'K';
            firstPlayerField[i * 2][1 + j * 4] = 'X';
        }
        j--;
    }

    i = x, j = y;
    while (firstPlayerMap[i][j] == 'X' || firstPlayerMap[i][j] == 'x' || firstPlayerMap[i][j] == 'K')
    {
        if (firstPlayerMap[i][j] == 'X')
        {
            firstPlayerMap[i][j] = 'K';
            firstPlayerField[i * 2][1 + j * 4] = 'X';
        }
        j++;
    }

    return true;
}

bool checkSecondPlayerKilledShip(int x, int y) /// the same as checkFirstPlayerKilledShip, but for second player
{
    /// Check ship is killed
    int i = x, j = y;
    while (secondPlayerMap[i][j] == 'X' || secondPlayerMap[i][j] == 'x')
    {
        if (secondPlayerMap[i][j] == 'x') return false;
        i++;
    }

    i = x, j = y;
    while (secondPlayerMap[i][j] == 'X' || secondPlayerMap[i][j] == 'x')
    {
        if (secondPlayerMap[i][j] == 'x') return false;
        i--;
    }

    i = x, j = y;
    while (secondPlayerMap[i][j] == 'X' || secondPlayerMap[i][j] == 'x')
    {
        if (secondPlayerMap[i][j] == 'x') return false;
        j--;
    }

    i = x, j = y;
    while (secondPlayerMap[i][j] == 'X' || secondPlayerMap[i][j] == 'x')
    {
        if (secondPlayerMap[i][j] == 'x') return false;
        j++;
    }


    /// Paint killed ship
    i = x, j = y;
    while (secondPlayerMap[i][j] == 'X' || secondPlayerMap[i][j] == 'x' || secondPlayerMap[i][j] == 'K')
    {
        if (secondPlayerMap[i][j] == 'X')
        {
            secondPlayerMap[i][j] = 'K';
            secondPlayerField[i * 2][1 + j * 4] = 'X';
        }
        i++;
    }

    i = x, j = y;
    while (secondPlayerMap[i][j] == 'X' || secondPlayerMap[i][j] == 'x' || secondPlayerMap[i][j] == 'K')
    {
        if (secondPlayerMap[i][j] == 'X')
        {
            secondPlayerMap[i][j] = 'K';
            secondPlayerField[i * 2][1 + j * 4] = 'X';
        }
        i--;
    }

    i = x, j = y;
    while (secondPlayerMap[i][j] == 'X' || secondPlayerMap[i][j] == 'x' || secondPlayerMap[i][j] == 'K')
    {
        if (secondPlayerMap[i][j] == 'X')
        {
            secondPlayerMap[i][j] = 'K';
            secondPlayerField[i * 2][1 + j * 4] = 'X';
        }
        j--;
    }

    i = x, j = y;
    while (secondPlayerMap[i][j] == 'X' || secondPlayerMap[i][j] == 'x' || secondPlayerMap[i][j] == 'K')
    {
        if (secondPlayerMap[i][j] == 'X')
        {
            secondPlayerMap[i][j] = 'K';
            secondPlayerField[i * 2][1 + j * 4] = 'X';
        }
        j++;
    }

    return true;
}


bool secondPlayerHit(string position) /// second player trying to hit the ship in position = position
{
    int y = position[0] - 'A' + 1;
    int x;
    if (position[2] == '0') x = 10;
    else x = position[1] - '0';
    if (firstPlayerMap[x][y] == 'x') // check the first player have ship in position x, y
    {
        firstPlayerField[x * 2][1 + y * 4] = 'x'; // mark ship as damaged
        firstPlayerMap[x][y] = 'X';
        cout << "\n[HITTED] Successful hit!";

        if (checkFirstPlayerKilledShip(x, y)) // if ship is killed
        {
            cout << "\n[KILLED] You just killed enemy ship. Good job!";
            firstPlayerShipsRemained--; // decrease the number of first player ships
            printAddDots(); // add dots around dead ship
        }
        return true;
    }

    if (firstPlayerMap[x][y] == 'X') // if hit before
    {
        cout << "\n[ERROR] You hit this point before. Try another one.";
        return true;
    }

    if (firstPlayerMap[x][y] == 'K') // if killed ship
    {
        cout << "\n[ERROR] This ship is already killed. Try another one.";
        return true;
    }

    if (firstPlayerMap[x][y] == ' ') // if empty field
    {
        cout << "\n[MISS] Not today! Try again later.";
        firstPlayerMap[x][y] = '.';
        firstPlayerField[x * 2][1 + y * 4] = '.';
        return false;
    }

    if (firstPlayerMap[x][y] == '.') // if missed here before
    {
        cout << "\n[ERROR] You missed here before. Try another position";
        return true;
    }
}

bool firstPlayerHit(string position) /// same as secondPlayerHit, but first player is hitting
{
    int y = position[0] - 'A' + 1;
    int x;
    if (position[2] == '0') x = 10;
    else x = position[1] - '0';
    if (secondPlayerMap[x][y] == 'x')
    {
        secondPlayerField[x * 2][1 + y * 4] = 'x';
        secondPlayerMap[x][y] = 'X';
        cout << "\n[HITTED] Successful hit!";
        if (checkSecondPlayerKilledShip(x, y))
        {
            cout << "\n[KILLED] You just killed enemy ship. Good job!";
            secondPlayerShipsRemained--;
            printAddDots();
        }
        return true;
    }

    if (secondPlayerMap[x][y] == 'X')
    {
        cout << "\n[ERROR] You hit this point before. Try another one.";
        return true;
    }

    if (secondPlayerMap[x][y] == 'K')
    {
        cout << "\n[ERROR] This ship is already killed. Try another one.";
        return true;
    }

    if (secondPlayerMap[x][y] == ' ')
    {
        cout << "\n[MISS] Not today! Try again later.";
        secondPlayerMap[x][y] = '.';
        secondPlayerField[x * 2][1 + y * 4] = '.';
        return false;
    }

    if (secondPlayerMap[x][y] == '.')
    {
        cout << "\n[ERROR] You missed here before. Try another position";
        return true;
    }
}

void firstPlayerBattleMove() /// first player battle move
{
    system("cls");
    PrintFields();
    cout << "\n\n\n" << firstPlayerCaption << endl;
    string position;
    cout << "\nChoose the position of your hit [LETTERnumber]: ";
    cin >> position;
    if (!checkPosition(position))
    {
        cout << "\n[ERROR] Incorrect position. Restarting entering hit information in 3 seconds...";
        Sleep(3000);
        system("cls");
        PrintFields();
        firstPlayerBattleMove();
        return;
    }

    if (firstPlayerHit(position))
    {
        if (secondPlayerShipsRemained == 0) // if zero ships remained after hit
        {
            cout << "\n[WIN] Congratulations! You just won this game!";
            return;
        }
        cout << "\n[ONE MORE MOVE FOR YOU]"; // if success hit one more turn for player
        Sleep(3000);
        firstPlayerBattleMove();
    }
    else
    {
        cout << "\n[CHANGING PLAYER]"; // if missed change the player
        Sleep(3000);
    }
    return;
}

void secondPlayerBattleMove() /// second player battle move
{
    system("cls");
    PrintFields();
    cout << "\n\n\n" << secondPlayerCaption << endl;
    string position;
    cout << "\nChoose the position of your hit [LETTERnumber]: ";
    cin >> position;
    if (!checkPosition(position))
    {
        cout << "\n[ERROR] Incorrect position. Restarting entering hit information in 3 seconds...";
        Sleep(3000);
        system("cls");
        PrintFields();
        secondPlayerBattleMove();
        return;
    }

    if (secondPlayerHit(position))
    {
        if (firstPlayerShipsRemained == 0)
        {
            cout << "\n[WIN] Congratulations! You just won this game!";
            return;
        }
        cout << "\n[ONE MORE TURN FOR YOU]";
        Sleep(3000);
        secondPlayerBattleMove();
    }
    else
    {
        cout << "\n[CHANGING PLAYER]";
        Sleep(3000);
    }
    return;

}

int theBattle() /// THE BATTLE
{
    firstPlayerBattleMove();
    if (secondPlayerShipsRemained == 0) return 1;
    secondPlayerBattleMove();
    if (firstPlayerShipsRemained == 0) return 2;
    theBattle();
}

void theGame()
{
    setSpacesSize(50);
    setConsoleSize();
    while (true)
    {
        system("cls");
        fillField();
        setFirstEmptyField();
        setSecondEmptyField();
        firstPlayerShipsPlaced = false;
        secondPlayerShipsPlaced = false;
        PrintFields();
        numberOfShips[1] = 4;
        numberOfShips[2] = 3;
        numberOfShips[3] = 2;
        numberOfShips[4] = 1;
        setPrintFirstPlayerShips();
        firstPlayerShipsPlaced = true;
        turnBattleModeFirstPlayer();
        numberOfShips[1] = 4;
        numberOfShips[2] = 3;
        numberOfShips[3] = 2;
        numberOfShips[4] = 1;
        setPrintSecondPlayerShips();
        secondPlayerShipsPlaced = true;
        turnBattleModeSecondPlayer();
        int result = theBattle();
        while (true)
        {
            cout << "\n[NEW GAME] Do you want to play one more time?[Y/N]: ";
            string answer;
            cin >> answer;
            if (answer == "Y") break;
            if (answer == "N") return;
        }
    }
}



int main()
{
    theGame();

    return 0;
}
