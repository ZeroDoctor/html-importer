# html-importer

## Fixes

- [ ] create a list of void elements
- [ ] check if potential tags match a void element
- [ ] continue until '>' is found even on the next line or if another '<' occurs

## TODO not set in stone

- [ ] In include tag, insert json labeled values to target template
  - [ ] Parse json content in include tags
  - [ ] Parse and look for "{{x}}" in target template
  - [ ] Add values from include tags to desired target template
- [ ] Insert/Replace includes with desired template into a build folder
- [ ] Allow multiple json elements in include tag
- [ ] Seperate "engine" from set template rules
