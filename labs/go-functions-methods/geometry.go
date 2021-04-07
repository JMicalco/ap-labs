// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 156.

// Package geometry defines simple types for plane geometry.
//!+point
package main

import (
	"math"
	"fmt"
	"math/rand"
	"time"
	"os"
	"strconv"
)

type Point struct{ x, y float64 }

func (p Point) X() float64 {
	return p.x
}

func (p Point) Y() float64 {
	return p.y
}

// traditional function
func Distance(p, q Point) float64 {
	return math.Hypot(q.X()-p.X(), q.Y()-p.Y())
}

// same thing, but as a method of the Point type
func (p Point) Distance(q Point) float64 {
	return math.Hypot(q.X()-p.X(), q.Y()-p.Y())
}

//!-point

//!+path

// A Path is a journey connecting the points with straight lines.
type Path []Point

// Distance returns the distance traveled along the path.
func (path Path) Distance() float64 {
	sum := 0.0
	for i := range path {
		if i > 0 {
			sum += path[i-1].Distance(path[i])
		}
	}
	return sum
}

func random() float64 {
    rand.Seed(time.Now().UnixNano())
    min := -100.0
    max := 100.0
    return (rand.Float64()*max - min + 1.0) + min
}

func  onSegment(p, q, r Point) bool {
	if q.x <= math.Max(p.x, r.x) && q.x >= math.Min(p.x, r.x) && q.y <= math.Max(p.y, r.y) && q.y >= math.Min(p.y, r.y) {
		return true;
	}
	return false;
}

func orientation(p, q, r Point) int{
	val := (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
  
    if val == 0 {
		return 0; 
	}

	if val > 0 {
		return 1
	} else {
		return 2
	}
}

func doIntersect(p1, q1, p2, q2 Point) bool {
	o1 := orientation(p1, q1, p2);
    o2 := orientation(p1, q1, q2);
    o3 := orientation(p2, q2, p1);
    o4 := orientation(p2, q2, q1);

	if o1 != o2 && o3 != o4 {return true}
	if o1 == 0 && onSegment(p1, p2, q1) {return true}
	if o2 == 0 && onSegment(p1, q2, q1) {return true}
	if o3 == 0 && onSegment(p2, p1, q2) {return true}
	if o4 == 0 && onSegment(p2, q1, q2) {return true}

	return false
}


func main() {
	sides, err:=strconv.Atoi(os.Args[1])
	if err != nil {
		fmt.Println(err)
		os.Exit(2)
	}
	if sides<3 {
		fmt.Println("- Not enough points")
	} else {
		path:=make(Path,0)
		perimeter:=0.0
		intersection:=false
		fmt.Printf("- Generating a [%d] sides path\n",sides)
		fmt.Println("- Figures's vertices")
		for {
			path=make(Path,0)
			for i := 0; i < sides; i++ {
				path=append(path, Point{ random(),random()})
			}
			for i:=0; i<sides; i++ {
			  if doIntersect(path[i%sides],path[(i+1)%sides],path[(i+2)%sides],path[(i+3)%sides]) {
				  intersection=true;
				  break;
			  }
			}
			if intersection==true {
				intersection=false
			} else {
				break
			}
		}

		for i:=0; i<sides; i++ {
			fmt.Printf(" - (%.3f, %.3f)\n", path[i].X(), path[i].Y())
		  }
		fmt.Printf("- Figures's Perimeter\n")
		for i := 0; i < sides; i++ {
			tempPerimeter:=path[i].Distance(path[(i+1)%sides])
			perimeter+=tempPerimeter
			fmt.Printf("%.3f + ",tempPerimeter)

		}
		fmt.Printf(" = %.3f\n",perimeter)
	}

}

//!-path
