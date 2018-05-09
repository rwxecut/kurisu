#include "Kurisu.h"

Kurisu::Vertex A = {-0.6f, 0.8};
Kurisu::Vertex B = {-0.4f, -0.5f};
Kurisu::Vertex C = {0.5, -0.9f};
Kurisu::Vertex D = {0.7, 0.6};

Kurisu::Edge AB = {A, B};
Kurisu::Edge AC = {A, C};
Kurisu::Edge AD = {A, D};
Kurisu::Edge BC = {B, C};
Kurisu::Edge BD = {B, D};
Kurisu::Edge CD = {C, D};

std::vector<Kurisu::Vertex> vertex = {A, B, C, D};
std::vector<Kurisu::Edge> solution1 = {AB, BC, CD, AD};
std::vector<Kurisu::Edge> solution2 = {AC, BC, BD, AD};


int main (int, char **) {
	Kurisu graph;
	graph.createWindow (1000, 500);
	Kurisu::Pane LPane = graph.addPane ({-0.5, 0}, 1);
	Kurisu::Pane RPane = graph.addPane ({0.5, 0}, 0.2);

	graph[RPane].setGraph (vertex, solution2);
	graph[RPane].setNotchSize (KURISU_NOTCH_SIZE_DEFAULT / 2);

	graph[LPane].setGraph (vertex, solution1);
	graph[LPane].setNotchSize (KURISU_NOTCH_SIZE_DEFAULT / 4);
	graph[LPane].setNotchInterval (KURISU_NOTCH_INTERVAL_DEFAULT / 2);
	graph[LPane].setDotSize (KURISU_DOT_SIZE_DEFAULT / 2);

	while (graph.update ())
		graph.render ();

	return 0;
}
