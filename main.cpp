#include <cstdlib>
#include <ctime>
#include <iostream>
#include <termios.h>
#include <vector>

using namespace std;
// ANSI转义序列设置前景色
#define NC "\e[0m"
#define RED1 "\e[31m"
#define RED2 "\e[91m"
#define GREEN1 "\e[32m"
#define GREEN2 "\e[92m"
#define YELLOW1 "\e[33m"
#define YELLOW2 "\e[93m"
#define BLUE1 "\e[34m"
#define BLUE2 "\e[94m"
#define MAGENTA1 "\e[35m"
#define MAGENTA2 "\e[95m"
#define CYAN1 "\e[36m"
#define CYAN2 "\e[96m"

int nums[4][4];
string logo2048 =
    "   ___          __       __ __           __\n /'___`\\      /'__`\\   "
    " /\\ \\\\ \\        /'_ `\\\n/\\_\\ /\\ \\    /\\ \\/\\ \\   \\ \\ "
    "\\\\ \\      /\\ \\L\\ \\\n\\/_/// /__   \\ \\ \\ \\ \\   \\ \\ \\\\ "
    "\\_    \\/_> _ <_\n   // /_\\ \\   \\ \\ \\_\\ \\   \\ \\__ ,__\\    "
    "/\\ \\L\\ \\\n  /\\______/    \\ \\____/    \\/_/\\_\\_/    \\ "
    "\\____/\n  \\/_____/      \\/___/        \\/_/       \\/___/\n";
string plgame = "  W or K or ↑ Up\n  A or H or ← => Left\n  S or J or ↓ => "
                "Down\n  D or L or → => Right\n  Q or esc => Quit\n\nPress the "
                "keys to start and continue.\n";

void print_board() {
    //打印整个棋盘，数字用ANSI转义序列输出不同的颜色
    cout << "┏━━━━━━━━┳━━━━━━━━┳━━━━━━━━┳━━━━━━━━┓\n";
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int a = nums[i][j];
            if (a == 0)
                cout << "┃        ";
            else if (a == 2)
                cout << "┃    " << GREEN1 << a << NC << "   ";
            else if (a == 4)
                cout << "┃    " << YELLOW1 << a << NC << "   ";
            else if (a == 8)
                cout << "┃    " << BLUE1 << a << NC << "   ";
            else if (a == 16)
                cout << "┃   " << MAGENTA1 << a << NC << "   ";
            else if (a == 32)
                cout << "┃   " << CYAN1 << a << NC << "   ";
            else if (a == 64)
                cout << "┃   " << GREEN2 << a << NC << "   ";
            else if (a == 128)
                cout << "┃   " << YELLOW2 << a << NC << "  ";
            else if (a == 256)
                cout << "┃   " << BLUE2 << a << NC << "  ";
            else if (a == 512)
                cout << "┃   " << MAGENTA2 << a << NC << "  ";
            else if (a == 1024)
                cout << "┃  " << RED2 << a << NC << "  ";
        }
        cout << "┃\n";
        if (i != 3)
            cout << "┣━━━━━━━━╋━━━━━━━━╋━━━━━━━━╋━━━━━━━━┫\n";
        else
            cout << "┗━━━━━━━━┻━━━━━━━━┻━━━━━━━━┻━━━━━━━━┛\n";
    }
    return;
}

void add_element() {
    vector<pair<int, int>> ve;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (nums[i][j] == 0)
                ve.push_back({i, j});
        }
    }
    int n = ve.size();
    if (n == 1)
        nums[ve[0].first][ve[0].second] = (rand() % 2 + 1) * 2;
    else if (n == 2) {
        nums[ve[0].first][ve[0].second] = (rand() % 2 + 1) * 2;
        nums[ve[1].first][ve[1].second] = (rand() % 2 + 1) * 2;
    }
    if (n > 2) {
        int a = rand() % (n - 1) + 1;
        int x = ve[a].first, y = ve[a].second;
        nums[x][y] = (a % 2 + 1) * 2;
        ve.erase(ve.begin() + a);
        int b = rand() % (n - 2) + 1;
        x = ve[b].first, y = ve[b].second;
        nums[x][y] = (b % 2 + 1) * 2;
    }
}

//实时监听终端输入
int scanKeyboard() {
    int in;
    struct termios new_settings;
    struct termios stored_settings;
    //设置终端参数
    tcgetattr(0, &stored_settings);
    new_settings = stored_settings;
    new_settings.c_lflag &= (~ICANON);
    new_settings.c_cc[VTIME] = 0;
    tcgetattr(0, &stored_settings);
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &new_settings);
    in = getchar();
    tcsetattr(0, TCSANOW, &stored_settings);
    return in;
}

