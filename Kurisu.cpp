#include "Kurisu.h"

#define _2D 2 // yep, Kurisu is 2D


int Kurisu::createWindow (int w, int h) {
	SDL_Init (SDL_INIT_VIDEO);
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
	window = SDL_CreateWindow ("Kurisu grapher",
	                           SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h,
	                           SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
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


Kurisu::Pane
Kurisu::addPane (Kurisu::Vertex center, GLfloat axesLineWidth, GLfloat notchLineWidth, GLfloat edgesLineWidth) {
	pane_objs.emplace_back (center, axesLineWidth, notchLineWidth, edgesLineWidth);
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
	renderSetup ();
	for (Pane_obj pane : pane_objs) pane.render ();
	renderFinish ();
}


void Kurisu::renderSetup () {
	glClear (GL_COLOR_BUFFER_BIT);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glScalef (Pane_obj::scale / 2, Pane_obj::scale, 1);
	glEnableClientState (GL_VERTEX_ARRAY);
}


void Kurisu::renderFinish () {
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


Kurisu::Pane_obj::Pane_obj (Vertex center, GLfloat axesLineWidth, GLfloat notchLineWidth, GLfloat edgesLineWidth) {
	this->center = center;
	this->axesLineWidth = axesLineWidth;
	this->notchLineWidth = notchLineWidth;
	this->edgesLineWidth = edgesLineWidth;
}


void Kurisu::Pane_obj::render () {
	drawAxes ();
	drawEdges ();
	drawVerticies ();
}


void Kurisu::Pane_obj::drawAxes () {
	glColor3fv (clAxis);
	// Draw axes lines
	glLineWidth (axesLineWidth);
	GLfloat p_axis[8] = {0};
	Kurisu::moveVertexArray (axis, p_axis, 4, center.x, center.y);
	glVertexPointer (_2D, GL_FLOAT, 0, p_axis);
	glDrawArrays (GL_LINES, 0, 4);
	// Draw notches
	glLineWidth (notchLineWidth);
	GLfloat p_notch_curr[4] = {0};
	glVertexPointer (_2D, GL_FLOAT, 0, p_notch_curr);
	// X notches
	for (int i = -notchCountHalf; i <= notchCountHalf; i++) {
		moveVertexArray (V_notch, p_notch_curr, 2, i * notchInterval + center.x, center.y);
		glDrawArrays (GL_LINES, 0, 2);
	}
	// Y notches
	glVertexPointer (_2D, GL_FLOAT, 0, p_notch_curr);
	for (int i = -notchCountHalf; i <= notchCountHalf; i++) {
		moveVertexArray (H_notch, p_notch_curr, 2, center.x, i * notchInterval + center.y);
		glDrawArrays (GL_LINES, 0, 2);
	}
}


void Kurisu::Pane_obj::drawEdges () {
	glLineWidth (edgesLineWidth);
	glVertexPointer (_2D, GL_FLOAT, 0, &edges[0]);
	glColor3fv (clLEdges);
	glDrawArrays (GL_LINES, 0, edges.size () / 2);
}


void Kurisu::Pane_obj::drawVerticies () {
	GLfloat dot_curr[8] = {0};
	glVertexPointer (_2D, GL_FLOAT, 0, dot_curr);
	glColor3fv (clLVertex);
	for (Vertex point : vertices) {
		moveVertexArray (dot, dot_curr, 4, point.x, point.y);
		glDrawArrays (GL_POLYGON, 0, 4);
	}
}


void Kurisu::Pane_obj::setGraph (const std::vector<Vertex> &vertices_in, const std::vector<Edge> &edges_in) {
	// Transform vertices
	vertices.clear ();
	vertices.reserve (vertices_in.size ());
	for (Vertex vertex : vertices_in) {
		vertices.push_back ({vertex.x + center.x, vertex.y + center.y});
	}

	// Transform edges
	edges.clear ();
	edges.reserve (edges_in.size () * 4);
	for (Edge edge : edges_in) {
		edges.push_back (edge.first.x + center.x);
		edges.push_back (edge.first.y + center.y);
		edges.push_back (edge.second.x + center.x);
		edges.push_back (edge.second.y + center.y);
	}
}
