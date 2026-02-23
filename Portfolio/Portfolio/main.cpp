//
//  main.cpp
//  Portfolio
//
//  Created by Marina Balamoon on 2/9/26.
//

#include <iostream>
#include <vector>
#include <string>

using namespace std;

const int ROWS = 10;
const int COLS = 10;

class Player{
private:
    int rows;
    int col;
    int hp;
    int attack;
    int def;
    int keys;
public:
    Player (int r, int c, int h, int a, int d){
        rows = r;
        col = c;
        hp = h;
        attack = a;
        def = d;
        keys = 0;
    }
    int getRow(){
        return rows;
    }
    int getCol(){
        return col;
    }
    int gethp(){
        return hp;
    }
    int getAttack(){
        return attack;
    }
    int getDef(){
        return def;
    }
    int getKeys(){
        return keys;
    }

    void setPlay(int r, int c){
        rows = r;
        col = c;
    }
    void changehp(int amount){
        hp += amount;
    }
    void changeAttack(int amount){
        attack += amount;
    }
    void changeDef(int amount){
        def += amount;
    }

    void addKey(){
        keys++;
    }
    bool useKey(){
        if(keys > 0){
            keys--;
            return true;
        }
        return false;
    }
};

class Enemy{
private:
    int hp;
    int attack;
    int def;
public:
    Enemy(int h, int a, int d){
        hp = h;
        attack = a;
        def = d;
    }
    int getHp(){
        return hp;
    }
    int getAttack(){
        return attack;
    }
    int getDef(){
        return def;
    }
};

struct DungeonInfo{
    string name;
    int usedRows;
    int usedCols;
    int spawnR;
    int spawnC;
    char grid[ROWS][COLS];
};

int readIntRange(string user, int low, int high){
    int x;

    while(true){
        cout << user;

        if(!(cin >> x)){
            cin.clear();

            char ch;
            while(cin.get(ch) && ch != '\n'){ }

            cout << "Invalid. Try again." << endl;
            continue;
        }

        char ch;
        while(cin.get(ch) && ch != '\n'){ }

        if(x >= low && x <= high) {
            return x;}

        cout << "Invalid. Try again." << endl;
    }
}

bool battle(Player &p, Enemy &e){
    int playerHP = p.gethp();
    int enemyHP = e.getHp();

    cout << "Battle started!" << endl;

    while(playerHP > 0 && enemyHP > 0){
        int enemyDamage = p.getAttack() - e.getDef();
        if(enemyDamage < 0) enemyDamage = 0;
        enemyHP -= enemyDamage;

        if(enemyHP <= 0) break;

        int playerDamage = e.getAttack() - p.getDef();
        if(playerDamage < 0) playerDamage = 0;
        playerHP -= playerDamage;
    }

    p.changehp(playerHP - p.gethp());

    if(playerHP > 0){
        cout << "You defeated the enemy!" << endl;
        return true;
    }
    else {
        cout << "You were defeated." << endl;
        return false;
    }
}

void makeDungeon(char grid[ROWS][COLS]){
    for(int r = 0; r < ROWS; r++){
        for(int c = 0; c < COLS; c++){
            grid[r][c] = ' ';
        }
    }

    for (int c = 0; c < COLS; c++){
        grid[0][c] = '#';
        grid[ROWS-1][c] = '#';
    }
    for (int r = 0; r < ROWS; r++){
        grid[r][0] = '#';
        grid[r][COLS-1] = '#';
    }
}

void makeDungeonSized(char grid[ROWS][COLS], int usedRows, int usedCols){
    for(int r = 0; r < ROWS; r++){
        for(int c = 0; c < COLS; c++){
            grid[r][c] = '#';
        }
    }

    for(int r = 0; r < usedRows; r++){
        for(int c = 0; c < usedCols; c++){
            grid[r][c] = ' ';
        }
    }

    for(int c = 0; c < usedCols; c++){
        grid[0][c] = '#';
        grid[usedRows-1][c] = '#';
    }
    for(int r = 0; r < usedRows; r++){
        grid[r][0] = '#';
        grid[r][usedCols-1] = '#';
    }
}

void printGame(char grid[ROWS][COLS], Player &p, int usedRows, int usedCols){
    for (int r = 0; r < usedRows; r++){
        for(int c = 0; c < usedCols; c++){
            if(r == p.getRow() && c == p.getCol()){
                cout << '@';
            }
            else {
                cout << grid[r][c];
            }
        }
        cout << endl;
    }
    cout << "Hp: " << p.gethp() << endl;
    cout << "Attack: " << p.getAttack() << endl;
    cout << "Def: " << p.getDef() << endl;
    cout << "Keys: " << p.getKeys() << endl;
}

