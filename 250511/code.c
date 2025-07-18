#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>

#define PROGRESS_BAR_WIDTH 30
#define BORDER_PADDING 2
#define WINDOW_WIDTH (PROGRESS_BAR_WIDTH + 2 * BORDER_PADDING + 2)
#define WINDOW_HEIGHT 5
#define PROGRESS_INCREMENT 3
#define DELAY 300000 // 微秒（300毫秒）

int main() {
    initscr();                          // 初始化 ncurses 模式
    start_color();                      // 启用颜色功能
    init_pair(1, COLOR_GREEN, COLOR_BLACK); // 已完成部分：绿色
    init_pair(2, COLOR_RED, COLOR_BLACK);   // 剩余部分：红色（仅演示）
    
    cbreak();                           // 禁用行缓冲
    noecho();                           // 关闭输入回显
    curs_set(FALSE);                    // 隐藏光标

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);     // 获取终端尺寸
    int start_y = (max_y - WINDOW_HEIGHT) / 2;  // 窗口居中
    int start_x = (max_x - WINDOW_WIDTH) / 2;

    WINDOW *win = newwin(WINDOW_HEIGHT, WINDOW_WIDTH, start_y, start_x);
    box(win, 0, 0);                     // 绘制窗口边框
    wrefresh(win);                      // 刷新窗口

    int progress = 0;
    int max_progress = PROGRESS_BAR_WIDTH;

    while (progress <= max_progress) {
        werase(win);                    // 清除窗口内容

        // 绘制已完成部分（绿色）
        attron(COLOR_PAIR(1));
        for (int i = 0; i < progress; i++) {
            mvwprintw(win, 1, BORDER_PADDING + 1 + i, "#");
        }
        attroff(COLOR_PAIR(1));

        // 绘制剩余部分（红色空格）
        attron(A_BOLD | COLOR_PAIR(2));
        for (int i = progress; i < max_progress; i++) {
            mvwprintw(win, 1, BORDER_PADDING + 1 + i, " ");
        }
        attroff(A_BOLD | COLOR_PAIR(2));

        // 显示百分比（居中）
        char percent_str[10];
        snprintf(percent_str, sizeof(percent_str), "%d%%", (progress * 100) / max_progress);
        mvwprintw(win, WINDOW_HEIGHT - 1, (WINDOW_WIDTH - strlen(percent_str)) / 2, percent_str);

        wrefresh(win);                  // 刷新窗口
        progress += PROGRESS_INCREMENT;  // 更新进度
        usleep(DELAY);                  // 延迟
    }

    delwin(win);    // 删除窗口
    endwin();       // 退出 ncurses 模式
    return 0;
}
