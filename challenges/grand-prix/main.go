/*
The race's lap can be static or automatically generated. Racers number can be configured on start.
Define a random max speed for each car. You will emulate the increasing speed behaviour that any
car has. If a car detect another car on his route and it's slower, it must slow down its speed or
try to rebase it. Each racer behaviour will be implemented as a separated thread. Cars threads
must use the same map or city layout data structure resource. Define how many laps before starting
the race. Display each car's speed, position, racing time and lap. At the end, display the top 3
winners.
*/

package main

import (
	"fmt"
	"io/ioutil"
	"log"
	"math/rand"
	"net/url"
	"os"
	"strconv"
	"time"

	// lorca lib for gui, this lib need crome browser
	"github.com/zserge/lorca"
)

// constant for races
const (
	//NumberOfCar   = 5
	BaseSpeed     = 2.0
	UltimateSpeed = 20.0
	//Laps          = 4
)

// message struct for car send out
type Message struct {
	Id       int
	Speed    float64
	Distance float64
	Clock    int64
}

// car function
func car(id int, out chan Message, in chan string) {

	var clock int64
	var distance float64

	// seed for rand function
	rand.Seed(time.Now().UnixNano())

	speed := BaseSpeed
	// random car speed
	maxSpeed := BaseSpeed + rand.Float64()*(UltimateSpeed-BaseSpeed)

	//all car space out about 100 unit distance
	distance = float64(id) * 100
	for {
		// send message to main thread car current stats
		out <- Message{
			Id:       id,
			Speed:    speed,
			Distance: distance,
			Clock:    clock,
		}

		// message comming from main tread
		msg := <-in

		// check message is need to acceleration or deacceleration
		switch msg {
		// recive acceleration command
		case "acc":
			// motion equation: v=u+at where a*t is equal 5
			speed = speed + 5
			if speed > maxSpeed {
				speed = maxSpeed
			}
			distance = distance + speed
		// recive deceleration command
		case "dec":
			// motion equation: v=u-at where a*t is equal 0.5
			speed = speed - 0.5
			if speed < BaseSpeed {
				speed = BaseSpeed
			}
			// motion equation: s=s0+v*t where t=1
			distance = distance + speed
		}

		// increating clock every 1000/20 th of millisecond
		clock++
		time.Sleep(time.Millisecond * 1000 / 20)
	}
}

// It holds all incomming channel for cars
var subs []chan string

// cars distance, speed and clock stats
var distances []float64
var speeds []float64
var clocks []int64

var NumberOfCar int
var Laps int

func main() {
	fmt.Println("Enter the number of car")
	fmt.Scanf("%d", &NumberOfCar)

	fmt.Println("Enter the number of laps")
	fmt.Scanf("%d", &Laps)
	//Laps = 5

	// init GUI 1000*1000 pixel size
	ui, err := lorca.New("", "", 1000, 1000)
	if err != nil {
		log.Fatal(err)
	}
	defer ui.Close()

	// reading html file for gui
	b, err := ioutil.ReadFile("index.html")
	if err != nil {
		log.Fatal(err)
	}

	// loading html to the gui function ui
	ui.Load("data:text/html," + url.PathEscape(string(b)))

	// set laps in gui
	ui.Eval("setLaps(" + strconv.Itoa(Laps) + ")")

	distances = make([]float64, NumberOfCar)
	speeds = make([]float64, NumberOfCar)
	clocks = make([]int64, NumberOfCar)

	// outcomming channel for cars thread
	out := make(chan Message)
	for i := 0; i < NumberOfCar; i++ {
		// create incoimmg channel one by one for cars thread
		in := make(chan string)
		subs = append(subs, in)

		// run car in seprate thread/gorutine
		go car(i, out, subs[i])
	}

	for {

		// selecting channels out or gui ui.Don()  channel
		select {
		case msg := <-out:

			// check car laps
			isLapsComlete := "false"
			distance := msg.Distance
			// 2000 is distance of one laps
			if distance > float64(Laps)*2000 {
				isLapsComlete = "true"
				distance = float64(Laps) * 2000
			}

			// add distance speed clock stats to array
			distances[msg.Id] = msg.Distance
			speeds[msg.Id] = msg.Speed
			clocks[msg.Id] = msg.Clock

			// call udadate function on index html
			// it update all stats of cars distance, speed, clock to the GUI
			go ui.Eval("update(" + fmt.Sprintf("{id:%d,distance:%f,speed:%f,clock:%d,completed:%s}",
				msg.Id, distance, msg.Speed, msg.Clock, isLapsComlete) + ")")

			// Car try always Car try always accelerate
			cmd := "acc"

			// If a car detect another car on his route about(5 unit distance) and it's slower
			// then car deaccelare its speed
			for i := 0; i < NumberOfCar; i++ {
				if i != msg.Id {
					d := distances[i] - msg.Distance
					if d < 5 && msg.Speed > speeds[i] {
						cmd = "dec"
					}
				}
			}

			// Send accelerate or decelerate command to car thread
			subs[msg.Id] <- cmd

		case <-ui.Done():
			// if ui is close it exit program
			os.Exit(0)
		}
	}
}