void printEditor(char grid[ROWS][COLS], int usedRows, int usedCols){
    cout << "   ";
    for(int c = 1; c <= usedCols; c++){
        cout << (c % 10) << ' ';
    }
    cout << endl;

    for(int r = 0; r < usedRows; r++){
        cout << (r + 1);
        if(r + 1 < 10) cout << "  ";
        else cout << " ";

        for(int c = 0; c < usedCols; c++){
            cout << grid[r][c] << ' ';
        }
        cout << endl;
    }
}

int movePlayer(char grid[ROWS][COLS], Player &p, char input){
    int newRow = p.getRow();
    int newCol = p.getCol();

    input = (char)tolower(input);

    if(input == 'u') newRow--;
    else if(input == 'd') newRow++;
    else if(input == 'l') newCol--;
    else if(input == 'r') newCol++;
    else{
        cout << "Invalid move." << endl;
        return 0;
    }

    char tile = grid[newRow][newCol];

    if(tile == '#'){
        cout << "You hit a wall!" << endl;
        return 0;
    }
    else if(tile == 'K'){
        cout << "You picked up a key!" << endl;
        p.addKey();
        grid[newRow][newCol] = ' ';
    }
    else if(tile == 'D'){
        if(p.useKey()){
            cout << "You opened the door." << endl;
            grid[newRow][newCol] = ' ';
        }
        else {
            cout << "You need a key!" << endl;
            return 0;
        }
    }
    else if(tile == 'H'){
        cout << "You gained 10 HP!" << endl;
        p.changehp(10);
        grid[newRow][newCol] = ' ';
    }
    else if(tile == 'S'){
        cout << "You gained 4 attack!" << endl;
        p.changeAttack(4);
        grid[newRow][newCol] = ' ';
    }
    else if(tile == 'F'){
        cout << "You gained 4 defense!" << endl;
        p.changeDef(4);
        grid[newRow][newCol] = ' ';
    }
    else if(tile == 'E'){
        Enemy enemy(20, 8, 3);
        if(battle(p, enemy)){
            grid[newRow][newCol] = ' ';
        }
        else {
            cout << "Game over." << endl;
            return 2;
        }
    }
    else if(tile == 'X'){
        cout << "You reached the goal! You win!" << endl;
        return 1;
    }

    p.setPlay(newRow, newCol);
    return 0;
}

bool parseMove(string in, char &moveChar){
    for(char &ch : in) ch = (char)tolower(ch);

    if(in == "l" || in == "left"){ moveChar = 'l'; return true; }
    if(in == "r" || in == "right"){ moveChar = 'r'; return true; }
    if(in == "u" || in == "up"){ moveChar = 'u'; return true; }
    if(in == "d" || in == "down"){ moveChar = 'd'; return true; }
    return false;
}

DungeonInfo loadDungeon1(){
    DungeonInfo d;
    d.name = "Tutorial";
    d.usedRows = 10;
    d.usedCols = 10;
    d.spawnR = 1;
    d.spawnC = 1;

    makeDungeon(d.grid);

    d.grid[1][3] = 'K';
    d.grid[2][5] = 'H';
    d.grid[3][6] = 'S';
    d.grid[4][4] = 'F';
    d.grid[5][7] = 'D';
    d.grid[6][2] = 'E';
    d.grid[8][8] = 'X';

    return d;
}

DungeonInfo loadDungeon2(){
    DungeonInfo d;
    d.name = "Level 1";
    d.usedRows = 10;
    d.usedCols = 10;
    d.spawnR = 1;
    d.spawnC = 1;

    makeDungeon(d.grid);

    d.grid[1][2] = 'E';
    d.grid[2][6] = 'H';
    d.grid[3][2] = 'K';
    d.grid[3][3] = 'D';
    d.grid[4][6] = 'S';
    d.grid[5][5] = 'F';
    d.grid[8][8] = 'X';

    d.grid[2][2] = '#';
    d.grid[2][3] = '#';
    d.grid[2][4] = '#';

    return d;
}

void playDungeon(const DungeonInfo& d){
    char grid[ROWS][COLS];
    for(int r = 0; r < ROWS; r++){
        for(int c = 0; c < COLS; c++){
            grid[r][c] = d.grid[r][c];
        }
    }

    Player player(d.spawnR, d.spawnC, 50, 10, 5);

    while(true){
        printGame(grid, player, d.usedRows, d.usedCols);

        cout << "Enter move (L,R,U,D) or Q to quit: ";
        string in;
        cin >> in;

        for(char &ch : in) ch = (char)tolower(ch);
        if(in == "q"){
            return;
        }

        char moveChar;
        if(!parseMove(in, moveChar)){
            cout << "Invalid move input." << endl;
            continue;
        }

        int result = movePlayer(grid, player, moveChar);

        if(result == 1){
            cout << "Returning to main menu" << endl;
            return;
        }
        if(result == 2){
            cout << "Returning to main menu" << endl;
            return;
        }
    }
}

