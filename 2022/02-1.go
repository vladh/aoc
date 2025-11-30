package main

import (
	"bufio"
	"log"
	"os"
)

func getShapeScore(shape string) int {
	if shape == "X" || shape == "A" {
		return 1
	} else if shape == "Y" || shape == "B" {
		return 2
	} else if shape == "Z" || shape == "C" {
		return 3
	}
	return -1
}

func getScoreForResult(theirs string, ours string) int {
	if getShapeScore(theirs) == getShapeScore(ours) {
		return 3
	} else {
		if ((ours == "X" && theirs == "C") || (ours == "Y" && theirs == "A") || (ours == "Z" && theirs == "B")) {
			return 6
		} else {
			return 0
		}
	}
}

func getScoreForRound(theirs string, ours string) int {
	return getShapeScore(ours) + getScoreForResult(theirs, ours)
}

func main() {
	log.SetFlags(0)

	totalScore := 0

	f, err := os.Open("data/02")
	if err != nil { log.Fatal(err) }
	defer f.Close()

	scanner := bufio.NewScanner(f)
	for scanner.Scan() {
		line := scanner.Text()
		theirs := string(line[0])
		ours := string(line[2])
		totalScore += getScoreForRound(theirs, ours)
	}
	if err := scanner.Err(); err != nil {
		log.Fatal(err)
	}

	log.Println(totalScore)
}
