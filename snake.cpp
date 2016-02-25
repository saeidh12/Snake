//
//  snake.cpp
//  snake
//
//  Created by Saeid on 11/13/15.
//
//

#include <iostream>
#include <termios.h>
#include <stdio.h>
#include <fcntl.h>
#include <cstdlib>
#include <ctime>


using namespace std;


int kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;
    
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    
    ch = getchar();
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    
    if(ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }
    
    return 0;
}

/*
 
    1
 
 2      3
 
    4
 
 
 */

char map[20][20];
int direction = 3;
bool running = true;

class BODY {
public:
    int x;
    int y;
    
    BODY *next;
    BODY *prev;
} apple;

BODY *dir;

class SNAKE {
public:
    int length;
    BODY *head;
    BODY *tale;
    
    SNAKE() {
        length = 2;
        
        dir = new BODY;
        head = new BODY;
        tale = new BODY;
        
        dir -> x = 8;
        dir -> y = 8;
        dir -> next = NULL;
        dir -> prev = NULL;
        
        head -> x = 8;
        head -> y = 8;
        head -> next = dir;
        head -> prev = tale;
        
        tale -> x = 8;
        tale -> y = 7;
        tale -> next = head;
        tale -> prev = NULL;
        
    }
    
    ~SNAKE() {
        BODY *temp = tale;
        BODY *d = tale;
        for (int i = 0; i < length; i++) {
            temp = temp -> next;
            delete d;
            d = temp;
        }
    }
    
    void add() {
        BODY *newb = new BODY;
        newb -> x = tale -> x;
        newb -> y = tale -> y;
        newb -> next = tale -> next;
        newb -> prev = tale;
        if (tale -> next -> x > tale -> x) {
            tale -> x--;
        }
        else if (tale -> next -> x < tale -> x) {
            tale -> x++;
        }
        if (tale -> next -> y > tale -> y) {
            tale -> y--;
        }
        else if (tale -> next -> y < tale -> y) {
            tale -> y++;
        }
        tale -> next = newb;
        length++;
        
        return;
    }
    
} snake;

void generate_apple() {
    do{
        srand(time(NULL));
        apple.x = (rand() % 10) * 2;
        if (apple.x == 0 || apple.x == 20) {
            apple.x = 5;
        }
        
        srand(time(NULL));
        apple.y = (rand() % 10) * 2;
        if (apple.y == 0 || apple.y == 20) {
            apple.y = 5;
        }
        
    }while(map[apple.x][apple.y] == '*');
    return;
}

void print_map() {
    
    // move the snake
    
    switch (direction) {
        case 1:
            dir -> x++;
            break;
            
        case 2:
            dir -> y--;
            break;
            
        case 3:
            dir -> y++;
            break;
            
        case 4:
            dir -> x--;
            break;
    }
    if (dir -> x == 19) {
        dir -> x = 1;
    }
    if (dir -> x == 0) {
        dir -> x = 18;
    }
    if (dir -> y == 19) {
        dir -> y = 1;
    }
    if (dir -> y == 0) {
        dir -> y = 18;
    }
    
    
    //// check if game ended
    
    if (map[dir -> x][dir -> y] == '*') {
        running = false;
    }
    
    if (map[dir -> x][dir -> y] == '@') {
        generate_apple();
        snake.add();
    }
    
    
    BODY *temp = snake.tale;
    for (int i = 0; i < snake.length; i++) {
        temp -> x = temp -> next -> x;
        temp -> y = temp -> next -> y;
        temp = temp -> next;
    }
    
    // clear the map
    
    for (int i = 1; i < 19; i++) {
        for (int j = 1; j < 19; j++) {
            map[i][j] = ' ';
        }
    }
    
    // draw snake on map
    
    temp = snake.tale;
    for (int i = 0; i < snake.length; i++) {
        map[temp -> x][temp -> y] = '*';
        temp = temp -> next;
    }
    
    map[apple.x][apple.y] = '@';
    
    // Print the new map
    
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            cout << map[i][j];
        }
        cout << endl;
    }

    
    return;
}

void play() {
    while (running) {
        while(!kbhit()) {
            system("clear"); // for the terminal
            print_map();
            if (!running) {
                return;
            }
            usleep(200000);
        }
        char key = getchar();
        switch (key) {
            case 's':
                direction = 1;
                break;
                
            case 'd':
                direction = 3;
                break;
                
            case 'a':
                direction = 2;
                break;
                
            case 'w':
                direction = 4;
                break;
            
            default:
                exit (0);
        }
        
    }
}





int main(void) {
    apple.x = 1;
    apple.y = 1;
    generate_apple();
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j += 19) {
            map[i][j] = '#';
            map[j][i] = '#';
        }
    }
    play();
    cout << endl << "Your length: " << snake.length << endl << "You Lost!" << endl << endl << endl;
    return 0;
}
