//
//  main.cpp
//  Portfolio
//
//  Created by Marina Balamoon on 2/9/26.
//

#include <iostream>
#include <vector>
#include <string>
#include <cctype>

using namespace std;

const int ROWS = 10;
const int COLS = 10;

const int LAVA_DAMAGE = 5;
const int SPIKE_DAMAGE = 3;
const int POISON_DAMAGE = 2;

class Player{
private:
    int rows;
    int col;
    int hp;
    int attack;
    int def;
    int blueKeys;
    int redKeys;

public:
    Player(int r, int c, int h, int a, int d){
        rows = r;
        col = c;
        hp = h;
        attack = a;
        def = d;
        blueKeys = 0;
        redKeys = 0;
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

    int getBlueKeys(){
        return blueKeys;
    }

    int getRedKeys(){
        return redKeys;
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

    void addBlueKey(){
        blueKeys++;
    }

    void addRedKey(){
        redKeys++;
    }

    bool useBlueKey(){
        if(blueKeys > 0){
            blueKeys--;
            return true;
        }
        return false;
    }

    bool useRedKey(){
        if(redKeys > 0){
            redKeys--;
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

    int enemyHp[ROWS][COLS];
    int enemyAttack[ROWS][COLS];
    int enemyDef[ROWS][COLS];

    int potionAmount[ROWS][COLS];
};

string trimSpaces(string s){
    int start = 0;
    while(start < (int)s.size() && isspace((unsigned char)s[start])){
        start++;
    }

    int end = (int)s.size() - 1;
    while(end >= start && isspace((unsigned char)s[end])){
        end--;
    }

    string out = "";
    for(int i = start; i <= end; i++){
        out += s[i];
    }

    return out;
}

string makeLower(string s){
    for(int i = 0; i < (int)s.size(); i++){
        s[i] = (char)tolower((unsigned char)s[i]);
    }
    return s;
}

string readLineText(string prompt){
    string s;
    cout << prompt;
    getline(cin, s);
    s = trimSpaces(s);
    return s;
}

string readLineLower(string prompt){
    return makeLower(readLineText(prompt));
}

bool isNumberString(string s){
    if(s.size() == 0){
        return false;
    }

    for(int i = 0; i < (int)s.size(); i++){
        if(!isdigit((unsigned char)s[i])){
            return false;
        }
    }

    return true;
}

int stringToInt(string s){
    int value = 0;
    for(int i = 0; i < (int)s.size(); i++){
        value = value * 10 + (s[i] - '0');
    }
    return value;
}

int readIntRange(string user, int low, int high){
    while(true){
        string s = readLineText(user);

        if(!isNumberString(s)){
            cout << "Invalid. Try again." << endl;
        }
        else{
            int x = stringToInt(s);

            if(x >= low && x <= high){
                return x;
            }
            else{
                cout << "Invalid. Try again." << endl;
            }
        }
    }
}

void clearArrays(DungeonInfo &d){
    for(int r = 0; r < ROWS; r++){
        for(int c = 0; c < COLS; c++){
            d.enemyHp[r][c] = 0;
            d.enemyAttack[r][c] = 0;
            d.enemyDef[r][c] = 0;
            d.potionAmount[r][c] = 0;
        }
    }
}

bool battle(Player &p, Enemy &e){
    int playerHP = p.gethp();
    int enemyHP = e.getHp();

    cout << "Battle started!" << endl;

    while(playerHP > 0 && enemyHP > 0){
        int enemyDamage = p.getAttack() - e.getDef();
        if(enemyDamage < 0){
            enemyDamage = 0;
        }

        cout << "You attack the enemy for " << enemyDamage << " damage." << endl;
        enemyHP -= enemyDamage;

        if(enemyHP > 0){
            int playerDamage = e.getAttack() - p.getDef();
            if(playerDamage < 0){
                playerDamage = 0;
            }

            cout << "The enemy attacks you for " << playerDamage << " damage." << endl;
            playerHP -= playerDamage;

            cout << "Your HP: " << playerHP << endl;
            cout << "Enemy HP: " << enemyHP << endl;
        }
    }

    p.changehp(playerHP - p.gethp());

    if(playerHP > 0){
        cout << "You defeated the enemy!" << endl;
        return true;
    }
    else{
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

    for(int c = 0; c < COLS; c++){
        grid[0][c] = '#';
        grid[ROWS - 1][c] = '#';
    }

    for(int r = 0; r < ROWS; r++){
        grid[r][0] = '#';
        grid[r][COLS - 1] = '#';
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
        grid[usedRows - 1][c] = '#';
    }

    for(int r = 0; r < usedRows; r++){
        grid[r][0] = '#';
        grid[r][usedCols - 1] = '#';
    }
}

void printLegend(){
    cout << "Legend:" << endl;
    cout << "@ = Player" << endl;
    cout << "# = Wall" << endl;
    cout << "X = Goal" << endl;
    cout << "K = Blue key" << endl;
    cout << "D = Blue door" << endl;
    cout << "R = Red key" << endl;
    cout << "d = Red door" << endl;
    cout << "E = Enemy" << endl;
    cout << "H = Health potion" << endl;
    cout << "S = Strength potion" << endl;
    cout << "F = Defense potion" << endl;
    cout << "L = Lava floor" << endl;
    cout << "T = Spike floor" << endl;
    cout << "P = Poison floor" << endl;
    cout << endl;
}

void printGame(char grid[ROWS][COLS], Player &p, int usedRows, int usedCols){
    for(int r = 0; r < usedRows; r++){
        for(int c = 0; c < usedCols; c++){
            if(r == p.getRow() && c == p.getCol()){
                cout << '@';
            }
            else{
                cout << grid[r][c];
            }
        }
        cout << endl;
    }

    cout << "Hp: " << p.gethp() << endl;
    cout << "Attack: " << p.getAttack() << endl;
    cout << "Def: " << p.getDef() << endl;
    cout << "Blue keys: " << p.getBlueKeys() << endl;
    cout << "Red keys: " << p.getRedKeys() << endl;
}

void printEditor(char grid[ROWS][COLS], int usedRows, int usedCols, int spawnR, int spawnC){
    cout << "   ";
    for(int c = 1; c <= usedCols; c++){
        cout << (c % 10) << ' ';
    }
    cout << endl;

    for(int r = 0; r < usedRows; r++){
        cout << (r + 1);
        if(r + 1 < 10){
            cout << "  ";
        }
        else{
            cout << " ";
        }

        for(int c = 0; c < usedCols; c++){
            if(r == spawnR && c == spawnC){
                cout << '@' << ' ';
            }
            else{
                cout << grid[r][c] << ' ';
            }
        }
        cout << endl;
    }
}

void inspectSquare(DungeonInfo &d, int r, int c, bool showSpawn){
    if(r < 0 || r >= d.usedRows || c < 0 || c >= d.usedCols){
        cout << "Out of bounds." << endl;
    }
    else if(showSpawn && r == d.spawnR && c == d.spawnC){
        cout << "This square has the player start (@)." << endl;
    }
    else{
        char tile = d.grid[r][c];

        if(tile == ' '){
            cout << "This square is empty." << endl;
        }
        else if(tile == '#'){
            cout << "This square has a wall." << endl;
        }
        else if(tile == 'X'){
            cout << "This square has the goal." << endl;
        }
        else if(tile == 'K'){
            cout << "This square has a blue key." << endl;
        }
        else if(tile == 'D'){
            cout << "This square has a blue door." << endl;
        }
        else if(tile == 'R'){
            cout << "This square has a red key." << endl;
        }
        else if(tile == 'd'){
            cout << "This square has a red door." << endl;
        }
        else if(tile == 'E'){
            cout << "This square has an enemy." << endl;
            cout << "HP: " << d.enemyHp[r][c] << endl;
            cout << "Attack: " << d.enemyAttack[r][c] << endl;
            cout << "Defense: " << d.enemyDef[r][c] << endl;
        }
        else if(tile == 'H'){
            cout << "This square has a health potion." << endl;
            cout << "Amount: " << d.potionAmount[r][c] << endl;
        }
        else if(tile == 'S'){
            cout << "This square has a strength potion." << endl;
            cout << "Amount: " << d.potionAmount[r][c] << endl;
        }
        else if(tile == 'F'){
            cout << "This square has a defense potion." << endl;
            cout << "Amount: " << d.potionAmount[r][c] << endl;
        }
        else if(tile == 'L'){
            cout << "This square has lava." << endl;
            cout << "Damage: " << LAVA_DAMAGE << endl;
        }
        else if(tile == 'T'){
            cout << "This square has spikes." << endl;
            cout << "Damage: " << SPIKE_DAMAGE << endl;
        }
        else if(tile == 'P'){
            cout << "This square has poison." << endl;
            cout << "Damage: " << POISON_DAMAGE << endl;
        }
    }
}

int movePlayer(DungeonInfo &d, Player &p, char input){
    int newRow = p.getRow();
    int newCol = p.getCol();

    if(input == 'u'){
        newRow--;
    }
    else if(input == 'd'){
        newRow++;
    }
    else if(input == 'l'){
        newCol--;
    }
    else if(input == 'r'){
        newCol++;
    }
    else{
        cout << "Invalid move." << endl;
        return 0;
    }

    char tile = d.grid[newRow][newCol];

    if(tile == '#'){
        cout << "You hit a wall!" << endl;
        return 0;
    }
    else if(tile == 'K'){
        cout << "You picked up a blue key!" << endl;
        p.addBlueKey();
        d.grid[newRow][newCol] = ' ';
    }
    else if(tile == 'R'){
        cout << "You picked up a red key!" << endl;
        p.addRedKey();
        d.grid[newRow][newCol] = ' ';
    }
    else if(tile == 'D'){
        if(p.useBlueKey()){
            cout << "You opened the blue door." << endl;
            d.grid[newRow][newCol] = ' ';
        }
        else{
            cout << "You need a blue key!" << endl;
            return 0;
        }
    }
    else if(tile == 'd'){
        if(p.useRedKey()){
            cout << "You opened the red door." << endl;
            d.grid[newRow][newCol] = ' ';
        }
        else{
            cout << "You need a red key!" << endl;
            return 0;
        }
    }
    else if(tile == 'H'){
        cout << "You gained " << d.potionAmount[newRow][newCol] << " HP!" << endl;
        p.changehp(d.potionAmount[newRow][newCol]);
        d.grid[newRow][newCol] = ' ';
        d.potionAmount[newRow][newCol] = 0;
    }
    else if(tile == 'S'){
        cout << "You gained " << d.potionAmount[newRow][newCol] << " attack!" << endl;
        p.changeAttack(d.potionAmount[newRow][newCol]);
        d.grid[newRow][newCol] = ' ';
        d.potionAmount[newRow][newCol] = 0;
    }
    else if(tile == 'F'){
        cout << "You gained " << d.potionAmount[newRow][newCol] << " defense!" << endl;
        p.changeDef(d.potionAmount[newRow][newCol]);
        d.grid[newRow][newCol] = ' ';
        d.potionAmount[newRow][newCol] = 0;
    }
    else if(tile == 'E'){
        Enemy enemy(d.enemyHp[newRow][newCol],
                    d.enemyAttack[newRow][newCol],
                    d.enemyDef[newRow][newCol]);

        if(battle(p, enemy)){
            d.grid[newRow][newCol] = ' ';
            d.enemyHp[newRow][newCol] = 0;
            d.enemyAttack[newRow][newCol] = 0;
            d.enemyDef[newRow][newCol] = 0;
        }
        else{
            cout << "Game over." << endl;
            return 2;
        }
    }
    else if(tile == 'L'){
        cout << "You stepped on lava and lost " << LAVA_DAMAGE << " HP!" << endl;
        p.changehp(-LAVA_DAMAGE);
    }
    else if(tile == 'T'){
        cout << "You stepped on spikes and lost " << SPIKE_DAMAGE << " HP!" << endl;
        p.changehp(-SPIKE_DAMAGE);
    }
    else if(tile == 'P'){
        cout << "You stepped on poison and lost " << POISON_DAMAGE << " HP!" << endl;
        p.changehp(-POISON_DAMAGE);
    }
    else if(tile == 'X'){
        cout << "You reached the goal! You win!" << endl;
        return 1;
    }

    p.setPlay(newRow, newCol);

    if(p.gethp() <= 0){
        cout << "You died." << endl;
        return 2;
    }

    return 0;
}

bool parseMove(string in, char &moveChar){
    in = makeLower(trimSpaces(in));

    if(in == "a" || in == "left"){
        moveChar = 'l';
        return true;
    }
    else if(in == "d" || in == "right"){
        moveChar = 'r';
        return true;
    }
    else if(in == "w" || in == "up"){
        moveChar = 'u';
        return true;
    }
    else if(in == "s" || in == "down"){
        moveChar = 'd';
        return true;
    }

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
    clearArrays(d);

    d.grid[1][3] = 'K';

    d.grid[2][5] = 'H';
    d.potionAmount[2][5] = 12;

    d.grid[3][6] = 'S';
    d.potionAmount[3][6] = 4;

    d.grid[4][4] = 'F';
    d.potionAmount[4][4] = 3;

    d.grid[5][7] = 'D';

    d.grid[6][2] = 'E';
    d.enemyHp[6][2] = 20;
    d.enemyAttack[6][2] = 8;
    d.enemyDef[6][2] = 3;

    d.grid[7][4] = 'L';
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
    clearArrays(d);

    d.grid[1][2] = 'E';
    d.enemyHp[1][2] = 15;
    d.enemyAttack[1][2] = 6;
    d.enemyDef[1][2] = 2;

    d.grid[2][6] = 'H';
    d.potionAmount[2][6] = 8;

    d.grid[3][2] = 'K';
    d.grid[3][3] = 'D';

    d.grid[4][6] = 'S';
    d.potionAmount[4][6] = 5;

    d.grid[5][5] = 'F';
    d.potionAmount[5][5] = 4;

    d.grid[6][6] = 'R';
    d.grid[6][7] = 'd';
    d.grid[7][2] = 'T';
    d.grid[7][6] = 'P';

    d.grid[8][8] = 'X';

    d.grid[2][2] = '#';
    d.grid[2][3] = '#';
    d.grid[2][4] = '#';

    return d;
}

void playDungeon(const DungeonInfo& original){
    DungeonInfo d = original;
    Player player(d.spawnR, d.spawnC, 50, 10, 5);

    printLegend();

    while(true){
        printGame(d.grid, player, d.usedRows, d.usedCols);

        string in = readLineLower("Enter move (W,A,S,D), inspect, or Q to quit: ");

        if(in == "q" || in == "quit"){
            return;
        }
        else if(in == "inspect"){
            int r = readIntRange("Row to inspect: ", 1, d.usedRows);
            int c = readIntRange("Col to inspect: ", 1, d.usedCols);
            inspectSquare(d, r - 1, c - 1, false);
        }
        else{
            char moveChar;
            if(!parseMove(in, moveChar)){
                cout << "Invalid move input." << endl;
            }
            else{
                int result = movePlayer(d, player, moveChar);

                if(result == 1 || result == 2){
                    cout << "Returning to main menu" << endl;
                    return;
                }
            }
        }
    }
}

void showObjectList(){
    cout << "empty" << endl;
    cout << "wall" << endl;
    cout << "start" << endl;
    cout << "goal" << endl;
    cout << "bluekey" << endl;
    cout << "bluedoor" << endl;
    cout << "redkey" << endl;
    cout << "reddoor" << endl;
    cout << "enemy" << endl;
    cout << "health" << endl;
    cout << "strength" << endl;
    cout << "defense" << endl;
    cout << "damagefloor" << endl;
}

char objectWordToChar(string s){
    s = makeLower(trimSpaces(s));

    if(s == "empty") return ' ';
    if(s == "wall") return '#';
    if(s == "start") return '@';
    if(s == "goal") return 'X';
    if(s == "bluekey") return 'K';
    if(s == "bluedoor") return 'D';
    if(s == "redkey") return 'R';
    if(s == "reddoor") return 'd';
    if(s == "enemy") return 'E';
    if(s == "health") return 'H';
    if(s == "strength") return 'S';
    if(s == "defense") return 'F';

    return '?';
}

void addObjectData(DungeonInfo &d, int rr, int cc, char place){
    d.enemyHp[rr][cc] = 0;
    d.enemyAttack[rr][cc] = 0;
    d.enemyDef[rr][cc] = 0;
    d.potionAmount[rr][cc] = 0;

    if(place == 'E'){
        d.enemyHp[rr][cc] = readIntRange("Enemy HP: ", 1, 999);
        d.enemyAttack[rr][cc] = readIntRange("Enemy attack: ", 0, 999);
        d.enemyDef[rr][cc] = readIntRange("Enemy defense: ", 0, 999);
    }
    else if(place == 'H'){
        d.potionAmount[rr][cc] = readIntRange("Health amount: ", 1, 999);
    }
    else if(place == 'S'){
        d.potionAmount[rr][cc] = readIntRange("Strength amount: ", 1, 999);
    }
    else if(place == 'F'){
        d.potionAmount[rr][cc] = readIntRange("Defense amount: ", 1, 999);
    }
}

void addDamageFloor(DungeonInfo &d, int rr, int cc){
    string type = readLineLower("Type floor (lava / spikes / poison): ");

    if(type == "lava"){
        d.grid[rr][cc] = 'L';
        cout << "Placed lava." << endl;
    }
    else if(type == "spikes"){
        d.grid[rr][cc] = 'T';
        cout << "Placed spikes." << endl;
    }
    else if(type == "poison"){
        d.grid[rr][cc] = 'P';
        cout << "Placed poison." << endl;
    }
    else{
        cout << "Invalid floor type." << endl;
    }
}

void editDungeon(DungeonInfo &d){
    bool hasGoal = false;

    for(int r = 0; r < d.usedRows; r++){
        for(int c = 0; c < d.usedCols; c++){
            if(d.grid[r][c] == 'X'){
                hasGoal = true;
            }
        }
    }

    while(true){
        cout << endl << "Level Editor" << endl;
        printEditor(d.grid, d.usedRows, d.usedCols, d.spawnR, d.spawnC);
        cout << "Type add, inspect, save, or cancel" << endl;

        string choice = readLineLower("Choose: ");

        if(choice == "cancel"){
            return;
        }
        else if(choice == "inspect"){
            int r = readIntRange("Row: ", 1, d.usedRows);
            int c = readIntRange("Col: ", 1, d.usedCols);
            inspectSquare(d, r - 1, c - 1, true);
        }
        else if(choice == "add"){
            showObjectList();

            string word = readLineLower("Type object name: ");
            int r = readIntRange("Row (1..rows): ", 1, d.usedRows);
            int c = readIntRange("Col (1..cols): ", 1, d.usedCols);

            int rr = r - 1;
            int cc = c - 1;

            if(rr == 0 || cc == 0 || rr == d.usedRows - 1 || cc == d.usedCols - 1){
                cout << "Can't place on border walls." << endl;
            }
            else if(word == "damagefloor"){
                addDamageFloor(d, rr, cc);
            }
            else{
                char place = objectWordToChar(word);

                if(place == '?'){
                    cout << "Invalid object." << endl;
                }
                else if(place == '@'){
                    d.spawnR = rr;
                    d.spawnC = cc;
                    cout << "Placed." << endl;
                }
                else{
                    if(place == 'X'){
                        hasGoal = true;
                    }

                    d.grid[rr][cc] = place;
                    addObjectData(d, rr, cc, place);
                    cout << "Placed." << endl;
                }
            }
        }
        else if(choice == "save"){
            if(!hasGoal){
                cout << "You must place a goal (X) before saving." << endl;
            }
            else{
                cout << "Dungeon updated!" << endl;
                return;
            }
        }
        else{
            cout << "Invalid choice." << endl;
        }
    }
}

DungeonInfo runEditor(){
    DungeonInfo d;
    d.name = "";

    int usedRows = readIntRange("Enter rows (5-10): ", 5, 10);
    int usedCols = readIntRange("Enter cols (5-10): ", 5, 10);

    d.usedRows = usedRows;
    d.usedCols = usedCols;

    makeDungeonSized(d.grid, usedRows, usedCols);
    clearArrays(d);

    int spawnR = 1;
    int spawnC = 1;
    bool hasSpawn = false;
    bool hasGoal = false;

    d.spawnR = spawnR;
    d.spawnC = spawnC;

    while(true){
        cout << endl << "Level Editor" << endl;
        printEditor(d.grid, usedRows, usedCols, d.spawnR, d.spawnC);
        cout << "1) Add an object" << endl;
        cout << "2) Save dungeon" << endl;
        cout << "3) Inspect a square" << endl;
        cout << "4) Cancel" << endl;
        cout << "You can also type add, save, inspect, or cancel" << endl;

        string choice = readLineLower("Choose: ");

        if(choice == "4" || choice == "cancel"){
            d.name = "";
            return d;
        }
        else if(choice == "3" || choice == "inspect"){
            int r = readIntRange("Row: ", 1, usedRows);
            int c = readIntRange("Col: ", 1, usedCols);
            inspectSquare(d, r - 1, c - 1, true);
        }
        else if(choice == "1" || choice == "add"){
            showObjectList();

            string word = readLineLower("Type object name: ");
            int r = readIntRange("Row (1..rows): ", 1, usedRows);
            int c = readIntRange("Col (1..cols): ", 1, usedCols);

            int rr = r - 1;
            int cc = c - 1;

            if(rr == 0 || cc == 0 || rr == usedRows - 1 || cc == usedCols - 1){
                cout << "Can't place on border walls." << endl;
            }
            else if(word == "damagefloor"){
                addDamageFloor(d, rr, cc);
            }
            else{
                char place = objectWordToChar(word);

                if(place == '?'){
                    cout << "Invalid object." << endl;
                }
                else if(place == '@'){
                    hasSpawn = true;
                    spawnR = rr;
                    spawnC = cc;
                    d.spawnR = spawnR;
                    d.spawnC = spawnC;
                    cout << "Placed." << endl;
                }
                else{
                    if(place == 'X'){
                        hasGoal = true;
                    }

                    d.grid[rr][cc] = place;
                    addObjectData(d, rr, cc, place);
                    cout << "Placed." << endl;
                }
            }
        }
        else if(choice == "2" || choice == "save"){
            if(!hasSpawn){
                cout << "You must place a player start (@) before saving." << endl;
            }
            else if(!hasGoal){
                cout << "You must place a goal (X) before saving." << endl;
            }
            else{
                d.name = readLineText("Enter a name for your dungeon: ");
                d.spawnR = spawnR;
                d.spawnC = spawnC;
                cout << "Dungeon saved!" << endl;
                return d;
            }
        }
        else{
            cout << "Invalid choice." << endl;
        }
    }
}

void printMainMenu(){
    cout << "Welcome to Magic Tower" << endl;
    cout << "1) Enter a dungeon" << endl;
    cout << "2) Design a dungeon" << endl;
    cout << "3) Exit" << endl;
    cout << "You can also type enter, design, or exit" << endl;
}

void printDesignMenu(){
    cout << "1) Create a new dungeon" << endl;
    cout << "2) Edit an existing dungeon" << endl;
    cout << "3) Back" << endl;
    cout << "You can also type create, edit, or back" << endl;
}

void listDungeons(const vector<DungeonInfo>& dungeons){
    cout << "Available dungeons:" << endl;
    for(int i = 0; i < (int)dungeons.size(); i++){
        cout << (i + 1) << ") " << dungeons[i].name << endl;
    }
}

int chooseDungeon(const vector<DungeonInfo>& dungeons){
    while(true){
        string pick = readLineLower("Type a dungeon number or name: ");

        if(isNumberString(pick)){
            int value = stringToInt(pick);
            if(value >= 1 && value <= (int)dungeons.size()){
                return value - 1;
            }
        }

        for(int i = 0; i < (int)dungeons.size(); i++){
            if(makeLower(dungeons[i].name) == pick){
                return i;
            }
        }

        if(pick == "tutorial dungeon"){
            for(int i = 0; i < (int)dungeons.size(); i++){
                if(makeLower(dungeons[i].name) == "tutorial"){
                    return i;
                }
            }
        }
        else if(pick == "level 1 dungeon"){
            for(int i = 0; i < (int)dungeons.size(); i++){
                if(makeLower(dungeons[i].name) == "level 1"){
                    return i;
                }
            }
        }

        cout << "Invalid dungeon choice." << endl;
    }
}

void runDesignMenu(vector<DungeonInfo> &dungeons){
    while(true){
        printDesignMenu();
        string choice = readLineLower("Choose: ");

        if(choice == "3" || choice == "back"){
            return;
        }
        else if(choice == "1" || choice == "create"){
            DungeonInfo created = runEditor();

            if(created.name.size() == 0){
                cout << "Editor canceled." << endl;
            }
            else{
                bool replaced = false;

                for(int i = 0; i < (int)dungeons.size(); i++){
                    if(makeLower(dungeons[i].name) == makeLower(created.name)){
                        dungeons[i] = created;
                        replaced = true;
                    }
                }

                if(replaced){
                    cout << "Updated \"" << created.name << "\"." << endl;
                }
                else{
                    dungeons.push_back(created);
                    cout << "Added \"" << created.name << "\" to playable dungeons." << endl;
                }
            }
        }
        else if(choice == "2" || choice == "edit"){
            listDungeons(dungeons);
            int pick = chooseDungeon(dungeons);
            editDungeon(dungeons[pick]);
        }
        else{
            cout << "Invalid choice." << endl;
        }
    }
}

int main() {
    vector<DungeonInfo> dungeons;
    dungeons.push_back(loadDungeon1());
    dungeons.push_back(loadDungeon2());

    while(true){
        printMainMenu();
        string choice = readLineLower("Choose: ");

        if(choice == "3" || choice == "exit" || choice == "enter a dungeon"){
            if(choice == "enter a dungeon"){
                listDungeons(dungeons);
                int pick = chooseDungeon(dungeons);
                playDungeon(dungeons[pick]);
            }
            else{
                if(choice == "3" || choice == "exit"){
                    cout << "Goodbye!" << endl;
                    break;
                }
            }
        }
        else if(choice == "1" || choice == "enter"){
            listDungeons(dungeons);
            int pick = chooseDungeon(dungeons);
            playDungeon(dungeons[pick]);
        }
        else if(choice == "2" || choice == "design" || choice == "design a dungeon"){
            runDesignMenu(dungeons);
        }
        else{
            cout << "Invalid choice." << endl;
        }
    }
}
