#include "PhysicsEngine.h"
#include "Collision.h"
#include "Helper.h"
#include "Renderer.h"
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h> // printf
#include <ctype.h> // isascii
#include <iostream>

#define JC_VORONOI_IMPLEMENTATION
// If you wish to use doubles
//#define JCV_REAL_TYPE double
//#define JCV_FABS fabs
//#define JCV_ATAN2 atan2
#include "jc_voronoi.h"

#define JC_VORONOI_CLIP_IMPLEMENTATION
#include "jc_voronoi_clip.h"

#ifdef HAS_MODE_FASTJET
#include <vector>
#include "../test/fastjet/voronoi.h"
#endif


void PhysicsEngine::addBody(const RigidBody& body) {
    setPoints.push_back(body);
}

static void relax_points(const jcv_diagram* diagram, jcv_point* points)
{
    const jcv_site* sites = jcv_diagram_get_sites(diagram);
    for (int i = 0; i < diagram->numsites; ++i)
    {
        const jcv_site* site = &sites[i];
        jcv_point sum = site->p;
        int count = 1;

        const jcv_graphedge* edge = site->edges;

        while (edge)
        {
            sum.x += edge->pos[0].x;
            sum.y += edge->pos[0].y;
            ++count;
            edge = edge->next;
        }

        points[site->index].x = sum.x / (jcv_real)count;
        points[site->index].y = sum.y / (jcv_real)count;
    }
}

static inline jcv_point remap(const jcv_point* pt, const jcv_point* min, const jcv_point* max, const jcv_point* scale)
{
    jcv_point p;
    p.x = (pt->x - min->x) / (max->x - min->x) * scale->x;
    p.y = (pt->y - min->y) / (max->y - min->y) * scale->y;
    return p;
}


void PhysicsEngine::relaxPointsNTimes(int n)
{
    jcv_point* clippoints = 0;
    jcv_clipper* clipper = 0;
    jcv_rect* rect = 0;
	int count = setPoints.size();

    //convert setPoints to jcv_point
    jcv_point* points = (jcv_point*)malloc(sizeof(jcv_point) * (size_t)count);
    if (!points)
		return;

    for (int i = 0; i < count; ++i)
    {
		points[i].x = stretch_number(setPoints[i].position.x, -1, 1, 0, 1920);
		points[i].y = stretch_number(setPoints[i].position.y, -1, 1, 0, 1080);
	}

    for (int i = 0; i < n; ++i)
    {
        jcv_diagram diagram;
        memset(&diagram, 0, sizeof(jcv_diagram));
        jcv_diagram_generate(count, (const jcv_point*)points, rect, clipper, &diagram);

        relax_points(&diagram, points);

        jcv_diagram_free(&diagram);
    }

    free(clippoints);
    free(rect);
    // convert jcv_point back to setPoints
    for (int i = 0; i < count; ++i)
    {
		setPoints[i].position.x = compress_number(points[i].x, 0, 1920, -1, 1);
		setPoints[i].position.y = compress_number(points[i].y, 0, 1080, -1, 1);
        //std::cout << "setPoints[" << i << "].position.x: " << setPoints[i].position.x << std::endl;
	}
    free(points);
}


void PhysicsEngine::update(float dt) {
    if (edges.size() > 0)
        return;
    edges.clear();
    //relaxPointsNTimes(0);
    calculateVoronoiRegions();
}

double PhysicsEngine::stretch_number(double number, double min_range, double max_range, double output_min, double output_max)
{
    if (number < min_range || number > max_range) {
        std::cout << "Number " << number << " is out of range: " << min_range << ", " << "max_range" << std::endl;
        throw std::invalid_argument("Number is out of range");
    }
    return (number - min_range) * (output_max - output_min) / (max_range - min_range) + output_min;

}

double PhysicsEngine::compress_number(double number, double min_range, double max_range, double output_min, double output_max)
{
    if (number < min_range || number > max_range) {
        std::cout << "Number " << number << " is out of range: " << min_range << ", " << "max_range" << std::endl;
        throw std::invalid_argument("Number is out of range");
    }
    return number * (output_max - output_min) / (max_range - min_range) + output_min;
}