void go_right() {
    for (int i = 0; i < 4; i++) {
        vector<int> ve;
        for (int j = 3; j >= 0; j--) {
            if (nums[i][j] != 0) ve.push_back(nums[i][j]);
        }
        vector<int> tmp;
        if (ve.size() != 0) {
            for (int j = 0; j < ve.size(); j++) {
                if (j + 1 < ve.size() && ve[j] == ve[j + 1]) {
                    tmp.push_back(ve[j] * 2);
                    j++;
                }
                else tmp.push_back(ve[j]);
            }
        }
        for (int j = 0; j < 4; j++) tmp.push_back(0);
        reverse(tmp.begin(), tmp.begin() + 4);
        for (int j = 0; j < 4; j++) nums[i][j] = tmp[j];
    }
}

void go_left() {
    for (int i = 0; i < 4; i++) {
        vector<int> ve;
        for (int j = 0; j < 4; j++) {
            if (nums[i][j] != 0)
                ve.push_back(nums[i][j]);
        }
        vector<int> tmp;
        if (ve.size() != 0) {
            for (int j = 0; j < ve.size(); j++) {
                if (j + 1 < ve.size() && ve[j] == ve[j + 1]) {
                    tmp.push_back(ve[j] * 2);
                    j++;
                } else
                    tmp.push_back(ve[j]);
            }
        }
        for (int j = 0; j < 4; j++)
            tmp.push_back(0);
        for (int j = 0; j < 4; j++)
            nums[i][j] = tmp[j];
    }
}

void go_up() {
    for (int j = 0; j < 4; j++) {
        vector<int> ve;
        for (int i = 0; i < 4; i++) {
            if (nums[i][j] != 0)
                ve.push_back(nums[i][j]);
        }
        vector<int> tmp;
        if (ve.size() != 0) {
            for (int i = 0; i < ve.size(); i++) {
                if (i + 1 < ve.size() && ve[i] == ve[i + 1]) {
                    tmp.push_back(ve[i] * 2);
                    i++;
                } else
                    tmp.push_back(ve[i]);
            }
        }
        for (int i = 0; i < 4; i++)
            tmp.push_back(0);
        for (int i = 0; i < 4; i++)
            nums[i][j] = tmp[i];
    }
}

void go_down() {
    for (int j = 0; j < 4; j++) {
        vector<int> ve;
        for (int i = 3; i >= 0; i--) {
            if (nums[i][j] != 0)
                ve.push_back(nums[i][j]);
        }
        vector<int> tmp;
        if (ve.size() != 0) {
            for (int i = 0; i < ve.size(); i++) {
                if (i + 1 < ve.size() && ve[i] == ve[i + 1]) {
                    tmp.push_back(ve[i] * 2);
                    i++;
                } else
                    tmp.push_back(ve[i]);
            }
        }
        for (int i = 0; i < 4; i++)
            tmp.push_back(0);
        reverse(tmp.begin(), tmp.begin() + 4);
        for (int i = 0; i < 4; i++)
            nums[i][j] = tmp[i];
    }
}

void change_board(int c) {
    if (c == 'w' || c == 'k')
        go_up();
    else if (c == 's' || c == 'j')
        go_down();
    else if (c == 'a' || c == 'h')
        go_left();
    else if (c == 'd' || c == 'l')
        go_right();
}

int check_status() {
    //返回3种状态，0表示无法继续游戏失败，1表示可以继续游戏，2表示已达成2048
    int status;
    bool win = 0, loss = 1;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (j != 3 && nums[i][j] == nums[i][j + 1])
                loss = 0;
            if (nums[i][j] == 0)
                loss = 0;
            if (nums[i][j] == 2048)
                win = 1;
            if (win)
                return 2;
        }
    }
    if (!loss)
        return 1;
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 3; i++) {
            if (nums[i][j] == nums[i + 1][j])
                loss = 0;
        }
    }
    if (loss)
        status = 0;
    else
        status = 1;
    if (win)
        status = 2;
    return status;
}

void play_game() {
    while (1) {
        int c = scanKeyboard();
        if (c == 'q' || c == 'Q')
            return;
        change_board(c);
        int status = check_status();
        system("clear");
        if (status == 0) {
            cout << "Game over!\n";
            return;
        } else if (status == 2) {
            cout << "You win!!!\n";
            return;
        } else {
            add_element();
            cout << GREEN1 << logo2048 << NC << "\n";
            print_board();
            cout << "\n" << plgame;
        }
    }
}

int main() {
    srand((unsigned)time(NULL));
    add_element();
    cout << GREEN1 << logo2048 << NC << "\n";
    print_board();
    cout << "\n" << plgame;
    play_game();

    return 0;
}
