use std::io;

fn main() {
    // 행렬의 크기 입력 받기
    let (rows, cols) = input_matrix_dimensions();

    // 첫 번째 행렬 입력 받기
    println!("첫 번째 행렬 입력:");
    let matrix1 = input_matrix(rows, cols);

    // 두 번째 행렬 입력 받기
    println!("두 번째 행렬 입력:");
    let matrix2 = input_matrix(rows, cols);

    // 두 행렬 더하기
    let result = add_matrices(&matrix1, &matrix2, rows, cols);

    // 결과 출력
    println!("두 행렬의 합:");
    print_matrix(&result, rows, cols);
}

// 행렬의 크기를 입력받는 함수
fn input_matrix_dimensions() -> (usize, usize) {
    let mut input = String::new();
    println!("행렬의 행과 열 입력 (예: 2 3):");
    io::stdin().read_line(&mut input).expect("입력 오류");
    let dims: Vec<usize> = input
        .trim()
        .split_whitespace()
        .map(|x| x.parse().expect("유효한 숫자가 아님"))
        .collect();

    if dims.len() != 2 {
        panic!("행과 열의 두 값을 입력해야 합니다.");
    }

    (dims[0], dims[1])
}

// 행렬의 값을 입력받는 함수
fn input_matrix(rows: usize, cols: usize) -> Vec<Vec<i32>> {
    let mut matrix = Vec::with_capacity(rows);

    for i in 0..rows {
        println!("{}번째 행의 값을 입력 (공백으로 구분):", i + 1);
        let mut input = String::new();
        io::stdin().read_line(&mut input).expect("입력 오류");

        let row: Vec<i32> = input
            .trim()
            .split_whitespace()
            .map(|x| x.parse().expect("유효한 숫자가 아님"))
            .collect();

        if row.len() != cols {
            panic!("{}개의 값을 입력해야 합니다.", cols);
        }

        matrix.push(row);
    }

    matrix
}

// 두 행렬을 더하는 함수
fn add_matrices(matrix1: &Vec<Vec<i32>>, matrix2: &Vec<Vec<i32>>, rows: usize, cols: usize) -> Vec<Vec<i32>> {
    let mut result = Vec::with_capacity(rows);

    for i in 0..rows {
        let mut row = Vec::with_capacity(cols);
        for j in 0..cols {
            row.push(matrix1[i][j] + matrix2[i][j]);
        }
        result.push(row);
    }

    result
}

// 행렬을 출력하는 함수
fn print_matrix(matrix: &Vec<Vec<i32>>, rows: usize, cols: usize) {
    for i in 0..rows {
        for j in 0..cols {
            print!("{} ", matrix[i][j]);
        }
        println!();
    }
}

