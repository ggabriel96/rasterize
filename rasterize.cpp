#include <set>
#include <math.h>
#include <vector>
#include <stdio.h>
#include <utility>
#include <string.h>
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

void init_edges(set<edge_t> &edges) {
  // Professor's case
  edges.insert(edge_t(vertex[0], vertex[1]));
  edges.insert(edge_t(vertex[0], vertex[2]));
  edges.insert(edge_t(vertex[1], vertex[3]));
  edges.insert(edge_t(vertex[2], vertex[3]));

  // square 50x50
  // edges.insert(edge_t(vertex[0], vertex[3]));
  // edges.insert(edge_t(vertex[1], vertex[2]));
  // edges.insert(edge_t(vertex[0], vertex[1]));
  // edges.insert(edge_t(vertex[3], vertex[2]));

  // hourglass
  // edges.insert(edge_t(vertex[0], vertex[2]));
  // edges.insert(edge_t(vertex[1], vertex[3]));
  // edges.insert(edge_t(vertex[0], vertex[1]));
  // edges.insert(edge_t(vertex[3], vertex[2]));

  // triangle, change NVERT to 3
  // edges.insert(edge_t(vertex[0], vertex[1]));
  // edges.insert(edge_t(vertex[0], vertex[2]));
  // edges.insert(edge_t(vertex[1], vertex[2]));
}

void print_edges(set<edge_t> &edges) {
  for (auto& edge: edges) {
    edge.echo(); printf("\n");
  }
}

void show_pixels() {
  int x, y;
  // y is height, so y is the current line of the matrix
  // starting from top because we go upside down in the terminal
  for (y = MAX - 1; y >= 0; y--)
    for (x = 0; x < MAX; x++)
      printf("%d%s", pixel[y][x], x + 1 == MAX ? "\n" : "");
}

void paint_pixels(set<edge_t>::iterator &edge, int y) {
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

bool insert_yyo(set<edge_t> &edges, set<edge_t> &LAA, int y) {
  bool inserted = false;
  for (auto edge: edges)
    if ((int) edge.yo == y && (int) edge.y != y) {
      LAA.insert(edge);
      inserted = true;
    }
  return inserted;
}

bool erase_yy(set<edge_t> &LAA, int y) {
  bool erased = false;
  auto edge = LAA.begin();
  while (edge != LAA.end())
    if ((int) edge->y == y) {
      edge = LAA.erase(edge);
      erased = true;
    } else edge++;
  return erased;
}

void delta_x(set<edge_t> &LAA) {
  size_t i;
  vector<edge_t> tmp;
  // set is const, gotta push all edges to this
  // vector, change them, and insert back on LAA
  for (auto& edge: LAA)
    tmp.push_back(edge);
  LAA.clear();
  for (i = 0; i < tmp.size(); i++) {
    tmp[i].x += tmp[i].delta;
    LAA.insert(tmp[i]);
  }
}

void rasterize(set<edge_t> &edges) {
  int y, ymin = min_y(), ymax = max_y();
  // set structure already sorts the elements
  // based on their '<' (less than) operator
  set<edge_t> LAA;
  for (y = ymin; y < ymax; y++) {
    insert_yyo(edges, LAA, y);
    erase_yy(LAA, y);
    for (set<edge_t>::iterator edge = LAA.begin(); next(edge) != LAA.end(); edge++)
      paint_pixels(edge, y);
    delta_x(LAA);
  }
}

int main(int argc, char const *argv[]) {
  set<edge_t> edges;
  init_edges(edges);
  memset(pixel, 0, sizeof (pixel));
  rasterize(edges);
  show_pixels();
  return 0;
}
