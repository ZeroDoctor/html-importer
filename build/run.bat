@echo off

echo Running...

cd Debug
import-html-template.exe %*
cd ..

echo Running - Done!

@echo on
