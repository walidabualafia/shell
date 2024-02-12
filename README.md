
# shell

This project contains multiple implementations of UNIX shells. The goal of this project is to build a JIT-compiled shell, that allows the user to send complex data structures through the pipeline (as opposed to bash and other popular UNIX shells). 

### Why compiled?

Well, most shells today are interpreters. The infamous bash, with all its limitations, remains king of shells. While most users are satisfied with bash, we wanted to push things a little further. A compiled shell, like Elvish, leverages real-time compiler JIT (just-in-time) to provide the same REPL feel users are used to.

### Why re-invent the wheel?

Bash is great, for most people. When dealing with larger pipelines, bash often runs into IO bottlenecks. Why? Well, resources. But not only that. Users can only pass text through bash pipelines, with no ability to forward complex data structures and streams. Enter shell, a retake on many modern shells, compiling your commands, running them for you, and bringing the terminal a step closer to your heart.




## Installation

Installation is as simple as can be. First step is to clone the repo.

```bash
  git clone <this_repo>
```

Once you have the repo handy, you can compile and run the `C` implementation.

```bash
  cd <this_repo>
  make
  ./shell
```

If you want to run the `Go` implementation, you can either compile it, or test it by running the `Go` interpreter:

```bash
  cd <this_repo>/go 

  # To compile
  go build
  ./shell

  # To run
  go run shell.go
```
    
The shell will also be implemented in `Rust` by March 1st, 2024.
## Roadmap

- Decide on the main language to implement the shell

- Compile list of important commands to reimplement

- Analyze Elvish commands and whether anything can be inherited from Elvish

- Create a clear roadmap section with visual cues


## Contributing

Contributions are always welcome!

See `contributing.md` for ways to get started.

Please adhere to the license policy.


## License

[MIT](https://choosealicense.com/licenses/mit/)

