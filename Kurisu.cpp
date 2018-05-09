#include "Kurisu.h"

#define _2D 2 // yep, Kurisu is 2D


int Kurisu::createWindow (int w, int h) {
	SDL_Init (SDL_INIT_VIDEO);
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
	window = SDL_CreateWindow ("Kurisu grapher",
	                           SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h,
	                           SDL_WINDOW_OPENGL);
	glContext = SDL_GL_CreateContext (window);
	SDL_GL_SetSwapInterval (1); // vsync

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	glClearColor (1, 1, 1, 1);
}


Kurisu::~Kurisu () {
	if (window) {
		SDL_GL_DeleteContext (glContext);
		SDL_DestroyWindow (window);
		SDL_Quit ();
	}
}


Kurisu::Pane Kurisu::addPane (Vertex center, GLfloat scale) {
	pane_objs.emplace_back (this, center, scale);
	return pane_objs.size () - 1;
}


bool Kurisu::update () {
	SDL_Event event;
	bool running = true;
	while (SDL_PollEvent (&event))
		if (event.type == SDL_QUIT)
			running = false;
	return running;
}


void Kurisu::render () {
	glClear (GL_COLOR_BUFFER_BIT);
	glEnableClientState (GL_VERTEX_ARRAY);
	for (Pane_obj pane : pane_objs) pane.render ();
	glDisableClientState (GL_VERTEX_ARRAY);
	glFlush ();
	SDL_GL_SwapWindow (window);
}


void Kurisu::moveVertexArray (const float *in, float *out, int count, float dx, float dy) {
	for (int i = 0; i < count; i++) {
		out[i * _2D + 0] = in[i * _2D + 0] + dx;
		out[i * _2D + 1] = in[i * _2D + 1] + dy;
	}
}


void Kurisu::mulVertexArray (const float *in, float *out, int count, float mx, float my) {
	for (int i = 0; i < count; i++) {
		out[i * _2D + 0] = in[i * _2D + 0] * mx;
		out[i * _2D + 1] = in[i * _2D + 1] * my;
	}
}


Kurisu::Pane_obj::Pane_obj (Kurisu *mother, Vertex center, GLfloat scale) {
	this->mother = mother;
	this->center = center;
	int winW, winH;
	SDL_GetWindowSize (mother->window, &winW, &winH);
	this->scale.x = scale * winH / winW;
	this->scale.y = scale;
}


void Kurisu::Pane_obj::render () {
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	drawAxes ();
	drawEdges ();
	drawVerticies ();
}


void Kurisu::Pane_obj::drawAxes () {
	glColor3fv (clAxis);
	// Draw axes lines
	glLineWidth (axisLineThickness);
	GLfloat p_axes[8] = {0};
	mulVertexArray (axes, p_axes, 4, scale.x, scale.y);
	moveVertexArray (p_axes, p_axes, 4, center.x, center.y);
	glVertexPointer (_2D, GL_FLOAT, 0, p_axes);
	glDrawArrays (GL_LINES, 0, 4);
	// Draw notches
	int notchCountHalfX = (int) ((axes[2] - axes[0]) / (2 * notchInterval));
	int notchCountHalfY = (int) ((axes[7] - axes[5]) / (2 * notchInterval));
	glLineWidth (notchLineThickness);
	GLfloat notch_curr[4] = {0};
	GLfloat scaled_notch[4] = {0};
	glVertexPointer (_2D, GL_FLOAT, 0, notch_curr);
	// X notches
	mulVertexArray (V_notch, scaled_notch, 2, notchSize * scale.x, notchSize * scale.y);
	for (int i = -notchCountHalfX; i <= notchCountHalfX; i++) {
		moveVertexArray (scaled_notch, notch_curr, 2, i * notchInterval * scale.x + center.x, center.y);
		glDrawArrays (GL_LINES, 0, 2);
	}
	// Y notches
	mulVertexArray (H_notch, scaled_notch, 2, notchSize * scale.x, notchSize * scale.y);
	for (int i = -notchCountHalfY; i <= notchCountHalfY; i++) {
		moveVertexArray (scaled_notch, notch_curr, 2, center.x, i * notchInterval * scale.y + center.y);
		glDrawArrays (GL_LINES, 0, 2);
	}
}


void Kurisu::Pane_obj::drawEdges () {
	glLineWidth (edgeLineThickness);
	glColor3fv (clEdge);
	GLfloat edge_curr[4] = {0};
	glVertexPointer (_2D, GL_FLOAT, 0, edge_curr);
	for (Edge edge : edges) {
		edge_curr[0] = edge.first.x;
		edge_curr[1] = edge.first.y;
		edge_curr[2] = edge.second.x;
		edge_curr[3] = edge.second.y;
		mulVertexArray (edge_curr, edge_curr, 2, scale.x, scale.y);
		moveVertexArray (edge_curr, edge_curr, 2, center.x, center.y);
		glDrawArrays (GL_LINES, 0, 2);
	}
}


void Kurisu::Pane_obj::drawVerticies () {
	glColor3fv (clVertex);
	GLfloat dot_curr[8] = {0};
	glVertexPointer (_2D, GL_FLOAT, 0, dot_curr);
	for (Vertex point : vertices) {
		mulVertexArray (dot, dot_curr, 4, dotSize * scale.x, dotSize * scale.y);
		moveVertexArray (dot_curr, dot_curr, 4, point.x * scale.x + center.x, point.y * scale.y + center.y);
		glDrawArrays (GL_POLYGON, 0, 4);
	}
}


void Kurisu::Pane_obj::setGraph (const std::vector<Vertex> &vertices_in, const std::vector<Edge> &edges_in) {
	vertices.clear ();
	vertices = vertices_in;
	edges.clear ();
	edges = edges_in;
}


void Kurisu::Pane_obj::setNotchSize (GLfloat size) { notchSize = size; }


void Kurisu::Pane_obj::setNotchInterval (GLfloat interval) { notchInterval = interval; }


void Kurisu::Pane_obj::setDotSize (GLfloat size) { dotSize = size; }


void Kurisu::Pane_obj::setAxisLineThickness (GLfloat thickness) { axisLineThickness = thickness; }


void Kurisu::Pane_obj::setEdgeLineThickness (GLfloat thickness) { edgeLineThickness = thickness; }


void Kurisu::Pane_obj::setNotchLineThickness (GLfloat thickness) { notchLineThickness = thickness; }


void Kurisu::Pane_obj::setAxesLines (const GLfloat lines[8]) { memcpy (axes, lines, 8 * sizeof (GLfloat)); }
