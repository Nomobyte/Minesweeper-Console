#include <iostream>
#include <vector>
#include <cstdlib>
#include <utility>
#include <ctime>


void rand_field(std::vector<std::vector<bool>>& field, int bombPercent)
{
    int rows = field.size();
    int cols = field[0].size();

    for(int row = 0; row < rows; row++)
    {
        for(int col = 0; col < cols; col++)
        {
            field[row][col] = rand() % 100 < bombPercent ? true : false;
        }
    }
}

void calculate_field(const std::vector<std::vector<bool>>& field, std::vector<std::vector<int>>& fieldVal)
{
    int rows = field.size();
    int cols = field[0].size();

    for(int row = 0; row < rows; row++)
    {
        for(int col = 0; col < cols; col++)
        {
            for(int inRow = row - 1; inRow <= row + 1; inRow++)
            {
                for(int inCol = col - 1; inCol <= col + 1; inCol++)
                {
                    if(inRow >= 0 && inCol >= 0 && inRow < rows && inCol < cols && (!(inRow == row && inCol == col)) && field[inRow][inCol])
                    {
                        fieldVal[row][col]++;
                    }
                }
            }
        }
    }
}

void open_fog(const std::vector<std::vector<bool>>& field, const std::vector<std::vector<int>>& fieldVal, std::vector<std::vector<int>>& fogOfWar, int row, int col)
{
    //std::cout << row << '\t' << col << std::endl;
    int rows = field.size();
    int cols = field[0].size();
    fogOfWar[row][col] = 1;

    if(fieldVal[row][col] == 0)
    {
        for(int inRow = row - 1; inRow <= row + 1; inRow++)
        {
            for(int inCol = col - 1; inCol <= col + 1; inCol++)
            {
                if(inRow >= 0 && inCol >= 0 && inRow < rows && inCol < cols && (!(inRow == row && inCol == col)) && fogOfWar[inRow][inCol] != 1)
                {
                    open_fog(field, fieldVal, fogOfWar, inRow, inCol);
                }
            }
        }
    }
}

bool is_win(const std::vector<std::vector<bool>>& field, std::vector<std::vector<int>>& fogOfWar)
{
    int rows = field.size();
    int cols = field[0].size();

    for(int row = 0; row < rows; row++)
    {
        for(int col = 0; col < cols; col++)
        {
            if (!field[row][col] && fogOfWar[row][col] == 0)
            {
                return false;
            }
        }
    }
    return true;
}

void print_2d_vec(const std::vector<std::vector<bool>>& vec)
{
    int rows = vec.size();
    int cols = vec[0].size();
    for(int row = 0; row < rows; row++)
    {
        for(int col = 0; col < cols; col++)
        {
            std::cout << (vec[row][col] ? '#' : ' ');
        }
        std::cout << '\n';
    }
}
void print_2d_vec(const std::vector<std::vector<int>>& vec)
{
    int rows = vec.size();
    int cols = vec[0].size();
    for(int row = 0; row < rows; row++)
    {
        for(int col = 0; col < cols; col++)
        {
            std::cout << vec[row][col];
        }
        std::cout << '\n';
    }
}

void print_field(const std::vector<std::vector<int>>& fieldVal, const std::vector<std::vector<int>>& fogOfWar)
{
    int rows = fieldVal.size();
    int cols = fieldVal[0].size();

    std::cout << '#';
    for(int i = 0; i < cols; i++) 
    {
        if (i % 2 == 0)
        {
            std::cout << '-';
        }
        else
        {
            std::cout << '=';
        }
    }
    std::cout << '#';
    std::cout << '\n';

    for(int row = 0; row < rows; row++)
    {
        if (row % 2 == 0)
        {
            std::cout << '|';
        }
        else 
        {
            std::cout << '+';
        }
        
        for(int col = 0; col < cols; col++)
        {
            if (fogOfWar[row][col] == 1)
            {
                if(fieldVal[row][col] == 0)
                {
                    std::cout << ' ';
                }
                else
                {
                    std::cout << fieldVal[row][col];
                }
            }
            else if (fogOfWar[row][col] == 2)
            {
                std::cout << '!';
            }
            else 
            {
                std::cout << '@';
            }
        }

        if (row % 2 == 0)
        {
            std::cout << '|';
        }
        else 
        {
            std::cout << '+';
        }

        std::cout << '\n';
    }

    std::cout << '#';
    for(int i = 0; i < cols; i++) 
    {
        if (i % 2 == 0)
        {
            std::cout << '-';
        }
        else
        {
            std::cout << '=';
        }
    }
    std::cout << '#';
    std::cout << '\n';
}

template <typename t>
bool check_answer(char* question, t ans)
{
    for (int i = 0; question[i] != '\0'; i++)
    {
        std::cout << question[i];
    }

    char userAns;
    std::cin >> userAns;

    return userAns == ans;
}

int main(void)
{
    srand(time(NULL));
    bool isRestart = true;

    do
    {
        bool isGameOver = false;
        bool isWin = false;

        int rows, cols, difficulty;
        std::cout << "Rows, cols: ";
        std::cin >> rows >> cols;

        std::vector<std::vector<bool>> field(rows, std::vector<bool>(cols, 0));
        std::vector<std::vector<int>> fieldVal(rows, std::vector<int>(cols, 0));
        std::vector<std::vector<int>> fogOfWar(rows, std::vector<int>(cols, 0));
        std::cout << "Difficulty 1-99: ";
        std::cin >> difficulty;
        rand_field(field, difficulty);
        calculate_field(field, fieldVal);
        
        unsigned long long begTime = clock();

        do
        {
            //print_2d_vec(fogOfWar);
            print_field(fieldVal, fogOfWar);

            int row, col;
            std::cout << "Row, col: ";
            std::cin >> row >> col;
            row--;
            col--;

            if (check_answer("Open or Flag(o/f)?: ", 'o'))
            {
                open_fog(field, fieldVal, fogOfWar, row, col);
                isGameOver = field[row][col];
            }
            else
            {
                fogOfWar[row][col] = 2;
            }
            
            std::cout << '\n';
            
            isWin = is_win(field, fogOfWar);
        } while(!isGameOver && !isWin);

        if (isWin && !isGameOver)
        {
            std::cout << "You Win!\n";
        }
        else
        {
            std::cout << "Game Over!\n";
        }

        unsigned long long endTime = (clock() - begTime) / 1000;
        
        std::cout << "Your time: " << endTime << "s\n";

        isRestart = check_answer("Restart (y/n)?: ", 'y');
    } while (isRestart);

    return 0;
}