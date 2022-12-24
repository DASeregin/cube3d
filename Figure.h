//
// Created by Denis Seregin on 14.12.2022.
//

#ifndef LAB_2_FIGURE_H
#define LAB_2_FIGURE_H
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

struct Point2D{
    float x,y;
    Point2D(): x(0), y(0){};
    Point2D(float x, float y){
        this->x = x;
        this->y = y;
    }
};


struct Edge2D{
    std::vector<Point2D> points;
    Edge2D(): points({}) {};
    Edge2D(std::vector<Point2D>& p): points(p){};

    void Draw(sf::RenderWindow& window){
        int n = points.size();
        for (int i = 0; i < n; ++i) {
            DrawLine(window, points[i], points[(i+1)%n]);
        }
    }
private:
    void DrawLine(sf::RenderWindow& window, Point2D& p1, Point2D& p2){
        sf::Vertex line[] = {{{p1.x, p1.y}}, {{p2.x, p2.y}}};
        window.draw(line, 2, sf::Lines);
    }
};

struct Point3D{
    float x,y,z;
    Point3D(): x(0), y(0), z(0){};
    Point3D(float x, float y, float z){
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

struct Edge3D{
    std::vector<Point3D> points;
    float ro, fi, th, w, h;
    Edge3D(): points({}){};
    Edge3D(std::vector<Point3D>& p): points(p){};

    void Draw(sf::RenderWindow& window, float ro, float fi, float th, int w, int h){
        this->ro = ro;
        this->fi = fi;
        this->th = th;
        this->w = w;
        this->h = h;
        Edge2D edge = Projection();
        edge.Draw(window);
    }

    bool Visible(){
        return True;
        Point3D p1 = FromWorldToView(points[0]), p2 = FromWorldToView(points[1]), p3 = FromWorldToView(points[2]);
        return (-p1.x * ((p2.y - p1.y) * (p3.z-p1.z) - (p2.z-p1.z) * (p3.y - p1.y)) +
               p1.y * ((p2.x-p1.x)*(p3.z-p1.z) - (p2.z-p1.z) * (p3.x-p1.x)) -
               p1.z * ((p2.x-p1.x)*(p3.y-p1.y) - (p2.y-p1.y)*(p3.x-p1.x))) > 0;
    }

public:
    Edge2D Projection(){
        std::vector<Point2D> points2D(points.size());
        for (int i = 0; i < points.size(); ++i) {
            points2D[i] = FromViewToScreen(FromWorldToView(points[i]));
        }
        Edge2D edge(points2D);
        return edge;
    }

    Point3D FromWorldToView(Point3D p){
        Point3D np(0,0,0);
        np.x = -p.x * sin(th) + p.y * cos(th);
        np.y = -p.x * cos(fi) * cos(th) - p.y * cos(fi) * sin(th) + p.z * sin(fi);
        np.z = -p.x * sin(fi) * cos(th) - p.y * sin(fi) * sin(th) - p.z * cos(fi) + ro;
        std::cout << np.x << ' ' << np.y << ' ' << np.z << '\n';
        return np;
    }

    Point2D FromViewToScreen(Point3D p){
        float d =  ro / 2;
        Point2D np;
        np.x = d * p.x / p.z  + float(w)/2;
        np.y = d * p.y / p.z  + float(h)/2;
        return np;
    }

};

class Figure{
public:
    std::vector<Edge3D> edges;
    float ro = 500, fi = M_PI / 4, th = M_PI / 4;
    int w, h;

    void ReadFromFile(std::string path){
        std::ifstream f(path);
        int n;
        f>>n;
        std::vector<Point3D> points(n);
        for (int i = 0; i < n; ++i) {
            f>>points[i].x >> points[i].y >> points[i].z;
            points[i].x = points[i].x - 100;
            points[i].y = points[i].y - 100;
            points[i].z = points[i].z - 100;
        }
        int m;
        f>>m;
        int dot_number;
        f>>dot_number;
        std::vector<Edge3D> es(m);
        for (int i = 0; i < m; ++i) {
            std::vector<Point3D> edge_points(dot_number);
            for (int j = 0; j < dot_number; ++j) {
                int current;
                f>>current;
                edge_points[j] = points[current];
            }
            Edge3D current_edge(edge_points);
            es[i] = current_edge;
        }
        edges = es;
    };

public:
    Figure();
    Figure(std::string path, int w, int h){
        this->w = w;
        this->h = h;
        ReadFromFile(path);
    }

    void Rotate(float a){
        fi += a/2;
        th += a/2;
    }

    void RotateZ(float a){
        for (int i = 0; i < edges.size(); ++i) {
            for (int j = 0; j < edges[i].points.size(); ++j) {
                edges[i].points[j].x = cos(a) * edges[i].points[j].x - sin(a) * edges[i].points[j].y;
                edges[i].points[j].y = sin(a) * edges[i].points[j].x + cos(a) * edges[i].points[j].y;
            }

        }
    }

    void Draw(sf::RenderWindow& window){
        for (int i = 0; i < edges.size(); ++i) {
            if (edges[i].Visible()) {
                edges[i].Draw(window, ro, fi, th, w, h);
            }
        }
    }

};



#endif //LAB_2_FIGURE_H
