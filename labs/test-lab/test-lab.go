package main

import (
	"fmt"
	"os"
)

func main() {
  args := os.Args[1:]
	fmt.Printf("Hello  %s Welcome to the course \n", args)
}
