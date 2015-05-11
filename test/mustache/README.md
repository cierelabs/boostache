Mustache related tests.

There are three main types of tests
* Parser
* Compiler
* End-to-end

## Parser Tests

Most parser tests are driven by `mustache_parser.cpp`. The input and expect files can be found in the `parser_test_dir` directory.

### Still needed (to-do)

* Failed parsing tests
* Don't support changing of delimeter
* Don't handle html escaping


## Compiler Tests

Compiler tests are driven by `mustache_compiler.cpp`. The input and expect files can be found in the `compiler_test_dir` directory. They test reading input through generation of the internal VM AST.


## End-to-end Tests

End-to-end tests are driven by `mustache_end2end.cpp`. The input and expect files can be found in the `end2end_test_dir` directory. They test reading input through generation of final output.

* What are the whitespace rules?