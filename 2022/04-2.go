package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
)

type elfRange struct {
	start int
	end int
}

type elfPair struct {
	elf1 elfRange
	elf2 elfRange
}

func isRangeOverlapping(big elfRange, small elfRange) bool {
	return (small.start >= big.start && small.start <= big.end) ||
		(small.end >= big.start && small.end <= big.end)
}

func main() {
	log.SetFlags(0)

	count := 0

	f, err := os.Open("data/04")
	if err != nil { log.Fatal(err) }
	defer f.Close()

	scanner := bufio.NewScanner(f)
	for scanner.Scan() {
		var elves elfPair
		line := scanner.Text()
		fmt.Sscanf(line, "%d-%d,%d-%d",
			&elves.elf1.start, &elves.elf1.end, &elves.elf2.start, &elves.elf2.end)
		log.Printf("%+v\n", elves)
		if isRangeOverlapping(elves.elf1, elves.elf2) || isRangeOverlapping(elves.elf2, elves.elf1) {
			log.Println("contained")
			count += 1
		}
	}
	if err := scanner.Err(); err != nil {
		log.Fatal(err)
	}

	log.Println(count)
}
