use std::fs;

fn main() {
  let numbers = fs::read_to_string("data/1")
    .unwrap()
    .split("\n")
    .filter(|line| !line.is_empty())
    .map(|line| line.parse::<i32>().unwrap())
    .collect::<Vec<i32>>();

  for (i, a) in numbers.iter().enumerate() {
    let b_numbers = &numbers[i..];
    for (j, b) in b_numbers.iter().enumerate() {
      let c_numbers = &numbers[j..];
      for c in c_numbers {
        if a + b + c == 2020 {
          println!("{:?}", a * b * c);
          return;
        }
      }
    }
  }
}
