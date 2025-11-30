use std::fs;

fn main() {
  let numbers = fs::read_to_string("data/1")
    .unwrap()
    .split("\n")
    .filter(|line| !line.is_empty())
    .map(|line| line.parse::<i32>().unwrap())
    .collect::<Vec<i32>>();

  for (i, a) in numbers.iter().enumerate() {
    let rest = &numbers[i..];
    for b in rest {
      if a + b == 2020 {
        println!("{:?}", a * b);
      }
    }
  }
}
