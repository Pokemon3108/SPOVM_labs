#include <ncurses.h>
#include <time.h>
#include <unistd.h>

int main()
{

    initscr();
    sleep(5); 
    beep();
   // move(15,40);  
    printw("Child:\n");  
    time_t t;
    t=time(NULL);
    printw(ctime(&t));
   refresh();  
                  
   getch();                     
   endwin();                    
    return 0;
}
