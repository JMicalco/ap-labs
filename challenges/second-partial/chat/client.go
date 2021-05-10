// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 227.

// Netcat is a simple read/write client for TCP servers.
package main

import (
	"io"
	"log"
	"net"
	"os"
	"flag"
	"fmt"
)

//!+
func main() {

	if len(os.Args) < 5 {
		log.Fatal("Not enough parameters to login as a user")

	} else {

		if os.Args[1] == "-user" && os.Args[3] == "-server"{ 
			userf := flag.String("user", " ", "user")
			server := flag.String("server", " ", "server")
			flag.Parse()
			
			conn, err := net.Dial("tcp", *server)
			fmt.Fprintf(conn, *userf)
			if err != nil {
				log.Fatal(err)
			}
			done := make(chan struct{})
			go func() {
				io.Copy(os.Stdout, conn) // NOTE: ignoring errors
				log.Println("done")
				done <- struct{}{} // signal the main goroutine
			}()
			fmt.Println("irc-server > Welcome to the Simple IRC Server")
			mustCopy(conn, os.Stdin)
			conn.Close()
			<-done // wait for background goroutine to finish


		} else {
			log.Fatal("missing either -user or -server flags")
		}
	}

}

//!-

func mustCopy(dst io.Writer, src io.Reader) {
	if _, err := io.Copy(dst, src); err != nil {
		log.Fatal(err)
	}
}
