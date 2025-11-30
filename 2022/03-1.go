package main

import (
	"bufio"
	"log"
	"os"
)

func getPriorityForItem(item rune) int {
	if item >= 'a' && item <= 'z' {
		return int(item) - int('a') + 1
	} else {
		return int(item) - int('A') + 27
	}
}

func findCommonItem(lhs string, rhs string) rune {
	for i := 0; i < len(lhs); i += 1 {
		for j := 0; j < len(rhs); j += 1 {
			if lhs[i] == rhs[j] {
				return []rune(lhs)[i]
			}
		}
	}
	return '?'
}

func main() {
	log.SetFlags(0)

	prioritySum := 0

	f, err := os.Open("data/03")
	if err != nil { log.Fatal(err) }
	defer f.Close()

	scanner := bufio.NewScanner(f)
	for scanner.Scan() {
		line := scanner.Text()
		lhs := line[:len(line) / 2]
		rhs := line[len(line) / 2:]
		commonItem := findCommonItem(lhs, rhs)
		priority := getPriorityForItem(commonItem)
		prioritySum += priority
		log.Println(lhs, rhs, commonItem, priority)
	}
	if err := scanner.Err(); err != nil {
		log.Fatal(err)
	}

	log.Println(prioritySum)
}
