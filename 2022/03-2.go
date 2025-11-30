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

func findCommonItem(l1 string, l2 string, l3 string) rune {
	for i := 0; i < len(l1); i += 1 {
		for j := 0; j < len(l2); j += 1 {
			for k := 0; k < len(l3); k += 1 {
				if l1[i] == l2[j] && l2[j] == l3[k] {
					return []rune(l1)[i]
				}
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
		l1 := scanner.Text()
		scanner.Scan()
		l2 := scanner.Text()
		scanner.Scan()
		l3 := scanner.Text()

		commonItem := findCommonItem(l1, l2, l3)
		priority := getPriorityForItem(commonItem)
		prioritySum += priority
		log.Println(commonItem, priority)
	}
	if err := scanner.Err(); err != nil {
		log.Fatal(err)
	}

	log.Println(prioritySum)
}
