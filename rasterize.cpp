#include <math.h>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#define MAX 100
#define NVERT 4
#define EPS 1e-9
#define eq(x, y) (fabs((x) - (y)) < EPS)
#define lt(x, y) ((x) + EPS < y)
#define le(x, y) (lt((x), (y)) || eq((x), (y)))
using namespace std;

typedef struct Point {
  double x, y;
  Point() {};
  Point(int _x, int _y): x(_x), y(_y) {};
} point_t;

typedef struct Edge {
  double yo, y, x, delta;
  Edge() {}
  Edge(point_t a, point_t b): yo(a.y), y(b.y), x(a.x) {
    delta = (b.x - a.x) / (b.y - a.y);
  }
  bool friend operator<(const Edge &l, const Edge &r) {
    // works, same as uncommented return
    // if (eq(l.x, r.x)) return le(l.y, r.y);
    // return lt(l.x, r.x);
    return lt(l.x, r.x) || (eq(l.x, r.x) && le(l.y, r.y));
    // return lt(l.x, r.x); // doesn't work
  }
  void echo() const {
    printf("{yo: %lf, y: %lf, x: %lf, delta: %lf}", this->yo, this->y, this->x, this->delta);
  }
} edge_t;

int pixel[MAX][MAX];

// Professor's case
point_t vertex[] = {
  point_t(20.0, 8.0), point_t(2.0, 45.0), point_t(80.0, 65.0), point_t(97.0, 60.0)
};

// square 50x50 or hourglass
// point_t vertex[] = {
//   point_t(0.0, 0.0), point_t(50.0, 0.0), point_t(50.0, 50.0), point_t(0.0, 50.0)
// };

// triangle, change NVERT to 3
// point_t vertex[] = {
//   point_t(0.0, 0.0), point_t(50.0, 0.0), point_t(25.0, 50.0)
// };

void init_edges(vector<edge_t> &edges) {
  // Professor's case
  edges.push_back(edge_t(vertex[0], vertex[1]));
  edges.push_back(edge_t(vertex[0], vertex[2]));
  edges.push_back(edge_t(vertex[1], vertex[3]));
  edges.push_back(edge_t(vertex[2], vertex[3]));

  // square 50x50
  // edges.push_back(edge_t(vertex[0], vertex[3]));
  // edges.push_back(edge_t(vertex[1], vertex[2]));
  // edges.push_back(edge_t(vertex[0], vertex[1]));
  // edges.push_back(edge_t(vertex[3], vertex[2]));

  // hourglass
  // edges.push_back(edge_t(vertex[0], vertex[2]));
  // edges.push_back(edge_t(vertex[1], vertex[3]));
  // edges.push_back(edge_t(vertex[0], vertex[1]));
  // edges.push_back(edge_t(vertex[3], vertex[2]));

  // triangle, change NVERT to 3
  // edges.push_back(edge_t(vertex[0], vertex[1]));
  // edges.push_back(edge_t(vertex[0], vertex[2]));
  // edges.push_back(edge_t(vertex[1], vertex[2]));
  
  sort(edges.begin(), edges.end());
}

void print_edges(vector<edge_t> &edges) {
  for (auto& edge: edges) {
    edge.echo();
    printf("\n");
  }
}

void print_pixels() {
  int x, y;
  // y is height, so y is the current line of the matrix
  // starting from top because we go upside down in the terminal
  for (y = MAX - 1; y >= 0; y--)
    for (x = 0; x < MAX; x++)
      printf("%d%s", pixel[y][x], x + 1 == MAX ? "\n" : "");
}

void paint_pixels(vector<edge_t>::iterator &edge, int y) {
  int x; auto next_edge = next(edge);
  for (x = (int) edge->x; x < (int) next_edge->x; x++)
    pixel[y][x] = 1; // again, y is height
}

int min_y() {
  int v, y = vertex[0].y;
  for (v = 1; v < NVERT; v++)
    if ((int) vertex[v].y < y)
      y = (int) vertex[v].y;
  return y;
}

int max_y() {
  int v, y = vertex[0].y;
  for (v = 1; v < NVERT; v++)
    if ((int) vertex[v].y > y)
      y = (int) vertex[v].y;
  return y;
}

bool insert_yyo(vector<edge_t> &edges, vector<edge_t> &LAA, int y) {
  bool inserted = false;
  for (auto edge: edges)
    if ((int) edge.yo == y && (int) edge.y != y) {
      LAA.push_back(edge);
      inserted = true;
    }
  return inserted;
}

bool erase_yy(vector<edge_t> &LAA, int y) {
  bool erased = false;
  auto edge = LAA.begin();
  while (edge != LAA.end())
    if ((int) edge->y == y) {
      edge = LAA.erase(edge);
      erased = true;
    } else edge++;
  return erased;
}

void paint(vector<edge_t> &LAA, int y) {
  for (auto edge = LAA.begin(); next(edge) != LAA.end(); edge++)
    paint_pixels(edge, y);
}

void delta_x(vector<edge_t> &LAA) {
  for (auto& edge: LAA)
    edge.x += edge.delta;
}

void rasterize(vector<edge_t> &edges) {
  vector<edge_t> LAA;
  int y, ymin = min_y(), ymax = max_y();
  for (y = ymin; y < ymax; y++) {
    insert_yyo(edges, LAA, y);
    erase_yy(LAA, y);
    sort(LAA.begin(), LAA.end());
    paint(LAA, y);
    delta_x(LAA);
  }
}

int main(int argc, char const *argv[]) {
  vector<edge_t> edges;
  init_edges(edges);
  memset(pixel, 0, sizeof (pixel));
  rasterize(edges);
  print_pixels();
  return 0;
}
