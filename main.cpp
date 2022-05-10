#include <GL/glut.h>
#include <windows.h>
#include "YahooAPI.h"
#include "constants.h"

static int window;
static int value = 0;

void printText(int x, int y, float r, float g, float b, void* font, string& s)
{
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    int len = s.length();
    for (int i = 0; i < len; i++) {
        glutBitmapCharacter(font, s[i]);
    }
}

string getTwoPlacesFromDouble(double num) {
    return to_string((int)num) + "." + to_string((int)((abs(num) * 100) - ((int)abs(num) * 100)));
}

void menu(int num) {
    if (num == 0) {
        glutDestroyWindow(window);
        exit(0);
    }
    else {
        value = num;
    }
    glutPostRedisplay();
}

void createMenu(void) {
    int submenu_id = glutCreateMenu(menu);
    for (int i = 0; i < constants::numStocks; i++) {
        glutAddMenuEntry(constants:: stockDisplayNames[i].c_str(), i);
    }
    int menu_id = glutCreateMenu(menu);
    glutAddSubMenu("Monthly Graph ", submenu_id);
    glutAddMenuEntry("Quit", 0);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

YahooAPI api;
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();

    string heading = constants:: stockDisplayNames[value];
    printText(50, constants:: windowHeight - 50, 0, 0, 0, constants:: headingFont, heading);
    glFlush();

    
    // Graph Lines
    glBegin(GL_LINES);
    glVertex2i(50, constants:: windowHeight-80);
    glVertex2i(50, constants::windowHeight - 380);
    glVertex2i(50, constants::windowHeight-380);
    glVertex2i(constants::windowWidth - 50, constants::windowHeight - 380);
    glEnd();
    glFlush();

    YahooAPIMonthData data = api.getMonthData(constants::stockSymbolRequests[value], constants::stockSymbols[value]);
    int closeSize = data.close.size();
    double interval = (constants:: windowWidth - 100.0) / (closeSize +1);
    double x = 50;

    if (data.isProfit) glColor3f(0, 255, 0);
    else glColor3f(255, 0, 0);

    vector<double> normalizedValues;
    double maxValue = *max_element(data.close.begin(), data.close.end());
    double minValue = *min_element(data.close.begin(), data.close.end());
    for (auto i : data.close) 
        normalizedValues.push_back(((i-minValue)/(maxValue-minValue) * 250)+50+(constants::windowHeight - 380));

    glBegin(GL_LINES);
    if(normalizedValues.size() > 0) glVertex2d(x, normalizedValues[0]);
    for (int i = 1; i<normalizedValues.size(); i++) {
        glVertex2d(x, normalizedValues[i]);
        glVertex2d(x, normalizedValues[i]);
        x += interval;
    }
    glEnd();
    

    double percentchange = (data.close[closeSize - 1] - data.close[0]) * 100.0 / data.close[0];
    string currentPrice = getTwoPlacesFromDouble(data.close[closeSize-1]);
    string change = getTwoPlacesFromDouble(data.close[closeSize - 1] - data.close[0]) + " (" + getTwoPlacesFromDouble(percentchange) + "%)";

    int r = 0, g = 0;
    if (data.isProfit) g = 255;
    else r = 255;

    printText(constants:: windowWidth - 150, constants::windowHeight - 50, r, g, 0, constants::headingFont, currentPrice);
    printText(constants::windowWidth - 150, constants::windowHeight - 70, r, g, 0, constants::subHeadingFont, change);
    
    string maxS = "MAX", minS = "MIN", currentS="PRICE", pChange="% CHNG";
    string maxV = getTwoPlacesFromDouble(maxValue), minV = getTwoPlacesFromDouble(minValue), pChangeV = getTwoPlacesFromDouble(percentchange) + " %";
    
    int intervalTile = (constants::windowWidth - 100) / 4;
    printText(50, constants::windowHeight - 430, 0, 0, 0, constants::headingFont, currentS);
    printText(50, constants::windowHeight - 460, 0, 0, 0, constants::subHeadingFont, currentPrice);

    printText(50 + intervalTile, constants::windowHeight - 430, 0, 0, 0, constants::headingFont, pChange);
    printText(50 + intervalTile, constants::windowHeight - 460, 0, 0, 0, constants::subHeadingFont, pChangeV);

    printText(50 + intervalTile * 2, constants::windowHeight - 430, 0, 0, 0, constants::headingFont, maxS);
    printText(50 + intervalTile * 2, constants::windowHeight - 460, 0, 0, 0, constants::subHeadingFont, maxV);

    printText(50 + intervalTile * 3, constants::windowHeight - 430, 0, 0, 0, constants::headingFont, minS);
    printText(50 + intervalTile * 3, constants::windowHeight - 460, 0, 0, 0, constants::subHeadingFont, minV);

    //TODO:: Graph Markings

    glPopMatrix();
    glFlush();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(constants:: windowWidth, constants:: windowHeight);
    glutInitWindowPosition(100, 100);
    window = glutCreateWindow("Live Stock Market Dashboard");

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, constants:: windowWidth, 0, constants:: windowHeight);

    createMenu();

    glutDisplayFunc(display);
    glutMainLoop();
    return EXIT_SUCCESS;
}
