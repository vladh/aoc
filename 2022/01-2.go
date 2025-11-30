package main

import (
	"bufio"
	"log"
	"os"
	"strconv"
)

func main() {
	log.SetFlags(0)

	max := make([]int, 3)
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
			for i := 0; i < 3; i++ {
				if sum > max[i] {
					for j := i + 1; j < 3; j++ {
						max[j] = max[i]
					}
					max[i] = sum
					break
				}
			}
			sum = 0
		}
	}
	if err := scanner.Err(); err != nil {
		log.Fatal(err)
	}

	total := 0
	for _, max_item := range max {
		total += max_item
	}
	log.Println(total)
}
