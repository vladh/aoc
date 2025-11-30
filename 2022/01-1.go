package main

import (
	"bufio"
	"log"
	"os"
	"strconv"
)

func main() {
	log.SetFlags(0)

	max := 0
	sum := 0

	f, err := os.Open("data/01")
	if err != nil { log.Fatal(err) }
	defer f.Close()

	scanner := bufio.NewScanner(f)
	for scanner.Scan() {
		line := scanner.Text()
		if len(line) > 0 {
			n, err := strconv.Atoi(line)
			if err != nil { log.Fatal(err) }
			sum += n
		} else {
			if sum > max {
				max = sum
			}
			sum = 0
		}
	}
	if err := scanner.Err(); err != nil {
		log.Fatal(err)
	}

	log.Println(max)
}
