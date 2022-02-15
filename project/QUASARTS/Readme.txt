Using Premake to do project management:

To generate .sln(for vs) or makefile(for linux) depends on your own env.

shift + right click -> open powershell -> use the command : ./premake5.exe /*your own vs version*/
for example : ./premake5.exe vs2022
then you get your own sln, open it by vs and build the project

I have changed the .gitignore to ignore the .sln or other things about vs or soemething.

more imformation on : https://premake.github.io/