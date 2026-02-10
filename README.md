# GePECC

Implementation of a method for generation of proof exercises with comparable level of complexity.

## Big picture of the method

The exercises targetted to this method are exercises that can be described by sequents of atomic first-order formulas. For example, an exercise such as “For any sets a, b and c, prove that a ⊆ c given that a ⊆ b and b ⊆ c” is under this scope as it can be described as "a ⊆ b, b ⊆ c ⊢ a ⊆ c".

In few words, the user should pass as the input of the method an exercise and, after executing it, receive a set of execises of comparable complexity to the input.

The formalism employed to search for the proofs is signed cut-based tableaux [1]. So, the exercises are actually described as sets of signed formulas. The sequent "a ⊆ b, b ⊆ c ⊢ a ⊆ c" is represented by the set {+ a ⊆ b, + b ⊆ c, - a ⊆ c}.

## Input files

Besides the exercises, two other inputs are required. A description of the three input files is presented in the following table:

| Input file | File name | What is provided on it
|--|--|--|
| Symbols | `symbols` | Function and predicate symbols of the signature we use in the working language and their respective arities |
| Expansion rules | `expansion_rules` | Set of theory-specific rules |
| Signed formulas | `signed_fmlas` | Set of signed formulas describing the input exercise |

The `symbols` file is placed inside the `src/inputs` folder and its content needs to be set manually or via `cp` command, as we exemplify in what follows. The other two inputs are passed as argument of the command that run the program. You can find examples at `sets` and `numbers` folders.

The format the input files are described below.


### Symbols

The symbols file is constituted by _m + 2_ lines in the following format: 

1st line: function

2nd line: `<symbols_line>`

...

_n_-th line: `<symbols_line>`

(_n_+1)-th line: predicate

(_n_+2)-th line: `<symbols_line>`

...

_m_-th line: `<symbols_line>`

_(m + 1)_-th line: skolem

_(m +2)_-th line: `<list_of_symbols>`

 
`<symbols_line>` has the format "`<arity>`: `<list_of_symbols>`", where `<arity>` is a numeral and `<list_of_symbols>` is a list of symbols of that arity splitted by commas. Backslashes ("\") are not accepted as symbols.

### Expansion rules

Before presenting the format of expansion rules file, we define the format of a `<signed_formula>`. A `<signed_formula>` is a string "(`<o>`, `<formula>`)", where `<o>` is either a "+" or a "-" and `<formula>` is an atomic formula written with prefixed symbols.

Thus, the lines of expansions rules files have the format "`<premises>`; `<conclusions>`", where both `<premises>` and `<conclusions>` are lists of `<signed_formulas>`'s splitted by commas. Between these lines, we can add comment lines by starting them with an opening square bracket, "[".  Moreover `<conclusions>` might be an empty list.

### Signed formulas

The signed formulas file is constituted by `n` lines containing exactly one `<signed_formula>`.

## Output

The program outputs the signed formulas representing the exercises with comparable complexity to the one provided as input. The intermediate step regarding the search for minimal proofs is also outputed.

## Running the program

In the `src` folder, run `make`. Then, run:

- `cp <path_to_symbols_file> inputs/symbols`

- `./main <path_to_expansion_rules> <path_to_signed_fmlas>`

### Example

Inside the `src/input` folder, there may be found two folders, `numbers` and `sets`, with examples of input files. The image below illustrates the output after the execution of the program by running the following commands in the `src` folder: 

- `cp inputs/sets/symbols inputs/symbols`

- `./main inputs/sets/expansion_rules inputs/sets/signed_fmlas/example1`

In the following image, there is the result displayed in the terminal after the execution of an example in `sets` folder:
![Alt text](src/inputs/sets/signed_fmlas/example1_execution.png?raw=true)

## Requirements

- g++ with support to C++17
- make

## Constraints and limitations

We have adopted some constraints to improve the performance of the prototype:
* Cut applications: The only restriction to apply a cut to a tableau T is the existence of a main premise in T. Not restricting the amount of cuts can make the procedure explodes easily. So we postpone the application of the cut to a tableau until no other linear rule can be applied. Moreover, a tableau can only have one cut application in our implementation.
* Tableaux size: The bigger the tableau, the more verifications must be done to apply a rule on it. Because of this, the tableaux in the tree of successor tableaux can have at most 120 nodes.

In this prototype version, there is no mechanism to verify if the input files are written according to the desired format and if the rules provided as input are theory-specific.

## References

[1] M. D’Agostino and M. Mondadori. The taming of the cut. Classical refutations with analytic cut. Journal of Logic and Computation, 4:285–319, 1994.