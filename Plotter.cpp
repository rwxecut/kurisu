#include "Plotter.h"

#define _2D 2


Plotter::Plotter () {
	SDL_Init (SDL_INIT_VIDEO);
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
	window = SDL_CreateWindow ("Plotter",
	                           SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
	                           SCR_W, SCR_H,
	                           SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	SDL_GL_CreateContext (window);
	SDL_GL_SetSwapInterval (1); // vsync

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	glClearColor (1, 1, 1, 1);
}


Plotter::~Plotter () {
	SDL_DestroyWindow (window);
	SDL_Quit ();
}


void Plotter::set (Pane pane, const std::vector<point2d> &vertex_in, const std::vector<edge2d> &edges_in) {
	std::vector<point2d> &vertex_out = (pane == Pane::Left) ? LVertex : RVertex;
	const point2d &paneCenter = (pane == Pane::Left) ? LPane_center : RPane_center;
	std::vector<float> &edges_out = (pane == Pane::Left) ? LEdgesPlain : REdgesPlain;

	// Copy vertex
	vertex_out.clear ();
	vertex_out = vertex_in;

	// Transform edges
	edges_out.clear ();
	edges_out.reserve (edges_in.size () * 4);
	for (auto edge : edges_in) {
		edges_out.push_back (edge.first.x + paneCenter.x);
		edges_out.push_back (edge.first.y + paneCenter.y);
		edges_out.push_back (edge.second.x + paneCenter.x);
		edges_out.push_back (edge.second.y + paneCenter.y);
	}
}


bool Plotter::update () {
	SDL_Event event;
	bool running = true;
	while (SDL_PollEvent (&event))
		if (event.type == SDL_QUIT)
			running = false;
	return running;
}


void Plotter::render () {
	renderSetup ();
	drawEdges ();
	drawVertex ();
	drawAxes ();
	renderFinish ();
}


void Plotter::renderSetup () {
	glClear (GL_COLOR_BUFFER_BIT);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glScalef (scale / 2, scale, 1);
	glEnableClientState (GL_VERTEX_ARRAY);
}


void Plotter::drawAxes () {
	glColor3fv (clAxis);

	// Draw separator
	glLineWidth (separatorLineWidth);
	glVertexPointer (_2D, GL_FLOAT, 0, separatorLine);
	glDrawArrays (GL_LINES, 0, 2);

	// Draw axis
	glLineWidth (axisLineWidth);
	GLfloat L_axis[8] = {0}, R_axis[8] = {0};
	moveVertexArray (axis, L_axis, 4, LPane_center.x, LPane_center.y);
	moveVertexArray (axis, R_axis, 4, RPane_center.x, RPane_center.y);
	glVertexPointer (_2D, GL_FLOAT, 0, L_axis);
	glDrawArrays (GL_LINES, 0, 4);
	glVertexPointer (_2D, GL_FLOAT, 0, R_axis);
	glDrawArrays (GL_LINES, 0, 4);

	// Draw notches
	glLineWidth (notchLineWidth);
	GLfloat H_notch_curr[4] = {0};
	GLfloat V_notch_curr[4] = {0};
	glVertexPointer (_2D, GL_FLOAT, 0, V_notch_curr);
	// L pane X
	for (int i = -notchCountHalf; i <= notchCountHalf; i++) {
		moveVertexArray (V_notch, V_notch_curr, 2, i * notchInterval + LPane_center.x, LPane_center.y);
		glDrawArrays (GL_LINES, 0, 2);
	}
	// R pane X
	for (int i = -notchCountHalf; i <= notchCountHalf; i++) {
		moveVertexArray (V_notch, V_notch_curr, 2, i * notchInterval + RPane_center.x, RPane_center.y);
		glDrawArrays (GL_LINES, 0, 2);
	}
	// L pane Y
	glVertexPointer (_2D, GL_FLOAT, 0, H_notch_curr);
	for (int i = -notchCountHalf; i <= notchCountHalf; i++) {
		moveVertexArray (H_notch, H_notch_curr, 2, LPane_center.x, i * notchInterval + LPane_center.y);
		glDrawArrays (GL_LINES, 0, 2);
	}
	// R pane Y
	for (int i = -notchCountHalf; i <= notchCountHalf; i++) {
		moveVertexArray (H_notch, H_notch_curr, 2, RPane_center.x, i * notchInterval + RPane_center.y);
		glDrawArrays (GL_LINES, 0, 2);
	}
}


void Plotter::drawVertex () {
	GLfloat dot_curr[8] = {0};
	glVertexPointer (_2D, GL_FLOAT, 0, dot_curr);
	// Left pane dots
	glColor3fv (clLVertex);
	for (auto point : LVertex) {
		moveVertexArray (dot, dot_curr, 4, point.x + LPane_center.x, point.y + LPane_center.y);
		glDrawArrays (GL_POLYGON, 0, 4);
	}
	// Right pane dots
	glColor3fv (clRVertex);
	for (auto point : RVertex) {
		moveVertexArray (dot, dot_curr, 4, point.x + RPane_center.x, point.y + RPane_center.y);
		glDrawArrays (GL_POLYGON, 0, 4);
	}
}


void Plotter::drawEdges () {
	glLineWidth (edgeLineWidth);
	// Left pane edges
	glVertexPointer (_2D, GL_FLOAT, 0, &LEdgesPlain[0]);
	glColor3fv (clLEdges);
	glDrawArrays (GL_LINES, 0, LEdgesPlain.size () / 2);
	// Right pane edges
	glVertexPointer (_2D, GL_FLOAT, 0, &REdgesPlain[0]);
	glColor3fv (clREdges);
	glDrawArrays (GL_LINES, 0, REdgesPlain.size () / 2);
}


void Plotter::renderFinish () {
	glDisableClientState (GL_VERTEX_ARRAY);
	glFlush ();
	SDL_GL_SwapWindow (window);
}


void Plotter::moveVertexArray (const float *in, float *out, int count, float dx, float dy) {
	for (int i = 0; i < count; i++) {
		out[i * _2D + 0] = in[i * _2D + 0] + dx;
		out[i * _2D + 1] = in[i * _2D + 1] + dy;
	}
}