void showObjectList(){
    cout << "1) Empty" << endl;
    cout << "2) Wall (#)" << endl;
    cout << "3) Player start (@)" << endl;
    cout << "4) Goal (X)" << endl;
    cout << "5) Key (K)" << endl;
    cout << "6) Locked door (D)" << endl;
    cout << "7) Enemy (E)" << endl;
    cout << "8) Health potion (H)" << endl;
    cout << "9) Strength potion (S)" << endl;
    cout << "10) Defense potion (F)" << endl;
}

char objectNumberToChar(int n){
    if(n == 1) return ' ';
    if(n == 2) return '#';
    if(n == 3) return '@';
    if(n == 4) return 'X';
    if(n == 5) return 'K';
    if(n == 6) return 'D';
    if(n == 7) return 'E';
    if(n == 8) return 'H';
    if(n == 9) return 'S';
    if(n == 10) return 'F';
    return ' ';
}

DungeonInfo runEditor(){
    DungeonInfo d;
    d.name = "";

    int usedRows = readIntRange("Enter rows (5-10): ", 5, 10);
    int usedCols = readIntRange("Enter cols (5-10): ", 5, 10);

    d.usedRows = usedRows;
    d.usedCols = usedCols;

    makeDungeonSized(d.grid, usedRows, usedCols);

    int spawnR = 1;
    int spawnC = 1;
    bool hasSpawn = false;
    bool hasGoal = false;

    while(true){
        cout << endl << "Level Editor" << endl;
        printEditor(d.grid, usedRows, usedCols);
        cout << "1) Add an object" << endl;
        cout << "2) Save dungeon" << endl;
        cout << "3) Cancel" << endl;

        int choice = readIntRange("Choose: ", 1, 3);

        if(choice == 3){
            d.name = "";
            return d;
        }

        if(choice == 1){
            showObjectList();
            int obj = readIntRange("Select object (1-10): ", 1, 10);

            int r = readIntRange("Row (1..rows): ", 1, usedRows);
            int c = readIntRange("Col (1..cols): ", 1, usedCols);

            int rr = r - 1;
            int cc = c - 1;

            if(rr == 0 || cc == 0 || rr == usedRows-1 || cc == usedCols-1){
                cout << "Can't place on border walls." << endl;
                continue;
            }

            char place = objectNumberToChar(obj);

            if(place == '@'){
                for(int i=0;i<usedRows;i++){
                    for(int j=0;j<usedCols;j++){
                        if(d.grid[i][j] == '@') {
                            d.grid[i][j] = ' ';
                        }
                    }
                }
                hasSpawn = true;
                spawnR = rr;
                spawnC = cc;
            }

            if(place == 'X') hasGoal = true;

            d.grid[rr][cc] = place;
            cout << "Placed." << endl;
        }

        if(choice == 2){
            if(!hasSpawn){
                cout << "You must place a player start (@) before saving." << endl;
                continue;
            }
            if(!hasGoal){
                cout << "You must place a goal (X) before saving." << endl;
                continue;
            }

            cout << "Enter a name for your dungeon: " << endl;
            cin >> d.name;

            d.spawnR = spawnR;
            d.spawnC = spawnC;

            d.grid[spawnR][spawnC] = ' ';

            cout << "Dungeon saved!" << endl;
            return d;
        }
    }
}

void printMainMenu(){
    cout << "Welcome to Magic Tower" << endl;
    cout << "1) Enter a dungeon" << endl;
    cout << "2) Design a dungeon" << endl;
    cout << "3) Exit" << endl;
}

void listDungeons(const vector<DungeonInfo>& dungeons){
    cout << "Available dungeons:" << endl;
    for(int i = 0; i < (int)dungeons.size(); i++){
        cout << (i+1) << ") " << dungeons[i].name << endl;
    }
}

int main() {
    vector<DungeonInfo> dungeons;
    dungeons.push_back(loadDungeon1());
    dungeons.push_back(loadDungeon2());

    while(true){
        printMainMenu();
        int choice = readIntRange("Choose: ", 1, 3);

        if(choice == 3){
            cout << "Goodbye!" << endl;
            break;
        }

        if(choice == 1){
            listDungeons(dungeons);
            int pick = readIntRange("Select a dungeon number: ", 1, (int)dungeons.size());
            playDungeon(dungeons[pick-1]);
        }

        if(choice == 2){
            DungeonInfo created = runEditor();
            if(created.name.size() == 0){
                cout << "Editor canceled." << endl;
            }
            else {
                dungeons.push_back(created);
                cout << "Added \"" << created.name << "\" to playable dungeons." << endl;
            }
        }
    }
}
