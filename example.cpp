#include "Plotter.h"

point2d A = {-0.6f, 0.8};
point2d B = {-0.4f, -0.5f};
point2d C = {0.5, -0.9f};
point2d D = {0.7, 0.6};

edge2d AB = {A, B};
edge2d AC = {A, C};
edge2d AD = {A, D};
edge2d BC = {B, C};
edge2d BD = {B, D};
edge2d CD = {C, D};

std::vector<point2d> vertex = {A, B, C, D};
std::vector<edge2d> solution1 = {AB, BC, CD, AD};
std::vector<edge2d> solution2 = {AC, BC, BD, AD};


int main (int, char**) {
	Plotter *plot = new Plotter ();

	plot->set (Plotter::Pane::Left, vertex, solution1);
	plot->set (Plotter::Pane::Right, vertex, solution2);
	while (plot->update ())
		plot->render ();

	delete plot;
	return 0;
}
