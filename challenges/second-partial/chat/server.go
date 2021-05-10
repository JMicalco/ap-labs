// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 254.
//!+

// Chat is a server that lets clients chat with each other.
package main

import (
	"bufio"
	"fmt"
	"log"
	"net"
	"os"
	"flag"
	"time"
	"strings"
)

//!+broadcaster
type client chan<- string // an outgoing message channel

type user struct {
	name string
	ip string
	chann chan string
	connection net.Conn
	time string
	isOnline bool
	isAdmin bool
}

var (
	entering = make(chan string)
	leaving  = make(chan string)
	messages = make(chan string) // all incoming client messages
	admin = false
	clients = make(map[string]*user)
)


func broadcaster() {
	for {
		select {
		case msg := <-messages:
			// Broadcast incoming message to all
			// clients' outgoing message channels.
			for cli := range clients {
				clients[cli].chann <- msg
			}

		case cli := <-entering:
			//clients[cli] = true
			clients[cli].isOnline = true

		case cli := <-leaving:
			client:=clients[cli]
			//clients[cli].connection.Close()
			close(clients[cli].chann)
			delete(clients, clients[cli].name)
			if client.isAdmin==true && len(clients)>0{
				for cli := range clients {
					clients[cli].chann <- "irc-server > You're the new IRC Server ADMIN"
					clients[cli].isAdmin = true
					fmt.Println("irc-server > [" + clients[cli].name + "] was promoted as the channel ADMIN")
					break
				}
			} else if (client.isAdmin) {
				admin=false
			}


		}
	}
}

//!-broadcaster

//!+handleConn
func handleConn(conn net.Conn) {
	userf := make([]byte, 256)
	name := ""
	ch := make(chan string) // outgoing client messages
	go clientWriter(conn, ch)
	conn.Read(userf);

	for i := 0; i < len(userf); i++ {
		if userf[i] != 0 {
			name = name + string(userf[i])
		} else {
			break
		}
	}

	if _,ok :=clients[name]; ok {
		ch <- "irc-server > Username not available"
		conn.Close()
	} else {
	
		location, _:= time.LoadLocation("America/Mexico_City")

		newUser:= user {
			name: name,
			ip: conn.RemoteAddr().String(),
			chann: ch,
			connection: conn,
			time: time.Now().In(location).Format("2006-01-02 15:04:05"),
			isOnline: true,
		}

		clients[name]= &newUser

		ch <- "irc-server > Your user [" + newUser.name +  "] is successfully logged"
		fmt.Println("irc-server > New connected user ["+newUser.name +"]")
		messages <- "irc-server > New connected user ["+newUser.name +"]"
		entering <- name

		if(admin==false){
			admin=true
			newUser.isAdmin=admin
			fmt.Println(" irc-server > ["+ newUser.name +"] was promoted as the channel ADMIN")
			ch <- "irc-server > Congrats, you were the first user."
			ch <- "irc-server > You're the new IRC Server ADMIN"
		}
	

		
		input := bufio.NewScanner(conn)
		for input.Scan() {
			if clients[name]!=nil {
				subCommand:= strings.Split(input.Text(), " ")
				if subCommand[0] == "/users"{
					for i := range clients {
						ch <- "irc-server > " + clients[i].name  +"- connected since " + clients[i].time
					}
				} else if subCommand[0] == "/msg"{
					if len(subCommand)<3{
						ch <- "irc-server >  incomplete sub-command"
					} else {
						sender:= newUser.name
						reciever:= subCommand[1]
						msg:= strings.Join(subCommand[2:], " ")
						if clients[reciever]!= nil{
							clients[reciever].chann <- sender + " > (Message) "+ msg 
						} else {
							ch <- "irc-server > User not found"
						}
					}
				} else if subCommand[0] == "/time"{
					ch <- "irc-server > Local Time: " + location.String() + " " + time.Now().In(location).Format("15:04")
	
				} else if subCommand[0] == "/user"{
					if len(subCommand)>1 {
						found:=false
						for i:= range clients {
							if subCommand[1] == clients[i].name{
								ch <- "irc-server > username: " + clients[i].name + ", IP: " + clients[i].ip +  " Connected since: "+ clients[i].time
								found=true
								break
							}
						}
						if !found {
							ch <- "irc-server >  User not found"
						}
					} else {
						ch <- "irc-server >  incomplete sub-command"
					}
	
	
				} else if subCommand[0] == "/kick" {
					if len(subCommand)<2 {
						ch <- "irc-server >  incomplete sub-command"
					} else {
						if newUser.isAdmin==true{
							if clients[subCommand[1]] != nil{
								if clients[subCommand[1]] != &newUser{
									clients[subCommand[1]].chann <- "irc-server > You're kicked from this channel"
									leaving <- clients[subCommand[1]].name
									fmt.Println("irc-server > [" + clients[subCommand[1]].name  + "] was kicked")
									messages <- "irc-server > [" + clients[subCommand[1]].name  + "] was kicked from channel"
								} else {
									ch <- "irc-server >  Unable to proceed, cant kick yourself"
								}
							} else {
								ch <- "irc-server >  User not found"
							}
						} else {
							ch <- "irc-server >  Unable to proceed, only the administrator can kick users"
						}
					}

	
				} else {
					messages <- newUser.name + ">" + input.Text()
				}
			}

		}
		// NOTE: ignoring potential errors from input.Err()
		if _,ok:=clients[name]; ok{
			leaving <- newUser.name
			messages <-"irc-server > [" + newUser.name + "] left"
			fmt.Println( "irc-server > [" + newUser.name + "] left")
			conn.Close()
		}

	}
}

func clientWriter(conn net.Conn, ch <-chan string) {
	for msg := range ch {
		fmt.Fprintln(conn, msg) // NOTE: ignoring network errors
	}
}

//!-handleConn

//!+main
func main() {
	
	if len(os.Args) < 5 {
		log.Fatal("Not enough parameters to start IRC Server")

	} else {

		if os.Args[1] == "-host" && os.Args[3] == "-port"{ 
			host := flag.String("host", " ", "Host")
			port := flag.String("port", " ", "port")
			flag.Parse()

			IRCServer := *host + ":" + *port

			listener, err := net.Listen("tcp", IRCServer)
			if err != nil {
				log.Fatal(err)
			}
	
			fmt.Println("irc-server > Simple IRC Server started at",IRCServer)
			fmt.Println("irc-server > Ready for receiving new clients")
		
			go broadcaster()
			for {
				conn, err := listener.Accept()
				if err != nil {
					log.Print(err)
					continue
				}
				go handleConn(conn)
			}

		} else {
			log.Fatal("missing either -host or -port flags")
			
		}

	}

}

//!-main
