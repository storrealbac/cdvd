#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

// clear screen macro
#define clearScreen printf("\e[1;1H\e[2J")

// constants
#define DVD_HEIGHT 7
#define DVD_WIDTH 30

// screen size
#define screen_rows w.ws_row
#define screen_cols w.ws_col

// colors
#define RED     "[31m"
#define GREEN   "[32m"
#define YELLOW  "[33m"
#define BLUE    "[34m"
#define MAGENTA "[35m"
#define CYAN    "[36m"
#define WHITE   "[37m"

// ascii art dvd
const char ASCII_DVD[DVD_HEIGHT][DVD_WIDTH] = {
    {"  /##########\\   /###########."},
    {" |##/   /#####\\ /#/|##|  ./##|"},
    {" |##| ./###)\\####/ |##|  /##/"},
    {" \\#######/'  \\##/  |######/ "},
    {"           ___\\/___           "},
    {".###########################/ "},
    {"\\##########################/ "}
};

// possible colors
const char COLORS[7][4] = { {RED}, {GREEN}, {YELLOW}, {BLUE}, {MAGENTA}, {CYAN}, {WHITE} };

// render dvd by line
void renderDVDLine(int line) {
    for (int i = 0; i < DVD_WIDTH; ++i)
        printf("%c", ASCII_DVD[line][i]);
}

// render entire screen
void render(int x, int y, int screen_x, int screen_y) {

    int i = 0, j = 0, dvd_line = 0;

    for (i = 0; i < screen_y; ++i) {
        for (j = 0; j < screen_x && (i >= y && i <= y+DVD_HEIGHT); ++j) {
            // render dvd
            if (dvd_line < DVD_HEIGHT && j == x ) {
                renderDVDLine(dvd_line);
                ++dvd_line;
                break;
            } else printf(" ");
        }
        printf("\n");
    }

}

int main (int argc, char **argv) {

    struct winsize w;

    int dvd_x = 1, dvd_y = 1;
    int y_direction = 1, x_direction = 1;

    // selected color
    int color_pos = 0;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int initial_rows = screen_rows, initial_cols = screen_cols;

    while(1) {

        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        clearScreen;

        if (initial_rows != screen_rows || initial_cols != screen_cols) {
            printf("You back!\n");
            return 0;
        }

        // render color
        printf("\x1B%s", COLORS[color_pos]);

        // render dvd
        render(dvd_x, dvd_y, screen_cols, screen_rows);

        // change position
        dvd_y += y_direction, dvd_x += x_direction;

        // y collision
        if (dvd_y + DVD_HEIGHT >= screen_rows || dvd_y <= 1) 
            y_direction *= -1, color_pos++;
        
        // x collision
        if (dvd_x + DVD_WIDTH >= screen_cols ||dvd_x <= 1)
            x_direction *= -1, color_pos++;

        // restart colors
        if (color_pos > 6) color_pos = 0;

        // frame delay
        usleep(100000);
    }
    return 0;
}

