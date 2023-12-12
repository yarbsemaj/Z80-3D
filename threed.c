#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h> // notice this! you need it!

#pragma output REGISTER_SP = 0xFFFF

#define W 32
#define H 64

#define FL 40
float AR = 0.1;

void drawPoint(int, int);
void plotPoint(int, int, int, int);
void rotatePoint(int, int, int, int);
void drawLine(int, int, int, int);
void cleanScreen();
void printScreen();
void copyVertex();

int screen[W][H];
// x , y , z
int verts[][3] = {
    {-10, -10, -10},
    {10, -10, -10},
    {10, 10, -10},
    {-10, 10, -10},

    {-10, -10, 10},
    {10, -10, 10},
    {10, 10, 10},
    {-10, 10, 10},
};

int edges[][2] = {
    {0, 1},
    {1, 2},
    {2, 3},
    {3, 0},

    {4, 5},
    {5, 6},
    {6, 7},
    {7, 4},

    {0, 4},
    {1, 5},
    {2, 6},
    {3, 7},
};

//Vertexes to be rendered
int vertexes[sizeof(verts) / sizeof(verts[0])][3];
//Vertex projected position
int projected[sizeof(verts) / sizeof(verts[0])][2];

int main(void)
{
  copyVertex();
  int numberOfVertexes = sizeof(verts) / sizeof(verts[0]);
  for (int l = 0; l < 30; l++)
  {
    cleanScreen();

    for (int i = 0; i < numberOfVertexes; i++)
    {
      plotPoint(vertexes[i][0], vertexes[i][1], vertexes[i][2], i);
    }

    //Join 2 vertexes with an edge
    int numberOfEdges = sizeof(edges) / sizeof(edges[0]);
    for (int i = 0; i < numberOfEdges; i++)
    {
      int e0 = edges[i][0];
      int e1 = edges[i][1];
      drawLine(projected[e0][0], projected[e0][1], projected[e1][0], projected[e1][1]);
    }

    printScreen();
    copyVertex();
    for (int i = 0; i < numberOfVertexes; i++)
    {
      rotatePoint(vertexes[i][0], vertexes[i][1], vertexes[i][2], i);
    }
    //Increase the rotation angel
    AR += 0.1;
  }

  return 0;
}

//Perform the matrix multiplication required to rotate the screen
void rotatePoint(int x, int y, int z, int vertexNumber)
{
  int rx = ((y * cos(AR)) + (z * sin(AR)));
  int rz = ((y * sin(AR)) - (z * cos(AR)));

  vertexes[vertexNumber][1] = rx;
  vertexes[vertexNumber][2] = rz;
}

//Project a point
void plotPoint(int x, int y, int z, int vertexNumber)
{

  int xp = (FL * x) / (FL + z);
  int yp = (FL * y) / (FL + z);

  projected[vertexNumber][0] = xp;
  projected[vertexNumber][1] = yp;

  drawPoint(xp, yp);
}

//add a projected point to the screen, while moving the origin to the center of the screen
void drawPoint(int x, int y)
{
  x = x + W / 2;
  y = y + H / 2;
  if (x >= 0 && x < W - 1 && y >= 0 && y < H - 1)
  {
    screen[x][y] = 1;
  }
}

//Join 2 points with a line
void drawLine(int x0, int y0, int x1, int y1)
{
  int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
  int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
  int err = dx + dy, e2; /* error value e_xy */

  for (;;)
  { /* loop */
    drawPoint(x0, y0);
    if (x0 == x1 && y0 == y1)
      break;
    e2 = 2 * err;
    if (e2 >= dy)
    {
      err += dy;
      x0 += sx;
    } /* e_xy+e_x > 0 */
    if (e2 <= dx)
    {
      err += dx;
      y0 += sy;
    } /* e_xy+e_y < 0 */
  }
}

//Clear the frame buffer
void cleanScreen()
{
  for (int i = 0; i < W; i++)
  {
    for (int j = 0; j < H; j++)
      screen[i][j] = 0;
  }
}

//Print the frame buffer
void printScreen()
{
  printf("\033[H");
  printf("\033[2J");
  for (int i = 0; i < W; i++)
  {
    for (int j = 0; j < H; j++)
      if (screen[i][j])
      {
        putchar('#');
      }
      else
      {
        putchar(' ');
      }
    putchar('\n');
  }
}

//Copy the vertexes into the working array
void copyVertex()
{
  int numberOfVertexes = sizeof(verts) / sizeof(verts[0]);

  for (int i = 0; i < numberOfVertexes; i++)
  {
    vertexes[i][0] = verts[i][0];
    vertexes[i][1] = verts[i][1];
    vertexes[i][2] = verts[i][2];
  }
}
