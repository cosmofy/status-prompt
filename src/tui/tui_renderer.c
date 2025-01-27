#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "keys_parser.h"

#define MAX_ROWS 300
#define MAX_COLS 20
#define WINDOW_WIDTH 122 // Width of the window for the line

void init_colors();
void print_table(WINDOW *win, KeyData *json_lines, int row_count);

int main(int argc, char* argv[]) {
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    refresh();

    if (!has_colors()) {
        endwin();
        printf("Your terminal does not support colors.\n");
        return 1;
    }

    // Check for command-line argument
    if (argc < 2) {
        endwin();
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    
    init_colors();
    char* filename = argv[1];

    int row_count = countLine(filename) * 2 + 2;
    // manually determined from keys_parser.h
    int col_count = 6;

    if (row_count == 0) {
        perror("keys_status.txt file not found.");
        endwin();
        return 1;
    }

    KeyData *json_lines = parserFile(filename);

    

    WINDOW *table_win = newwin(row_count + 6, WINDOW_WIDTH, 0, 0);
    if (table_win == NULL) {
        perror("Failed to create window");
        endwin();
        return 1;
    }
    print_table(table_win, json_lines, row_count);

    getch();
    endwin();

    return 0;
}

void init_colors() {
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_BLACK, COLOR_BLACK);
    init_pair(4, COLOR_WHITE, COLOR_BLACK);
}

void print_table(WINDOW *win, KeyData *json_lines, int row_count) {
    int i;
    wbkgd(win, COLOR_PAIR(3));

    wattron(win, COLOR_PAIR(4));
    box(win, 0, 0);
    wattroff(win, COLOR_PAIR(4));
    
    wattroff(win, COLOR_PAIR(3));
    wrefresh(win);

    // There is nothing "smart" I could figure out here. Had to write each case by hand.
    int ctr = 0;
    int space_between = 20;
    int fixed_modifier = 0;

    // manual fixes
    int location_modification = 4;
    int status_modification = -2;
    int cost_modification = -4;
    int manual_status_modifier = -2;
    int manual_key_vals_modifier = -2;
    int manual_cost_modifier = -1;

    wattron(win, COLOR_PAIR(4));
    char line[WINDOW_WIDTH - 1];
    memset(line, '-', WINDOW_WIDTH - 2);  // Fill the array with dashes
    line[WINDOW_WIDTH - 2] = '\0';  // Null-terminate the string 

    mvwprintw(win, 1, 2, "location");
    fixed_modifier = location_modification;
    mvwprintw(win, 1, space_between*(ctr + 1) + fixed_modifier, "|");
    line[space_between*(ctr + 1) + fixed_modifier - 1] = '+';
    ctr++;
    mvwprintw(win, 1, (space_between + 2)*(ctr) + fixed_modifier, "date");
    mvwprintw(win, 1, space_between*(ctr + 1) + fixed_modifier, "|");
    line[space_between*(ctr + 1) + fixed_modifier - 1] = '+';
    ctr++;   
    mvwprintw(win, 1, (space_between + 2)*(ctr) + fixed_modifier, "KeyId");
    mvwprintw(win, 1, space_between*(ctr + 1) + fixed_modifier, "|");
    line[space_between*(ctr + 1) + fixed_modifier - 1] = '+';
    ctr++; 
    mvwprintw(win, 1, (space_between + 2)*(ctr) + fixed_modifier + manual_status_modifier, "Status");
    fixed_modifier = status_modification;
    mvwprintw(win, 1, space_between*(ctr + 1) + fixed_modifier, "|");
    line[space_between*(ctr + 1) + fixed_modifier - 1] = '+';
    ctr++;  
    mvwprintw(win, 1, (space_between + 2)*(ctr) + fixed_modifier, "Key");
    mvwprintw(win, 1, space_between*(ctr + 1) + fixed_modifier, "|");
    line[space_between*(ctr + 1) + fixed_modifier - 1] = '+';
    ctr++;
    fixed_modifier = cost_modification;   
    mvwprintw(win, 1, (space_between + 2)*(ctr) + fixed_modifier + manual_cost_modifier, "Cost Limit");

    
    mvwprintw(win, 2, 1, "%s", line);  // Print the line
    wattroff(win, COLOR_PAIR(4));

    
    for (i = 0; i < row_count - 2; i+=2) {
        ctr = 0;
        wattron(win, COLOR_PAIR(4));
        fixed_modifier = location_modification;
        mvwprintw(win, i + 3, 2, "%-3s", json_lines[i/2].location);
        mvwprintw(win, i + 3, space_between*(ctr + 1) + fixed_modifier, "|");
        ctr++;
        mvwprintw(win, i + 3, (space_between + 2)*(ctr) + fixed_modifier, "%-3s", json_lines[i/2].date);
        mvwprintw(win, i + 3, space_between*(ctr + 1) + fixed_modifier, "|");
        ctr++;
        mvwprintw(win, i + 3, (space_between + 2)*(ctr) + fixed_modifier, "%-3s", json_lines[i/2].keyId);
        mvwprintw(win, i + 3, space_between*(ctr + 1) + fixed_modifier, "|"); 
        ctr++;
        wattroff(win, COLOR_PAIR(4));
        if (json_lines[i/2].revoked == 'F') {
            wattron(win, COLOR_PAIR(1));
            mvwprintw(win, i + 3, (space_between + 2)*(ctr) + fixed_modifier, "%-5s", "OK");
            wattroff(win, COLOR_PAIR(1));
        } else {
            wattron(win, COLOR_PAIR(2));
            mvwprintw(win, i + 3, (space_between + 2)*(ctr) + fixed_modifier, "%-5s", "BAD");
            wattroff(win, COLOR_PAIR(2));
        }
        wattron(win, COLOR_PAIR(4));
        fixed_modifier = status_modification;
        mvwprintw(win, i + 3, space_between*(ctr + 1) + fixed_modifier, "|"); 
        ctr++;
        mvwprintw(win, i + 3, (space_between + 2)*(ctr) + fixed_modifier + manual_key_vals_modifier, "%-3s", json_lines[i/2].key);
        mvwprintw(win, i + 3, space_between*(ctr + 1) + fixed_modifier, "|");
        ctr++;
        fixed_modifier = cost_modification;
        mvwprintw(win, i + 3, (space_between + 2)*(ctr) + fixed_modifier, "%-3f", json_lines[i/2].costLimitInUsdOverTime);
        wattroff(win, COLOR_PAIR(4));

        if (i < row_count - 1) {
            char line[WINDOW_WIDTH - 1];
            memset(line, '-', WINDOW_WIDTH - 2);  // Fill the array with dashes
            line[WINDOW_WIDTH - 2] = '\0';  // Null-terminate the string
            // line[space_between*(ctr + 1) + fixed_modifier - 1] = '+';
            // ctr++;
            // line[space_between*(ctr + 1) + fixed_modifier - 1] = '+';
            // ctr++;
            wattron(win, COLOR_PAIR(4));
            mvwprintw(win, i + 4, 1, "%s", line);  // Print the line
            wattroff(win, COLOR_PAIR(4));
        }
    }

    wrefresh(win);
}