void PhysicsEngine::calculateVoronoiRegions()
{
    // Number of sites to generate
    // Image dimension
    int count = setPoints.size();
    int width = 1920;
    int height = 1080;
    int numrelaxations = 0;
    const char* inputfile = 0;
    const char* clipfile = 0; // a file with clipping points
    //const char* outputfile = "example.png";


    jcv_point* points = 0;
    jcv_rect* rect = 0;


   
    {
        points = (jcv_point*)malloc(sizeof(jcv_point) * (size_t)count);
        if (!points)
            return ;
        
        //int pointoffset = 10; // move the points inwards, for aestetic reasons

        srand(0);
        for (int i = 0; i < count; ++i)
        {
            points[i].x = stretch_number(setPoints[i].position.x, -1, 1, 0, 1920);
            points[i].y = stretch_number(setPoints[i].position.y, -1, 1, 0, 1080);
            std::cout << "points[" << i << "].x: " << points[i].x << ", points[" << i << "].y: " << points[i].y << std::endl;
        }

    }
    

    jcv_point* clippoints = 0;
    int clipcount = 0;
    jcv_clipping_polygon polygon;
    jcv_clipper* clipper = 0;
    /*if (clippoints)
    {

        printf("Clip polygon '%s' used\n", clipfile);
        polygon.num_points = clipcount;
        polygon.points = clippoints;

        jcv_clipper polygonclipper;
        polygonclipper.test_fn = jcv_clip_polygon_test_point;
        polygonclipper.clip_fn = jcv_clip_polygon_clip_edge;
        polygonclipper.fill_fn = jcv_clip_polygon_fill_gaps;
        polygonclipper.ctx = &polygon;

        clipper = &polygonclipper;
    }
    else {
        polygon.num_points = 0;
        polygon.points = 0;
    }*/

    for (int i = 0; i < numrelaxations; ++i)
    {
        jcv_diagram diagram;
        memset(&diagram, 0, sizeof(jcv_diagram));
        jcv_diagram_generate(count, (const jcv_point*)points, rect, clipper, &diagram);

        relax_points(&diagram, points);

        jcv_diagram_free(&diagram);
    }

    //size_t imagesize = (size_t)(width * height * 3);
    //unsigned char* image = (unsigned char*)malloc(imagesize);
    //memset(image, 0, imagesize);

    //unsigned char color_pt[] = { 255, 255, 255 };
    //unsigned char color_line[] = { 220, 220, 220 };
    //unsigned char color_delauney[] = { 64, 64, 255 };

    jcv_diagram diagram;
    jcv_point dimensions;
    dimensions.x = (jcv_real)width;
    dimensions.y = (jcv_real)height;
    {
        memset(&diagram, 0, sizeof(jcv_diagram));
        jcv_diagram_generate(count, (const jcv_point*)points, rect, clipper, &diagram);
        // If you want to draw triangles, or relax the diagram,
        // you can iterate over the sites and get all edges easily

        /*if (0)
        {
            const jcv_site* sites = jcv_diagram_get_sites(&diagram);
            for (int i = 0; i < diagram.numsites; ++i)
            {
                const jcv_site* site = &sites[i];

                srand((unsigned int)site->index); // for generating colors for the triangles

                unsigned char color_tri[3];
                unsigned char basecolor = 120;
                color_tri[0] = basecolor + (unsigned char)(rand() % (235 - basecolor));
                color_tri[1] = basecolor + (unsigned char)(rand() % (235 - basecolor));
                color_tri[2] = basecolor + (unsigned char)(rand() % (235 - basecolor));

                jcv_point s = remap(&site->p, &diagram.min, &diagram.max, &dimensions);

                const jcv_graphedge* e = site->edges;
                while (e)
                {
                    jcv_point p0 = remap(&e->pos[0], &diagram.min, &diagram.max, &dimensions);
                    jcv_point p1 = remap(&e->pos[1], &diagram.min, &diagram.max, &dimensions);

                    draw_triangle(&s, &p0, &p1, image, width, height, 3, color_tri);
                    e = e->next;
                }
            }
        }*/

        // If all you need are the edges
        const jcv_edge* edge = jcv_diagram_get_edges(&diagram);
        while (edge)
        {
            jcv_point p0 = remap(&edge->pos[0], &diagram.min, &diagram.max, &dimensions);
            jcv_point p1 = remap(&edge->pos[1], &diagram.min, &diagram.max, &dimensions);
            //draw_line((int)p0.x, (int)p0.y, (int)p1.x, (int)p1.y, image, width, height, 3, color_line);
            edge = jcv_diagram_get_next_edge(edge);
            double x0 = compress_number(p0.x, 0, 1920, -1, 1);
            double y0 = compress_number(p0.y, 0, 1080, -1, 1);
            double x1 = compress_number(p1.x, 0, 1920, -1, 1);
            double y1 = compress_number(p1.y, 0, 1080, -1, 1);
            edges.push_back(Edge(Vector2D(x0, y0), Vector2D(x1, y1)));
        }

        //jcv_delauney_iter delauney;
        //jcv_delauney_begin(&diagram, &delauney);
        //jcv_delauney_edge delauney_edge;
        //while (jcv_delauney_next(&delauney, &delauney_edge))
        //{
        //    jcv_point p0 = remap(&delauney_edge.pos[0], &diagram.min, &diagram.max, &dimensions);
        //    jcv_point p1 = remap(&delauney_edge.pos[1], &diagram.min, &diagram.max, &dimensions);
        //    //draw_line((int)p0.x, (int)p0.y, (int)p1.x, (int)p1.y, image, width, height, 3, color_delauney);
        //}
        //printf("Done.\n"); // rendering

        //jcv_diagram_free(&diagram);
    }

    //// draw the clipping polygon
    //for (int i = 0; i < polygon.num_points; ++i)
    //{
    //    jcv_point p0 = remap(&polygon.points[i], &diagram.min, &diagram.max, &dimensions);
    //    jcv_point p1 = remap(&polygon.points[(i + 1) % polygon.num_points], &diagram.min, &diagram.max, &dimensions);
    //    //draw_line((int)p0.x, (int)p0.y, (int)p1.x, (int)p1.y, image, width, height, 3, color_line);
    //}

    //// Plot the sites
    //for (int i = 0; i < count; ++i)
    //{
    //    if (clipper && !clipper->test_fn(clipper, points[i]))
    //        continue;
    //    jcv_point p = remap(&points[i], &diagram.min, &diagram.max, &dimensions);
    //    //plot((int)p.x, (int)p.y, image, width, height, 3, color_pt);
    //}

    free(clippoints);
    free(points);
    free(rect);

    std::cout << "setPoints size: " << setPoints.size() << std::endl;

    //free(image);

}



