# Purpose

This script exists to monitor the state of keys for the Cosmofy application

Execute runme.bash

# Required Packages

* Ncurses
* cmake
* gnupg

```bash
 sudo apt-get install libncurses5-dev libncursesw5-dev cmake gnupg
```

# Reccomendation

Make this script available everywhere on your computer. Bellow code makes a "bin" directory in your home directory and modifies .bashrc to include that in the path. This makes it so that you can execute the code from anywhere in your computer. Note that this only works for the regular user. If you want to make it available to all users, it would be better to place it in ``/usr/local``

```bash
mkdir -p ~/bin

echo 'export PATH="$HOME/bin:$PATH"' >> ~/.bashrc

source ~/.bashrc

mv runme.sh ~/bin/
```
