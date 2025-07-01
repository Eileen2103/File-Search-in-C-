I propose to develop a command-line utility in C++ that recursively scans a given
directory of text files and searches each one for a user-specified literal string or regular
expression. Leveraging std::filesystem for traversal and a configurable thread pool
(std::thread) for concurrency, the tool will report every match with file name, line
number, and column position. Optional features such as case-insensitive mode,
colored output, and summary statistics can be enabled without sacrificing core
performance. Version 4 is the most stable one with the idle rate of the CPU's below %10,and with %100 performance.
