#include <conio.h>
#include <dos.h>
#include <graphics.h>
#include <stdio.h>

int main()
{
  int gd = DETECT, gm;
  int i, x, y, flag = 0;
  initgraph(&gd, &gm, "..\\BGI");

  /* get mid positions in x and y-axis */
  x = getmaxx() / 2;
  y = 30;

  while (!kbhit())
  {
    if (y >= getmaxy() - 30 || y <= 30)
      flag = !flag;

    setcolor(WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    circle(x, y, 30);
    floodfill(x, y, WHITE);

    delay(50);

    cleardevice();

    if (flag)
    {
      y = y + 5;
    }
    else
    {
      y = y - 5;
    }
  }

  getch();
  closegraph();
  return 0;
}
