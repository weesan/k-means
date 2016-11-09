/*
 * main.cc - Main program to demo k-means.
 *
 * Author: WeeSan Lee <weesan@weesan.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <vector>
#include "config.h"
#include "k-means.h"

#define DISPLAY_SIZE        1000
#define TIMEOUT             0.1
#define FEATURE_SIZE        5
#define CLUSTER_HEAD_SIZE   20

using namespace std;

// Forward declaration
class MyDisplay;
static void cb_timeout(MyDisplay *pcDisplay);

class MyDisplay : public Fl_Double_Window, public Kmeans<int> {
private:
    float fTimeout;

private:
    int pickAColor(int i) {
	int piColor[] = {
	    FL_BLUE,
	    FL_RED,
	    FL_GREEN,
	    FL_MAGENTA,
	    FL_BLACK,
            FL_LIGHT1,
	    FL_CYAN,
	    FL_YELLOW,
	    FL_GRAY,
	    FL_DARK_CYAN,
	    FL_DARK_MAGENTA,
            FL_DARK_GREEN,
            FL_DARK_YELLOW,
            FL_DARK_BLUE
	};
	int iTotalColor = sizeof(piColor) / sizeof(piColor[0]);

        return piColor[i];
    }
    
protected:
    void draw(void) {
	Fl_Double_Window::draw();
	char pzcLabel[256];

	snprintf(pzcLabel, sizeof(pzcLabel),
		 "%s-%s: step %d", PACKAGE_NAME, PACKAGE_VERSION, steps());
	label(pzcLabel);

        // Display cluster heads.
	fl_line_style(FL_SOLID, 3);
	for (int i = 0; i < clusterHeads().size(); i++) {
	    int x = clusterHeads()[i][0];
	    int y = clusterHeads()[i][1];
	    int iColor = pickAColor(i);
	    //printf("%d: %d\n", i, (*this)[i].cluster());
	    //fl_point(p.x(), p.y());
	    fl_color(iColor);
	    //fl_arc(x, y, CLUSTER_HEAD_SIZE, CLUSTER_HEAD_SIZE, 0, 360);
	    fl_pie(x, y, CLUSTER_HEAD_SIZE, CLUSTER_HEAD_SIZE, 0, 360);
	}

        // Display data points.
	fl_line_style(FL_SOLID, 1);
	for (int i = 0; i < Kmeans<int>::size(); i++) {
	    int x = (*this)[i][0];
	    int y = (*this)[i][1];
	    int iColor = pickAColor((*this)[i].cluster());
	    //printf("%d: %d\n", i, (*this)[i].cluster());
	    //fl_point(p.x(), p.y());
	    fl_color(iColor);
	    //fl_arc(x, y, FEATURE_SIZE, FEATURE_SIZE, 0, 360);
	    fl_rect(x, y, FEATURE_SIZE, FEATURE_SIZE);
	}
    }

public:
    MyDisplay(int iTotal, int iClusters, float fTimeout = TIMEOUT) :
	Fl_Double_Window(0, 0, DISPLAY_SIZE, DISPLAY_SIZE,
			 PACKAGE_NAME "-" PACKAGE_VERSION),
	Kmeans<int>(iClusters),
	fTimeout(fTimeout) {
	color(FL_WHITE);
	show();
	// Setup a time for animation
	Fl::add_timeout(timeout(), (Fl_Timeout_Handler)cb_timeout, this);
	// Randomly generate some data
	for (int i = 0; i < iTotal; i++) {
	    KmeansData<int> d;
	    for (int j = 0; j < d.size(); j++) {
		d[j] = random() % DISPLAY_SIZE;
	    }
	    push_back(d);
	}
    }
    float timeout(void) const {
	return (fTimeout);
    }
};

static void cb_timeout(MyDisplay *pcDisplay) {
    // Ask kmean class to re-computation, continue until false
    if (!pcDisplay->nextStep()) {
	Fl::repeat_timeout(pcDisplay->timeout(),
			   (Fl_Timeout_Handler)cb_timeout,
			   pcDisplay);
    }
    // Redraw after each re-computation
    pcDisplay->redraw();
}

int main(int argc, char *argv[]) {
    if (argc < 3 || argc > 4) {
	fprintf(stderr, "Usage: %s #_of_pts #_of_cluster [timeout]\n", argv[0]);
	exit(-1);
    }

    MyDisplay cDisplay(atoi(argv[1]), atoi(argv[2]),
		     argc == 4 ? atof(argv[3]) : TIMEOUT);
    return (Fl::run());
}
