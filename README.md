# html-importer

Note: This repo uses c++17

Gets the job done but with a naive "parser"

## Fixes

- [ ] create a list of void elements
- [ ] check if potential tags match a void element
- [x] continue until '>' is found even on the next line or if another '<' occurs

## TODO not set in stone

- [ ] In include tag, insert json labeled values to target template
  - [ ] Parse json content in include tags
  - [x] Parse and look for "{#x}" in target template
  - [ ] Add values from include tags to desired target template
- [ ] Insert/Replace includes with desired template into a build folder
- [x] Allow multiple json elements in include tag
- [x] Seperate "engine" from set template rules
- [ ] Correctly ignore non-html files
- [ ] Add option to dynamically change dom if id doesn't exists
