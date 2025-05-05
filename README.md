# GePECC

In this project, we implement a generator of proof exercises of comparable complexity. More details on how this generator works are described at [add link]. Basically, four input files are required for the procedure to work. As output, the program prints in the terminal the minimal proofs extracted and sets of signed formulas describing proof exercises with comparable level of complexity.

In what follows, we detail the format of these input files and, then, the instructions on how to run the project.

## Input files

A description of the four input files is presented in the following table:

| Input file | File name | What is provided on it
|--|--|--|
| Symbols | `symbols` | Function and predicate of symbols of the signature we use in the working language |
| Skolem symbols | `skolem_symbols` | Skolemization function symbols of our definitional axioms |
| Expansion rules | `expansion_rules` | Set of cut-based expansion rules |
| Signed formulas | `signed_fmlas` | Set of signed formulas describing the input exercise |

All the input files should be inside the `src/inputs` folder. You can find examples at `sets` and `numbers` folders.

The format the input files should have are described below.


### Symbols

The symbols file is constituted by _m_ lines in the following format: 

1st line: function

2nd line: `symbols_line`

...

_n_-th line: `symbols_line`

(_n_+1)-th line: predicate

(_n_+2)-th line: `symbols_line`

...

_m_-th line: `symbols_line`

 
`symbols_line` has the format "`arity`: `list_of_symbols`", where `arity` is a numeral and `list_of_symbols` is a list of symbols of that arity splitted by commas.

### Skolem symbols

The skolem symbols file is a single `symbols_line` file.

### Expansion rules

Before presenting the format of expansion rules file, we define the format of a `signed_formula`. A `signed_formula` is a string "(`o`, `formula`)", where `o` is either a "+" or a "-" and `formula` is a theory-specific formula written with prefixed symbols.

Thus, the lines of expansions rules files have the format "`premisses`; `conclusions`", where both `premises` and `conclusions` are lists of `signed_formulas`'s splitted by commas. Between these lines, we can add comment lines by starting them with an opening square bracket, "[".  

### Signed formulas

The signed formulas file is constituted by `n` lines containing exactly a `signed_formula`.

## Running the program

In the `src` folder, run `make`. Then, run `./main expansion_rules signed_fmlas`.

## Requirements

- C++17
- make