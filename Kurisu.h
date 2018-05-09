/******************************************************************************
 *                                 INTERFACES                                 *
 ******************************************************************************
typedef struct { float x, y; } Vertex;
 -- type for graph verticies
typedef std::pair<Vertex, Vertex> Edge;
 -- type for graph edges
typedef size_t Pane;
 -- type for pane identificator
Pane_obj &operator[] (Pane pane)
 -- access pane via it's ID
void Kurisu::createWindow (int w, int h)
 -- create window with width %w and height %h
Kurisu::Pane Kurisu::addPane (Vertex center, GLfloat scale)
 -- add graph pane with %scale and position at %center (in OpenGL coordinates)
bool Kurisu::update ()
 -- update window, handle events; returns false if window is closed
void Kurisu::render ()
 -- render panes
void Kurisu::Pane_obj::setGraph (const std::vector<Vertex> &vertices_in, const std::vector<Edge> &edges_in)
 -- set graph vertices and edges
void Kurisu::Pane_obj::setNotchSize (GLfloat size)
 -- set axes notches size (in local coordinates)
void Kurisu::Pane_obj::setNotchInterval (GLfloat interval)
 -- set axes notches interval (in local coordinates)
void Kurisu::Pane_obj::setDotSize (GLfloat size)
 -- set vertex dot size (in local coordinates)
void Kurisu::Pane_obj::setAxisLineThickness (GLfloat thickness)
 -- set axes lines thickness (in OpenGL coordinates)
void Kurisu::Pane_obj::setEdgeLineThickness (GLfloat thickness)
 -- set edges lines thickness (in OpenGL coordinates)
void Kurisu::Pane_obj::setNotchLineThickness (GLfloat thickness)
 -- set notches lines thickness (in OpenGL coordinates)
void Kurisu::Pane_obj::setAxesLines (const GLfloat lines[8])
 -- set axes lines (in OpenGL coordinates scales to local coordinates)


/******************************************************************************
 *                               DEFAULT CONFIG                               *
 *****************************************************************************/
#define KURISU_AXIS_LINE_THICKNESS_DEFAULT 1.0f
#define KURISU_EDGE_LINE_THICKNESS_DEFAULT 1.0f
#define KURISU_NOTCH_LINE_THICKNESS_DEFAULT 1.0f
#define KURISU_NOTCH_SIZE_DEFAULT 0.2f
#define KURISU_NOTCH_INTERVAL_DEFAULT 0.2f
#define KURISU_DOT_SIZE_DEFAULT 0.1f
#define KURISU_AXIS_COLOR_DEFAULT {0.0f, 0.0f, 0.0f}
#define KURISU_VERTEX_COLOR_DEFAULT {0.0f, 0.0f, 1.0f}
#define KURISU_EDGE_COLOR_DEFAULT {1.0f, 0.0f, 0.0f}
#define KURISU_AXES_LINES_DEFAULT {-1, 0, 1, 0, 0, -1, 0, 1}
#define KURISU_DOT_SHAPE_DEFAULT {-0.5f, 0.5f, -0.5f, -0.5f, \
                                  0.5f, -0.5f, 0.5f, 0.5f};
#define KURISU_HORIZONTAL_NOTCH_DEFAULT {-1, 0, 1, 0}
#define KURISU_VERTICAL_NOTCH_DEFAULT {0, -1, 0, 1}


/******************************************************************************
 *                                   SOURCE                                   *
 *****************************************************************************/
#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <vector>
#include <utility>
#include <cstring>


class Kurisu {
public:
	typedef struct {
		float x, y;
	} Vertex;
	typedef std::pair<Vertex, Vertex> Edge;
	typedef size_t Pane;

	class Pane_obj;

private:
	SDL_Window *window;
	SDL_GLContext glContext;

	std::vector<Pane_obj> pane_objs;

	static void moveVertexArray (const float *in, float *out, int count, float dx, float dy);
	static void mulVertexArray (const float *in, float *out, int count, float mx, float my);

public:
	int createWindow (int w, int h);
	~Kurisu ();


	inline Pane_obj &operator[] (Pane pane) {
		return pane_objs.at (pane);
	}


	Pane addPane (Vertex center, GLfloat scale);
	bool update ();
	void render ();
};


class Kurisu::Pane_obj {
	friend class Kurisu;

	Kurisu *mother;

	std::vector<Vertex> vertices;
	std::vector<Edge> edges;

	Vertex center, scale;
	GLfloat axisLineThickness = KURISU_AXIS_LINE_THICKNESS_DEFAULT,
			notchLineThickness = KURISU_NOTCH_LINE_THICKNESS_DEFAULT,
			edgeLineThickness = KURISU_EDGE_LINE_THICKNESS_DEFAULT;

	void render ();
	void drawAxes ();
	void drawEdges ();
	void drawVerticies ();

	// Dots
	GLfloat dotSize = KURISU_DOT_SIZE_DEFAULT;
	GLfloat dot[8] = KURISU_DOT_SHAPE_DEFAULT;
	// Axis notches
	GLfloat notchInterval = KURISU_NOTCH_INTERVAL_DEFAULT;
	GLfloat notchSize = KURISU_NOTCH_SIZE_DEFAULT;
	GLfloat H_notch[4] = KURISU_HORIZONTAL_NOTCH_DEFAULT;
	GLfloat V_notch[4] = KURISU_VERTICAL_NOTCH_DEFAULT;
	GLfloat axes[8] = KURISU_AXES_LINES_DEFAULT;
	// Colours
	GLfloat clAxis[3] = KURISU_AXIS_COLOR_DEFAULT;
	GLfloat clVertex[3] = KURISU_VERTEX_COLOR_DEFAULT;
	GLfloat clEdge[3] = KURISU_EDGE_COLOR_DEFAULT;

public:
	Pane_obj (Kurisu *mother, Vertex center, GLfloat scale);
	void setGraph (const std::vector<Vertex> &vertices_in, const std::vector<Edge> &edges_in);

	void setNotchSize (GLfloat size);
	void setNotchInterval (GLfloat interval);
	void setDotSize (GLfloat size);
	void setAxisLineThickness (GLfloat thickness);
	void setEdgeLineThickness (GLfloat thickness);
	void setNotchLineThickness (GLfloat thickness);
	void setAxesLines (const GLfloat lines[8]);
};
