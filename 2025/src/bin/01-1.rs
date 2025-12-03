use std::fs::File;
use std::io::{self, BufRead};
use std::path::Path;

fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<File>>>
where P: AsRef<Path>, {
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}

fn main() {
    let mut n_zeros = 0;
    let mut pos = 50;
    let lines = read_lines("data/01").expect("could not open file");
    for line in lines.map_while(Result::ok) {
        let mut chars = line.chars();
        let dir = chars.next().expect("did not get direction character");
        let mut amount: i32 = chars.as_str().parse().expect("did not get amount");
        if dir == 'L' {
            amount *= -1;
        }
        pos += amount;
        pos = pos.rem_euclid(100);
        if pos == 0 {
            n_zeros += 1;
        }
        println!("-> {}", pos);
    }
    println!("{}", n_zeros);
}
