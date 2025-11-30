package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
)

func printStacks(stacks [][]rune) {
	for i := 0; i < len(stacks); i += 1 {
		fmt.Printf("%d | ", i)
		for j := 0; j < len(stacks[i]); j += 1 {
			fmt.Printf("%c ", stacks[i][j])
		}
		fmt.Printf("\n")
	}
}

func doMove(stacks [][]rune, count int, src int, dst int) {
	srclen := len(stacks[src])
	stacks[dst] = append(stacks[dst], stacks[src][srclen - count:srclen]...)
	stacks[src] = stacks[src][:srclen - count]
}

func main() {
	log.SetFlags(0)

	stacks := make([][]rune, 9)
	for i := 0; i < len(stacks); i++ {
		stacks[i] = make([]rune, 0)
	}

	f, err := os.Open("data/05")
	if err != nil { log.Fatal(err) }
	defer f.Close()

	scanner := bufio.NewScanner(f)
	for scanner.Scan() {
		line := scanner.Text()
		if len(line) == 0 {
			break
		}
		if line[1] == '1' {
			continue
		}
		stackIdx := 0
		for i := 1; i < len(line); i += 4 {
			if line[i] != ' ' {
				stacks[stackIdx] = append([]rune{[]rune(line)[i]}, stacks[stackIdx]...)
			}
			stackIdx += 1
		}
	}
	if err := scanner.Err(); err != nil {
		log.Fatal(err)
	}

	printStacks(stacks)

	for scanner.Scan() {
		line := scanner.Text()
		var count, src, dst int
		fmt.Sscanf(line, "move %d from %d to %d", &count, &src, &dst)
		doMove(stacks, count, src - 1, dst - 1)
	}
	if err := scanner.Err(); err != nil {
		log.Fatal(err)
	}

	log.Printf("\n");
	printStacks(stacks)

	for i := 0; i < len(stacks); i += 1 {
		fmt.Printf("%c", stacks[i][len(stacks[i]) - 1])
	}
	fmt.Printf("\n")
}
