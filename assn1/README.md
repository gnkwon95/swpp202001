## SWPP, Assignment 1: Merging Branches

What you should do is to:

0. Fork this repo & clone the repo into your computer

1. Make a new commit that replaces `aqjune` from `hello.cpp` with your github id,
so it prints:

```
make
./hello
Hello, my name is <your Github ID>
```

The title of the commit should be "Use my github id at hello.cpp".

2. This repo has three branches: `master`, `bugfix`, `usemap`.
Merge the existing `bugfix` branch into `master` using `git merge` command.

3. On top of that, merge the existing `usemap` branch again. This causes a merge conflict;
please fix it carefully.

NOTE: You should carefully see why it causes merge conflict. To do this, you
need to understand what `usemap` branch did.

4. Check that `make; ./check.sh` works successfully. :)

After this, `git log --oneline` at `master` branch should show the commits
including bugfix/usemap commits, "Use my github id at hello.cpp" commit,
and merge commits.

`git log --online --no-merges` should hide the merge commits.

5. Push your master branch to your github repo, and submit your repo link to
https://forms.gle/R8pSyzrXuLhUPs187 .
